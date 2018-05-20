
#ifndef __ZCOMMON_H__
#define __ZCOMMON_H__


#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <map>
using namespace std;

typedef  map<string,string> PocketMap;



#define   PRIV_MOD_PWD      1                  //�޸�����
#define   PRIV_GRD_NOM      2                  //ҵ���Ա
#define   PRIV_GRD_ADM      4                  //�߼�����Ա
#define   PRIV_GRD_MID      8                  //�м���Ա
#define   PRIV_GRD_CUS      16                 //�Զ����Ա
#define   PRIV_OPT_CLK      32                 //��Ա����
#define   PRIV_OPT_DEP      64                 //��������
#define   PRIV_OPT_VCH      128                //ƾ֤����
#define   PRIV_OPT_CFG      256                //���ù���
#define   PRIV_OPT_PBC      512                //���ݵ���
#define   PRIV_OPT_DST      1024               //���
#define   PRIV_OPT_SCN      2048               //ɨ��
#define   PRIV_OPT_VOU      4096               //��Ʊ����
#define   PRIV_OPT_ENV      8192               //�ŷ⴦��
#define   PRIV_OPT_UPD      16384              //Ʊ���ϴ�
#define   PRIV_OPT_REP      32768              //Ӱ���滻


#define   EXP_PRIV_CLK					"��Ա����"
#define   EXP_PRIV_DEP					"��������"
#define   EXP_PRIV_VCH					"ƾ֤����"
#define	  EXP_PRIV_CFG					"���ù���"
#define	  EXP_PRIV_PBC					"��������"
#define	  EXP_PRIV_DST					"Ʊ�����"
#define	  EXP_PRIV_SCN					"����ɨ��"
#define	  EXP_PRIV_VOU					"Ʊ�ݴ���"
#define	  EXP_PRIV_ENV					"�ŷ⴦��"
#define	  EXP_PRIV_SPC					"��ɫҵ��"
#define	  EXP_PRIV_UPD					"Ʊ���ϴ�"
#define	  EXP_PRIV_REP					"Ӱ���滻"


#define    BRNO_BJ                         "110001"             //����
#define    BRNO_NJ                         "320001"             //�Ͼ�

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

#define DebugMessage(a) 	MessageBox(GetTopWindow(NULL),a,"��Ϣ",MB_OK|MB_ICONWARNING)


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
    default :      strncpy( nameBuf, "δ֪����", bufLen );  break;
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
	int sid;				/**<��ˮ��*/
	char date[12];			/**<����*/
	char session[4];		/**<����*/
	char exchno[14];		/**<���뽻���к�*/
	char vsexno[14];		/**<��������к�*/
	char leader[14];		/**<���뽻������������*/
	char vchtype[4];		/**<ƾ֤���*/
	char cdcode[2];			/**<�����־*/
	char distno[12];		/**<�����ˮ*/
	char qfjh[4];
	char accno[32];			/**<�˺�*/
	char accname[60];		/**<����*/
	char vsaccno[32];		/**<�Է��˺�*/
	char vsname[60];		/**<�Է�����*/
	char vchno[12];			/**<Ʊ�ݺ�*/
	char amount[16];		/**<Ʊ�ݽ��*/
	char amttype[2];		/**<��С���־H/L*/
	char paper[2];			/**<ֽ�ʱ�־Y/N*/
	char paycode[24];		/**<֧������*/
	char fname[80];			/**<����ͼ��*/
	char bname[80];			/**<����ͼ��*/
	int  envelop   ;        /**<�Ƿ�Ϊ�ŷ�*/
	char ip[24];			/**<ip��ַ*/
	char reserved[512];		/**<����*/
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