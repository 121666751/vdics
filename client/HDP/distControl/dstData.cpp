/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#include "dstData.h"
#include <dstcommon.h>
#include <zcommon.h>
#include <windows.h>
#include <string>
#include <myplus.h>
#include "termsvc.h"

#define HUGEMONER  500000.00


class MyList
{
public:
	DISTDATALIST DataList;
};

dstData::dstData()
{
	pData = new DISTDATA;
	pOcrData = new DISTDATA;
	pList = new MyList;
	TheMyDb = new MyDb;
	clearData();
}

dstData::~dstData()
{
	delete TheMyDb;
	delete pList;
	delete pOcrData;
	delete pData;
}

/**
	初始清分数据链
*/
int dstData::Initialize(int nDstMode, int currency, int netno, int vch_tc, bool isTallyMode)	
{
	int count = 0;
	
	clearData();
	pList->DataList.clear();						    
	
	DstMode = nDstMode;
	Currency = currency;
	m_isTallyMode = isTallyMode;
	m_vchTc = vch_tc;
	

	debugout("初始化");


	char sql[1024];
	memset(sql,0,sizeof(sql));

	
	sprintf(sql,"select vchtype,pockno,pocdesc from pocket where distno=1");

	int ret =TheMyDb->Query(sql);
	
    if(ret)  
	{
		debugout("sql:[%s]\n[%s]\n",mysql_error(TheMyDb->Handle()),sql);
		return ret;
	}

	MyRes res = TheMyDb->Store();
	MyRow row = res.Fetch();
	
	pocMap.clear();
	desMap.clear();
	while(row!=NULL)
	{
		pocMap[row[0]]= row[1];
		desMap[row[0]] = row[2];
		row = res.Fetch();
	}

	memset(sql,0,sizeof(sql));

	
	sprintf(sql,"select vchtype,cdcode from vchtypes where area='110001'");

	ret =TheMyDb->Query(sql);
	
    if(ret)  
	{
		debugout("sql:[%s]\n[%s]\n",mysql_error(TheMyDb->Handle()),sql);
		return ret;
	}
	
	res = TheMyDb->Store();
	row = res.Fetch();
	

	cdMap.clear();
	while(row!=NULL)
	{
		cdMap[row[0]]= row[1];
		row = res.Fetch();
	}



	vpMap.clear();

	sprintf(sql,"select right(accno,14),exchno from vips ");

	ret =TheMyDb->Query(sql);
	
    if(ret)  
	{
		debugout("sql:[%s]\n[%s]\n",mysql_error(TheMyDb->Handle()),sql);
		return ret;
	}
	
	res = TheMyDb->Store();
	row = res.Fetch();
	
	while(row!=NULL)
	{
		vpMap[row[0]]= row[1];
		row = res.Fetch();
	}
	return count;
}


void dstData::clearData()
{
	memset( pData, 0, sizeof( DISTDATA ));
	memset( pOcrData, 0, sizeof( DISTDATA ));
}

/**
	保存结果
*/
int dstData::StoreState(int mode)
{
	int ret = SaveResult(mode);	
	return ret;
}

/**
	连接数据库
*/
int dstData::OpenDb()
{
	if( TheMyDb->Open("ZTIC_DB_HOST","gsip","gsip0728","gsipdb") ) 
	{
		debugout("open database error [%s]\n",mysql_error(TheMyDb->Handle()));
		zreturn(DB_OPEN_FAIL);
	}
	return 0;
}

bool dstData::SetCodeData(char *codeline,bool micr)
{
	return true;
}

/**
	返回口袋号
*/
int dstData::GetPocketNum(int mode)
{
	//获取口袋号
	int pocket = getPocEx(mode);
	
	
	return pocket;
}

/**
	返回口袋号
*/
int dstData::getPocEx(int mode)
{
	
	if( !MicrOK ) return -1;
	
	char cMode = ( mode == MD_FIRST ) ? 'A':'C';
	int ret=0;
	int pocket = 0;
	MyRes res;
	MyRow row;

	
	
	//二次清分
	if( mode == MD_SECOND ) 
	{
		char cmd[1024];
		char state[13];	
		string tmp;
		int  count=0;	
		int  preres=-1;
		int  tallyres=-1;
		int  mergeres=-1;
		int  ctrlres=-1;
		int  manidres=-1;
		double money;
		
		
		memset(cmd,0,sizeof(cmd));
		memset(state,0,sizeof(state));
		debugout("accno=[%s].\n",pData->codeacc);
		
		pocket = 1;

		sprintf( pData->VchKindCod, "%c%02s",cMode,pData->vchtype );
		return pocket;
	}


		
	else if(mode == MD_FIRST )	//一次清分
	{
		if( atoi(pData->vchtype) < 0 ) zreturn(0);	 


		string ps;

		ps = vpMap[pData->codeacc];

		if(strcmp((char*)ps.c_str(),pData->exchno)==0)
		{
			ps = pocMap["VP"];
			strcpy(pData->VchKindTxt, desMap[pData->vchtype].c_str() );			
			strcpy(pData->PockText, "重点客户" );		
			
			if(ps.size()<=0) zreturn(0);
				
			pocket = stoi(ps);
		}
		
		else
		{
			ps = pocMap[pData->vchtype];
			strcpy(pData->VchKindTxt, desMap[pData->vchtype].c_str() );			
			strcpy(pData->PockText, desMap[pData->vchtype].c_str() );

			if(ps.size()<=0) zreturn(0);

			pocket = stoi(ps);

			//放在信封里的支票
			if( atoi(pData->vchtype) == 0 && pData->isEnvelop == 1)
			{
				pocket = pocket +1;
			}


			if( atoi(pData->vchtype) >=50 && pData->isEnvelop == 0)
			{
				ps = pocMap["JZD"];
				strcpy(pData->VchKindTxt, desMap[pData->vchtype].c_str() );			
				strcpy(pData->PockText, "进账单" );		
			
				if(ps.size()<=0) zreturn(0);
				
				pocket = stoi(ps);
			}

		}



		sprintf( pData->VchKindCod, "%c%02s",cMode,pData->vchtype);

		return pocket;
	}
	else return 0;
}


