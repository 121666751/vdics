#ifndef __ZVCHINFODEF_H__
#define __ZVCHINFODEF_H__
/**********************************************************************************/

#if defined( _MSC_VER ) || defined( __BORLANDC__ )
#pragma pack(4)
#endif

#define MAX_STAMP_SIZE	1024000
#define SOCK_BUFFER		5000
#define PRIVILEGE_COUNT 5

#define ct_AccLen  (20+1)
#define ct_PackSize  1024  	// in Bytes
//const int ct_TimeStrLen = 17;	// "YYYY-MM-DD hh:mm\0"


enum CURRENCY_SYMBOL
{
    CS_UNKNOWN      = 00,       // Unknown currency type.
    CS_RMB          = 01,       // chinese Ren Min Bi
    CS_GBP          = 12,       // Greatekingdom British Pound
    CS_HKD          = 13,       // HongKong Dollar
    CS_USD          = 14,       // United State Dollar
    CS_CHF          = 15,       // Helvetic Franic
    CS_DEM          = 16,       // DEutschland Mark
    CS_FRF          = 17,       // FRanch Franic
    CS_SGD          = 18,       // SinGapore Dollar
    CS_NLG          = 20,       // NetherLands Guilder
    CS_SEK          = 21,       // Sweden Krona
    CS_JPY          = 27,       // JaPan Yen
    CS_EUR          = 38        // EURope Yuan
};

#define AB_UNKNOWN  ""
#define AB_RMB  "人民币[RMB]"
#define AB_GBP  "英镑[GBP]"
#define AB_HKD  "港币[HKD]"
#define AB_USD  "美元[USD]"
#define AB_CHF  "瑞士法郎[CHF]"
#define AB_DEM  "德国马克[DEM]"
#define AB_FRF  "法郎[FRF]"
#define AB_SGD  "新加坡元[SGD]"
#define AB_NLG  "荷兰盾[NLG]"
#define AB_SEK  "瑞典克朗[SEK]"
#define AB_JPY  "日元[JPY]"
#define AB_EUR  "欧元[EUR]"

enum
{
	UNP_GS		= 0,
	UNP_G		= 16,
	UNP_S		= 32
};

/* Defined all of the Event */
enum _EventID
{
	//	Voucher Events
	EN_NULL 				= 0,
	EN_ADD_VCH				= 1,
	EN_UNADD_VCH			= 2,
	EN_LOSE_VCH				= 3,
	EN_RENEW_VCH			= 4,
	EN_GET_VCHINFO			= 5,
	EN_USE_VCH				= 6,
	EN_UNUSE_VCH			= 7,

	//	Image Upload Events
	EN_NEW_ACCOUNT			= 8,
	EN_CHANGE_STAMP			= 9,
	EN_RESCAN_STAMP_A		= 10,
	EN_RESCAN_STAMP_B		= 11,
	EN_MODIFY_ACCOUNT		= 12,

	//	Image Download Events
	EN_IDENTIFY				= 13,
	EN_RESUME_A				= 14,
	EN_RESUME_B				= 15,

	//	Special Account Events
	EN_NEED_ACCOUNT_ID		= 16,
	EN_RM_TMP_ACCOUNT		= 17,

	EN_DEL_ACCOUNT			= 18,
	EN_FREEZE_ACCOUNT		= 19,
	EN_RECOVER_ACCOUNT		= 20,

	EN_PRE_DEL_ACCOUNT		= 21,

	//	Voucher Sample Events
	EN_GET_NEW_SAMPLES		= 22,
	EN_UPDATE_SAMPLE		= 23,


	//	Download Account Info Events
	EN_GET_ACCOUNT_QUENE	= 24,
	EN_GET_ACCOUNT_DETAIL	= 25,

	//	Download Voucher Range
	EN_GET_VCH_RANGE		= 26,
	EN_GET_NETNAME			= 27,

