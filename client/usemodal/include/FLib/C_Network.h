//////////////////////////////////////////////////////////////////
//																//
//		用途 : 网络相关操作										//
//		创建 : [Foolish] / 2002-9-29							//
//		更新 :													//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_NETWORK_H__
#define		__FOO_NETWORK_H__
#include <windows.h>
#include <TCHAR.H>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/****************************************************************/
	/*				显示域名选择对话框 (win2000/XP)					*/
	/*				 dwBufLen为缓冲区大小(字符数)					*/
	/*					用户按OK键, 返回TRUE						*/
	/****************************************************************/
	BOOL  fooChooseDomainDlg (HWND hParent, LPWSTR wCompName, DWORD dwBufLen) ;

//===================================================================

#ifdef __cplusplus
}
#endif
#endif