#include "authinput.h"
#include <errno.h>
#include <gsipcom.h>
#include <transimg.h>
#include <sstream>
#include <PubFun.h>

#define MAXLINE 1024

#define TASK_COUNT 1000

using namespace std;

CAuthInput::CAuthInput(){}

CAuthInput::~CAuthInput(){}

int verbose;

static int task_count = 0;
//PM配置文件信息
#define pm_cfg "./config/config.cfg"

//配置文件全局变量
string  brno, ip, port, db_host, db_uid, db_pwd, db_dbAlias, recog_cfg, checkOCR_cfg;

string  auth_rate,  env_rate,  count_rate,  notax_rate;

//启动服务
int CAuthInput::StartServer(int debug)
{
	verbose = debug;

	if(verbose)
	{
		LogPrint( AUTHINPUT_NODE, "./pasrv -v");
	}
	else
	{
		LogPrint( AUTHINPUT_NODE, "./pasrv");
		init_dameon();
	}

	printf("pasrv verbose?%d\n", verbose);

	ZSocket so;
	MemT pRGB;

	//打印图像库版本
	char ppl_version[128];
	TilGetPatternRecogVersion(ppl_version);

	LogPrint( AUTHINPUT_NODE, "Progress verssion %s\nppl_version %s\n", GSIP_VERSION, ppl_version);
	printf( "pasrv %s\nppl_version %s\n", GSIP_VERSION, ppl_version);

	char cfg_name[] = "./config/config.cfg";
	char bj_bank[] = "./config/bj_bank.cfg";
	char pkg_len[PKG_LEN_LEN+1], cm_code[512];
	char img_name[80];
	struct pkg_authinput_res_s authinput_res;
	struct tb_authinput_s tb_authinput;
	int sizeX = 0, sizeY = 0, pannel = 0, model = 0;
	int m_pbcfile = 0;
	int ret = 0;

	//bill_type 1、2-委收; 3-非税
	int bill_type = 0;

	try
	{
		LogPrint( AUTHINPUT_NODE,"Parseauthinput running ...\n");

		if(InitCfg(cfg_name))
		{
			LogPrint( AUTHINPUT_NODE,"InitCfg\n");
			LogPrint( AUTHINPUT_NODE,"读取配置文件错误!\n");
			return -1;
		}
		
		if(InitDBInfo())
		{
			LogPrint( AUTHINPUT_NODE,"InitDBInfo\n");
			LogPrint( AUTHINPUT_NODE,"连接数据库失败!\n");
			return -1;
		}

		for(;;)
		{
			//任务调度计数器
			if(task_count++ > TASK_COUNT)
				task_count = 0;
	
			memset(&authinput_res, 0, sizeof(authinput_res));

			if( (ret = get_task(ip, port, &authinput_res))!= 0 )
			{
				if(ret == CONN_SERVER_ERR)
				{
					LogPrint( AUTHINPUT_NODE,"Cann't connect to %s %d\n", task_ip, task_port);
					sleep(5);
					continue;
				}
				else
				{ 
					LogPrint( AUTHINPUT_NODE,"get_task\n");
					LogPrint( AUTHINPUT_NODE,"获取任务失败!\n");
					sleep(5);
					continue;
				}
			}

			memset(&tb_authinput, 0, sizeof(tb_authinput));
		
			if(ret = get_msg( authinput_res.task, &tb_authinput) != 0)
			{
				if(ret == SQL_NO_RESULT)
				{	
		 			//LogPrint( AUTHINPUT_NODE,"任务队列空!\n");
					sleep(3);
					continue;
				}
				else if(ret == -1)
				{
					LogPrint( AUTHINPUT_NODE,"get_msg\n");
					LogPrint( AUTHINPUT_NODE,"获取报文体错误!\n");
					continue;
				}
			}

			LogPrint( AUTHINPUT_NODE,"获取任务成功!\n" );

			//获取图像	
			memset(img_name, '\0', sizeof(img_name));

			if(get_img_name(tb_authinput.fname, tb_authinput.date, tb_authinput.session, tb_authinput.pkgno,img_name )!=0)
			{
				LogPrint( AUTHINPUT_NODE,"get_img_name! 获取图像名错误!\n");
				sleep(5);
				continue;
			}
	
			//LogPrint( AUTHINPUT_NODE,"img_name=[%s]\n", img_name);
			if(get_img(img_name, &tb_authinput, pRGB, sizeX, sizeY, pannel)!=0)
			{
				LogPrint( AUTHINPUT_NODE,"get_img! 取图失败! img_name=[%s] ip=[%s] fname->[%s]\n", img_name, tb_authinput.ip, tb_authinput.fname);
				update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_FAIL);
				sleep(5);
				continue;	
			}

			LogPrint( AUTHINPUT_NODE,"取图成功! sizeX->[%d] sizeY->[%d]\n", sizeX, sizeY );

			memset(cm_code, '\0', sizeof(cm_code));	

			
			//根据sizeX, sizeY判断是信封还是票据。如果是信封用老库识别，返回磁码，版式; 如果是票据用新算法识别，返回金额+正反面

			//判断票据类型
			//bill_type 1、2-委收; 3-非税
			bill_type = tb_authinput.multiflag;

			//非税识别	
			if( bill_type==3)
			{
				//保存金额及正反面及状态
				LogPrint( AUTHINPUT_NODE, "处理非税...\n");

				double nt_amount = 0;

				// 因为非税是蓝色的，所以mainflag返回2, 这里应该为1
				ret = Til_SegmentPrnChnChar(pRGB.data(), sizeX, sizeY, pannel, tb_authinput.dpi, VOUCHTYPE_FEISHUI, nt_amount, (int &)tb_authinput.mainflag);
				//处理非税业务(信封+票据)
				if(ret!=0)
				{
					//打印识别失败结果
					Prt_Til_SegmentPrnChnCharRet(ret);

					LogPrint( AUTHINPUT_NODE,"正在处理非税信封...\n");
					//版面识别 获取磁码
					if( (ret =rec_model(pRGB.data(), sizeX, sizeY, pannel, tb_authinput.dpi, cm_code , model))!=0)
					{
						LogPrint( AUTHINPUT_NODE,"信封识别失败! cm_code->[%s] model->[%d] ret->[%d]\n", cm_code, model, ret);
						LogPrint( AUTHINPUT_NODE,"rec_mode\n");
			
						update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
						//在数据库标识信封
						m_db.UpColumnByPK_1( "sid", tb_authinput.sid , "authinput", "envid", "sid", 0);

						usleep(200);

						continue;
					}

					if( model!=2 && model!=6 && model!=7 )
					{
						LogPrint( AUTHINPUT_NODE,"OCR识别失败, 未知版式! codeline->[%s], model->[%d]\n", cm_code, model);			
						update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
						continue;
					}

					LogPrint( AUTHINPUT_NODE,"OCR识别成功! codeline->[%s], model->[%d]\n", cm_code, model);			

					memset(m_accno, '\0', sizeof(m_accno));
					memset(m_exchno, '\0', sizeof(m_exchno));
					memset(m_vchno, '\0', sizeof(m_vchno));
					memset(m_vchtype, '\0', sizeof(m_vchtype));
					memset(m_amount, '\0', sizeof(m_amount));

					if(spart_code(cm_code)!=0)
					{
						LogPrint( AUTHINPUT_NODE,"spart_code!\n");
						LogPrint( AUTHINPUT_NODE,"信封磁码切割失败! codeline=[%s]\n", cm_code);
						update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
						//在数据库标识信封
						m_db.UpColumnByPK_1( "sid", tb_authinput.sid , "authinput", "envid", "sid", 0);

						usleep(200);
						continue;
					}

					LogPrint( AUTHINPUT_NODE,"切割磁码成功!\n");
					LogPrint( AUTHINPUT_NODE,"codeline->[%s] \nm_vchno->[%s] \nm_exchno->[%s] \nm_accno->[%s] \nm_vchtype->[%s] \nm_amount->[%s]\n", cm_code, m_vchno, m_exchno, m_accno, m_vchtype, m_amount);
					
					//保存状态及识别信息 
					if(save_msg_state(tb_authinput.sid, m_accno, m_vchno, m_exchno, m_vchtype, m_amount, model, AUTHINPUT_STATE_SUCC, 0, tb_authinput.mainflag )!=0)
					{
						LogPrint( AUTHINPUT_NODE,"save_msg_state\n");
						LogPrint( AUTHINPUT_NODE,"保存状态及识别信息失败!\n");
						continue;	
					}
					LogPrint( AUTHINPUT_NODE,"任务[%d]完成!\n\n", tb_authinput.sid);
				}
				//处理非税票据
				else
				{	
					tb_authinput.mainflag=1;

					sprintf( m_amount, "%f", nt_amount);

					if( nt_amount==0 )
					{
						LogPrint( AUTHINPUT_NODE, "非税识别缺失金额! nt_amount->[%s] mainflag->[%d]\n", m_amount ,tb_authinput.mainflag);
						update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
				
						continue;
					}

					if( tb_authinput.mainflag==0 )
					{
						LogPrint( AUTHINPUT_NODE, "非税识别缺失主副件! nt_amount->[%f] mainflag->[%s]\n", nt_amount, tb_authinput.mainflag );
					update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
					continue;
					}

					LogPrint( AUTHINPUT_NODE, "非税OCR识别! m_amount->[%s] mainflag->[%d]\n", m_amount, tb_authinput.mainflag );

					if(save_msg_state(tb_authinput.sid, m_accno, m_vchno, m_exchno, m_vchtype, m_amount, model, AUTHINPUT_STATE_SUCC, 1, tb_authinput.mainflag )!=0)
					{
						LogPrint( AUTHINPUT_NODE,"save_msg_state\n");
						LogPrint( AUTHINPUT_NODE,"保存状态及识别信息失败!\n");
						continue;	
					}
					LogPrint( AUTHINPUT_NODE,"任务[%d]完成!\n\n", tb_authinput.sid);
				}
			}		
			//处理委收业务
			else if(bill_type==1 || bill_type==2)
			{
				//保存金额及正反面及状态
				LogPrint( AUTHINPUT_NODE, "处理主副委收...\n");

				double ws_amount = 0;
				tb_authinput.mainflag=0;

				ret = Til_SegmentPrnChnChar(pRGB.data(), sizeX, sizeY, pannel, tb_authinput.dpi, VOUCHTYPE_WEISHOU, ws_amount, (int &)tb_authinput.mainflag);

				//如果委收处理失败，继续按信封票据处理
				LogPrint( AUTHINPUT_NODE, "ret->[%d] ws_amount->[%f], tb_authinput.mainflag->[%d]\n", ret,  ws_amount, tb_authinput.mainflag);

				if(ret!=0)
				{
					//打印失败结果
					Prt_Til_SegmentPrnChnCharRet(ret);

					LogPrint( AUTHINPUT_NODE,"正在处理信封...\n");
					//版面识别 获取磁码
					if( (ret =rec_model(pRGB.data(), sizeX, sizeY, pannel, tb_authinput.dpi, cm_code , model))!=0)
					{
						LogPrint( AUTHINPUT_NODE,"信封识别失败! cm_code->[%s] model->[%d] ret->[%d]\n", cm_code, model, ret);
						LogPrint( AUTHINPUT_NODE,"rec_mode\n");
			
						update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
						//在数据库标识信封
						m_db.UpColumnByPK_1( "sid", tb_authinput.sid , "authinput", "envid", "sid", 0);

						usleep(200);

						continue;
					}

					if( model!=2 && model!=6 && model!=7 )
					{
						LogPrint( AUTHINPUT_NODE,"OCR识别失败, 未知版式! codeline->[%s], model->[%d]\n", cm_code, model);			
						update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
						continue;
					}
					else
					{
						LogPrint( AUTHINPUT_NODE,"OCR识别成功! codeline->[%s], model->[%d]\n", cm_code, model);			
					}
					
					memset(m_accno, '\0', sizeof(m_accno));
					memset(m_exchno, '\0', sizeof(m_exchno));
					memset(m_vchno, '\0', sizeof(m_vchno));
					memset(m_vchtype, '\0', sizeof(m_vchtype));
					memset(m_amount, '\0', sizeof(m_amount));

					if(spart_code(cm_code)!=0)
					{
						LogPrint( AUTHINPUT_NODE,"spart_code!\n");
						LogPrint( AUTHINPUT_NODE,"信封磁码切割失败! codeline=[%s]\n", cm_code);
						update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
						//在数据库标识信封
						m_db.UpColumnByPK_1( "sid", tb_authinput.sid , "authinput", "envid", "sid", 0);

						usleep(200);
						continue;
					}

					LogPrint( AUTHINPUT_NODE,"切割磁码成功!\n");
					LogPrint( AUTHINPUT_NODE,"codeline->[%s] \nm_vchno->[%s] \nm_exchno->[%s] \nm_accno->[%s] \nm_vchtype->[%s] \nm_amount->[%s]\n", cm_code, m_vchno, m_exchno, m_accno, m_vchtype, m_amount);
					
					//保存状态及识别信息 
					if(save_msg_state(tb_authinput.sid, m_accno, m_vchno, m_exchno, m_vchtype, m_amount, model, AUTHINPUT_STATE_SUCC, 0, tb_authinput.mainflag )!=0)
					{
						LogPrint( AUTHINPUT_NODE,"save_msg_state\n");
						LogPrint( AUTHINPUT_NODE,"保存状态及识别信息失败!\n");
						continue;	
					}
					LogPrint( AUTHINPUT_NODE,"任务[%d]完成!\n\n", tb_authinput.sid);
				}
				//按委收票据处理
				else
				{
					sprintf( m_amount, "%.2f", ws_amount);

					if( ws_amount==0 )
					{
						LogPrint( AUTHINPUT_NODE, "委收识别缺失金额! ws_amount->[%s] mainflag->[%d]\n", m_amount ,tb_authinput.mainflag);
						update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
				
						continue;
					}

					if( tb_authinput.mainflag==0 )
					{
						LogPrint( AUTHINPUT_NODE, "委收识别缺失主附件! ws_amount->[%f] mainflag->[%s]\n", ws_amount, tb_authinput.mainflag );
						update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
						continue;
					}

					LogPrint( AUTHINPUT_NODE, "委收OCR识别! m_amount->[%s] mainflag->[%d]\n", m_amount, tb_authinput.mainflag );

					if(ret!=0)
					{
						LogPrint( AUTHINPUT_NODE,"Til_SegmentPrnChnChar\n");
						LogPrint( AUTHINPUT_NODE,"识别委托失败!\n");
						update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_REC_FAIL);
						continue;	
					}

					if(save_msg_state(tb_authinput.sid, m_accno, m_vchno, m_exchno, m_vchtype, m_amount, model, AUTHINPUT_STATE_SUCC, 1, tb_authinput.mainflag )!=0)
					{
						LogPrint( AUTHINPUT_NODE,"save_msg_state\n");
						LogPrint( AUTHINPUT_NODE,"保存状态及识别信息失败!\n");
						continue;	
					}
					LogPrint( AUTHINPUT_NODE,"任务[%d]完成!\n\n", tb_authinput.sid);
				}

			}
			else 
			{
				LogPrint( AUTHINPUT_NODE, "无法区分的票据类型!不做版面分析!");
				update_authinput_state(tb_authinput.sid, "state", AUTHINPUT_STATE_FAIL);
				continue;
			}
		}
	}
	catch(...)
	{
		LogPrint( AUTHINPUT_NODE,"Error occured: %s!\n", strerror(errno));
		return -1;
	}
}

