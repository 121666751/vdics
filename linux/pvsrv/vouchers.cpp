#include "vouchers.h"
#include <errno.h>
#include <tb_vouchers.h>
#include <transimg.h>
#include <sstream>
#include <TC_Bocom_Pattern_Analysis.h>
#include <PubFun.h>

#define MAXLINE 1024

#define TASK_COUNT 1000

int verbose;

static int task_count = 0;
using namespace std;

CVouchers::CVouchers(){}

CVouchers::~CVouchers(){}

string  brno, ip, port, db_host, db_uid, db_pwd, db_dbAlias, recog_cfg, checkOCR_cfg;
//启动服务
int CVouchers::StartServer(int debug)
{
	verbose = debug;

	if(verbose)
	{
		LogPrint( VOUCHER_NODE, "./pvsrv -v");
	}
	else
	{
		LogPrint( VOUCHER_NODE, "./pvsrv");
		init_dameon();
	}

	LogPrint( VOUCHER_NODE, "pvsrv verbose?%d", verbose);

	ZSocket so;
	MemT pRGB;

	LogPrint( VOUCHER_NODE, "pvsrv %s", GSIP_VERSION);

	char cfg_name[] = "./config/config.cfg";
	char pkg_len[PKG_LEN_LEN+1], cm_code[512];
	char img_name[80];
	//char m_accno[33], m_exchno[17], m_vchno[13],m_vchtype[5], m_amount[14], 
	char ret_vchno[256];
	char ppl_version[128];
	struct pkg_vouchers_res_s vouchers_res;
	struct tb_vouchers_s tb_vouchers;
	int sizeX = 0, sizeY = 0, channel = 0, model = 0;
	int ret = 0;

	try
	{
		//打印图像处理动态库版本
		TilGetPatternRecogVersion(ppl_version);
		LogPrint( VOUCHER_NODE, "Picture process library version %s", ppl_version);
		printf("Picture process library version %s\n", ppl_version);
	
		LogPrint( VOUCHER_NODE,"ParseVouchers running ...");

		//读配置文件	
		if(InitCfg(cfg_name) !=0)
		{
			LogPrint( VOUCHER_NODE,"InitCfg");
			LogPrint( VOUCHER_NODE,"读取配置文件失败!");
			return -1;
		}

		//初始化数据库信息
		if(InitDBInfo())
		{
			LogPrint( VOUCHER_NODE,"InitDBInfo!");
			LogPrint( VOUCHER_NODE,"连接数据库服务器失败!");
			return -1;
		}

		//读数据库config配置表
		if(get_config())
		{
			LogPrint( VOUCHER_NODE,"get_config!");
			LogPrint( VOUCHER_NODE,"获取数据库参数列表失败!");
			return -1;
		}	

		//任务计数器

		LogPrint(VOUCHER_NODE,"服务启动...");
		for(;;)
		{
			//从数据库获取参数表计数器， 计数器的作用同时防止SQL连接超时
			if( ++task_count > TASK_COUNT)
			{
				task_count = 0;

				if(get_config())
				{
					LogPrint( VOUCHER_NODE, "get_config!");
					LogPrint( VOUCHER_NODE, "获取数据库参数列表失败!");
					return -1;
				}
			}
		
			memset(&vouchers_res, 0, sizeof(vouchers_res));

			//请求任务
			if( (ret = get_task(ip, port, &vouchers_res))!= 0 )
			{
				if(ret == CONN_SERVER_ERR)
				{
					LogPrint( VOUCHER_NODE,"连接任务调度服务器失败: %s %d", task_ip, task_port);
					sleep(5);
					continue;
				}
				else
				{ 
					LogPrint( VOUCHER_NODE,"get_task!");
					LogPrint( VOUCHER_NODE,"获取任务失败!");
					sleep(5);
					continue;
				}
			}
		
			memset(&tb_vouchers, 0, sizeof(tb_vouchers));

			if( atoi(vouchers_res.task) == 0)
			{
				if(verbose)
					LogPrint( VOUCHER_NODE, "任务队列为空!");
				sleep(5);
				continue;
			}

			//获取任务明细
			if(ret = get_msg( vouchers_res.task, &tb_vouchers) != 0)
			{
				if(ret == SQL_NO_RESULT)
				{
					LogPrint( VOUCHER_NODE,"get_msg -> SQL_NORESULT");
					LogPrint( VOUCHER_NODE,"[%s]获取任务明细失败，无此任务明细!", vouchers_res.task);
					sleep(3);
					continue;
				}
				else if(ret == -1)
				{
					LogPrint( VOUCHER_NODE,"get_msg!");
					LogPrint( VOUCHER_NODE,"获取任务明细错误!");
					sleep(3);
					continue;
				}
			}

			LogPrint( VOUCHER_NODE, "任务[%d]明细获取成功!", tb_vouchers.sid);

			memset(img_name, '\0', sizeof(img_name));

			if(get_img_name(tb_vouchers.fname, tb_vouchers.date, tb_vouchers.session, tb_vouchers.pkgno,img_name )!=0)
			{
				LogPrint( VOUCHER_NODE,"get_img_name!");
				LogPrint( VOUCHER_NODE,"[%d]获取文件名失败!", tb_vouchers.sid);
				continue;
			}

			LogPrint( VOUCHER_NODE, "开始取图... img_name->[%s] area->[%s]", img_name, tb_vouchers.area);
		
			//获取图像	
			if(get_img(img_name, &tb_vouchers, pRGB, sizeX, sizeY, channel)!=0)
			{
				LogPrint( VOUCHER_NODE,"[%d]取图失败!get_img!  img_name->[%s], ip->[%s]", tb_vouchers.sid, img_name, tb_vouchers.ip);
				update_vouchers_state(tb_vouchers.sid, "state", VOUCHERS_STATE_FAIL);
				sleep(5);
				continue;	
			}

			LogPrint( VOUCHER_NODE,"取图成功! sizeX=[%d] sizeY=[%d] channel=[%d] filesize[%d] dpi[%d]", sizeX, sizeY, channel,pRGB.size(),tb_vouchers.dpi);

			memset(cm_code, '\0', sizeof(cm_code));	
			memset(ret_vchno, '\0', sizeof(ret_vchno));

			//版面识别, 获取磁码、版式及凭证号
			if( (ret = rec_model(pRGB.data(), sizeX, sizeY, channel, tb_vouchers.dpi, cm_code , model, ret_vchno))!=0)
			{
				LogPrint( VOUCHER_NODE,"rec_mode");
				LogPrint( VOUCHER_NODE,"[%d]版面识别失败! cm_code->[%s] model->[%d] ret->[%d]", tb_vouchers.sid, cm_code, model, ret);
				update_vouchers_state(tb_vouchers.sid, "state", VOUCHERS_STATE_REC_FAIL);
				continue;
			}
					
			LogPrint( VOUCHER_NODE,"版面识别成功! 磁码:[%s] 版式:[%d]", cm_code, model);

			//新版识别增加新旧版面标志modelfag , modflag=0旧版面， modeflag=1新版面
			int modeflag;

			//新支票版面为9，旧支票版面为5
			if(model==9)
			  modeflag = 1;
			else if(model==5)
			  modeflag = 0;
			else
			  modeflag = 0;

			//版式2、6、7为信封
			if(model==0 || model==2 || model==6 || model ==7)
			{
				LogPrint( VOUCHER_NODE,"rec_mode!");
				LogPrint( VOUCHER_NODE,"[%d]版面识别失败!", tb_vouchers.sid);
				update_vouchers_state(tb_vouchers.sid, "state", VOUCHERS_STATE_REC_FAIL);
				continue;
			}

			if(!isNumeric(ret_vchno))
				strcpy(ret_vchno, "00000000");

			if(!isNumeric(m_vchno))
				strcpy(m_vchno, "00000000");

			memset(m_accno, '\0', sizeof(m_accno));
			memset(m_exchno, '\0', sizeof(m_exchno));
			memset(m_vchno, '\0', sizeof(m_vchno));
			memset(m_vchtype, '\0', sizeof(m_vchtype));
			memset(m_amount, '\0', sizeof(m_amount));
			
			int compare = 0, merge = 0;
		
			//获取校验位和匹配位	
			ret = m_db.GetCmpMerByExchno( tb_vouchers.area, tb_vouchers.exchno, &compare, &merge);

			if(ret==-1)
			{
				LogPrint( VOUCHER_NODE, "GetCmpMerByExchno!");
				LogPrint( VOUCHER_NODE, "[%d]获取是否校验及是否匹配失败!", tb_vouchers.sid);
				update_vouchers_state(tb_vouchers.sid, "state", VOUCHERS_STATE_FAIL);
				continue;
			}

			//切割磁码 输入cm_code, 如果需要校验或需要匹配则切割磁码
			if(compare==1 || merge==1)
			{
				if( (ret=spart_code(cm_code))!=0)
				{
					LogPrint( VOUCHER_NODE,"spart_code!");
					LogPrint( VOUCHER_NODE,"[%d]磁码切割失败! codeline->[%s] ret[%d]", tb_vouchers.sid, cm_code, ret);
					update_vouchers_state(tb_vouchers.sid, "state", VOUCHERS_STATE_REC_FAIL);
					continue;
				}
				
				LogPrint( VOUCHER_NODE,"切割磁码成功! codeline->[%s] \nm_vchno->[%s] \tm_exchno->[%s] \tm_accno->[%s] \tm_vchtype->[%s] \tm_amount->[%s]", cm_code, m_vchno, m_exchno, m_accno, m_vchtype, m_amount);

				//南京分行保存右上脚凭证号 brno=320999
				if(!strcmp(brno.c_str(), "320999"))
					strcpy(m_vchno, ret_vchno);
			}
			else
			{
				LogPrint( VOUCHER_NODE, "不进行磁码切割!");
			}

			//版面校验, 即根据区域+凭证类型从vchtypes表查找对应的model字段,然后与识别出的版式进行比对,如果相同,则版面校验成功; liu_sh 2013-6-4 添加该注释
			if(compare==1)
			{
				LogPrint( VOUCHER_NODE, "开始版面校验!");

				if(chk_model(model, m_vchtype, tb_vouchers.area)!=0)
				{
					LogPrint( VOUCHER_NODE,"[%d]版面校验失败! model->[%d], m_vchtype->[%s]", tb_vouchers.sid, model, m_vchtype);

					//保存版面识别信息
					ret = save_state_msg( (unsigned long)tb_vouchers.sid, m_accno, m_vchno, m_exchno, m_vchtype, m_amount, model, VOUCHERS_STATE_MATCH_FAIL, modeflag);
					if(ret!=0)
					{
						LogPrint( VOUCHER_NODE,"[%d]保存版面识别信息失败!", tb_vouchers.sid);
					}

					continue;
				}

				LogPrint(VOUCHER_NODE,"版面校验成功!");
			}
			else
			{
				LogPrint( VOUCHER_NODE,"不进行版面校验!");

				//如果凭证类型为空，则在vchtypes表中根据area+model查出凭证类型填充进去
				if(m_db.GetVchtypeByModel(model, tb_vouchers.area, m_vchtype)==-1)
				{
					LogPrint( VOUCHER_NODE,"[%d]版面识别失败! model->[%d], m_vchtype->[%s]", tb_vouchers.sid, model, m_vchtype);

					update_vouchers_state(tb_vouchers.sid, "state", VOUCHERS_STATE_REC_FAIL);

					continue;
				}
			}

			if(merge==1)
			{
				//匹配人行数据	
				LogPrint( VOUCHER_NODE,"开始匹配数据...");

				int pbc_sid; 

				int mode =  atoi(cfg_dic["mode"].c_str());

				if(mode==0)
				{
					LogPrint( VOUCHER_NODE, "自行扫描模式!");
				}
				else if(mode==1)
				{
					LogPrint(VOUCHER_NODE, "人行代扫模式!");
				}
				else if(mode==2)
				{
					LogPrint( VOUCHER_NODE, "清分模式!");
				}
				else 
				{
					LogPrint( VOUCHER_NODE, "模式没有注册! mode->[%d]", mode);
				}

				//进行数据匹配
				if((pbc_sid = match_pcb_data(m_vchno, tb_vouchers.exchno, m_accno, m_amount, m_vchtype, tb_vouchers.area, atoi(cfg_dic["mode"].c_str()), tb_vouchers.session, tb_vouchers.date)) == -1)
				{
					LogPrint( VOUCHER_NODE,"match_pcb_data");
					LogPrint( VOUCHER_NODE,"[%d]匹配人行电子数据失败!", tb_vouchers.sid);
				}
				else if(pbc_sid == -2)
				{
					LogPrint( VOUCHER_NODE,"match_pcb_data!");
					LogPrint( VOUCHER_NODE,"[%d]数据库查询错误，匹配人行电子数据失败!", tb_vouchers.sid);
				}
				else
				{	
					//交换ID, 并保存匹配成功状态
					if(save_match_result(tb_vouchers.sid, pbc_sid, mode)!=0)
					{
						LogPrint( VOUCHER_NODE,"save_match_result!");
						LogPrint( VOUCHER_NODE,"[%d]交换ID失败!", tb_vouchers.sid);
					}
				}
			
				LogPrint( VOUCHER_NODE, "sid->[%d] m_vchno->[%s] ret_vchno->[%s] ret->[%d]", tb_vouchers.sid, m_vchno, ret_vchno, ret);

				if(atoi(ret_vchno)<=0)
				{
					bzero(ret_vchno, sizeof(ret_vchno));
					strcpy(ret_vchno, m_vchno);
				}

				ret = save_state_msg( (unsigned long)tb_vouchers.sid, m_accno, m_vchno, m_exchno, m_vchtype, m_amount, model, VOUCHERS_STATE_SUCC, modeflag);

				if(ret!=0)
				{
					LogPrint( VOUCHER_NODE,"[%d]保存识别信息失败!", tb_vouchers.sid);

				}
			
				LogPrint( VOUCHER_NODE,"匹配人行电子数据完成!");
			}
			else
			{
				LogPrint( VOUCHER_NODE,"不匹配人行数据!");
				ret = save_state_msg( (unsigned long)tb_vouchers.sid, m_accno, m_vchno, m_exchno, m_vchtype, m_amount, model, VOUCHERS_STATE_SUCC, modeflag);
				if(ret!=0)
				{
					LogPrint( VOUCHER_NODE,"[%d]保存识别信息失败!", tb_vouchers.sid);
				}
			}
			LogPrint( VOUCHER_NODE,"任务[%d]完成!\n", tb_vouchers.sid);
		}
	}
	catch(...)
	{
		LogPrint( VOUCHER_NODE,"Error occured: %s!\n", strerror(errno));
		return -1;
	}
}


