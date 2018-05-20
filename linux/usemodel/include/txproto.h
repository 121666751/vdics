#ifndef __TXPROTO_H__
#define __TXPROTO_H__

#if defined( _MSC_VER ) || defined( __BORLANDC__ )
#pragma pack(4)
#endif



/*
****************************************************************
����1201��
��;����ҵ�������ϻ�ȡ�ͻ���Ϣ��
ͨѶ·�ߣ�������ӡ��������>����elink
���������txRequest
����������txClientInfo
����ֵ��0-�ɹ�������ʧ��
˵����
����ṹ�� eID=1201�� reqno���ͻ��ʺ�, flag=�ʻ�����
�����ṹ��Ӧ�����ͻ�������Ϣ������δ��˵�����ֶβ���Ҫ��䡣

....
���н��״��룺0116   �������ƣ����ͻ��ʺŲ�ѯ�ͻ���Ϣ(�Թ�����˽)
�������������������ڲ�ѯ����ͻ����ϼ�¼��Ϣ�����ݹ�Ա����Ŀͻ�
�ʺţ���ѯ���ݿ⣬Ȼ�������ѯ�Ľ����
****************************************************************
*/

typedef struct _txRequest
{
    long eID;       /* =1201*/
    char reqno[40]; /* �ʺ�*/
    long mode;      /* ���Ʋ���*/
    char info[12];  /* ���Ʋ���*/
    char flag[4];   /* ƾ֤����*/
    char clkno[12]; /* ��Ա��*/
    char depno[8];  /* ���źţ������ţ�*/
} txRequest;

typedef struct _txClientInfo
{
    char    accno[40];      /* �ʺ�*/
    char    compname[60];   /* ��˾����*/
    char    compaddr[60];   /* ��˾��ַ*/
    char    postalcode[8];  /* �ʱ�*/
    char    linkman[12];    /* ��ϵ��*/
    char    linkmanid[20];  
    char    tel[20];        /* ��ϵ�绰*/
    char    fax[20];        /* ����*/
    char    type[4];        /* �ͻ�����*/
    char    state[4];
    char    property[8];
    char    used[4];
    char    openclerk[12];  
    char    opendate[10];   /* ��������*/
    char    enabledate[10]; /* ��������*/
    char    disabledate[10];
    char    builddate[10];
    char    comtrade[4];
    char    netname[30];    /* ����������*/
    char    netdepno[10];   /* �����в��ź�*/
    char    subject[12];    /* ��Ŀ*/
    long    clientid;
    long    modifyno;
    long    moneylimit;
    char    passwd[20];
    char    memo[2028];
	char	netno[12];
    char    passwdflag[8];
    char    tx_result[8];       /*����ִ�н��*/
} txClientInfo;

/*
****************************************************************
����1202��
��;����ҵ�������ϻ�ȡ��Ա��Ϣ��
ͨѶ·�ߣ�������ӡ��������>����ilink
���������txRequest
����������txTellerInfo
����ֵ��0-�ɹ�������ʧ��
˵����
����ṹ�� eID=1202�� reqno����Ա��
�����ṹ��Ӧ������Ա������Ϣ������δ��˵�����ֶβ���Ҫ��䡣

����0����ù�Ա����ҵ���������еǼǲ���������ʹ�ã���������
��Ա������Ч��

......
���н��״��룺9307   �������ƣ��޸Ĺ�Ա��Ϣǰ��ѯ
�����������޸Ĺ�Ա��Ϣǰ��ѯ�� ǰ̨����ֻ�з��У����й���Ա������


****************************************************************
*/
typedef struct _txTellerInfo
{
    char    telrno[12];
    char    name[20];           /* ��Ա����*/
    char    pswd[16];       
    char    state[4];   
    char    grade[4];
    char    depno[12];
    long    priv;
    char    tx_result[8];       /*����ִ�н��*/
} txTellerInfo;


/*
****************************************************************
����1203��
��;����ҵ�������ϻ�ȡƾ֤��Ϣ����ѯ�Ƿ����ƾ֤�ѳ��۸�ָ���ʺ�
ͨѶ·�ߣ�������ӡ��������>����ilink
���������txVchReq
����������txVchInfo
����ֵ��0-�ɹ�������ʧ��
˵����
����ṹ�� eID=1203�� reqno���ʺţ�info=ƾ֤�ţ�flag=ƾ֤����
�����ṹ��Ӧ����ƾ֤������Ϣ������δ��˵�����ֶβ���Ҫ��䡣

����0����ù�Ա����ҵ���������еǼǲ���������ʹ�ã���������
��Ա������Ч��

......
���н��״��룺7117 ��ѯƾ֤����

****************************************************************
*/
typedef struct _txVchReq
{
    char accno[40]; /* �ʺ�*/
    char acctp[4];  /* �ͻ��ʺ�����*/
    char vchno1[10]; /* ��ʼƾ֤��*/
    char vchno2[10]; /* ��ֹƾ֤��*/
    long index;     /* ��ʼ����*/
    long total;     /* ��ѯ����*/
    char vchtp[4];  /* ƾ֤����*/
    char clkno[12]; /* ��Ա��*/
    char depno[8];  /* ���źţ������ţ�*/
    char bankno[4]; /* ���д���*/
    char vers[4];   /* ƾ֤����*/
} txVchReq;