//初始化配置文件
int  CAuthInput::InitCfg(char *cfg_name)
{
	try
	{
		string tmp;

		Config cfg(cfg_name);

		ip = cfg.value("task_ip",1);
		if(ip.size() <= 0)
		{
			LogPrint( AUTHINPUT_NODE, "配置错误: task_ip不能为空!\n");
			printf("配置错误： task_ip不能为空!\n");
			return -1;
		}

		port = cfg.value("task_port",1);
		if(port.size() <= 0)
		{
			LogPrint( AUTHINPUT_NODE, "配置错误: task_port不能为空!\n");
			printf("配置错误： task_port不能为空!\n");
			return -1;
		}

		strcpy(task_ip ,ip.c_str());
		task_port = atoi(port.c_str());

		db_host = cfg.value("db_host",1);
		if(db_host.size() <= 0)
		{
			LogPrint( AUTHINPUT_NODE, "配置错误: db_host不能为空!\n");
			printf("配置错误： db_host不能为空!\n");
			return -1;
		}

		db_uid = cfg.value("db_uid",1);
		if(db_uid.size() <= 0)
		{
			LogPrint( AUTHINPUT_NODE, "配置错误: db_uid不能为空!\n");
			printf("配置错误： db_uid不能为空!\n");
			return -1;
		}

		db_pwd = cfg.value("db_pwd",1);
		if(db_pwd.size() <= 0)
		{
			LogPrint( AUTHINPUT_NODE, "配置错误: db_pwd不能为空!\n");
			printf("配置错误： db_pwd不能为空!\n");
			return -1;
		}

		db_dbAlias = cfg.value("db_dbAlias",1);
		if(db_dbAlias.size() <= 0)
		{
			LogPrint( AUTHINPUT_NODE, "配置错误: db_dbAlias不能为空!\n");
			printf("配置错误： db_dbAlias不能为空!\n");
			return -1;
		}

		brno  = cfg.value("brno", 1);    
		if(brno.size() <= 0)
		{
			LogPrint( AUTHINPUT_NODE, "配置错误: brno不能为空!\n");
			printf("配置错误： brno不能为空!\n");
			return -1;
		}
		else if(brno.size() > 6)
		{
			LogPrint( AUTHINPUT_NODE, "配置错误: brno长度超长!\n");
			printf("配置错误： brno长度超长!\n");
			return -1;
		}

        Config mdl("./config/model.cfg");

        recog_cfg = mdl.value(brno.c_str(), "recog_cfg", 1);
		if(recog_cfg.size() <= 0)
		{
			LogPrint( AUTHINPUT_NODE, "配置错误: recog_cfg不能为空!\n");
			printf("配置错误： recog_cfg不能为空!\n");
			return -1;
		}
	

        checkOCR_cfg = mdl.value(brno.c_str(), "checkOCR_cfg", 1);
		if(checkOCR_cfg.size() <= 0)
		{
			LogPrint( AUTHINPUT_NODE, "配置错误: checkOCR_cfg不能为空!\n");
			printf("配置错误： checkOCR_cfg不能为空!\n");
			return -1;
		}

        LogPrint( AUTHINPUT_NODE, "brno->[%s]\nrecog_cfg->[%s]\ncheckOCR_cfg->[%s]\n", brno.c_str(), recog_cfg.c_str(), checkOCR_cfg.c_str());
	}
	catch(...)
	{
		LogPrint( AUTHINPUT_NODE,"read config error...\n");
		return -1;
	}	
	return 0;
}