//初始化配置信息
int  CVouchers::InitCfg(char *cfg_name)
{
	try
	{
		Config cfg(cfg_name);

		db_host = cfg.value("db_host",1);
		if(db_host.size()<=0)
		{
			LogPrint( VOUCHER_NODE, "配置错误： db_host不能为空!");
			printf("配置错误： db_host不能为空!\n");
			return -1;
		}
		LogPrint( VOUCHER_NODE, "读取dbhost完成");

		db_uid = cfg.value("db_uid",1);
		if(db_uid.size()<=0)
		{
			LogPrint( VOUCHER_NODE, "配置错误： db_uid不能为空!");
			printf("配置错误： db_uid不能为空!\n");
			return -1;
		}

		db_pwd = cfg.value("db_pwd",1);
		if(db_pwd.size()<=0)
		{
			LogPrint( VOUCHER_NODE, "配置错误： db_pwd不能为空!");
			printf("配置错误： db_pwd不能为空!\n");
			return -1;
		}
		LogPrint( VOUCHER_NODE, "读取数据库ID完成");

		db_dbAlias = cfg.value("db_dbAlias",1);
		if(db_dbAlias.size()<=0)
		{
			LogPrint( VOUCHER_NODE, "配置错误： db_dbAlias不能为空!");
			printf("配置错误： db_dbAlias不能为空!\n");
			return -1;
		}
		LogPrint(  VOUCHER_NODE, "读取数据库实例完成");

		ip = cfg.value("task_ip",1);
		if(ip.size()<=0)
		{
			LogPrint( VOUCHER_NODE, "配置错误： task_ip不能为空!");
			printf("配置错误： task_ip不能为空!\n");
			return -1;
		}
		LogPrint(  VOUCHER_NODE, "读取调度服务ip完成");

		port = cfg.value("task_port",1);
		if(port.size()<=0)
		{
			LogPrint( VOUCHER_NODE, "配置错误： task_port不能为空!");
			printf("配置错误： task_port不能为空!\n");
			return -1;
		}
		LogPrint(  VOUCHER_NODE, "读取调度服务端口完成");

		brno  = cfg.value("brno", 1);	
		if(brno.size()<=0)
		{
			LogPrint( VOUCHER_NODE, "配置错误: brno不能为空!");
			printf("配置错误: brno不能为空!\n");
			return -1;
		}
		LogPrint(  VOUCHER_NODE, "读取分行号完成");

		if(brno.size()>6)
		{
			LogPrint( VOUCHER_NODE, "配置错误: brno长度不能超过6位!");
			printf( "配置错误: brno长度不能超过6位!\n");
			return -1;
		}

		strcpy(task_ip ,ip.c_str());

		task_port = atoi(port.c_str());

		Config mdl("./config/model.cfg");

		recog_cfg = mdl.value(brno.c_str(), "recog_cfg", 1);
		if(recog_cfg.size()<=0)
		{
			LogPrint( VOUCHER_NODE, "配置错误： recog_cfg不能为空!");
			printf("配置错误：recog_cfg不能为空!\n");
			return -1;
		}

		checkOCR_cfg = mdl.value(brno.c_str(), "checkOCR_cfg", 1);
		if(checkOCR_cfg.size()<=0)
		{
			LogPrint( VOUCHER_NODE, "配置错误: checkOCR_cfg不能为空!");
			printf("配置错误: checkOCR_cfg不能为空!\n");
			return -1;
		}

		LogPrint( VOUCHER_NODE, "brno->[%s]\nrecog_cfg->[%s]\ncheckOCR_cfg->[%s]", brno.c_str(), recog_cfg.c_str(), checkOCR_cfg.c_str());
	}
	catch(...)
	{
		LogPrint( VOUCHER_NODE,"read config error...\n");
		printf( "读取配置文件错误!\n");
		return -1;
	}		
	return 0;
}	