typedef struct _txVchInfo
{
    char    accno[40];
    char    vchno[12];          
    char    vchtc[4];       
    char    state[4];           /*��ǰƾ֤״̬*/
    char    pswd[12];
    char    vers[4];            /*ƾ֤���κ�*/
    char    tx_result[8];       /*����ִ�н��*/
} txVchInfo;


/*
****************************************************************
����1208��
��;�������Զ�����
ͨѶ·�ߣ�������ӡ��������>����ilink
���������txTally
����������txTallyRes
����ֵ��0-�ɹ�������ʧ��
˵����
����ṹ�� eID=1208�� accno���ʺţ�vchno=ƾ֤�ţ�
vchtp=ƾ֤���룬vchdate=ƾ֤���ڣ�clkno=��Ա�ţ�
amount=ƾ֤���

����0������ʳɹ�

......
���н��״��룺8103��������: ͬ����ؽ跽
��������:���пͻ��ӱ���Ʊ�ݽ�������������пͻ��ĸ���Ҫ���Ľ��ף�
��������ͨ��ͬ��Ʊ���ֹ���Ʊ�ķ�ʽ����ͬ�Ƿ�Χ����


****************************************************************
*/
typedef struct _txTally
{
    long    eID;                /* ���״���      =1208 */
    char    account[32];        /* *�ʺ� */
    char    exchno[12];         /* *������ */
    char    vchno[10];          /* *ƾ֤��8 */
    char    vchtp[4];           /* ƾ֤����2 */
    char    vers[4];            /* ���κ�1 */
    char    bankno[4];          /* ���д���2 */
    char    amount[16];         /* *��� */
    char    amsrc[4];           /* *�ʽ���Դ */
    char    clkno[12];          /* *��Ա�� */
    char    depno[8];           /* *������ */
    char    digest[80];         /* *ժҪ */
    char    memo[80];           /* *��ע */
} txTally;

typedef struct _SINGLETALLYINFO		
{
	char accno[32];			//�ʺ�
	char vchtp[4];			//ƾ֤����
	char batno[4];			//���κ�
	char vchno[12];			//ƾ֤��
	char money[16];			//���
	char idres[4];			//ʶ����
	char sysres[12];		//ϵͳʶ����
	char manres[12];		//�˹�ʶ����
	char amsrc[32];			//��Դ��
	char date[12];			//����
	char clkno[16];			//��Ա��
	char depno[12];			//������
	char exno[12];			//������
	char memo[80];			//��ע
	char digest[80];		//ժҪ
	char tmp[128];			//�����ֶ�
}SINGLETALLYINFO;


typedef struct _txTallyRes
{
    char        tx_result[8];       /*���״�����*/
} txTallyRes;



#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus*/


/*
****************************************************************
�ӿں�����ʽ��
int tx_call(int eID, char* pRequest, char* pResult);


�˺������������ӡϵͳ��>elink֮���ͨѶ

eID: ���״���
pRequest: ���������ΪtxRequest��txClientData���͵�ָ�롣
pResult:�������������ڷ���ִ�нṹ�ṹ��

****************************************************************
*/
int tu_call(int eID, char* pRequest, char* pResult);


/*
****************************************************************
�ӿں�����ʽ��

int tx_tran(int eID, char* pRequest, int ReqLen, 
            char** ppResult, int* pResLen);

�˺������������ӡϵͳ��>elink֮���ͨѶ

eID: ���״���
pRequest: ���������ΪtxRequest��txClientData���͵�ָ�롣
ReqLen: ���볤�ȡ�
ppResult:�������������ڷ���ִ�нṹ�ṹ��
pResLen:�������ݳ��ȵ�ַ�����ڴ洢�������ȡ�
****************************************************************
*/
int tx_tran(int eID, char* pRequest, int ReqLen, 
            char** ppResult, int* pResLen);

/*
****************************************************************
�ӿں�����ʽ��
void tx_free(char* pResult);

pResult:����tx_tran���صĽṹָ�롣

˵����
�ú��������ͷ�tx_call�ж�̬������ڴ�

****************************************************************
*/
void tx_free(char* pResult);

/*
****************************************************************
�ӿں�����ʽ��
int svc_proc(int eID, char* pRequest, int ReqLen, char* realip, 
        char** ppResult, int* pResLen);

˵����
����realip����ǰ���ӵ�����IP,���������ͬ��tx_tran.

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
