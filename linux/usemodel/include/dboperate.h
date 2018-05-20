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
 * �ļ���	$Id: dboperate.h,v 1.1 2008/03/30 10:05:04 Wu Exp $
 * �汾��	$Revision: 1.1 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
 *
 ***************************************************************************
 */

 #ifndef __dboperate_h_
#define __dboperate_h_

#include <libmem.h>
#include <STRING>
#include <zcommon.h>
#include <vchproc.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
	dboperate.dll �ṩ�Զ�����ϵͳ�����ݿ���ʵĹ���
*/


struct idres_node_t{
	int sid;					/**<��ˮ��*/
	char accno[30];				/**<�˺�*/
	char vchno[20];				/**<֧Ʊ��*/
	char exchno[20];			/**<������*/
	char amount[20];			/**<���*/
	int sysidres;				/**<�Զ�ʶ����*/
	int manidres;				/**<�ֶ�ʶ����*/
	int state;					/**<��¼״̬*/
	char vchtype[5];			/**<ƾ֤����*/
	int  modifyno;				/**<���ĺ�*/
	char pkgno[20];				/**<����*/
	char clerk[20];				/**<��Ԥ��Ա*/
	char chkclk[20];			/**<��Ʊ��Ա*/
	char extflag[16];			/**<������Ϣ*/
	char reverse[512];			/**<�����ֶ�*/
};

struct idres_detail_t{
	int sid;					/**<��ˮ��*/
	char accno[30];				/**<�ʺ�*/
	char vchno[10];				/**<֧Ʊ��*/
	char vchtype[5];			/**<֧Ʊ����*/
	char moneytype[5];			/**<��������*/
	char exchno[20];			/**<������*/
	char amount[20];			/**<���*/
	int sysidres;				/**<�Զ�ʶ����*/
	int manidres;				/**<�ֶ�ʶ����*/
	char clerk[20];				/**<��Ԥ������Ա��*/
	int state;					/**<״̬λ��0-����Ԥ 1-���ύ 2-���ڸ�Ԥ 3-��Ԥ���*/
	char sysstpres[20];			/**<�Զ�ÿ����ʶ����*/
	char manstpres[20];			/**<�ֶ�ÿ����ʶ����*/
	unsigned char *resimg;		/**<ƥ��ͼ*/
	long imglen;				/**<ƥ��ͼ����*/
	int modifyno;				/**<���ĺ�*/
	char e13b[60];				/**<�����*/
	char fname[40];				/**<ͼ���ļ����ļ���*/
	char pkgno[20];				/**<����*/
	char idtime[25];			/**<ʶ��ʱ��*/
	char extflag[16];			/**<������Ϣ*/
	char tel[40];				/**<��ϵ�绰*/
	char checkclk[20];			/**<��Ʊ��Ա*/
	char warrantclk[20];		/**<Ʊ�������Ȩ��Ա*/
	char warrantidres[20];      /**<��ӡ�����Ȩ��Ա*/
	char memo[200];				/**<�ͻ���Ϣ*/
        char idmemo[100];           /**<ʶ����Ϣ*/	
	char companyname[64];       /**<��˾����*/
	char vsacc[32];             /**<�տ����ʺ�*/
	char vsname[64];            /**<�տ��˻���*/
	char date[12];              /**<����*/
	char session[4];            /**<����*/
        char money_e13b[20];		/**< ԭʼ������ */
	char money_digit[20];		/**< ԭʼСд��� */
	char money_cn[20]; 			/**< ԭʼ��д��� */
	char vchdate[12];			/**< ƾ֤���� */
	char vchno_e13b[10];        /**<����� ֧Ʊ��*/
	char vchno_digit[10];       /**<Сдƾ֤��*/ 
	char reverse[156];			/**<Ԥ���ֶ�*/
};

struct imgfile_node_t{
	char fname[40];				/**<�ļ���*/	
	int dpi;					/**<dpiֵ*/
	double calx;				/**<ɨ����x��У׼ֵ*/
	double caly;				/**<ɨ����y��У׼ֵ*/
	int rotate;					/**<��ת�Ƕ�*/
	char pkgno[20];				/**<���κ�*/
	int pkgflag;				/**<�������ͱ�־λ��0-���� 1-������*/
	char pkgdate[10];			/**<��������*/
	int pkgsess;				/**<����*/
	char prefix;				/**<ͼ���ļ����е�ǰ׺���������ֲ�̨ͬɨ����*/
	char side;					/**<���������棺A-���� B-����*/
	int fcount;					/**<ͼ���ļ����е����ֲ���*/
	char ip[20];				/**<�ļ����ڼ����ip��ַ*/
	int state;					/**<����״̬*/
	char colortype[12];			/**<��ɫ����*/
	char clerk[20];				/**<������Ա��*/
	int proctype;				/**<��������:0-������ 1-����*/
	char reserve[512];			/**<������Ϣ*/
};

