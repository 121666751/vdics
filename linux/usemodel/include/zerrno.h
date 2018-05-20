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

#define WK_STATE0_TXT "�ȴ�"
#define WK_STATE1_TXT "���Խ���ɨ��"
#define WK_STATE2_TXT "�ȴ�"
#define WK_STATE3_TXT "���Խ����˹���Ԥ"
#define WK_STATE4_TXT "�ȴ�"
#define WK_STATE5_TXT "���Խ��ж������"
#define WK_STATE6_TXT "�ȴ�"
#define WK_STATE7_TXT "���"
#define WK_STATE8_TXT "����λ"
#define ST_STATE0_TXT "���ڽ����״����"
#define ST_STATE1_TXT "�״�������"
#define ST_STATE2_TXT "���ڽ���ɨ��"
#define ST_STATE3_TXT "ɨ�����"
#define ST_STATE4_TXT "���ڽ����˹���Ԥ"
#define ST_STATE5_TXT "�˹���Ԥ���"
#define ST_STATE6_TXT "���ڽ��ж������"
#define ST_STATE7_TXT "���"
#define ST_STATE8_TXT "����λ"




enum
{
    //  NA - NEW ACCOUNT
    SUCCESS         =   0,      /*�ɹ�*/
    NA_CONN_FAIL    = -1,   /**< ��������ʧ�� */
    NA_SEND_FAIL    = -2,   /**< ���ݷ���ʧ�� */
    NA_RECV_FAIL    = -3,   /**< ���ݽ���ʧ�� */
    NA_RECV_NOTHING = -4,   /**< ����δ���յ� */
    NA_EID_WRONG    = -5,   /**< ��������� */
    NA_APPLY_FAIL   = -6,   /**< Ӧ��ʧ�� */
    NA_SAVE_CLIINFO = -7,   /**< ����ͻ�����ʧ�� */
    NA_SAVE_STPINFO = -8,   /**< ����ӡ������ʧ�� */
    NA_DEL_NCLIINFO = -9,   /**< ɾ������ʧ�� */
    NA_SAVE_TASK    = -10,  /**< ��������ʧ�� */
    NA_SCAN_EXCPT   = -11,  /**< ɨ�跢���쳣 */
    NA_SAVE_EXCPT   = -12,  /**< ���淢���쳣 */
    // CK - CHECK IDENTIFY
    CK_GET_VCHSAMP  = -21,  /**< ����Ʊ����Ϣʧ�� */
    CK_VCHSAMP_ERR  = -22,  /**< ����ƾ֤ʧ�� */
    CK_CHK_VCHSAMP  = -23,  /**< ���ƾ֤ʧ�� */
    CK_NO_LOACC     = -24,  /**< �ޱ��ؿͻ� */
    CK_ABTIME_FAIL  = -25,  /**< ��ȡӡ��ʱ��ʧ�� */
    CK_CONN_FAIL    = -26,  /**< ��������ʧ�� */
    CK_SEND_FAIL    = -27,  /**< ���ݷ���ʧ�� */
    CK_RECV_FAIL    = -28,  /**< ���ݽ���ʧ�� */
    CK_DL_FAIL      = -29,  /**< ���ؿͻ�����ʧ�� */
    CK_NO_STAMP     = -30,  /**< �ͻ���ӡ�� */
    CK_VDT_WRONG    = -31,  /**< ƾ֤���ڴ��� */
    CK_UKN_AB       = -32,  /**< δ֪ӡ���� */
    CK_ID_FAIL      = -33,  /**< ʶ��ʧ�� */
    CK_ID_EXCPT     = -34,  /**< ʶ���쳣 */
    CK_STPCOUNT_ERR = -35,  /**< ӡ������������ */
    CK_UNKNOWN_RES  = -36,  /**< δ֪ʶ���� */
    CK_ANGEL_IMG    = -37,  /**< �����۽�ͼʧ�� */
    CK_CHK_TYP_MISS = -38,  /**< ƾ֤���ʹ��� */
    CK_CHK_TYP_UNCERTAIN    = -39,  /**< ƾ֤����δ֪ */
    CK_CHK_MARK_MISS    = -40,  /**< ƾ֤���б�־���� */
    CK_CHK_MARK_UNCERTAIN   = -41,  /**< ƾ֤���б�־���� */
    CK_TER_DOWN     = -42,
    CK_QUERY_VCHNAME= -43,  /**< ��ѯƾ֤����ʧ�� */
    CK_PRE_ALTER    = -44,  /**< ��ͼ�� */
    CK_SHAPE_IMG    = -45,  /**< ��������ͼʧ�� */
    CK_CENTER_DENY  = -46,  /**< ��̨�ܾ����� */
    
