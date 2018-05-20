#ifndef __TXPROTO_H__
#define __TXPROTO_H__

#if defined( _MSC_VER ) || defined( __BORLANDC__ )
#pragma pack(4)
#endif



/*
****************************************************************
交易1201：
用途：从业务主机上获取客户信息。
通讯路线：分行验印服务器－>分行elink
传入参数：txRequest
传出参数：txClientInfo
返回值：0-成功。否则失败
说明：
传入结构中 eID=1201， reqno＝客户帐号, flag=帐户类型
传出结构中应包含客户资料信息，其中未做说明的字段不需要填充。

....
银行交易代码：0116   交易名称：按客户帐号查询客户信息(对公、对私)
功能描述：本交易用于查询储蓄客户资料记录信息。根据柜员输入的客户
帐号，查询数据库，然后输出查询的结果。
****************************************************************
*/

typedef struct _txRequest
{
    long eID;       /* =1201*/
    char reqno[40]; /* 帐号*/
    long mode;      /* 控制参数*/
    char info[12];  /* 控制参数*/
    char flag[4];   /* 凭证种类*/
    char clkno[12]; /* 柜员号*/
    char depno[8];  /* 部门号（机构号）*/
} txRequest;

typedef struct _txClientInfo
{
    char    accno[40];      /* 帐号*/
    char    compname[60];   /* 公司名称*/
    char    compaddr[60];   /* 公司地址*/
    char    postalcode[8];  /* 邮编*/
    char    linkman[12];    /* 联系人*/
    char    linkmanid[20];  
    char    tel[20];        /* 联系电话*/
    char    fax[20];        /* 传真*/
    char    type[4];        /* 客户类型*/
    char    state[4];
    char    property[8];
    char    used[4];
    char    openclerk[12];  
    char    opendate[10];   /* 开户日期*/
    char    enabledate[10]; /* 启用日期*/
    char    disabledate[10];
    char    builddate[10];
    char    comtrade[4];
    char    netname[30];    /* 开户行名称*/
    char    netdepno[10];   /* 开户行部门号*/
    char    subject[12];    /* 科目*/
    long    clientid;
    long    modifyno;
    long    moneylimit;
    char    passwd[20];
    char    memo[2028];
	char	netno[12];
    char    passwdflag[8];
    char    tx_result[8];       /*交易执行结果*/
} txClientInfo;

/*
****************************************************************
交易1202：
用途：从业务主机上获取柜员信息。
通讯路线：分行验印服务器－>分行ilink
传入参数：txRequest
传出参数：txTellerInfo
返回值：0-成功。否则失败
说明：
传入结构中 eID=1202， reqno＝柜员号
传出结构中应包含柜员资料信息，其中未做说明的字段不需要填充。

返回0代表该柜员号在业务主机中有登记并可以正常使用，否则代表该
柜员号码无效。

......
银行交易代码：9307   交易名称：修改柜员信息前查询
功能描述：修改柜员信息前查询。 前台控制只有分行，总行管理员做交易


****************************************************************
*/
typedef struct _txTellerInfo
{
    char    telrno[12];
    char    name[20];           /* 柜员姓名*/
    char    pswd[16];       
    char    state[4];   
    char    grade[4];
    char    depno[12];
    long    priv;
    char    tx_result[8];       /*交易执行结果*/
} txTellerInfo;


/*
****************************************************************
交易1203：
用途：从业务主机上获取凭证信息。查询是否该张凭证已出售给指定帐号
通讯路线：分行验印服务器－>分行ilink
传入参数：txVchReq
传出参数：txVchInfo
返回值：0-成功。否则失败
说明：
传入结构中 eID=1203， reqno＝帐号，info=凭证号，flag=凭证代码
传出结构中应包含凭证资料信息，其中未做说明的字段不需要填充。

返回0代表该柜员号在业务主机中有登记并可以正常使用，否则代表该
柜员号码无效。

......
银行交易代码：7117 查询凭证交易

****************************************************************
*/
typedef struct _txVchReq
{
    char accno[40]; /* 帐号*/
    char acctp[4];  /* 客户帐号类型*/
    char vchno1[10]; /* 起始凭证号*/
    char vchno2[10]; /* 终止凭证号*/
    long index;     /* 起始笔数*/
    long total;     /* 查询笔数*/
    char vchtp[4];  /* 凭证种类*/
    char clkno[12]; /* 柜员号*/
    char depno[8];  /* 部门号（机构号）*/
    char bankno[4]; /* 分行代码*/
    char vers[4];   /* 凭证批号*/
} txVchReq;