struct pkgno_t{
	char pkgno[20];				/**<����*/
	char reserve[512];
};

struct idmemo_t{
	char idmemo[98];			/**<ʶ����Ϣ*/
	char reserve[512];
};

struct at_exchst_t
{
	char pkgno[20];
	char state[20];
	char reserved[100];
};


/**
	*�ṩ���ݿ�������,������ʧ�ܷ���NULL
*/

HANDLE dbo_init();


/**
	*�ṩ���ݿ�������,������ʧ�ܷ���NULL
*/

HANDLE dbo_init_ex(const char *database);

/**
	*������е���ӡ���	
	*@dbh:		���ݿ������� 
	*@nodelist:	[out]�ڵ���Ϣ�б�
	*@count��	[out]���صĽڵ���Ϣ����,resnode����mem_free()�ͷ�
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/

int dbo_loadalldata(HANDLE dbh,idres_node_t *&resnode,int &count);


/**
	*ɾ�����ݿ�������
	*@dbh: ���ݿ�������
*/

void dbo_close(HANDLE dbh);



/**
	*������б�modifyno��Ľڵ���Ϣ	
	*@dbh:		���ݿ������� 
	*@modifyno: ���ĺ�
	*@nodelist:	[out]�ڵ���Ϣ�б�
	*@count:	[out]���صĽڵ���Ϣ����,resnode����mem_free()�ͷ�
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_loadnewdata(HANDLE dbh,int modifyno,idres_node_t *&resnode,int &count);


/**
	*���ĳ����ˮ������Ӧ��ʶ����Ϣ����ϸ��Ϣ	
	*@dbh:			���ݿ������� 
	*@sid:			��ˮ��
	*@resdetail:	[out] ��ϸ��Ϣ�ṹ��,resdetail.resimg����mem_free()�ͷ�
	*@return:		0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_loaddetail(HANDLE dbh,int sid,idres_detail_t &resdetail);

/**
	*���ڻ��baseno��������¼��
	*@dbh:		���ݿ������� 
	*@flag:		ϵͳ��־λ 0-�Զ�����
	*@modifyno: [out]����¼��
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_getmodifyno(HANDLE dbh,int flag,int &modifyno);

/**
	*���ڼ�¼ĳ����ˮ������Ӧ��ʶ����Ϣ���˹���Ԥ���
	*@dbh:		���ݿ������� 
	*@detail:	��ϸ��Ϣ
	*@imgflag:	0 ��ͼ�� 1 ����ͼ��
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_updateidres(HANDLE dbh,const idres_detail_t *detail,int imgflag);

/**
	*��������idres����sid����Ӧ��state��ֵ
	*@dbh:		���ݿ�������
	*sid:		��ˮ��
	*state:		ӡ�µĸ�Ԥ��� 0-����Ԥ 1-���ύ 2-���ڸ�Ԥ 3-��Ԥ���
*/
int dbo_setidres(HANDLE dbh,int sid,int state);

/**
	*���ڽ�����Ԥ��¼��״̬����Ϊ���ڸ�Ԥ
	*@dbh:		���ݿ������� 
	*@sid:		��ˮ��
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_lockidres(HANDLE dbh,int sid);

/**
   *���ڻ��ʶ����Ϣ
   *@dbh:	 ���ݿ������� 
   *@mem:	 ʶ����Ϣ
   *@count:  ����
   *@return: 0-�ɹ� ����:ʧ��
*/
int dbo_loadidmemo(HANDLE dbh,MemT &mem,int &count);

/**
   *���ڲ���ʶ����Ϣ
   *@dbh:		���ݿ������� 
   *@memolist:	ʶ����Ϣ
   *@count:     effect rows
   *@return: 0-�ɹ� ����:ʧ��
*/
int dbo_insertidmemo(HANDLE dbh,const char* memo,int &count);

