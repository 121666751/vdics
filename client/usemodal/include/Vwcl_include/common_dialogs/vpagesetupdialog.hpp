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

#ifndef VPAGESETUPDIALOG
#define VPAGESETUPDIALOG

#include "../vstandard.h"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"
#include "vcommondialog.hpp"

/** VPageSetupDialog is a common dialog wrapper which supports the
Page Setup dialog box.*/
class VPageSetupDialog : public VCommonDialog
{
public:
	/** Default constructor initializes base class with RTTI information
	and initializes internal members.*/
	VPageSetupDialog()
		: VCommonDialog(VWCL_RTTI_PAGE_SETUP_DIALOG)
		{ m_hDevMode = m_hDevNames = NULL; }

	/** Show the dialog box. Returns VTRUE on success, VFALSE if the user
	clicked the Cancel button. nFlags corresponds to the PAGESETUPDLG
	structure Flags member. Please see this structure documentation for
	more information.*/
	VBOOL					DoModal(
		VWindow const&	windowParent,
		VDWORD			nFlags =
							PSD_DEFAULTMINMARGINS |
							PSD_INTHOUSANDTHSOFINCHES)
	{
		/* Initialize the PAGESETUPDLG structure.*/
		PAGESETUPDLG psd;
		VZEROSTRUCT(psd);
		psd.lStructSize =		sizeof(psd);
		psd.hwndOwner =			windowParent.GetHandle();
		psd.hDevMode =			m_hDevMode;
		psd.hDevNames =			m_hDevNames;
		psd.Flags =				nFlags | PSD_ENABLEPAGESETUPHOOK;
		psd.lCustData =			(DWORD)this;
		psd.lpfnPageSetupHook =	HookProc;

		/* Are margins known?*/
		if ( m_rMinMargin.IsNotNothing() )
		{
			psd.rtMinMargin =	m_rMinMargin.GetRECT();
			psd.Flags |=		PSD_MINMARGINS;
		}

		if ( m_rMargin.IsNotNothing() )
		{
			psd.rtMargin =		m_rMargin.GetRECT();
			psd.Flags |=		PSD_MARGINS;
		}

		/* Call virtual function before showing the dialog.*/
		PreShow(psd);

		if ( PageSetupDlg(&psd) )
		{
			/* Save modes.*/
			m_hDevMode =	psd.hDevMode;
			m_hDevNames =	psd.hDevNames;

			/* Save paper size.*/
			m_ptPaperSize = psd.ptPaperSize;

			/* Save margins?*/
			if ( !(psd.Flags & PSD_DISABLEMARGINS) )
			{
				m_rMinMargin =	psd.rtMinMargin;
				m_rMargin =		psd.rtMargin;
			}

			return VTRUE;
		}

		return VFALSE;
	}

	/** Return a reference to the DEVMODE structure. This is a handle, so it
	must locked with GlobalLock() before using it, and unlocked with
	GlobalLock() when you are done with it.*/
	HANDLE&					GetDevMode() const
		{ return (HANDLE&)m_hDevMode; }

	/** Return a reference to the DEVNAME structure. This is a handle, so it
	must locked with GlobalLock() before using it, and unlocked with
	GlobalLock() when you are done with it.*/
	HANDLE&					GetDevNames() const
		{ return (HANDLE&)m_hDevNames; }

	/** Return a reference to the left, top, right, and bottom margins. It
	is initialized to 0,0,0,0 and will populated after the first call to
	DoModal(). The dimensions default to thousandths of inches, but is
	controlled by passing either PSD_INTHOUSANDTHSOFINCHES or
	PSD_INHUNDREDTHSOFMILLIMETERS as one of the flags to DoModal().*/
	VRect&					GetMargin() const
		{ return (VRect&)m_rMargin; }

	/** Return a reference to the minimum allowable widths for the left,
	top, right, and bottom margins. These values must be less than or equal
	to the values specified using GetMargin(). It is initialized to 0,0,0,0
	and will populated after the first call to DoModal().The dimensions
	default to thousandths of inches, but is controlled by passing either
	PSD_INTHOUSANDTHSOFINCHES or PSD_INHUNDREDTHSOFMILLIMETERS as one of
	the flags to DoModal().*/
	VRect&					GetMinMargin() const
		{ return (VRect&)m_rMinMargin; }

	/** Return a reference to the dimensions of the paper selected by the
	user. This will be populated after the first call to DoModal(). The
	dimensions default to thousandths of inches, but is controlled by
	passing either PSD_INTHOUSANDTHSOFINCHES or
	PSD_INHUNDREDTHSOFMILLIMETERS as one of the flags to DoModal().*/
	VPoint&					GetPaperSize() const
		{ return (VPoint&)m_ptPaperSize; }

	/** Set the internal device mode and names handles. These are normally
	populated by this class, but could be given from a print dialog box
	using the VPrintDialog common dialog class. If you use both classes,
	when each others DoModal() function returns VTRUE, the device modes and
	names should be set into each other. For instance, if you use
	VPrintDialog and its DoModal() function returns VTRUE, you should call
	this function and pass the results of VPrintDialog::GetDevMode(), and
	VPrintDialog::GetDevNames() to this function. If at a later time this
	dialog box resets these values, you should call this same function in
	VPrintDialog to update its values. This idea is to keep them in sync.*/
	void					SetDevices(	HANDLE hDevMode,
										HANDLE hDevNames)
	{
		m_hDevMode =	hDevMode;
		m_hDevNames =	hDevNames;
	}

protected:
	/** Static callback procedure used to attach to VWCL.*/
	static VUINT CALLBACK	HookProc(	HWND	hWnd,
										VUINT	nMessage,
										VWPARAM	wParam,
										VLPARAM	lParam)
	{
		if ( nMessage == WM_INITDIALOG )
		{
			VASSERT(lParam)
			VPageSetupDialog* pThis =
				(VPageSetupDialog*)((PAGESETUPDLG*)lParam)->lCustData;
			VASSERT(pThis)

			pThis->Attach(hWnd);

			return 1;
		}

		return 0;
	}

	/** Called just before the dialog box is shown. An override can
	adjust PAGESETUPDLG members as needed.*/
	virtual void			PreShow(PAGESETUPDLG& psd)
		{;}

	/** Embedded Members.*/
	VRect					m_rMinMargin;
	VRect					m_rMargin;
	VPoint					m_ptPaperSize;
	HANDLE					m_hDevMode;
	HANDLE					m_hDevNames;
};

#endif /* VPAGESETUPDIALOG*/
