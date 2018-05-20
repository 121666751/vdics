#include "TaskManager.h"
#include <PubFun.h>

#define MAXLINE 1024

#define TASK_COUNT 100

#define IP_ACC_LIST "ip_acc_lst"

using namespace std;

CTaskManagerServer::CTaskManagerServer(){}

CTaskManagerServer::~CTaskManagerServer(){}

static int verbose = 0;

static const char *bankno = NULL;

/*初始化数据库信息
@ecfg_name: 配置文件名
@返回： 成功-0 ，失败-其他
*/
int  CTaskManagerServer::InitDBInfo()
{
	return m_db.DbOpen(db_host.c_str(), db_uid.c_str(), db_pwd.c_str(), db_dbAlias.c_str());
}

//初始化配置文件信息
int CTaskManagerServer::InitCfg(char *cfg_name)
{
	try
	{
		Config cfg(cfg_name);

		brno = cfg.value("brno", 1);   
	    if(brno.size()<=0)
	    {    
			LogPrint( TASK_NODE, "配置错误: brno不能为空!");
		    printf("配置错误: brno不能为空!\n");
		    return -1;
		}    
		if(brno.size()>6)
		{    
			LogPrint( TASK_NODE, "配置错误: brno长度不能超过6位!");
			printf( "配置错误: brno长度不能超过6位!\n");
		    return -1;
		}
		bankno = brno.c_str();
		LogPrint( TASK_NODE, "brno->[%s]", bankno);

		ip = cfg.value("task_ip",1);
		if(ip.size()<=0)
		{
			LogPrint( TASK_NODE, "配置错误： task_ip不能为空!");
			printf( "配置错误： task_ip不能为空!");
			return -1;
		}

		port = cfg.value("task_port",1);
		if(port.size()<=0)
		{
			LogPrint( TASK_NODE, "配置错误： task_port不能为空!");
			printf( "配置错误： task_port不能为空!");
			return -1;
		}

		db_host = cfg.value("db_host",1);
		if(db_host.size()<=0)
		{
			LogPrint( TASK_NODE, "配置错误： db_host不能为空!");
			printf( "配置错误： db_host不能为空!");
			return -1;
		}

		db_uid = cfg.value("db_uid",1);
		if(db_uid.size()<=0)
		{
			LogPrint( TASK_NODE, "配置错误： db_uid不能为空!");
			printf( "配置错误： db_uid不能为空!");
			return -1;
		}

		db_pwd = cfg.value("db_pwd",1);
		if(db_pwd.size()<=0)
		{
			LogPrint( TASK_NODE, "配置错误： db_pwd不能为空!");
			printf( "配置错误： db_pwd不能为空!");
			return -1;
		}

		db_dbAlias = cfg.value("db_dbAlias",1);
		if(db_dbAlias.size()<=0)
		{
			LogPrint( TASK_NODE, "配置错误： db_dbAlias不能为空!");
			printf( "配置错误： db_dbAlias不能为空!");
			return -1;
		}

		pbc_qs_mx_path = cfg.value("pbc_qs_mx_path",1);
		if(pbc_qs_mx_path.size()<=0)
		{
			LogPrint( TASK_NODE, "配置错误： pbc_qs_mx_path不能为空!");
			printf( "配置错误： pbc_qs_mx_path不能为空!");
			return -1;
		}

		string use_ip = cfg.value("use_ip_acc",1);
		if(use_ip.size()<=0)
		{
			LogPrint( TASK_NODE, "配置错误： use_ip_acc不能为空!");
			printf( "配置错误： use_ip_acc不能为空!");
			return -1;
		}

		use_ip_acc = atoi(use_ip.c_str());
	}
	catch(...)
	{
		LogPrint( TASK_NODE, "InitDbInfo");
		return -1;
	}

	return 0;
}

/* 处理单票任务
@fd: socket文件描述符
返回： 成功-0 ， 失败-其他 
*/
int CTaskManagerServer::proc_vouchers_task( int fd, int date, int session)
{
	CAnsiMemFile m_cm;

	char result[PKG_RESULTCODE_LEN+1];
	char taskid[PKG_TASKID_LEN+1];

	memset(result, '\0', sizeof(result));
	memset(taskid, '\0', sizeof(taskid));

	//预留报文长度 
	m_cm.Write(" ", PKG_LEN_LEN ,1);

	//写入报文类型
	m_cm.Write(PKG_USR_RES, PKG_TYPE_LEN, strlen(PKG_USR_RES), true, '0');

	//写入交易码
  	m_cm.Write(VOUCHERS_TRADECODE_RES, PKG_TRADECODE_LEN, strlen(VOUCHERS_TRADECODE_RES), true, '0');

	int sql_ret = 0;
	try
	{
		//如果当前队列为空,查询数据库获取任务列表
		if(vc_vc.size()==0)
		{
			int sql_ret = m_db.GetVouchersSIDSList(vc_vc, DB_QUEUE_LIMIT, date, session, bankno);

			//查询后队列仍为空
			if(sql_ret == 0)
			{
				if(vc_vc.size()>0)
				{
					strcpy(result, TASK_RESULT_SUCC);
					strcpy(taskid, itos(vc_vc.at(0)).c_str());
				}
				else
				{
					strcpy(result, TASK_RESULT_NONE);
					strcpy(taskid, "0");	
				}
			}
			//查询数据库错误
			else if(sql_ret==-1)
			{
				strcpy(result, TASK_SQL_ERROR);
				strcpy(taskid, "0");
			}
		}
		//如果当前队列不为空
		else if(vc_vc.size()>0)	
		{
				strcpy(result, TASK_RESULT_SUCC);
				strcpy(taskid, itos(vc_vc.at(0)).c_str());
		}

		//写入result
  		m_cm.Write(result, PKG_RESULTCODE_LEN, strlen(result), true, '0');

		//写入taskid
  		m_cm.Write(taskid, PKG_TASKID_LEN, strlen(taskid), true, '0');
		

		//报文体长度
		int len = m_cm.GetLength()- PKG_LEN_LEN;

		m_cm.Seek(0, CAnsiMemFile::begin);

		//写入报文体长度
		m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, strlen(itos(len).c_str()), true,'0');

		LogPrint(TASK_NODE , "Send->[%s]", m_cm.GetPtr());
		
		if(vc_vc.size() > 0)
		{

			//任务锁定
			TaskLock( VOUCHERS_TRADECODE_REQ, vc_vc.at(0));

			vc_vc.erase(vc_vc.begin());
			if(vc_vc.size()==0)
				m_db.GetVouchersSIDSList(vc_vc, DB_QUEUE_LIMIT, date, session, bankno);
		}

		//发送任务
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -1;
	}
	catch(...)
	{
    	m_cm.Write(TASK_RESULT_FAIL, PKG_RESULTCODE_LEN, strlen(TASK_RESULT_FAIL), true, '0');
		
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -2;

		return -1;
	}

	return 0;
}