/**
	*���ڻ��ĳ״̬����Ӧ�����а���
	*@dbh:		���ݿ������� 
	*@flag: 	״̬��Ϣ,0-δ�ύ 1-���ύ
	*@pkgno:	[out]������Ϣ�ṹ���ڴ�顣����mem_free()�ͷ�
	*@count:	[out]���صĽṹ������
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_loadpkgno(HANDLE dbh, int flag,pkgno_t *&pkgno,int &count,const char*date,const char*session);

/**
	*���ڻ��ĳ��������Ӧ��ĳ״̬�����нڵ���Ϣ
	*@dbh:		���ݿ������� 
	*@pkgno:	���κ�
	*@flag: 	״̬��Ϣ��0-δ�ύ 1-���ύ
	*@reslist:	[out]�ڵ���Ϣ�ṹ���ڴ�顣
	*@count:	[out]���صĽṹ������
	*@date:     ����
	*@session:  ����
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_loadpkg(HANDLE dbh,const char *pkgno,int flag,idres_node_t *&reslist,int &count,const char*date,const char*session);

/**
	*�����ύĳ���ε�������
	*@dbh:		���ݿ������� 
	*@pkgno:	���κ�
	*@clkno:    �ύ��Ա 
	*@date:     ����
	*@session:  ����
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>	
*/
int dbo_submit(HANDLE dbh,const char *pkgno,const char*date,const char*session,const char* clkno);

/**
	*�������ύǰ����Ƿ�����ύ
	*@dbh:		���ݿ������� 
	*@pkgno:	���κ�
	*@date:     ����
	*@session:  ����
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_presubmit(HANDLE dbh,const char *pkgno,const char*date,const char*session);

/**
	*���ڷ���imgfile����ĳǰ׺����Ӧ����fcount��ֵ
	*@dbh:		���ݿ�������
	*@prefix:	ǰ׺
	*@fcount:	[out]���ڷ�������fcount,���û�з��ϼ�¼����Ϣ����-1
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_image_maxcount(HANDLE dbh,char prefix,long &fcount);

/**
	*���ڸ���imgfile����ĳǰ׺����Ӧ����fcount��ֵ
	*@dbh:  	���ݿ�������
	*@prefix:	ǰ׺
	*@fcount:	[in]����fcount
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/

int dbo_image_updatemax(HANDLE dbh,char prefix,long &fcount);

/**
	*������imgfile���в���ڵ���Ϣ��
	*@dbh:		���ݿ�������
	*@node:		Ҫ����Ľڵ���Ϣ�Ľ����
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_image_insert(HANDLE dbh,const imgfile_node_t &node);

/**
	*�������ڲ�ѯat_exchst����ĳ��������Ӧ��ɨ���ǵ�״̬��
	*@dbh:		���ݿ�������
	*@pkgno:	����
	*@state:	[out]���ڷ���״̬��Ϣ,�������κ�����Ӧ��¼����-1
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_image_scaninfo(HANDLE dbh,const char *pkgno,int &state);

/**
	*������at_exchst���и��»����ĳ��������Ӧ��ɨ���ǵ�״̬
	*@dbh:		���ݿ�������
	*@pkgno:	����
	*@state:	״̬��Ϣ
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_image_update(HANDLE dbh,const char *pkgno,int state);

/**
	*�������ڴ�imgfile���еõ�δ������fcountֵ��С���ļ���Ϣ��fname����Ӧ��Ϣ��
	*@dbh:		���ݿ�������
	*@fname:	�ļ��������ΪNULL����δ������fcountֵ��С���ļ���Ϣ�����򷵻��ļ�������Ӧ��Ϣ
	*@imgnode��	[out]imgfile_node_t�ṹ�壬���ڷ����ļ���Ϣ
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_getimgfile(HANDLE dbh,const char *fname,imgfile_node_t &imgnode);

/**
	*������������imgfile����state��ֵ
	*@dbh:		���ݿ�������
	*@fname:	�ļ�����
	*@state:	�����״̬��0-δ���� 1-������� 2-��������
*/
int dbo_setimgfile(HANDLE dbh,const char *fname,int state);

/**
	�������ڱ�����������Ϣ��
	*@dbh:		���ݿ�������
	*@resnode:	idres_allnode_t�ṹ�����ڴ洢��������Ϣ
	*@imgflag:	0 ��ͼ�� 1 ����ͼ��
	*@return:	0-�ɹ�������ʧ�ܣ��������<zerrno.h>
*/
int dbo_saveidres(HANDLE dbh,const idres_detail_t &resnode,int imgflag);

/**
	����������ȡconfig���У�name�ֶ�����Ӧ��value��ֵ
	*@:dbh:		���ݿ�������
	*@:name:	Ԥ��ѯ��name��ֵ
	*@:value:	[out]���ڷ���value��ֵ
	*@return:	0-�ɹ���1-�޼�¼ ����ʧ��
*/
int dbo_getcfg(HANDLE dbh,const char *name,char *value);


