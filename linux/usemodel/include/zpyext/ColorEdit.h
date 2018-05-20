#if !defined(AFX_COLOREDIT_H__A3D7EFFF_05F0_11D4_89BB_0010DC223CBC__INCLUDED_)
#define AFX_COLOREDIT_H__A3D7EFFF_05F0_11D4_89BB_0010DC223CBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorEdit.h : header file
//


#define COLOR_RED	RGB(255,0,0)
#define COLOR_GREEN RGB(0,255,0)
#define COLOR_BLUE	RGB(0,0,255)
#define COLOR_WHITE RGB(255,255,255)
#define COLOR_GRAY	RGB(220,220,220)
#define COLOR_BLACK RGB(0,0,0)


/////////////////////////////////////////////////////////////////////////////
// CColorEdit window

class ZPYEXT_API CColorEdit : public CEdit
{
// Construction
public:
	CColorEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBkColor(COLORREF color);
	void SetTextColor(COLORREF color);
	void SetReadOnlyBkColor(COLORREF color);
	void SetReadOnlyTextColor(COLORREF color);
	virtual ~CColorEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_rgbBackColor;
	COLORREF m_rgbTextColor,m_rgbInactiveBackColor,m_rgbInactiveTextColor;
	CBrush m_InactiveBrush;
	CBrush m_ActiveBrush;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDIT_H__A3D7EFFF_05F0_11D4_89BB_0010DC223CBC__INCLUDED_)