//初始化数据库信息
int  CVouchers::InitDBInfo()
{
	return m_db.DbOpen(db_host.c_str(), db_uid.c_str(), db_pwd.c_str(), db_dbAlias.c_str());
}

/*接收包拆包
@pkg_recv: 接收包
@pkg_body: 接收报文报文体
return : 成功-0， 失败-其他 
*/
int CVouchers::pkg_recv_resolve(char *pkg_recv,struct pkg_vouchers_res_s *p)
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
@vouchers_res: 输出参数， 封装了任务信息
return :   成功-0 ， 失败-其他
*/
int CVouchers::get_task(string ip, string port, struct pkg_vouchers_res_s *vouchers_res)
{
	ZSocket so;
	char *pkg_send = NULL, *pkg_recv = NULL;
	int pkg_send_len=0;
	char pkg_len[PKG_LEN_LEN+1];
	
	pkg_send_len = PKG_LEN_LEN + PKG_TYPE_LEN +PKG_TRADECODE_LEN +1;


	if((pkg_send=(char *)malloc(pkg_send_len))==NULL)
	{
		LogPrint( VOUCHER_NODE,"Malloc memory error: %s\n", strerror(errno));
		return -1;
	}
			
	memset(pkg_send , '\0', pkg_send_len);

	if(get_pkg(pkg_send, PKG_USR_REQ, VOUCHERS_TRADECODE_REQ) != 0)
	{
		if(pkg_send != NULL)
			free(pkg_send);

		LogPrint( VOUCHER_NODE,"get_pkg\n", strerror(errno));
		return -1;
	}

	//LogPrint( VOUCHER_NODE,"Connect to %s %s\n", ip.c_str(), port.c_str());	

	if((so.Connect(ip.c_str(), atoi(port.c_str())))!= ZS_SUCCESS)
		return CONN_SERVER_ERR;

	//LogPrint( VOUCHER_NODE,"开始获取任务ID...\n");	

	if(so.Send(pkg_send, strlen(pkg_send)) != strlen(pkg_send))
	{
		if(pkg_send != NULL)
			free(pkg_send);

		LogPrint( VOUCHER_NODE,"Cann't send to task server: %s %s\n", ip.c_str(), port.c_str());
		so.Close();
		return -1;
	}

	//LogPrint( VOUCHER_NODE,"Send->[%s]\n",pkg_send);
	
	if(pkg_send!=NULL)
	{	
		free(pkg_send);
	}

	//recieve package message
	//recv pachage len
	memset(pkg_len, '\0', sizeof(pkg_len));
	int m_len = 0;	
	if((m_len = so.RecvEx(pkg_len, PKG_LEN_LEN)) != PKG_LEN_LEN)
	{
		if(m_len==0)
		{
			LogPrint( VOUCHER_NODE, "错误! 调度服务关闭连接!");
		}
		else
		{
			LogPrint( VOUCHER_NODE,"错误!报文头错误!");
		}

		so.Close();
		return -1;
	}

	//recv body
	for(int j=0; j<5; j++)
		if((pkg_recv = (char *)malloc(atoi(pkg_len)+1)) != NULL)
			break;

	if(pkg_recv==NULL)
	{
		LogPrint( VOUCHER_NODE,"Malloc memory error.[%s]\n", strerror(errno));
		so.Close();
		return -1;
	}

	memset(pkg_recv, '\0', atoi(pkg_len)+1);

	if(so.RecvEx(pkg_recv, atoi(pkg_len))	!= atoi(pkg_len))
	{
		free(pkg_recv);
		so.Close();
		LogPrint( VOUCHER_NODE,"Receive body error.\n");
		return -1;
	}

	so.Close();	

	if(task_count%20==0)
		LogPrint( VOUCHER_NODE,"Recv->[%s%s]\n", pkg_len, pkg_recv);	

	if(pkg_recv_resolve(pkg_recv, vouchers_res)!=0)
	{
		LogPrint( VOUCHER_NODE,"pkg_recv_resole\n");

		if(pkg_recv!=NULL)
			free(pkg_recv);

		return -1;
	}

	//cann't get task
	if((atoi(vouchers_res->result)!= atoi(TASK_RESULT_SUCC)) && (atoi(vouchers_res->trade)==0))
	{
		free(pkg_recv);
		return -1;
	}

	free(pkg_recv);	

	return 0;
}