bool dstData::setCode(DISTDATA *mdata,DISTDATA *odata)
{
	return true;
}

bool dstData::chkMicrCode()           
{
	return true;
}

void dstData::GetVchKindNum(DISTDATA *data)    ///////////字符型转变为整型
{

}

/**
	存储数据
*/
void dstData::StoreList(HWND hWnd)
{
	
	if( DstMode == MD_FIRST || DstMode == MD_THIRD )
	{
		pList->DataList.push_back( *pData );
		bReDist = false;		
	}
}

/**
	保存结果
*/
int dstData::SaveResult(int mode)
{
	debugout("mode =[%d]\n",mode);
	int ret =0 ;
	if (mode == MD_FIRST)  ret = SaveFirstResult(); 
	if (mode == MD_SECOND) ret = SaveSecondResult();
	if (mode == MD_THIRD)  ret = SaveThirdResult();
	return ret;
}

int dstData::GetIDResult()
{
	return 0;
}

/**
	保存一次清分结果
*/
int dstData::SaveFirstResult()
{
	DISTDATALIST::iterator dist_cur = pList->DataList.begin();
	int ret = 0;
	
	if( dist_cur == pList->DataList.end() ) return 0; // no data to store!
	
	while( dist_cur != pList->DataList.end())
	{
		
		DISTDATA ddd = *dist_cur;
		
		ret = StoreData( &ddd );
		if(ret) return ret;
		
		++dist_cur;
	}	
	
	return 0;
}

int dstData::SaveSecondResult()
{
	return 0;
}

/**
	得到链表所有数据
*/
int dstData::GetRightItems(DISTDATA *pData, int *count)
{
	DISTDATALIST::iterator dist_cur = pList->DataList.end();
	int i=0;
	
	while( dist_cur != pList->DataList.begin() ) 
	{
		--dist_cur;
		pData[i] = *dist_cur;
		i++;
	}
	
	*count = i;	
	return 0;
}

/**
	删除倒数第几个记录
*/
int dstData::DelRightItems(int rightIndex)
{
	DISTDATALIST::iterator dist_cur = pList->DataList.end();
	int i=0;
	
	while( dist_cur != pList->DataList.begin() && i<=rightIndex ) 
	{
		--dist_cur;
		if(i==rightIndex) 
		{
			pList->DataList.erase(dist_cur);
			break;
		}
		i++;
	}
	
	return (i==rightIndex);	
}

/**
	删除当前项
*/
int dstData::DeleteCurrent()
{
	DISTDATALIST::iterator dist_cur = pList->DataList.end();
	pList->DataList.erase(dist_cur);
	return 0;
}

/**
	计算总额
*/
int dstData::CalTotalAmount(double &dramount, int& drcount, double &cramount, int& crcount)
{
	DISTDATALIST::iterator dist_cur = pList->DataList.begin();
	
	dramount = cramount = 0;
	drcount = crcount = 0;
	
	while( dist_cur != pList->DataList.end() ) 
	{

		//借方
		if(dist_cur->cdcode == 0)
		{
			cramount += atof( dist_cur->amount )/100.0;
			crcount++;
		}
		//贷方
		else
		{
			dramount += atof( dist_cur->amount )/100.0;
			drcount++;
		}
		++dist_cur;
	}
	
	return 0;
}

/**
	保存数据
*/
int dstData::StoreData(DISTDATA *data)
{
	char sql[1024];
	memset(sql,0,sizeof(sql));

	string nip;
	nip = get_terminal_ip();


	//保存到一次清分表
	sprintf(sql,"insert into dists(accno,vchno,vchtype,amount,exchno,date,session,fname,bname,pkgno,ip, isenv, scantime,modified,cdcode) \
			values('%s','%s','%s',%.2f,'%s','%s','%s','%s','%s','%s','%s',%d, now(),%d,%d)",
			data->codeacc,data->vchno,data->vchtype,atof(data->amount)/100,data->exchno,
			data->date, data->session,data->fname,data->rname,data->exchno,nip.c_str() ,data->isEnvelop,data->modified,data->cdcode);
	
	int rec =TheMyDb->Query(sql);
	
    if(rec) 
	{
		debugout("sql:[%s]\n[%s]\n",mysql_error(TheMyDb->Handle()),sql);
		return rec;
	}
	
	
	return 0;
}

/**
	三次清分保存数据
*/
int dstData::SaveThirdResult()
{
	
    debugout("in save third result \n");
	DISTDATALIST::iterator dist_cur = pList->DataList.begin();
	int ret = 0;
	
	if( dist_cur == pList->DataList.end() ) 
	{
		debugout("no data need to be stored \n");
		return 0; // no data to store!
	}
	
	while( dist_cur != pList->DataList.end())
	{		
		DISTDATA ddd = *dist_cur;		
		ret = StoreData_Third( &ddd );		
		++dist_cur;
	}	
	
	return 0;	
}

/**
	保存三次清分数据
*/
int dstData::StoreData_Third(DISTDATA *data)
{
	return 0;
}
