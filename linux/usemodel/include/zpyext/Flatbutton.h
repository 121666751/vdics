/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Yuheng Zhao
// All rights reserved
//
// Distribute freely, except: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// If you have any questions, I can be reached as follows:
//    yuheng@ministars.com
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _FLATBUTTON_H_
#define _FLATBUTTON_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class ZPYEXT_API CFlatButton : public CButton
{
// Construction
public:
	CFlatButton();

// Attributes
public:
	BOOL m_bMouseIn;
	//BOOL m_bMouseDown;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	//CRect rect;
	virtual ~CFlatButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlatButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#endif
