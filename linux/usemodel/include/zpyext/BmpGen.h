// BmpGen.h: interface for the CBmpGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMPGEN_H__EAEFEEC6_4CA7_11D4_97B5_0080C8E98569__INCLUDED_)
#define AFX_BMPGEN_H__EAEFEEC6_4CA7_11D4_97B5_0080C8E98569__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ZPYEXT_API CBmpGen  
{
public:
	CBmpGen();
	virtual ~CBmpGen();

public:
	int			CreatePalatte(LPLOGPALETTE pPal=NULL);
	void*		Create(SIZE& bmpSize,LPLOGPALETTE pPal=NULL);
	CBitmap		m_Bitmap;
	CPalette	m_Palette;
	void*		m_pBits;
};

#endif // !defined(AFX_BMPGEN_H__EAEFEEC6_4CA7_11D4_97B5_0080C8E98569__INCLUDED_)
