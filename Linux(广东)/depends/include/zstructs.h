/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 �����촴������Ϣ�Ƽ����޹�˾    ��Ȩ����
 *
 * ���ļ�Ϊ�촴���������Ʒ��һ���֡�
 * �촴���������Ʒ�����в���,�����������������Ʒ���������κ�ͼ����Ƭ��
 * ���������ֺ͸��ӳ���applets�������ܿ����渽��ӡˢ���ϡ�Դ�����ļ���
 * �������Ʒ���κθ�����һ������Ȩ��֪ʶ��Ȩ�������Ȩ���촴����ӵ�С�
 *
 * �ļ���	$Id$
 * �汾��	$Revision$
 * ��ַ��   www.tchzt.com
 * ���䣺   support@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __ZSTRUCT_H__
#define __ZSTRUCT_H__

// 8583����������
enum ISO8583_INDEX
{
	II_ACCNO				= 2, 			// �����ʻ�
	II_TXN_CODE 			= 3,			// ���״�����
	II_AMOUNT_OU			= 4, 			// ���׽��
	II_AMOUNT_IN			= 6, 			// ���˽��
	II_DEP_CODE				= 8,			// ���������
	II_SUBJECT				= 10,			// ��Ŀ
	II_SYSTEM_TRACE_NO		= 11,			// ϵͳ���ٺ�
	II_TRANS_TIME			= 12,			// ����ʱ��
	II_TRANS_DATE			= 13,			// ��������
	II_CARD_TYPE			= 18,			// ������
	II_ACT_TYPE				= 20,			// �ʻ�����
	II_ACCNO_BIND			= 34,			// ���ʻ�
	II_TRACK_2				= 35,			// ���ŵ���Ϣ
	II_REFER_NO				= 37,			// ϵͳ�ο���
	II_RET_CODE				= 39,			// ������
	II_TERM_ID				= 41,			// �ն˺�
	II_MERCH_NO				= 42,			// �̻���
	II_IN_REVERSE_SEQ		= 45,			// ���������
	II_OUT_REVERSE_SEQ		= 46,			// ȡ��������
	II_ACC_NAME				= 47,			// �ʻ�����
	II_PRIVATE_KEY			= 48,			// ������Կ
	II_PWD					= 52,			// ����
	II_BATCHNO				= 60,			// ���κ�
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
	char teleid[28];		  /*�绰����*/
	char termid[24];      /*�ն˺�*/
	char psamid[16];      /*PSAM������*/
	char clkno[12];       /*�ƹ��Ա*/
	char amount[16];			/*���׽��*/	
	char inbrno[8];				/*�տ��˷��к�*/
	char indepno[12];     /*�տ��������*/
	char inaccno[32];     /*�տ��*/
	char inmainaccno[32]; /*�տ������˺�*/	
	int  inatype;					/*�տ������˻�����*/	
	char oubrno[8];				/*�����˷��к�*/
	char oudepno[12];     /*�����������*/	
	char ouaccno[32];    	/*�����˿���*/
	char oumainaccno[32];	/*���������˺�*/	
	int  ouatype;					/*���������˻�����*/
	char swapno[8];       /*POS������ˮ*/
	char cspno[24];       /*cspno��ˮ*/
	char curdate[12];     /*��ǰ����*/
	char curtime[8];      /*��ǰʱ��*/	
	char charge[12];      /*������*/
	char profit[12];      /*��������������*/
	int  chargetype;	    /*������ģʽ,0-�۳ֿ��� 1-���̻�*/
	char inname[24]; 			/*�տ�������*/
	int  dealtype;				/*������� 110-ת��*/
	int  cardtype;   			/*������0-ʡ�ڿ� 1-ʡ�⿨2-���п�*/
	long maxpay;					/*����֧�����*/
	int  actstate;				/*����ִ�����*/
	int  result;          /*���׽��*/
	char printtimes[4];   /*��ӡ����*/
	char pswdstr[18];			/*��������*/
	char atmkey[18];			/*atmkey��Կ*/
	char trk2[40];				/*���ŵ���Ϣ*/
	char merchno[20];			/*�̻���*/
	char termno[20];			/*�ն˺�*/
	char cspcode[8];      /*csp������*/
	char cspstrs[8];      /*csp������Ϣ*/
	char cspno2[24];      /*csp������Ϣ*/
	char reserved[140];
} deals_info_t;

