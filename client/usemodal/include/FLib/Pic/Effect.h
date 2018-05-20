//////////////////////////////////////////////////////////////////
//																//
//		��; : 32 λ DIB ��Ч����								//
//		���� : [Foolish] / 2001-4-14							//
//		���� : 2002-11-6										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_DIBEFFECT_H__
#define		__FOO_DIBEFFECT_H__
#include "Pic.h"
#include <list>
#pragma once
using namespace std ;

//====================================================================
//	DIBЧ��
//====================================================================
/************************************************************************/
/*	1 . ���к�������ر�ָ����ֻ�� 32 λɫ��Ч, ������ConvertTo32Bit.	*/
/*	2 . ����rect, �Һ��±߽���ͼ��ѧ��һ��Ϊ������ (��������)			*/
/*		��ѡȫͼΪ: (0, 0, 800, 600) ������ (0, 0, 799, 599)			*/
/*	3 . ��һ����ͼ��ǰҪ����->ClearAllList () ;						*/
/*	4 . ���ע�� : (֧�� AUTO-Undo/Redo)								*/
/*		��m_UndoFlag == trueʱ, �����Զ����浱ǰDIB					*/
/************************************************************************/
class FCDibEffect : public FCDib
{
public :
	FCDibEffect (bool bUndo = false) ;
	FCDibEffect (FCDibEffect & dib) ;
	virtual ~FCDibEffect () ;
	void	AddToUndoList () ;	// ��(*this����ǰDIB)����Undo List
	void	ClearAllList () ; // ���Undo/Redo����
	void	operator= (const FCDibEffect & pic) ;
	void	Undo () ;	// ����һ������
	void	Redo () ;	// ����һ������
	bool	IsUndoEnable () ;
	bool	IsRedoEnable () ;
	BOOL	GetUndoFlag () ;
	void	SetUndoEnable (bool bEnable = false) ; // �Զ�Undo/Redo

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

	//	ƽ�Ʊ任, �ñ߽�ɫ���հ����� (AUTO-Undo/Redo)
	void	operator << (int iWidth) ;
	void	operator >> (int iWidth) ;
	void	ShiftUp (int iHeight) ;
	void	ShiftDown (int iHeight) ;
	void	ColShiftUp (int iCol, int iHeight) ;	// ������
	void	ColShiftDown (int iCol, int iHeight) ;	// ������

