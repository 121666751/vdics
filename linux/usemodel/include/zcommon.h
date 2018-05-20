
#ifndef __ZCOMMON_H__
#define __ZCOMMON_H__


#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <map>
using namespace std;

typedef  map<string,string> PocketMap;



#define   PRIV_MOD_PWD      1                  //修改密码
#define   PRIV_GRD_NOM      2                  //业务柜员
#define   PRIV_GRD_ADM      4                  //高级管理员
#define   PRIV_GRD_MID      8                  //中级柜员
#define   PRIV_GRD_CUS      16                 //自定义柜员
#define   PRIV_OPT_CLK      32                 //柜员管理
#define   PRIV_OPT_DEP      64                 //机构管理
#define   PRIV_OPT_VCH      128                //凭证管理
#define   PRIV_OPT_CFG      256                //配置管理
#define   PRIV_OPT_PBC      512                //数据导入
#define   PRIV_OPT_DST      1024               //清分
#define   PRIV_OPT_SCN      2048               //扫描
#define   PRIV_OPT_VOU      4096               //单票处理
#define   PRIV_OPT_ENV      8192               //信封处理
#define   PRIV_OPT_UPD      16384              //票据上传
#define   PRIV_OPT_REP      32768              //影像替换


#define   EXP_PRIV_CLK					"柜员管理"
#define   EXP_PRIV_DEP					"机构管理"
#define   EXP_PRIV_VCH					"凭证管理"
#define	  EXP_PRIV_CFG					"配置管理"
#define	  EXP_PRIV_PBC					"人行数据"
#define	  EXP_PRIV_DST					"票据清分"
#define	  EXP_PRIV_SCN					"批量扫描"
#define	  EXP_PRIV_VOU					"票据处理"
#define	  EXP_PRIV_ENV					"信封处理"
#define	  EXP_PRIV_SPC					"特色业务"
#define	  EXP_PRIV_UPD					"票据上传"
#define	  EXP_PRIV_REP					"影像替换"


#define    BRNO_BJ                         "110001"             //北京
#define    BRNO_NJ                         "320001"             //南京

// ************************************************************************
// struct for MIL
// ************************************************************************
struct MIL_GROUP
{
	long App;
	long Sys;
	long Disp;
};


#include <ztime.hpp>
#include <myplus.h>
#include <zstream.hpp>
#include <zfile.hpp>
#include <znetdef.h>
#include <zvchtran.h>
#include <zvchinfodef.h>
#include <cisnfun.h>
#include <zerrno.h>



class MyDbEx : public MyDb
{
public:
    int Open() { return MyDb::Open(LOCAL_DB_HOST,LDB_USERNAME,LDB_PASSWORD,LOCAL_DB_NAME); }
};

#define DebugMessage(a) 	MessageBox(GetTopWindow(NULL),a,"信息",MB_OK|MB_ICONWARNING)


inline char* GetCurrencyName( CURRENCY_SYMBOL symVal, char* nameBuf, int bufLen )
{
    switch(symVal)
    {
    case CS_RMB:   strncpy( nameBuf, "RMB", bufLen );  break;
    case CS_GBP:   strncpy( nameBuf, "GBP", bufLen );  break;
    case CS_HKD:   strncpy( nameBuf, "HKD", bufLen );  break;
    case CS_USD:   strncpy( nameBuf, "USD", bufLen );  break;
    case CS_CHF:   strncpy( nameBuf, "CHF", bufLen );  break;
    case CS_DEM:   strncpy( nameBuf, "DEM", bufLen );  break;
    case CS_FRF:   strncpy( nameBuf, "FRF", bufLen );  break;
    case CS_SGD:   strncpy( nameBuf, "SGD", bufLen );  break;
    case CS_NLG:   strncpy( nameBuf, "NLG", bufLen );  break;
    case CS_SEK:   strncpy( nameBuf, "SEK", bufLen );  break;
    case CS_JPY:   strncpy( nameBuf, "JPY", bufLen );  break;
    case CS_EUR:   strncpy( nameBuf, "EUR", bufLen );  break;
    default :      strncpy( nameBuf, "未知币种", bufLen );  break;
    }

    return nameBuf;
}


inline void debugout(const char* fmt, ...)
{
	char buf[512];
    try{
        va_list ap;
        va_start(ap,fmt);
        _vsnprintf(buf,sizeof(buf)-1,fmt,ap);
        va_end(ap);

        OutputDebugString(buf);
    }
    catch(...)
    {
    	OutputDebugString("exception in construct debug message\n");
    }
}

/**
 * convert string into int, more safe than atoi
 */ 
inline int stoi(string s)
{
	if( s.empty()) return 0;
	return atoi(s.c_str());
}

inline string itos(int i)
{
	stringstream c;
	string out;
	c << i;
	c >> out;
	return out;
}

struct  pbc_node_t
{
	int sid;				/**<流水号*/
	char date[12];			/**<日期*/
	char session[4];		/**<场次*/
	char exchno[14];		/**<提入交换行号*/
	char vsexno[14];		/**<提出交换行号*/
	char leader[14];		/**<提入交换行所属机构*/
	char vchtype[4];		/**<凭证类别*/
	char cdcode[2];			/**<借贷标志*/
	char distno[12];		/**<清分流水*/
	char qfjh[4];
	char accno[32];			/**<账号*/
	char accname[60];		/**<名称*/
	char vsaccno[32];		/**<对方账号*/
	char vsname[60];		/**<对方名称*/
	char vchno[12];			/**<票据号*/
	char amount[16];		/**<票据金额*/
	char amttype[2];		/**<大小额标志H/L*/
	char paper[2];			/**<纸质标志Y/N*/
	char paycode[24];		/**<支付密码*/
	char fname[80];			/**<正面图名*/
	char bname[80];			/**<反面图名*/
	int  envelop   ;        /**<是否为信封*/
	char ip[24];			/**<ip地址*/
	char reserved[512];		/**<保留*/
};


typedef list<pbc_node_t> PBCLIST, *LPPBCLIST;


typedef struct DIST_TASK
{
    char imageid[28];
    char brno[8];
    char accdate[10];
    char areano[8];
    char exchno[14];
    char session[4];
    char ccycode[4];
    char imgtype[4];
    char reserved[60];
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
    char vchno[10];
    char actamt[16];
    char payact[24];
    char smr[64];
    char reserved[80];
}distDataT;

typedef struct DIST_CFG_T
{
    HWND    hWnd;
    char signip[24];
    char ecmip[24];
    char flowip[24];
    int  signport;
    int  ecmport;
    int  flowport;
    char area[8];
    char brno[8];
    int  needmoney;
    int  ratef;
    int  rateb;
    char order[100];
    int  mode;
    char imgpath[120];     
}distCFGT;

typedef struct DIST_FILE_LIST
{
    char fileFullName[160];
    char fileName[80];
}distFileList;

typedef vector<distFileList> FileVector;

#endif // __ZVCHTRAN_H__