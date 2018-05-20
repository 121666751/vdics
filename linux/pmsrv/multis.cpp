#include "multis.h"
#include <errno.h>
#include <tb_multis.h>
#include <transimg.h>
#include <sstream>
#include <PubFun.h>

#define MAXLINE 1024

#define TASK_COUNT 100

using namespace std;

CMultis::CMultis(){}

CMultis::~CMultis(){}


int verbose;
static int task_count = 0;
string brno;

//PM配置文件信息
#define pm_cfg "./config/config.cfg"

//配置文件全局变量
string ip, port, db_host, db_uid, db_pwd, db_dbAlias, recog_cfg, checkOCR_cfg;

//启动服务
int CMultis::StartServer(int debug)
{
	verbose = debug;

	if(verbose)
	{
		LogPrint( MULTIS_NODE, "./pmsrv -v");
	}
	else
	{
		LogPrint( MULTIS_NODE, "./pmsrv");
		init_dameon();
	}

	printf("pmsrv verbose?%d", verbose);
	
	ZSocket so;
	MemT pRGB;

	char ppl_version[128];
	printf("pmsrv %s", GSIP_VERSION);
	LogPrint( MULTIS_NODE, "pmsrv %s", GSIP_VERSION);

	TilGetPatternRecogVersion(ppl_version);
	LogPrint( MULTIS_NODE, "Picture process library version %s", ppl_version);
	printf("Picture process library version %s", ppl_version);


	char cfg_name[] = "./config/config.cfg";
	char pkg_len[PKG_LEN_LEN+1], cm_code[512];
	char img_name[80];
	//char m_accno[32], m_exchno[12], m_vchno[12],m_vchtype[4], m_amount[16];
	struct pkg_multis_res_s multis_res;
	struct tb_multis_s tb_multis;
	int sizeX = 0, sizeY = 0, pannel = 0, model = 0, isBoard = 0;
	int m_pbcfile = 0;
	int ret = 0;


	try
	{
		//打印图像库版本
		char ppl_version[128];
		TilGetPatternRecogVersion(ppl_version);
		
		LogPrint( MULTIS_NODE,"Parsemultis running ...\nProgress Version %s\nppl_version %s", GSIP_VERSION, ppl_version);

		if(InitCfg(cfg_name))
		{
			LogPrint( MULTIS_NODE,"InitCfg");
			LogPrint( MULTIS_NODE,"读取配置文件错误!");
			return -1;
		}

		if(InitDBInfo())
		{
			LogPrint( MULTIS_NODE,"InitDBInfo");
			LogPrint( MULTIS_NODE,"连接数据库失败!");
			return -1;
		}


		for(;;)
		{
			//任务调度计数器
			if(task_count++ > TASK_COUNT)
				task_count = 0;
	
			memset(&multis_res, 0, sizeof(multis_res));

			if( (ret = get_task(ip, port, &multis_res))!= 0 )
			{
				if(ret == CONN_SERVER_ERR)
				{
					LogPrint( MULTIS_NODE,"Cann't connect to %s %d", task_ip, task_port);
					sleep(5);
					continue;
				}
				else
				{ 
					LogPrint( MULTIS_NODE,"get_task");
					LogPrint( MULTIS_NODE,"获取任务失败!");
					sleep(5);
					continue;
				}
			}

			memset(&tb_multis, 0, sizeof(tb_multis));
		
			if(ret = get_msg( multis_res.task, &tb_multis) != 0)
			{
				if(ret == SQL_NO_RESULT)
				{
					if(verbose)
						LogPrint( MULTIS_NODE,"任务队列空!");
					sleep(3);
					continue;
				}
				else if(ret == -1)
				{
					LogPrint( MULTIS_NODE,"get_msg");
					LogPrint( MULTIS_NODE,"获取报文体错误!");
					continue;
				}
			}

			LogPrint( MULTIS_NODE,"获取[%d]任务明细成功!", tb_multis.sid );

			//获取图像	
			memset(img_name, '\0', sizeof(img_name));

			if(get_img_name(tb_multis.fname, tb_multis.date, tb_multis.session, tb_multis.pkgno,img_name )!=0)
			{
				LogPrint( MULTIS_NODE,"get_img_name! 获取图像名错误!");
				continue;
			}
	

			//LogPrint( MULTIS_NODE,"img_name=[%s]", img_name);

			if(get_img(img_name, &tb_multis, pRGB, sizeX, sizeY, pannel)!=0)
			{
				LogPrint( MULTIS_NODE,"[%d]get_img! 取图失败! img_name=[%s] ip=[%s]", tb_multis.sid, img_name, tb_multis.ip);
				update_multis_state(tb_multis.sid, "state", MULTIS_STATE_FAIL);
				sleep(5);
				continue;	
			}

			memset(cm_code, '\0', sizeof(cm_code));	

			model = 0;

			//版面识别 获取磁码

			//此处新函数接口要加modeflag
			if( (ret =rec_model(pRGB.data(), sizeX, sizeY, pannel, tb_multis.dpi, cm_code , model, isBoard))!=0)
			{
				if(strlen(cm_code)==0)
				{
					LogPrint( MULTIS_NODE,"[%d]版面识别失败！磁码为空! model->[%d]", tb_multis.sid, model);
				}

				LogPrint( MULTIS_NODE,"[%d]版面识别失败！cm_code->[%s] model->[%d] ret->[%d]", tb_multis.sid, cm_code, model, ret);

				LogPrint( MULTIS_NODE,"rec_mode");
				
				update_multis_state(tb_multis.sid, "state", MULTIS_STATE_REC_FAIL);

				continue;
			}

			LogPrint( MULTIS_NODE,"版面识别成功! isBoard->[%d] codeline->[%s] model->[%d]", isBoard, cm_code, model);			

			//支票新版面为9，老版面为5
			int modeflag = 0;

			if(model==9)
			  modeflag = 1;
			else if(model==5)
			  modeflag = 0;
			else
			  modeflag = 0;

			memset(m_accno, '\0', sizeof(m_accno));
			memset(m_exchno, '\0', sizeof(m_exchno));
			memset(m_vchno, '\0', sizeof(m_vchno));
			memset(m_vchtype, '\0', sizeof(m_vchtype));
			memset(m_amount, '\0', sizeof(m_amount));


			//隔板model返回值？？？===========================================
		


			//如果是信封，切割磁码
			if( (model==2) || (model==6) || (model==7))
			{
				LogPrint( MULTIS_NODE, "正在处理信封...");

				//非隔板的进行磁码切割
				if(!isBoard)
				{
					ret = spart_code(cm_code);
					if( ret!=0 )
					{
						LogPrint( MULTIS_NODE,"spart_code!");
						LogPrint( MULTIS_NODE,"[%d]信封磁码切割处理失败! codeline=[%s] ret[%d]", tb_multis.sid, cm_code, ret);
						update_multis_state(tb_multis.sid, "state", MULTIS_STATE_REC_FAIL);
						continue;
					}
				}
				else
				{
					ret = spart_board(cm_code);
					if(ret!=0)
					{
						LogPrint( MULTIS_NODE, "spart_board! ret[%d]", ret);
						update_multis_state(tb_multis.sid, "state", MULTIS_STATE_REC_FAIL);
						continue;
					}
				}
		
				LogPrint( MULTIS_NODE,"切割磁码成功!");
				LogPrint( MULTIS_NODE,"codeline->[%s]\nm_vchno->[%s]\nm_exchno->[%s]\nm_accno->[%s]\nm_vchtype->[%s] \nm_amount->[%s]", cm_code, m_vchno, m_exchno, m_accno, m_vchtype, m_amount);
				
				
				//保存状态及识别信息 
				if(save_msg_state(tb_multis.sid, m_accno, m_vchno, m_exchno, m_vchtype, m_amount, model, MULTIS_STATE_SUCC, modeflag)!=0)
				{
					LogPrint( MULTIS_NODE,"save_msg_state");
					LogPrint( MULTIS_NODE,"[%d]保存状态及识别信息失败!", tb_multis.sid);
					continue;	
				}
			}
			//不是信封保存版式和状态
			else
			{
				LogPrint( MULTIS_NODE, "处理主附件...");

				if(m_db.UpMultisNotEnv(tb_multis.sid, model , MULTIS_STATE_SUCC)!=0)
				{
					LogPrint( MULTIS_NODE,"save_msg_state");
					LogPrint( MULTIS_NODE,"[%d]保存状态及识别信息失败!", tb_multis.sid);
					continue;	
				}
			}
			
			LogPrint( MULTIS_NODE,"任务[%d]完成!\n", tb_multis.sid);
		}
	}
	catch(...)
	{
		LogPrint( MULTIS_NODE,"Error occured: %s!\n", strerror(errno));
		return -1;
	}
}

