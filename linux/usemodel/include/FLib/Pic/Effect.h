//////////////////////////////////////////////////////////////////
//																//
//		用途 : 32 位 DIB 特效处理								//
//		创建 : [Foolish] / 2001-4-14							//
//		更新 : 2002-11-6										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_DIBEFFECT_H__
#define		__FOO_DIBEFFECT_H__
#include "Pic.h"
#include <list>
#pragma once
using namespace std ;

//====================================================================
//	DIB效果
//====================================================================
/************************************************************************/
/*	1 . 所有函数如非特别指明则只对 32 位色有效, 必须先ConvertTo32Bit.	*/
/*	2 . 对于rect, 右和下边界与图形学里一样为开区间 (即不包含)			*/
/*		如选全图为: (0, 0, 800, 600) 而不是 (0, 0, 799, 599)			*/
/*	3 . 打开一幅新图像前要调用->ClearAllList () ;						*/
/*	4 . 如果注明 : (支持 AUTO-Undo/Redo)								*/
/*		则当m_UndoFlag == true时, 函数自动保存当前DIB					*/
/************************************************************************/
class FCDibEffect : public FCDib
{
public :
	FCDibEffect (bool bUndo = false) ;
	FCDibEffect (FCDibEffect & dib) ;
	virtual ~FCDibEffect () ;
	void	AddToUndoList () ;	// 把(*this即当前DIB)加入Undo List
	void	ClearAllList () ; // 清空Undo/Redo队列
	void	operator= (const FCDibEffect & pic) ;
	void	Undo () ;	// 撤销一步操作
	void	Redo () ;	// 重作一步操作
	bool	IsUndoEnable () ;
	bool	IsRedoEnable () ;
	BOOL	GetUndoFlag () ;
	void	SetUndoEnable (bool bEnable = false) ; // 自动Undo/Redo

public :
	// Region selection support
	void	SetRectRgn (int nLeft, int nTop, int nRight, int nBottom) ;
	void	SetRoundRectRgn (int nLeft, int nTop, int nRight, int nBottom, int nWidthEllipse, int nHeightEllipse) ;
	void	SetEllipticRgn (int nLeft, int nTop, int nRight, int nBottom) ;
	void	SetPolygonRgn (POINT * ppt, int cNumPoint, int nFillMode = ALTERNATE) ;
	bool	HasSelection () ;
	bool	IsInSelection (int x, int y) ;
	void	GetBoundBox (RECT * rect) ;
	void	ClearSelection () ;

	//	平移变换, 用边界色填充空白区域 (AUTO-Undo/Redo)
	void	operator << (int iWidth) ;
	void	operator >> (int iWidth) ;
	void	ShiftUp (int iHeight) ;
	void	ShiftDown (int iHeight) ;
	void	ColShiftUp (int iCol, int iHeight) ;	// 列上移
	void	ColShiftDown (int iCol, int iHeight) ;	// 列下移

