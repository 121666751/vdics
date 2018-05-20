
#ifndef __DBOPERATE_H__
#define __DBOPERATE_H__

#include <myplus.h>
#include <string>
#include <vector>
#include "../common/gsipcom.h"

//////////////////////////////////////////////////////////////////////////
// 常量定义

//////////////////////////////////////////////////////////////////////////
// 数据库类

using namespace std;

class CDboperate 
{
public:
	CDboperate();
	~CDboperate();

private:
	MyDb	_db;

public:
	/*
	 *打开数据库
	 */
	int dbo_open(const char*dbhost,const char*dbuser,const char*dbpswd,const char*dbname);
	
	/*
	 *查询config配置
	 */
	int dbo_get_config(KeyMap &vcfg);
	
	/*
	 *查询凭证类型表
	 */
	int dbo_get_vchtype(KeyMap &vcfg);
	
	/*
	 *查询交换区域
	 */
	int dbo_get_area(KeyMap &vcfg);

	int dbo_get_exchno(KeyMap &exno);
	/*
	 *获取票据上传任务明细
	 */
	int dbo_get_sv_tasks(const char*sid,scanInfoT& si);	
	int dbo_get_sc_tasks(const char*sid,scanInfoT& si);	
	
	/*
	 *更改任务状态
	 */
	int dbo_update_sv_state(const char*sid,int st);	
	int dbo_update_sv_state_desc(const char*sid,int st, char *senddesc, int sendcode);	
	
	int dbo_update_sc_state(const char*sid,int st);	
	int dbo_update_sc_state_desc(const char*sid,int st, char *senddesc, int sendcode);	
	
	/*
	 *保存影像ID
	 */
	int dbo_save_sv_imageid(const char*sid,const char* imageid);	
	int dbo_save_sc_imageid(const char*sid,const char* imageid);	

	//methods add by Fan
	int dbo_get_sm_tasks(const char*sid, SCAN_INFO_T& si, vector<string>& vc_fname, vector<string>& vc_bname);

	//get authinput detail 
	int dbo_get_sa_tasks(const char*sid, SCAN_INFO_T& si, vector<string>& va_fname, vector<string>& va_bname, vector<string>& va_imgid);

	int dbo_update_sm_state(const char*sid, int st);
	int dbo_update_sm_state_desc(const char*sid, int st, char *sendstr, int sendcode);

	//update authinput state
	int dbo_update_sa_state(const char*sid,int st);
	int dbo_update_sa_state_desc(const char*sid,int st, char *cmdstr, int sendcode, long elpId);

	int dbo_save_sm_imageid(const char*sid, const char* imageid);

	int dbo_save_sa_imageid(const char*sid,const char* imageid);

};

#endif //__DBOPERATE_H__
