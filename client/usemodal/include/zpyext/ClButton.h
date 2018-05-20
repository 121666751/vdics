#if !defined(AFX_CLBUTTON_H__87237E3D_4BE1_11D4_8A2C_0010DC223CBC__INCLUDED_)
#define AFX_CLBUTTON_H__87237E3D_4BE1_11D4_8A2C_0010DC223CBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClButton window
const COLORREF CLOUDBLUE = RGB(128, 184, 223);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF BLACK = RGB(1, 1, 1);
const COLORREF DKGRAY = RGB(128, 128, 128);
const COLORREF LTGRAY = RGB(192, 192, 192);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF DKYELLOW = RGB(128, 128, 0);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF DKRED = RGB(128, 0, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF DKBLUE = RGB(0, 0, 128);
const COLORREF CYAN = RGB(0, 255, 255);
const COLORREF DKCYAN = RGB(0, 128, 128);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF DKGREEN = RGB(0, 128, 0);
const COLORREF MAGENTA = RGB(255, 0, 255);
const COLORREF DKMAGENTA = RGB(128, 0, 128);

#define ZWM_CLBUTTON_CLICK WM_USER+123

class ZPYEXT_API CClButton : public CButton
{
// Construction
public:
	CClButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBkColor(COLORREF color);
	void SetTextColor(COLORREF color);
	virtual ~CClButton();

	// Generated message map functions
protected:
	CBrush m_Brush;
	COLORREF m_TextColor;
	COLORREF m_BkColor;
	//{{AFX_MSG(CClButton)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CClButtonEx window

class ZPYEXT_API CClButtonEx : public CClButton
{
// Construction
public:
	CClButtonEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClButtonEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClButtonEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClButtonEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLBUTTON_H__87237E3D_4BE1_11D4_8A2C_0010DC223CBC__INCLUDED_)
