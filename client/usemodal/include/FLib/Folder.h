//////////////////////////////////////////////////////////////////
//																//
//		用途 : 文件夹											//
//		创建 : [Foolish] / 2001-4-24							//
//		更新 : 2002-1-12										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_FOLDER_H__
#define		__FOO_FOLDER_H__
#include <windows.h>
#include <shlobj.h>
#pragma once

//===================================================================
//	显示选择文件夹的类
//===================================================================
class FCSelectFolder
{
public :
	FCSelectFolder (HWND hParent = NULL, PCTSTR szTitle = NULL) ;
	~FCSelectFolder () ;

	int		Select () ;	// return IDOK or IDCANCEL
	PCTSTR	GetPathName () ;

protected :
	//	重载函数
	virtual void  OnSelChange (ITEMIDLIST * pIdl) ;
	virtual void  BrowseDispatchMsg (HWND hWnd, UINT uMsg, LPARAM lParam) ;

	void	EnableOK (BOOL bEnable = TRUE) ;
	void	SetStatusText (PCTSTR szStatusText) ;

protected :
	BROWSEINFO	m_bi ;
	TCHAR		m_szPath[_MAX_PATH] ;
	HWND		m_hSelectDlg ;

	friend int CALLBACK __fooBrowseCallbackProc (HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData) ;
} ;

//===================================================================
inline FCSelectFolder::FCSelectFolder (HWND hParent, PCTSTR szTitle) {
	ZeroMemory (&m_bi, sizeof (BROWSEINFO)) ;
	m_bi.hwndOwner		= hParent ;
	m_bi.pidlRoot		= NULL ;
	m_bi.pszDisplayName	= m_szPath ;	
	m_bi.lpszTitle		= (szTitle == NULL) ? TEXT("选择文件夹") : szTitle ;
	m_bi.ulFlags		= BIF_STATUSTEXT | BIF_USENEWUI ;
	m_bi.lpfn			= __fooBrowseCallbackProc ;
	m_bi.lParam			= (LPARAM) this ;
	m_szPath[0]			= TEXT('\0') ;
}
inline FCSelectFolder::~FCSelectFolder () {
}
inline PCTSTR FCSelectFolder::GetPathName () {
	return (m_szPath[0] == TEXT('\0')) ? NULL : m_szPath ;
}
inline void  FCSelectFolder::EnableOK (BOOL bEnable) {
	::SendMessage (m_hSelectDlg, BFFM_ENABLEOK, (bEnable ? 1 : 0), 0) ;
}
inline void  FCSelectFolder::SetStatusText (PCTSTR szStatusText) {
	::SendMessage (m_hSelectDlg, BFFM_SETSTATUSTEXT, 0, (LPARAM) szStatusText) ;
}

#endif