/*获取消息
@sid : 任务id
@tb_vouchers: 单票信息
return : 成功-0， 失败-其他 
*/
int CVouchers::get_msg(const char *sid, struct tb_vouchers_s *tb_vouchers)
{
	if(atoi(sid)==0)
	  return -1;
	return m_db.GetVouchersRowBySid( sid, tb_vouchers);
}

//获取图像
int CVouchers::get_img(const char *fname, struct tb_vouchers_s *v, MemT& pRGB, int& sizeX, int& sizeY, int& pannel)
{
	//LogPrint( VOUCHER_NODE, "in ti_getimage fname->[%s]	v->ip->[%s]  angle->[%d]\n",fname, v->ip, v->rotate);
	int ret = ti_getimage( fname, v->ip, 3210, v->rotate,  pRGB, sizeX, sizeY, pannel );	
	//LogPrint( VOUCHER_NODE, "out ti_getimage\n");

	return ret;
}

//版面识别
int CVouchers::rec_model(unsigned char *srcImage, int sizeX, int sizeY, int channel, int dpi, char *cm_code , int& model, char *ret_vchno)
{
	int ret = 0;
	try
	{
		TilOperationAndReturn t;
		
		memset(&t, 0, sizeof(t));

		t.recogPattern_type = 1;

		t.E13b_type = 1;

		t.OCR_type = 2;

		strcpy(t.recog_cfg, recog_cfg.c_str());

		strcpy(t.checkOCR_cfg, checkOCR_cfg.c_str());

		LogPrint( VOUCHER_NODE, "开始版面识别...");

		LogPrint( VOUCHER_NODE, "recogPattern_type->[%d] E13b_type->[%d] OCR_type->[%d]", t.recogPattern_type, t.E13b_type, t.OCR_type);

		ret = TilPatternRecogClassify( srcImage, sizeX, sizeY, channel, dpi, t);

		LogPrint( VOUCHER_NODE, "版面识别结束[%d]", ret);

		if(ret==0)
		{
			strcpy(cm_code, t.rst_E13b);

			model = t.rst_pattern;
				
			strcpy(ret_vchno, t.rst_num);
		}
		else if(ret == -1)
		{
			LogPrint( VOUCHER_NODE,"输入的cfg_no值不一样\n");
		}
		else if(ret == -2)
		{
			LogPrint( VOUCHER_NODE,"读取模板数据出错!\n");
		}
		else if(ret == -3)
		{
			LogPrint( VOUCHER_NODE,"读图有误!\n");
		}
		else if(ret == 1)
		{	
			LogPrint( VOUCHER_NODE,"识别版面出错。\n");
		}
		else if(ret == 2)
		{
			LogPrint( VOUCHER_NODE,"求出的磁码区域出错！\n");
		}
		else if(ret == -6)
		{
			LogPrint( VOUCHER_NODE,"识别磁码出错!\n");
		}
		else 
		{
			LogPrint( VOUCHER_NODE,"识别未知错误!\n");
		}
	}
	catch(...)
	{
		LogPrint( VOUCHER_NODE,"\n识别异常!\n");
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
int	CVouchers::spart_code(const char *cm_code)
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
			LogPrint( VOUCHER_NODE,"str [%d] [%s]\n",i,str[i].c_str());
			++i;
		}    

		if(i == 4)
        {
            if(str[0].length()!=6)
            {
                LogPrint( VOUCHER_NODE,"凭证号丢失\n");
			    str[4]= str[3];
			    str[3]= str[2];
			    str[2]= str[1];
			    str[1]= str[0];
                str[0]= "000000";
            }
            else if(str[2].length()==2)
            {
                LogPrint( VOUCHER_NODE,"账号丢失\n");
                str[4]=str[3];
                str[3]=str[2];
			    str[2]="00000000000000";    
            }
            else
            {
                LogPrint( VOUCHER_NODE,"类型丢失\n");
                str[4]=str[3];
			    str[3]="00";      
            }
        }

        if(i == 3)
        {
            if(str[0].length()>6)
            {
                LogPrint( VOUCHER_NODE,"凭证号缺失\n");
                if( str[1].length()==2)
                {
                    LogPrint( VOUCHER_NODE,"账号缺失\n");
			        str[4]= str[2];
			        str[3]= str[1];
			        str[2]= "00000000000000";
			        str[1]= str[0];                     
                    str[0]= "000000";
                }
                else
                {
                    LogPrint( VOUCHER_NODE,"类型缺失\n");
                    str[4]= str[2];
			        str[2]= str[1];
			        str[3]= "00";
			        str[1]= str[0];                    
                    str[0]= "000000";
                }
            }
            else
            {
                    LogPrint( VOUCHER_NODE,"账号缺失\n");
                    LogPrint( VOUCHER_NODE,"类型缺失\n");
                    
                    str[4]= str[2];
			        str[3]= "00";
			        str[2]= "00000000000000";
            }
        }           
	
		for(int j=0; j<5; j++) _session.push_back(str[j]);    

		if( _session.size()<3 ) return(-3);

		if( _session[1].length()<4)
			return(-4);

	}
	catch(...)
    {
		return -1;
	} 

	strncpy(m_vchno, _session[0].c_str(), 6);
	strncpy(m_exchno, _session[1].c_str(), 9);