	//	Adjust system time
	EN_GET_SYSTIME			= 28,

	//	Download All Account Info of One Net
	EN_GET_NET_ACCOUNTS		= 29,
	EN_GET_CLIENTINDEX		= 30,

	EN_BAT_RESUME_A			= 31,
	EN_BAT_RESUME_B			= 32,
	
	//
	EN_UPLOAD_SAMPLE			= 68,

	//	Update Center Holidays
	EN_UPLOAD_HOLIDAYS			= 70,

	//  Account Name Query
	EN_NEED_ACCOUNT_NAME		= 73,

	//	Server Admin
	EN_CSERVER_SHUTDOWN 		= 100,
	EN_LSERVER_SHUTDOWN			= 101,


	//  Custom Request
	EN_CUSTOM_CMD				= 110,

	// Get Client special info
	EN_GET_COMPANY_NAME			= 120,
	EN_GET_HISTORY_LIB			= 121,

	// TRANSFER NET SETTINGS
	EN_TRAN_NET_SETTING         = 130,

	// New Get System time event, with time control
	EN_GET_SYSTIME_EX           = 140,

	EN_LOGOUT                   = 141,

	EN_ACCOUNT_DIE              = 150,  // 销户

	EN_GET_WORK_STATUS          = 160,
	
	/***************************************************
		this request can download all cients and stamps
		of one net.
		in the taskinfo table, eID->200, autoID->netno
	***************************************************/
	// EN_GET_NET_STAMP			= 200, not realize by now
	
};

typedef int EVENTID, *LPEVENTID;


//**********************************************************
// NOTE: the following events are defined for DumpServer instead of
// CenterServer

enum {
	EN_QUERY_DUMP_INFO          = 1001,  //
	EN_UPLOAD_DUMP_DAT          = 1002

};

#define DUMP_PACK_SIZE       1024*100

typedef struct _DumpRes
{
    int         eID;
    int         LocalSize;
    int         CenterSize;
    int         CurPackageSize;
    char        DateString[20];
} DUMPRES;





/*	structure used in sending voucher info  */
typedef struct _VchInfo
{
    EVENTID     eID;
    int			ClientAutoID;
    int         iVchNo_A;
    int         iVchNo_B;
    double		sTime;
    char		sRomanCode[4];
    char		cVchType;

} VCHINFO, *LPVCHINFO;



/*	structure used in receive voucher operate result  */
#define FIELD_SEP_CHAR      "~"

typedef struct _VchResult
{
    EVENTID	    eID;
    int         iResult;
    double 		sTime;
    char        cOwner[ct_AccLen+10];

} VCHRESULT, *LPVCHRESULT;

/*	structure used in sending general info  */
typedef struct _SystimeResult
{
    EVENTID	    eID;
    int         iResult;
    double 		sTime;
    char        cOwner[ct_AccLen+20];       // first n bytes mean netno followed by FIELD_SEP_CHAR
    int         Privileges[PRIVILEGE_COUNT];

} SYSTIMERESULT, *LPSYSTIMERESULT;

//  Voucher Process
/////////////////////////////////////////////////////////////////////////////////////////////////
//  Image Process

/**************************************************************************

	struct _ImgRequest

	eID = EN_NEED_ACCOUNT_ID:
		[iNetNo]

***************************************************************************/
/*	structure used in IDENTIFY request */

typedef struct _ImgRequest
{
	EVENTID 	eID;
	int			ClientAutoID;
	double		dImgTime_A;
	double		dImgTime_B;
	double		dLastModify;

	int			iVchNo;
	char		cVchType;
	double		dVchTime;

	int			iNetNo;
	char		sAccount[ct_AccLen];
	char		cReason;		// 0 - Vch Only, 1 - Vch &  Account  2 - Account Only
//			int			iTag;

} IMGREQUEST, *LPIMGREQUEST;

