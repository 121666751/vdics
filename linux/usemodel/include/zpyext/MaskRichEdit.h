#if !defined(AFX_MASKRICHEDIT_H__31FEFA86_50B8_11D4_97BC_0080C8E98569__INCLUDED_)
#define AFX_MASKRICHEDIT_H__31FEFA86_50B8_11D4_97BC_0080C8E98569__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaskRichEdit.h : header file
//
#include <zbitmask.hpp>
/////////////////////////////////////////////////////////////////////////////
// CMaskRichEdit window

class ZPYEXT_API CMaskRichEdit : public CRichEditCtrl
{
// Construction
public:
	ZBitmask m_TextMask;
	CHARFORMAT cf;
	CMaskRichEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaskRichEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	UINT m_nAfterDot;
	virtual ~CMaskRichEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMaskRichEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	bool m_bHasDot;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASKRICHEDIT_H__31FEFA86_50B8_11D4_97BC_0080C8E98569__INCLUDED_)