//	strncpy(accno, _session[2].c_str(), _session[2].size());
	strncpy(m_accno, _session[2].c_str(), sizeof(m_accno) - 1);
	strncpy(m_vchtype, _session[3].c_str(), sizeof(m_vchtype) - 1);
	strncpy(m_amount, _session[4].c_str(), sizeof(m_amount) - 1);

	return 0;
}	

/*版面校验 ,凭证类型一致返回0, 否则返回非0
 *@model: 板式,识别结果
 *@vchtype: 凭证类型
 * return :成功-0, 失败-"-1"  查无结果-"2" 匹配失败-"1"
 *
 * 
 *  版面校验: 即根据区域+凭证类型从vchtypes表查找对应的model字段,然后与识别出的版式进行比对,如果相同,则版面校验成功;
 * Modifer:
 *    1. 添加注释 liu_sh 2013-6-4
**/
int CVouchers::chk_model(const int model, const char *m_vchtype, const char *area)
{
	int ret, tb_model=0;

	try
	{
		if( (strlen(m_vchtype)==0) && (strlen(area)==0))
		{
			LogPrint( VOUCHER_NODE, "停止校验!凭证号或交换区域为空!\n");	
			
			return -1;
		}
		
		tb_model = m_db.GetModelByVchtype( m_vchtype, area);

		if(tb_model==-1)
		{
			LogPrint( VOUCHER_NODE, "版面校验失败! area->[%s] m_vchtype->[%s] model->[%d]  tb_model->[%d]\n", area, m_vchtype, model, tb_model );
			return -1;
		}

		//新版支票版面为9，打码00； 老版支票版面为5，打码00； 但凭证管理打码为主键，只能配一个00，所以新版支票版面校验为老版本支票的版面。
		if( model==9 && tb_model==5)
			return 0;

		if( tb_model != model )
		{
			LogPrint( VOUCHER_NODE, "版面校验失败! area->[%s] m_vchtype->[%s] model->[%d]  tb_model->[%d]\n", area, m_vchtype, model, tb_model );
			return -1;
		}
	}
	catch(...)
	{
		LogPrint( VOUCHER_NODE,"捕捉到错误! 版面校验失败：model->[%d], tb_model->[%d], m_vchtype->[%s], area->[%s]\n", model, tb_model, m_vchtype, area);
		return -1;
	}

	return 0;
}

