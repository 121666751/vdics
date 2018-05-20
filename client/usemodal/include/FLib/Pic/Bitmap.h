//////////////////////////////////////////////////////////////////
//																//
//		用途 : Bitmap ( DDB ) 操作								//
//		创建 : [Foolish] / 2001-1-18							//
//		更新 : 2002-11-6										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef	 __FOO_BITMAP_H__
#define	 __FOO_BITMAP_H__
#include <windows.h>
#include "..\StdDefine.h"
#pragma once

//	此函数返回 FALSE 则中断 ->DrawEx(...) 的操作 (在多线程中使用)
//extern BOOL fooIsShowDibEffectContinue() ;
static BOOL fooIsShowDibEffectContinue(){
	return TRUE ;
}
//===================================================================
//	DDB 操作类
//===================================================================
class FCBitmap
{
public :
	FCBitmap () ;
	virtual ~FCBitmap () ;

	//	获取一DDB
	BOOL	CreateCompatibleBitmap (int iWidth, int iHeight) ;
	void	Attach (HBITMAP	hBitmap) ; // Attach a DDB, 不删除当前位图
	BOOL	LoadBitmap (PCTSTR szResBitmap) ;
	BOOL	LoadBitmap (UINT uIDBitmap) ;
	BOOL	LoadBitmapFromFile (PCTSTR szFileName) ;
	BOOL	LoadOEMBitmap (UINT uIDBitmap) ; // 在WinUser.h中定义 OBM_*
	HBITMAP	Detach () ;	// 和Attach成对调用, 不删除m_hBitmap, 只把它置NULL。并返回当前位图句柄。
	void	Clear () ; // 删除m_hBitmap

	//	获取DDB信息
	HBITMAP	GetHandle () ;
	void	GetBitmapInfo (BITMAP * pBitmap) ;
	int		GetWidth () ;
	int		GetHeight () ;
	int		GetColorBit () ;

	// 缩放系数 (m_nMin to -1) & (1 to m_nMax)
	void	SetZoomScale (int iScale) ;
	int		GetZoomScale () ;
	void	IncreaseZoomScale () ;
	void	DecreaseZoomScale () ;
	int		GetScaledWidth () ;
	int		GetScaledHeight () ;
	void	MapScaledPoint (POINT & pt) ;
	void	MapScaledRect (RECT & rect) ;
	void	MapRealPoint (POINT & pt) ;
	void	MapRealRect (RECT & rect) ;

	//	Misc
	void	SetCpuSpeed (int iFreq) ; // 设置CPU主频, DrawEx中用
	void	FillRect (HBRUSH hBrush, RECT * rcFill = NULL) ;

	//	alpha 为当前位图的权值 [0..100]
	void	AlphaBlendDraw (HDC hdc, int alpha, int x = 0, int y = 0) ;
	void	Draw (HDC hdc, int xDest = 0, int yDest = 0,
						   int xSrc = 0, int ySrc = 0) ;
	void	DrawEx (HDC hdc, SHOWEFFECT ShowParam) ; // 详见StdDefine.h

protected :
	HBITMAP		m_hBitmap ;
	int			m_iScale ;
	int			m_iFreq ;
	int			m_nMax ; // limit of zoom out == 16
	int			m_nMin ; // limit of zoom in == -16
protected :
	void	ShowDib_SCAN (HDC hdc, HDC hDCmem, const BITMAP & bmif, const SHOWEFFECT & Param) ;
	void	ShowDib_MOVE (HDC hdc, HDC hDCmem, const BITMAP & bmif, const SHOWEFFECT & Param) ;
	void	ShowDib_MOSAIC (HDC hdc, HDC hDCmem, const BITMAP & bmif, const SHOWEFFECT & Param) ;
	void	ShowDib_FADE (HDC hdc, HDC hDCmem, const BITMAP & bmif, const SHOWEFFECT & Param) ;
	void	ShowDib_FADESCAN (HDC hdc, HDC hDCmem, BITMAP bmif, DWORD dwParam) ;
} ;

