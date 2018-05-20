#include "db_common.h"
#include "tb_vouchers.h"
#include "tb_multis.h"
#include <PubFun.h>

#define MAXLINE 1024

using namespace std;


CDBCommon::CDBCommon()
{
}

/*
关闭数据库连接
*/
CDBCommon::~CDBCommon()
{
	DbClose();
}

/*重新打开数据库
**/
int CDBCommon::DbReOpen()
{
	DbClose();

	return DbOpen(host, uid, pwd, dbAlias);
}

/*打开数据库
@M_host: 据库服务器名
@m_uid:  数据库用户名
@m_pwd:  数据库密码
@m_dbAlias: 数据库实例名
返回：成功0, 失败返回非0
*/
int CDBCommon::DbOpen(const char *m_host,const char *m_uid,const char *m_pwd,const char *m_dbAlias)
{
	strcpy(host, m_host);
	strcpy(uid, m_uid);
	strcpy(pwd, m_pwd);
	strcpy(dbAlias, m_dbAlias);

	return db.Open(host, uid, pwd, dbAlias);	
}

/*关闭数据库
**/
void CDBCommon::DbClose()
{
	db.Close();
}

/*返回单票识别任务列表
@vc_ret : 返回的id列表结合
@lim_num:每次查询获得任务的最大数量
@date: 交换日期
@session: 交换场次
return: 成功返回0，失败返回非-1
*/
int CDBCommon::GetVouchersSIDSList(vector<unsigned long> &vc_set, int lim_num, int date, int session, const char *brno)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	UpdateParseTimeOut("vouchers");

	memset(str_sql, '\0', sizeof(str_sql));

	/*北京分行上传校验场次, 其他行不校验场次*/
	if(!strcmp(brno, "110999"))
	{
		sprintf(str_sql, "select sid from vouchers where date=%d and session=%d and (state = 0 or (state = 3 and times< 10 and (now()-timestamp>120) )) order by times limit %d", date, session, lim_num);
	}
	else
	{
		sprintf(str_sql, "select sid from vouchers where date=%d and (state = 0 or (state = 3 and times< 10 and (now()-timestamp>120) )) order by times limit %d", date, lim_num); 
	}

	try
	{
		if(ret = db.Query(str_sql))
		{
			LogPrint( TASK_NODE, "str_sql->[%s]", str_sql);
			LogPrint( TASK_NODE, "GetVouchersSIDSList error[%s]", mysql_error(db.Handle()));

			return -1;
		}

		MyRes res = db.Store();

		MyRow row = res.Fetch();
	
		while(row != NULL)
		{
			vc_set.push_back(atol(row[0]));
	
			row = res.Fetch();
		}
	}
	catch(...)
	{
			LogPrint( TASK_NODE, "Catch GetVouchersSIDSList error[%s]", mysql_error(db.Handle()));
			LogPrint( TASK_NODE, "str_sql->[%s]", str_sql);

	 		return -1;
	}

	return 0;
}

/*返回单票上传任务列表
@vc_ret : 返回的id列表结合
@lim_num:每次查询获得任务的最大数量
@date: 交换日期
＠session: 交换场次
return: 成功返回0，失败返回非-1
*/

int  CDBCommon::GetVouchersSendList(vector<unsigned long> &vcu_set, int lim_num, int date, int session, const char *brno)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	memset(str_sql, '\0', sizeof(str_sql));

	UpdateSendTimeOut("vouchers");

	//sendstate=5 上传ECM失败
	if(!strcmp(brno, "110999"))
	{
		sprintf(str_sql, "select sid from vouchers where  date=%d and session=%d and ((state = 1 and mergestate = 1) and (sendstate = 0 or((sendstate = 3 or sendstate=4) and times<100))  and (now()-timestamp) >60) order by times limit %d", date, session, lim_num);
	}
	else
	{
		sprintf(str_sql, "select sid from vouchers where  date=%d and ((state = 1 and mergestate = 1) and (sendstate = 0 or((sendstate = 3 or sendstate=4) and times<100)) and (now()-timestamp) >60) order by times limit %d", date, lim_num);
	}

	try
	{
		if(ret = db.Query(str_sql))
		{
			LogPrint( TASK_NODE, "GetVouchersSendList error[%s]", mysql_error(db.Handle()));
			LogPrint( TASK_NODE, "str_sql->[%s]", str_sql);

			return -1;
		}

		MyRes res = db.Store();

		MyRow row = res.Fetch();
	
		while(row != NULL)
		{
			vcu_set.push_back(atol(row[0]));
	
			row = res.Fetch();
		}
	}
	catch(...)
	{
		LogPrint( TASK_NODE, "Catch GetVouchersSendList error[%s]", mysql_error(db.Handle()));
		LogPrint( TASK_NODE, "str_sql->[%s]", str_sql);

		return -1;
	}

	return 0;
}


//获取信封识别任务列表
int CDBCommon::GetMultisParseList(vector<unsigned long> &mt_set, int lim_num, int date, int session, const char *brno)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	bzero(str_sql, sizeof(str_sql));
	UpdateParseTimeOut("multis");

	if(!strcmp(brno, "110999"))
	{
		sprintf(str_sql, "select sid from multis where date=%d and session=%d and (state = 0 or (state=3 and times<10 and (now()-timestamp>120))) order by times limit %d", date, session, lim_num);
	}
	else
	{
		sprintf(str_sql, "select sid from multis where date=%d and (state = 0 or (state=3 and times<10 and (now()-timestamp>120))) order by times limit %d", date, lim_num);
	}

	try
	{
		if(ret = db.Query(str_sql))
		{
			LogPrint( TASK_NODE, "GetMultisParseList error[%s]", mysql_error(db.Handle()));
			LogPrint( TASK_NODE, "str_sql->[%s]", str_sql);
			
			return -1;
		}

		MyRes res = db.Store();

		MyRow row = res.Fetch();
	
		while(row != NULL)
		{
			mt_set.push_back(atol(row[0]));
	
			row = res.Fetch();
		}
	}
	catch(...)
	{
		LogPrint( TASK_NODE, "Catch GetMultisParseList error[%s]", mysql_error(db.Handle()));
		LogPrint( TASK_NODE, "str_sql->[%s]", str_sql);

		return -1;
	}

	return 0;
}

//获取信封上传任务列表
int CDBCommon::GetMultisSendList(vector<unsigned long> &mtu_set, int lim_num, int date, int session, const char *brno)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	bzero(str_sql, sizeof(str_sql));

	UpdateSendTimeOut("multis");

	if(!strcmp(brno, "110999"))
	{
		sprintf(str_sql, "select sid from multis where  date=%d and session=%d and (substate=1 and mainflag = 1 and (sendstate=0 or ( (sendstate=3 or sendstate=4) and times<100)) and (now()-timestamp) >60) order by times limit %d", date, session, lim_num);
	}
	else
	{
		sprintf(str_sql, "select sid from multis where  date=%d and (substate=1 and mainflag = 1 and (sendstate=0 or ( (sendstate=3 or sendstate=4) and times<100)) and (now()-timestamp) >60) order by times limit %d", date, lim_num);
	}

	try
	{
		if(ret = db.Query(str_sql))
		{
			LogPrint( TASK_NODE, "GetMultisSendList error[%s] sql->[%s]", str_sql,  mysql_error(db.Handle()));

			return -1;
		}

		MyRes res = db.Store();

		MyRow row = res.Fetch();
	
		while(row != NULL)
		{
			mtu_set.push_back(atol(row[0]));
	
			row = res.Fetch();
		}
	}
	catch(...)
	{
		LogPrint( TASK_NODE, "Catch GetMultisSendList error[%s] sql->[%s]", str_sql,  mysql_error(db.Handle()));

		return -1;
	}

	return 0;
}

