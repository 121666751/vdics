
#ifndef __DBOPERATE_H__
#define __DBOPERATE_H__

#include <myplus.h>
#include <string>
#include <vector>
#include "../common/gsipcom.h"

//////////////////////////////////////////////////////////////////////////
// ��������

//////////////////////////////////////////////////////////////////////////
// ���ݿ���

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
	 *�����ݿ�
	 */
	int dbo_open(const char*dbhost,const char*dbuser,const char*dbpswd,const char*dbname);
	
	/*
	 *��ѯconfig����
	 */
	int dbo_get_config(KeyMap &vcfg);
	
	/*
	 *��ѯƾ֤���ͱ�
	 */
	int dbo_get_vchtype(KeyMap &vcfg);
	
	/*
	 *��ѯ��������
	 */
	int dbo_get_area(KeyMap &vcfg);

	int dbo_get_exchno(KeyMap &exno);
	/*
	 *��ȡƱ���ϴ�������ϸ
	 */
	int dbo_get_sv_tasks(const char*sid,scanInfoT& si);	
	int dbo_get_sc_tasks(const char*sid,scanInfoT& si);	
	
	/*
	 *��������״̬
	 */
	int dbo_update_sv_state(const char*sid,int st);	
	int dbo_update_sv_state_desc(const char*sid,int st, char *senddesc, int sendcode);	
	
	int dbo_update_sc_state(const char*sid,int st);	
	int dbo_update_sc_state_desc(const char*sid,int st, char *senddesc, int sendcode);	
	
	/*
	 *����Ӱ��ID
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
