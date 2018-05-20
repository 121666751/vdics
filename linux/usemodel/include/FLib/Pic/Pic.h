//////////////////////////////////////////////////////////////////
//																//
//		��; : DIB ������										//
//		���� : [Foolish] / 2001-2-13							//
//		���� : 2002-10-31										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_32DIB_H__
#define		__FOO_32DIB_H__
#include <windows.h>
#include "..\StdDefine.h"
#include "ImageFormat.h"
#pragma once

//===================================================================
//	DIB����
//===================================================================
/********************************************************************/
/*	1 . DIB���ش����½Ǵ洢, ���ڴ��д洢��ʽΪ B-G-R-a				*/
/*	2 . �����е�����ԭ��(0,0)ȡ���Ͻ�								*/
/********************************************************************/
class FCDib
{
public :
	FCDib () ;
	FCDib (const FCDib & dib) ;
	virtual		~FCDib () ;
	void  Unload () ; // �����ǰλͼ
	FCDib & operator= (const FCDib & dib) ;
	void	RegionFill (RGBQUAD rgb, RECT * rect = NULL) ;
	bool	BoundRect (RECT & rect) ;
	BOOL	CaptureScreen (LPRECT lpRect = NULL) ;	// NULL Ϊ��׽ȫ��

	/****************************************************************/
	/*	��  �� �����ڴ��д���һ���յ�DIB							*/
	/*	��  �� ��0Ϊ��ǰ���������									*/
	/*	˵  �� ���ڲ�����->Unload (), �����ǰ��DIB					*/
	/*			 8Bit����λͼ�����յ�ɫ��							*/
	/*			 ��֧�����϶��´洢��DIB (��iHeight < 0)			*/
	/****************************************************************/
	BOOL	Create (int iWidth, int iHeight, WORD wColorBit = 0) ; 

	/****************************************************************/
	/*	��  �� ��DDB ==> DIB										*/
	/*	˵  �� ��hBitmapΪDDB Handle�����ú����ɾ��hBitmap			*/
	/****************************************************************/
	BOOL	AttachDDB (HBITMAP hBitmap, int iColor = 32) ;

	/****************************************************************/
	/*	��  �� ��DIB ==> DDB										*/
	/*	˵  �� ����������Ļ���ݵ�HBITMAP, �����ֶ�ɾ�����صĶ���	*/
	/*			 95/98/Me : ���ܴ�������16M��DDB					*/
	/****************************************************************/
	HBITMAP	GetDDB_Handle (HDC hdc = NULL) ;

public :
	//	��ȡͼ����Ϣ (Attributes)
	BYTE  * GetBits (int iLine = 0) const ;	// ȡ�õ� iLine ��ָ��, ���Ͻ�Ϊ(0,0), ���϶���
	BYTE  * GetBits (int x, int y) const ;	// ȡ�� (x,y) ���ָ��, ���Ͻ�Ϊ(0,0), ���϶��£��������
	BYTE  * GetMemStart () const ;			// ���DIB���½�����ָ��(Ҳ����ʼ��ַ)
	DWORD	Width () const ;				// ��
	DWORD	Height () const ;				// ��
	WORD	ColorBits () const ;			// ��ɫλ��
	DWORD	GetPitch () const ;				// �п��ֽ��� (32λ����)
	HBITMAP	GetHandle () const ;			// Get DIBSECTION handle (DIB-Handle)
	int		ColorsUsed () const ;			// ͳ��ͼ��ʹ�õ���ɫ��
	BOOL	TakeKeyColor (RGBQUAD * rgb) ;	// �ҳ�һ�ֲ���ͼ���г��ֵ���ɫ (>=16Bit)
	void	GetDibInfo (BITMAPINFOHEADER * bmif) const ;
	float	GetResolutionX() { return m_ResolutionX; }; // zwjia
	float	GetResolutionY() { return m_ResolutionY; }; // zwjia

	//	��ɫת��
	void	ConvertToGray () ; // ת���ɻҶ�ͼ
	void	ConvertTo16Bit () ;	// 1, 4, 8, 24, 32 ==> 16
	void	ConvertTo24Bit () ;	// 1, 4, 8, 16, 32 ==> 24
	void	ConvertTo32Bit () ;	// 1, 4, 8, 16, 24 ==> 32
	void	ConvertColorTo (int iColor) ; // ==> iColor (��ɫʱЧ������)

