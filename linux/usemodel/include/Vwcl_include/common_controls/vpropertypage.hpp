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

Primary Author of this source code file:  Chris Waldren (chriswa@microsoft.com)
Other Author(s) of this source code file: Todd Osborne (todd@vwcl.org)
*/

#ifndef VPROPERTYPAGE
#define VPROPERTYPAGE

#include "../vstandard.h"
#include "../windows/vdialog.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"

class VPropertySheet;

/** VPropertyPage is the base class for dialogs that are children of a
VPropertySheet. This is an encapsulation of the Windows property sheet and
property page functions. Derived classes should override OnOK() and
OnCancel() to perform user acceptance or cancellation messages.*/
class VPropertyPage : public VDialog
{
friend class VPropertySheet;

public:
	/** Default constructor initializes base class and internal members. In
	addition, you can initialize the page with a dialog template ID, and a
	user (you) defined nParam value.*/
	VPropertyPage(	VUINT		nDialogTemplateID =
									0,
					HINSTANCE	hResource =
									NULL,
					VLPARAM		nParam =
									0,
					VRTTI_TYPE	nRTTI =
									VWCL_RTTI_PROPERTY_PAGE)
		: VDialog(nRTTI)
	{
		VZEROSTRUCT(m_psPage);
		m_psPage.dwSize =		sizeof(m_psPage);
		m_psPage.pfnDlgProc =	(DLGPROC)WindowProcShared;
		m_psPage.lParam =		(VLPARAM)this;

		if ( nDialogTemplateID )
			Init(nDialogTemplateID, hResource);
	}

	/** Return a reference to the internal PROPSHEETPAGE structure.*/
	PROPSHEETPAGE&		GetPROPSHEETPAGE() const
		{ return (PROPSHEETPAGE&)m_psPage; }

	/** Initialize the property page with a dialog template ID.*/
	void				Init(	VUINT		nDialogTemplateID,
								HINSTANCE	hResource =
												NULL)
	{
		VASSERT(nDialogTemplateID)

		m_psPage.hInstance =
			(hResource) ? hResource : VGetResourceHandle();

		m_psPage.pszTemplate =	MAKEINTRESOURCE(nDialogTemplateID);
	}

    /** Set the icon for the page from a resource ID.*/
	void				SetIcon(VUINT nIconID)
		{ SetIcon(MAKEINTRESOURCE(nIconID)); }

	/** Set the icon for the page from resources.*/
	void				SetIcon(VSTRING_CONST pszIcon)
	{
		m_psPage.dwFlags |= PSP_USEICONID;
		m_psPage.pszIcon = pszIcon;
	}

protected:
	/** Notifies a page that the user has clicked the Help button.*/
	virtual void		OnHelp()
		{;}

	/** Notifies a page that it is about to lose activation either because
	another page is being activated or the user has clicked the OK button.
	Return VTRUE to prevent the page from losing the activation, or VFALSE
	to allow it.*/
	virtual VBOOL		OnKillActive()
		{ return VFALSE; }

	/** Notifies a page that it is about to be activated. Return zero to
	accept the activation, or -1 to activate the next or the previous page
	(depending on whether the user clicked the Next or Back button). To set
	the activation to a particular page, return the resource identifier of
	the page.*/
	virtual VLONG		OnSetActive()
		{ return 0; }

	/** Notifies a page that the user has clicked the Back button in a
	wizard property sheet. Return VFALSE to prevent the property sheet from
	advancing to the previous page or VTRUE otherwise.*/
	virtual VBOOL		OnWizardBack()
		{ return VTRUE; }

	/** Notifies a page that the user has clicked the Next button in a
	wizard property sheet. Return VFALSE to prevent the property sheet from
	advancing to the next page or VTRUE otherwise.*/
	virtual VLONG		OnWizardNext()
		{ return VTRUE; }

	/** Notifies a page that the user has clicked the Finish button in a
	wizard property sheet. Return a VTRUE to prevent the property sheet from
	being destroyed or VFALSE otherwise.*/
	virtual VBOOL		OnWizardFinish()
		{ return VFALSE; }

	/** Override looks for notification messages to call virtual functions.*/
	virtual VLONG		OnNotify(	VUINT	nIDControl,
									NMHDR&	nmhdr)
	{
		VLONG nResult = VTRUE;

		/* Dispatch the property sheet notification.*/
		switch ( nmhdr.code )
		{
			/* Property page activated.*/
			case PSN_SETACTIVE:
				nResult = OnSetActive();
				break;

			/* Property page deactivated.*/
			case PSN_KILLACTIVE:
				nResult = OnKillActive();
				break;

			/* User selected Help.*/
			case PSN_HELP:
				OnHelp();
				break;

			/* User clicked Back on wizard.*/
			case PSN_WIZBACK:
				nResult = (OnWizardBack()) ? 0 : -1;
				break;

			/* User clicked Next on wizard.*/
			case PSN_WIZNEXT:
				nResult = (OnWizardNext()) ? 0 : -1;
				break;

			/* User clicked Finish on wizard.*/
			case PSN_WIZFINISH:
				nResult = OnWizardFinish();
				break;

			default:
				nResult = VDialog::OnNotify(nIDControl, nmhdr);
				break;
		}

		return nResult;
	}

	/** Embedded Members.*/
	PROPSHEETPAGE				m_psPage;
};

#endif /* VPROPERTYPAGE*/