/*匹配人行数据
@m_vchno: 凭证号
@m_exchno:交换号
@m_accno:账号
@m_amount:金额
@mode: 2-清分模式 1-人行带扫模式 0-自行扫描模式
return : 成功-0， 失败-其他 */
int CVouchers::match_pcb_data(char *m_vchno, char *m_exchno, char *m_accno, char *m_amount, char *m_vchtype, char * area, int mode, int session, char* date)
{
	int ret;

	switch(mode)
	{
		//自行扫描模式
		case 0:
			ret =  m_db.MatchVouchersPcb(m_vchno, m_exchno, m_accno, m_amount, m_vchtype, area, session, date);	
			break;

		//人行代扫模式
		case 1:
			ret =  m_db.MatchVouchersPcb(m_vchno, m_exchno, m_accno, m_amount, m_vchtype, area, session, date);	
			break;

		//清分模式
		case 2:
			ret =  m_db.MatchPcbQF(m_vchno, m_exchno, m_accno, m_amount, m_vchtype, area, session, date);	
			break;

		default:
			ret = -1;		
			LogPrint( VOUCHER_NODE, "match_pbc_data error: 无法识别的模式! mode->[%d] \nmode-0 自行扫描； mode-1:人行带扫; mode-2:清分模式\n", mode);
			break;
	}
	return ret;
}