//获取委收识别任务列表
int CDBCommon::GetAuthInputParseList(vector<unsigned long> &ai_set, int lim_num, int date , int session)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	bzero(str_sql, sizeof(str_sql));
	UpdateParseTimeOut("authinput");

		sprintf(str_sql, "select sid from authinput where date=%d and session=%d and (state = 0 or (state=3 and times<10 and (now()-timestamp>120))) order by times limit %d", date, session, lim_num);

	try
	{
		if(ret = db.Query(str_sql))
		{
			LogPrint( AUTHINPUT_NODE, "GetAuthInputParseList error[%s]", mysql_error(db.Handle()));
			LogPrint( AUTHINPUT_NODE, "str_sql->[%s]", str_sql);
			
			return -1;
		}

		MyRes res = db.Store();

		MyRow row = res.Fetch();
	
		while(row != NULL)
		{
			ai_set.push_back(atol(row[0]));
	
			row = res.Fetch();
		}
	}
	catch(...)
	{
		LogPrint( AUTHINPUT_NODE, "Catch GetAuthInputParseList error[%s]", mysql_error(db.Handle()));
		LogPrint( AUTHINPUT_NODE, "str_sql->[%s]", str_sql);

		return -1;
	}

	return 0;
}

//获取委收上传任务列表
int CDBCommon::GetAuthInputSendList(vector<unsigned long> &aiu_set, int lim_num, int date, int session)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	bzero(str_sql, sizeof(str_sql));

	UpdateSendTimeOut("authinput");

	//sprintf(str_sql, "select sid from authinput where date=%d and session=%d and (checkstate=1 and mainflag = 1 and state=1 and (sendstate=0 or ( (sendstate=3 or sendstate=4) and times<100)) and (now()-timestamp) >60) order by times limit %d", date, session, lim_num);

		sprintf(str_sql, "select sid from authinput where date=%d and session=%d and times<100 and (checkstate=1 and mainflag = -1 and state=1 and (sendstate=0 or sendstate=6) ) and (now()-timestamp) >60 order by times limit %d", date, session, lim_num);

	try
	{
		if(ret = db.Query(str_sql))
		{
			LogPrint( TASK_NODE, "GetAuthInputSendList error[%s] sql->[%s]", str_sql,  mysql_error(db.Handle()));

			return -1;
		}

		MyRes res = db.Store();

		MyRow row = res.Fetch();

		while(row != NULL)
		{
			aiu_set.push_back(atol(row[0]));
	
			row = res.Fetch();
		}
	}
	catch(...)
	{
		LogPrint( TASK_NODE, "Catch GetAuthInputSendList error[%s] sql->[%s]", str_sql,  mysql_error(db.Handle()));

		return -1;
	}

	return 0;
}



//更新单票识别超时任务
int CDBCommon::UpdateParseTimeOut( char *tb_name)
{
	int ret;
	char strsql[512];
	bzero(strsql, sizeof(strsql));

	sprintf( strsql, "update %s set state = 0, times = times+1, timestamp=now()  where state=2 and (now()-timestamp) >120", tb_name);

	//LogPrint( VOUCHER_NODE, " UpdateParseTimeout strsql->[%s]\n", strsql);

	if( ret=db.Query(strsql))
	{
		LogPrint(TASK_NODE, "UpdateParseTimeOut error[%s]\nstrsql->[%s]\n", mysql_error(db.Handle()), strsql);

		return -1;
	}

	return 0;
}


//更新信封识别超时任务
int CDBCommon::UpdateSendTimeOut( char *tb_name)
{
	//更新上传ECM失败的状态
	if(db.VQuery(512, "update %s set sendstate = 0, times = times+1, timestamp=now()  where sendstate=2 and ((now()-timestamp) >120) and length(imageid)<8",tb_name))
	{
		LogPrint(TASK_NODE, "UpdateSendTimeOut error[%s][%s]", mysql_error(db.Handle()), tb_name);

		return -1;
	}

	//更新上传图像成功，但触发流程失败的状态sendstate=4
	if(db.VQuery(512, "update %s set sendstate = 5, times = times+1  where sendstate=2 and ((now()-timestamp) >120) and length(imageid)>8",tb_name))
	{
		LogPrint(TASK_NODE, "UpdateSendTimeOut error[%s][%s]", mysql_error(db.Handle()), tb_name);

		return -1;
	}

	//新增回收特色业务
	if(!strcmp(tb_name,"authinput"))
	{
		if(db.VQuery(512, "update %s set sendstate = 6, times = times+1  where sendstate=7 and ((now()-timestamp) >120)",tb_name))
		{
			LogPrint(TASK_NODE, "UpdateSendTimeOut error[%s][%s]", mysql_error(db.Handle()), tb_name);

			return -1;
		}
	}

	return 0;
}