//初始化数据库信息
int  CAuthInput::InitDBInfo()
{
	return m_db.DbOpen(db_host.c_str(), db_uid.c_str(), db_pwd.c_str(), db_dbAlias.c_str());
}

/*接收包拆包
@pkg_recv: 接收包
@pkg_body: 接收报文报文体
return : 成功-0， 失败-其他 
*/
int CAuthInput::pkg_recv_resolve(char *pkg_recv,struct pkg_authinput_res_s *p)
{
	int count = 0;
	try
	{
		strncpy(p->type, pkg_recv, PKG_TYPE_LEN);
		count += PKG_TYPE_LEN;
		strncpy(p->trade, pkg_recv + count, PKG_TRADECODE_LEN);
		count += PKG_TRADECODE_LEN;

		strncpy(p->result, pkg_recv + count, PKG_RESULTCODE_LEN);
		count += PKG_RESULTCODE_LEN;

		strncpy(p->task, pkg_recv + count, PKG_TASKID_LEN);
		count += PKG_TASKID_LEN;

		if(count != strlen(pkg_recv))
			return -1;
	}
	catch(...)
	{
		return -1;
	}	

	return 0;	
}

/*获取单票识别任务
@ip: 任务调度服务器ip
@port:服务器端口
@authinput_res: 输出参数， 封装了任务信息
return :   成功-0 ， 失败-其他
*/
int CAuthInput::get_task(string ip, string port, struct pkg_authinput_res_s *authinput_res)
{
	ZSocket so;
	char *pkg_send = NULL, *pkg_recv = NULL;
	int pkg_send_len=0;
	char pkg_len[PKG_LEN_LEN+1];
	
	pkg_send_len = PKG_LEN_LEN + PKG_TYPE_LEN +PKG_TRADECODE_LEN +1;


	if((pkg_send=(char *)malloc(pkg_send_len))==NULL)
	{
		LogPrint( AUTHINPUT_NODE,"Malloc memory error: %s\n", strerror(errno));
		return -1;
	}
			
	memset(pkg_send , '\0', pkg_send_len);

	if(get_pkg(pkg_send, PKG_USR_REQ, AUTHINPUT_TRADECODE_REQ) != 0)
	{
		if(pkg_send != NULL)
			free(pkg_send);

		LogPrint( AUTHINPUT_NODE,"get_pkg\n", strerror(errno));
		return -1;
	}

	if(verbose)
		LogPrint( AUTHINPUT_NODE,"Connect to %s %s\n", ip.c_str(), port.c_str());	

	if((so.Connect(ip.c_str(), atoi(port.c_str())))!= ZS_SUCCESS)
		return CONN_SERVER_ERR;

	//LogPrint( AUTHINPUT_NODE,"开始获取任务ID...\n");	

	if(so.Send(pkg_send, strlen(pkg_send)) != strlen(pkg_send))
	{
		if(pkg_send != NULL)
			free(pkg_send);

		LogPrint( AUTHINPUT_NODE,"Cann't send to task server: %s %s\n", ip.c_str(), port.c_str());
		so.Close();
		return -1;
	}

	//LogPrint( AUTHINPUT_NODE,"Send->[%s]\n",pkg_send);
	
	if(pkg_send!=NULL)
	{	
		free(pkg_send);
	}

	memset(pkg_len, '\0', sizeof(pkg_len));
	int m_len = 0;	
	if((m_len = so.RecvEx(pkg_len, PKG_LEN_LEN)) != PKG_LEN_LEN)
	{
		if(m_len==0)
		{
			LogPrint( AUTHINPUT_NODE,"错误!服务器关闭连接!\n");
		}
		else
		{
			LogPrint( AUTHINPUT_NODE,"错误!错误的报文头!\n");
		}
		so.Close();
		return -1;
	}

	//recv pachage body
	for(int j=0; j<5; j++)
		if((pkg_recv = (char *)malloc(atoi(pkg_len)+1)) != NULL)
			break;

	if(pkg_recv==NULL)
	{
		LogPrint( AUTHINPUT_NODE,"Malloc memory error.[%s]\n", strerror(errno));
		so.Close();
		return -1;
	}

	memset(pkg_recv, '\0', atoi(pkg_len)+1);

	if(so.RecvEx(pkg_recv, atoi(pkg_len))	!= atoi(pkg_len))
	{
		free(pkg_recv);
		so.Close();
		LogPrint( AUTHINPUT_NODE,"Receive body error.\n");
		return -1;
	}

	so.Close();	

	if(task_count%20==0)
		LogPrint( AUTHINPUT_NODE,"Recv->[%s%s]\n", pkg_len, pkg_recv);	

	if(pkg_recv_resolve(pkg_recv, authinput_res)!=0)
	{
		LogPrint( AUTHINPUT_NODE,"pkg_recv_resole\n");

		if(pkg_recv!=NULL)
			free(pkg_recv);

		return -1;
	}

	//cann't get task
	if((atoi(authinput_res->result)!= atoi(TASK_RESULT_SUCC)) && (atoi(authinput_res->trade)==0))
	{
		free(pkg_recv);
		return -1;
	}

	free(pkg_recv);	

	return 0;
}

