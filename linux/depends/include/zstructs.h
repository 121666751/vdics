/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 北京天创征腾信息科技有限公司    版权所有
 *
 * 此文件为天创征腾软件产品的一部分。
 * 天创征腾软件产品的所有部分,包括但不限于软件产品中所含的任何图象、照片、
 * 动画、文字和附加程序（applets）、加密卡、随附的印刷材料、源代码文件及
 * 本软件产品的任何副本的一切所有权和知识产权，均归版权人天创征腾拥有。
 *
 * 文件：	$Id$
 * 版本：	$Revision$
 * 网址：   www.tchzt.com
 * 邮箱：   support@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __ZSTRUCT_H__
#define __ZSTRUCT_H__

// 8583报文域索引
enum ISO8583_INDEX
{
	II_ACCNO				= 2, 			// 交易帐户
	II_TXN_CODE 			= 3,			// 交易处理码
	II_AMOUNT_OU			= 4, 			// 交易金额
	II_AMOUNT_IN			= 6, 			// 入账金额
	II_DEP_CODE				= 8,			// 交易网点号
	II_SUBJECT				= 10,			// 科目
	II_SYSTEM_TRACE_NO		= 11,			// 系统跟踪号
	II_TRANS_TIME			= 12,			// 交易时间
	II_TRANS_DATE			= 13,			// 交易日期
	II_CARD_TYPE			= 18,			// 卡类型
	II_ACT_TYPE				= 20,			// 帐户类型
	II_ACCNO_BIND			= 34,			// 绑定帐户
	II_TRACK_2				= 35,			// 二磁道信息
	II_REFER_NO				= 37,			// 系统参考号
	II_RET_CODE				= 39,			// 返回码
	II_TERM_ID				= 41,			// 终端号
	II_MERCH_NO				= 42,			// 商户号
	II_IN_REVERSE_SEQ		= 45,			// 存款冲正序号
	II_OUT_REVERSE_SEQ		= 46,			// 取款冲正序号
	II_ACC_NAME				= 47,			// 帐户名称
	II_PRIVATE_KEY			= 48,			// 工作密钥
	II_PWD					= 52,			// 密码
	II_BATCHNO				= 60,			// 批次号
};

enum 
{
	UNKNOW_DEAL_CODE  		= -100,
	NEXT_DEAL_CODE  		= -101,
	DEC_MSG_ERROR 			= -1000,
	DEC_ACCOUNT_ERROR 		= -1001,
	CAL_MAC_ERROR   		= -1002,
	VER_MAC_ERROR   		= -1003,
	TRS_PIN_ERROR   		= -1004,
	
	HSM_CONNECT_ERROR 		= -2001,
	HSM_SEND_ERROR    		= -2002,
	HSM_RECV_ERROR    		= -2003,
	HSM_DSWK_ERROR			= -2004,
	HSM_MAC_ERROR			= -2005,
	HSM_DES_ERROR			= -2006,
	HSM_CPIN_ERROR			= -2007,
	HSM_CKEY_ERROR			= -2008,
	HSM_GKEY_ERROR			= -2009,
	
	DB_OPEN_ERROR			= -4000,
	DB_SQL_ERROR			= -4001,
	DB_QUERY_EMPTY			= -4002,
	DB_KEYS_EMPTY			= -4003,
	
	
	TM_TELEID_ERROR	  		= -4100,
	TM_TERMID_ERROR			= -4101,
	TM_PSAMID_ERROR			= -4102,
	TM_ACCNO_ERROR			= -4103,
	TM_STATE_ERROR			= -4104,
	
	TM_TRANSQ_ERROR	  		= -4105,
	
	TM_INOUT_ERROR			= -4106,
	TM_HIS_ERROR			= -4107,
	TM_ENABLE_ERROR			= -4108,
	TM_DISABLE_ERROR		= -4109,
	TM_MAXPAY_ERROR	  		= -4110,
	TM_CHARGE_ERROR   		= -4111,
	TM_CARD_ERROR   		= -4112,
	TM_CARD_BIND   		= -4113,
	
	CSP_ERROR		    	= -5000,
	CSP_CONNECT_ERROR		= -5001,
	CSP_SEND_ERROR    		= -5002,
	CSP_RECV_ERROR    		= -5003,
	CSP_KEY_ERROR			= -5004,
	CSP_TANS_RECV_ERROR		= -5005,
	
	NAS_CONNECT_ERROR		= -5006,
	NAS_SEND_ERROR    		= -5007,
	NAS_RECV_ERROR    		= -5008,
	NAS_TANS_RECV_ERROR		= -5009,
	
	CHECK_PSW_ERROR 		= -5100,
	MALLOC_ERROR = -5200,	
	
