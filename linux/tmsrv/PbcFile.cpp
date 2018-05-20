#include <stdlib.h>
#include <vector>
#include "PbcFile.h"
#include <mem.h>
#include <PubFun.h>
#include "TaskManager.h"
#include <string>
#include <AnsiMemfile.h>
#include <package.h>
#include <sys/types.h>
#include <dirent.h>

#define  MAXLINE 4096

using namespace std;

CTaskManagerServer ct;

//处理人行数据
int proc_pbc_data(const char *pkg_in)
{
	char tradecode[PKG_TRADECODE_LEN+1];
	char fname_qs[512];
	char fname_mx[512];
	int ret = 0;

	//package header
	pbc_header_t h;

	memset(tradecode, '\0', sizeof(tradecode));
	memset(fname_qs, '\0', sizeof(fname_qs));
	memset(fname_mx, '\0', sizeof(fname_mx));
	memset(&h, 0, sizeof(h));

	if(get_pack_tradecode( pkg_in, tradecode)!=0)
	{
		LogPrint(TASK_NODE, "get_pack_tradecode\n");
		LogPrint(TASK_NODE, "Process QS MX fail!\n");
		return -1;
	}

	if(resolve_pbc_pkg( pkg_in, &h)!=0)
	{
		LogPrint(TASK_NODE, "resolve_pbc_pkg\n");
		LogPrint(TASK_NODE, "Process QS MX fail!\n");
		return -1;
	}

	LogPrint(TASK_NODE, "Processing QS MX ...");

	//printf("date->[%s] area->[%s] session->[%s]\n", h.date, h.area, h.session);

	//获取清算文件名
	if(get_pbc_filename( PBC_FILE_TYPE_QS, fname_qs, h.date, h.area, h.session)==NULL)
	{
		LogPrint(TASK_NODE, "get_pbc_filename!\n");
		LogPrint(TASK_NODE, "Process QS MX fail!\n");
		return -1;
	}

	//获取明细文件名
	if(get_pbc_filename( PBC_FILE_TYPE_MX, fname_mx, h.date, h.area, h.session)==NULL)
	{
		LogPrint(TASK_NODE, "get_pbc_filename!\n");
		LogPrint(TASK_NODE, "Process QS MX fail!\n");
		return -1;
	}

	//LogPrint( TASK_NODE, "fname_qs->[%s] fname_mx->[%s]\n", fname_qs, fname_mx);

	//数据导入
	LogPrint( TASK_NODE, "tradecode->[%s] PBC_TRADECODE_EX_REQ->[%s]\n", tradecode, PBC_TRADECODE_EX_REQ);

	if(!strcmp(tradecode, PBC_TRADECODE_IM_REQ))
	{
		LogPrint( TASK_NODE, "Match PBC_TRADECODE_IM_REQ ok!\n");
		return pbc_import( &h, fname_qs, fname_mx);	
	}
	//数据导出
	else if(!strcmp(tradecode, PBC_TRADECODE_EX_REQ))
	{
		LogPrint( TASK_NODE, "Match PBC_TRADECODE_EX_REQ ok!\n");
		return pbc_export( &h, fname_qs, fname_mx);	
	}
	else
	{
		LogPrint(TASK_NODE, "cann't match tradecode!\n");
		LogPrint(TASK_NODE, "Process QS MX fail!\n");
		return -1;
	}

	LogPrint(TASK_NODE, "Process QS MX success!\n");

	return ret;
}

//人行数据导入
int pbc_import( pbc_header_t *h, const char *fname_qs, const char *fname_mx)
{
	int qs_ret = 0, mx_ret = 0;

	if(fname_qs==NULL || fname_mx==NULL)
		return -1;
	
	//导入明细数据
	if(fname_mx!=NULL)
	{
		if((qs_ret = pbc_mx_import( h, fname_mx))!=0)
			LogPrint(TASK_NODE, "pbc_import_1\n");
	}

	//导入清算数据
	if(fname_qs!=NULL)
	{
		if((mx_ret = pbc_qs_import( h, fname_qs))!=0)
			LogPrint(TASK_NODE, "pbc_import_2\n");
	}

	return (qs_ret || mx_ret);	
}