//初始化配置文件
int  CMultis::InitCfg(char *cfg_name)
{
	try
	{
		Config cfg(cfg_name);

		ip = cfg.value("task_ip",1);
		if(ip.size()<=0)
		{
			LogPrint( MULTIS_NODE, "配置错误： task_ip不能为空!\n");
			printf( "配置错误： task_ip不能为空!\n");
			return -1;
		}

		port = cfg.value("task_port",1);
		if(port.size()<=0)
		{
			LogPrint( MULTIS_NODE, "配置错误： task_port不能为空!\n");
			printf( "配置错误： task_port不能为空!\n");
			return -1;
		}

		strcpy(task_ip ,ip.c_str());
		task_port = atoi(port.c_str());

		db_host = cfg.value("db_host",1);
		if(db_host.size()<=0)
		{
			LogPrint( MULTIS_NODE, "配置错误： db_host不能为空!\n");
			printf( "配置错误： db_host不能为空!\n");
			return -1;
		}

		db_uid = cfg.value("db_uid",1);
		if(db_uid.size()<=0)
		{
			LogPrint( MULTIS_NODE, "配置错误： db_uid不能为空!\n");
			printf( "配置错误： db_uid不能为空!\n");
			return -1;
		}

		db_pwd = cfg.value("db_pwd",1);
		if(db_pwd.size()<=0)
		{
			LogPrint( MULTIS_NODE, "配置错误： db_pwd不能为空!\n");
			printf( "配置错误： db_pwd不能为空!\n");
			return -1;
		}


		db_dbAlias = cfg.value("db_dbAlias",1);
		if(db_dbAlias.size()<=0)
		{
			LogPrint( MULTIS_NODE, "配置错误： db_dbAlias不能为空!\n");
			printf( "配置错误： db_dbAlias不能为空!\n");
			return -1;
		}



		brno  = cfg.value("brno", 1);    
		if(brno.size()<=0)
		{
			LogPrint( MULTIS_NODE, "配置错误： brno不能为空!\n");
			printf( "配置错误： brno不能为空!\n");
			return -1;
		}
		else if(brno.size() > 6)
		{
			LogPrint( MULTIS_NODE, "配置错误： brno长度过长!\n");
			printf( "配置错误： brno长度过长!\n");
			return -1;
		}

        Config mdl("./config/model.cfg");

        recog_cfg = mdl.value(brno.c_str(), "recog_cfg", 1);
		if(recog_cfg.size()<=0)
		{
			LogPrint( MULTIS_NODE, "配置错误: recog_cfg不能为空!\n");
			printf("配置错误: recog_cfg不能为空!\n");
			return -1;
		}

        checkOCR_cfg = mdl.value(brno.c_str(), "checkOCR_cfg", 1);
		if(checkOCR_cfg.size()<=0)
		{
			LogPrint( MULTIS_NODE, "配置错误：checkOCR_cfg不能为空!\n");
			printf("配置错误：checkOCR_cfg不能为空!\n");
			return -1;
		}

        LogPrint( MULTIS_NODE, "brno->[%s]\nrecog_cfg->[%s]\ncheckOCR_cfg->[%s]\n", brno.c_str(), recog_cfg.c_str(), checkOCR_cfg.c_str());
	}
	catch(...)
	{
		LogPrint( MULTIS_NODE,"read config error...\n");
		return -1;
	}	
	return 0;
}