//===================================================================
//	Implement
//===================================================================
inline FCBitmap::FCBitmap () {
	m_hBitmap = NULL ;	m_iFreq = 233 ; m_iScale = 1 ;
	m_nMax = 16 ; m_nMin = -16 ;
}
inline FCBitmap::~FCBitmap () {
	this->Clear () ;
}
inline void  FCBitmap::SetCpuSpeed (int iFreq) {
	m_iFreq = iFreq ;
}
inline void  FCBitmap::FillRect (HBRUSH hBrush, RECT * rcFill) {
	RECT	rect ;
	BITMAP	bmif ;
	::GetObject (m_hBitmap, sizeof(BITMAP), &bmif) ;
	(rcFill == NULL) ? (::SetRect (&rect, 0, 0, bmif.bmWidth, bmif.bmHeight)) :
					   (::CopyRect (&rect, rcFill)) ;
	HDC		hDCmem = ::CreateCompatibleDC (NULL) ;
	HBITMAP	hOld = (HBITMAP)::SelectObject (hDCmem, m_hBitmap) ;
	::FillRect (hDCmem, &rect, hBrush) ;
	::SelectObject (hDCmem, hOld) ;
	::DeleteObject (hDCmem) ;
}
inline void FCBitmap::Clear () {
	if (m_hBitmap != NULL) {
		::DeleteObject (m_hBitmap) ;
		m_hBitmap = NULL ;
	}
}
inline HBITMAP  FCBitmap::GetHandle () {
	return m_hBitmap ;
}
inline void  FCBitmap::Attach (HBITMAP hBitmap) {
	m_hBitmap = hBitmap ;
}
inline HBITMAP  FCBitmap::Detach () {
	HBITMAP		hRet = m_hBitmap ;
	m_hBitmap = NULL ;
	return hRet ;
}
inline void  FCBitmap::GetBitmapInfo (BITMAP * pBitmap) {
	if (pBitmap != NULL)
		::GetObject (m_hBitmap, sizeof(BITMAP), pBitmap) ;
}
inline int  FCBitmap::GetWidth () {
	BITMAP	bmif ;
	return (::GetObject (m_hBitmap, sizeof(BITMAP), &bmif) == 0) ? 0 : bmif.bmWidth ;
}
inline int  FCBitmap::GetHeight () {
	BITMAP	bmif ;
	return (::GetObject (m_hBitmap, sizeof(BITMAP), &bmif) == 0) ? 0 : bmif.bmHeight ;
}
inline int  FCBitmap::GetColorBit () {
	BITMAP	bmif ;
	return (::GetObject (m_hBitmap, sizeof(BITMAP), &bmif) == 0) ? 0 : bmif.bmBitsPixel ;
}
inline BOOL  FCBitmap::CreateCompatibleBitmap (int iWidth, int iHeight) {
	this->Clear () ;
	HDC		dcScreen = ::CreateIC (TEXT("display"), NULL, NULL, NULL) ;
	m_hBitmap = ::CreateCompatibleBitmap (dcScreen, iWidth, iHeight) ;
	::ReleaseDC (NULL, dcScreen) ;
	return (m_hBitmap != NULL) ;
}
inline BOOL  FCBitmap::LoadBitmap (UINT uIDBitmap) {
	return this->LoadBitmap (MAKEINTRESOURCE(uIDBitmap)) ;
}
inline BOOL  FCBitmap::LoadBitmap (PCTSTR szResBitmap) {
	this->Clear () ;
	return (BOOL) (m_hBitmap = (HBITMAP)::LoadBitmap (::GetModuleHandle (NULL), szResBitmap)) ;
}
inline BOOL  FCBitmap::LoadBitmapFromFile (PCTSTR szFileName) {
	this->Clear () ;
	return (BOOL) (m_hBitmap = (HBITMAP)::LoadImage (NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) ;
}
inline BOOL  FCBitmap::LoadOEMBitmap (UINT uIDBitmap) {
	this->Clear () ;
	return (BOOL) (m_hBitmap = (HBITMAP)::LoadImage (NULL, MAKEINTRESOURCE (uIDBitmap), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) ;
}
inline void  FCBitmap::SetZoomScale (int iScale) {
	if (iScale >= 0)
		m_iScale = max (1, min (m_nMax, iScale)) ;
	else
		m_iScale = max (m_nMin, min (-1, iScale)) ;
}
inline int  FCBitmap::GetZoomScale () {
	return m_iScale ;
}
inline void  FCBitmap::IncreaseZoomScale () {
	if (m_iScale < m_nMax) {
		if (m_iScale == -1)
			m_iScale = 1 ;
		m_iScale++ ;
	}
}
inline void  FCBitmap::DecreaseZoomScale () {
	if (m_iScale > m_nMin) {
		if (m_iScale == 1)
			m_iScale = -1 ;
		m_iScale-- ;
	}
}
inline int  FCBitmap::GetScaledWidth () {
	int			nWidth ;
	BITMAP		bmif ;
	::GetObject (m_hBitmap, sizeof(BITMAP), &bmif) ;
	if (m_iScale > 0)
		nWidth = bmif.bmWidth * m_iScale ;
	else
		nWidth = bmif.bmWidth / (-m_iScale) ;
	return nWidth ;
}
inline int  FCBitmap::GetScaledHeight () {
	int			nHeight ;
	BITMAP		bmif ;
	::GetObject (m_hBitmap, sizeof(BITMAP), &bmif) ;
	if (m_iScale > 0)
		nHeight = bmif.bmHeight * m_iScale ;
	else
		nHeight = bmif.bmHeight / (-m_iScale) ;
	return nHeight ;
}

#endif