    CK_NOCASH_ACC   = -49,  /**< �������ֻ� */
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
    CM_GETINFO_FAIL = -61,  /**< ��ȡ�ͻ���Ϣʧ�� */ 
    CM_WRT_ITEM     = -62,  /**< д�б�ʧ�� */ 
    CM_PRECHG_ITEM  = -63,  /**< �޸��б� */ 
    CM_FRZ_REC      = -64,  /**< �����¼ʧ�� */ 
    // CMT - CLIENT MANAGER TASK INFO
    CMT_ACC_PRE     = -65,  /**<  */ 
    CMT_ACC_ERR     = -66,  /**<  */ 
    CMT_ACC_ROW     = -67,  /**<  */ 
    CMT_INS_FAIL    = -68,  /**< �б����ʧ�� */ 
    CM_FIND_ITEM    = -69,  /**< �б����ʧ�� */ 
    // AD - ACCOUNT DETAIL  
    AD_GETINFO_FAIL = -71,  /**< ��ȡ�ͻ���Ϣʧ�� */ 
    AD_INFO_ERR     = -72,  /**< �ͻ����ݴ��� */
    AD_ERR_DEL      = -73,  /**< �ͻ�ɾ��ʧ�� */
    AD_BTIME_FAIL   = -74,  /**< �ɿ�ʱ����� */
    AD_BTIME_ROW    = -75,  /**< �ɿ��������� */
    AD_SCAN_EXCPT   = -76,  /**< ɨ���쳣 */
    // ADS - ACCOUNT DETAIL SAVE
    ADS_ACC_FAIL    = -77,  /**< ����ʧ�� */ 
    ADS_TASK_FAIL   = -78,  /**< ��������ʧ�� */ 
    ADS_STMP_FAIL   = -79,  /**< ����ӡ��ʧ�� */ 
    ADS_DOB_FAIL    = -80,  /**< �����ɿ�ʧ�� */ 
    ADS_INB_FAIL    = -81,  /**< ����ɿ�ʧ�� */ 
    ADS_DOA_FAIL    = -82,  /**< �����¿�ʧ�� */ 
    // AD                   
    AD_CK_OLD       = -83,  /**<  */ 
    
    
    // VS - VOUCHER SOCKET
    VS_CONN_FAIL    = -101, /**< ����ʧ�� */
    VS_SEND_FAIL    = -102, /**< ����ʧ�� */
    VS_RECV_FAIL    = -103, /**< ����ʧ�� */
    VS_RECV_NOTHING = -104, /**< ���������� */
    VS_EID_WRONG    = -105, /**< ��������� */
    // ICI - IS CLIENT IN DB
    ICI_READ_FAIL   = -106, /**< ��ȡ���ݿ�ʧ�� */
    ICI_NOT_IN      = -107, /**< ��Ϣ������ */
    ICI_READ_ROW    = -108, /**< �������� */
    ICI_ROW_ERR     = -109, /**< ��¼���� */
    ICI_MORE_ACC    = -110, /**< �������� */
                            
                    
    // SL - STAMP LIB PROC
    SL_READ_FAIL    = -111, /**< ��ȡʧ�� */
    SL_NOT_ONE      = -112, /**< ӡ����Ψһ */
    SL_UNCMP_FAIL   = -113, /**< ��ѹ��ʧ�� */
    SL_SCANIMG_FAIL = -114, /**< ɨ��ʧ�� */
    // SSL - SET STAMP LIB  
    SSL_NO_STMP     = -115, /**< ��ӡ���� */
    SSL_CMP_FAIL    = -116, /**< ѹ��ʧ�� */
    SSL_SAV_FAIL    = -117, /**< ����ʧ�� */
                            
    // SS - STAMP SHOW      
    SS_NO_STMPIMG   = -121, /**< ��ӡ���� */
    SS_CRET_FAIL    = -122, /**<  */
    // STM - STAMP WND
    STM_CREATE_DIB  = -125, /**< ����DIBʧ�� */
    STM_SHOW_SIGN   = -126, /**< ��ʾǩ��ʧ�� */
    STM_NO_SIGN     = -127, /**< ��ǩ�� */
                            