/*处理单票上传
@fd: 文件描述符
返回： 成功-0 ， 失败-其他*/
int CTaskManagerServer::proc_vouchers_up_task( int fd, int date, int session)
{
	CAnsiMemFile m_cm;

	m_cm.Write(" ", PKG_LEN_LEN ,1);
	m_cm.Write(PKG_USR_RES, PKG_TYPE_LEN, strlen(PKG_USR_RES), true, '0');
    m_cm.Write(VOUCHERS_TRADECODE_UP_RES, PKG_TRADECODE_LEN, strlen(VOUCHERS_TRADECODE_UP_RES), true, '0');

	char result[PKG_RESULTCODE_LEN+1];
	char taskid[PKG_TASKID_LEN+1];

	memset(result, '\0', sizeof(result));
	memset(taskid, '\0', sizeof(taskid));

	try
	{
		if(vc_vcu.size()==0)
		{
			int sql_ret = m_db.GetVouchersSendList(vc_vcu, DB_QUEUE_LIMIT, date, session, bankno);

			if(sql_ret == 0)
			{
				if(vc_vcu.size()>0)
				{
					strcpy(result, TASK_RESULT_SUCC);
					strcpy(taskid, itos(vc_vcu.at(0)).c_str());
				}
				else
				{
					strcpy(result, TASK_RESULT_NONE);
					strcpy(taskid, "0");
				}
			}
			else if(sql_ret==-1)
			{
					strcpy(result, TASK_RESULT_FAIL);
					strcpy(taskid, "0");
			}
		}
		else if(vc_vcu.size()>0)	
		{
			strcpy(result, TASK_RESULT_SUCC);
			strcpy(taskid, itos(vc_vcu.at(0)).c_str());
		}

		//写入result

  		m_cm.Write(result, PKG_RESULTCODE_LEN, strlen(result), true, '0');

		//写入taskid
  		m_cm.Write(taskid, PKG_TASKID_LEN, strlen(taskid), true, '0');
		
		m_cm.Seek(0, CAnsiMemFile::begin);

		int len = m_cm.GetLength()- PKG_LEN_LEN;

		//写如包长
		m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, strlen(itos(len).c_str()), true,'0');

		LogPrint(TASK_NODE,"Send->[%s]", m_cm.GetPtr());
		
		if(vc_vcu.size() > 0)
		{

			//任务锁定
			TaskLock(  VOUCHERS_TRADECODE_UP_REQ, vc_vcu.at(0)); 
			vc_vcu.erase(vc_vcu.begin());
			if(vc_vcu.size()==0)
				m_db.GetVouchersSendList(vc_vcu, DB_QUEUE_LIMIT, date, session, bankno);
		}

		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -1;
	}
	catch(...)
	{
    	m_cm.Write(TASK_RESULT_FAIL, PKG_RESULTCODE_LEN, strlen(TASK_RESULT_FAIL), true, '0');
		
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -2;

		return -1;
	}

	return 0;
}