/*获取消息
@sid : 任务id
@tb_authinput: 单票信息
return : 成功-0， 失败-其他 
*/
int CAuthInput::get_msg(const char *sid, struct tb_authinput_s *tb_authinput)
{
	return m_db.GetAuthInputRowBySid( sid, tb_authinput);
}

//获取图像
int CAuthInput::get_img(const char *fname, struct tb_authinput_s *v, MemT& pRGB, int& sizeX, int& sizeY, int& pannel)
{

	return  ti_getimage(fname,v->ip,3210, v->rotate, pRGB, sizeX, sizeY, pannel);
}

//版面识别
int CAuthInput::rec_model(unsigned char *srcImage, int sizeX, int sizeY, int channel, int dpi, char *cm_code , int& model)
{
    int ret = 0;
    try 
    {   
        TilOperationAndReturn t;
    
        memset(&t, 0, sizeof(t));

        t.recogPattern_type = 1;

        t.E13b_type = 1;

        t.OCR_type = 0;

        strcpy(t.recog_cfg, recog_cfg.c_str());

        strcpy(t.checkOCR_cfg, checkOCR_cfg.c_str());

        ret = TilPatternRecogClassify( srcImage, sizeX, sizeY, channel, dpi, t); 

        if(ret==0)
        {   
            strcpy(cm_code, t.rst_E13b);

            model = t.rst_pattern;
        }   
        else if(ret == -1) 
		{
            LogPrint( AUTHINPUT_NODE,"输入的cfg_no值不一样\n");
		}
        else if(ret == -2) 
		{
            LogPrint( AUTHINPUT_NODE,"读取模板数据出错!\n");
		}
        else if(ret == -3) 
		{
            LogPrint( AUTHINPUT_NODE,"读图有误!\n");
		}
        else if(ret == 1)
		{
            LogPrint( AUTHINPUT_NODE,"识别版面出错。\n");
		}
        else if(ret == 2)
		{
            LogPrint( AUTHINPUT_NODE,"求出的磁码区域出错！\n");
		}
        else if(ret == -6) 
		{
            LogPrint( AUTHINPUT_NODE,"识别磁码出错!\n");
		}
        else 
		{
            LogPrint( AUTHINPUT_NODE,"识别未知错误!\n");
		}
    }   
    catch(...)
    {   
        LogPrint( AUTHINPUT_NODE,"\n识别异常!\n");
        return -1; 
    }   

    return ret;
}

