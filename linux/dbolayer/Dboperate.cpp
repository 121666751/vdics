
#include <string.h>
#include <sstream>
#include <db_common.h>
#include <Dboperate.h>
#include <PubFun.h>
#include "../common/ExportLog.h"

//////////////////////////////////////////////////////////////////////////
// Êý¾Ý¿âÀà

using namespace std;

// -----------------------------------------------------------------------
CDboperate::CDboperate()
{
}

// -----------------------------------------------------------------------
CDboperate::~CDboperate()
{

}

// -----------------------------------------------------------------------
int CDboperate::dbo_open(const char*dbhost,const char*dbuser,const char*dbpswd,const char*dbname)
{	
	return _db.Open(dbhost,dbuser,dbpswd,dbname);
}

int CDboperate::dbo_get_config(KeyMap &vcfg)
{
	int ret = _db.VQuery(512,"select name,value from config");
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	MyRes res = _db.Store();
	MyRow row = res.Fetch();	
	
	while(row!=NULL)
	{
		vcfg[row[0]] = row[1];
		row = res.Fetch();
	}
	
	return 0;
}

int CDboperate::dbo_get_vchtype(KeyMap &vcfg)
{
	//primary key = vchtype + area
	int ret = _db.VQuery(512,"select concat(vchtype,area), banktype from vchtypes");
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	MyRes res = _db.Store();
	MyRow row = res.Fetch();
	
	while(row!=NULL)
	{
		vcfg[row[0]] = row[1];
		row = res.Fetch();
	}
	return 0;
}

int CDboperate::dbo_get_area(KeyMap &vcfg)
{
	int ret = _db.VQuery(512,"select exchno,area from bocnets");
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	MyRes res = _db.Store();
	MyRow row = res.Fetch();
	
	while(row!=NULL)
	{
		vcfg[row[0]] = row[1];
		row = res.Fetch();
	}
	return 0;
}