/*保存匹配结果
@vouchers_sid : 单票id
@pcb_sid: 人行数据id
return : 成功-0, 失败-其他   */
int CVouchers::save_match_result(const unsigned long  vouchers_sid, const unsigned long pcb_sid, int mode)
{
	int ret=0;
	if(mode==0 || mode==1)
	{
		ret = m_db.UpdateVouchersPcb( vouchers_sid, pcb_sid);
	}
	//清分模式
	else if(mode==2)
	{
		ret = m_db.UpdateVouchersDists(vouchers_sid, pcb_sid);
	}
	else
	{
		ret = -1;
		LogPrint( VOUCHER_NODE, "save_match_result error: 无法识别的模式 mode->[%d]\n", mode);
	}

	return ret;
}

/*更新单票识别状态
@sid: 单票识别id
@col_name: 列名称
@col_val: 列值
@times : times=0时times字段不加, times =1, 更新times次数加1，times=2时恢复数据库中的times=0
return : 成功-0 ，失败-其他  */
int CVouchers::update_vouchers_state(const unsigned long sid, const char *col_name, int col_val)
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

	ret = m_db.UpColumnByPK("sid", sid, "vouchers", col_name, value);

	if(ret ==0 )
	{	
		if( col_val == VOUCHERS_STATE_SUCC)
			ret = m_db.UpColumnByPK("sid", sid, "vouchers", "times", "0");
		else
			ret = m_db.UpColumnByPK("sid", sid, "vouchers", "times", "times+1");
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
int CVouchers::get_img_name(const char *fname,const char *date, const int session, const char *pkgno , char *img_name)
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

/*单票识别完成后更新状态并保存信息
修改时间: 2011-1-04
修改人:范朋飞
修改内容: 新增新旧版面标志 int modeflag;
**/
int CVouchers::save_state_msg( unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model, int state, int modeflag)
{
	//调用数据层方法更新单票状态并保存信息
	return m_db.UpdateVouchersParse(sid, accno, vchno, exchno, vchtype, amount, model, state, modeflag);
}

int CVouchers::get_config()
{
		int ret;
		
		cfg_dic.clear();
		if((ret = m_db.get_tb_config(cfg_dic))!=0)
		{
			LogPrint( VOUCHER_NODE,"get_tb_config error: %s, return %d\n", strerror(errno), ret);
			return -1;
		}

	return 0;
}

char * CVouchers::GetNow()
{
	dt.Now();
	memset(m_now, '\0', sizeof(m_now));
	sprintf(m_now, "%s %s", dt.BankDateString().c_str(), dt.TimeString().c_str());
	
	return m_now;	
}