	//	ֱ�����ش�ȡ (pixel operations)
	RGBQUAD	ParsePixelData (DWORD dwData) const ;
	DWORD	GetPixelData (int x, int y) const ;
	void	GetPixelColor (int x, int y, RGBQUAD * prgb) const ;
	void	GetPixelColor32 (int x, int y, RGBQUAD * prgb) const ; // 32 λ
	void	SetPixelData (int x, int y, DWORD dwPixel) ;
	void	SetPixelColor (int x, int y, RGBQUAD rgb) ;		// �� 16, 24, 32 λɫ��Ч
	void	SetPixelColor32 (int x, int y, RGBQUAD rgb) ;	// 32 λ
	void	NegatePixel (int x, int y) ;
	void	AlphaBlendPixel (int x, int y, RGBQUAD rgb) ; // ����rgb�е�alphaͨ������

	//	ͨ������ (24,32Bit��Ч)(��ͨ��λͼ��Ϊ8Bit)
	void	GetRGBChannel (FCDib * red, FCDib * green, FCDib * blue, FCDib * alpha = NULL) ;
	void	GetBlueChannel (FCDib * blue) ;
	void	GetGreenChannel (FCDib * green) ;
	void	GetRedChannel (FCDib * red) ;
	void	GetAlphaChannel (FCDib * alpha) ;
	void	CombineChannel (FCDib * red, FCDib * green, FCDib * blue, FCDib * alpha = NULL) ;
	void	AppendAlphaChannel (const FCDib & alpha) ;
	void	InvertAlphaChannel () ;

	//	��ɫ����� (palette operations) ( 8 λɫ������Ч ), index��0��ʼ
	bool	IsGrayPalette () const ;
	void	SetGrayPalette () ;
	BOOL	GetColorTable (int index, RGBQUAD * prgb) const ;
	BOOL	SetColorTable (int index, RGBQUAD * prgb) ;
	BOOL	GetColorTable (int iFirstIndex, int iNumber, RGBQUAD * pColors) const ;
	BOOL	SetColorTable (int iFirstIndex, int iNumber, RGBQUAD * pColors) ;

	//	����DIB���д���
	BOOL	GetSubBlock (FCDib * SubDib, const RECT & rcBlock) const ; // SubDib����Ϊthis
	BOOL	CoverBlock (const FCDib & Dib, int x, int y) ;
	void	AlphaBlend (const FCDib & MaskDib, int alpha, int x, int y) ; // alphaΪMaskDibλͼ��Ȩֵ [0..0xFF]
	void	AlphaBlend (const FCDib & MaskDib32, int x=0, int y=0) ; // ����MaskDib32�е�alphaͨ������
	void	LogicalOperate (const FCDib & Dib, DWORD dwRop, int x=0, int y=0) ; // dwRop��StdDefine.h
	void	AND (const FCDib & Dib, int x=0, int y=0) ;
	void	 OR (const FCDib & Dib, int x=0, int y=0) ;
	void	XOR (const FCDib & Dib, int x=0, int y=0) ;

	//	������
	BOOL	CopyToClipboard (const RECT * rect = NULL) ;
	BOOL	PasteFromClipboard (const RECT * rect = NULL) ;
	BOOL	IsPasteAvailable () ;

	//	����� (��̬)
	static RGBQUAD	fooSplit16Bit_565 (WORD wPixel) ;
	static RGBQUAD	fooSplit16Bit_555 (WORD wPixel) ;
	static WORD		fooCombine16Bit_565 (const RGBQUAD * rgb) ;
	static WORD		fooCombine16Bit_555 (const RGBQUAD * rgb) ;
	static DWORD	fooDibRowBytes (int iWidth, int iColorBit) ;
	static BYTE		fooGetGrayscale (RGBQUAD rgb) ;
	static void		fooSwapRGB (BYTE * prgb) ;
	static void		fooRGBtoHLS (RGBTRIPLE rgb, double * H, double * L, double * S) ;
	static RGBTRIPLE fooHLStoRGB (const double & H, const double & L, const double & S) ;

protected :
	FOODIB	* pFooDib ;

	float	m_ResolutionX;
	float	m_ResolutionY;

protected :
	void	__ConvertToTrueColor (int iColor) ; // ת��Ϊ���ɫ��iColorΪ24 or 32
	static	double  __fooHuetoRGB (double m1, double m2, double h) ;
} ;