int CTaskManagerServer::proc_multis_task( int fd, int date, int session)
{
	CAnsiMemFile m_cm;

	char result[PKG_RESULTCODE_LEN+1];
	char taskid[PKG_TASKID_LEN+1];
	memset(result, '\0', sizeof(result));
	memset(taskid, '\0', sizeof(taskid));

	m_cm.Write(" ", PKG_LEN_LEN ,1);
	//写入类型
	m_cm.Write(PKG_USR_RES, PKG_TYPE_LEN, strlen(PKG_USR_RES), true, '0');
	//写入交易码
  	m_cm.Write(MULTIS_TRADECODE_RES, PKG_TRADECODE_LEN, strlen(MULTIS_TRADECODE_RES), true, '0');

	try
	{
		if(vc_mt.size()==0)
		{
			int sql_ret = m_db.GetMultisParseList(vc_mt, DB_QUEUE_LIMIT, date, session, bankno);
			
			if(sql_ret == 0)
			{
				if(vc_mt.size()>0)
				{
					strcpy(result, TASK_RESULT_SUCC);
					strcpy(taskid, itos(vc_mt.at(0)).c_str());
				}
				else
				{
					strcpy(result, TASK_RESULT_NONE);
					strcpy(taskid, "0");
				}
			}
			else if(sql_ret==-1)
			{
				strcpy(result, TASK_SQL_ERROR);
				strcpy(taskid, "0");
			}
		}
		else if(vc_mt.size()>0)	
		{
				strcpy(result, TASK_RESULT_SUCC);
				strcpy(taskid, itos(vc_mt.at(0)).c_str());
		}

		//写入结果
		m_cm.Write(result , PKG_RESULTCODE_LEN, strlen(result),  true,'0');

		//写入任务ID
		m_cm.Write(taskid , PKG_TASKID_LEN, strlen(taskid),  true,'0');

		int len = m_cm.GetLength()- PKG_LEN_LEN;

		m_cm.Seek(0, CAnsiMemFile::begin);

		m_cm.Write( itos(len).c_str(), PKG_LEN_LEN, strlen(itos(len).c_str()), true, '0' );

		LogPrint(TASK_NODE,"Send->[%s]", m_cm.GetPtr());
		
		if(vc_mt.size() > 0)
		{

			//任务锁定
			TaskLock( MULTIS_TRADECODE_REQ, vc_mt.at(0));

			vc_mt.erase(vc_mt.begin());
			if(vc_mt.size()==0)
				m_db.GetMultisParseList(vc_mt, DB_QUEUE_LIMIT, date, session, bankno);
		}

		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -1;
	}
	catch(...)
	{
    		m_cm.Write(TASK_RESULT_FAIL, PKG_RESULTCODE_LEN, strlen(TASK_RESULT_FAIL), true, '0');
		
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -2;

		return -1;
	}

	return 0;
}

//处理信封上传任务
int CTaskManagerServer::proc_multis_up_task( int fd, int date, int session)
{
	CAnsiMemFile m_cm;

	char result[PKG_RESULTCODE_LEN+1];
	char taskid[PKG_TASKID_LEN+1];
	memset(result, '\0', sizeof(result));
	memset(taskid, '\0', sizeof(taskid));

	m_cm.Write(" ", PKG_LEN_LEN ,1);
	m_cm.Write(PKG_USR_RES, PKG_TYPE_LEN, strlen(PKG_USR_RES), true, '0');
    m_cm.Write(MULTIS_TRADECODE_UP_RES, PKG_TRADECODE_LEN, strlen(MULTIS_TRADECODE_UP_RES), true, '0');

	try
	{
		if(vc_mtu.size()==0)
		{
			int sql_ret = m_db.GetMultisSendList(vc_mtu, DB_QUEUE_LIMIT, date, session, bankno);

			if(sql_ret == 0)
			{
				if(vc_mtu.size()>0)
				{
						strcpy(result, TASK_RESULT_SUCC);
						strcpy(taskid, itos(vc_mtu.at(0)).c_str());
				}
				else
				{
						strcpy(result, TASK_RESULT_NONE);
						strcpy(taskid, "0");
				}
			}
			else if(sql_ret==-1)
			{
					strcpy(result, TASK_SQL_ERROR);
					strcpy(taskid, "0");
			}
		}
		else if(vc_mtu.size()>0)	
		{
				strcpy(result, TASK_RESULT_SUCC);
				strcpy(taskid, itos(vc_mtu.at(0)).c_str());
		}

		//写入结果
		m_cm.Write(result, PKG_RESULTCODE_LEN, strlen(result), true,'0');

		//写入任务id
		m_cm.Write(taskid, PKG_TASKID_LEN, strlen(taskid), true,'0');

		m_cm.Seek(0, CAnsiMemFile::begin);

		int len = m_cm.GetLength()- PKG_LEN_LEN;

		//写入包长
		m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, strlen(itos(len).c_str()), true,'0');

		LogPrint(TASK_NODE,"Send->[%s]", m_cm.GetPtr());
		
		if(vc_mtu.size() > 0)
		{
			//任务锁定
			TaskLock( MULTIS_TRADECODE_UP_REQ ,vc_mtu.at(0));

			vc_mtu.erase(vc_mtu.begin());
			if(vc_mtu.size()==0)
				m_db.GetMultisSendList(vc_mtu, DB_QUEUE_LIMIT, date, session, bankno);
		}
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -1;
	}
	catch(...)
	{
    	m_cm.Write(TASK_RESULT_FAIL, PKG_RESULTCODE_LEN, strlen(TASK_RESULT_FAIL), true, '0');
		
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -2;

		return -1;
	}

	return 0;
}