/***************************************************************************

	struct _ImgReply

	eID = EN_NEED_ACCOUNT_ID:
		[iImgResult] : Server's Reply code,
		[iTagInfo]	 : Account ID.

***************************************************************************/
/*	Remote Img Server Reply Structure  */
typedef struct _ImgReply
{
	EVENTID		eID;
	int			iImgResult;
	
	/*  sAccTime[0] = 1 mean this account is in blacklist when eID=EN_IDENTIFY */
	char		sAccTime[ct_AccLen];		

	/*	iTagInfo means Totalbytes need download when eID == EN_IDENTIFY, and when
		eID == EN_RESUME_STAMP,	it means Restbyte need download  				*/
	int			iTagInfo;

	/*	iVchResult means  whether the client' stamp lib is changed 
		[0-no change, 1-changed].		*/
	int			iVchResult;
	int			iAccountType;

} IMGREPLY, *LPIMGREPLY;

/*	Upload Request and Resume of Download structure  */
typedef struct _ImgTransfer
{
	EVENTID		eID;

	int			ClientAutoID;
	double		sAccTime;

	int			iTotalBytes;
	int			iRecvBytes;

} IMGTRANSFER, *LPIMGTRANSFER;

/*	Package Structure  */
typedef struct _ImgPack
{
	int			iPackNo;
	int			iTotalCount;
	int			iPackSize;
	char		pData[1];
} IMGPACK, *LPIMGPACK;


/****************************************************************/
/* Define File Structures */
typedef struct tagGATEFILEHEADER
{
	int  iEventID;
	int  iTotalBytes;

} GATEFILEHEADER, *LPGATEFILEHEADER;

typedef struct tagACCOUNTINFO
{
	int		CenterAutoID;
	int		NetNo;
	char	Account[ct_AccLen];	//		varchar(20)	not null			,
	char	CompanyName[61];	//		varchar(60)	not null			,
	char	CompanyAddr[61];	//		varchar(60)
	char	PostalCode[7];		//	 	varchar(6)					,
	char	LinkMan[11];		//    	varchar(10)	not null			,
	char	LinkManID[21];		//		varchar(20)						,
	char	Telephone[21];		//		varchar(20)	        			,
	char	Fax[21];			//     	varchar(20)	        			,
	double	OpenDate;			//   	datetime   	not null			,
	int		OpenOperator;		//		int(4)						,
	double	EnableDate;			//		datetime	not null			,
	double	DisableDate;		//		datetime	not null			,
	short	State;				//     	smallint(2)					,
	double	LastModify;		//		datetime	not null			,
	short 	AccountType;		//		smallint	not null

} ACCOUNTINFO, *LPACCOUNTINFO;

// *************************************************************************
//  This structure is a replacement of old ACCOUNTINFO by added some new
//  fields. New version application should use this struct.
//  NOTE: Members of this structure will not corresponding to database fields,
//  so functions which perform operation between this structure and database
//  should translate by itself.
struct ClientInfo
{
	int		CenterAutoID;
	int		NetNo;
	char	Account[ct_AccLen];	//		varchar(20)	not null			,
	char	CompanyName[61];	//		varchar(60)	not null			,
	char	CompanyAddr[54];	//		varchar(60)		NOTE: only 53 byte can use
	char	PostalCode[7];		//
	char	LimitMoney[7];		//	 	varchar(6)					,
	char	LinkMan[11];		//    	varchar(10)	not null			,
	char	ZFPassword[21];		//		varchar(20)						,
	char	Telephone[21];		//		varchar(20)	        			,
	char	OpenPlace[21];		//     	varchar(20)	        			,
	double	OpenDate;			//   	datetime   	not null			,
	int		OpenOperator;		//		int(4)						,
	double	EnableDate;			//		datetime	not null			,
	double	DisableDate;		//		datetime	not null			,
	short	State;				//     	smallint(2)					,
	double	LastModify;			//		datetime	not null			,
	short 	AccountType;		//		smallint	not null
};


