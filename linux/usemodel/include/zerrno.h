/******************************************************************
    
        This file defined common error return code.     

******************************************************************/

#ifndef __ZERRNO_H__
#define __ZERRNO_H__

#include <stdio.h>

enum
{
    ZE_SUCCESS = 0,

    //  Section for class CAccIdBridge.
    ZE_AI_CREATE_FAIL   = 1000,
    ZE_AI_CONN_FAIL ,
    ZE_AI_SEND_FAIL,
    ZE_AI_RECV_FAIL,
    ZE_AI_RATE_ERR
};

enum
{
    ZST_FIRST_DIST  = 0,
    ZST_FIRST_OVER  = 1,
    ZST_CHECK_SCAN  = 2,
    ZST_SCAN_OVER   = 3,
    ZST_CHECK_ID    = 4,
    ZST_ID_OVER     = 5,
    ZST_SECOND_DIST = 6,
    ZST_SECOND_OVER = 7,
    ZST_NET_RESCAN  = 8
};

#define WK_STATE0_TXT "等待"
#define WK_STATE1_TXT "可以进行扫描"
#define WK_STATE2_TXT "等待"
#define WK_STATE3_TXT "可以进行人工干预"
#define WK_STATE4_TXT "等待"
#define WK_STATE5_TXT "可以进行二次清分"
#define WK_STATE6_TXT "等待"
#define WK_STATE7_TXT "完毕"
#define WK_STATE8_TXT "请求复位"
#define ST_STATE0_TXT "正在进行首次清分"
#define ST_STATE1_TXT "首次清分完毕"
#define ST_STATE2_TXT "正在进行扫描"
#define ST_STATE3_TXT "扫描完毕"
#define ST_STATE4_TXT "正在进行人工干预"
#define ST_STATE5_TXT "人工干预完毕"
#define ST_STATE6_TXT "正在进行二次清分"
#define ST_STATE7_TXT "完毕"
#define ST_STATE8_TXT "请求复位"




enum
{
    //  NA - NEW ACCOUNT
    SUCCESS         =   0,      /*成功*/
    NA_CONN_FAIL    = -1,   /**< 网络连接失败 */
    NA_SEND_FAIL    = -2,   /**< 数据发送失败 */
    NA_RECV_FAIL    = -3,   /**< 数据接收失败 */
    NA_RECV_NOTHING = -4,   /**< 数据未接收到 */
    NA_EID_WRONG    = -5,   /**< 交易码错误 */
    NA_APPLY_FAIL   = -6,   /**< 应用失败 */
    NA_SAVE_CLIINFO = -7,   /**< 保存客户资料失败 */
    NA_SAVE_STPINFO = -8,   /**< 保存印鉴数据失败 */
    NA_DEL_NCLIINFO = -9,   /**< 删除数据失败 */
    NA_SAVE_TASK    = -10,  /**< 保存任务失败 */
    NA_SCAN_EXCPT   = -11,  /**< 扫描发生异常 */
    NA_SAVE_EXCPT   = -12,  /**< 保存发生异常 */
    // CK - CHECK IDENTIFY
    CK_GET_VCHSAMP  = -21,  /**< 提起票据信息失败 */
    CK_VCHSAMP_ERR  = -22,  /**< 处理凭证失败 */
    CK_CHK_VCHSAMP  = -23,  /**< 审核凭证失败 */
    CK_NO_LOACC     = -24,  /**< 无本地客户 */
    CK_ABTIME_FAIL  = -25,  /**< 读取印鉴时间失败 */
    CK_CONN_FAIL    = -26,  /**< 网络连接失败 */
    CK_SEND_FAIL    = -27,  /**< 数据发送失败 */
    CK_RECV_FAIL    = -28,  /**< 数据接收失败 */
    CK_DL_FAIL      = -29,  /**< 下载客户数据失败 */
    CK_NO_STAMP     = -30,  /**< 客户无印鉴 */
    CK_VDT_WRONG    = -31,  /**< 凭证日期错误 */
    CK_UKN_AB       = -32,  /**< 未知印鉴库 */
    CK_ID_FAIL      = -33,  /**< 识别失败 */
    CK_ID_EXCPT     = -34,  /**< 识别异常 */
    CK_STPCOUNT_ERR = -35,  /**< 印鉴库数量错误 */
    CK_UNKNOWN_RES  = -36,  /**< 未知识别结果 */
    CK_ANGEL_IMG    = -37,  /**< 计算折角图失败 */
    CK_CHK_TYP_MISS = -38,  /**< 凭证类型错误 */
    CK_CHK_TYP_UNCERTAIN    = -39,  /**< 凭证类型未知 */
    CK_CHK_MARK_MISS    = -40,  /**< 凭证银行标志错误 */
    CK_CHK_MARK_UNCERTAIN   = -41,  /**< 凭证银行标志不清 */
    CK_TER_DOWN     = -42,
    CK_QUERY_VCHNAME= -43,  /**< 查询凭证名称失败 */
    CK_PRE_ALTER    = -44,  /**< 金额被图改 */
    CK_SHAPE_IMG    = -45,  /**< 计算轮廓图失败 */
    CK_CENTER_DENY  = -46,  /**< 后台拒绝服务 */
    