//根据id获取单票信息, 返回信息封装到结构体v中
int CDBCommon::GetVouchersRowBySid(const char *sid, struct tb_vouchers_s *v)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	try
	{
		//更新时间戳
		memset(str_sql, '\0', sizeof(str_sql));
		sprintf(str_sql, "update vouchers set timestamp=now() where sid=%ld and state=2", atol(sid));

		if(ret = db.Query(str_sql))
		{
			LogPrint( VOUCHER_NODE, "GetVouchersSIDSList error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
			return -1;
		}

		//获取明细
		memset(str_sql, '\0', sizeof(str_sql));
		sprintf(str_sql, "SELECT distinct sid,fname,bname,rotate,ip,dpi,date, right(exchno,5) ,session,state,vchno,accno,vchtype,amount,model,mergestate,mergeid,sendstate,imageid,timestamp,submitstate,scantime,prestate,clkno,times,right(pkgno, 5), area,(case when modeflag then 'N' else '0' end) as modeflag  FROM vouchers WHERE sid = %d ", atoi(sid));

		if(ret = db.Query(str_sql))
		{
			
			LogPrint(TASK_NODE, "GetVouchersSIDSList error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
			return -1;
		}
		MyRes res = db.Store();

		if(res.Count()==0)
		return SQL_NO_RESULT;

		MyRow row = res.Fetch();
		v->sid = atol(row[0]);
		strcpy(v->fname, row[1]);
		strcpy(v->bname, row[2]);
		v->rotate = atoi(row[3]);
		strcpy(v->ip, row[4]);
		v->dpi = atoi(row[5]);

		strcpy(v->date, row[6]);
		strcpy(v->exchno, row[7]);
		v->session = atoi( row[8]);
		v->state = atoi(row[9]);
		strcpy(v->vchno, row[10]);
		strcpy(v->accno, row[11]);
		strcpy(v->vchtype, row[12]);
		strcpy(v->amount, row[13]);	

		v->model = atoi( row[14]);
		v->mergestate = atoi(row[15]);
		v->mergeid = atol(row[16]);
		v->sendstate = atoi(row[17]);
		strcpy(v->imageid, row[18]);

		//strcpy(v->timestamp, row[19]);
		v->submitstate = atoi(row[20]);
		//strcpy(v->scantime, row[21]);
		v->prestate = atoi(row[22]);
		strcpy(v->clkno, row[23]);
		v->times = atoi(row[24]);
		strcpy(v->pkgno, row[25]);
		strcpy(v->area, row[26]);
		strcpy(v->modeflag, row[27]);
	}
	catch(...)
	{
		
		LogPrint( VOUCHER_NODE, "GetVouchersSIDSList error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}
	

	return 0;
}

//根据id获取信封信息, 返回信息封装到结构体v中
int CDBCommon::GetMultisRowBySid(const char *sid, struct tb_multis_s *v)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	try
	{
		//更新时间戳
		memset(str_sql, '\0', sizeof(str_sql));

		sprintf(str_sql, "update multis set  timestamp=now() where sid=%ld and state=2", atol(sid));

		if(ret = db.Query(str_sql))
		{
			LogPrint( MULTIS_NODE, "GetMultisIDSList error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		memset(str_sql, '\0', sizeof(str_sql));
		sprintf(str_sql, "SELECT distinct sid,fname,bname,rotate,ip,dpi,date, right(exchno,5) ,session, right(pkgno,5),vchno,accno,vchtype, amount, multiflag, state, mergestate, mergeid, model, mainid, envid, mainflag, checkstate, prestate, sendstate, imageid, timestamp, substate, scantime, clkno, times, area,(case when modeflag then 'N' else '0' end) as modeflag  FROM multis WHERE sid = %d ", atoi(sid));

		if(ret = db.Query(str_sql))
		{
			LogPrint( MULTIS_NODE, "GetMultisIDSList error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		MyRes res = db.Store();

		if(res.Count()==0)
			return SQL_NO_RESULT;

		MyRow row = res.Fetch();

		v->sid = atol(row[0]);
		strcpy(v->fname, row[1]);
		strcpy(v->bname, row[2]);
		v->rotate = atoi(row[3]);
		strcpy(v->ip, row[4]);
		v->dpi = atoi(row[5]);
		strcpy(v->date, row[6]);
		strcpy(v->exchno, row[7]);
		v->session = atoi(row[8]);
		strcpy(v->pkgno, row[9]);
		strcpy(v->vchno, row[10]);
		strcpy(v->accno, row[11]);
		strcpy(v->vchtype, row[12]);
		strcpy(v->amount, row[13]);
		v->multiflag = atoi(row[14]);
		v->state = atoi(row[15]);
		v->mergestate = atoi(row[16]);
		v->mergeid = atol(row[17]);
		v->model = atoi(row[18]);
		v->mainid = atol(row[19]);
		v->envid = atol(row[20]);
		v->mainflag = atoi(row[21]);
		v->checkstate = atoi(row[22]);
		v->prestate = atoi(row[23]);
		v->sendstate = atoi(row[24]);
		strcpy(v->imageid, row[25]);
		//strcpy(v->timestamp, row[26]);
		v->submitstate = atoi(row[27]);
		//strcpy(v->scantime, row[28]);
		strcpy(v->clkno, row[29]);
		v->times = atoi(row[30]);
		strcpy(v->area, row[31]);
		strcpy(v->modeflag, row[32]);
	}
	catch(...)
	{
		LogPrint( MULTIS_NODE, "GetMultisIDSList error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return 0;
}

/*通过主键更新某表中的某列的值
@pk_name: 主键名
@pk_val:  主键值
@tb_name: 表名
@col_name:列名
@col_val: 列值 
*/
int CDBCommon::UpColumnByPK(const char *pk_name, const unsigned long pk_val, const char *tb_name, const char *col_name, const char *col_val)
{
	char str_sql[MAX_SQL_LEN];
	int ret;
	memset(str_sql, '\0', sizeof(str_sql));

	try
	{
		sprintf(str_sql, "UPDATE %s SET %s=%s WHERE %s=%ld", tb_name, col_name, col_val, pk_name, pk_val);

		if(ret = db.Query(str_sql))
		{
			LogPrint(TASK_NODE, "UpColumnByPK error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
		//LogPrint(TASK_NODE, "UpColumnByPK error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "UpColumnByPK error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return 0;
}

int CDBCommon::UpColumnByPK_1(const char *pk_name, const unsigned long pk_val, const char *tb_name, const char *col_name, const char *col_val, int hasDot)
{
	char str_sql[MAX_SQL_LEN];

	char m_col_val[256];
	bzero(m_col_val, sizeof(m_col_val));
	bzero(str_sql, sizeof(str_sql));

	if(!hasDot)
	{
		strcpy( m_col_val, col_val);
	}
	else
	{
		sprintf( m_col_val, "'%s'", col_val);
	}

	try
	{
		sprintf(str_sql, "UPDATE %s SET %s=%s WHERE %s=%ld", tb_name, col_name, col_val, pk_name, pk_val);

		if(db.Query(str_sql))
		{
			LogPrint(TASK_NODE, "UpColumnByPK_1 error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "UpColumnByPK error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return 0;
}

//匹配人行电子数据 
//返回：成功返回id, 失败返回-1 ， 单票匹配成功=1 ，信封匹配=2
int CDBCommon::MatchVouchersPcb(char *vchno, char *exchno, char *accno, char *amount, char *vchtype, char *area, int session, char* date)
{
	char str_sql[MAX_SQL_LEN];
	char m_vchno[MAX_SQL_LEN];
	char m_accno[MAX_SQL_LEN];

	memset(str_sql, '\0', sizeof(str_sql));
	memset(m_vchno, '\0', sizeof(m_vchno));
	memset(m_accno, '\0', sizeof(m_accno));

	int sid;

	Trim(vchno);
	
	Trim(accno);
			
	if(strlen(vchno)!=0)
	{
		if(atol(vchno)!=0)
			sprintf(m_vchno, " ( right(trim(vchno), 6)=right('%s', 6)) ", vchno);
		else
			sprintf(m_vchno, " ( right(trim(vchno), 6)=right('%s', 6)  or trim(vchno)='') ", vchno);
	}
	else
		strcpy(m_vchno, " (right(vchno, 12)='000000000000' or trim(vchno)='')");

	if(strlen(accno)!=0)
	{

		if(atol(accno)!=0)
			sprintf(m_accno, " (right(trim(accno), 12)=right('%s', 12)) ", accno);
		else
			sprintf(m_accno, " (right(trim(accno), 12)=right('%s', 12) or trim(accno)='')", accno);
	}
	else	
		strcpy(m_vchno, " ( right(trim(accno), 12)='000000000000' or trim(accno)='')");

	try
	{
		sprintf(str_sql, "SELECT distinct sid FROM pbcdata where %s and mergestate=0 and right(exchno,5) = right('%s',5) and %s and trim(amount*100)=%s and vchtype='%s' and area='%s' and session=%d and date='%s'", m_vchno, exchno, m_accno, amount, vchtype, area, session, date);

		//LogPrint( VOUCHER_NODE,"sql->[%s]", str_sql);

		if( db.Query(str_sql) != 0 )
		{
			LogPrint( VOUCHER_NODE, "MatchVouchersPcb error[%s] \nstrsql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -2;
		}
		MyRes res = db.Store();

		if(res.Count()<=0)
		{
			LogPrint( VOUCHER_NODE, "can't match pbc data strsql->[%s]\n", str_sql);

			return -1;
		}

		MyRow row = res.Fetch();
			
		sid = atoi(row[0]);
	}
	catch(...)
	{
		LogPrint( VOUCHER_NODE, "catch MatchVouchersPcb error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return sid;
}
//匹配数据--清分模式
//返回：成功返回id, 失败返回-1 ， 单票匹配成功=1 ，信封匹配=2
int CDBCommon::MatchPcbQF(char *vchno, char *exchno, char *accno, char *amount, char *vchtype, char *area, int session, char* date)
{
	char str_sql[MAX_SQL_LEN];
	char m_vchno[MAX_SQL_LEN];
	char m_accno[MAX_SQL_LEN];

	memset(str_sql, '\0', sizeof(str_sql));
	memset(m_vchno, '\0', sizeof(m_vchno));
	memset(m_accno, '\0', sizeof(m_accno));

	int sid;

	Trim(vchno);
	
	Trim(accno);
			
	if(strlen(vchno)!=0)
	{
		if(atol(vchno)!=0)
			sprintf(m_vchno, " ( right(trim(vchno), 6)=right('%s', 6)) ", vchno);
		else
			sprintf(m_vchno, " ( right(trim(vchno), 6)=right('%s', 6)  or trim(vchno)='') ", vchno);
	}
	else
		strcpy(m_vchno, " (right(vchno, 12)='000000000000' or trim(vchno)='')");

	if(strlen(accno)!=0)
	{

		if(atol(accno)!=0)
			sprintf(m_accno, " right(trim(accno), 10)=right('%s', 10) ", accno);
		else
			sprintf(m_accno, " (right(trim(accno), 10)=right('%s', 10) or trim(accno)='')", accno);
	}
	else	
		strcpy(m_accno, " ( right(trim(accno), 10)='0000000000' or trim(accno)='')");

	try
	{
		//在dists表中查询非信封数据 北京分行的单票exchno为9位  8＋1校验位, 扫描仪扫出来的exchno也是 8+1校验位， 但清分机清出来的交换号为 去前两位 ，再去最后一位校验位
		sprintf(str_sql, "SELECT distinct sid FROM dists where %s and mergestate=0 and  %s and trim(amount*100)=%s and vchtype='%s' and area='%s' and isenv<>1  and session=%d and date='%s'", m_vchno, m_accno, amount, vchtype, area, session, date);

		//LogPrint( VOUCHER_NODE, "MatchQF str_sql->[%s]\n", str_sql);

		if( db.Query(str_sql) != 0 )
		{
			LogPrint( VOUCHER_NODE, "MatchPbcQF error[%s] \nstrsql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -2;
		}
		MyRes res = db.Store();

		if(res.Count()<=0)
		{
			LogPrint( VOUCHER_NODE, "can't match dists data, match sql->[%s]\n", str_sql);

			return -1;
		}

		MyRow row = res.Fetch();
			
		sid = atoi(row[0]);
	}
	catch(...)
	{
		LogPrint( VOUCHER_NODE, "catch MatchPbcQF error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return sid;
}

/*匹配人行数据成功，更新pcbdata中的mergeis值为表voucher的sid, 并更新vouchers表中的mergeid为表pcbdata中的sid
@vouchers_id : 表voucher中的sid
@pcb_sid: 	表pcbdata中的sid
*/
int CDBCommon::UpdateVouchersPcb( unsigned long vouchers_sid, unsigned long pcb_sid)
{
	char str_sql[MAX_SQL_LEN];
	try
	{
		memset(str_sql, '\0', sizeof(str_sql));
		int sid;
		
		
			
		//更新pcbdata表中的mergeid值为Vouchers表中的sid, 保存匹配状态
		sprintf(str_sql, "UPDATE pbcdata set mergeid = %ld, mergestate=1 where sid=%ld and mergestate=0 ", vouchers_sid, pcb_sid);

		if( db.Query(str_sql) != 0 )
		{
			
			LogPrint( VOUCHER_NODE, "MatchVouchersPcb error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		//更新vouchers表中的mergeid值为pcb表中的sid, 保存匹配状态
		memset(str_sql, '\0', sizeof(str_sql));
		sprintf(str_sql, "UPDATE vouchers set mergeid = %ld, mergestate=1 where sid=%ld", pcb_sid, vouchers_sid);

		if( db.Query(str_sql) != 0 )
		{
			
			LogPrint( VOUCHER_NODE, "MatchVouchersPcb error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
	}
	catch(...)
	{
		
		LogPrint( VOUCHER_NODE, "Catch MatchVouchersPcb error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
		return -1;
	}
	

	return 0;
}

/*匹配人行数据成功，更新dists中的mergeid值为表voucher的sid, 并更新vouchers表中的mergeid为表dists中的sid
@vouchers_id : 表voucher中的sid
@dists_sid: 	表pcbdata中的sid
*/
int CDBCommon::UpdateVouchersDists( unsigned long vouchers_sid, unsigned long dists_sid)
{
	char str_sql[MAX_SQL_LEN];

	try
	{
		int sid;

		memset(str_sql, '\0', sizeof(str_sql));

		//更新pcbdata表中的mergeid值为Vouchers表中的sid, 保存匹配状态
		sprintf(str_sql, "UPDATE dists set mergeid = %ld, mergestate=1 where sid=%ld and mergestate=0", vouchers_sid, dists_sid);

		//LogPrint( VOUCHER_NODE, "strsql->[%s]", str_sql);

		if( db.Query(str_sql) != 0 )
		{
			LogPrint( VOUCHER_NODE, "MatchVouchersDists error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		//更新vouchers表中的mergeid值为pcb表中的sid, 保存匹配状态
		memset(str_sql, '\0', sizeof(str_sql));

		sprintf(str_sql, "UPDATE vouchers set mergeid = %ld, mergestate=1 where sid=%ld", dists_sid, vouchers_sid);

		//LogPrint( VOUCHER_NODE, "strsql->[%s]", str_sql);

		if( db.Query(str_sql) != 0 )
		{
			
			LogPrint( VOUCHER_NODE, "MatchVouchersDists error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
	}
	catch(...)
	{
		
		LogPrint( VOUCHER_NODE, "Catch MatchVouchersDists error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
		return -1;
	}
	

	return 0;
}


/*单票识别完成后更新Multis表状态并存储信息
@sid: 		信封id
@accno: 	账号
@vchno: 	凭证号
@exchno:	交换号
@vchtype: 	交换类型
@amount: 	金额
@model: 	版式
@state: 	状态
return : 	成功-0 ，失败-其他 
**/
int CDBCommon::UpdateVouchersParse(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model ,int state, int modeflag)
{
	char str_sql[MAX_SQL_LEN];
	memset(str_sql, '\0', sizeof(str_sql));

	try
	{
		//如果金额为空，写入0, 防止atof(amount)出错
		if(!strlen(amount))	
			strcpy(amount, "0");

		//如果vouchers表中accno和amount(人行带扫模式)不为空时，只更新凭证号和模式，否则还要更新账号、金额和凭证类型
		sprintf(str_sql, "UPDATE vouchers set vchno='%s', model=%d, state=%d, modeflag=%d  where sid=%ld", vchno, model, state, modeflag, sid);

		//LogPrint( VOUCHER_NODE, "strsql->[%s]", str_sql);

		
		if( db.Query(str_sql) != 0 )
		{
			
			LogPrint( VOUCHER_NODE, "UpdateVouchersParse error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		sprintf(str_sql, "UPDATE vouchers set accno = '%s', vchtype='%s', amount='%.2f' where sid=%ld and length(accno)<8 and length(amount)<2", accno, vchtype, atof(amount)/100.00, sid);

		if( db.Query(str_sql) != 0 )
		{
			
			LogPrint( VOUCHER_NODE, "UpdateVouchersParse error[%s]", mysql_error(db.Handle()));

			return -1;
		}

	}
	catch(...)
	{
		

		LogPrint( VOUCHER_NODE, "Catch UpdateVouchersParse error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	

	return 0;
}


/*信封识别完成后更新Multis表状态并存储信息
@sid: 		信封id
@accno: 	账号
@vchno: 	凭证号
@exchno:	交换号
@vchtype: 	交换类型
@amount: 	金额
@model: 	版式
@state: 	状态
@mode:		0-自行扫描  1-人行带扫 2-清分模式
return : 	成功-0 ，失败-其他 
**/
int CDBCommon::UpdateMultisParse(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model ,int state, int modeflag)
{

	char str_sql[MAX_SQL_LEN];
	try
	{
		memset(str_sql, '\0', sizeof(str_sql));

		//如果金额为空，写入0，防止atof(amount)出错
		if(!strlen(amount))	
			strcpy(amount, "0");

		//如果multis表中accno和amount(人行带扫模式)不为空时，只更新凭证号和模式，否则还要更新账号、金额和凭证类型
		sprintf(str_sql, "UPDATE multis set vchno='%s', state=%d, modeflag=%d  where sid=%ld", vchno, state, modeflag, sid);

		
		if( db.Query(str_sql)!=0)
		{
			LogPrint( MULTIS_NODE, "UpdateMultisParse error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
			

			return -1;

		}

		sprintf(str_sql, "UPDATE multis set accno = '%s', amount='%.2f',vchtype='%s', model='%d' where sid=%ld and (length(accno)<8 or length(amount)<2)", accno, atof(amount)/100.00, vchtype, model, sid);

		if( db.Query(str_sql) != 0 )
		{
			
			LogPrint( MULTIS_NODE, "UpdateMultisParse error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
	}
	catch(...)
	{
		
		LogPrint( MULTIS_NODE, "Catch UpdateMultisParse error[%s]", mysql_error(db.Handle()));

		return -1;
	}
	

	return 0;
}

/*信封is表状态并存储信息
@sid: 		信封id
@model: 	版式
@state: 	状态
return : 	成功-0 ，失败-其他 
**/
int CDBCommon::UpMultisNotEnv(unsigned long sid, int model ,int state)
{

	char str_sql[MAX_SQL_LEN];
	try
	{
		memset(str_sql, '\0', sizeof(str_sql));
		
		sprintf(str_sql, "UPDATE multis set state=%d  where sid=%ld", state, sid);

		if( db.Query(str_sql) != 0 )
		{
			LogPrint( MULTIS_NODE, "UpMultisNotEnv error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		sprintf(str_sql, "UPDATE multis set model=%d where sid=%ld and model=0", model, sid);

		if( db.Query(str_sql) != 0 )
		{
			LogPrint( MULTIS_NODE, "UpMultisNotEnv error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

	}
	catch(...)
	{
		LogPrint( MULTIS_NODE, "UpMultisNotEnv error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return 0;
}

/*读配置表返回map
@cfg_dic: 	配置表数据集合
return: 	成功-0 ，失败-其他
**/
int CDBCommon::get_tb_config( StrDict& cfg_dic)
{
	char str_sql[MAX_SQL_LEN];

	try
	{
		memset(str_sql, '\0', MAX_SQL_LEN);
	
		strcpy(str_sql, "select name, value from config");
		if( db.Query(str_sql) != 0 )
		{
			LogPrint(TASK_NODE, "get_tb_config error[%s] str_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		MyRes res = db.Store();
		MyRow row = res.Fetch();

		while(row!=NULL)
		{
			cfg_dic[row[0]] = row[1];
			row = res.Fetch();
		}
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "get_tb_config error[%s] str_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return 0;	
}

//获取bocnets表中model-vchtype列表
int CDBCommon::get_model_vchtype( StrDict& cfg_dic)
{
	char str_sql[MAX_SQL_LEN];
	try
	{
		memset(str_sql, '\0', MAX_SQL_LEN);

		//修改为(凭证类型+交换区域)
		strcpy(str_sql, "select distinct concat(area,vchtype) ,model from vchtypes");

		if( db.Query(str_sql) != 0 )
		{
			LogPrint(TASK_NODE, "get_model_vchtype error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		MyRes res = db.Store();
		MyRow row = res.Fetch();

		while(row!=NULL)
		{
			cfg_dic[row[0]] = row[1];
			row = res.Fetch();
		}
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "Catch get_model_vchtype error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return 0;	
}

//获取bocnets表中model-area列表
int CDBCommon::GetVchtypeByModel( StrDict& cfg_dic)
{
	char str_sql[MAX_SQL_LEN];
	try
	{
		memset(str_sql, '\0', MAX_SQL_LEN);

		//修改为(凭证类型+交换区域)
		strcpy(str_sql, "select distinct concat(area,model) ,vchtype from vchtypes");

		if( db.Query(str_sql) != 0 )
		{
			LogPrint(TASK_NODE, "get_vchtype_model error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		MyRes res = db.Store();
		MyRow row = res.Fetch();

		while(row!=NULL)
		{
			cfg_dic[row[0]] = row[1];
			row = res.Fetch();
		}
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "Catch GetVchtypeByModel error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return 0;	
}


//返回: 键=(area+exchno) 值=(compare+merge)
int CDBCommon::get_tb_bocnets( StrDict& cfg_dic)
{
	try
	{
		char str_sql[MAX_SQL_LEN];
		memset(str_sql, '\0', MAX_SQL_LEN);

		//字段说明: compare--是否校验  merge--是否匹配
		strcpy(str_sql, "select concat(area, right(exchno,5)) ,concat(compare,merge) from bocnets;");
		if( db.Query(str_sql) != 0 )
		{
			LogPrint(TASK_NODE, "get_tb_bocnets error[%s]", mysql_error(db.Handle()));

			return -1;
		}

		MyRes res = db.Store();
		MyRow row = res.Fetch();

		while(row!=NULL)
		{
			cfg_dic[row[0]] = row[1];
			row = res.Fetch();
		}
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "Catch get_tb_bocnets error[%s]", mysql_error(db.Handle()));

		return -1;
	}

	return 0;	
}


//锁定指定任务
int CDBCommon::GetTaskLock(char *tb_name,char *sid_name, unsigned long sid_val,char *col_name, int col_val)
{
	char str_sql[MAX_SQL_LEN];
	int ret;
	memset(str_sql, '\0', sizeof(str_sql));

	try
	{
//		LogPrint(TASK_NODE, "TaskLock error [%s] \nstr_sql->[%d]\n", mysql_error(db.Handle()), str_sql);

		sprintf(str_sql, "UPDATE %s SET %s=%d, timestamp=now() WHERE %s=%ld", tb_name, col_name, col_val, sid_name, sid_val);

		if(ret = db.Query(str_sql))
		{
			LogPrint(TASK_NODE, "TaskLock error [%s] \nstr_sql->[%d]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "Catch TaskLock error [%s] \nstr_sql->[%d]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return 0;
}


//写入人行清算数据
int CDBCommon::PutPbcQSData( pbc_header_t *h, pbc_qs_t *q)
{
	char str_sql[MAX_SQL_LEN];
	try
	{
		memset(str_sql, '\0', MAX_SQL_LEN);
	
		sprintf(str_sql, "insert into bocctrl(date, session,exchno,ccamount, dcamount, cramount, dramount ) values('%s', '%s', '%s', '%s', '%s', '%s','%s'", h->date, h->session, q->exchno, q->ccamount, q->dcamount, q->cramount, q->dramount);

		if( db.Query(str_sql) != 0 )
		{
			LogPrint(TASK_NODE, "PutPbcQSData error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "Catch PutPbcQSData error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return 0;
}

//写入人行明细数据
int CDBCommon::PutPbcMXData( pbc_header_t *h, pbc_mx_t *m)
{
	char str_sql[MAX_SQL_LEN];
	try
	{
		memset(str_sql, '\0', MAX_SQL_LEN);
	
		sprintf(str_sql, "insert into pbcdata( date,session, exchno, amount, accno, vchno, vchtype, date) values( '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",h->date, h->session, m->exchno, m->amount, m->accno, m->vchno, m->vchtype, m->vchdate );

		if( db.Query(str_sql) != 0 )
		{
			LogPrint(TASK_NODE, "PutPbcMXData error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "Catch PutPbcMXData error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}

	return 0;
}

/*获取人行明细数据
@m: 输入结构体传入参数
@ma: 输出结构体数组返回结果
**/
int CDBCommon::GetPbcMXData( pbc_header_t *h, vector<pbc_mx_t>& v)
{
	pbc_mx_t *q;
	char str_sql[MAX_SQL_LEN];

	try
	{
		//查询bocnet得到exchno
		memset(str_sql, '\0', MAX_SQL_LEN);

		//查询出来的金额是以分为单位	
		sprintf( str_sql,"select  v.exchno,v.amount*100 , v.accno, vc.banktype, v.vchno, vc.cdcode from vouchers  v inner join bocnets b on v.area=b.area and v.pkgno=b.exchno inner join vchtypes vc on v.vchtype=vc.vchtype and v.area = vc.area where (vc.banktype='113' or vc.banktype = '115' or vc.banktype='105') and v.area='%s' and  v.date='%s' and v.session=%s", h->area, h->date, h->session );

		//sprintf(str_sql, " select  exchno,amount*100 , accno, vchtype, vchno from vouchers where  date='%s' and session=%s and exchno in (select distinct exchno from  bocnets where area='%s') and vchtype in ( select vchtype from vchtypes where (banktype='113' or banktype = '115' or banktype='105') and area='%s' ) order by exchno", h->date, h->session, h->area, h->area);

		if( db.Query(str_sql) != 0 )
		{
			LogPrint(TASK_NODE, "GetPbcMXData error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
		q = (pbc_mx_t *)malloc(sizeof(pbc_mx_t));
		memset(q, 0, sizeof(pbc_mx_t));
		
		MyRes res = db.Store();
		MyRow row = res.Fetch();
	
		while(row!=NULL)
		{
			memset(q, 0, sizeof(pbc_mx_t));

			strcpy(q->exchno, row[0]);
			strcpy(q->amount, row[1]);
			strcpy(q->accno, row[2]);
			strcpy(q->vchtype, row[3]);
			strcpy(q->vchno, row[4]);
			strcpy(q->cdcode, row[5]);

			v.push_back(*q);

			row = res.Fetch();
		}

		if(q!=NULL)
			free(q);
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "Catch GetPbcMXData error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		if(q!=NULL)
			free(q);
		return -1;
	}

	return 0;

}

/*获取人行清算数据
@q: 输入结构体传入参数
@qa: 输出结构体数组返回结果集
**/
int CDBCommon::GetPbcQSData( pbc_header_t *h, vector<pbc_qs_t>& v)
{
	pbc_qs_t *q;
	char str_sql[MAX_SQL_LEN];

	try
	{
		memset(str_sql, '\0', MAX_SQL_LEN);

		//查询bocnet得到exchno

		memset(str_sql, '\0', MAX_SQL_LEN);

		//查询出来的金额是以分为单位	

		sprintf(str_sql, "select  exchno, ccamount*100, dcamount*100, cramount*100, dramount*100 from bocctrl where  date='%s' and session=%s and exchno in (select distinct exchno from  bocnets where area='%s') and area='%s' order by exchno", h->date, h->session, h->area, h->area);

		if( db.Query(str_sql) != 0 )
		{
			LogPrint(TASK_NODE, "GetPbcQSData error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
		q = (pbc_qs_t *)malloc(sizeof(pbc_qs_t));
		memset(q, 0, sizeof(pbc_qs_t));
		
		MyRes res = db.Store();
		MyRow row = res.Fetch();


		while(row!=NULL)
		{

			memset(q, 0, sizeof(pbc_qs_t));

			strcpy(q->exchno, row[0]);
			strcpy(q->ccamount, row[1]);
			strcpy(q->dcamount, row[2]);
			strcpy(q->cramount, row[3]);
			strcpy(q->dramount, row[4]);

			row = res.Fetch();
			v.push_back(*q);
		}

		if(q!=NULL)
			free(q);
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "GetPbcQSData error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		if(q!=NULL)
			free(q);

		return -1;
	}

	return 0;
}

//根据id获取委收信息, 返回信息封装到结构体v中
int CDBCommon::GetAuthInputRowBySid(const char *sid, struct tb_authinput_s *v)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	try
	{
		//更新时间戳
		memset(str_sql, '\0', sizeof(str_sql));
		sprintf(str_sql, "update authinput set  timestamp=now() where sid=%ld and state=2", atol(sid));

		if(ret = db.Query(str_sql))
		{
			
			LogPrint( AUTHINPUT_NODE, "GetAuthInputRowBySid error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		memset(str_sql, '\0', sizeof(str_sql));
		sprintf(str_sql, "SELECT distinct sid,fname,bname,rotate,ip,dpi,date, right(exchno,5) ,session, right(pkgno,5),vchno,accno,vchtype, amount, multiflag, state, mergestate, mergeid, model, mainid, envid, mainflag, checkstate, prestate, sendstate, imageid, timestamp, substate, scantime, clkno, times, area  FROM authinput WHERE sid = %d ", atoi(sid));

		if(ret = db.Query(str_sql))
		{
			
			LogPrint( AUTHINPUT_NODE, "GetAuthInputRowBySid error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}

		MyRes res = db.Store();

		if(res.Count()==0)
			return SQL_NO_RESULT;

		MyRow row = res.Fetch();

		v->sid = atol(row[0]);
		strcpy(v->fname, row[1]);
		strcpy(v->bname, row[2]);
		v->rotate = atoi(row[3]);
		strcpy(v->ip, row[4]);
		v->dpi = atoi(row[5]);
		strcpy(v->date, row[6]);
		strcpy(v->exchno, row[7]);
		v->session = atoi(row[8]);
		strcpy(v->pkgno, row[9]);
		strcpy(v->vchno, row[10]);
		strcpy(v->accno, row[11]);
		strcpy(v->vchtype, row[12]);
		strcpy(v->amount, row[13]);
		v->multiflag = atoi(row[14]);
		v->state = atoi(row[15]);
		v->mergestate = atoi(row[16]);
		v->mergeid = atol(row[17]);
		v->model = atoi(row[18]);
		v->mainid = atol(row[19]);
		v->envid = atol(row[20]);
		v->mainflag = atoi(row[21]);
		v->checkstate = atoi(row[22]);
		v->prestate = atoi(row[23]);
		v->sendstate = atoi(row[24]);
		strcpy(v->imageid, row[25]);
		//strcpy(v->timestamp, row[26]);
		v->submitstate = atoi(row[27]);
		//strcpy(v->scantime, row[28]);
		strcpy(v->clkno, row[29]);
		v->times = atoi(row[30]);
		strcpy(v->area, row[31]);
	}
	catch(...)
	{
		
		LogPrint( AUTHINPUT_NODE, "GetAuthInputRowBySid error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}
	

	return 0;
}

/*委收识别完成后更新authinput表状态并存储信息
@sid: 		信封id
@accno: 	账号
@vchno: 	凭证号
@exchno:	交换号
@vchtype: 	交换类型
@amount: 	金额
@model: 	版式
@state: 	状态
@mode:		0-自行扫描  1-人行带扫 2-清分模式
@isEnv:		1-信封, 0-票据
@mainflag:  更新票据时使用， 1-第一联, 2-第二联
return : 	成功-0 ，失败-其他 
**/
int CDBCommon::UpdateAuthInputParse(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model ,int state, int isAuthInput, int mainflag)
{

	char str_sql[MAX_SQL_LEN];

	if(!strlen(amount))	
		strcpy(amount, "0");
	try
	{
		//如果是信封
		if(!isAuthInput)
		{
			memset(str_sql, '\0', sizeof(str_sql));

			//如果金额为空，写入0，防止atof(amount)出错
				//如果authinput表中accno和amount(人行带扫模式)不为空时，只更新凭证号和模式，否则还要更新账号、金额和凭证类型
			sprintf(str_sql, "UPDATE authinput set vchno='%s', model=%d, state=%d, envid=%ld,accno='%s', amount='%.2f', vchtype='%s'  where sid=%ld", vchno, model, state, sid, accno, atof(amount)/100.00, vchtype, sid);

			if( db.Query(str_sql)!=0)
			{
				LogPrint( AUTHINPUT_NODE, "UpdateAuthInputParse error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
				

				return -1;

			}
		}
		//如果是票据, 存金额和主副联
		else
		{
			memset(str_sql, '\0', sizeof(str_sql));
		
			if(!strlen(amount))
				strcpy(amount, "0");
	
			sprintf( str_sql, "UPDATE authinput set amount=%.2f, mainflag=%d, state=%d where sid = %ld", atof(amount), mainflag, state, sid);

			if( db.Query(str_sql)!=0)
			{
				LogPrint( AUTHINPUT_NODE, "UpdateAuthInputParse error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

				return -1;
			}
		}
	}
	catch(...)
	{
		LogPrint( AUTHINPUT_NODE, "Catch UpdateAuthInputParse error[%s]", mysql_error(db.Handle()));

		return -1;
	}

	return 0;
}

//定期调用此函数可以防止因长期不连数据库导致服务与数据连接断开.
//成功返回0， 失败返回1， 异常返回-1
int CDBCommon::HasVersion()
{
	char str_sql[MAXLINE];
	memset(str_sql, '\0', sizeof(str_sql));

	strcpy(str_sql, "select @@VERSION");

	if( db.Query(str_sql)!=0 )		
	{
		LogPrint( TASK_NODE, "GetVersion error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
		
		return -1;
	}	
	MyRes res = db.Store();

	MyRow row = res.Fetch();

	if(row==NULL)
		return SQL_NO_RESULT;

	return 0;
}

//返回tb_model
int CDBCommon::GetModelByVchtype( const char * vchtype, const char* area)
{
	char str_sql[MAXLINE];
	memset(str_sql, '\0', sizeof(str_sql));

	sprintf(str_sql, "select model from vchtypes where area='%s' and vchtype='%s' ", area, vchtype);

	if( db.Query(str_sql)!=0 )		
	{
		LogPrint( TASK_NODE, "GetModelByVchtype error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
		
		return -1;
	}	

	MyRes res = db.Store();

	MyRow row = res.Fetch();

	if(row==NULL)
		return -1;

	return atoi(row[0]);
}

//通过area+model取凭证类型
int CDBCommon::GetVchtypeByModel( int model, char* area, char *vchtype)
{
	char str_sql[MAXLINE];
	memset(str_sql, '\0', sizeof(str_sql));

	sprintf(str_sql, "select vchtype from vchtypes where area='%s' and model=%d limit 1", area, model);

	if( db.Query(str_sql)!=0 )		
	{
		LogPrint( TASK_NODE, "GetVchtypeByModel error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
		
		return -1;
	}	

	MyRes res = db.Store();

	MyRow row = res.Fetch();

	if(row==NULL)
		return -1;
	
	strcpy(vchtype, row[0]);

	return 0;
}


//通过凭证类型获取校验位和匹配位
int CDBCommon::GetCmpMerByExchno(char *area, char *exchno, int *compare, int *merge)
{
	char str_sql[MAXLINE];
	memset(str_sql, '\0', sizeof(str_sql));

	sprintf(str_sql, "select compare, merge from bocnets where area='%s' and right(exchno,5)='%s'", area, exchno);

	if( db.Query(str_sql)!=0 )		
	{
		LogPrint( VOUCHER_NODE, "GetVchtypeByModel error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
		
		return -1;
	}	

	MyRes res = db.Store();

	MyRow row = res.Fetch();

	if(row==NULL)
	{
		LogPrint( VOUCHER_NODE, "GetVchtypeByModel error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
		return -1;
	}

	if(strlen(row[0]) > 0)	
		*compare = atoi(row[0]);

	if(strlen(row[1]) > 0)
		*merge = atoi(row[1]);

	return 0;
}
int CDBCommon::SaveCtrl(const char*date,const char*session,const char*area,const char*exchno,const char*dramount,const char*cramount,int flag)
{
	char str_sql[MAXLINE];
	memset(str_sql,0,sizeof(str_sql));
	sprintf(str_sql,"insert into bocctrl(date,session,area,exchno,dramount,cramount,checked) values('%s','%s','%s','%s',%s/100.00,%s/100.00,%d)",date,session,area,exchno,dramount,cramount,flag);
	
	int ret = db.Query(str_sql);
	if(ret)
	{
		LogPrint(TASK_NODE,"save ctrl data error[%s]",mysql_error(db.Handle()), str_sql);
		return ret;
	}	
	return ret;
}

int CDBCommon::SavectrlNotExist(const char*date,const char*session,const char*area)
{
	char str_sql[MAXLINE];
	memset(str_sql,0,sizeof(str_sql));
	
	int ret = db.VQuery(512,"select bocnets.exchno from bocnets left join bocctrl on bocnets.exchno=bocctrl.exchno where bocctrl.date='%s' and bocctrl.session='%s' and bocctrl.area='%s'",date,session,area);
	if(ret)
	{
		LogPrint(TASK_NODE,"save ctrl data error[%s]",mysql_error(db.Handle()), str_sql);
                return ret;
	}
	MyRes res = db.Store();
	MyRow row = res.Fetch();
	while(row!=NULL)
	{
		SaveCtrl(date,session,area,row[0],"000","000",1);
		row = res.Fetch();
	}
	
	db.Query("update bocctrl set checked=1 where checked=0 and dramount='0.00' and cramount='0.00'");
	return 0;
}


int CDBCommon::GetCinputSIDSList(vector<unsigned long> &vc_set, int lim_num, int date, int session, const char *brno)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	UpdateParseTimeOut("cinputs");

	memset(str_sql, '\0', sizeof(str_sql));

	sprintf(str_sql, "select sid from cinputs where date=%d and (state = 0 or (state = 3 and times< 10 and (now()-timestamp>120) )) order by times limit %d", date, lim_num); 

	try
	{
		if(ret = db.Query(str_sql))
		{
			LogPrint( TASK_NODE, "str_sql->[%s]", str_sql);
			LogPrint( TASK_NODE, "GetCinputSIDSList error[%s]", mysql_error(db.Handle()));

			return -1;
		}

		MyRes res = db.Store();

		MyRow row = res.Fetch();
	
		while(row != NULL)
		{
			vc_set.push_back(atol(row[0]));
	
			row = res.Fetch();
		}
	}
	catch(...)
	{
			LogPrint( TASK_NODE, "Catch GetCinputSIDSList error[%s]", mysql_error(db.Handle()));
			LogPrint( TASK_NODE, "str_sql->[%s]", str_sql);

	 		return -1;
	}

	return 0;
}

int  CDBCommon::GetCinputSendList(vector<unsigned long> &vcu_set, int lim_num, int date, int session, const char *brno)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	memset(str_sql, '\0', sizeof(str_sql));

	UpdateSendTimeOut("cinputs");

	sprintf(str_sql, "select sid from cinputs where  vchtype='62' and date=%d and ((state = 1 and mergestate = 1) and (sendstate = 0 or((sendstate = 3 or sendstate=4) and times<100)) and (now()-timestamp) >60) order by times limit %d", date, lim_num);

	try
	{
		if(ret = db.Query(str_sql))
		{
			LogPrint( TASK_NODE, "GetCinputSendList error[%s]", mysql_error(db.Handle()));
			LogPrint( TASK_NODE, "str_sql->[%s]", str_sql);

			return -1;
		}

		MyRes res = db.Store();

		MyRow row = res.Fetch();
	
		while(row != NULL)
		{
			vcu_set.push_back(atol(row[0]));
	
			row = res.Fetch();
		}
	}
	catch(...)
	{
		LogPrint( TASK_NODE, "Catch GetCinputSendList error[%s]", mysql_error(db.Handle()));
		LogPrint( TASK_NODE, "str_sql->[%s]", str_sql);

		return -1;
	}

	return 0;
}

int CDBCommon::GetCinputRowBySid(const char *sid, struct tb_vouchers_s *v)
{
	int ret, ct;	
	char str_sql[MAX_SQL_LEN];

	try
	{
		//更新时间戳
		memset(str_sql, '\0', sizeof(str_sql));
		sprintf(str_sql, "update cinputs set timestamp=now() where sid=%ld and state=2", atol(sid));

		if(ret = db.Query(str_sql))
		{
			LogPrint( CINPUT_NODE, "GetCinputSIDSList error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
			return -1;
		}

		//获取明细
		memset(str_sql, '\0', sizeof(str_sql));
		sprintf(str_sql, "SELECT distinct sid,fname,bname,rotate,ip,dpi,date, right(exchno,5) ,session,state,vchno,accno,vchtype,amount,model,mergestate,mergeid,sendstate,imageid,timestamp,submitstate,scantime,prestate,clkno,times,right(pkgno, 5), area,(case when modeflag then 'N' else '0' end) as modeflag  FROM cinputs WHERE sid = %d ", atoi(sid));

		if(ret = db.Query(str_sql))
		{
			
			LogPrint(  CINPUT_NODE, "GetCinputSIDSList error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);
			return -1;
		}
		MyRes res = db.Store();

		if(res.Count()==0)
		return SQL_NO_RESULT;

		MyRow row = res.Fetch();
		v->sid = atol(row[0]);
		strcpy(v->fname, row[1]);
		strcpy(v->bname, row[2]);
		v->rotate = atoi(row[3]);
		strcpy(v->ip, row[4]);
		v->dpi = atoi(row[5]);

		strcpy(v->date, row[6]);
		strcpy(v->exchno, row[7]);
		v->session = atoi( row[8]);
		v->state = atoi(row[9]);
		strcpy(v->vchno, row[10]);
		strcpy(v->accno, row[11]);
		strcpy(v->vchtype, row[12]);
		strcpy(v->amount, row[13]);	

		v->model = atoi( row[14]);
		v->mergestate = atoi(row[15]);
		v->mergeid = atol(row[16]);
		v->sendstate = atoi(row[17]);
		strcpy(v->imageid, row[18]);

		//strcpy(v->timestamp, row[19]);
		v->submitstate = atoi(row[20]);
		//strcpy(v->scantime, row[21]);
		v->prestate = atoi(row[22]);
		strcpy(v->clkno, row[23]);
		v->times = atoi(row[24]);
		strcpy(v->pkgno, row[25]);
		strcpy(v->area, row[26]);
		strcpy(v->modeflag, row[27]);
	}
	catch(...)
	{
		
		LogPrint( CINPUT_NODE, "GetCinputSIDSList error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}
	

	return 0;
}

int CDBCommon::UpdateCinputParse(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model ,int state, int modeflag)
{
	char str_sql[MAX_SQL_LEN];
	memset(str_sql, '\0', sizeof(str_sql));

	try
	{
		sprintf(str_sql, "UPDATE cinputs set  model=%d, state=%d, modeflag=%d  where sid=%ld",  model, state, modeflag, sid);

		if( db.Query(str_sql) != 0 )
		{
			
			LogPrint( CINPUT_NODE, "Update cinputs error[%s]\nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

			return -1;
		}
	}
	catch(...)
	{
		LogPrint( CINPUT_NODE, "Catch UpdateCinputParse error[%s] \nstr_sql->[%s]\n", mysql_error(db.Handle()), str_sql);

		return -1;
	}
	return 0;
}