typedef struct tagSTAMPINFO
{
	int		iStampNum;
	short	StampType;
	short	StampColor;
	double 	dEnableDate;
	int  	iImageBytes;

} STAMPINFO, *LPSTAMPINFO;


typedef struct tagZIMGFILE
{
	GATEFILEHEADER	FileHeader;
	ACCOUNTINFO AccountInfo;
	STAMPINFO StampInfo;
	//char pImgData[1];	//...

} ZIMGFILE, *LPZIMGFILE;


typedef struct tagZINFOFILE
{
	GATEFILEHEADER	FileHeader;
	ACCOUNTINFO		AccountInfo;

} ZINFOFILE, *LPZINFOFILE;


/**************************************************************************/
/*  The following structure is used to send transfer percent info when
	local server downloading. */
typedef enum _TRAN_ID
{
	TR_DOWNLOADING 	= 	3000,
	TR_FINISHED		=	3001
} TRAN_ID;

typedef struct _TransRate
{
	TRAN_ID		tID;
	int			iRate;	// 0 - 100
} TRANSRATE, *LPTRANSRATE;

/**********************************************************************************/
/*	Voucher Samples comm structures		*/

typedef struct NewVchSamplesHeader_
{
	int		iSize;				// Size of this structure
	double	dCenterNewestDate;
	int		iCount;
	int		iTag;
} NEWVCHSAMPLESHEADER, *LPNEWVCHSAMPLESHEADER;

typedef struct _NewVchSamples
{
	NEWVCHSAMPLESHEADER	Header;
	char	iTypeCode[1];		//...

} NEWVCHSAMPLES,*LPNEWVCHSAMPLES;

typedef struct _NewVchDataHeader
{
	int		iSize;				// Size of this structure
	char	sVchName[21];
	short	iTypeCode;
	short	iState;
	double	dModifyDate;
	int		iDatalen;			// Data length of pData
} NEWVCHDATAHEADER,*LPNEWVCHDATAHEADER;

typedef struct _NewVchData
{
	NEWVCHDATAHEADER Header;
	char	pData[1];			//...
} NEWVCHDATA,*LPNEWVCHDATA;


//-------------------------------------------------------
typedef struct NETNAMEINFO
{
	EVENTID		eID;
	int			iNetNo;
	int			iOldNetNo;
	double		dTheTime;
	char		sNetName[41];
}NETNAMEINFO;

typedef struct _NETNAMEHEADER
{
	EVENTID		eID;
	int			iCount;
	int			iSize;
	int			iTag;
} NETNAMEHEADER;

typedef struct _NETNAMEQUENE
{
	NETNAMEHEADER 	Header;
	NETNAMEINFO		pNet[1];
} NETNAMEQUENE;

////////////////////////////////////////////////////////////////////////////////////
//
typedef struct CLI_INFOHEADER_
{
	int			iVersion;				// Package Version -- should be 0x01
	int			iSize;					// Size of this struct;
	int			iCount;					// Count of AccountInfo in this package
	int			iSectionSize;			// Size of struct ACCOUNTINFO
} CLI_INFOHEADER, *LPCLI_INFOHEADER;

typedef struct CLI_INFO
{
	CLI_INFOHEADER		Header;
	ACCOUNTINFO			Info[1];
}CLI_INFO;


typedef struct _GetAccountQuene
{
	EVENTID		eID;
	int			iNetNo;
	int			iHaveGet;
	int			iTotal;
} GETACCQUENE, *LPGETACCQUENE;

enum
{
	ZQUE_FIRST,
	ZQUE_MID,
	ZQUE_END
};

typedef struct _SendAccountQueneHeader
{
	EVENTID			eID;
	unsigned int	Size;
	unsigned int	Count;
	unsigned int	Total;
	unsigned int	Flag;
}_SendAccountQueneHeader;

