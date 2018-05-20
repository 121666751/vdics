#ifndef _LINUX_AUTHINPUT_H
#define _LINUX_AUTHINPUT_H

#include <iostream>
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
#include <TC_Bocom_Pattern_Analysis.h>
#include <til_PrnChnOCR.h>

#define DB_QUEUE_LIMIT 100

//单票识别任务请求数据包结构体
struct pkg_authinput_res_s
{
	char type[PKG_TYPE_LEN+1];
	char trade[PKG_TRADECODE_LEN+1];
	char result[PKG_RESULTCODE_LEN+1];
	char task[PKG_TASKID_LEN+1];
};

//单票识别类
class CAuthInput
{
public:
	CAuthInput();
	~CAuthInput();
	
	//初始化配置文件
	int	InitCfg(char *cfg_name);
	//初始化数据库信息 
	int InitDBInfo();
	
	//启动服务
	int StartServer(int verbose);

	//接收数据包解包
	int pkg_recv_resolve(char *pkg_recv, struct pkg_authinput_res_s *pkg_body);

	//获取任务
	int get_task(string ip, string port, struct pkg_authinput_res_s *authinput_res);

	//获取信息
	int get_msg(const char *sid, struct tb_authinput_s *tb_authinput);

	//获取图像
	int get_img(const char *fname, struct tb_authinput_s *v, MemT& pRGB, int& sizeX, int& sizeY, int& pannel);

	//识别板式
	int rec_model(unsigned char *srcImage, int sizeX, int sizeY, int pannel, int dpi, char *cm_code , int& model);

	//磁码切割
	int	spart_code(const char *cm_code);

	//模式校验
	int chk_model(const int model, const char *vchtype);

	//匹配人行数据
	int match_pcb_data(const char *m_vchno, const char *m_exchno, const char *m_accno, const char *m_amount);

	//保存匹配结果
	int save_match_result(const unsigned long authinput_sid, const unsigned long pcb_sid);

	//更新单票识别状态, times=1时同时更新authinput表的times = times +1
	int update_authinput_state(const unsigned long sid, const char *col_name, int col_val);

	//获取图像名
	int get_img_name(const char *imageid,const char *date, const int session, const char * pkgno , char *img_name);

	int save_msg_state(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model, int state, int isAuthInput ,int mainflag);

	//返回票据类型
	int IsAuthInput(int sizeX, int sizeY);

	//是否信封
	int IsEnv(int sizeX, int sizeY);

	//是否计数单
	int IsCount(int sizeX, int sizeY);

	//是否计数单
	int IsNoTax(int sizeX, int sizeY);

	//获取北京分行配置文件	
	int InitBJCfg(char *cfg_name);

	//获取当前时间
	char * GetNow();

	//打印非税委收库识别错误返回信息
	void Prt_Til_SegmentPrnChnCharRet(int ret);

	//数据库对象
	CDBCommon m_db;

	//标准文件内存流对象
	CAnsiMemFile m_cm;

	ZDate dt;
private:
	//任务调度ip
	char task_ip[24];

	//任务调度port
	int task_port;

    char m_accno[33];
    char m_vchno[31];
    char m_vchtype[5];
    char m_exchno[13];
    char m_amount[17];
	//时间
	char m_now[50];
};

#endif