/*磁码切割 凭证号 交换号 账号 凭证类型 金额 根据字符切割出5个域
@cm_code：磁码信息
@vchno: 凭证号
@exchno:交换号
@accno: 账号
@vchtype: 凭证类型
@amount: 金额
return : 成功-0， 失败-其他 */
int	CAuthInput::spart_code(const char *cm_code)
{
 	string s = cm_code;
    vector<string> _session;

    for(int i=0; i<s.length(); i++)
    {   
        if( (s[i]<'0' || s[i]>'9') && s[i]!='-' ) s[i] = ' ';
    }   

    istringstream is(s.c_str());

    string str[5];
    str[4]="0";

    try{
        int i = 0;
        while( !is.eof() && i<5 )
        {   
            string item;
            is >> item;
            if(i == 0 && item.length() == 1) continue;

            if( item.length()>1 ) str[i] = item;
            else continue;
            //LogPrint( AUTHINPUT_NODE,"str [%d] [%s]\n",i,str[i].c_str());
            ++i;
        }    

        if(i == 4)
        {   
            if(str[0].length()!=6)
            {   
                LogPrint( AUTHINPUT_NODE,"凭证号丢失\n");
                str[4]= str[3];
                str[3]= str[2];
                str[2]= str[1];
                str[1]= str[0];
                str[0]= "000000";
            }
            else if(str[2].length()==2)
            {
                LogPrint( AUTHINPUT_NODE,"账号丢失\n");
                str[4]=str[3];
                str[3]=str[2];
                str[2]="00000000";
            }
            else
            {
                LogPrint( AUTHINPUT_NODE,"类型丢失\n");
                str[4]=str[3];
                str[3]="00";
            }
        }

        if(i == 3)
        {
            if(str[0].length()>6)
            {
                LogPrint( AUTHINPUT_NODE,"凭证号缺失\n");
                if( str[1].length()==2)
                {
                    LogPrint( AUTHINPUT_NODE,"账号缺失\n");
                    str[4]= str[2];
                    str[3]= str[1];
                    str[2]= "0000000000";
                    str[1]= str[0];
                    str[0]= "000000";
                }
                else
                {
                    LogPrint( AUTHINPUT_NODE,"类型缺失\n");
                    str[4]= str[2];
                    str[2]= str[1];
                    str[3]= "00";
                    str[1]= str[0];
                    str[0]= "000000";
                }
            }
            else
            {
                    LogPrint( AUTHINPUT_NODE,"账号缺失\n");
                    LogPrint( AUTHINPUT_NODE,"类型缺失\n");

                    str[4]= str[2];
                    str[3]= "00";
                    str[2]= "0000000000";
            }
        }

        for(int j=0; j<5; j++) _session.push_back(str[j]);

        if( _session.size()<3 ) return(-3);

        if( _session[1].length()<4 || _session[2].length()<8 )
            return(-4);

    }
    catch(...)
    {
        return -1;
    }

    strncpy(m_vchno, _session[0].c_str(), sizeof(m_vchno) - 1);
    strncpy(m_exchno, _session[1].c_str(), sizeof(m_exchno) - 1);
    strncpy(m_accno, _session[2].c_str(), sizeof(m_accno) - 1);
    strncpy(m_vchtype, _session[3].c_str(), sizeof(m_vchtype) - 1);
    strncpy(m_amount, _session[4].c_str(), sizeof(m_amount) - 1);

    return 0;
}	