typedef struct _SendAccountQuene
{
	_SendAccountQueneHeader 	Header;
	int							pAutoID[1];
} SENDACCQUENE, *LPSENDACCQUENE;

//	set for Download Account Quene
#define ZMAX_QUENE		1000

//------------------------------------------------------------------------
typedef struct RangeInfoHeader
{
	EVENTID			eID;
	int				ClientAutoID;
	int				PackSize;
	int				RangeCount;
}RangeInfoHeader;

typedef struct RangeData
{
	int				iType;
	int				iBeginNo;
	int				iEndNo;
	int				iTag;
}RangeData;

typedef struct _RangeInfo
{
	RangeInfoHeader Header;
	RangeData		pData[1];
} RANGEINFO, *LPRANGEINFO;

//------------------------------------------------------------------------
//	Holidays Info Transfer Section
typedef struct _HOLIDAYINFOHEADER{
	int		iSize;			// Size of this structure;
	int		iDatalen;		// Package length
	double	dMinDate;		// Earliest date inf the package
	short	iCount;			// Count of Holiday Group in the package
	char	bCompressed;	// The package is compressed by zlib
} HOLIDAYINFOHEADER, *LPHOLIDAYINFOHEADER;

typedef struct _HOLIDAYGROUP{
	double	dDate;			// Holiday Date;
	char	iState;			// Holiday State
} HOLIDAYGROUP, *LPHOLIDAYGROUP;

typedef struct _HOLIDAYINFO{
	HOLIDAYINFOHEADER	Header;
	char				pPackage[1]; //...
} HOLIDAYINFO, *LPHOLIDAYINFO;


//------------------------------------------------------------------------
#define CUST_CMD_SQL	412

typedef struct CUSTOM_CMDHEADER
{
	EVENTID		eID;
	int			iSize;		// Sizeof of this structure;
	int			iDatalen;	// Package length
	short		iCmdType;	// CUST_CMD_SQL

	char		cChksum;
	char		cChkxor;
}CUSTOM_CMDHEADER;

typedef struct CUSTOM_CMD
{
	CUSTOM_CMDHEADER 	Header;
	char				pData[1];
}CUSTOM_CMD;

typedef struct CUSTOM_CMD_REPLY
{
	EVENTID		eID;
	int			iResult;
}CUSTOM_CMD_REPLY;

typedef struct tagCOMPANY_NAME
{
	EVENTID		eID;
	char		CompanyName[61];
}COMPANY_NAME, *LPCOMPANY_NAME;

struct INFOHEADER
{
	int eID;
	char cReason;       // 1-Client to Server,  0-Server to Client
	int GradeCount;
    int TimeCount;
};

struct OPERATORGRADE
{
	int nGrade;
    int nMoney;
};

struct WORKTIME
{
	char cNetNo[20];
    char start_time[20];
    char end_time[20];
};

struct TRANINFO
{
	INFOHEADER header;
    OPERATORGRADE *pGrade;
    WORKTIME *pTimes;           // do not use directly, use sizeof( OPERATORCLASS ) * header.GradeCount
                                // to move pointer.
};


//***************************************************
struct CS_GETWORKSTATUS
{
    EVENTID eID;
    double  last_time;
};

struct CS_WORKSTATUSHEADER
{
    EVENTID eID;
    int     SizeOfHeader;
    int     ItemCount;
    int     Tag;
};

struct CS_WORKSTATUSITEM
{
    int     ip;
    double  login_tm;
    double  last_opr_tm;
    int     opr_count;
    double  logout_tm;
    char    NetNo[10];
    char    NetName[40];
};

struct ClientIndexItem
{
	int		autoID;
	double	lastModify;
	char	stampLibCount;
	char	reserved[3];
};

struct ClientIndexHeader
{
	short	headerSize;
	short	reserve;
	int		netno;
	int		indexCount;
};
//****************************************************

