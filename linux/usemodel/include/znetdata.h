 #ifndef __ZNETDATA_H__
#define __ZNETDATA_H__

#include "zerrno.h"
#include "txproto.h"

enum {
    SID_LOGIN       = 1001,     /**< clerk login*/
    SID_SVLOG       = 1002,     /**< save log*/
    SID_SNLOG       = 1003,     /**< new save log*/
    SID_CAGET		= 1010,		/**< load cachet seal from center.*/
    SID_CASET		= 1011,		/**< save cachet saal to center.*/
	SID_UPSTM		= 1020,		/**< set account to centersrv */
	SID_DLSTM		= 1021,		/**< get account from centersrv */
	SID_UPMOD		= 1022,		/**< up load voucher module */
	SID_DLMOD		= 1023,		/**< down load voucher module */
	SID_LKACC		= 1030,		/**< create account link */
	SID_ACCMA		= 1031,		/**< manage account */
	
	SID_ACCVT		= 1050,		/**< query account,exist or not */

	SID_OPVCH		= 1100,		/**< operator voucher, such as get accno from voucher */

    SID_TUACC       = 1201,     /**< get account info by tu_call.*/
    SID_TUCLK       = 1202,     /**< get clerk info by tu_call.*/
    SID_TUVCH       = 1203,     /**< get vchinfo by tu_call*/
    SID_TUTAL       = 1208,     /**< tally by tu_call*/
    SID_ACCDT       = 1311,		/**< get account info from bank*/
    SID_TALLY		= 1320,		/**< auto tally*/
    SID_VCHNO       = 1321,     /**< make one vchno*/
    SID_LHSWP		= 1325,		/**< luanch auto tally swap by tu_call*/
    SID_TLRES		= 1326,		/**< request auto tally result by tu_call*/
    SID_CLKSG		= 1328,		/**< clerk sign or leave by tu_call*/
    SID_TRUST		= 1330,		/**< get trust info by tu_call */
    
    SID_CLKMA       = 1401,     /**< clerk manage*/
    SID_ACPWD	    = 1411,     /**< get/set account password*/
    SID_STMCK		= 1421,		/**< stamp check service */
	SID_RESID		= 1430,		/**< result reidentify service */

    SID_AUTAC       = 1501,     /**< trans file in auto account */
    SID_CENTL       = 1601,     /**< centralized deal ,write/read warrant request */
    SID_GWKEY		= 2000,		/**< get Working Key from susrv.*/
    SID_APSWD		= 2010,		/**< create/verify account password at susrv */
    SID_VPSWD		= 2012,		/**< create/modify voucher password at susrv */
    SID_UPACC		= 2020,		/**< upload client and its stamp*/
    SID_DLACC		= 2021,		/**< download client and/or its stamp*/

	SID_IDGET		= 12100,		/**< download idcard information */
	SID_LKGET		= 12101,		/**< download idcard-accno link information */
	SID_IDSET		= 12102,		/**< upload idcard information */
	SID_LKSET		= 12103,		/**< upload idcard-accno link information */
    SID_LGGET       = 12104,
    SID_LGSET       = 12105,
    SID_CKNEW       = 12106,		/**< new clerk management*/
    SID_CKOLD       = 11401,		/**< old clerk + mysql */

//    SID_VPVRF       = 2013,      /* verify voucher password*/
/*    SID_VPSWD       = 1300,     /* process voucher password*/
/*    SID_VPMOD       = 1301,     /* modify voucher password*/
/*    SID_VPVRF       = 1302,     /* verify voucher password*/
};



/*
**************************************************************************
SID_LOGIN:  ->txRequest | txLoginRes->
SID_SVLOG

SID_VPSWD
SID_VPMOD
SID_VPVRF

SID_ACCDT:	->txRequest | txClientInfo->

SID_CLKMA


**************************************************************************
*/


typedef struct _OperLogT
{
    char    type[4];                /* log type in string 000-999*/
    long    time;                   /* ansi time in time_t format*/
    char    event[4];               /* log event in string 000-999*/
    char    clkno[12];              /* clerk no*/
    char    orgip[16];              /* client's IP address.*/
    char    result[4];              /* operate result. 000-999*/
    char    text[100];              /* log text*/
} OperLogT;
    
    
    
/************************************************************************
 Clerk Manage data format.
************************************************************************/
typedef struct _ClerkManT
{
    char    mode[4];        /**< "MN"-New, "MM"-Modify, "MD"-Delete,*/
                            /**< "MS"-Show, "ML"-list All of this net*/
    txTellerInfo    info;
} ClerkManT;

typedef struct _ClerkListItem
{
    char    clerkno[12];
    char    name[20];
    char    state[4];
    char    grade[4];
} ClerkListItem;