	//	特效处理 (支持 AUTO-Undo/Redo)
		// 加/减边框 (用黑色填充边框)(全图)
	void	AddFrame (int iLeft, int iTop, int iRight, int iBottom) ;
	void	EraseFrame (int iLeft, int iTop, int iRight, int iBottom) ;
	void	DuplicateFrame (int iLeft, int iTop, int iRight, int iBottom) ;
		// 上下翻转
	void	Flip () ;
		// 左右镜像
	void	Mirror () ;
		// 顺时针旋转 90' (全图)
	void	Rotate90 () ;
		// 顺时针旋转 270' (全图)
	void	Rotate270 () ;
		// 顺时针旋转给定角度 (全图) iAngle在[1..89]
	void	Rotate (int iAngle) ;
		// iType == TRUE 为宽高成比例缩放 (全图)
	void	Stretch (DWORD dwWidth, DWORD dwHeight, int iType = TRUE) ;
		//	调节RGB值
	void	AdjustRGB (int iValueR, int iValueG, int iValueB) ;
		//	亮度	bFlag == TRUE, 则iValue为按增量
		//			bFlag == FALSE, 则iValue为按百分比, 如150 (means 150%)
	void	AdjustBrightness (int iValue, BOOL bFlag = TRUE) ;
	void	AdjustBrightnessPixel (int x, int y, int iValue, BOOL bFlag = TRUE) ;
		//	对比度 (iPercent为按百分比的增量)
	void	AdjustContrast (int iPercent) ;
	void	AdjustContrastPixel (int x, int y, int iPercent) ;
		//	色调
	void	AdjustHue (int iPercent) ;
		//	饱和度
	void	AdjustSaturation (int iPercent) ;
		//	色调 && 饱和度
	void	AdjustHS (int iPercentHue, int iPercentSat) ;
		//	颜色反转
	void	Negate () ;
		//	浮雕效果
	void	Emboss (int nStep = 2, int nDirection = DIRECT_RIGHT) ;
		//  风化效果
		//	nStep 为移动象素 : [1..x]
		//	nDirection 为风化方向
	void	Windy (int nStep, int nDirection = DIRECT_RIGHT, bool bSmooth = true) ;
	void	WindySoftLine (int nLine, int nStep, int nDirection = DIRECT_RIGHT) ;
		//	模糊效果, 使用平均算法
		//	iBlockLen为小方块边长, 在[2..32]
	void	SmoothAverage (int iBlockLen) ;
	void	SmoothAverageNoEdge (int iBlockLen) ;
		//	锐化效果
	void	Sharp (int nStep = 1) ;
		//	马赛克效果
		//	iBlockLen为小方块边长 ( >=2 )
	void	Mosaic (int iBlockLen) ;
		//	玻璃效果 (飞溅)
		//	iBlockLen为小方块边长, 在[2..100]
	void	Glass (int iBlockLen) ;
		//	球化效果
	void	Sphere () ;
		//	柱面效果
	void	Column () ;
		//	锥
	void	ConeSunken (int iHeight) ;
		//	带状效果 (iSwing : 振幅[0..100]		iFrequency : 频率)
	void	Stripe (int iSwing, int iFrequency) ;
		//	卷积运算, iElement从矩阵左上角开始, iLength为矩阵宽度
	void	ConvoluteDIB (int * iElement, int iLength, int iDivisor) ;
	void	ConvoluteDIBNoEdge (int * iElement, int iLength, int iDivisor) ;
		//	基于卷积的特效算法
	void	Algorithm (int iAlgorithm) ; // iAlgorithm见StdDefine.h

protected :
	void	ClearRedoList () ;
	void	ClearUndoList () ;
	void	__CopyCol (FCDib & dibDest, int iColD, const FCDib & dibSrc, int iCol) ; // 列copy
	void	__BltBlock (const FCDib & Dib, int x, int y) ; // Cover block on current DIB, only the region specified by m_hSelRgn
	void	__CopyPalette (const FCDib & DibSrc) ;
	void	__SetPixelData (BYTE * pDest, int x, BYTE * pSrc) ;
protected :
	BOOL			m_UndoFlag ;	// 是否支持Undo/Redo
	RECT			m_SelRect ;		// 区域的外围矩形
	HRGN			m_hSelRgn ;		// 选择区域
	list<FCDib *>	m_UndoList, m_RedoList ;	// Undo/Redo队列 "注意 : 如果把FCDib *改为FCDibEffect *, 要修改operate = "
} ;