/*处理任务队列
@cli_fd: socket文件描述符
#pkg_in: socket接受缓冲区
返回： 成功-0 ，失败-其他 */
int CTaskManagerServer::proc_tasks(int cli_fd,const char *pkg_in)
{
	char trade_code[PKG_TRADECODE_LEN+1];
	int cfg_date = 0, cfg_session = 0;

	memset(&trade_code,	'\0', sizeof(trade_code));

	get_pack_tradecode(pkg_in, trade_code);

	int trade = atoi(trade_code);
	memset(trade_code , '\0', sizeof(trade_code));
	sprintf(trade_code, "%d", trade);

	if(verbose)
		LogPrint(TASK_NODE , "pvsize->[%d] pmsize->[%d] pasize->[%d] svsize->[%d] smsize->[%d] sasize->[%d]", vc_vc.size(),  vc_mt.size(), vc_ai.size(), vc_vcu.size(), vc_mtu.size(), vc_aiu.size() );

	if(get_config()==-1)
	{
		LogPrint( TASK_NODE, "get_config!");
		return -1;
	}
	//获取日期及场次=====================修改日期20110127=================================
	
	if( (strlen(cfg_dic["date"].c_str())==0) || (strlen(cfg_dic["session"].c_str())==0) || (!isNumeric((char *)cfg_dic["date"].c_str())) || (!isNumeric((char *)cfg_dic["session"].c_str())) )
	{
		LogPrint( TASK_NODE, "交换日期或场次配置错误! cfg_date[%s] cfg_session[%s]", cfg_dic["date"].c_str(), cfg_dic["session"].c_str());
		return -1;
	}

	cfg_date = atoi(cfg_dic["date"].c_str());

	cfg_session = atoi(cfg_dic["session"].c_str());

	if(cfg_date==0 || cfg_session==0)
	{
		LogPrint( TASK_NODE, "交换日期或场次为0, 配置错误！cfg_date[%d] cfg_session[%d]", cfg_date, cfg_session);
		return -1;
	}
	//=========================20110127===================================================

	//1. 单票识别
	if(!strcmp(trade_code, VOUCHERS_TRADECODE_REQ))
			return proc_vouchers_task(cli_fd, cfg_date, cfg_session);

	//2. 单票上传
	else if (!strcmp(trade_code, VOUCHERS_TRADECODE_UP_REQ))
			return proc_vouchers_up_task(cli_fd, cfg_date, cfg_session);	

	//3. 信封识别
	else if (!strcmp(trade_code, MULTIS_TRADECODE_REQ))
			return proc_multis_task(cli_fd, cfg_date, cfg_session);	

	//4. 信封上传
	else if	(!strcmp(trade_code, MULTIS_TRADECODE_UP_REQ))
			return proc_multis_up_task(cli_fd, cfg_date , cfg_session);	

	//5. 委收识别
	else if (!strcmp(trade_code, AUTHINPUT_TRADECODE_REQ))
			return proc_authinput_task(cli_fd, cfg_date, cfg_session);

	//6. 委托上传
	else if (!strcmp(trade_code, AUTHINPUT_TRADECODE_UP_REQ))
			return proc_authinput_up_task(cli_fd, cfg_date, cfg_session);


	//7. 上海进账单识别
	else if (!strcmp(trade_code, CINPUT_TRADECODE_REQ))
			return proc_cinput_task(cli_fd, cfg_date, cfg_session);
	//8. 上海进账单上传
	else if (!strcmp(trade_code, CINPUT_TRADECODE_UP_REQ))
			return proc_cinput_up_task(cli_fd, cfg_date, cfg_session);

	//处理人行数据导出 导入人行清分信息到数据库
	else if	(!strcmp(trade_code, PBC_TRADECODE_EX_REQ))
	{
		int ret = proc_pbc_data(pkg_in);	

		if(ret == 0)
		{
			write(cli_fd, "000000100410110000", 18);
		}
		else 
		{
			write(cli_fd, "000000100410111111", 18);
		}
		return 	ret;
	}
	//清算人行数据导入
	else if	(!strcmp(trade_code, PBC_TRADECODE_IM_REQ))
	{
		int ret = proc_pbc_data(pkg_in);	

		if(ret == 0)
		{
			write(cli_fd, "000000100410090000", 18);
		}
		else 
		{
			write(cli_fd, "000000100410091111", 18);
		}
		return 	ret;

	}
	else if(strcmp(trade_code,PBC_CTRL_LOAD_REQ)==0)
	{
		char res[128];
		memset(res,0,sizeof(res));

		int ret = QueryTotalCtrlInfo(pkg_in);
		if(ret == 0)
		{
			write(cli_fd,"000000100420010000",18);
		}
		else
		{
			sprintf(res, "00000010042001%04d",abs(ret));
			write(cli_fd,res,18);
		}	
		
	}
	else
	{
			LogPrint(TASK_NODE , "Cann't find the package header type, error trade code[%s]", trade_code);

			close(cli_fd);

			return -1;
	}
}

/*处理消息
@cliSock : 客户端连接文件描述符
return : 成功-0 ， 失败-其他  */
int CTaskManagerServer::ProcMsg(int cliSock)
{
	int body_len;
	char	pkg_type[PKG_TYPE_LEN+1];
	char	pkg_len[PKG_LEN_LEN+1]; 
	int m_read;

	memset(pkg_type, '\0', sizeof(pkg_type));	
	memset(pkg_len, '\0', sizeof(pkg_len));	

	try
	{
		if((read( cliSock, pkg_len, PKG_LEN_LEN) != PKG_LEN_LEN))
		{
			LogPrint(TASK_NODE, "read pkg_len :%s, fd=%d", strerror(errno), cliSock);
			return -1;
		}
	
		body_len = atoi(pkg_len);

		char buf[body_len+1];
		memset(buf, '\0', sizeof(buf));

		if((m_read = read( cliSock, buf, body_len)) != body_len)
		{
			LogPrint(TASK_NODE, "read pkg_body body_len=[%d] read_len[%d]", body_len, m_read);
			return -1;
		}

		LogPrint(TASK_NODE,"Recv->[%s]", buf);

		if(get_pack_type(buf, pkg_type)!=0)
		{
			LogPrint( TASK_NODE, "get_pack_type");

			return -1;
		}
	
		//处理业务报文	
		if(!strcmp(pkg_type, PKG_USR_REQ))
		{
			if(proc_tasks(cliSock, buf)==-1)
			{
				LogPrint( TASK_NODE, "proc_tasks");
				return -1;
			}
		}

		//处理系统报文
		else if(!strcmp(pkg_type, PKG_SYS_REQ))
		{
			int ret = -1;
		
			ret = heart_response(cliSock);
			if(ret==-1)
				LogPrint(TASK_NODE,"应答错误！");
		}
		else
			LogPrint(TASK_NODE, "Cann't find the package header type, error header type[%d]", pkg_type);
	}
	catch(...)
	{
		return -1;
	}

	return 0;
}

