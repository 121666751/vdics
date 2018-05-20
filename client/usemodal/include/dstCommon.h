#ifndef __DSTCOMMON_H__
#define __DSTCOMMON_H__

#include <list>
using namespace std ;

#define ST_STOP         0
#define ST_RUN          1
#define ST_PAUSE        2
#define ST_INIT_DIST    3
#define ST_DIST			4
#define ST_RE_DIST		5
#define ST_ERR_VCH		6
#define MD_FIRST        1
#define MD_SECOND       2
#define MD_THIRD        3
#define ZWM_DIST_CHECK	WM_APP+307

#define DC_DEBIT	0			//�跽
#define DC_CREDIT	1			//����

#define VCH_MAX_COUNT	5000

enum
{
		INVALID_POCKET  = 0,
		CHECK_POCKET    = 1,     // CHECK
		DJVCH_POCKET    = 2,     // ����ƾ֤
		DFKX_POCKET		= 3,     // �����������
		DSKX_POCKET		= 4,     // ���տ������
		BPQT_POCKET		= 5,     // ��Ʊ������ר�ý��ƾ֤�������跽ƾ֤
		SDQT_POCKET		= 6,     // ˰������������ƾ֤
		JFXF_POCKET		= 7,     // �跽ר���ŷ�
		DFXF_POCKET		= 8,     // ����ר���ŷ�
		JFTP_POCKET     = 9,     // �跽��Ʊר��ƾ֤
		DFTP_POCKET     = 10	 // ������Ʊר��ƾ֤
};

struct DISTDATA
{
	char   codeline[64];
	char   ocr[64];
	char   currency[4];
    char   vchno[10];
    char   exchno[11];
    char   codeacc[22];
	int    cdcode;
	char   vchtype[4];			// type from int to short to add 
	char   pbcvt[4];	     	// pbc  voucher type code.
    char   VchKindCod[7];
    char   VchKindTxt[24];
    char   amount[21];
    int    PockNum;
    int    proctype;
    char   PockCode[9];
    char   PockText[24];
    bool   modified;
    char   date[12];
	char   session[4];
	char   clerk[12];
    char   fname[40];
    char   rname[40];
    int    fcount;
    double calx;
    double caly;
    int    dpi;
    char   ip[20];
    char   prefix[4];
    int    isEnvelop;
    char   pkgno[20];
};

typedef list<DISTDATA> DISTDATALIST;

struct DISTCTRLDATA
{
	int dstMode;		// MD_FIRST or MD_SECOND
	int dstState;		// state of distor
	int currency;
	int netno;
	int vch_tc;
	bool isTallyMode;		// flag that whether use tally mode. true to use tally
							// and false to use old mode when second sort
};


struct IDResDistItem
{
    char Account[22];
	char NetNo[11];
    char VchNo[10];
	char Money[12];
	int  VchKind;
    int  DistCode;
	char DistSign;
	int  Operator;
	double OperateTime;
	int  VchDistNo;
};

#endif //__DSTCOMMON_H__
 