    // ST - SERVER TIME     
    ST_SOCK_ERR     = -131, /**< ����ͨѶ���� */
    ST_LOAD_DLL     = -132, /**< װ��Dllʧ�� */
    ST_GET_PROC     = -133, /**< װ�غ���ʧ�� */
    ST_SETTIM_FAIL  = -134, /**< ����ʱ��ʧ�� */
                            
    // DB - DATA BASE OPERATION
    DB_OPEN_FAIL    = -161, /**< �����ݿ�ʧ�� */
    DB_QUERY_FAIL   = -162, /**< ��ѯʧ�� */
    DB_RECORD_EMPTY = -163, /**< ������ */

    PARAMETER_ERR   = -171, /**< ��Ч�Ŀ��Ʋ�����*/

    // MS - MESSAGE SERVER SOCKET
    MS_DISCONNECT   = -181, /**< ���ӶϿ����� */

    // NW - Net Work Data Operation
    NW_CONN_FAIL    = -190, /**< ����ʧ�� */
    NW_SEND_FAIL    = -191, /**< ����ʧ�� */
    NW_RECV_FAIL    = -192, /**< ����ʧ�� */
    NW_COMP_FAIL    = -193, /**< ѹ��ʧ�� */
    NW_UCMP_FAIL    = -194, /**< ��ѹ��ʧ�� */
    NW_RECV_ERR     = -195, /**< ���մ��� */
    NW_OPEN_FAIL    = -196, /**< ��������ʧ�ܡ�*/
    NW_GETHOST      = -197, /**< ����gethostname()/gethostbyname()ʧ�ܡ�*/
    NW_BAD_SIZE     = -198, /**< ���ݴ�С����*/
    NW_BAD_POINTER  = -199, /**< ��Ч��ָ�롣*/
    
    XML_NOT_FOUND   = -200, /**< ����xml���ݣ�δ�ҵ�������Ϣ��*/
    
    
    AUTHORITY_FAIL  = -201,     /**< ��Ȩʧ�ܣ�*/
    AUTHORITY_BY_SELF = -202,   /**< ��Ȩ��Ա����Ϊ�Լ� */
    AUTHORITY_LEADER  = -203,  /**����һ����Ȩ */

//  IDENTIFY_INTERUPT = -1024
    XML_PARSE_ERROR     =-240,  /**< xml����ʧ�� */
    XML_DOCUMENT_ERROR  =-241,  /**< xml�ĵ����� */
    XML_NODE_ERROR      =-242,  /**< xml�ڵ���� */
    ALLOCATE_MEMORRY_FAILED=-243,   /**< �����ڴ�ʧ�� */
    MODE_VALUE_ERROR    =-244,  /**< ģʽ���� */
    
    
    //ǰ̨����
    FG_GEN_ERR      =-300,  /**< δ֪���� */
    FG_OPEN_FAIL    =-301,  /**< �����ݿ���� */
    FG_QUERY_FAIL   =-302,  /**< ��ѯ���� */
    FG_NO_RECORD    =-304,  /**< �޲�ѯ���� */
    FG_LEN_ERR      =-305,  /**< ���ȴ��� */
    FG_INSERT_FAIL  =-306,  /**< ������� */
    FG_DELETE_FAIL  =-307,  /**< ɾ������ */
    FG_UPDATE_FAIL  =-308,  /**< ���´��� */

    COM_SEND_ERR        =   -311,   /**< ���ܷ���ָ�� */
    COM_GET_CHAR_ERR    =   -312,   /**< ���ܵõ����봮 */
    COM_OPEN_ERR        =   -313,   /**< ���ܴ򿪴��� */
    COM_INIT_ERR        =   -314,   /**< ���ܳ�ʼ������ */
    COM_CLOSE_ERR       =   -315,   /**< ���ܹرմ��� */
    COM_OVER_TIME_ERR   =   -321,   /**< �������������ʱ */
    COM_PSWD_INPUT_ERR  =   -322,   /**< ����������� */