/* 启动服务 */
int CTaskManagerServer::StartServer(int debug)
{
	verbose = debug;

	if(verbose)
	{
		LogPrint( TASK_NODE,  "./tmsrv -v");
	}
	else
	{
		LogPrint( TASK_NODE,  "./tmsrv");
		init_dameon();
	}

	printf("tmsrv verbose?%d", debug);

	printf("tmsrv version->[%s]", GSIP_VERSION);

	LogPrint( TASK_NODE,  GSIP_VERSION);

	char buf[MAXLINE];
	char cfg_name[] = "./config/config.cfg";
	ZSocket so;
	int cliaddr_len;
	struct sockaddr_in cliaddr;
	char buf_cli[50];
	SOCKET cliSock;
	int task_count = 0;

	try
	{
		memset(buf, '\0', sizeof(buf));

		//初始化参数列表	
		if(InitCfg(cfg_name))
		{
			LogPrint( TASK_NODE, "InitCfg Error!");
			LogPrint( TASK_NODE, "初始化配置文件失败!");
			return -1;
		}

		//初始化数据库信息
		if(InitDBInfo())
		{
			LogPrint( TASK_NODE, "InitDBInfo Error!");
			LogPrint( TASK_NODE, "初始化数据库信息失败!");
			return -1;
		}

				if( use_ip_acc )
		{
			//获取ip认证列表
			if(get_ip_acc_list())
			{
				LogPrint( TASK_NODE, "打开IP列表文件失败!ip_acc_lst文件是不存在!");
				return -1;
			}

			//ip认证列表为空
			if(!ip_acc_lst.size())
			{
				LogPrint( TASK_NODE, "严重警告：IP认证列表为空, 这将拒绝任何连接!!");
			}
			
			//本地服务ip不验证
			ip_acc_lst.push_back("127.0.0.1");
		}

		so.Create();

		if(so.Bind(atoi(port.c_str())))
			return -1;

		so.Listen();

		LogPrint(TASK_NODE, "\n\nServer address: %s %s\nServer running...\n",ip.c_str(), port.c_str());
	}
	catch(...)
	{
		LogPrint(TASK_NODE, "Error occured!");

		return -1;
	}

	//循环接入
	for(;;)
	{
		//任务计数器
		if(++task_count > TASK_COUNT)
		{
			task_count = 0;
			if(m_db.DbReOpen())
			{
				LogPrint(TASK_NODE, "ReOpen!");
				LogPrint(TASK_NODE, "重新打开数据库失败!进程终止!");

				return -1;
			}

			
		}

		memset( &cliaddr, 0, sizeof(cliaddr));
		memset( buf_cli, '\0', sizeof(buf_cli));

		cliaddr_len = sizeof(cliaddr);
		
	    if((cliSock = so.Accept( (struct sockaddr *)&cliaddr, cliaddr_len )) == -1)
		{
			LogPrint( TASK_NODE, "Accept connection error :%s  fd=%d", strerror(errno), (int)cliSock);
			close(cliSock);
			continue;
		}

		inet_ntop( AF_INET, &(cliaddr.sin_addr.s_addr), buf_cli, sizeof(buf_cli));

		if(verbose)
			LogPrint(TASK_NODE,"New connection->%s", buf_cli);

		//如果启用ip白名单
		if(use_ip_acc)
		{
			//如果连接ip不在ip认证列表，跳过处理流程
			if(!is_in_acc_list(buf_cli))
			{
				LogPrint(TASK_NODE,"The connection %s access fail , that is not in ip_acc_lst", buf_cli);
				LogPrint(TASK_NODE,"IP->[%s]不在IP白名单内!拒绝连接!", buf_cli);
				close((int)cliSock);
				continue;
			}
		}

		//处理连接
		if(ProcMsg((int)cliSock) == -1)
		{
			LogPrint( TASK_NODE, "ProMsg Error!");
		}

		close((int)cliSock);
	}
}
/*任务锁定
@type: 任务请求类型   VOUCHERS_TRADECODE_REQ/VOUCHERS_TRADECODE_UP_REQ/MULTIS_TRADECODE_REQ/MULTIS_TRADECODE_REQ
return :成功-0 失败-其他
**/
int CTaskManagerServer::TaskLock( char *task_type ,unsigned long sid)
{
	try
	{

		if(!strcmp(task_type, VOUCHERS_TRADECODE_REQ))
		{
			m_db.GetTaskLock("vouchers", "sid", sid, "state", VOUCHERS_STATE_DOING);
		}
		else if(!strcmp(task_type, VOUCHERS_TRADECODE_UP_REQ))
		{
			m_db.GetTaskLock("vouchers", "sid", sid, "sendstate", VOUCHERS_SENDSTATE_DOING);
		}
		//上海进帐单
		else if(!strcmp(task_type, CINPUT_TRADECODE_REQ))
		{
			m_db.GetTaskLock("cinputs", "sid", sid, "state", CINPUT_STATE_DOING);
		}
		else if(!strcmp(task_type, CINPUT_TRADECODE_UP_REQ))
		{
			m_db.GetTaskLock("cinputs", "sid", sid, "sendstate", CINPUT_SENDSTATE_DOING);
		}
		
		else if(!strcmp(task_type, MULTIS_TRADECODE_REQ))
		{
			m_db.GetTaskLock("multis", "sid", sid, "state", MULTIS_STATE_DOING);
		}
		else if(!strcmp(task_type, MULTIS_TRADECODE_UP_REQ ))
		{
			m_db.GetTaskLock("multis", "sid", sid, "sendstate", MULTIS_SENDSTATE_DOING);
		}
		else if(!strcmp(task_type, AUTHINPUT_TRADECODE_REQ))
		{
			m_db.GetTaskLock("authinput", "sid", sid, "state", AUTHINPUT_STATE_DOING); }
		else if(!strcmp(task_type, AUTHINPUT_TRADECODE_UP_REQ ))
		{
			int ret;
			char sids[50];
			struct tb_authinput_s s;

			sprintf(sids, "%ld", sid);

			if( (ret = m_db.GetAuthInputRowBySid(sids, &s)) != 0 )
			{
				LogPrint(AUTHINPUT_SEND_NODE, "GetAuthinput error[%d]", ret);
				return -1;
			}

			if(s.sendstate==0)
				m_db.GetTaskLock("authinput", "sid", sid, "sendstate", AUTHINPUT_SENDSTATE_DOING);
			else if(s.sendstate==6)
				m_db.GetTaskLock("authinput", "sid", sid, "sendstate", AUTHINPUT_SENDSTATE_BILL_DOING);
		}
		else
		{
			return -1;
		}
		}
		catch(...)
		{ 
			return -1;
		}

	return 0;
}

