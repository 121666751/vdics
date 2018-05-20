//////////////////////////////////////////////////////////////////
//																//
//		用途 : 托盘类											//
//		创建 : [Foolish] / 2001-5-10							//
//		更新 :													//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_TRAYICON_H__
#define		__FOO_TRAYICON_H__
#include <windows.h>
#include <shellapi.h>
#pragma once

#define		FM_TRAYICON		(WM_APP + 0x013)
#define		TRAYICON_ID		0x678
/************************************************************/
/*		鼠标事件发生时发送 FM_TRAYICON 给 hwnd				*/
/*	wParam ：为 TrayIcon 标示符 ( ID )						*/
/*	lParam ：WM_L（R）BUTTONDOWN ,							*/
/*			 WM_L（R）BUTTONDBLCLK ,						*/
/*			 WM_MOUSEMOVE									*/
/************************************************************/
//===================================================================
//	Declaration
//===================================================================
class FCTrayIcon
{
public :
	 FCTrayIcon () ;
	~FCTrayIcon () ;

	BOOL	Add (HWND hwnd, HICON hIcon, UINT uID = TRAYICON_ID, PCTSTR szTip = TEXT("foolish tray")) ;
	BOOL	Add (HWND hwnd, UINT ID_Icon, UINT uID = TRAYICON_ID, PCTSTR szTip = TEXT("foolish tray")) ;
	BOOL	Delete () ;

	BOOL	ChangeIcon (HICON hIcon) ;
	BOOL	ChangeIcon (UINT ID_Icon) ;
	BOOL	ChangeTip (PCTSTR szTip) ;

	void	SetMenu (PCTSTR szMenuRes) ; // Menu由类自己维护
	void	SetMenu (UINT nIDMenu) ;
	void	PopupMenu (UINT uFlags = TPM_RIGHTALIGN) ;
	HMENU	GetMenu () ;
	UINT	GetCtrlID () ;

protected :
	HMENU				m_hMenu;
	NOTIFYICONDATA		m_nid ;
} ;

//===================================================================
//	Implement
//===================================================================
inline FCTrayIcon::FCTrayIcon () {
	ZeroMemory (&m_nid, sizeof(m_nid)) ;
	m_nid.cbSize			= sizeof (m_nid) ;
	m_nid.uCallbackMessage	= FM_TRAYICON ;
	m_nid.uFlags			= NIF_ICON | NIF_TIP | NIF_MESSAGE ;
	m_hMenu					= NULL ;
}
inline FCTrayIcon::~FCTrayIcon() {
	if (m_hMenu)
		::DestroyMenu (m_hMenu) ;
	this->Delete () ;
}
//	Add a Icon to System Tray
inline BOOL  FCTrayIcon::Add (HWND hwnd, HICON hIcon, UINT uID, PCTSTR lpszTip) {
	if ( !hwnd || !hIcon )
		return FALSE ;
	m_nid.hWnd	= hwnd ;
	m_nid.hIcon	= hIcon ;
	m_nid.uID	= uID ;
	if (lpszTip)
		lstrcpy (m_nid.szTip, lpszTip) ;
	return ::Shell_NotifyIcon (NIM_ADD, &m_nid) ;
}
inline BOOL FCTrayIcon::ChangeIcon (HICON hIcon) {
	if (!hIcon)
		return FALSE ;
	m_nid.uFlags = NIF_ICON ;
	m_nid.hIcon  = hIcon ;
	return Shell_NotifyIcon (NIM_MODIFY, &m_nid) ;
}
inline BOOL  FCTrayIcon::ChangeIcon (UINT ID_Icon) {
	return this->ChangeIcon (LoadIcon ((HINSTANCE)GetModuleHandle (NULL), MAKEINTRESOURCE(ID_Icon))) ;
}
inline BOOL FCTrayIcon::ChangeTip (PCTSTR szTip) {
	if (!szTip)
		return FALSE ;
	m_nid.uFlags = NIF_TIP ;
	lstrcpy (m_nid.szTip, szTip) ;
	return Shell_NotifyIcon (NIM_MODIFY, &m_nid) ;
}
inline void FCTrayIcon::PopupMenu (UINT uFlags) {
	if ( !m_hMenu )
		return ;
	POINT	pt ;
	::GetCursorPos (&pt) ;
	HMENU hMenu = GetSubMenu (m_hMenu, 0) ;
	::SetForegroundWindow (m_nid.hWnd) ;
	TrackPopupMenuEx (hMenu, uFlags | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
					  pt.x, pt.y, m_nid.hWnd, NULL) ;
}
inline BOOL  FCTrayIcon::Add (HWND hwnd, UINT ID_Icon, UINT uID, LPCTSTR lpszTip) {
	return this->Add (hwnd, LoadIcon ((HINSTANCE)GetModuleHandle (NULL), MAKEINTRESOURCE(ID_Icon)), uID, lpszTip) ;
}
inline BOOL FCTrayIcon::Delete () {
	return (::Shell_NotifyIcon (NIM_DELETE, &m_nid)) ;
}
inline void  FCTrayIcon::SetMenu (PCTSTR szMenuRes) {
	if (m_hMenu)
		::DestroyMenu (m_hMenu) ;
	m_hMenu = ::LoadMenu ((HINSTANCE)GetModuleHandle (NULL), szMenuRes) ;
}
inline void  FCTrayIcon::SetMenu (UINT nIDMenu) {
	this->SetMenu (MAKEINTRESOURCE(nIDMenu)) ;
}
inline HMENU FCTrayIcon::GetMenu () {
	return m_hMenu ;
}
inline UINT  FCTrayIcon::GetCtrlID () {
	return m_nid.uID ;
}

#endif