	RBS_GET_ACCNO_ERROR		= -6000,
	RBS_ACT_TYPE_ERROR		= -6001,
	RBS_PASSWD_ERROR		= -6002,
	RBS_DEAL_ERROR			= -6003,
	RBS_REVERSE_ERROR		= -6004,
	RBS_REVERSE_SUCCESS 	= -6005,
	RBS_FEE_OUT_ERROR		= -6006,

};

typedef struct _DEALS_INFO_T
{	
	int  sid;
	char teleid[28];		  /*电话号码*/
	char termid[24];      /*终端号*/
	char psamid[16];      /*PSAM卡号码*/
	char clkno[12];       /*推广柜员*/
	char amount[16];			/*交易金额*/	
	char inbrno[8];				/*收款人分行号*/
	char indepno[12];     /*收款人网点号*/
	char inaccno[32];     /*收款卡号*/
	char inmainaccno[32]; /*收款人主账号*/	
	int  inatype;					/*收款人主账户类型*/	
	char oubrno[8];				/*付款人分行号*/
	char oudepno[12];     /*付款人网点号*/	
	char ouaccno[32];    	/*付款人卡号*/
	char oumainaccno[32];	/*付款人主账号*/	
	int  ouatype;					/*付款人主账户类型*/
	char swapno[8];       /*POS交易流水*/
	char cspno[24];       /*cspno流水*/
	char curdate[12];     /*当前日期*/
	char curtime[8];      /*当前时间*/	
	char charge[12];      /*手续费*/
	char profit[12];      /*我行手续费收入*/
	int  chargetype;	    /*手续费模式,0-扣持卡人 1-扣商户*/
	char inname[24]; 			/*收款人名称*/
	int  dealtype;				/*交易类别 110-转账*/
	int  cardtype;   			/*卡类型0-省内卡 1-省外卡2-他行卡*/
	long maxpay;					/*最大可支付金额*/
	int  actstate;				/*交易执行情况*/
	int  result;          /*交易结果*/
	char printtimes[4];   /*打印次数*/
	char pswdstr[18];			/*加密密码*/
	char atmkey[18];			/*atmkey密钥*/
	char trk2[40];				/*二磁道信息*/
	char merchno[20];			/*商户号*/
	char termno[20];			/*终端号*/
	char cspcode[8];      /*csp错误码*/
	char cspstrs[8];      /*csp错误信息*/
	char cspno2[24];      /*csp错误信息*/
	char reserved[140];
} deals_info_t;

typedef struct _PHONE2SYS_INFO
{
	char pakType[4];		 		 /*报文类型bcd*/
	char pakTel[54];         /*终端电话bcd*/ 
	char pakTelEx[28];       /*终端电话数字*/
	char pakFixed[16];		 	 /*信道号bcd---结束标志bcd*/	
	char progVer[4];         /*版本号bcd*/
	char appVer[8];          /*应用版本号*/
	char callerFlag[4];      /*来电显示标记*/
	char psamCardNo[16];     /*psam卡号*/
	char psamCardEx[16];     /*psam卡号数字*/
	char dealFlowNo[4];      /*交易流水号*/
	char dealFlowNoEx[8];    /*交易流水号字符*/
	char dealCode[8];        /*交易代码*/
	int  dealCodeEx;     	 	 /*交易代码数字*/
	char opCodeCnt[4];       /*操作码数量*/
	int  opCodeCntEx;		 		 /*操作码数量*/
	char pinBlock[20];			 /*PIN BLOCK*/
	char macBlock[16];			 /*MAC*/
}phone2sys_info_t;

typedef struct _SYS2PHONE_INFO{
	char pakType[2];        /*报文类型*/
	char endFlag[2];        /*结束标记*/
	char psamCardNo[8];     /*psam卡号*/
	char sysDate[4];        /*系统日期*/
	char sysTime[3];        /*系统时间*/
	char dealFlowNo[3];     /*交易流水号*/
	char dealCode[3];       /*交易代码*/
	char opCodeCnt;         /*操作码数量*/

}sys2phone_info_t;


/*增加操作码
 type-1,2,3字节操作码
 idx-操作码
 pct-提示信息索引
 destype-加密方式
*/
typedef struct _ACTION_INFO
{
	int  type;
	int  act;
	char idx[8];
	char pct[8];
	int  msgtLen;
	int  msgaLen;
	char msga[128];
	int  msgbLen;
	char msgb[128];
	char reserved[256];
}action_info_t;