//初始化数据库信息
int  CMultis::InitDBInfo()
{
	return m_db.DbOpen(db_host.c_str(), db_uid.c_str(), db_pwd.c_str(), db_dbAlias.c_str());
}

/*接收包拆包
@pkg_recv: 接收包
@pkg_body: 接收报文报文体
return : 成功-0， 失败-其他 
*/
int CMultis::pkg_recv_resolve(char *pkg_recv,struct pkg_multis_res_s *p)
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
@multis_res: 输出参数， 封装了任务信息
return :   成功-0 ， 失败-其他
*/
int CMultis::get_task(string ip, string port, struct pkg_multis_res_s *multis_res)
{
	ZSocket so;
	char *pkg_send = NULL, *pkg_recv = NULL;
	int pkg_send_len=0;
	char pkg_len[PKG_LEN_LEN+1];
	
	pkg_send_len = PKG_LEN_LEN + PKG_TYPE_LEN +PKG_TRADECODE_LEN +1;


	if((pkg_send=(char *)malloc(pkg_send_len))==NULL)
	{
		LogPrint( MULTIS_NODE,"Malloc memory error: %s\n", strerror(errno));
		return -1;
	}
			
	memset(pkg_send , '\0', pkg_send_len);

	if(get_pkg(pkg_send, PKG_USR_REQ, MULTIS_TRADECODE_REQ) != 0)
	{
		if(pkg_send != NULL)
			free(pkg_send);

		LogPrint( MULTIS_NODE,"get_pkg\n", strerror(errno));
		return -1;
	}

	if(verbose)
		LogPrint( MULTIS_NODE,"Connect to %s %s\n", ip.c_str(), port.c_str());	

	if((so.Connect(ip.c_str(), atoi(port.c_str())))!= ZS_SUCCESS)
		return CONN_SERVER_ERR;

	//LogPrint( MULTIS_NODE,"开始获取任务ID...\n");	

	if(so.Send(pkg_send, strlen(pkg_send)) != strlen(pkg_send))
	{
		if(pkg_send != NULL)
			free(pkg_send);

		LogPrint( MULTIS_NODE,"Cann't send to task server: %s %s\n", ip.c_str(), port.c_str());
		so.Close();
		return -1;
	}

	//LogPrint( MULTIS_NODE,"Send->[%s]\n",pkg_send);
	
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
			LogPrint( MULTIS_NODE,"错误!调度服务关闭连接!\n");
		}
		else
		{
			LogPrint( MULTIS_NODE,"错误!报文头错误!\n");
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
		LogPrint( MULTIS_NODE,"Malloc memory error.[%s]\n", strerror(errno));
		so.Close();
		return -1;
	}

	memset(pkg_recv, '\0', atoi(pkg_len)+1);

	if(so.RecvEx(pkg_recv, atoi(pkg_len))	!= atoi(pkg_len))
	{
		free(pkg_recv);
		so.Close();
		LogPrint( MULTIS_NODE,"Receive body error.\n");
		return -1;
	}

	so.Close();	

	if(task_count%20==0)
		LogPrint( MULTIS_NODE,"Recv->[%s%s]\n", pkg_len, pkg_recv);	

	if(pkg_recv_resolve(pkg_recv, multis_res)!=0)
	{
		LogPrint( MULTIS_NODE,"pkg_recv_resole\n");

		if(pkg_recv!=NULL)
			free(pkg_recv);

		return -1;
	}

	//cann't get task
	if((atoi(multis_res->result)!= atoi(TASK_RESULT_SUCC)) && (atoi(multis_res->trade)==0))
	{
		free(pkg_recv);
		return -1;
	}

	free(pkg_recv);	

	return 0;
}

