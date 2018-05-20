#ifndef _LINUX_TASK_MANAGER_H
#define _LINUX_TASK_MANAGER_H

#include <zsocket.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <db_common.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include <package.h>
#include <myplus.h>
#include <ConfigFile.h>
#include <gsip_util.h>
#include <AnsiMemfile.h>
#include <mem.h>
#include <ZTime.hpp>
#include "PbcFile.h"

#define DB_QUEUE_LIMIT 100

//任务调度类
class CTaskManagerServer
{
public:
	CTaskManagerServer();
	~CTaskManagerServer();

	//初始化数据库
	int InitDBInfo();

	//读取配置表
	int get_config();

	//启动服务
	int StartServer(int verbose);

	//消息处理 
	int ProcMsg(int cliSock);

	//处理单票识别任务
	int proc_vouchers_task(int fd, int date, int session);

	//处理单票上传任务
	int proc_vouchers_up_task(int fd, int date, int session);

	//处理信封识别任务
	int proc_multis_task(int fd, int date, int session);

	//处理信封上传任务
	int proc_multis_up_task(int fd, int date, int session);

	//处理委收识别任务
	int proc_authinput_task( int fd, int date, int session);

	//处理委收上传任务
	int proc_authinput_up_task( int fd, int date, int session);

	//任务处理
	int proc_tasks( int fd, const char *pkg_in);

	//锁定任务
	int TaskLock( char *task_type, unsigned long sid );

	//获取当前时间
	char * GetNow();

	//数据库操作对象
	CDBCommon m_db;

	//标准内存文件对象
	CAnsiMemFile m_cm;

	//单票识别队列
	vector<unsigned long> vc_vc;

	//单票上传队列
	vector<unsigned long> vc_vcu;

	//信封识别队列
	vector<unsigned long> vc_mt;

	//信封上传队列
	vector<unsigned long> vc_mtu;

	//委收识别队列
	vector<unsigned long> vc_ai;

	//委收上传队列
	vector<unsigned long> vc_aiu;

	//ip认证列表
	vector<string> ip_acc_lst;

	//获取ip列表	
	int get_ip_acc_list();

	//对ip进行认证	
	int is_in_acc_list(char *ip);

	//初始化配置文件信息	 
	int InitCfg(char *cfg_name);

public:	
	ZDate dt;	
	StrDict  cfg_dic;
	char m_now[50];
	string brno;
	string db_host;
	string db_uid;
	string db_pwd;
	string db_dbAlias;
	string pbc_qs_mx_path;
	string ip;
	string port;
	int use_ip_acc;
};

#endif