typedef struct _ClerkList
{
    long    count;
    ClerkListItem item[1];
} ClerkList;


typedef struct _txLoginRes
{
    char    clkno[12];
    long    priv;
    long    grade;
	char    netinfo[40];	/**< Format:"netno depno netname*/
	long    systime;
    long    cs_result;
} txLoginRes;


/**
****************************************************************
����1211��
��;���ϴ��ͻ���Ϣ
ͨѶ·�ߣ�������ӡ��������>������ӡ������
���������txClientData
������������
����ֵ��0-�ɹ�������ʧ��

˵����
����ṹtxtClientData��
(1) ��Աreq
eID=1202�� mode=�ṹ���а����Ķ�̬���飨ӡ������txStampInfo����
������
(2) ��Աclient
�ͻ�������Ϣ�����亯��ֻ��Ҫ���ո��ֶ��������ͽ��д��ͣ������˽�
����庬�塣
(3) ��Աstamp_ptr
�ó�ԱΪһtxStampInfo���͵�����ָ�룬ָ��̬�����txStampInfo
���͵����飬����������req.mode�С�

��txStampInfo�У�imagedata_ptrָ��ӡ��ͼ�����ݵ��ڴ��ַ������
Ϊimagelen��

����0�����ϴ��ɹ�����������ϴ�ʧ��
****************************************************************
*/

typedef struct _txStampInfo
{
	char	accno[36];
	long    state;
    char    libno[4];
    char    type[4];
    char    color[4];
    long    modifyno;
    long 	clientid;
	char    enabledate[10];
    char    disabledate[10];
    long    imagelen;           /* ͼ�����ݳ���*/
    void*   imagedata_ptr;      /* ͼ������ָ��*/
} txStampInfo;

typedef struct _txClientData
{
    txRequest       req;
    txClientInfo    client;
    txStampInfo     *stamp_ptr;
    char            tx_result[8];/*����ִ�н��*/
} txClientData;


/*
****************************************************************
����1221��
��;�����ؿͻ���Ϣ
ͨѶ·�ߣ�������ӡ��������>������ӡ������
���������txRequest
����������txClientData
����ֵ��0-�ɹ�������ʧ��

˵����
���������
eID=1221, ���������ʵ���ͼ���

����ṹtxClientData��
(1) ��Աreq
eID=1221�� mode=�ṹ���а����Ķ�̬���飨ӡ������txStampInfo����
������
(2) ��Աclient
�ͻ�������Ϣ�����亯��ֻ��Ҫ���ո��ֶ��������ͽ��д��ͣ������˽�
����庬�塣
(3) ��Աstamp_ptr
�ó�ԱΪһtxStampInfo���͵�����ָ�룬ָ��̬�����txStampInfo
���͵����飬����������req.mode�С�

��txStampInfo�У�imagedata_ptrָ��ӡ��ͼ�����ݵ��ڴ��ַ������
Ϊimagelen��

����0�������سɹ��������������ʧ��

���ݽ��չ���������Ҫ��̬�����ڴ棬Ӧ�ṩ��Ӧ���ͷź��������ڴ�
�ͷš�
****************************************************************
*/


