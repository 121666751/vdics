//////////////////////////////////////////////////////////////////
//																//
//		用途 : DIB 操作类										//
//		创建 : [Foolish] / 2001-2-13							//
//		更新 : 2002-10-31										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_32DIB_H__
#define		__FOO_32DIB_H__
#include <windows.h>
#include "..\StdDefine.h"
#include "ImageFormat.h"
#pragma once

//===================================================================
//	DIB基类
//===================================================================
/********************************************************************/
/*	1 . DIB像素从左下角存储, 在内存中存储格式为 B-G-R-a				*/
/*	2 . 本类中的坐标原点(0,0)取左上角								*/
/********************************************************************/
class FCDib
{
public :
	FCDib () ;
	FCDib (const FCDib & dib) ;
	virtual		~FCDib () ;
	void  Unload () ; // 清除当前位图
	FCDib & operator= (const FCDib & dib) ;
	void	RegionFill (RGBQUAD rgb, RECT * rect = NULL) ;
	bool	BoundRect (RECT & rect) ;
	BOOL	CaptureScreen (LPRECT lpRect = NULL) ;	// NULL 为捕捉全屏

	/****************************************************************/
	/*	功  能 ：在内存中创建一个空的DIB							*/
	/*	参  数 ：0为当前桌面的设置									*/
	/*	说  明 ：内部调用->Unload (), 清除当前的DIB					*/
	/*			 8Bit以下位图则建立空调色板							*/
	/*			 不支持自上而下存储的DIB (即iHeight < 0)			*/
	/****************************************************************/
	BOOL	Create (int iWidth, int iHeight, WORD wColorBit = 0) ; 

	/****************************************************************/
	/*	功  能 ：DDB ==> DIB										*/
	/*	说  明 ：hBitmap为DDB Handle，调用后可以删除hBitmap			*/
	/****************************************************************/
	BOOL	AttachDDB (HBITMAP hBitmap, int iColor = 32) ;

	/****************************************************************/
	/*	功  能 ：DIB ==> DDB										*/
	/*	说  明 ：创建与屏幕兼容的HBITMAP, 必须手动删除返回的对象	*/
	/*			 95/98/Me : 不能创建超过16M的DDB					*/
	/****************************************************************/
	HBITMAP	GetDDB_Handle (HDC hdc = NULL) ;

public :
	//	获取图像信息 (Attributes)
	BYTE  * GetBits (int iLine = 0) const ;	// 取得第 iLine 行指针, 左上角为(0,0), 自上而下
	BYTE  * GetBits (int x, int y) const ;	// 取得 (x,y) 点的指针, 左上角为(0,0), 自上而下，自左而右
	BYTE  * GetMemStart () const ;			// 获得DIB左下角象素指针(也即启始地址)
	DWORD	Width () const ;				// 宽
	DWORD	Height () const ;				// 高
	WORD	ColorBits () const ;			// 颜色位数
	DWORD	GetPitch () const ;				// 行宽字节数 (32位补齐)
	HBITMAP	GetHandle () const ;			// Get DIBSECTION handle (DIB-Handle)
	int		ColorsUsed () const ;			// 统计图像使用的颜色数
	BOOL	TakeKeyColor (RGBQUAD * rgb) ;	// 找出一种不在图象中出现的颜色 (>=16Bit)
	void	GetDibInfo (BITMAPINFOHEADER * bmif) const ;
	float	GetResolutionX() { return m_ResolutionX; }; // zwjia
	float	GetResolutionY() { return m_ResolutionY; }; // zwjia

	//	颜色转换
	void	ConvertToGray () ; // 转换成灰度图
	void	ConvertTo16Bit () ;	// 1, 4, 8, 24, 32 ==> 16
	void	ConvertTo24Bit () ;	// 1, 4, 8, 16, 32 ==> 24
	void	ConvertTo32Bit () ;	// 1, 4, 8, 16, 24 ==> 32
	void	ConvertColorTo (int iColor) ; // ==> iColor (减色时效果不好)

	//	直接像素存取 (pixel operations)
	RGBQUAD	ParsePixelData (DWORD dwData) const ;
	DWORD	GetPixelData (int x, int y) const ;
	void	GetPixelColor (int x, int y, RGBQUAD * prgb) const ;
	void	GetPixelColor32 (int x, int y, RGBQUAD * prgb) const ; // 32 位
	void	SetPixelData (int x, int y, DWORD dwPixel) ;
	void	SetPixelColor (int x, int y, RGBQUAD rgb) ;		// 对 16, 24, 32 位色有效
	void	SetPixelColor32 (int x, int y, RGBQUAD rgb) ;	// 32 位
	void	NegatePixel (int x, int y) ;
	void	AlphaBlendPixel (int x, int y, RGBQUAD rgb) ; // 根据rgb中的alpha通道计算

	//	通道处理 (24,32Bit有效)(各通道位图都为8Bit)
	void	GetRGBChannel (FCDib * red, FCDib * green, FCDib * blue, FCDib * alpha = NULL) ;
	void	GetBlueChannel (FCDib * blue) ;
	void	GetGreenChannel (FCDib * green) ;
	void	GetRedChannel (FCDib * red) ;
	void	GetAlphaChannel (FCDib * alpha) ;
	void	CombineChannel (FCDib * red, FCDib * green, FCDib * blue, FCDib * alpha = NULL) ;
	void	AppendAlphaChannel (const FCDib & alpha) ;
	void	InvertAlphaChannel () ;

	//	调色板操作 (palette operations) ( 8 位色以下有效 ), index从0开始
	bool	IsGrayPalette () const ;
	void	SetGrayPalette () ;
	BOOL	GetColorTable (int index, RGBQUAD * prgb) const ;
	BOOL	SetColorTable (int index, RGBQUAD * prgb) ;
	BOOL	GetColorTable (int iFirstIndex, int iNumber, RGBQUAD * pColors) const ;
	BOOL	SetColorTable (int iFirstIndex, int iNumber, RGBQUAD * pColors) ;

	//	两个DIB进行处理
	BOOL	GetSubBlock (FCDib * SubDib, const RECT & rcBlock) const ; // SubDib不能为this
	BOOL	CoverBlock (const FCDib & Dib, int x, int y) ;
	void	AlphaBlend (const FCDib & MaskDib, int alpha, int x, int y) ; // alpha为MaskDib位图的权值 [0..0xFF]
	void	AlphaBlend (const FCDib & MaskDib32, int x=0, int y=0) ; // 根据MaskDib32中的alpha通道计算
	void	LogicalOperate (const FCDib & Dib, DWORD dwRop, int x=0, int y=0) ; // dwRop见StdDefine.h
	void	AND (const FCDib & Dib, int x=0, int y=0) ;
	void	 OR (const FCDib & Dib, int x=0, int y=0) ;
	void	XOR (const FCDib & Dib, int x=0, int y=0) ;

	//	剪贴板
	BOOL	CopyToClipboard (const RECT * rect = NULL) ;
	BOOL	PasteFromClipboard (const RECT * rect = NULL) ;
	BOOL	IsPasteAvailable () ;

	//	杂项函数 (静态)
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
	void	__ConvertToTrueColor (int iColor) ; // 转换为真彩色。iColor为24 or 32
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
		return false ; // 没有相交区域
	}
	return true ;
}

#endif