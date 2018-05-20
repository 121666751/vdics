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

#ifndef VCOLORDIALOG
#define VCOLORDIALOG

#include "../vstandard.h"
#include "vcommondialog.hpp"

static VUINT vwcl_gs_vcolordialog_nMsgSETRGB = 0;

/** VColorDialog is a common dialog wrapper which supports the
standard color chooser dialog.*/
class VColorDialog : public VCommonDialog
{
public:
	/** Default constructor initializes base class with RTTI information
	and initializes internal members.*/
	VColorDialog()
		: VCommonDialog(VWCL_RTTI_COLOR_DIALOG)
	{
		m_crColor = 0;

		/* Register custom message (as needed).*/
		if ( !vwcl_gs_vcolordialog_nMsgSETRGB )
			vwcl_gs_vcolordialog_nMsgSETRGB =
				RegisterWindowMessage(SETRGBSTRING);
	}

	/** Return the selected color. This is valid to call after the dialog
	box has gone away (when DoModal() returns).*/
	COLORREF				Color() const
		{ return m_crColor; }

	/** Show the dialog box. Returns VTRUE on success, VFALSE if the user
	cancelled the dialog, or an error occurred. If crInitColor is given, it
	will be used to initialized the default color presented to the user. If
	not given, black is assumed. To use this dialog box over and over again,
	and have it initialized with the last chosen color, you can get the
	last color using the Color() method of this class when call DoModal().
	The nFlags parameter corresponds to the CHOOSECOLOR structure Flags
	parameter. Please see this structure for more information.*/
	VBOOL					DoModal(	VWindow const&	windowParent,
										COLORREF		crInitColor =
															0,
										VDWORD			nFlags =
															CC_FULLOPEN)
	{
		static COLORREF sharedCustomColors[16];

		/* Initialize structure.*/
		CHOOSECOLOR cc;
		VZEROSTRUCT(cc);
		cc.lStructSize =	sizeof(cc);
		cc.lCustData =		(VDWORD)this;
		cc.lpfnHook =		HookProc;
		cc.hwndOwner =		windowParent.GetHandle();
		cc.rgbResult =		crInitColor;
		cc.lpCustColors =	sharedCustomColors;
		cc.Flags =			nFlags | CC_ENABLEHOOK | CC_RGBINIT;

		/* Call virtual function before showing the dialog.*/
		PreShow(cc);

		if ( ChooseColor(&cc) )
		{
			m_crColor = cc.rgbResult;
			return VTRUE;
		}

		return VFALSE;
	}

protected:
	/** Set the current color. This function is protected since it can only
	be called after DoModal() is called and a window created, which means
	there would be no good way to call it from outside the class, since the
	dialog would be in a modal state preventing other code from being able
	to call this member function.*/
	void					Color(COLORREF crColor) const
		{ VWINDOW_SM2(vwcl_gs_vcolordialog_nMsgSETRGB, 0, crColor); }

	/** Static callback procedure used to attach the HWND to the object.*/
	static VUINT CALLBACK	HookProc(	HWND	hWnd,
										VUINT	nMessage,
										VWPARAM	wParam,
										VLPARAM	lParam)
	{
		/* Verify attachment.*/
		if ( nMessage == WM_INITDIALOG )
		{
			VASSERT(lParam)
			VColorDialog* pThis =
				(VColorDialog*)((CHOOSECOLOR*)lParam)->lCustData;
			VASSERT(pThis)
			VASSERT(!pThis->GetSafeWindow())

			pThis->Attach(hWnd);

			return 1;
		}

		return 0;
	}

	/** Called just before the dialog box is shown. An override can adjust
	CHOOSECOLOR members as needed.*/
	virtual void			PreShow(CHOOSECOLOR& cc)
		{;}

	/** Embedded Members.*/
	COLORREF				m_crColor;
};

#endif /* VCOLORDIALOG*/
