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
 * �ļ���	$Id: ztpbm.h,v 1.1 2008/03/30 10:32:38 Wu Exp $
 * �汾��	$Revision: 1.1 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
 *
 ***************************************************************************
 */

#ifndef __ZTPBM_H__
#define __ZTPBM_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

	/** 
	* ��ʼ��Application���ú���Ӧ��ϵͳ����ʱ���е��á�
	* @param ver �ͻ��˵İ汾
	* @return 0-success
	*/
	int zt_pub_initApp(int ver);

	/** 
	* ��¼���ơ���ʾ��¼���棬���ȴ���Ա���룬Ȼ����֤��Ա��Ч�ԡ�
	* @return 0-success, 2 ȡ��
	*/
	int zt_pub_login();

	/**
	ѹ����½��Ϣ��
	*/
	int zt_pub_login_encode(unsigned char*src,unsigned char *desc);

	int zt_pub_login_decode(unsigned char*src,unsigned char*desc);
/**
	
*/
/**
	*�����е�½
	* @return 0-success, 2 ȡ��
*/
int zt_pub_login_ex(const char*cmd);
/**
 * �޸Ĺ�Ա����
 *
 * @return 0-success
 */
void zt_pub_chpasswd();
 
/** 
 * ��õ�¼�Ĺ�Ա��
 * @return ��Ա��
 */
const char* zt_pub_getClkno();
 
/** 
 * @return ��Ա��¼����
 */
const char* zt_pub_getPasswd();
 
/** 
 * ��û�����
 */
const char* zt_pub_getDepno();

const char* zt_pub_getBankId();
 
/** 
 * �ж��Ƿ�ӵ��ĳ��Ȩ��
 * @return true/false
 */
bool zt_pub_hasPriv(const char* priv);
 
/** 
 * ���й�Ա����
 */
void zt_pub_clerkManage();
 
 
/** 
 * ���н�ɫȨ�޹���
 */
void zt_pub_roleManage();
 
/** 
 * ���л�������
 */
void zt_pub_departManage();
 
/** 
 * IP����
 */
void zt_pub_ipManage();
 
/** 
 * ��ʾ�ͻ���ϸ����
 * @param accno �˺�
 * @return 0-success
 */
int zt_pub_clientDetail(const char* accno);
 
 
/** 
 * @return ����������
 */
const char* zt_pub_serverDate();
 
/** 
 * @return web server address
 */
const char* zt_pub_webAddress();
 
 
/** 
 * ��Ȩ���������ô˺���֮ǰ�����������ù�ui_login��¼������
 * @param opdepno		�������Ļ�����
 * @param caption		��Ҫ����Ȩ��
 * @param out_warrent_clkno	[out]��Ȩ��Ա��,������������ɵ����߷����㹻�Ŀռ䡣
 * @return 0-��Ȩ�ɹ�����Ҫ��Ȩ, -1-failed.
 */
int zt_pub_warrant(const char* opdepno,const char* priv, char* out_warrant_clkno);
 
 
/**
 * ���ýڼ�����Ϣ
 *
 * @return 
 */
void zt_pub_holiday();


/**
 * ������Ȩ����
 * @return 0-�ɹ�������-ʧ��
 */
void zt_pub_warrantSetup();
void zt_pub_warrantDef();
/*
	����Ա�Ƿ����ض��Ĳ���Ȩ��
*/
zt_pub_checkPriv(const char* opdepno,const char* priv);

#ifdef __cplusplus
}
#endif

/************************************************************************/
/* Only for VC++                                                        */
/************************************************************************/
#ifdef _MSC_VER
#ifndef OPER_EXPORT
#define OPERAPI __declspec(dllimport)
#else
#define OPERAPI __declspec(dllexport)
#endif //_USRDLL

class CBaseApp;
OPERAPI CBaseApp* GetBaseApp();


#endif //_MSC_VER

// --------------------------------------------------------------------------
#endif//__OPERMAN_H__
