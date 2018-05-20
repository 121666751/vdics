#ifndef _LINUX_VOUCHERS_H
#define _LINUX_VOUCHERS_H

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

#define DB_QUEUE_LIMIT 100

//单票识别任务请求数据包结构体
struct pkg_vouchers_res_s
{
	char type[PKG_TYPE_LEN+1];
	char trade[PKG_TRADECODE_LEN+1];
	char result[PKG_RESULTCODE_LEN+1];
	char task[PKG_TASKID_LEN+1];
};

//单票识别类
class CVouchers
{
public:
	CVouchers();
	~CVouchers();

	//初始化配置信息	
	int InitCfg(char *cfg_name);

	//初始化数据库信息 
	int InitDBInfo();
	
	//启动服务 verbose=1为调试模式, 打印的信息全
	int StartServer(int verbose);

	//接收数据包解包
	int pkg_recv_resolve(char *pkg_recv, struct pkg_vouchers_res_s *pkg_body);

	//获取任务
	int get_task(string ip, string port, struct pkg_vouchers_res_s *vouchers_res);

	//获取信息
	int get_msg(const char *sid, struct tb_vouchers_s *tb_vouchers);

	//获取图像
	int get_img(const char *fname, struct tb_vouchers_s *v, MemT& pRGB, int& sizeX, int& sizeY, int& pannel);

	//识别板式
	int rec_model(unsigned char *srcImage, int sizeX, int sizeY, int pannel, int dpi, char *cm_code , int& model, char *ret_vchno);

	//磁码切割
	int	spart_code(const char *cm_code);

	//模式校验
	int chk_model(const int model, const char *vchtype, const char *area);

	//匹配人行数据
	int match_pcb_data(char *m_vchno, char *m_exchno, char *m_accno, char *m_amount, char *vchtype, char *area, int mode, int session, char * date);

	//保存匹配结果
	int save_match_result(const unsigned long vouchers_sid, const unsigned long pcb_sid, int mode);

	//更新单票识别状态, times=1时同时更新vouchers表的times = times +1
	int update_vouchers_state(const unsigned long sid, const char *col_name, int col_val);

	//获取图像名
	int get_img_name(const char *imageid,const char *date, const int session, const char * pkgno , char *img_name);

	//单票识别完成后更新状态并保存账号、凭证号、交换号、凭证类型、金额信息
	int save_state_msg( unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model, int state, int modeflag);

	//获取数据库参数信息
	int get_config( );

	//获取当前时间
	char * GetNow();

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

	StrDict  cfg_dic;
	
	StrDict nets_dic;

	StrDict nets_md_vch;

	StrDict nets_vch_md;

	char m_accno[33];
	char m_vchno[31];
	char m_vchtype[5];
	char m_exchno[13];
	char m_amount[17];
		//时间
	char m_now[50];
};

#endif
