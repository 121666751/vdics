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
 * 文件：	$Id: ztpbm.h,v 1.1 2008/03/30 10:32:38 Wu Exp $
 * 版本：	$Revision: 1.1 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */

#ifndef __ZTPBM_H__
#define __ZTPBM_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

	/** 
	* 初始化Application。该函数应在系统启动时进行调用。
	* @param ver 客户端的版本
	* @return 0-success
	*/
	int zt_pub_initApp(int ver);

	/** 
	* 登录控制。显示登录界面，并等待柜员输入，然后验证柜员有效性。
	* @return 0-success, 2 取消
	*/
	int zt_pub_login();

	/**
	压缩登陆信息。
	*/
	int zt_pub_login_encode(unsigned char*src,unsigned char *desc);

	int zt_pub_login_decode(unsigned char*src,unsigned char*desc);
/**
	
*/
/**
	*命令行登陆
	* @return 0-success, 2 取消
*/
int zt_pub_login_ex(const char*cmd);
/**
 * 修改柜员密码
 *
 * @return 0-success
 */
void zt_pub_chpasswd();
 
/** 
 * 获得登录的柜员号
 * @return 柜员号
 */
const char* zt_pub_getClkno();
 
/** 
 * @return 柜员登录密码
 */
const char* zt_pub_getPasswd();
 
/** 
 * 获得机构号
 */
const char* zt_pub_getDepno();

const char* zt_pub_getBankId();
 
/** 
 * 判断是否拥有某个权限
 * @return true/false
 */
bool zt_pub_hasPriv(const char* priv);
 
/** 
 * 进行柜员管理
 */
void zt_pub_clerkManage();
 
 
/** 
 * 进行角色权限管理
 */
void zt_pub_roleManage();
 
/** 
 * 进行机构管理
 */
void zt_pub_departManage();
 
/** 
 * IP管理
 */
void zt_pub_ipManage();
 
/** 
 * 显示客户详细资料
 * @param accno 账号
 * @return 0-success
 */
int zt_pub_clientDetail(const char* accno);
 
 
/** 
 * @return 服务器日期
 */
const char* zt_pub_serverDate();
 
/** 
 * @return web server address
 */
const char* zt_pub_webAddress();
 
 
/** 
 * 授权函数。调用此函数之前必须曾经调用过ui_login登录函数。
 * @param opdepno		被操作的机构号
 * @param caption		需要检查的权限
 * @param out_warrent_clkno	[out]授权柜员号,输出参数，须由调用者分配足够的空间。
 * @return 0-授权成功或不需要授权, -1-failed.
 */
int zt_pub_warrant(const char* opdepno,const char* priv, char* out_warrant_clkno);
 
 
/**
 * 设置节假日信息
 *
 * @return 
 */
void zt_pub_holiday();


/**
 * 设置授权交易
 * @return 0-成功，其它-失败
 */
void zt_pub_warrantSetup();
void zt_pub_warrantDef();
/*
	检查柜员是否有特定的操作权限
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