/*获取消息
@sid : 任务id
@tb_multis: 单票信息
return : 成功-0， 失败-其他 
*/
int CMultis::get_msg(const char *sid, struct tb_multis_s *tb_multis)
{
	return m_db.GetMultisRowBySid( sid, tb_multis);
}

//获取图像
int CMultis::get_img(const char *fname, struct tb_multis_s *v, MemT& pRGB, int& sizeX, int& sizeY, int& pannel)
{

	return  ti_getimage(fname,v->ip,3210, v->rotate, pRGB, sizeX, sizeY, pannel);
}

//版面识别
int CMultis::rec_model(unsigned char *srcImage, int sizeX, int sizeY, int channel, int dpi, char *cm_code , int& model, int &isBoard)
{
    int ret = 0;

	isBoard = 0;
	
    try 
    {   
        TilOperationAndReturn t;
    
        memset(&t, 0, sizeof(t));

		t.E13b_type = 2;

        t.OCR_type = 0;

        strcpy(t.recog_cfg, recog_cfg.c_str());

        strcpy(t.checkOCR_cfg, checkOCR_cfg.c_str());

		//隔板判定
		t.recogPattern_type = 3;

		ret = TilPatternRecogClassify( srcImage, sizeX, sizeY, channel, dpi, t); 

		LogPrint( MULTIS_NODE, "隔板判定: recogPattern->[%d] rst_pattern->[%d], rst_E13b->[%s] ret->[%d]", t.recogPattern_type, t.rst_pattern, t.rst_E13b, ret );

		//隔板
		if( t.rst_pattern==2)
		{
			isBoard = 1;
		}
		//非隔板
		else if(t.rst_pattern==0)
		{
			t.recogPattern_type = 1;

			LogPrint( MULTIS_NODE, "非隔板识别: E13b_type->[%d] OCR_type->[%d] recgPattern_type->[%d]", t.E13b_type, t.OCR_type , t.recogPattern_type);

			ret = TilPatternRecogClassify( srcImage, sizeX, sizeY, channel, dpi, t); 
		}
		//未知
		else
		{
			LogPrint(MULTIS_NODE, "未知类型: ret->[%d], isboard->[%d], rst_pattern[%d]", ret, isBoard, t.rst_pattern);
			return -1;
		}

        if( ret==0 )
        {   
            model = t.rst_pattern;

            strcpy(cm_code, t.rst_E13b);
        }   
		LogPrint(MULTIS_NODE, "ret->[%d], isboard->[%d], model[%d], codeline[%s]", ret, isBoard, t.rst_pattern, t.rst_E13b);
    }   
    catch(...)
    {   
        LogPrint( MULTIS_NODE,"\n识别异常!\n");

        return -1; 
    }   

    return ret;
}