int CDboperate::dbo_get_exchno(KeyMap &exno)
{
	int ret = _db.Query("select exchno,pkgno from bocnets");
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE,"sql error[%s]",mysql_error(_db.Handle()));
		return ret;
	}
	MyRes res = _db.Store();
	MyRow row = res.Fetch();
	while(row!=NULL)
	{
		exno[row[0]] = row[1];
		row = res.Fetch();
	}
	return 0;
}
int CDboperate::dbo_get_sv_tasks(const char*sid,SCAN_INFO_T& si)
{
     //int ret = _db.VQuery(512, "select v.fname,v.bname,v.pkgno,v.date,v.session,v.ip ,v.rotate,v.dpi, concat('310',right(d.exchno,6),'01',right(v.accno,10)) ,v.vchno,v.vchtype,right(d.exchno,5),d.amount*100,v.imageid,v.sid,v.clkno,v.area,(case when v.modeflag then 'N' else 'O' end) as modeflag from vouchers v,dists d where v.sid=%s and v.mergeid=d.sid", sid);
//     int ret = _db.VQuery(512, "select fname,bname,pkgno,date,session,ip,rotate,dpi,vchno,vchtype,exchno,imageid,sid,clkno,area,(case when modeflag then 'N' else 'O' end) as modeflag from vouchers where sid=%s", sid);
	int ret = _db.VQuery(512, "select fname,bname,pkgno,date,session,ip ,rotate,dpi, concat(right(pkgno,6),'01',right(accno,10)) ,vchno,vchtype,right(exchno,5),amount*100,imageid,sid,clkno,area,(case when modeflag then 'N' else 'O' end) as modeflag from vouchers where sid=%s", sid);
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	MyRes res = _db.Store();
	MyRow row = res.Fetch();
	if(row!=NULL)
	{
		/*
		strncpy(si.fname,row[0],sizeof(si.fname)-1);
		strncpy(si.bname,row[1],sizeof(si.bname)-1);
		strncpy(si.pkgno,row[2],sizeof(si.pkgno)-1);
		strncpy(si.date,row[3],sizeof(si.date)-1);
		strncpy(si.sess,row[4],sizeof(si.sess)-1);
		strncpy(si.ipaddr,row[5],sizeof(si.ipaddr)-1);
		si.rotate = atoi(row[6]);
		si.dpi = atoi(row[7]);
		//strncpy(si.accno,row[8],sizeof(si.accno)-1);
		strncpy(si.vchno,row[8],sizeof(si.vchno)-1);
		strncpy(si.vchtype,row[9],sizeof(si.vchno)-1);
		strncpy(si.exchno,row[10],sizeof(si.exchno)-1);
		//strncpy(si.amount,row[12],sizeof(si.amount)-1);
		strncpy(si.imageid,row[11],sizeof(si.imageid)-1);
		LogPrint(VOUCHER_SEND_NODE, "clkno[%s]", row[13]);
		si.sid = atoi(row[12]);
		strcpy(si.clkno,row[13]);
		strcpy(si.area,row[14]);
		strcpy(si.modeflag,row[15]);

		*/

		strncpy(si.fname,row[0],sizeof(si.fname)-1);
		strncpy(si.bname,row[1],sizeof(si.bname)-1);
		strncpy(si.pkgno,row[2],sizeof(si.pkgno)-1);
		strncpy(si.date,row[3],sizeof(si.date)-1);
		strncpy(si.sess,row[4],sizeof(si.sess)-1);
		strncpy(si.ipaddr,row[5],sizeof(si.ipaddr)-1);
		si.rotate = atoi(row[6]);
		si.dpi = atoi(row[7]);
		strncpy(si.accno,row[8],sizeof(si.accno)-1);
		strncpy(si.vchno,row[9],sizeof(si.vchno)-1);
		strncpy(si.vchtype,row[10],sizeof(si.vchno)-1);
		strncpy(si.exchno,row[11],sizeof(si.exchno)-1);
		strncpy(si.amount,row[12],sizeof(si.amount)-1);
		strncpy(si.imageid,row[13],sizeof(si.imageid)-1);
		si.sid = atoi(row[14]);
		strcpy(si.clkno,row[15]);
		LogPrint(VOUCHER_SEND_NODE, "clkno[%s]", si.clkno);
		strcpy(si.area,row[16]);
		strcpy(si.modeflag,row[17]);
	}	
	else
	{
		LogPrint(VOUCHER_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return -1;
	}

	//get banktype
	ret = _db.VQuery(512, "select banktype from vchtypes where vchtype='%s' and area='%s'", si.vchtype, si.area);
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	res = _db.Store();	
	row = res.Fetch();

	if(row!=NULL && row[0]!=NULL)
		strcpy( si.banktype, row[0]);
	else
		strcpy(si.banktype, "000");

	return 0;
}

int CDboperate::dbo_update_sv_state(const char*sid,int st)
{
	int ret = _db.VQuery(512,"update vouchers set sendstate=%d where sid=%s",st,sid);
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	if(st == 3 || st==4 || st==5)
		_db.VQuery(512,"update vouchers set times=times+1, timestamp=now() where sid=%s",sid);	
	return 0;	
}

int CDboperate::dbo_update_sc_state(const char*sid,int st)
{
	int ret = _db.VQuery(512,"update cinputs set sendstate=%d where sid=%s",st,sid);
	if(ret)
	{
		LogPrint(CINPUT_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	if(st == 3 || st==4 || st==5)
		_db.VQuery(512,"update cinputs set times=times+1, timestamp=now() where sid=%s",sid);	
	return 0;	
}


int CDboperate::dbo_update_sv_state_desc(const char*sid,int st, char *cmdstr, int sendcode)
{
	int ret = _db.VQuery(512,"update vouchers set sendstate=%d, senddesc='%s', sendcode=%d where sid=%s",st, cmdstr, sendcode, sid);
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	if(st == 3 || st==4 || st==5)
		_db.VQuery(512,"update vouchers set times=times+1,timestamp=now() where sid=%s",sid);	
	return 0;	
}

int CDboperate::dbo_update_sc_state_desc(const char*sid,int st, char *cmdstr, int sendcode)
{
	int ret = _db.VQuery(512,"update cinputs set sendstate=%d, senddesc='%s', sendcode=%d where sid=%s",st, cmdstr, sendcode, sid);
	if(ret)
	{
		LogPrint(CINPUT_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	if(st == 3 || st==4 || st==5)
		_db.VQuery(512,"update cinputs set times=times+1,timestamp=now() where sid=%s",sid);	
	return 0;	
}


//update authinput state
int CDboperate::dbo_update_sa_state(const char*sid,int st)
{
	int ret = _db.VQuery(512,"update authinput set sendstate=%d where sid=%s",st,sid);
	if(ret)
	{
		LogPrint( AUTHINPUT_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	if(st == 3 || st==4 || st==5)
		_db.VQuery(512,"update authinput set times=times+1, timestamp=now() where sid=%s",sid);	
	return 0;	
}

int CDboperate::dbo_update_sa_state_desc(const char*sid,int st, char * cmdstr, int sendcode, long elpId)
{
	char senddesc[120];
	code_convert("utf-8", "gb2312", cmdstr, strlen(cmdstr), senddesc, sizeof(senddesc));

	int ret = _db.VQuery(512,"update authinput set sendstate=%d, senddesc='%s', sendcode=%d where sid=%s",st, senddesc, sendcode, sid);
	if(ret)
	{
		LogPrint( AUTHINPUT_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}

	if(st == 3 || st==4 || st==5)
	{
		//fail! update envelop times+=1; and update sid times+=1
		_db.VQuery(512,"update authinput set times=times+1, timestamp=now() where sid=%ld or sid = %s",elpId, sid);	
	}
	//every bill update envelop timestamp
	else
	{
		_db.VQuery(512,"update authinput set timestamp=now() where envid=%ld and mainflag<>0",elpId);	
	}


	return 0;	
}


//update imageid
int CDboperate:: dbo_save_sa_imageid(const char*sid,const char* imageid)
{
	int ret = _db.VQuery(512,"update authinput set imageid='%s' where sid=%s",imageid,sid);
	if(ret)
	{
		LogPrint( AUTHINPUT_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	return 0;		
}


int CDboperate:: dbo_save_sv_imageid(const char*sid,const char* imageid)
{
	int ret = _db.VQuery(512,"update vouchers set imageid='%s' where sid=%s",imageid,sid);
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	return 0;		
}

int CDboperate:: dbo_save_sc_imageid(const char*sid,const char* imageid)
{
	int ret = _db.VQuery(512,"update cinputs set imageid='%s' where sid=%s",imageid,sid);
	if(ret)
	{
		LogPrint(CINPUT_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	return 0;		
}

int CDboperate::dbo_get_sm_tasks(const char*sid,SCAN_INFO_T& si, vector<string>& vc_fname, vector<string>& vc_bname)
{
	//get mainid
	char strsql[1024];
	unsigned long mainid = 0;
	int envid = 0;

	bzero(strsql, sizeof(strsql));
	sprintf(strsql, "select fname,bname,pkgno,date,session,ip ,rotate,dpi,concat(right(pkgno,6),'01',right(accno,10)),vchno,vchtype,right(exchno,5),amount*100,imageid,sid,mainid,envid ,clkno,area,(case when modeflag then 'N' else 'O' end) as modeflag from multis where sid=%ld", atol(sid));

	//LogPrint(MULTIS_SEND_NODE, "strsql->[%s]\n", strsql);

	int ret = _db.Query(strsql);
	if(ret!=0)
	{
		LogPrint(MULTIS_SEND_NODE, "get_sm_tasks 1 error[%s]", strsql);
		LogPrint(MULTIS_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
		return ret;
	}

	MyRes res = _db.Store();
	MyRow row = res.Fetch();

	if(row!=NULL)
	{
			strncpy(si.fname,row[0],sizeof(si.fname)-1);
			strncpy(si.bname,row[1],sizeof(si.bname)-1);
			strncpy(si.pkgno,row[2],sizeof(si.pkgno)-1);
			strncpy(si.date,row[3],sizeof(si.date)-1);
			strncpy(si.sess,row[4],sizeof(si.sess)-1);
			strncpy(si.ipaddr,row[5],sizeof(si.ipaddr)-1);
			si.rotate = atoi(row[6]);
			si.dpi = atoi(row[7]);
			strncpy(si.accno,row[8],sizeof(si.accno)-1);
			strncpy(si.vchno,row[9],sizeof(si.vchno)-1);
			strncpy(si.vchtype,row[10],sizeof(si.vchtype)-1);
			strncpy(si.exchno,row[11],sizeof(si.exchno)-1);
			strncpy(si.amount,row[12],sizeof(si.amount)-1);
			strncpy(si.imageid,row[13],sizeof(si.imageid)-1);
			si.sid = atoi(row[14]);
			
			mainid = atol(row[15]);
			envid = atoi(row[16]);
			
			strcpy(si.clkno,row[17]);
			strcpy(si.area,row[18]);
			strcpy(si.modeflag,row[19]);

		//put main into vector
		vc_fname.push_back(row[0]);
		vc_bname.push_back(row[1]);

		//LogPrint(MULTIS_SEND_NODE, "row[0]->[%s], row[1]->[%s]", row[0], row[1]);
	}
	else
	{
		LogPrint(MULTIS_SEND_NODE, "get_sm_tasks 1 error[%s]", strsql);
		LogPrint(MULTIS_SEND_NODE, "sql error, row is null [%s]", mysql_error(_db.Handle()));
		return -1;
	}

	//get banktype
	ret = _db.VQuery(512, "select banktype from vchtypes where vchtype='%s' and area='%s'", si.vchtype, si.area);
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	res = _db.Store();	
	row = res.Fetch();

	if(row!=NULL && row[0]!=NULL)
		strcpy( si.banktype, row[0]);
	else
		strcpy(si.banktype, "000");

	// put detachment + enviment
	bzero(strsql, sizeof(strsql));

	//sprintf(strsql, "select fname, bname from multis where mainflag<>1 and mainid =%s ", sid);
	//sprintf(strsql, "select fname, bname from multis where mainflag<>1 and envid =%d ", envid);
	sprintf(strsql, "select fname, bname from multis where mainflag<>1 and envid =%d and (mainid=%s or mainid=0)", envid, sid);

	ret = _db.Query(strsql);
	if(ret!=0)
	{
		LogPrint(MULTIS_SEND_NODE, "get_sm_tasks 2 error[%s]", strsql);
		return ret;
	}

	res = _db.Store();
	row = res.Fetch();

	while(row!=NULL)
	{
		vc_fname.push_back(row[0]);
		vc_bname.push_back(row[1]);

		row = res.Fetch();
	}

	return 0;
}

//get authinput detail
int CDboperate::dbo_get_sa_tasks(const char*sid, SCAN_INFO_T& si, vector<string>& va_fname, vector<string>& va_bname, vector<string>& va_imgid)
{
	char strsql[1024];
	unsigned long mainid = 0;
	int envid = 0, ret = -1;
	MyRes res;
	MyRow row;

	bzero(strsql, sizeof(strsql));

	//The first row is env, the other is main
	sprintf(strsql, "select fname,bname,pkgno,date,session,ip ,rotate,dpi,right(accno,10),vchno,vchtype,right(exchno,12),amount*100,imageid,sid,mainid,envid ,clkno,area,multiflag,sendstate from authinput where envid=%d and mainflag<>0 and sendstate=7 or sendstate=0 ) )", atol(sid));

	ret = _db.Query(strsql);
	if(ret!=0)
	{
		LogPrint( AUTHINPUT_SEND_NODE, "get_sa_tasks 1 error[%s]", strsql);
		LogPrint( AUTHINPUT_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}

	res = _db.Store();
	row = res.Fetch();

	while (row!=NULL)
	{
			memset(&si,0, sizeof(si));

			strncpy(si.fname,row[0],sizeof(si.fname)-1);
			strncpy(si.bname,row[1],sizeof(si.bname)-1);
			strncpy(si.pkgno,row[2],sizeof(si.pkgno)-1);
			strcpy(si.date,row[3]);
			strcpy(si.sess,row[4]);
			strcpy(si.ipaddr,row[5]);
			si.rotate = atoi(row[6]);
			si.dpi = atoi(row[7]);
			strncpy(si.accno,row[8],sizeof(si.accno)-1);
			strncpy(si.vchno,row[9],sizeof(si.vchno)-1);
			strncpy(si.vchtype,row[10],sizeof(si.vchtype)-1);
			strncpy(si.exchno,row[11],sizeof(si.exchno)-1);
			strncpy(si.amount,row[12],sizeof(si.amount)-1);
			strncpy(si.imageid,row[13],sizeof(si.imageid)-1);
			si.sid = atoi(row[14]);
			
			mainid = atol(row[15]);
			envid = atoi(row[16]);
			
			strcpy(si.clkno,row[17]);
			strcpy(si.area,row[18]);
			si.multiflag = atoi(row[19]);
			si.sendstate = atoi(row[20]);

			va_fname.push_back(row[0]);
			va_bname.push_back(row[1]);
			va_imgid.push_back(row[13]);

			row = res.Fetch();
	}

	return 0;
}


int CDboperate::dbo_update_sm_state(const char*sid,int st)
{
	int ret = _db.VQuery(512,"update multis set sendstate=%d where sid=%s",st,sid);
	if(ret)
	{
		LogPrint(MULTIS_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	if(st == 3 || st==4 || st==5)
		_db.VQuery(512,"update multis set times=times+1,timestamp =now() where sid=%s",sid);	

	return 0;	
}

int CDboperate::dbo_update_sm_state_desc(const char*sid,int st, char *desc, int sendcode)
{	

	char senddesc[120];
	code_convert("utf-8", "gb2312", desc, strlen(desc), senddesc, sizeof(senddesc));

	int ret = _db.VQuery(512,"update multis set sendstate=%d, senddesc='%s', sendcode=%d where sid=%s",st, senddesc, sendcode, sid);
	if(ret)
	{
		LogPrint(MULTIS_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	if(st == 3 || st==4 || st==5)
		_db.VQuery(512,"update multis set times=times+1, timestamp=now() where sid=%s",sid);	

	return 0;	
}


int CDboperate:: dbo_save_sm_imageid(const char*sid,const char* imageid)
{
	int ret = _db.VQuery(512,"update multis set imageid='%s' where sid=%s",imageid,sid);
	if(ret)
	{
		LogPrint(MULTIS_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}

	return 0;		
}

int CDboperate::dbo_get_sc_tasks(const char*sid,SCAN_INFO_T& si)
{
     int ret = _db.VQuery(512, "select fname,bname,pkgno,date,session,ip ,rotate,dpi, concat('310',right(pkgno,6),'01',right(accno,10)) ,vchno,vchtype,right(exchno,5),amount*100,imageid,sid,clkno,area,(case when modeflag then 'N' else 'O' end) as modeflag from cinputs where sid=%s", sid);
	if(ret)
	{
		LogPrint(CINPUT_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	MyRes res = _db.Store();
	MyRow row = res.Fetch();
	if(row!=NULL)
	{
		strncpy(si.fname,row[0],sizeof(si.fname)-1);
		strncpy(si.bname,row[1],sizeof(si.bname)-1);
		strncpy(si.pkgno,row[2],sizeof(si.pkgno)-1);
		strncpy(si.date,row[3],sizeof(si.date)-1);
		strncpy(si.sess,row[4],sizeof(si.sess)-1);
		strncpy(si.ipaddr,row[5],sizeof(si.ipaddr)-1);
		si.rotate = atoi(row[6]);
		si.dpi = atoi(row[7]);
		strncpy(si.accno,row[8],sizeof(si.accno)-1);
		strncpy(si.vchno,row[9],sizeof(si.vchno)-1);
		strncpy(si.vchtype,row[10],sizeof(si.vchno)-1);
		strncpy(si.exchno,row[11],sizeof(si.exchno)-1);
		strncpy(si.amount,row[12],sizeof(si.amount)-1);
		strncpy(si.imageid,row[13],sizeof(si.imageid)-1);
		si.sid = atoi(row[14]);
		strcpy(si.clkno,row[15]);
		strcpy(si.area,row[16]);
		strcpy(si.modeflag,row[17]);
	}	
	else
	{
		LogPrint(CINPUT_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return -1;
	}

	//get banktype
	ret = _db.VQuery(512, "select banktype from vchtypes where vchtype='%s' and area='%s'", si.vchtype, si.area);
	if(ret)
	{
		LogPrint(CINPUT_SEND_NODE, "sql error[%s]", mysql_error(_db.Handle()));
		return ret;
	}
	res = _db.Store();	
	row = res.Fetch();

	if(row!=NULL && row[0]!=NULL)
		strcpy( si.banktype, row[0]);
	else
		strcpy(si.banktype, "000");

	return 0;
}