//===================================================================
//	Implement
//===================================================================
inline FCDib::FCDib () {
	pFooDib = new FOODIB ;
	pFooDib->hBitmap = NULL ;
	pFooDib->pByte   = NULL ;
	pFooDib->ppLine  = NULL ;
	::ZeroMemory (&pFooDib->DibInfo, sizeof(pFooDib->DibInfo)) ;
	m_ResolutionX = 0;
	m_ResolutionY = 0;
}
inline FCDib::FCDib (const FCDib & dib) {
	pFooDib = new FOODIB ;
	pFooDib->hBitmap = NULL ;
	pFooDib->pByte   = NULL ;
	pFooDib->ppLine  = NULL ;
	::ZeroMemory (&pFooDib->DibInfo, sizeof(pFooDib->DibInfo)) ;
	if (dib.GetHandle () != NULL)
		FCDib::operator = (dib) ;
}
inline FCDib::~FCDib () {
	this->Unload () ;
	delete pFooDib ;
}
inline BYTE * FCDib::GetBits (int iLine) const {
	return (this->GetHandle() == NULL) ? NULL : pFooDib->ppLine[iLine] ;
}
inline BYTE * FCDib::GetBits (int x, int y) const {
	return (this->GetHandle() == NULL) ? NULL : (pFooDib->ppLine[y] + x * this->ColorBits() / 8) ;
}
inline void FCDib::GetDibInfo (BITMAPINFOHEADER * bmif) const {
	if (bmif != NULL)
		::CopyMemory (bmif, &pFooDib->DibInfo, sizeof(pFooDib->DibInfo)) ;
}
inline DWORD FCDib::Width () const {
	return pFooDib->DibInfo.biWidth ;
}
inline DWORD FCDib::Height () const {
	return pFooDib->DibInfo.biHeight ;
}
inline WORD FCDib::ColorBits () const {
	return pFooDib->DibInfo.biBitCount ;
}
inline HBITMAP	FCDib::GetHandle () const {
	return pFooDib->hBitmap ;
}
inline 	DWORD  FCDib::GetPitch ( ) const {
	return ( 4 * ((this->Width () * (DWORD)this->ColorBits () + 31) / 32) ) ;
}
inline void	 FCDib::GetPixelColor32 (int x, int y, RGBQUAD * prgb) const {
	if ((prgb != NULL) && (this->GetHandle() != NULL))
		* prgb = * (RGBQUAD *)(pFooDib->ppLine[y] + x * 4) ;
}
inline void	 FCDib::SetPixelColor32 (int x, int y, RGBQUAD rgb) {
	if (this->GetHandle() != NULL)
		* (RGBQUAD *)(pFooDib->ppLine[y] + x * 4) = rgb ; 
}
inline BOOL  FCDib::GetColorTable (int index, RGBQUAD * prgb) const {
	return this->GetColorTable (index, 1, prgb) ;
}
inline BOOL  FCDib::SetColorTable (int index, RGBQUAD * prgb) {
	return this->SetColorTable (index, 1, prgb) ;
}
inline RGBQUAD  FCDib::fooSplit16Bit_565 (WORD wPixel) {
	RGBQUAD		rgb ;
	rgb.rgbRed   = (BYTE) (((MASK16_RED_565 & wPixel) >> 11)  << 3) ;
	rgb.rgbGreen = (BYTE) (((MASK16_GREEN_565 & wPixel) >> 5) << 3) ;
	rgb.rgbBlue  = (BYTE) ((MASK16_BLUE_565 & wPixel) << 3) ;
	return rgb ;
}
inline RGBQUAD  FCDib::fooSplit16Bit_555 (WORD wPixel) {
	RGBQUAD		rgb ;
	rgb.rgbRed   = (BYTE) (((MASK16_RED_555 & wPixel) >> 10)  << 3) ;
	rgb.rgbGreen = (BYTE) (((MASK16_GREEN_555 & wPixel) >> 5) << 3) ;
	rgb.rgbBlue  = (BYTE) ((MASK16_BLUE_555 & wPixel) << 3) ;
	return rgb ;
}
inline WORD  FCDib::fooCombine16Bit_565 (const RGBQUAD * rgb) {
	WORD		wPixel ;
	wPixel  = ((rgb->rgbRed >> 3) << 11) ;
	wPixel |= ((rgb->rgbGreen >> 3) << 5) ;
	wPixel |= (rgb->rgbBlue >> 3) ;
	return wPixel ;
}
inline WORD  FCDib::fooCombine16Bit_555 (const RGBQUAD * rgb) {
	WORD		wPixel ;
	wPixel  = ((rgb->rgbRed >> 3) << 10) ;
	wPixel |= ((rgb->rgbGreen >> 3) << 5) ;
	wPixel |= (rgb->rgbBlue >> 3) ;
	return wPixel ;
}
inline DWORD  FCDib::fooDibRowBytes (int iWidth, int iColorBit) {
	return ( 4 * ((iWidth * iColorBit + 31) / 32) ) ;
}
inline BYTE  FCDib::fooGetGrayscale (RGBQUAD rgb) {
	register UINT32		tol = 30*rgb.rgbRed + 59*rgb.rgbGreen + 11*rgb.rgbBlue ;
	return max (min (tol / 100, 0xFF), 0) ;
}
inline void  FCDib::fooSwapRGB (BYTE * prgb) {
	if (prgb != NULL) {
		BYTE	temp = prgb[0] ;
		prgb[0] = prgb[2] ;
		prgb[2] = temp ;
	}
}
inline BYTE  * FCDib::GetMemStart () const {
	return (this->GetHandle() == NULL) ? NULL : pFooDib->pByte ;
}
inline void  FCDib::GetPixelColor (int x, int y, RGBQUAD * prgb) const {
	if (prgb != NULL)
		* prgb = this->ParsePixelData (this->GetPixelData (x, y)) ;
}
inline void  FCDib::ConvertTo24Bit () {
	this->__ConvertToTrueColor (24) ;
}
inline void  FCDib::ConvertTo32Bit () {
	this->__ConvertToTrueColor (32) ;
}
inline void  FCDib::GetRedChannel (FCDib * red) {
	this->GetRGBChannel (red, NULL, NULL, NULL) ;
}
inline void  FCDib::GetGreenChannel (FCDib * green) {
	this->GetRGBChannel (NULL, green, NULL, NULL) ;
}
inline void  FCDib::GetBlueChannel (FCDib * blue) {
	this->GetRGBChannel (NULL, NULL, blue, NULL) ;
}
inline void  FCDib::GetAlphaChannel (FCDib * alpha) {
	this->GetRGBChannel (NULL, NULL, NULL, alpha) ;
}
inline void  FCDib::NegatePixel (int x, int y) {
	RGBQUAD		rgb ;
	this->GetPixelColor (x, y, &rgb) ;
	rgb.rgbBlue = ~rgb.rgbBlue ;
	rgb.rgbGreen = ~rgb.rgbGreen ;
	rgb.rgbRed = ~rgb.rgbRed ;
	this->SetPixelColor (x, y, rgb) ;
}
inline void  FCDib::AlphaBlendPixel (int x, int y, RGBQUAD rgb) {
	RGBQUAD		pix ;
	this->GetPixelColor (x, y, &pix) ;
	pix.rgbBlue = max (0, min (0xFF, ((rgb.rgbBlue - pix.rgbBlue) * rgb.rgbReserved) / 0xFF + pix.rgbBlue)) ;
	pix.rgbGreen = max (0, min (0xFF, ((rgb.rgbGreen - pix.rgbGreen) * rgb.rgbReserved) / 0xFF + pix.rgbGreen)) ;
	pix.rgbRed = max (0, min (0xFF, ((rgb.rgbRed - pix.rgbRed) * rgb.rgbReserved) / 0xFF + pix.rgbRed)) ;
	this->SetPixelColor (x, y, pix) ;
}
inline void  FCDib::AND (const FCDib & Dib, int x, int y) {
	this->LogicalOperate (Dib, LOG_OPERATION_AND, x, y) ;
}
inline void  FCDib::OR (const FCDib & Dib, int x, int y) {
	this->LogicalOperate (Dib, LOG_OPERATION_OR, x, y) ;
}
inline void  FCDib::XOR (const FCDib & Dib, int x, int y) {
	this->LogicalOperate (Dib, LOG_OPERATION_XOR, x, y) ;
}
inline bool  FCDib::BoundRect (RECT & rect) {
	if (this->GetHandle () == NULL)
		return false ;
	RECT	img = {0, 0, this->Width(), this->Height()}, src ;
	::CopyRect (&src, &rect) ;
	if (::IntersectRect (&rect, &img, &src) == 0)
	{
		::SetRectEmpty (&rect) ;
		return false ; // û���ཻ����
	}
	return true ;
}

#endif