#ifdef __cplusplus
extern "C" {
#endif


/** 
 *************************************************************************
 alloc cs handle
 @return: handle for cs_functions processing
 ************************************************************************/
void* cs_open();            


/** 
 ***********************************************************************
 send data to centersrv and auto store result
 cs_call/cs_call_ex only send to host "center_server:8088"
 cs_call_host/cs_call_host_ex can send data to anywhere.
 cs_call_ex/cs_call_host_ex will compress before transfer.

 @param cshandle: cs_open() alloced handle
 @param sid:	  service id.
 @param svcdata:  pointer to data that to be send
 @param len:      length of data to be send
 @param host:	  server' host address. its format like "host_name:port" or
				  "x.x.x.x:port"
 @return: 0-success 
 ************************************************************************/
int cs_call(void* cshandle, int sid, void* svcdata, int len);
int cs_call_ex(void* cshandle, int sid, void* svcdata, int len);
int cs_call_host(void* cshandle, int sid, void* svcdata, int len, const char *host);
int cs_call_host_ex(void* cshandle, int sid, void* svcdata, int len, const char *host);


/** ***********************************************************************
* get stored result
* @param cshandle: cs_open() alloced handle
* @return: result's buffer pointer and store len into len
* ************************************************************************/
void* cs_result(void* cshandle, int* len);


/** ***********************************************************************
* close and free
* @param cshandle: cs_open() alloced handle
* ************************************************************************/
void cs_close(void* cshandle);



/** ***********************************************************************
*  return data  from centersrv to client
*  cs_return_ex will compress before transfer.
*  @return: 0-success
* ************************************************************************/
int cs_return(int sid, void* data, int len);
int cs_return_ex(int sid, void* data, int len);



/** ***********************************************************************
*  write log data
*  logdata: multi <XX>***</XX> section of xml node
*  refdata: binary extern refence file of yencode
*  return: 0-success
* ************************************************************************/
typedef struct CS_LOGPARAM
{			
	int  mode;
	int  serialid;
	char event[80];
	char clkno[12]; 
	char depno[12]; 
	long time;
	char verify[20];
	char *logdata;
	char *refdata;
} CS_LOGPARAM;

typedef struct CS_NEWLOGPARAM
{
	int serialid;
	int eventid;
	char clkno[10]; 
	char clkname[22];
	char warrantclk[10];
	char targetclk[10]; 
	char depno[12];
	char time[10]; 
	char date[10]; 
	int  result;
	char accno[42];  
	char vchno[22];
	char vchtype[6];
	char vchmoney[20];
	char vchdate[10];
	int  sysidres;
	int  manidres;
	char sysstpres[42];
	char manstpres[42];
	char vchimgid[22];
	char stampimgid[22];
	char imgtype[12];
	int  vchpwdres;
	char contractno[42];
	char memo[102];
} CS_NEWLOGPARAM;

int cs_wlog(CS_LOGPARAM *log_param);

int cs_wlog_new(CS_NEWLOGPARAM *log_param);



/* ***********************************************************************
*  encode password string.
*  @param	in:		password to be encycripted.
*  @param   out:	password after encycripted. it at lease 18 bytes.
*  @return: out
* ************************************************************************/
char* cs_passwd(char* in, char* out);

/* ***********************************************************************
*  get clientinfo from bank system by centersrv.
*  @param accno:	account no
*  @param clkno:	clerk no 
*  @param depno:	department no of the clerk
*  @param retClient: pointer of txClientInfo that receive client info.
*  return: 0-success
* ************************************************************************/
int cs_accdt(char* accno, char* clkno, char* depno, txClientInfo* retClient);

/** ***********************************************************************
*  get WKEY(working key) from center or supercenter
*  @parma	out: caller alloced buffer to store the WKEY. it must alloced at least
*        36 bytes.
*  @param   depno: department no.
*  @return: 0-success
* ************************************************************************/
int cs_wkey(char* depno, char* out);
int cs_wkey_force(char* depno, char* out);

/** ***********************************************************************
*  decode errno from xml-format' result
*  @param	res_buf: the result buffer contain errno in xml format
*  @return: errno decoded from xml result
* ************************************************************************/
int cs_res_errno(const char* res_buf);

/** ***********************************************************************
*  decode errstr from xml-format' result
*  @param	res_buf: the result buffer contain errno in xml format
*  @return: errstr decoded from xml result
* ************************************************************************/
const char* cs_res_errstr(const char* res_buf);

/**
 *	@param accno:	account  which need to be created
 *  @param compname: company name of this new account
 *  @param orgAccno: origin account, which new account to link to 
 *  @param outError: [out] buffer to receive error message. it must at lease has 128 bytes.
 *  @return 0-success.
 *  create account linkage, so new account can use origen account's
 *  stamp directly.
 */
int cs_create_acclink(const char* accno, 
					  const char* compname, 
					  const char* orgAccount, 
					  char* outError);


/**
 *	increase g_bgthread_count value
 *	@return thread count;
 */
int cs_thread_increase();

/**
 *	decrease g_bgthread_count value
 *	@return thread count;
 */
int cs_thread_decrease();

/**
 *	read g_bgthread_cout value
 *	@return thread count;
 */
int cs_thread_count();

/**
 *	@param login:	clerk who operate
 *  @param accno:	account
 *  @param state:	(-100)��ɾ����0����Ч��1��������2��������3�����ᣬ4��δ���ˣ�5������ʧ��
 *	@return 0���ɹ�;
 *  manage account state
 */
struct csLogin;
int cs_manage_acc(csLogin* login, const char* accno, int state);



typedef struct ACCFLAGS
{
	int acc_state;		/**< �ͻ�״̬ 1-OK */
	int passwdflag;		/**< �Ƿ�ʹ�ÿͻ����� 0-NO, 1-YES */
	int stm1_state;		/**< ӡ����1 ״̬ */
	int stm2_state;		/**< ӡ����2 ״̬ */
	int stm_change;		/**< �Ƿ�λ���¾�ӡ�������� 0-NO, 1-YES */
	int cht_state;		/**< ����״̬ */
	char accno[40];		/**< �ͻ��ʺ� */
	char reserved[60]; /**< not used */
}ACCFLAGS;

/**
 * @param accno: account
 * @param outFlag: buffer to receive account state
 * @return 0-��ѯ�ɹ�, 711-�ͻ������ڣ� other-error
 * �����ʺŲ�ѯ�ͻ�״̬��Ϣ
 */
int cs_get_accflags(const char* accno, ACCFLAGS* outFlag);


/**
 * @param vchno: voucher no
 * @param vchtype: voucher type
 * @param outFlag: buffer to receive account state
 * @return 0-��ѯ�ɹ�, 711-�ͻ������ڣ� other-error
 * ����ƾ֤��Ϣ��ѯ�ͻ�״̬��Ϣ
 */
int cs_get_vchacc(const char* vchno, int vchtype, ACCFLAGS* outFlag);





/** ***********************************************************************
*  write warrant request or read warrant result
*  return: 0-success
* ************************************************************************/

typedef struct _CS_CENTRALIZE
{
	int mode;			
	char serialid[10];
	char imageid[20];
	char ip[42];
	int  handle;
	int  idwarrant;
	int  amtwarrant;
	int  idresult;
	int  amtresult;
	char memo[250];
	char date[10];
	char time[8];
	char warclkno[10];
	char wardepno[12];
	char idclkno[10];
	char iddepno[12];
	char accno[42];
	char vchno[22];
	char vchtype[6];
	char vchmoney[20];
	char vchdate[10];
	char sysstpres[42];
	char manstpres[42];
	int  sysidres;
	int  manidres;			
	char *imageA;
	char  imageAFmt[6];  	/**����Ʊ��ͼ���ʽ**/
	int  imageASize;		/**����Ʊ��ͼ���С**/
	int  imageAPosi;  		/**����Ʊ��ͼ���������洢�ռ���λ��**/
	char *imageB;
	char  imageBFmt[6]; 	/**����Ʊ��ͼ���ʽ**/
	int  imageBSize;  		/**����Ʊ��ͼ���С**/
	int  imageBPosi;		/**����Ʊ��ͼ���������洢�ռ���λ��**/
	char *imageRes;
	char imageResFmt[6]; 	/**�Ա�ͼ��ʽ**/
	int  imageResSize; 		/**�Ա�ͼ��С**/
	int  imageResPosi; 		/**�Ա�ͼ�������洢�ռ���λ��**/
} CS_CENTRALIZE;

typedef struct _SealData
{
	char serialno[20];
	char clkno[16];
    char depno[12];
    char accno[32];
    char vchno[16];
    char vchtype[4];
    char amount[20];
    char result[200];
    void* imagedata_ptr;   /* ͼ������ָ��*/
    long imagelen;         /* ͼ�����ݳ���*/
} SealData;

/**
 * @param centl: the struct above
 * @param sid: request serial id
 * @param centl: the structure of request warrant
 * @param resxml: the xml char list of query warrant result
 * @return 0-�ɹ�, other-error
 * �����ڼ��д������̨����������Ϣ��xml����ʽ��
 */
int cs_central(CS_CENTRALIZE *centl,char* sid,char* resxml);

/** ***********************************************************************
*  save errinfo in xml parse
*  return: 0-success
* ************************************************************************/

typedef struct _ERR_INFO
{
	int errno;
	char errstr[200];
} ERR_INFO;

/**
 * @param xmlstr: xml string
 * @param errinfo: include errno and errstr
 * @param centl: the structure after parsing xml
 * @return 0-�ɹ�, other-error
 * ���д����ȡ���ݿ���Ϣʱ������̨������xml��
 */

int cs_parse_cenxml(char* xmlstr,ERR_INFO* errinfo,CS_CENTRALIZE* centl);
/*************************************************************************/

/**
 * @param xmlstr: xml string
 * @param errinfo: include errno and errstr
 * @param centl: the structure after parsing xml
 * @return 0-�ɹ�, other-error
 * ���д����ȡ���ݿ���Ϣʱ������̨������xml��
 */

int cs_pipe_xml(char* xmlstr,SealData* pipexml);
/*************************************************************************/

/**
 * @param clkno: ��Ա��
 * @param clkinfo: the structure of clerk information
 * @return 0-�ɹ�, other-error
 * ���ݹ�Ա�ŵõ���Ա��Ϣ
 */

int cs_get_clerkinfo(char* clkno,csLogin* clkinfo);
/*************************************************************************/

/**
 * @param cmd: �����в���
 * @param clkinfo: the structure of clerk information
 * @return 0-�ɹ�, other-error
 * ���������в�����Ϣ�õ���Ա��Ϣ
 */

int cs_get_clklog_info(char* cmd,csLogin* clkinfo);
/*************************************************************************/

#ifdef __cplusplus
}
#endif


#endif /*__ZNETDATA_H__*/
