//////////////////////////////////////////////////////////////////
//																//
//		用途 : General ( Win32 常用函数集 )						//
//		创建 : [Foolish] / 2001-3-15							//
//		更新 :													//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_GENERAL_WIN32_H__
#define		__FOO_GENERAL_WIN32_H__
#include <windows.h>
#include "StdDefine.h"
#pragma once

//=============================================================================

////////////////////////////////////////////////////////////
//	功  能 ：获得错误代码的文字描述
////////////////////////////////////////////////////////////
PTSTR  fooGetErrorString (DWORD dwError = ::GetLastError ()) ;
////////////////////////////////////////////////////////////
//	功  能 ：消息框
////////////////////////////////////////////////////////////
void  fooMsgBox (PCTSTR szMessage) ;
////////////////////////////////////////////////////////////
//	功  能 ：打开一个控制面板
//	参  数 ：
//	返回值 ：
//	说  明 ：桌面属性 --0		Internet属性--1		多媒体属性--2
//			 Modem属性--3		网络属性--4			电源管理--5
//			 添加/删除程序--6	鼠标属性--7			系统属性--8
//			 日期/时间--9		桌面主题--10
////////////////////////////////////////////////////////////
BOOL  fooControlPad (HWND hwnd, DWORD dwType) ;
////////////////////////////////////////////////////////////
//	作  用 ：屏保
//	参  数 ：
//	说  明 ：uiTimeOut 为分钟
////////////////////////////////////////////////////////////
BOOL  fooSetScreenSave (BOOL bEnable, UINT uiTimeOut) ;
BOOL  fooGetScreenSave (BOOL * bEnable, UINT * uiTimeOut) ;
////////////////////////////////////////////////////////////
//	作  用 ：把数字转化成字符串, 并加入","
//	参  数 ：iBufLen为szBuffer大小
////////////////////////////////////////////////////////////
void  fooFormatCommaNumber (DWORD dwNum, TCHAR * szBuffer, int iBufLen) ;
////////////////////////////////////////////////////////////
//	作  用 ：通用对话框操作
//	参  数 ：szFilter = "Executable Files (*.exe, *.dll)\0*.exe;*.dll\0"
//						"All Files (*.*)\0*.*\0\0" ;
//	说  明 ：
////////////////////////////////////////////////////////////
BOOL  fooShowFileDialog (HWND hWnd, BOOL bOpen,
						 TCHAR * szFullName,	// 返回的文件名
						 PCTSTR szIniDir = NULL,	// 当前目录
						 PCTSTR szTitle = NULL,
						 PCTSTR szFilter = NULL) ;

BOOL  fooShowColorDialog (COLORREF * ColorSelect,
						  COLORREF   InitColor[16] = NULL) ;
////////////////////////////////////////////////////////////
//	作  用 ：获取桌面显示信息
//	说  明 ：
////////////////////////////////////////////////////////////
void  fooGetLargestDisplayMode (DISPLAYMODE * mode) ;
void  fooGetCurrentDisplayMode (DISPLAYMODE * mode) ;
LONG  fooSetDisplayMode (DISPLAYMODE mode) ;
////////////////////////////////////////////////////////////
//	作  用 ：重定位控件
//	参  数 ：rcDest,rcSrc 为 GetClientRect 的返回值
//			 nMethod : DLG_RESIZE_MOVE 或 DLG_RESIZE_STRETCH
//	返回值 ：
//	说  明 ：在 OnSize 中调用
////////////////////////////////////////////////////////////
void  fooOnChangeDlgSize (HWND hDlg,
						  RECT rcDest, RECT rcSrc,
						  UINT nNumCtrl,
						  UINT nCtrl[],		// 控件数组
						  UINT nMethod[]) ;	// 定位方法数组
////////////////////////////////////////////////////////////
//	作  用 ：
//	参  数 ：fExpand : TRUE则展开对话框
//			 DefaultBox_ID : 预设对话框ID
//	返回值 ：
//	说  明 ：必须在Init时调用一次 缩小(fExpand : FALSE)
////////////////////////////////////////////////////////////
void  fooExpandBox (BOOL fExpand, HWND hDlg, int DefaultBox_ID) ;

//===================================================================
//	Implement
//===================================================================
inline void  fooMsgBox (PCTSTR szMessage) {
	TCHAR	szTitle[128] ;
	::GetModuleFileName (NULL, szTitle, 128) ;
	::MessageBox (NULL, szMessage, szTitle, MB_OK) ;
}
inline BOOL  fooSetScreenSave (BOOL bEnable, UINT uiTimeOut) {
	BOOL	bResult = SystemParametersInfo (SPI_SETSCREENSAVEACTIVE, bEnable, NULL, SPIF_SENDCHANGE| SPIF_UPDATEINIFILE) ;
	if (bEnable)
		bResult &= SystemParametersInfo (SPI_SETSCREENSAVETIMEOUT, uiTimeOut*60, NULL, SPIF_SENDCHANGE| SPIF_UPDATEINIFILE) ;
	return bResult ;
}
inline BOOL  fooGetScreenSave (BOOL *bEnable, UINT *uiTimeOut) {
	BOOL	bResult = SystemParametersInfo (SPI_GETSCREENSAVEACTIVE, 0, bEnable, SPIF_SENDCHANGE) ;
	bResult &= SystemParametersInfo (SPI_GETSCREENSAVETIMEOUT, 0, uiTimeOut, SPIF_SENDCHANGE) ;
	*uiTimeOut /= 60 ;
	return bResult ;
}

//===================================================================

#endif