	//	��Ч���� (֧�� AUTO-Undo/Redo)
		// ��/���߿� (�ú�ɫ���߿�)(ȫͼ)
	void	AddFrame (int iLeft, int iTop, int iRight, int iBottom) ;
	void	EraseFrame (int iLeft, int iTop, int iRight, int iBottom) ;
	void	DuplicateFrame (int iLeft, int iTop, int iRight, int iBottom) ;
		// ���·�ת
	void	Flip () ;
		// ���Ҿ���
	void	Mirror () ;
		// ˳ʱ����ת 90' (ȫͼ)
	void	Rotate90 () ;
		// ˳ʱ����ת 270' (ȫͼ)
	void	Rotate270 () ;
		// ˳ʱ����ת�����Ƕ� (ȫͼ) iAngle��[1..89]
	void	Rotate (int iAngle) ;
		// iType == TRUE Ϊ��߳ɱ������� (ȫͼ)
	void	Stretch (DWORD dwWidth, DWORD dwHeight, int iType = TRUE) ;
		//	����RGBֵ
	void	AdjustRGB (int iValueR, int iValueG, int iValueB) ;
		//	����	bFlag == TRUE, ��iValueΪ������
		//			bFlag == FALSE, ��iValueΪ���ٷֱ�, ��150 (means 150%)
	void	AdjustBrightness (int iValue, BOOL bFlag = TRUE) ;
	void	AdjustBrightnessPixel (int x, int y, int iValue, BOOL bFlag = TRUE) ;
		//	�Աȶ� (iPercentΪ���ٷֱȵ�����)
	void	AdjustContrast (int iPercent) ;
	void	AdjustContrastPixel (int x, int y, int iPercent) ;
		//	ɫ��
	void	AdjustHue (int iPercent) ;
		//	���Ͷ�
	void	AdjustSaturation (int iPercent) ;
		//	ɫ�� && ���Ͷ�
	void	AdjustHS (int iPercentHue, int iPercentSat) ;
		//	��ɫ��ת
	void	Negate () ;
		//	����Ч��
	void	Emboss (int nStep = 2, int nDirection = DIRECT_RIGHT) ;
		//  �绯Ч��
		//	nStep Ϊ�ƶ����� : [1..x]
		//	nDirection Ϊ�绯����
	void	Windy (int nStep, int nDirection = DIRECT_RIGHT, bool bSmooth = true) ;
	void	WindySoftLine (int nLine, int nStep, int nDirection = DIRECT_RIGHT) ;
		//	ģ��Ч��, ʹ��ƽ���㷨
		//	iBlockLenΪС����߳�, ��[2..32]
	void	SmoothAverage (int iBlockLen) ;
	void	SmoothAverageNoEdge (int iBlockLen) ;
		//	��Ч��
	void	Sharp (int nStep = 1) ;
		//	������Ч��
		//	iBlockLenΪС����߳� ( >=2 )
	void	Mosaic (int iBlockLen) ;
		//	����Ч�� (�ɽ�)
		//	iBlockLenΪС����߳�, ��[2..100]
	void	Glass (int iBlockLen) ;
		//	��Ч��
	void	Sphere () ;
		//	����Ч��
	void	Column () ;
		//	׶
	void	ConeSunken (int iHeight) ;
		//	��״Ч�� (iSwing : ���[0..100]		iFrequency : Ƶ��)
	void	Stripe (int iSwing, int iFrequency) ;
		//	�������, iElement�Ӿ������Ͻǿ�ʼ, iLengthΪ������
	void	ConvoluteDIB (int * iElement, int iLength, int iDivisor) ;
	void	ConvoluteDIBNoEdge (int * iElement, int iLength, int iDivisor) ;
		//	���ھ������Ч�㷨
	void	Algorithm (int iAlgorithm) ; // iAlgorithm��StdDefine.h

protected :
	void	ClearRedoList () ;
	void	ClearUndoList () ;
	void	__CopyCol (FCDib & dibDest, int iColD, const FCDib & dibSrc, int iCol) ; // ��copy
	void	__BltBlock (const FCDib & Dib, int x, int y) ; // Cover block on current DIB, only the region specified by m_hSelRgn
	void	__CopyPalette (const FCDib & DibSrc) ;
	void	__SetPixelData (BYTE * pDest, int x, BYTE * pSrc) ;
protected :
	BOOL			m_UndoFlag ;	// �Ƿ�֧��Undo/Redo
	RECT			m_SelRect ;		// �������Χ����
	HRGN			m_hSelRgn ;		// ѡ������
	list<FCDib *>	m_UndoList, m_RedoList ;	// Undo/Redo���� "ע�� : �����FCDib *��ΪFCDibEffect *, Ҫ�޸�operate = "
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
		iPercent = 128 * iPercent / 100 ; // ת����x/128
	pPixel->rgbBlue = max (min (bFlag ? (pPixel->rgbBlue + iPercent)
									  : (pPixel->rgbBlue * iPercent / 128), 255), 0) ;
	pPixel->rgbGreen = max (min (bFlag ? (pPixel->rgbGreen + iPercent)
									   : (pPixel->rgbGreen * iPercent / 128), 255), 0) ;
	pPixel->rgbRed = max (min (bFlag ? (pPixel->rgbRed + iPercent)
									 : (pPixel->rgbRed * iPercent / 128), 255), 0) ;
}
inline void  FCDibEffect::AdjustContrastPixel (int x, int y, int iPercent) {
	RGBQUAD		* pPixel = (RGBQUAD *)this->GetBits (x, y) ;
	iPercent = 128 * iPercent / 100 ; // ת����x/128
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
	int			nSpan = dibDt.ColorBits() / 8 ; // ÿ�����ֽ���2, 3, 4
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
			FillMemory (pDest, nSpan, 0xFF) ; // ����ɫ
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
			FillMemory (pDest, nSpan, 0xFF) ; // ����ɫ
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