//===================================================================
//	Implement
//===================================================================
inline void  FCDibEffect::SetUndoEnable (bool bEnable) {
	m_UndoFlag = bEnable ;
}
inline bool  FCDibEffect::IsRedoEnable () {
	return !m_RedoList.empty () ;
}
inline bool  FCDibEffect::IsUndoEnable () {
	return !m_UndoList.empty () ;
}
inline BOOL  FCDibEffect::GetUndoFlag () {
	return m_UndoFlag ;
}
inline void  FCDibEffect::AdjustBrightnessPixel (int x, int y, int iPercent, BOOL bFlag) {
	RGBQUAD		* pPixel = (RGBQUAD *) this->GetBits (x, y) ;
	if (!bFlag)
		iPercent = 128 * iPercent / 100 ; // 转换成x/128
	pPixel->rgbBlue = max (min (bFlag ? (pPixel->rgbBlue + iPercent)
									  : (pPixel->rgbBlue * iPercent / 128), 255), 0) ;
	pPixel->rgbGreen = max (min (bFlag ? (pPixel->rgbGreen + iPercent)
									   : (pPixel->rgbGreen * iPercent / 128), 255), 0) ;
	pPixel->rgbRed = max (min (bFlag ? (pPixel->rgbRed + iPercent)
									 : (pPixel->rgbRed * iPercent / 128), 255), 0) ;
}
inline void  FCDibEffect::AdjustContrastPixel (int x, int y, int iPercent) {
	RGBQUAD		* pPixel = (RGBQUAD *)this->GetBits (x, y) ;
	iPercent = 128 * iPercent / 100 ; // 转换成x/128
	pPixel->rgbBlue = max (min (128 + (pPixel->rgbBlue - 128) * iPercent / 128, 255), 0) ;
	pPixel->rgbGreen = max (min (128 + (pPixel->rgbGreen - 128) * iPercent / 128, 255), 0) ;
	pPixel->rgbRed   = max (min (128 + (pPixel->rgbRed - 128) * iPercent / 128, 255), 0) ;
}
inline void  FCDibEffect::AdjustHue (int iPercent) {
	this->AdjustHS (iPercent, 100) ;
}
inline void  FCDibEffect::AdjustSaturation (int iPercent) {
	this->AdjustHS (100, iPercent) ;
}
inline void  FCDibEffect::__CopyCol (FCDib & dibDt, int iColD, const FCDib & dibSr, int iCol) {
	BYTE		* pDest = dibDt.GetBits (iColD, 0),
				* pSrc = dibSr.GetBits (iCol, 0) ;
	int			nSpan = dibDt.ColorBits() / 8 ; // 每象素字节数2, 3, 4
	DWORD		dwOldPitch = dibSr.GetPitch (),
				dwPitch = dibDt.GetPitch () ;
	for (int y=0 ; y < dibDt.Height() ; y++, pDest -= dwPitch, pSrc -= dwOldPitch)
		CopyMemory (pDest, pSrc, nSpan) ;
}
inline void  FCDibEffect::__CopyPalette (const FCDib & DibSrc) {
	if (this->ColorBits () <= 8) {
		RGBQUAD		pPalette[0x100] ;
		DibSrc.GetColorTable (0, 1 << ColorBits(), pPalette) ;
		this->SetColorTable (0, 1 << ColorBits(), pPalette) ;
	}
}
__forceinline void  FCDibEffect::__SetPixelData (BYTE * pDest, int x, BYTE * pSrc) {
	switch (this->ColorBits())
	{
		case 8  :	*(pDest + x) = *pSrc ; break ;
		case 16 :	CopyMemory ((WORD *)pDest + x, pSrc, 2) ; break ;
		case 24 :	CopyMemory ((RGBTRIPLE *)pDest + x, pSrc, 3) ; break ;
		case 32 :	CopyMemory ((DWORD *)pDest + x, pSrc, 4) ; break ;
	}
}
inline void  FCDibEffect::ColShiftUp (int iCol, int iHeight) {
	if ((this->ColorBits() >= 8) && (iCol > 0) && (iCol < this->Width()) && (iHeight > 0) && (iHeight < this->Height()))
	{
		BYTE		* pSrc = this->GetBits (iCol, iHeight),
					* pDest = this->GetBits (iCol, 0) ;
		int			iMax = Height() - iHeight,
					nSpan = this->ColorBits() / 8 ;
		DWORD		dwPitch = this->GetPitch () ;
		for (int i=0 ; i < iMax ; i++, pSrc -= dwPitch, pDest -= dwPitch)
			CopyMemory (pDest, pSrc, nSpan) ;
		for (int m=0 ; m < iHeight ; m++, pDest -= dwPitch)
			FillMemory (pDest, nSpan, 0xFF) ; // 填充白色
	}
}
inline void  FCDibEffect::ColShiftDown (int iCol, int iHeight) {
	if ((this->ColorBits() >= 8) && (iCol > 0) && (iCol < this->Width()) && (iHeight > 0) && (iHeight < this->Height()))
	{
		BYTE		* pSrc = this->GetBits (iCol, Height() - 1 - iHeight),
					* pDest = this->GetBits (iCol, Height() - 1) ;
		int			iMax = Height() - iHeight,
					nSpan = this->ColorBits() / 8 ;
		DWORD		dwPitch = this->GetPitch () ;
		for (int i=0 ; i < iMax ; i++, pSrc += dwPitch, pDest += dwPitch)
			CopyMemory (pDest, pSrc, nSpan) ;
		for (int m=0 ; m < iHeight ; m++, pDest += dwPitch)
			FillMemory (pDest, nSpan, 0xFF) ; // 填充白色
	}
}
inline void  FCDibEffect::SetEllipticRgn (int nLeft, int nTop, int nRight, int nBottom) {
	this->ClearSelection () ;
	m_hSelRgn = ::CreateEllipticRgn (nLeft, nTop, nRight+1, nBottom+1) ;
	::GetRgnBox (m_hSelRgn, &m_SelRect) ;
}
inline void  FCDibEffect::SetRoundRectRgn (int nLeft, int nTop, int nRight, int nBottom, int nWidthEllipse, int nHeightEllipse) {
	this->ClearSelection () ;
	m_hSelRgn = ::CreateRoundRectRgn (nLeft, nTop, nRight+1, nBottom+1, nWidthEllipse, nHeightEllipse) ;
	::GetRgnBox (m_hSelRgn, &m_SelRect) ;
}
inline void  FCDibEffect::SetRectRgn (int nLeft, int nTop, int nRight, int nBottom) {
	this->ClearSelection () ;
	m_hSelRgn = ::CreateRectRgn (nLeft, nTop, nRight, nBottom) ;
	::GetRgnBox (m_hSelRgn, &m_SelRect) ;
}
inline void  FCDibEffect::SetPolygonRgn (POINT * ppt, int cNumPoint, int nFillMode) {
	if (ppt != NULL) {
		this->ClearSelection () ;
		m_hSelRgn = ::CreatePolygonRgn (ppt, cNumPoint, nFillMode) ;
		::GetRgnBox (m_hSelRgn, &m_SelRect) ;
	}
}
inline void  FCDibEffect::GetBoundBox (RECT * rect) {
	if (rect != NULL)
		::CopyRect (rect, &m_SelRect) ;
}
inline bool  FCDibEffect::HasSelection () {
	return (m_hSelRgn != NULL) ;
}
inline bool  FCDibEffect::IsInSelection (int x, int y) {
	return (::PtInRegion (m_hSelRgn, x, y) != 0) ;
}
inline void  FCDibEffect::ClearSelection () {
	if (HasSelection ())
		::DeleteObject (m_hSelRgn) ;
	m_hSelRgn = NULL ;
	::SetRectEmpty (&m_SelRect) ;
}

#endif