/*更新单票识别状态
@sid: 单票识别id
@col_name: 列名称
@col_val: 列值
@times : times =1, 更新times次数加1，否则不更新数据库中的times
return : 成功-0 ，失败-其他  */
int CAuthInput::update_authinput_state(const unsigned long sid, const char *col_name, int col_val)
{
	char value[512];
	int ret = 0;
	try
	{
		memset(value, '\0', sizeof(value));
		sprintf(value , "%d", col_val);	
	}
	catch(...)
	{
		return -1;
	}

	ret = m_db.UpColumnByPK("sid", sid, "authinput", col_name, value);
	
	if(ret==0)
	{
		if(col_val == AUTHINPUT_STATE_SUCC)
			ret = m_db.UpColumnByPK("sid", sid, "authinput", "times", "0");
		else
			ret = m_db.UpColumnByPK("sid", sid, "authinput", "times", "times+1");
	}

	return ret;
}

/*获取图像名
@imageid:图像id
@date: 日期
@session : 场次
@exchno: 交换号
@img_name : 输出参数,图像名
return ： 成功-0 ，失败-其他  */
int CAuthInput::get_img_name(const char *fname,const char *date, const int session, const char *pkgno , char *img_name)
{
	try
	{
		sprintf(img_name, "%s//%02d//%05d//%s", date, session, atoi(pkgno),fname );
	}
	catch(...)
	{
		return -1;
	}

	return 0;
}