char * CTaskManagerServer::GetNow()
{
	dt.Now();
	memset(m_now, '\0', sizeof(m_now));
	sprintf(m_now, "%s %s", dt.BankDateString().c_str(), dt.TimeString().c_str());

	return m_now;
}

/*获取ip认证列表
**/
int CTaskManagerServer::get_ip_acc_list()
{
	FILE * fp;
	char tmp[128];

	if((fp = fopen( IP_ACC_LIST, "r"))==NULL)
	{
		LogPrint( TASK_NODE, "get_ip_acc_list : %s", strerror(errno));
		return -1;
	}

	while( fgets(tmp, sizeof(tmp), fp) != NULL)
	{
		tmp[strlen(tmp)-1] = '\0';
		ip_acc_lst.push_back(tmp);
		bzero(tmp, sizeof(tmp));
	}

	//打印ip认证列表
	LogPrint( TASK_NODE, "IP access list :");

	for(int i=0; i< ip_acc_lst.size(); i++)
	{
		LogPrint( TASK_NODE, "access lst[%d]->[%s]", i, (ip_acc_lst.at(i)).c_str());
	}

	return 0;
}

/*验证ip是否在认证列表中
@return 在列表中-返回1, 不在列表中-返回0
**/
int CTaskManagerServer::is_in_acc_list(char *ip)
{
	int i;

	if(!strlen(ip))
		return -1;

	for( i=0; i< ip_acc_lst.size(); i++)
	{
		//LogPrint( TASK_NODE, "ip->[%s] lst[%d]->[%s]", ip, i, (ip_acc_lst.at(i)).c_str());
		if(!strcmp( ip, (ip_acc_lst.at(i)).c_str()))	
			return 1;
	}		

	return 0;
}

//委收识别
int CTaskManagerServer::proc_authinput_task( int fd, int date, int session)
{
	CAnsiMemFile m_cm;

	char result[PKG_RESULTCODE_LEN+1];
	char taskid[PKG_TASKID_LEN+1];
	memset(result, '\0', sizeof(result));
	memset(taskid, '\0', sizeof(taskid));

	m_cm.Write(" ", PKG_LEN_LEN ,1);
	//写入类型
	m_cm.Write(PKG_USR_RES, PKG_TYPE_LEN, strlen(PKG_USR_RES), true, '0');
	//写入交易码
  	m_cm.Write(AUTHINPUT_TRADECODE_RES, PKG_TRADECODE_LEN, strlen(AUTHINPUT_TRADECODE_RES), true, '0');

	try
	{
		if(vc_ai.size()==0)
		{
			int sql_ret = m_db.GetAuthInputParseList(vc_ai, DB_QUEUE_LIMIT, date, session);
			
			if(sql_ret == 0)
			{
				if(vc_ai.size()>0)
				{
					strcpy(result, TASK_RESULT_SUCC);
					strcpy(taskid, itos(vc_ai.at(0)).c_str());
				}
				else
				{
					strcpy(result, TASK_RESULT_NONE);
					strcpy(taskid, "0");
				}
			}
			else if(sql_ret==-1)
			{
				strcpy(result, TASK_SQL_ERROR);
				strcpy(taskid, "0");
			}
		}
		else if(vc_ai.size()>0)	
		{
				strcpy(result, TASK_RESULT_SUCC);
				strcpy(taskid, itos(vc_ai.at(0)).c_str());
		}

		//写入结果
		m_cm.Write(result , PKG_RESULTCODE_LEN, strlen(result),  true,'0');

		//写入任务ID
		m_cm.Write(taskid , PKG_TASKID_LEN, strlen(taskid),  true,'0');

		int len = m_cm.GetLength()- PKG_LEN_LEN;

		m_cm.Seek(0, CAnsiMemFile::begin);

		m_cm.Write( itos(len).c_str(), PKG_LEN_LEN, strlen(itos(len).c_str()), true, '0' );

		LogPrint( TASK_NODE,"Send->[%s]", m_cm.GetPtr());
		
		if(vc_ai.size() > 0)
		{

			//任务锁定
			TaskLock( AUTHINPUT_TRADECODE_REQ, vc_ai.at(0));

			vc_ai.erase(vc_ai.begin());
			if(vc_ai.size()==0)
				m_db.GetAuthInputParseList(vc_ai, DB_QUEUE_LIMIT, date, session);
		}

		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -1;
	}
	catch(...)
	{
    		m_cm.Write(TASK_RESULT_FAIL, PKG_RESULTCODE_LEN, strlen(TASK_RESULT_FAIL), true, '0');
		
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -2;

		return -1;
	}

	return 0;
}