//切割隔板
//000000-00-02
//交换号-2位票据类型-2位票据数量
int CMultis::spart_board(char *cm_code)
{
	char *s = cm_code;
	char *p, *delim="-";
	char str[3][50];

	p = strtok( s, delim);

	strcpy(str[0], p);

	for( int i=1;(p = strtok(NULL, delim));i++)
    {   
		strcpy(str[i], p);
    }

	strncpy(m_vchno, "000000", sizeof(m_vchno) - 1);
    strncpy(m_exchno, str[0], sizeof(m_exchno) - 1);
    strncpy(m_accno, "000000000000", sizeof(m_accno) - 1);
    strncpy(m_vchtype, str[1], sizeof(m_vchtype) - 1);
    strncpy(m_amount, "0", sizeof(m_amount) - 1);

	return 0;
}

/*磁码切割 凭证号 交换号 账号 凭证类型 金额 根据字符切割出5个域
@cm_code：磁码信息
@vchno: 凭证号
@exchno:交换号
@accno: 账号
@vchtype: 凭证类型
@amount: 金额
return : 成功-0， 失败-其他 */
int	CMultis::spart_code(const char *cm_code)
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
            LogPrint( MULTIS_NODE,"str [%d] [%s]",i,str[i].c_str());
            ++i;
        }    

        if(i == 4)
        {   
            if(str[0].length()!=6)
            {   
                LogPrint( MULTIS_NODE,"凭证号丢失");
                str[4]= str[3];
                str[3]= str[2];
                str[2]= str[1];
                str[1]= str[0];
                str[0]= "000000";
            }
            else if(str[2].length()==2)
            {
                LogPrint( MULTIS_NODE,"账号丢失");
                str[4]=str[3];
                str[3]=str[2];
                str[2]="00000000";
            }
            else
            {
                LogPrint( MULTIS_NODE,"类型丢失");
                str[4]=str[3];
                str[3]="00";
            }
        }

        if(i == 3)
        {
            if(str[0].length()>6)
            {
                LogPrint( MULTIS_NODE,"凭证号缺失");
                if( str[1].length()==2)
                {
                    LogPrint( MULTIS_NODE,"账号缺失");
                    str[4]= str[2];
                    str[3]= str[1];
                    str[2]= "0000000000";
                    str[1]= str[0];
                    str[0]= "000000";
                }
                else
                {
                    LogPrint( MULTIS_NODE,"类型缺失");
                    str[4]= str[2];
                    str[2]= str[1];
                    str[3]= "00";
                    str[1]= str[0];
                    str[0]= "000000";
                }
            }
            else
            {
                    LogPrint( MULTIS_NODE,"账号缺失");
                    LogPrint( MULTIS_NODE,"类型缺失");

                    str[4]= str[2];
                    str[3]= "00";
                    str[2]= "0000000000";
            }
        }

        for(int j=0; j<5; j++) 
		{
			_session.push_back(str[j]);
		}

        if( _session.size()<3 ) return(-3);

        if( _session[1].length()<4)
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
int CMultis::update_multis_state(const unsigned long sid, const char *col_name, int col_val)
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

	ret = m_db.UpColumnByPK("sid", sid, "multis", col_name, value);
	
	if(ret==0)
	{
		if(col_val == MULTIS_STATE_SUCC)
			ret = m_db.UpColumnByPK("sid", sid, "multis", "times", "0");
		else
			ret = m_db.UpColumnByPK("sid", sid, "multis", "times", "times+1");
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
int CMultis::get_img_name(const char *fname,const char *date, const int session, const char *pkgno , char *img_name)
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

int CMultis::save_msg_state(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model, int state, int modeflag)
{
	//调用数据层信封识别状态及信息更新方法
	return m_db.UpdateMultisParse(sid, accno, vchno, exchno, vchtype, amount, model, state, modeflag);
}

//获取当前系统时间
char * CMultis::GetNow()
{
	dt.Now();
	memset(m_now, '\0', sizeof(m_now));
	sprintf(m_now, "%s %s", dt.BankDateString().c_str(), dt.TimeString().c_str());
	
	return m_now;	
}
