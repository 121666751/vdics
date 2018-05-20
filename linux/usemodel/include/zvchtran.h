#ifndef __ZVCHTRAN_H__
#define __ZVCHTRAN_H__


// ************************************************************************
// struct for Image data transfer
// ************************************************************************

#define VPC_OPERATOR		-1000
#define READY_FOR_RECV	    6000


enum { 
	SCANNER_FJ4750C = 0,
	SCANNER_FJ4990C = 1,
	SCANNER_KODAK	= 2,
};

// event id to imgsrv
enum {
	EID_IMG_DONE	=7000,
	EID_IMG_FAIL	=7001,
	EID_GET_IMG		=7002,
	EID_GET_FILE	=7010,
	EID_GET_FILE_EX	=7011,
	EID_PID_EXIT    =7020,
	EID_SCAN_RST    =7021,
	EID_NET_REST	=7022
};

//
// VPC_IMG_FILE can get an image from imgsrv by special filename.
// this usually used in watchid.
// this structure was also used when msgsrv find that one calculate program
// exit or dead, then msgsrv will send eID=EID_PID_EXIT and the pid's IP interger string value
// in FileName member. imgsrv can use atoi(FileName) to get IP interger value.
//

struct VPC_IMG_FILE
{
	int eID;
	char FileName[36];
};

enum {
	ISE_SUCCESS		=0,
	ISE_UNKNOW		=8000,
	ISE_NO_IMAGE	=8001,
	ISE_BMP_RGB		=8002,
	ISE_NOT_READY	=8003,
	ISE_NET_NOTDONE =8004,
	ISE_NET_NOTFOUND=8005,
	ISE_NET_DONE    =8006,
	ISE_MODE_ERR	=8007

};

struct IMGPKGHEADER
{
	int sizeX;
	int sizeY;
	short Dpi;
	char FileName[25];
	char ScannerType;
	unsigned long  fromHost;
	float calX;
	float calY;
};

struct IMGPKG
{
	IMGPKGHEADER Header;
	unsigned char pData[1];
};

struct IMGINFOHEADER
{
	char Netno[10];
	char Account[30];
	char Vchno[10];
	int Vchtype;
	float VchMoney;
	char Vchdate[30];
	char FileName[30];
	int FileSize;
};

struct IMGINFO
{
	IMGINFOHEADER Header;
	unsigned char pData[1];
};

// ************************************************************************
// Struct for broadcast info
// ************************************************************************
/*
struct RESBRD
{
	//##ModelId=3BC6562D033C
	int SerialID;
	//##ModelId=3BC6562D0346
	int SysGenRes;
};*/

struct VCHFACTORS
{
	int VchSerialNo;
	long mil_app;
	long mil_sys;
	int Netno;
	char Account[40];
	int Vchno;
	int Vchtype;
	__int64 VchMoney;
	double Vchdate;
	int	ImgSrvHost;
    int IDMode;
    char VchPassword[20];
    int Reserved;
};

//**********************************************************************
// message defined for exchange between voucher center computer groups.
//**********************************************************************


enum  MSGID {
    MID_UNKNOWN     = 0,

    MID_LOGIN       = 2001,
    MID_CHANGE_NET  = 2002,

    MID_MAN_ID      = 2010,
    MID_MAN_SAVE    = 2011,

    MID_SUBMITTING  = 2012,
    MID_SUBMIT_OK   = 2013,
    MID_SUBMIT_FAIL = 2014,

    MID_MAN_NEXT    = 2015,     // man id move to next but not save.

    MID_MAN_TIMEOUT = 2020,     // man id time out

    MID_SYS_ID      = 2050,     // system auto identify

    MID_SORT_FIRST  = 2060,     // first sort
    MID_SORT_SECOND = 2061,     // second sort

    MID_SORT_FIRST_DONE     = 2062,
    MID_SORT_SECOND_DONE    = 2063,


    MID_WORK_REFRESH= 2070,     // tell work list program to refresh and reload data.

    MID_RESCAN_NET  = 2071,     // tell imgsrv to delete all bad file

    MID_CANCEL_REC	= 2072,		// tell imgsrv to delete current id result. in fact, delete is fade, just hide it

    MID_SCANNING    = 2080,
    MID_SCAN_OVER   = 2081,

    MID_CANCEL      = 2500,     // any process want to cancel job

    MID_GET_SETUP   = 2550,
    MID_SET_SETUP   = 2551,

	MID_GET_IMGSRVS = 2555,		// get imgsrv hosts list.
};

enum PROGTYPE {
    PID_WATCHID     = 0,
    PID_CIDPROG     = 1,
    PID_SCANCTL     = 2,
    PID_SORTCTL     = 3,
    PID_IMGSRV      = 4,
    PID_WORKLST     = 5
};

// Message Result Error no
enum {
    MERR_SCANNING   = 4001,   // submit when current netno have'nt finished scan.
    MERR_NO_IMGSRV  = 4008,   // imgsrv not found.
    MERR_ID_DONE    = 4009,   // sys id not finished.
    MERR_ID_MATCH   = 4010,   // man id result do not match first sort result
    MERR_NO_RECORD  = 4011,
    MERR_MYSQL_DB   = 4100,
    MERR_
};

// State of msgsrv setup
enum {
    MST_HAS_SORTER      = 0x00000001,   // whether use sorter in zwzx
    MST_MORNING_SORTER  = 0X00000002,   // whether use sorter at morning
    MST_AFTERNOON_SORTER= 0X00000004,   // whether use sorter at afternoon
    MST_CURRENT_SORTER  = 0X00000008    // whether use sorter at now
};



struct MSG_STATE
{
    MSGID       msg_id;
    int         prog_type;       //PROGTYPE
    int         opreator;
    int         Tag;
    char        net_no[16];
};

struct MSG_INFO
{
    MSGID       msg_id;
    int         record_id;
    int         sys_res;
    int         man_res;		// when msg_id is MID_SYS_ID man_res stored net_no
    char        Reserved[16];
};

#define ZMSGBOX(a) MessageBox(0,a,0,MB_OK)
#endif // __ZVCHTRAN_H__
