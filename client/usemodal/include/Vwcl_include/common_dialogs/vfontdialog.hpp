/* VWCL - The Virtual Windows Class Library.
Copyright (C) 1996-2000 The VWCL Alliance

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

Primary Author of this source code file:	Simon Capewell
											(simonc@bristol.crosswinds.net)
Other Author(s) of this source code file:	Todd Osborne
											(todd@vwcl.org)
*/

#ifndef VFONTDIALOG
#define VFONTDIALOG

#include "../vstandard.h"
#include "vcommondialog.hpp"

/** VFontDialog encapsulates the standard font selection dialog box.*/
class VFontDialog : public VCommonDialog
{
public:
	/** Default constructor initializes base class with RTTI
	information and initializes internal members.*/
	VFontDialog()
		: VCommonDialog(VWCL_RTTI_FONT_DIALOG)
	{
		VZEROSTRUCT(m_LogFont);
		m_crColor = 0;
	}

	/** Return a reference to the last selected font color.*/
	COLORREF&				Color() const
		{ return (COLORREF&)m_crColor; }

	/** Show the dialog box. If pLogFont is not given, the internal LOGFONT
	structure will be used. If it is given, the internal LOGFONT structure
	is NOT modified by this function, so referring to it later may give
	incorrect results. If you specify pLogFont, just understand that this
	class is working on behalf of that structure, not the internal one.
	Returns VTRUE on SUCCESS, or VFALSE is the user cancelled the dialog
	box. The nFlags parameter represents the Flags parameter of the
	CHOOSEFONT structure. Please see the documentation on CHOOSEFONT for
	more information.*/
	VBOOL					DoModal(VWindow const&	windowParent,
									LOGFONT*		pLogFont =
														NULL,
									VDWORD			nFlags =
														CF_BOTH |
														CF_EFFECTS)
	{
		/* Because this dialog procedure always modifies the LOGFONT
		passed to it, make a temp copy.*/
		LOGFONT lf;
		CopyLogFont(&lf, (pLogFont) ? pLogFont : &m_LogFont);

		/* Initialize structure.*/
		CHOOSEFONT cf;
		VZEROSTRUCT(cf);
		cf.lStructSize =	sizeof(cf);
		cf.hwndOwner =		windowParent.GetHandle();
		cf.Flags =			nFlags | CF_ENABLEHOOK | CF_INITTOLOGFONTSTRUCT;
		cf.lpLogFont =		&lf;
		cf.lCustData =		(VDWORD)this;
		cf.lpfnHook =		HookProc;
		cf.rgbColors =		m_crColor;

		/* Call virtual function before showing the dialog.*/
		PreShow(cf);

		/* Show the dialog box.*/
		if ( ChooseFont(&cf) )
		{
			/* Copy the temp LOGFONT back to permanent storage.*/
		   CopyLogFont((pLogFont) ? pLogFont : &m_LogFont, &lf);

		   /* Save color.*/
		   m_crColor = cf.rgbColors;

		   /* Success.*/
		   return VTRUE;
		}

		return VFALSE;
	}

	/** Same as above, but returns a created font on success, NULL on
	failure. The HFONT can be put into a VFont class if desired. It must be
	free'd with DeleteObject() when caller is done using it if not placed
	into a VFont.*/
	HFONT					DoModalCreateFont(
		VWindow const&	windowParent,
		LOGFONT*		pLogFont =
							NULL,
		VDWORD			nFlags =
							CF_BOTH |
							CF_EFFECTS)
	{
		LOGFONT* pLOGFONT = (pLogFont) ? pLogFont : &m_LogFont;

		if ( DoModal(windowParent, pLOGFONT, nFlags) )
			return CreateFontIndirect(pLOGFONT);

		return NULL;
	}

	/** Return a pointer to the internal LOGFONT structure.*/
	LOGFONT*				GetPointer() const
		{ return (LOGFONT*)&m_LogFont; }

	/** Return a reference to the internal LOGFONT structure.*/
	LOGFONT&				GetLOGFONT() const
		{ return (LOGFONT&)m_LogFont; }

protected:
	/** Correctly copy one LOGFONT to another.*/
	void					CopyLogFont(	LOGFONT* pDest,
											LOGFONT* pSource) const
	{
		VASSERT(pDest)
		VASSERT(pSource)

		*pDest = *pSource;
		VSTRCPY(pDest->lfFaceName, pSource->lfFaceName);
	}

	/** Get the currently selected font. This function is protected since it
	can only be called after DoModal() is called and a window created, which
	means there would be no good way to call it from outside the class,
	since the dialog would be in a modal state preventing other code from
	being able to call this member function.*/
	void					GetFont(LOGFONT& lf) const
		{ VWINDOW_SM2(WM_CHOOSEFONT_GETLOGFONT, 0, &lf); }

	/** Hook procedure used to attach to VWCL.*/
	static VUINT CALLBACK	HookProc(	HWND	hWnd,
										VUINT	nMessage,
										VWPARAM	wParam,
										VLPARAM	lParam)
	{
		if ( nMessage == WM_INITDIALOG )
		{
			VASSERT(lParam)
			VFontDialog* pThis =
				(VFontDialog*)((CHOOSEFONT*)lParam)->lCustData;
			VASSERT(pThis)
			VASSERT(!pThis->GetSafeWindow())

			pThis->Attach(hWnd);

			return 1;
		}

		return 0;
	}

	/** Called just before the dialog box is shown. An override can
	adjust CHOOSEFONT members as needed.*/
	virtual void			PreShow(CHOOSEFONT& cf)
		{;}

	/** Set the sample text using a string. This function is protected
	since it can only be called after DoModal() is called and a window
	created, which means there would be no good way to call it from outside
	the class, since the dialog would be in a modal state preventing other
	code from being able to call this member function.*/
	void					SetSampleText(VSTRING_CONST pszText) const
		{ SetDialogItemText(1092, pszText); }

	/** Embedded Members.*/
	LOGFONT					m_LogFont;
	COLORREF				m_crColor;
};

#endif /* VFONTDIALOG*/