typedef struct _PHONE2SYS_INFO
{
	char pakType[4];		 		 /*��������bcd*/
	char pakTel[54];         /*�ն˵绰bcd*/ 
	char pakTelEx[28];       /*�ն˵绰����*/
	char pakFixed[16];		 	 /*�ŵ���bcd---������־bcd*/	
	char progVer[4];         /*�汾��bcd*/
	char appVer[8];          /*Ӧ�ð汾��*/
	char callerFlag[4];      /*������ʾ���*/
	char psamCardNo[16];     /*psam����*/
	char psamCardEx[16];     /*psam��������*/
	char dealFlowNo[4];      /*������ˮ��*/
	char dealFlowNoEx[8];    /*������ˮ���ַ�*/
	char dealCode[8];        /*���״���*/
	int  dealCodeEx;     	 	 /*���״�������*/
	char opCodeCnt[4];       /*����������*/
	int  opCodeCntEx;		 		 /*����������*/
	char pinBlock[20];			 /*PIN BLOCK*/
	char macBlock[16];			 /*MAC*/
}phone2sys_info_t;

typedef struct _SYS2PHONE_INFO{
	char pakType[2];        /*��������*/
	char endFlag[2];        /*�������*/
	char psamCardNo[8];     /*psam����*/
	char sysDate[4];        /*ϵͳ����*/
	char sysTime[3];        /*ϵͳʱ��*/
	char dealFlowNo[3];     /*������ˮ��*/
	char dealCode[3];       /*���״���*/
	char opCodeCnt;         /*����������*/

}sys2phone_info_t;


/*���Ӳ�����
 type-1,2,3�ֽڲ�����
 idx-������
 pct-��ʾ��Ϣ����
 destype-���ܷ�ʽ
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
	char 	KeyIndexWK[8];					//WK������
	char 	KeyIndexBWK[8];					//BWK������
	int  	DisCountWK;						//WK��ɢ����
	int  	DisCountBWK;					//BWK��ɢ����
	char 	DisParamWK[256];				//WK��ɢ����
	char 	DisParamBWK[256];				//BWK��ɢ����
	int  	EncryptTypeWK;					//ԴPIN��ʽ
	int  	EncryptTypeBWK;					//Ŀ��PIN��ʽ
	char 	SourcePinBlock[1024];			//ԭ��������
	int		SourcePinBlockLen;				//ԭ�������ĳ���
	char 	MainPan[128];					//���˺���Ϣ
	int  	MainPanLen;						//���˺ų���	
}pin_info_t;


typedef struct _CRYPT_SOURCE_T
{
	char KeyIndex[8];						//���ܻ���Կ����
	int  DisCount;							//��ɢ����
	char DisParam[128];						//��ɢ����
	int  Type;								//�㷨����		
	int	 KeyType;							//Key����		
	char SourceData[1024];					//ԭ����
}crypt_source_t;

typedef struct _CRYPT_RESULT_T
{
	char 	RstCode[16];					//�������
	char 	RstData[64];					//����/ת����/MAC/��������洢
	char 	RstCKData[20];					//У��ֵ
}crypt_result_t;

//
typedef	struct	_KEY_INFO_T
{
	int		KekLen;							//KEK��Կ���ȱ�־�� 1���������� 2��˫������ 3��3����
	char	KekIdx[8];						//KEK����
	int  	WkLen;							//WK����
	char	WdData[50];						//WK����
	char	WkIndex[8];						//WK����							
}key_info_t;


/*
	������ͷ
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
	LoadLMK ����
*/
typedef struct _BANK_BODY_LMK
{
	char customid[32];			// �ͻ����(�豸��ţ�GZ47504411/GZ47504412)
	char oacc[32];					// �ۿ��ʻ�(�����)
	char oacctype[2];				// �ʻ�����(0-��ǿ�)
	char clientip[20];			// IP��ַ(�����)
	char clientport[5];			// �˿�(�����)
	char brnid[8];					// �����
	char belong[4];					// ��������
	char LK[24];						// LK
	char tmlid[22];					// �ն˺�
	char bstransport[6];		// BS��ͷ���׶˿�
	char tbtransport[6];		// TB��ͷ���׶˿�
	char sysdate[22];				// ϵͳ����ʱ��
	char brntele[22];				// ������ϵ�绰
}bank_body_lmk_t;

/*
	LoadKey ����
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
	ת�ʰ���
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
	�����뱨��
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
	������
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
	��ѯkeys������
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