typedef struct _SHOW_INFO_T
{
	int  msgfmt;
	int  msglen;
	char msgstr[512];
}show_info_t;
typedef struct _PIN_INFO
{
	char 	KeyIndexWK[8];					//WK索引号
	char 	KeyIndexBWK[8];					//BWK索引号
	int  	DisCountWK;						//WK离散次数
	int  	DisCountBWK;					//BWK离散次数
	char 	DisParamWK[256];				//WK离散因子
	char 	DisParamBWK[256];				//BWK离散因子
	int  	EncryptTypeWK;					//源PIN格式
	int  	EncryptTypeBWK;					//目标PIN格式
	char 	SourcePinBlock[1024];			//原密码密文
	int		SourcePinBlockLen;				//原密码密文长度
	char 	MainPan[128];					//主账号信息
	int  	MainPanLen;						//主账号长度	
}pin_info_t;


typedef struct _CRYPT_SOURCE_T
{
	char KeyIndex[8];						//加密机密钥索引
	int  DisCount;							//离散次数
	char DisParam[128];						//离散因子
	int  Type;								//算法类型		
	int	 KeyType;							//Key类型		
	char SourceData[1024];					//原数据
}crypt_source_t;

typedef struct _CRYPT_RESULT_T
{
	char 	RstCode[16];					//错误代码
	char 	RstData[64];					//加密/转加密/MAC/操作结果存储
	char 	RstCKData[20];					//校验值
}crypt_result_t;

//
typedef	struct	_KEY_INFO_T
{
	int		KekLen;							//KEK密钥长度标志。 1：单倍长； 2：双倍长； 3：3倍长
	char	KekIdx[8];						//KEK索引
	int  	WkLen;							//WK长度
	char	WdData[50];						//WK数据
	char	WkIndex[8];						//WK索引							
}key_info_t;


/*
	公共包头
*/
typedef struct _BANK_PUB_HEAD_T
{
	char cspcode[5];
	char txnid[5];
	char brnid[8];
	char tmlid[3];
	char telid[3];
	char trans[2];
	char filter[4];
	char mchno_len;
	char merch_no[9];
	char mch_name[25];
	char cisscode[8];
	char cissmesg[64];
	char refno[24];
	char cspno[9];
	char filer2[50];
}bank_pub_head_t;

/*
	LoadLMK 包体
*/
typedef struct _BANK_BODY_LMK
{
	char customid[32];			// 客户编号(设备编号：GZ47504411/GZ47504412)
	char oacc[32];					// 扣款帐户(左对齐)
	char oacctype[2];				// 帐户类型(0-借记卡)
	char clientip[20];			// IP地址(左对齐)
	char clientport[5];			// 端口(左对齐)
	char brnid[8];					// 网点号
	char belong[4];					// 所属分行
	char LK[24];						// LK
	char tmlid[22];					// 终端号
	char bstransport[6];		// BS打头交易端口
	char tbtransport[6];		// TB打头交易端口
	char sysdate[22];				// 系统日期时间
	char brntele[22];				// 网点联系电话
}bank_body_lmk_t;

/*
	LoadKey 包体
*/
typedef struct _BANK_BODY_KEY
{
	char tmkudlk[18];
	char cvudtmk[18];
	char atmtpk[18];
	char tpkudtmk[18];
	char cvudtpk[18];
}bank_body_key_t;

/*
	转帐包体
*/
typedef struct _BANK_BODY_TRANS
{
	char customid[32];
	char clientip[20];
	char oacc[32];
	char oacctp[2];
	char iacc[32];
	char iacctp[2];
	char amount[16];
	char pinblock[20];
	char tpkudtmk[20];
	char iname[24];
	char fee[16];
	char authid[8];
	char cspno[24];
	char clientid[24];
	char code2[40];
}bank_body_trans_t;

/*
	验密码报文
*/

typedef struct _BANK_BODY_PVV
{
	char transid[8];
	char transtype[8];
	char netbankid[16];
	char tbpasswd[8];
	char cspcode[8];
	char subaccno[32];
	char pinblock[20];
	char lmk[20];	
	char accno[32];
	char cspno[24];
	char code2[40]; 
	char refno[24];
	char merch_no[10];
}bank_body_pvv_t;	
/*
	查余额报文
*/
typedef struct _BANK_QUERY_AMT
{
	char transid[8];
	char transtype[8];
	char netbankid[16];
	char tbpasswd[8];
	char cspcode[8];
	char subaccno[32];
	char password[8];
	char subacctp[4];
	char subacccur[4];
	char amt[20];	
}bank_query_amt_t;

/*
	查询keys表序列
*/
typedef struct _BANK_QUERY_MERCHNO_T
{
	char termno[21];
	char merchno[21];
	char tpkidx[11];
	char tmkidx[11];
	int  keytype;
	int  state;
}bank_query_merchno_t;

#endif