/*保存处理信息和状态到数据库
*/

int CAuthInput::save_msg_state(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model, int state, int isEnv, int mainflag )
{
	//调用数据层信封识别状态及信息更新方法
	return m_db.UpdateAuthInputParse(sid, accno, vchno, exchno, vchtype, amount, model, state, isEnv, mainflag);
}

//获取当前系统时间
char * CAuthInput::GetNow()
{
	dt.Now();
	memset(m_now, '\0', sizeof(m_now));
	sprintf(m_now, "%s %s", dt.BankDateString().c_str(), dt.TimeString().c_str());
	
	return m_now;	
}

void CAuthInput::Prt_Til_SegmentPrnChnCharRet(int ret)
{
	switch(ret)
	{
		case 0:
			LogPrint( AUTHINPUT_NODE, "[%d] RETURN_SUCC\n", ret);
			break;
		case -1:
			LogPrint( AUTHINPUT_NODE, "[%d] RETURN_READCONFIG_ERROR\n", ret);
			break;
		case -2:
			LogPrint( AUTHINPUT_NODE, "[%d] RETURN_IMAGESIZE_ERROR\n", ret);
			break;
		case -3:
			LogPrint( AUTHINPUT_NODE, "[%d] RETURN_PREIMAGESIZE_ERROR\n", ret);
			break;
		case -4:
			LogPrint( AUTHINPUT_NODE, "[%d] RETURN_CALLINE_ERROR\n", ret);
			break;
		case -5:
			LogPrint( AUTHINPUT_NODE, "[%d] RETURN_SEGAMOUNTSIZE_ERROR\n", ret);
			break;
		case -6:
			LogPrint( AUTHINPUT_NODE, "[%d] RETURN_EXACTCALPOS_ERROR\n", ret);
			break;
		case -7:
			LogPrint( AUTHINPUT_NODE, "[%d] RETURN_SEGAMOUNTEXACT_ERROR\n", ret);
			break;
		case -8:
			LogPrint( AUTHINPUT_NODE, "[%d] RETURN_LOADMOD_ERROR\n", ret);
			break;
		default:
			LogPrint( AUTHINPUT_NODE, "undefine errror!\n", ret);
	}
}