    /*ǰ̨һƱһ����ش���*/
    BG_RANDOM_ERR       =   -330,   /*���ܵõ�RDM*/
    BG_PINBLOCK_ERR     =   -331,   /*���ܵõ�PIN*/
    BG_ENCODEPIN_FAIL   =   -332,   /*�����ҵ�ENCODEPIN��*/
    BG_GETRANDOM_FAIL   =   -333,   /*�����ҵ�RDM����*/
    BG_GETPINBLK_FAIL   =   -334,   /*�����ҵ�PIN����*/
    BG_GETWKEY_FAIL     =   -335,   /*���ܵõ�WORKKEY*/
    BG_PWDCHECK_ERR     =   -336,   /*���ܵõ�WORKKEY*/
    BG_RECORD_EXIST     =   -337,   /*֧Ʊ�Ѵ���*/


    //��̨����
    BG_GEN_ERR      =-400,  /**< δ֪����  */
    BG_OPEN_FAIL    =-401,  /**< �����ݿ���� */
    BG_QUERY_FAIL   =-402,  /**< ��ѯ���� */
    BG_NO_RECORD    =-404,  /**< �޲�ѯ���� */
    BG_INSERT_FAIL  =-406,  /**< ������� */
    BG_DELETE_FAIL  =-407,  /**< ɾ������ */

    BG_RUN_TIME         =   -405,   /**< ���ݿ�������� */
    BG_COMMIT_FAIL  =-408,/*�����ύ����*/
        //BG_VRF_FAIL         =   -408,   /*�������*/           /////
    //2004.1.9
    BG_MODE_FAIL    =-409,/*�����ܲ���*/
    BG_MODE_INVALID =-410,/*������Ч*/
    BG_VRF_FAIL     =-411,/*��Ч����*/
    BG_CRT_FAIL     =-412,/*����ʧ��*/
    BG_MOD_FAIL     =-413,/*�޸�ʧ��*/
    BG_GET_WKEY_FAIL=-414,/*��ȡwkeyʧ��*/
    //BG_RECORD_EXIST =-415,/*���������Ѵ���*/

    BG_PVV_NEED_UPDATE =-416,/*pin is valid and pvv must be updated.*/
    
    BG_BREAK_TRANS =-417,/*cannot continue the trancsation*/
    
    BG_CHEQ_USED=-418,/*֧Ʊ��ʹ��*/
    
    BG_XML_NO_DATA  =-420, /*XML����������*/
    BG_GET_CLIENT_FAIL=-421 ,
    BG_GET_STAMP_FAIL =-422 ,
    BG_GET_CACHET_FAIL=-423 ,
    //ALLOCATE_MEMORRY_FAILED=-1111
    //2004.1.9
    BS_ENCODE_ERR       =   -451,   /**< ת�����ݸ�ʽʧ�� */
    BS_DECODE_ERR       =   -452,   /**< ��ת�����ݸ�ʽʧ�� */
    
    /**< XML���������� */
    
    XML_OPEN_ERR        =   -610,   /**< ���ܽ���XML���� */
    XML_GET_ROOT_ERR    =   -611,   /**< ���ܵõ�root�ڵ� */
    XML_GET_FIELD_ERR   =   -612,   /**< ���ܵõ�checkid�� */
    XML_ICONV_OPEN_ERR  =   -613,   /**< ���ܴ�ת������ */
    XML_ICONV_CLOSE_ERR =   -614,   /**< ���ܹر�ת������ */
    XML_ICONV_ERR       =   -615,   /**< ����ת���� */
    XML_KEYWORD_ERR     =   -621,    /**< �����ҵ��ؼ��� */
    XML_STAMP_DATA_ERR  =   -622,    /**< �����������ڴ����ҵ�ͼ�����ݶ� */
    XML_APPEND          =   -623,    /**< ���ܰ�����ճ����ָ������ĩβ */
    
    /**< �ļ��������� */
    FL_OPEN_ERR         =   -631,    /**< ���ܴ��ļ� */
    FL_READ_ERR         =   -632,    /**< ���ļ������ݴ��� */
    FL_ATTRIBUTE_ERR    =   -633,    /**< ���ܵõ��ļ����� */
    FL_DELETE_ERR       =   -634,    /**< ����ɾ���ļ� */
    FL_WRITE_ERR        =   -635,    /**< ����д���ݵ��ļ� */

    DT_GET_PHOTO        =   -641,    /*���ܵõ�ͼ������*/
    DT_NEXT_NONE        =   -645,    /*û����������*/
    /**< ���״��� */
    RQT_INVALID         =   -651,    /**< ��Ч���� */
    MOD_INVALID         =   -652,    /**< ��Чģʽ */

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