typedef struct _txVchInfo
{
    char    accno[40];
    char    vchno[12];          
    char    vchtc[4];       
    char    state[4];           /*当前凭证状态*/
    char    pswd[12];
    char    vers[4];            /*凭证批次号*/
    char    tx_result[8];       /*交易执行结果*/
} txVchInfo;


/*
****************************************************************
交易1208：
用途：单笔自动记帐
通讯路线：分行验印服务器－>分行ilink
传入参数：txTally
传出参数：txTallyRes
返回值：0-成功。否则失败
说明：
传入结构中 eID=1208， accno＝帐号，vchno=凭证号，
vchtp=凭证代码，vchdate=凭证日期，clkno=柜员号，
amount=凭证金额

返回0代表记帐成功

......
银行交易代码：8103交易名称: 同城提回借方
功能描述:本行客户从本城票据交换中心提回他行客户的付款要求后的交易，
本交易是通过同城票交手工提票的方式参与同城范围清算


****************************************************************
*/
typedef struct _txTally
{
    long    eID;                /* 交易代码      =1208 */
    char    account[32];        /* *帐号 */
    char    exchno[12];         /* *交换号 */
    char    vchno[10];          /* *凭证号8 */
    char    vchtp[4];           /* 凭证种类2 */
    char    vers[4];            /* 批次号1 */
    char    bankno[4];          /* 分行代号2 */
    char    amount[16];         /* *金额 */
    char    amsrc[4];           /* *资金来源 */
    char    clkno[12];          /* *柜员号 */
    char    depno[8];           /* *机构号 */
    char    digest[80];         /* *摘要 */
    char    memo[80];           /* *备注 */
} txTally;

typedef struct _SINGLETALLYINFO		
{
	char accno[32];			//帐号
	char vchtp[4];			//凭证类型
	char batno[4];			//批次号
	char vchno[12];			//凭证号
	char money[16];			//金额
	char idres[4];			//识别结果
	char sysres[12];		//系统识别结果
	char manres[12];		//人工识别结果
	char amsrc[32];			//来源号
	char date[12];			//日期
	char clkno[16];			//柜员号
	char depno[12];			//机构号
	char exno[12];			//交换号
	char memo[80];			//备注
	char digest[80];		//摘要
	char tmp[128];			//备用字段
}SINGLETALLYINFO;


typedef struct _txTallyRes
{
    char        tx_result[8];       /*交易处理结果*/
} txTallyRes;



#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus*/


/*
****************************************************************
接口函数格式：
int tx_call(int eID, char* pRequest, char* pResult);


此函数用于完成验印系统－>elink之间的通讯

eID: 交易代码
pRequest: 传入参数，为txRequest或txClientData类型的指针。
pResult:传出参数，用于返回执行结构结构。

****************************************************************
*/
int tu_call(int eID, char* pRequest, char* pResult);


/*
****************************************************************
接口函数格式：

int tx_tran(int eID, char* pRequest, int ReqLen, 
            char** ppResult, int* pResLen);

此函数用于完成验印系统－>elink之间的通讯

eID: 交易代码
pRequest: 传入参数，为txRequest或txClientData类型的指针。
ReqLen: 传入长度。
ppResult:传出参数，用于返回执行结构结构。
pResLen:传出数据长度地址。用于存储传出长度。
****************************************************************
*/
int tx_tran(int eID, char* pRequest, int ReqLen, 
            char** ppResult, int* pResLen);

/*
****************************************************************
接口函数格式：
void tx_free(char* pResult);

pResult:传入tx_tran返回的结构指针。

说明：
该函数用于释放tx_call中动态分配的内存

****************************************************************
*/
void tx_free(char* pResult);

/*
****************************************************************
接口函数格式：
int svc_proc(int eID, char* pRequest, int ReqLen, char* realip, 
        char** ppResult, int* pResLen);

说明：
参数realip代表当前连接的物理IP,其余参数等同于tx_tran.

****************************************************************
*/
int svc_proc(int eID, char* pRequest, int ReqLen, char* realip, 
        char** ppResult, int* pResLen);



#ifdef __cplusplus
}
#endif  /*__cplusplus*/

#if defined( _MSC_VER ) || defined( __BORLANDC__ )
#pragma pack(8)
#endif

#endif /*__TXPROTO_H__*/