/**
	����������config���У�����name�ֶμ�value���ֶ�
	*@:dbh:		���ݿ�������
	*@:name:	Ԥ�����name��ֵ
	*@:value:	Ԥ�����value��ֵ
*/
int dbo_setcfg(HANDLE dbh,const char *name,const char *value);

/************************************************************************/
/*	������������ĳ���κŵĲ��μ������Ʊ�ݣ���at_exchst������ӡλ��״̬	*/
/************************************************************************/
int dbo_set_cridstate(HANDLE dbh,const char *pkgno,int state);

/**
	*��ȡȫ����������״̬
*/
int dbo_allexchst(HANDLE dbh,MemT& mem, int& count,const char*date,const char*session);


/**
	*��ȡĳ��������״̬
*/
int dbo_getexchst(HANDLE dbh,const char* pkgno, char* state,const char*date,const char*session);

/**
	*����ĳ��������״̬
*/
int dbo_setexchst(HANDLE dbh,const char* pkgno, char* state,const char*date,const char*session);

/*
 *�õ�һ��Ҫ���ʵ�����Ľ����ź�״̬�����Ѹ������޸�Ϊ������
 *@param dbh: ���ݿ���
 *@param state: ״̬
 *@param exchno: ������
 *@return:  0-��ȷ������-����ȷ
 */
int dbo_gettallyexno(HANDLE dbh,char* pkgno, char* state);

/*
 *������Ƿ���ȷ
 *@param dbh: ���ݿ���
 *@param amount: ���
 *@param exchno: ������
 *@return:  0-��ȷ������-����ȷ
 */
int dbo_checkamount(HANDLE dbh,const char* amount,const char* exchno,const char*date,const char*session);

/*
 *	���ҹ���ʺ�
 *@param dbh: ���ݿ���
 *@param accno: �ʺ�
 *@reutrn: 0-���� 1-�� ��������ѯ����
 */
int dbo_checkvip(HANDLE dbh,const char* accno);

/*
 *	�Ƿ����û��ʶ�����Ʊ��
 *@param dbh:    ���ݿ���
 *@param exchno: ������
 *@reutrn:       0-�����, 1-δ���, ��������ѯ����
 */
int dbo_isfinished(HANDLE dbh,const char* exchno,const char*date,const char*session);

/*
 *	�޸�״̬Ϊδʶ��
 *@param dbh:     ���ݿ���
 *@param exchno:  ������
 *@reutrn:        0-����, ��������ѯ����
 */
int dbo_reidentify(HANDLE dbh,const char* exchno,const char*date,const char*session);

/*
 * ɾ��һ�����εļ�¼����������	
 *@param dbh:     ���ݿ���
 *@param exchno:  ������
 *@reutrn:        0-����, ��������ѯ����
 */
int dbo_deletepkg(HANDLE dbh,const char* exchno,const char*date,const char*session);

/*
 *@param dbh: ���ݿ���
 *@param exchno:    ������
 *@param value:     �ܽ��
 *@reutrn:          0-����, ��������ѯ����
 */
int dbo_totalmoney(HANDLE dbh,const char* exchno,char* value,const char*date,const char*session);

/*
 *@param dbh:   ���ݿ���
 *@param sid:   ͳһ���
 *@reutrn:      0-����, ��������ѯ����	
 */
int dbo_processone(HANDLE dbh,int sid);

/*
 *ɾ��һ����¼
 *@param dbh: ���ݿ���
 *@param sid: ͳһ���
 *@reutrn:    0-����, ��������ѯ����	
 */
int dbo_deleteone(HANDLE dbh,int sid);



/*
 *����ֱ��е�ȡͼ����Ϊfname���ʺ�,ƾ֤�ŵ���Ϣ
 *@param dbh:   ���ݿ���
 *@param fname: ͼ����
 *@outInfo:     ����ṹ��
 */

int dbo_getVchInfo(HANDLE dbh,const char*fname,VCHOCRRESULT *outInfo);

/*
 *ǿ�Ƹ�����ӡ�����Ϣ
 *@param dbh:   ���ݿ���
 *@param sid:   ���
 *@result:      ���
 *@idclkno:     ��Ԥ��Ա
 *@warclkno:    ��Ȩ��Ա
 */

int dbo_forceupdate(HANDLE dbh,int sid,int result,const char* idclkno,const char* warclkno);

/*
 *�����ļ����õ����ڳ���
 *@param dbh :���ݿ���
 *@fname     :�ļ���
 *@date      :����
 *@session   :����
 */
int dbo_getds(HANDLE dbh,const char*fname,char *date,char*session );

#ifdef __cplusplus
}
#endif

#endif