//读配置表
int CTaskManagerServer::get_config()
{
	int ret;

	cfg_dic.clear();
	if((ret = m_db.get_tb_config(cfg_dic))!=0)
    {
         LogPrint( VOUCHER_NODE,"get_tb_config error: %s, return %d", strerror(errno), ret);
         return -1;
    }

    return 0;
}

//处理委收上传任务
int CTaskManagerServer::proc_authinput_up_task( int fd, int date, int session)
{
	CAnsiMemFile m_cm;

	char result[PKG_RESULTCODE_LEN+1];
	char taskid[PKG_TASKID_LEN+1];
	memset(result, '\0', sizeof(result));
	memset(taskid, '\0', sizeof(taskid));

	m_cm.Write(" ", PKG_LEN_LEN ,1);
	m_cm.Write(PKG_USR_RES, PKG_TYPE_LEN, strlen(PKG_USR_RES), true, '0');
    m_cm.Write(AUTHINPUT_TRADECODE_UP_RES, PKG_TRADECODE_LEN, strlen(AUTHINPUT_TRADECODE_UP_RES), true, '0');

	try
	{
		if(vc_aiu.size()==0)
		{
			int sql_ret = m_db.GetAuthInputSendList(vc_aiu, DB_QUEUE_LIMIT, date, session);

			if(sql_ret == 0)
			{
				if(vc_aiu.size()>0)
				{
						strcpy(result, TASK_RESULT_SUCC);
						strcpy(taskid, itos(vc_aiu.at(0)).c_str());
				}
				else
				{
						strcpy(result, TASK_RESULT_NONE);
						strcpy(taskid, "0");
				}
			}
			else if(sql_ret==-1)
			{
					strcpy(result, TASK_SQL_ERROR);
					strcpy(taskid, "0");
			}
		}
		else if(vc_aiu.size()>0)	
		{
				strcpy(result, TASK_RESULT_SUCC);
				strcpy(taskid, itos(vc_aiu.at(0)).c_str());
		}

		//写入结果
		m_cm.Write(result, PKG_RESULTCODE_LEN, strlen(result), true,'0');

		//写入任务id
		m_cm.Write(taskid, PKG_TASKID_LEN, strlen(taskid), true,'0');

		m_cm.Seek(0, CAnsiMemFile::begin);

		int len = m_cm.GetLength()- PKG_LEN_LEN;

		//写入包长
		m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, strlen(itos(len).c_str()), true,'0');

		LogPrint(TASK_NODE,"Send->[%s]", m_cm.GetPtr());
		
		if(vc_aiu.size() > 0)
		{
			//任务锁定
			TaskLock( AUTHINPUT_TRADECODE_UP_REQ ,vc_aiu.at(0));

			vc_aiu.erase(vc_aiu.begin());
			if(vc_aiu.size()==0)
				m_db.GetAuthInputSendList(vc_aiu, DB_QUEUE_LIMIT, date, session);
		}
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -1;
	}
	catch(...)
	{
    	m_cm.Write(TASK_RESULT_FAIL, PKG_RESULTCODE_LEN, strlen(TASK_RESULT_FAIL), true, '0');
		
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -2;

		return -1;
	}

	return 0;
}

int CTaskManagerServer::QueryTotalCtrlInfo(const char*req)
{
	char session[4];
	char date[12];
	char area[8];

	memset(date,0,sizeof(date));
	memset(session,0,sizeof(session));
	memset(area,0,sizeof(area));
	
	strncpy(date,req+6,8);
	strncpy(area,req+14,6);
	strncpy(session,req+20,3);
	
	string fn = string(pbc_qs_mx_path)+"exchq"+date+string(session)+"2";

	LogPrint(TASK_NODE,"[%s][%s][%s][%s]",date,session,area,req);

    	if( !ZDoesFileExist(fn.c_str()) )
	 {

		LogPrint(TASK_NODE,"清算信息文件[%s]不存在",fn.c_str());
		return -1001;
    	}

    	int ret = 0;
    	ifstream file(fn.c_str());
    	string buf;

    	while( true ) 
	{
        	file >> buf;
        	if( file.eof() ) break;

        	char enstr[128];
        	strcpy( enstr,buf.c_str());
       	 	char ostr[128];
        	bzero(ostr,sizeof(ostr));
        	int r = UnEncrypt(ostr,enstr);
        	if( r ) 
        	{
           		LogPrint(TASK_NODE, "UnEncrypt FAILED!");
            		continue;
        	}	

        	buf = ostr;

        	if( buf.length() < 32 ) 
        	{
            		continue;
        	}

        	string nets =  buf.substr(6,6).c_str();
        	string dramt = buf.substr(12,15);
        	string cramt = buf.substr(27,15).c_str();
       
		ret = m_db.SaveCtrl(date,session,area,nets.c_str(),dramt.c_str(),cramt.c_str(),0);
		if(ret)
		{
	    		return -1002;
			file.close();
		}
	
    	}
    
	file.close();

   	LogPrint(TASK_NODE,"get ctrls from file finished");

   
   	string fnok = fn+".ok";
   	unlink( fnok.c_str() );
   	rename(fn.c_str(),fnok.c_str());
	return 0;
}

