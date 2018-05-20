#ifndef _LINUX_DB_COMMON_H
#define _LINUX_DB_COMMON_H

#include <zsocket.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <vector>
#include <myplus.h>
#include <gsip_util.h>
#include <ConfigFile.h>
#include <ExportLog.h>
#include <gsipcom.h>

#define MAX_SQL_LEN	1024
#define MAX_SEC_OUT 180

#define ERR_SQL_QUERY 					401
#define SQL_NO_RESULT					1
#define COMPUTE_NORMAL                  0              

#define COMPUTE_STARTED                 1              
#define COMPUTE_SUCCESSED               2              

#define COMPUTE_GETIMAGE_ERROR          3              
#define COMPUTE_GETSTAMP_ERROR          4              
#define COMPUTE_ERROR                   5              
#define COMPUTE_SAVERESULT_ERROR        6              

#define SEND_NORMAL                     0              
#define SEND_SUCCESSFUL                 1              
#define SEND_ERROR                      2              

#define SEND_RETRY_COUNT                3              

#define MANUAL_FINISHED                 2              
#define MANUAL_NORMAL                   0              

#define COMPUTE_RESULT_PASS             1              
#define COMPUTE_RESULT_NOTPASS          0              

#define SEND_RESULT_COUNT               100            

#define GETIMAGE_ERROR                  31             
#define SAVERESULT_ERROR                61             

using namespace std;

class CDBCommon
{
public:
	CDBCommon();
	~CDBCommon();
	
	int DbReOpen();

	int DbOpen(const char *,const char*,const char *,const char *);

	void DbClose();

	int GetVouchersSIDSList(vector<unsigned long> &vc_set, int lim_num, int date, int session, const char *brno);

	int GetCinputSIDSList(vector<unsigned long> &vc_set, int lim_num, int date, int session, const char *brno);

	int GetCinputSendList(vector<unsigned long> &vcu_set, int lim_num, int date, int session, const char *brno);

	int GetVouchersSendList(vector<unsigned long> &vcu_set, int lim_num, int date, int session, const char *brno);

	int GetMultisParseList(vector<unsigned long> &mt_set, int lim_num, int date, int session, const char *brno);

	int GetMultisSendList(vector<unsigned long> &mtu_set, int lim_num, int date, int session, const char *brno);

	int GetAuthInputParseList(vector<unsigned long> &ai_set, int lim_num, int date, int session);

	int GetAuthInputSendList(vector<unsigned long> &aiu_set, int lim_num, int date, int session);

	int GetAuthInputRowBySid(const char *sid, struct tb_authinput_s *v);

	int UpdateAuthInputParse(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model ,int state, int isAuthInput, int mainflag);

	int UpdateVouchersOutDateRows(int out_sec);

	int UpdateMultisOutDateRows(int out_sec);

	int UpMultisNotEnv(unsigned long sid, int model ,int state);

	int UpdateVouchersDists( unsigned long vouchers_sid, unsigned long dists_sid);

	int GetVouchersRowBySid(const char *sid, struct tb_vouchers_s *v);

	int GetCinputRowBySid(const char *sid, struct tb_vouchers_s *v);

	int GetMultisRowBySid(const char *sid, struct tb_multis_s *v);

	//int GetModelByVchtype( char *model, const char *vchtype);

	int UpColumnByPK(const char *pk_name, const unsigned long pk_val, const char *tb_name, const char *col_name, const char *col_val);

	int UpColumnByPK_1(const char *pk_name, const unsigned long pk_val, const char *tb_name, const char *col_name, const char *col_val, int hasDot);

	int MatchVouchersPcb(char *vchno, char *exchno, char *accno, char *amount, char *vchtype, char *area, int session, char* date);

	int MatchPcbQF(char *vchno, char *exchno, char *accno, char *amount, char *vchtype, char *area, int session, char* date);

	int UpdateVouchersPcb(unsigned long vouchers_sid, unsigned long pcb_sid);

	int get_tb_config( StrDict& cfg_dic);

	int get_tb_bocnets( StrDict& boc_dic);

	int UpdateMultisParse(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model ,int state, int modeflag);

	int UpdateVouchersParse(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model ,int state, int modeflag);

	int UpdateCinputParse(unsigned long sid, char *accno, char *vchno, char *exchno, char *vchtype, char *amount, int model ,int state, int modeflag);

	int UpdateParseTimeOut( char *tb_name);

	int UpdateSendTimeOut( char *tb_name);

	int GetTaskLock(char *tb_name,char *sid_name, unsigned long sid_val,char *col_name, int col_val);

	//写入明细数据
	int PutPbcMXData( pbc_header_t *h, pbc_mx_t *m);
	
	//写入清算数据
	int PutPbcQSData( pbc_header_t *h, pbc_qs_t *q);

	//获取人行清算数据
	int GetPbcQSData( pbc_header_t *h, vector<pbc_qs_t>& mx_set);

	//获取人明细算数据	
	int GetPbcMXData( pbc_header_t *h, vector<pbc_mx_t>& qs_set);

	int get_model_vchtype( StrDict& cfg_dic);

	int GetVchtypeByModel( StrDict& cfg_dic);
	
	int HasVersion();
	
	int GetModelByVchtype( const char * vchtype, const char* area);

	int GetVchtypeByModel( int model, char* area, char * vchtype);

	int GetCmpMerByExchno(char *area, char *exchno, int *compare, int *merge);
	
	int SaveCtrl(const char*date,const char*session,const char*area,const char*exchno,const char*dramount,const char*cramount,int flag);

	int SavectrlNotExist(const char*date,const char*session,const char*area);
private:
	MyDb db;
	char host[50];
	char uid[50];
	char pwd[50];
	char dbAlias[50];
};

#endif