// -----------------------------------------------------------------------
#define OPERATOR "1"
#define LOCALNETNO	111
#define EARLEST_TIME "1960-1-1"

//-----------------------------------------------------------------------
#define KEYBACK		VK_PRIOR
#define KEYFOREWARD	VK_NEXT


#if defined( _MSC_VER ) || defined( __BORLANDC__ )
#pragma pack(8)
#endif

typedef struct tagRES_BRD_INFO
{
	int	SerialID;
	int GenRes;
}RES_BRD_INFO, *LPRES_BRD_INFO;


////////////////////////////////////////////////////////////////////////////////////
#endif /*__ZVCHINFODEF_H__*/


/**********************************************************************************\

	_VchResult::iResult is a Globle Operate Result and Error Variant, it has
	bellow meaning:

	The Following Meaning is just Example, Detail Infomation was in "ZVchInfoDef.rc"

		// 	<<	Unknow Error >> is defined 		0
		0		"未知错误。"

		//	<<	General Error >> 				1 - 99
		1 		"无法在远程主机上执行存储过程。"

		//	<<	Add Voucher Result >> 			100 - 199
		100 	"祝贺，凭证已被成功添加!"
		101 	"Error of Begin No > End No."
		102 	"this Account not valid."
		103 	"Error of Account not Found."
		104		add voucher already exist
		105		"Range bigger than 100"

		//  << UnAdd Voucher Result >> 			200 - 299
		200 	"祝贺，凭证已被成功删除!"		// UnAdd voucher operate successfully.
		201 	"Error of Begin No > End No."
		202 	"Error of Some voucher does not exist."
		203 	"Voucher not at the edge."

		//  << Lose Voucher Result >>			300 - 399
		300 	"祝贺，凭证已被成功挂失!"		// Lose voucher operate successfully.
		302 	"操作错误，不能挂失已被挂失的凭证！"	// Error of Target voucher has been Losed.
		303 	"Error of Account not Found."

		//  << Reset Voucher Result >>			400 - 499
		400 	"祝贺，凭证已被成功恢复!"		// Reset voucher operate successfully.
		402 	"Error of Target voucher is just Ready to Use."
		403 	"Error of Account not Found."
		404		"凭证未找到或状态不是"已使用"

		//  << Use Voucher Result >>			500 - 599
		500   	"Use voucher operate successfully."
		502 	"Error of Target voucher has been Used."
		503		it has been freeze
		504		it has been used
		505		target is not exist
		510    	other reason of fail

		//  << Get Voucher Info Result >>		600 - 699
		600 	"Target Voucher is Vilid to Use."
		601		"Losed"
		602 	"销户"
		603 	"冻结"
		604		"used"
		605		"Unkown voucher state"
		606		"not found"

		610		-- Account Invalid
		611		-- Account Deleted
		612		-- Account Freezed
		613		-- Unkown Account State


		//	<< Img Reply Result >>				700 - 799
		700		"当前客户信息 A 有效."
		701		"当前客户信息 B 有效."

		702		"need to download A"
		703		"need to download B"

		705		"凭证有效，客户未检查"

		710		"Cannot connect to Remote Server."

		711		"客户不存在"
		712		"凭证无效"
		713 	"凭证日期无效"
		714		"凭证日期早于开户日期"
		715 	"Stamp not found"
		716		"客户无效"
		717		"印鉴未复核,不能启用"
		718		"印鉴复核未通过，不能启用"

	 	//	 << Apply Account >>
	 	800		"New Account Apply Success."
	 	801		"New Account Already Exist".

	 	//	<< Predelete Account >>
	 	900		"Account can be delete"
	 	901		"Account's voucher not empty"
	 	902		"Account not exist"
	 	903		"Account has been used."


	 	// << IP control >>
	 	910     IP OK
	 	911     Time out of worktime
	 	912     IP not found.


\**********************************************************************************/