//处理上海进账单识别
int CTaskManagerServer::proc_cinput_task( int fd, int date, int session)
{
	CAnsiMemFile m_cm;

	char result[PKG_RESULTCODE_LEN+1];
	char taskid[PKG_TASKID_LEN+1];

	memset(result, '\0', sizeof(result));
	memset(taskid, '\0', sizeof(taskid));

	//预留报文长度 
	m_cm.Write(" ", PKG_LEN_LEN ,1);

	//写入报文类型
	m_cm.Write(PKG_USR_RES, PKG_TYPE_LEN, strlen(PKG_USR_RES), true, '0');

	//写入交易码
  	m_cm.Write(CINPUT_TRADECODE_RES, PKG_TRADECODE_LEN, strlen(CINPUT_TRADECODE_RES), true, '0');

	int sql_ret = 0;
	try
	{
		//如果当前队列为空,查询数据库获取任务列表
		if(vc_ci.size()==0)
		{
			int sql_ret = m_db.GetCinputSIDSList(vc_ci, DB_QUEUE_LIMIT, date, session, bankno);

			//查询后队列仍为空
			if(sql_ret == 0)
			{
				if(vc_ci.size()>0)
				{
					strcpy(result, TASK_RESULT_SUCC);
					strcpy(taskid, itos(vc_ci.at(0)).c_str());
				}
				else
				{
					strcpy(result, TASK_RESULT_NONE);
					strcpy(taskid, "0");	
				}
			}
			//查询数据库错误
			else if(sql_ret==-1)
			{
				strcpy(result, TASK_SQL_ERROR);
				strcpy(taskid, "0");
			}
		}
		//如果当前队列不为空
		else if(vc_ci.size()>0)	
		{
				strcpy(result, TASK_RESULT_SUCC);
				strcpy(taskid, itos(vc_ci.at(0)).c_str());
		}

		//写入result
  		m_cm.Write(result, PKG_RESULTCODE_LEN, strlen(result), true, '0');

		//写入taskid
  		m_cm.Write(taskid, PKG_TASKID_LEN, strlen(taskid), true, '0');

		//报文体长度
		int len = m_cm.GetLength()- PKG_LEN_LEN;

		m_cm.Seek(0, CAnsiMemFile::begin);

		//写入报文体长度
		m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, strlen(itos(len).c_str()), true,'0');

		LogPrint(TASK_NODE , "Send->[%s]", m_cm.GetPtr());
		
		if(vc_ci.size() > 0)
		{

			//任务锁定
			TaskLock( CINPUT_TRADECODE_REQ, vc_ci.at(0));

			vc_ci.erase(vc_ci.begin());
			if(vc_ci.size()==0)
				m_db.GetCinputSIDSList(vc_ci, DB_QUEUE_LIMIT, date, session, bankno);
		}

		//发送任务
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -1;
	}
	catch(...)
	{
    	m_cm.Write(TASK_RESULT_FAIL, PKG_RESULTCODE_LEN, strlen(TASK_RESULT_FAIL), true, '0');
		
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -2;

		return -1;
	}

	return 0;
}

/*处理上海进账单上传
@fd: 文件描述符
返回： 成功-0 ， 失败-其他*/
int CTaskManagerServer::proc_cinput_up_task( int fd, int date, int session)
{
	CAnsiMemFile m_cm;

	m_cm.Write(" ", PKG_LEN_LEN ,1);
	m_cm.Write(PKG_USR_RES, PKG_TYPE_LEN, strlen(PKG_USR_RES), true, '0');
	m_cm.Write(CINPUT_TRADECODE_UP_RES, PKG_TRADECODE_LEN, strlen(CINPUT_TRADECODE_UP_RES), true, '0');

	char result[PKG_RESULTCODE_LEN+1];
	char taskid[PKG_TASKID_LEN+1];

	memset(result, '\0', sizeof(result));
	memset(taskid, '\0', sizeof(taskid));

	try
	{
		if(vc_ciu.size()==0)
		{
			int sql_ret = m_db.GetCinputSendList(vc_ciu, DB_QUEUE_LIMIT, date, session, bankno);

			if(sql_ret == 0)
			{
				if(vc_ciu.size()>0)
				{
					strcpy(result, TASK_RESULT_SUCC);
					strcpy(taskid, itos(vc_ciu.at(0)).c_str());
				}
				else
				{
					strcpy(result, TASK_RESULT_NONE);
					strcpy(taskid, "0");
				}
			}
			else if(sql_ret==-1)
			{
					strcpy(result, TASK_RESULT_FAIL);
					strcpy(taskid, "0");
			}
		}
		else if(vc_ciu.size()>0)	
		{
			strcpy(result, TASK_RESULT_SUCC);
			strcpy(taskid, itos(vc_ciu.at(0)).c_str());
		}

		//写入result

  		m_cm.Write(result, PKG_RESULTCODE_LEN, strlen(result), true, '0');

		//写入taskid
  		m_cm.Write(taskid, PKG_TASKID_LEN, strlen(taskid), true, '0');
		
		m_cm.Seek(0, CAnsiMemFile::begin);

		int len = m_cm.GetLength()- PKG_LEN_LEN;

		//写如包长
		m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, strlen(itos(len).c_str()), true,'0');

		LogPrint(TASK_NODE,"Send->[%s]", m_cm.GetPtr());
		
		if(vc_ciu.size() > 0)
		{

			//任务锁定
			TaskLock(  CINPUT_TRADECODE_UP_REQ, vc_ciu.at(0)); 
			vc_ciu.erase(vc_ciu.begin());
			if(vc_ciu.size()==0)
				m_db.GetCinputSendList(vc_ciu, DB_QUEUE_LIMIT, date, session, bankno);
		}

		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -1;
	}
	catch(...)
	{
    	m_cm.Write(TASK_RESULT_FAIL, PKG_RESULTCODE_LEN, strlen(TASK_RESULT_FAIL), true, '0');
		
		if(write(fd, (void *)m_cm.GetPtr(), m_cm.GetLength())!= m_cm.GetLength())
			return -2;

		return -1;
	}

	return 0;
}