    CK_NOCASH_ACC   = -49,  /**< 不可提现户 */
    // CKS - CHECK IDENTIFY RESULT SAVE
    CKS_VIR_FAIL    = -51,  /**<  */
    CKS_VRN_FAIL    = -52,  /**<  */
    CKS_VRN_ERR     = -53,  /**<  */
    CKS_VIR_ROW     = -54,  /**<  */
    CKS_SIR_FAIL    = -55,  /**<  */
    CKS_DEL_NVIR    = -56,  /**<  */
                            
    CK_FJT_CANCEL   = -57,  /**<  */
    CK_CHK_VCHNO    = -58,  /**<  */ 
                            
    // CM - CLIENT MANAGER  
    CM_GETINFO_FAIL = -61,  /**< 读取客户信息失败 */ 
    CM_WRT_ITEM     = -62,  /**< 写列表失败 */ 
    CM_PRECHG_ITEM  = -63,  /**< 修改列表 */ 
    CM_FRZ_REC      = -64,  /**< 冻结记录失败 */ 
    // CMT - CLIENT MANAGER TASK INFO
    CMT_ACC_PRE     = -65,  /**<  */ 
    CMT_ACC_ERR     = -66,  /**<  */ 
    CMT_ACC_ROW     = -67,  /**<  */ 
    CMT_INS_FAIL    = -68,  /**< 列表插入失败 */ 
    CM_FIND_ITEM    = -69,  /**< 列表查找失败 */ 
    // AD - ACCOUNT DETAIL  
    AD_GETINFO_FAIL = -71,  /**< 读取客户信息失败 */ 
    AD_INFO_ERR     = -72,  /**< 客户数据错误 */
    AD_ERR_DEL      = -73,  /**< 客户删除失败 */
    AD_BTIME_FAIL   = -74,  /**< 旧库时间错误 */
    AD_BTIME_ROW    = -75,  /**< 旧库数量错误 */
    AD_SCAN_EXCPT   = -76,  /**< 扫描异常 */
    // ADS - ACCOUNT DETAIL SAVE
    ADS_ACC_FAIL    = -77,  /**< 保存失败 */ 
    ADS_TASK_FAIL   = -78,  /**< 设置任务失败 */ 
    ADS_STMP_FAIL   = -79,  /**< 保存印章失败 */ 
    ADS_DOB_FAIL    = -80,  /**< 操作旧库失败 */ 
    ADS_INB_FAIL    = -81,  /**< 插入旧库失败 */ 
    ADS_DOA_FAIL    = -82,  /**< 操作新库失败 */ 
    // AD                   
    AD_CK_OLD       = -83,  /**<  */ 
    
    
    // VS - VOUCHER SOCKET
    VS_CONN_FAIL    = -101, /**< 连接失败 */
    VS_SEND_FAIL    = -102, /**< 发送失败 */
    VS_RECV_FAIL    = -103, /**< 接收失败 */
    VS_RECV_NOTHING = -104, /**< 接收无数据 */
    VS_EID_WRONG    = -105, /**< 交易码错误 */
    // ICI - IS CLIENT IN DB
    ICI_READ_FAIL   = -106, /**< 读取数据库失败 */
    ICI_NOT_IN      = -107, /**< 信息不存在 */
    ICI_READ_ROW    = -108, /**< 数量错误 */
    ICI_ROW_ERR     = -109, /**< 记录错误 */
    ICI_MORE_ACC    = -110, /**< 数量过多 */
                            
                    
    // SL - STAMP LIB PROC
    SL_READ_FAIL    = -111, /**< 读取失败 */
    SL_NOT_ONE      = -112, /**< 印鉴不唯一 */
    SL_UNCMP_FAIL   = -113, /**< 解压缩失败 */
    SL_SCANIMG_FAIL = -114, /**< 扫描失败 */
    // SSL - SET STAMP LIB  
    SSL_NO_STMP     = -115, /**< 无印鉴库 */
    SSL_CMP_FAIL    = -116, /**< 压缩失败 */
    SSL_SAV_FAIL    = -117, /**< 保存失败 */
                            
