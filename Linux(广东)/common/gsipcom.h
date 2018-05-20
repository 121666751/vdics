#ifndef __GSIP_COMMON_H
#define __GSIP_COMMON_H
#include <sstream>
#include <list>
#include <vector>
#include <map>
using namespace std;


typedef map<string,string> KeyMap;

typedef struct SCAN_INFO_T
{
	int  sid;
	char fname[80];
	char bname[80];
	char accno[32];
	char vchno[31];
	char vchtype[5];
	char exchno[14];
	char amount[17];
	char ipaddr[24];
	char date[12];
	char sess[4];
	char imageid[48];
	int	 port;
	int  rotate;
	int  dpi;
	char clkno[12];
	char pkgno[14];
	char area[12];
	char banktype[14];
	int	 multiflag;
	char modeflag[2];
	int  sendstate;
	char reserved[436];
	char orgcode[20];

}scanInfoT;

typedef struct DIST_TASK_T
{
    char orgcode[12];
    char clkno[8];
    char imageid[33];
    char pageno[11];
    char brno[12];
    char accdate[10];
    char areano[8];
    char exchno[14];
    char session[4];
    char ccycode[4];
    char imgtype[4];
    char modeflag[2];
    char reserved[58];
}distTaskT;

typedef struct DIST_DATA_T
{
    char imageid[28];
    char accdate[10];
    char areano[8];
    char exchno[14];
    char session[4];
    char ccycode[4];
    char ovchtype[4];
    char brno[8];
    char rpflg[2];
    char vchtype[4];
    char vchno[31];
    char actamt[16];
    char payact[24];
    char smr[64];
	char modeflag[2];
    char reserved[78];
}distDataT;


typedef struct DIST_FILE_LIST
{
    char fileFullName[160];
    char fileName[80];
}distFileList;

typedef vector<distFileList> FileVector;


//人行清算明细数据
#define PBC_HEADER_DATE_LEN         8
#define PBC_HEADER_AREA_LEN         6
#define PBC_HEADER_SESSION_LEN      3
#define PBC_HEADER_BRNO_LEN         11

typedef struct pbc_data_header_s pbc_header_t;

typedef struct pbc_qs_data_s    pbc_qs_t;

typedef struct pbc_mx_data_s    pbc_mx_t;

//人行数据包头
struct pbc_data_header_s
{
    //会计日期
    char date[9];

    //交换区域
    char area[7];

    //交换场次
    char session[4];

    //分行号
    char brno[7];
};

//人行清算数据结构
struct pbc_qs_data_s
{
    //交换号
    char exchno[14];

    //币种
    char cntype[4];

    //提出贷方结算金额 LEN=16
    char ccamount[16];
    
    //提出借方结算金额
    char dcamount[16];

    //提回贷方结算金额
    char cramount[16];

    //提回借方结算金额
    char dramount[16];

    //人行贷方差额
    char ccsub[16];

    //人行借方差额
    char ddsub[16];
};

//人行明细数据结构
struct pbc_mx_data_s
{
    //交换号
    char exchno[14];

    //币种
    char cntype[4];

    //交易金额
    char amount[16];

    //客户账号  
    char accno[22];

    //凭证号
    char vchno[31];

    //凭证类型
    char vchtype[4];

    //凭证日期
    char vchdate[9];

    //付款账号
    char payno[33];

    //付款人姓名
    char payname[61];

    //收款人账号
    char rcvno[33];

    //收款人姓名
    char rcvname[61];

    //概述
    char summary[61];

    //借贷标志  
    char cdcode[2];
};

//委收结构体
struct tb_authinput_s
{
	unsigned long sid;
	char fname[80];
	char bname[80];
	int rotate;
	char ip[24];
	short dpi;
	char date[30];
	char exchno[16];
	char pkgno[14];
	short session;
	short state;
	char vchno[12];
	char accno[32];
	char vchtype [4];
	short multiflag;
	char amount[16];
	short model;
	unsigned long mainid;
	unsigned long envid;
	short mainflag;
	short checkstate;
	short mergestate;
	unsigned long mergeid;
	char imageid[80];
	short sendstate;
	char timestamp[24];
	short submitstate;
	char scantime[24];
	short prestate;
	char clkno[12];
	short times;
	char area[9];
	short isEnv;
};

#endif