//人行数据导出
int pbc_export(pbc_header_t *h, const char *fname_qs, const char *fname_mx)
{
	int qs_ret = 0, mx_ret = 0;

	if(fname_qs==NULL || fname_mx==NULL)
		return -1;
	
	//导出明细
	if(fname_mx!=NULL)
	{
		LogPrint( TASK_NODE, "Start export mx data...\n");
		qs_ret = pbc_mx_export( h, fname_mx);
		if(qs_ret==0)
		{
			LogPrint( TASK_NODE, "Export mx data success\n");
		}
		else
		{
			LogPrint( TASK_NODE, "Export mx data fail\n");
		}
	}

	//导出清算
	if(fname_qs!=NULL)
	{
		LogPrint( TASK_NODE, "start Export qs data...\n");
		mx_ret = pbc_qs_export( h, fname_qs);
		if(qs_ret==0)
		{
			LogPrint( TASK_NODE, "Export qs data success\n");
		}
		else
		{
			LogPrint( TASK_NODE, "Export qs data fail\n");
		}
	}

	return (qs_ret || mx_ret);	
}

/*人行明细数据导入
@fname: 输入文件名
return：成功返回0 ， 失败返回-1
**/
int pbc_mx_import( pbc_header_t *h, const char *fname_mx)
{
	FILE * fp;
	char line[MAXLINE];
	char subname[strlen(fname_mx)+1];
	int ret;
	
	pbc_mx_t mx;	
	pbc_header_t hd;

	memset(&mx, 0, sizeof(mx));
	memset(&hd, 0, sizeof(hd));
	memset(subname, '\0', sizeof(subname));

	strncpy(subname, fname_mx+2, PBC_HEADER_DATE_LEN + PBC_HEADER_AREA_LEN + PBC_HEADER_SESSION_LEN );
	try
	{
		if((fp = fopen("fname", "r")) == NULL)
		{
			LogPrint(TASK_NODE, "打开清算导入文件出错!'\n");
			return -2;
		}

		//处理包头信息	
		if(fgets(line, sizeof(line)-1, fp) == NULL)
		{
			LogPrint(TASK_NODE, "获取header出错!'\n");
			fclose(fp);
			return -3;
		}

		//文件名与header匹配
		if(strncmp(subname, line, strlen(subname))!=0)
		{
			LogPrint(TASK_NODE, "文件名与包头header匹配不一致!'\n");
			fclose(fp);
			return -4;
		}

		//解析header
		if(spart_pbc_header( line, &hd) == NULL)
		{
			LogPrint(TASK_NODE, "解析包头错误!'\n");
			fclose(fp);
			return -5;
		}
	
		//循环导入信息	
		while(fgets( line, sizeof(line)-1, fp) == NULL)
		{
			memset(line, '\0', sizeof(line));

			//解析包体			
			if( spart_pbc_mx( line, &mx) == NULL )
			{
				LogPrint(TASK_NODE, "解析包体错误!'\n");
				fclose(fp);
				return -6;
			}

			//写入数据库

			if(ct.m_db.PutPbcMXData( &hd, &mx) != 0)
			{
				LogPrint(TASK_NODE, "写入清算数据失败!\n");
				fclose(fp);
				return -7;
			}
		}
	}
	catch(...)
	{
		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;
}

/*人行清算数据导入数据库
@fname: 文件名
**/
int pbc_qs_import( pbc_header_t *h, const char *fname_qs)
{
	FILE * fp;
	char line[MAXLINE];
	char subname[strlen(fname_qs)+1];
	int ret;
	
	pbc_qs_t qs;	
	pbc_header_t hd;

	memset(&qs, 0, sizeof(qs));
	memset(&hd, 0, sizeof(hd));
	memset(subname, '\0', sizeof(subname));

	strncpy(subname, fname_qs+2, PBC_HEADER_DATE_LEN + PBC_HEADER_AREA_LEN + PBC_HEADER_SESSION_LEN );
	try
	{
		if((fp = fopen("fname", "r")) == NULL)
		{
			LogPrint(TASK_NODE, "打开清算导入文件出错!'\n");
			return -2;
		}

		//处理包头信息	
		if(fgets(line, sizeof(line)-1, fp) == NULL)
		{
			LogPrint(TASK_NODE, "获取header出错!'\n");
			fclose(fp);
			return -3;
		}

		//文件名与header匹配
		if(strncmp(subname, line, strlen(subname))!=0)
		{
			LogPrint(TASK_NODE, "文件名与包头header匹配不一致!'\n");
			fclose(fp);
			return -4;
		}

		//解析header
		if(spart_pbc_header( line, &hd) == NULL)
		{
			LogPrint(TASK_NODE, "解析包头错误!'\n");
			fclose(fp);
			return -5;
		}
	
		//循环导入信息	
		while(fgets( line, sizeof(line)-1, fp) == NULL)
		{
			memset(line, '\0', sizeof(line));

			//解析包体			
			if( spart_pbc_qs( line, &qs) == NULL )
			{
				LogPrint(TASK_NODE, "解析包体错误!'\n");
				fclose(fp);
				return -6;
			}

			//写入数据库
			CDBCommon m_db;

			if(m_db.PutPbcQSData( &hd, &qs) != 0)
			{
				LogPrint(TASK_NODE, "写入清算数据失败!\n");
				fclose(fp);
				return -7;
			}
		}
	}
	catch(...)
	{
		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;
}



/*获取文件名
@type: 0-清算文件, 1-明细文件
return: 成功-返回文件名， 失败返回NULL
*/
char *get_pbc_filename(int type, char *fname, const char *date, const char *area, const char *session)
{
	char cfg_name[] = "./config/config.cfg";

    if(ct.InitCfg(cfg_name))
    {   
        LogPrint(TASK_NODE, "InitDBInfo Error!\n");
        return NULL; 
    }	
	
	if(ct.InitDBInfo())
	{
		LogPrint(TASK_NODE, "InitDBInfo Error!\n");
		return NULL;
	}

	DIR *pdir = NULL;
	if((pdir = opendir(ct.pbc_qs_mx_path.c_str()))==NULL)
	{
        LogPrint( TASK_NODE, "打开目录失败! [%s]\n", ct.pbc_qs_mx_path.c_str());
		return NULL;
	}

	if(!type)
		sprintf(fname, "%sQS%s%s%s.txt", ct.pbc_qs_mx_path.c_str(), date, area, session);
	else
		sprintf(fname, "%sMX%s%s%s.txt", ct.pbc_qs_mx_path.c_str(), date, area, session);
	return fname;
}

/*分解清算数据包
@line: 输入数据包
@qs:   返回清算结构体
return: 返回清算结构体
**/
pbc_qs_t * spart_pbc_qs(const char *line, pbc_qs_t *qs)
{
	//分解正文信息
	int count = 0;

	try
	{
		strncpy(qs->exchno, line+count, 12);
		count += 12;

		strncpy(qs->cntype, line+count, 3);
		count += 3;

		strncpy(qs->ccamount, line+count, 15);
		count += 15;
	
		strncpy(qs->dcamount, line+count, 15);
		count += 15;

		strncpy(qs->cramount, line+count, 15);
		count += 15;

		strncpy(qs->dramount, line+count, 15);
		count += 15;

		strncpy(qs->ccsub, line+count, 15);
		count += 15;

		strncpy(qs->ddsub, line+count, 15);
		count += 15;
	}
	catch(...)
	{
		qs = NULL;
	}
		
	return qs;
}

//分解明细数据
pbc_mx_t * spart_pbc_mx(const char *line, pbc_mx_t *mx)
{
	int count = 0;

	try
	{
		strncpy( mx->exchno, line+count, 12);	
		count += 6;
		
		strncpy( mx->cntype, line+count, 3);	
		count += 3;
		
		strncpy( mx->amount, line+count, 15);	
		count += 15;
	
		strncpy( mx->accno, line+count, 21);	
		count += 21;
	
		//8改30
		strncpy( mx->vchno, line+count, 30);	
		count += 30;

		strncpy( mx->vchtype, line+count, 3);	
		count += 3;

		strncpy( mx->vchdate, line+count, 8);	
		count += 8;

		strncpy( mx->payno, line+count, 32);	
		count += 32;

		strncpy( mx->payname, line+count, 60);	
		count += 60;

		strncpy( mx->rcvno, line+count, 32);	
		count += 32;

		strncpy( mx->rcvname, line+count, 60);	
		count += 60;

		strncpy( mx->summary, line+count, 60);	
		count += 60;

		strncpy( mx->cdcode, line+count, 1);	
		count += 1;
	}
	
	catch(...)
	{
		mx = NULL;
	}
		

	return mx;
}

/*解包获取请求信息
@line: 		输入数据包
@h: 		返回header结构体 
return: 	成功返回结构体指针， 失败返回NULL
**/
pbc_header_t * spart_pbc_header(char *line, pbc_header_t *h)
{
	try
	{
		int count = 0;

		//获取日期，交换区域, 交换场次
		strncpy( h->date, line+count, PBC_HEADER_DATE_LEN );
		count += PBC_HEADER_DATE_LEN;
		
		strncpy( h->area, line+count, PBC_HEADER_AREA_LEN );
		count += PBC_HEADER_AREA_LEN;

		strncpy( h->session, line+count, PBC_HEADER_SESSION_LEN );
		count += PBC_HEADER_SESSION_LEN;

		strncpy( h->brno, line+count, PBC_HEADER_BRNO_LEN );
	}
	catch(...)
	{
		h = NULL;
	}

	return h;
}

/*人行清算数据从数据库导出 
**/
int pbc_qs_export( pbc_header_t *h,  const char *fname)
{

	FILE * fp;
	try
	{
		vector<pbc_qs_t> v;
		pbc_qs_t *qs = NULL;
		long ccsub, ddsub;
		StrDict cfg_dic;	

		if((ct.m_db.get_tb_config(cfg_dic))!=0)
		{
			LogPrint(TASK_NODE, "get_tb_config!\n");
			return -1;
		}

		//记录分行号
		strcpy( h->brno, cfg_dic["brno"].c_str());

		if(ct.m_db.GetPbcQSData( h, v)!=0)
		{
			LogPrint( TASK_NODE, "GetPbcQSData!\n");
			return -1;
		}
		
		LogPrint( TASK_NODE, "GetPbcQSData OK!");

		//LogPrint( TASK_NODE, "fanme->[%s]\n", fname);

		if((fp = fopen( fname, "w+")) == NULL)	
		{
           	LogPrint(TASK_NODE, "打开清算导入文件出错!'\n");
           	return -2; 
		}
		
		//write header	
		CAnsiMemFile m_hd;
		m_hd.Write( h->date, 8, strlen(h->date), true, '0');			
		m_hd.Write( h->area, 6, strlen(h->area), true, '0');
		m_hd.Write( h->session, 3, strlen(h->session), true, '0');
		m_hd.Write( h->brno, 6, strlen(h->brno), true, '0');
		m_hd.Write( "\n", 1, 1, true, '0');

		fwrite( (const char *)m_hd.GetPtr(), m_hd.GetLength(), 1, fp);	

		while(v.size() > 0)
		{
			CAnsiMemFile m_cm;
			qs = &(v.at(0));
			strcpy( qs->cntype, PBC_COINTYPE);

			m_cm.Write( qs->exchno, 12, strlen(qs->exchno), true, '0'); 					
			m_cm.Write( qs->cntype, 3, strlen(qs->exchno), true, '0'); 					
			
			//去掉后三位".00"
			qs->ccamount[strlen(qs->ccamount)-3] = '\0';
			m_cm.Write( qs->ccamount, 15, strlen(qs->ccamount), true, '0'); 					
				
			qs->dcamount[strlen(qs->dcamount)-3] = '\0';
			m_cm.Write( qs->dcamount, 15, strlen(qs->dcamount), true, '0'); 					
		
			qs->cramount[strlen(qs->cramount)-3] = '\0';
			m_cm.Write( qs->cramount, 15, strlen(qs->cramount), true, '0'); 					
		
			qs->dramount[strlen(qs->dramount)-3] = '\0';
			m_cm.Write( qs->dramount, 15, strlen(qs->dramount), true, '0'); 					

			//公式 ： dcamount+cramount-(ccamount+dramount);
			//如果这个差额为正，就显示为借方的余额，如果差额为负，显示在贷方余额；
		
			long ce = atol(qs->dcamount)+atol(qs->cramount)-(atol(qs->ccamount)+atol(qs->dramount));

			if (ce > 0)
			{
				sprintf(qs->ccsub, "%ld", abs(ce));
				m_cm.Write( qs->ccsub, 15, strlen(qs->ccsub), true, '0'); 					
				m_cm.Write( "0", 15, 1 , true, '0'); 				
			}
			else
			{
				m_cm.Write( "0", 15, 1 , true, '0'); 					
				sprintf(qs->ddsub, "%ld", abs(ce));
				m_cm.Write( qs->ddsub, 15, strlen(qs->ddsub), true, '0'); 
			}

			m_cm.Write( "\n", 1, 1, true, '0');

			fwrite( (const char *)m_cm.GetPtr(), m_cm.GetLength(), 1, fp);

			v.erase(v.begin());	
		}	
	}
	catch(...)
	{
		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;
}


/*人行明细数据导出
@fname: 文件名
**/
int pbc_mx_export( pbc_header_t *h, const char *fname)
{
	FILE * fp;
	try
	{
		vector<pbc_mx_t> v;
		pbc_mx_t *mx = NULL;
		StrDict cfg_dic;	

		if((ct.m_db.get_tb_config(cfg_dic))!=0)
		{
			LogPrint(TASK_NODE, "get_tb_config!\n");
			return -1;
		}

		//记录分行号
		strcpy( h->brno, cfg_dic["brno"].c_str());

		if(ct.m_db.GetPbcMXData( h, v)!=0)
		{
			LogPrint( TASK_NODE, "GetPbcMXData!\n");
			return -1;
		}
		
		LogPrint( TASK_NODE, "GetPbcMXData OK!");

		//LogPrint( TASK_NODE, "fanme->[%s]\n", fname);

		if((fp = fopen( fname, "w+")) == NULL)	
		{
           	LogPrint(TASK_NODE, "打开清算导入文件出错!'\n");
           	return -2; 
		}
	
		LogPrint( TASK_NODE, "Open %s file ok.\n", fname);

		//write header	
		CAnsiMemFile m_hd;
		m_hd.Write( h->date, 8, strlen(h->date), true, '0');			
		m_hd.Write( h->area, 6, strlen(h->area), true, '0');
		m_hd.Write( h->session, 3, strlen(h->session), true, '0');
		m_hd.Write( h->brno, 6, strlen(h->brno), true, '0');
		m_hd.Write( "\n", 1, 1, true, '0');

		fwrite( (const char *)m_hd.GetPtr(), m_hd.GetLength(), 1, fp);	

		while(v.size() > 0)
		{
			CAnsiMemFile m_cm;
			mx = &(v.at(0));

			LogPrint( TASK_NODE, "mx_exchno->[%s]\tmx_amount->[%s]\tmx_accno->[%s]\tmx_vchno->[%s]\tmx_vchtype->[%s]\n", mx->exchno, mx->amount, mx->accno, mx->vchno, mx->vchtype);

			strcpy( mx->cntype, PBC_COINTYPE);

			m_cm.Write( mx->exchno, 12, strlen(mx->exchno), true, '0'); 					

			m_cm.Write( mx->cntype, 3, strlen(mx->exchno), true, '0'); 					
			

			//去掉后三位".00"
			m_cm.Write( mx->amount, 15, strlen(mx->amount), true, '0'); 					
				
			m_cm.Write( mx->accno, 21, strlen(mx->accno), false, ' '); 					
		
			m_cm.Write( mx->vchno, 30, strlen(mx->vchno), false, '0'); 					

			m_cm.Write( mx->vchtype, 3, strlen(mx->vchtype), true, '0'); 					

			//写入vchdate
			m_cm.Write( " ", 8, 1 , false, ' '); 					
			
			//写入payno
			m_cm.Write( " ", 32, 1 , false, ' '); 					

			//写入payname 
			m_cm.Write( " ", 60, 1, false, ' ');

			//写入rcvaccno 
			m_cm.Write( " ", 32, 1, false, ' ');

			//写入rcvname 
			m_cm.Write( " ", 60, 1, false, ' ');

			//写入summary 
			m_cm.Write( " ", 60, 1, false, ' ');

			//0-D-借方  1-C-贷方
			if(atoi(mx->cdcode))
			{
			
				m_cm.Write( "C", 1, 1, true, '0'); 					
			}
			else
			{
				m_cm.Write( "D", 1, 1, true, '0'); 					
			}

			//写入换行符
			m_cm.Write( "\n", 1, 1, true, '0');

			fwrite( (const char *)m_cm.GetPtr(), m_cm.GetLength(), 1, fp);

			LogPrint( TASK_NODE, "write header ok");

			v.erase(v.begin());	

			LogPrint( TASK_NODE, "write 2 header ok");
		}	
	}
	catch(...)
	{
		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;

}


int resolve_pbc_pkg( const char *pkg_in, pbc_header_t *h)
{
	int count = PKG_TYPE_LEN + PKG_TRADECODE_LEN;

	//日期+交换区域+场次
	try
	{	
		strncpy( h->date, pkg_in+count, PBC_HEADER_DATE_LEN);
		count += PBC_HEADER_DATE_LEN;
		
		strncpy( h->area, pkg_in+count, PBC_HEADER_AREA_LEN);
		count += PBC_HEADER_AREA_LEN;

		strncpy( h->session, pkg_in+count, PBC_HEADER_SESSION_LEN);
		count += PBC_HEADER_SESSION_LEN;
	}
	catch(...)
	{
		return -1;
	}

	return 0;	
}