    // SS - STAMP SHOW      
    SS_NO_STMPIMG   = -121, /**< 无印鉴库 */
    SS_CRET_FAIL    = -122, /**<  */
    // STM - STAMP WND
    STM_CREATE_DIB  = -125, /**< 创建DIB失败 */
    STM_SHOW_SIGN   = -126, /**< 显示签字失败 */
    STM_NO_SIGN     = -127, /**< 无签字 */
                            
    // ST - SERVER TIME     
    ST_SOCK_ERR     = -131, /**< 网络通讯错误 */
    ST_LOAD_DLL     = -132, /**< 装载Dll失败 */
    ST_GET_PROC     = -133, /**< 装载函数失败 */
    ST_SETTIM_FAIL  = -134, /**< 设置时间失败 */
                            
    // DB - DATA BASE OPERATION
    DB_OPEN_FAIL    = -161, /**< 打开数据库失败 */
    DB_QUERY_FAIL   = -162, /**< 查询失败 */
    DB_RECORD_EMPTY = -163, /**< 无数据 */

    PARAMETER_ERR   = -171, /**< 无效的控制参数。*/

    // MS - MESSAGE SERVER SOCKET
    MS_DISCONNECT   = -181, /**< 连接断开错误 */

    // NW - Net Work Data Operation
    NW_CONN_FAIL    = -190, /**< 连接失败 */
    NW_SEND_FAIL    = -191, /**< 发送失败 */
    NW_RECV_FAIL    = -192, /**< 接收失败 */
    NW_COMP_FAIL    = -193, /**< 压缩失败 */
    NW_UCMP_FAIL    = -194, /**< 解压缩失败 */
    NW_RECV_ERR     = -195, /**< 接收错误 */
    NW_OPEN_FAIL    = -196, /**< 创建对象失败。*/
    NW_GETHOST      = -197, /**< 调用gethostname()/gethostbyname()失败。*/
    NW_BAD_SIZE     = -198, /**< 数据大小错误。*/
    NW_BAD_POINTER  = -199, /**< 无效的指针。*/
    
    XML_NOT_FOUND   = -200, /**< 处理xml数据，未找到所需信息。*/
    
    
    AUTHORITY_FAIL  = -201,     /**< 授权失败！*/
    AUTHORITY_BY_SELF = -202,   /**< 授权柜员不能为自己 */
    AUTHORITY_LEADER  = -203,  /**给上一级授权 */

//  IDENTIFY_INTERUPT = -1024
    XML_PARSE_ERROR     =-240,  /**< xml解析失败 */
    XML_DOCUMENT_ERROR  =-241,  /**< xml文档错误 */
    XML_NODE_ERROR      =-242,  /**< xml节点错误 */
    ALLOCATE_MEMORRY_FAILED=-243,   /**< 分配内存失败 */
    MODE_VALUE_ERROR    =-244,  /**< 模式错误 */
    
    
    //前台错误
    FG_GEN_ERR      =-300,  /**< 未知错误 */
    FG_OPEN_FAIL    =-301,  /**< 打开数据库错误 */
    FG_QUERY_FAIL   =-302,  /**< 查询错误 */
    FG_NO_RECORD    =-304,  /**< 无查询数据 */
    FG_LEN_ERR      =-305,  /**< 长度错误 */
    FG_INSERT_FAIL  =-306,  /**< 插入错误 */
    FG_DELETE_FAIL  =-307,  /**< 删除错误 */
    FG_UPDATE_FAIL  =-308,  /**< 更新错误 */

    COM_SEND_ERR        =   -311,   /**< 不能发送指令 */
    COM_GET_CHAR_ERR    =   -312,   /**< 不能得到输入串 */
    COM_OPEN_ERR        =   -313,   /**< 不能打开串口 */
    COM_INIT_ERR        =   -314,   /**< 不能初始化串口 */
    COM_CLOSE_ERR       =   -315,   /**< 不能关闭串口 */
    COM_OVER_TIME_ERR   =   -321,   /**< 密码输入操作超时 */
    COM_PSWD_INPUT_ERR  =   -322,   /**< 密码输入错误 */

    /*前台一票一密相关错误*/
    BG_RANDOM_ERR       =   -330,   /*不能得到RDM*/
    BG_PINBLOCK_ERR     =   -331,   /*不能得到PIN*/
    BG_ENCODEPIN_FAIL   =   -332,   /*不能找到ENCODEPIN库*/
    BG_GETRANDOM_FAIL   =   -333,   /*不能找到RDM函数*/
    BG_GETPINBLK_FAIL   =   -334,   /*不能找到PIN函数*/
    BG_GETWKEY_FAIL     =   -335,   /*不能得到WORKKEY*/
    BG_PWDCHECK_ERR     =   -336,   /*不能得到WORKKEY*/
    BG_RECORD_EXIST     =   -337,   /*支票已存在*/


    //后台错误
    BG_GEN_ERR      =-400,  /**< 未知错误  */
    BG_OPEN_FAIL    =-401,  /**< 打开数据库错误 */
    BG_QUERY_FAIL   =-402,  /**< 查询错误 */
    BG_NO_RECORD    =-404,  /**< 无查询数据 */
    BG_INSERT_FAIL  =-406,  /**< 插入错误 */
    BG_DELETE_FAIL  =-407,  /**< 删除错误 */

    BG_RUN_TIME         =   -405,   /**< 数据库操作错误 */
    BG_COMMIT_FAIL  =-408,/*事务提交错误*/
        //BG_VRF_FAIL         =   -408,   /*密码错误！*/           /////
    //2004.1.9
    BG_MODE_FAIL    =-409,/*请求不能操作*/
    BG_MODE_INVALID =-410,/*请求无效*/
    BG_VRF_FAIL     =-411,/*无效密码*/
    BG_CRT_FAIL     =-412,/*创建失败*/
    BG_MOD_FAIL     =-413,/*修改失败*/
    BG_GET_WKEY_FAIL=-414,/*获取wkey失败*/
    //BG_RECORD_EXIST =-415,/*创建数据已存在*/

    BG_PVV_NEED_UPDATE =-416,/*pin is valid and pvv must be updated.*/
    
    BG_BREAK_TRANS =-417,/*cannot continue the trancsation*/
    
    BG_CHEQ_USED=-418,/*支票已使用*/
    
    BG_XML_NO_DATA  =-420, /*XML解析无数据*/
    BG_GET_CLIENT_FAIL=-421 ,
    BG_GET_STAMP_FAIL =-422 ,
    BG_GET_CACHET_FAIL=-423 ,
    //ALLOCATE_MEMORRY_FAILED=-1111
    //2004.1.9
    BS_ENCODE_ERR       =   -451,   /**< 转换数据格式失败 */
    BS_DECODE_ERR       =   -452,   /**< 反转换数据格式失败 */
    
    /**< XML串解析错误 */
    
    XML_OPEN_ERR        =   -610,   /**< 不能解析XML数据 */
    XML_GET_ROOT_ERR    =   -611,   /**< 不能得到root节点 */
    XML_GET_FIELD_ERR   =   -612,   /**< 不能得到checkid域 */
    XML_ICONV_OPEN_ERR  =   -613,   /**< 不能打开转换函数 */
    XML_ICONV_CLOSE_ERR =   -614,   /**< 不能关闭转换函数 */
    XML_ICONV_ERR       =   -615,   /**< 不能转换串 */
    XML_KEYWORD_ERR     =   -621,    /**< 不能找到关键字 */
    XML_STAMP_DATA_ERR  =   -622,    /**< 不能在所给内存里找到图象数据段 */
    XML_APPEND          =   -623,    /**< 不能把数据粘贴到指定数据末尾 */
    
    /**< 文件操作错误 */
    FL_OPEN_ERR         =   -631,    /**< 不能打开文件 */
    FL_READ_ERR         =   -632,    /**< 从文件读数据错误 */
    FL_ATTRIBUTE_ERR    =   -633,    /**< 不能得到文件属性 */
    FL_DELETE_ERR       =   -634,    /**< 不能删除文件 */
    FL_WRITE_ERR        =   -635,    /**< 不能写数据到文件 */

    DT_GET_PHOTO        =   -641,    /*不能得到图象数据*/
    DT_NEXT_NONE        =   -645,    /*没有下条数据*/
    /**< 交易错误 */
    RQT_INVALID         =   -651,    /**< 无效请求 */
    MOD_INVALID         =   -652,    /**< 无效模式 */

};



#ifndef WIN32
#define OutputDebugString(__str_to_output)  fprintf(stderr,__str_to_output)
#endif


#define zreturn(RETVAL) {\
    char __b[512];\
    int __r = (int)RETVAL;\
    if(__r){\
        const char* __p = __FILE__;\
        int __len = strlen(__p);\
        if( __len>20) __p += (__len-20);\
        sprintf(__b,"  err[%d]: %s:%d\n",__r,__p,__LINE__);\
        OutputDebugString(__b);\
    }\
    return __r;\
}

extern "C" void set_errstr(int errNo, const char* errStr);

extern "C" const char* get_errstr(int errNo);


#endif //__ZERRNO_H__
