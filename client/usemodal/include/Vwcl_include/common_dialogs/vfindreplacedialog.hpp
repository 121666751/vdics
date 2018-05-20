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

#ifndef VFINDREPLACEDIALOG
#define VFINDREPLACEDIALOG

#include "../vstandard.h"
#include "../windows/vwindowmessagefilter.hpp"
#include "vcommondialog.hpp"

static VUINT vwcl_gs_vfindreplacedialog_nMsgFIND = 0;

/** VFindReplaceDialog implements the Find and Replace common dialog box.*/
class VFindReplaceDialog : public VCommonDialog, public VWindowMessageFilter
{
public:
	/** Default constructur initializes base class with RTTI information
	and initializes internal members.*/
	VFindReplaceDialog()
		: VCommonDialog(VWCL_RTTI_FIND_DIALOG)
	{
		VZEROSTRUCT(m_FindReplace);
		VZEROMEMORY(m_szFind, sizeof(m_szFind));
		VZEROMEMORY(m_szReplace, sizeof(m_szReplace));

		/* The base class assumes a modal dialog. We aren't, so change this.*/
		m_bModal = VFALSE;

		if ( !vwcl_gs_vfindreplacedialog_nMsgFIND )
			vwcl_gs_vfindreplacedialog_nMsgFIND =
				RegisterWindowMessage(FINDMSGSTRING);
	}

	/** Create a non-modal dialog box. There is no such thing as a modal
	find and replace dialog. On success, a valid HWND is returned, which
	can be wrapped in a VWindow object. NULL is returned on failure. There
	is no need to manually destroy the window unless you want to. It will
	be destroyed when the user closes the dialog box or when this class is
	reused. Because this dialog is implemented as a non-modal window, your
	main window must call VTranslateDialogMessage() from an override of
	PreTranslateMessage(). This is not possible for modal VMainDialog
	applications, but is easy using VMainWindow or one of its derived
	classes, or a non-modal VMainDialog. After this function is called the
	first time, each future call will initialize the dialog with the
	previous search and replace strings. You can however still specify
	these strings by using GetFindString() and GetReplaceString(). The
	maximum size of each of these strings is 1024 characters.
	
	The nFlags parameter is the same as the Flags parameter of the FINDREPLACE
	structure. Please see this structure documentation for more information.
	Once created, this type of dialog does not actually do and Finding or
	Replacing. That is application specific. It will automatically call
	overrides of the OnFind() and OnReplace() functions, which you must
	implement in a derived class.*/
	HWND				Create(	VWindow const&	windowParent,
								VBOOL			bFind =
													VTRUE,
								VDWORD			nFlags =
													0)
	{
		/* Re-create window?*/
		Destroy();

		/* Initialize structure members.*/
		VZEROSTRUCT(m_FindReplace);
		m_FindReplace.lStructSize =		sizeof(m_FindReplace);
		m_FindReplace.lCustData =		(VDWORD)this;
		m_FindReplace.lpfnHook =		HookProc;
		m_FindReplace.hwndOwner =		windowParent.GetHandle();
		m_FindReplace.Flags =			nFlags | FR_ENABLEHOOK;
		m_FindReplace.lpstrFindWhat =	m_szFind;
		m_FindReplace.wFindWhatLen =	sizeof(m_szReplace);

		if ( !bFind )
		{
			m_FindReplace.lpstrReplaceWith =	m_szReplace;
			m_FindReplace.wReplaceWithLen =		sizeof(m_szReplace);
		}

		/* Call virtual function before showing the dialog.*/
		PreShow(m_FindReplace);

		/* Save find or replace flag.*/
		if ( bFind )
			return FindText(&m_FindReplace);

		return ReplaceText(&m_FindReplace);
	}

	/** Return a pointer to the internal string used to hold find text.*/
	VSTRING				GetFindString() const
		{ return (VSTRING)m_szFind; }

	/** Return a pointer to the internal string used to hold replacement
	text.*/
	VSTRING				GetReplaceString() const
		{ return (VSTRING)m_szReplace; }

protected:
	/** Static callback procedure used to attach the HWND to
	VFindReplaceDialog.*/
	static VUINT CALLBACK	HookProc(	HWND	hWnd,
										VUINT	nMessage,
										WPARAM	wParam,
										LPARAM	lParam)
	{
		if ( nMessage == WM_INITDIALOG )
		{
			VASSERT(lParam)
			VFindReplaceDialog* pThis =
				(VFindReplaceDialog*)((FINDREPLACE*)lParam)->lCustData;
			VASSERT(pThis)
			VASSERT(!pThis->GetSafeWindow())

			/* Attach to VWCL.*/
			pThis->Attach(hWnd);

			/* Install a message filter in parent window to get
			notifications we care about.*/
			VWindow* pParent = pThis->GetVParent();

			/* The parent window is required!*/
			VASSERT(pParent)

			if ( pParent )
				pParent->InstallMessageFilter(VREF_PTR(pThis));

			return 1;
		}

		return 0;
	}

	/** Override allows the parent window to be free'd of message routing
	chores.*/
	virtual void			OnDestroy()
	{
		VWindow* pParent = GetVParent();

		if ( pParent )
			pParent->InstallMessageFilter(VREF_THIS, VFALSE);
	}

	/** VWindowMessageFilter base class override looks for messages that
	only we know about.*/
	virtual VBOOL			OnMessageFilter(	VWindow&	parentWindow,
												VUINT		nMessage,
												VWPARAM		wParam,
												VLPARAM		lParam,
												VLONG&		nResult,
												VBOOL&		bUseResult)
	{
		if ( nMessage == vwcl_gs_vfindreplacedialog_nMsgFIND )
		{
			VBOOL bDown =
				(m_FindReplace.Flags & FR_DOWN) ? VTRUE : VFALSE;

			VBOOL bMatchCase =
				(m_FindReplace.Flags & FR_MATCHCASE) ? VTRUE : VFALSE;

			VBOOL bWholeWord =
				(m_FindReplace.Flags & FR_WHOLEWORD) ? VTRUE : VFALSE;

			/* Prepare to call virtual functions.*/
			VBOOL bKeepDialog = VTRUE;

			if ( m_FindReplace.Flags & FR_FINDNEXT )
				bKeepDialog =
					OnFindNext(	m_szFind,
								bDown,
								bMatchCase,
								bWholeWord);

			else if ( m_FindReplace.Flags & FR_REPLACEALL )
				bKeepDialog =
					OnReplace(	m_szFind,
								m_szReplace,
								VTRUE,
								bDown,
								bMatchCase,
								bWholeWord);

			else if ( m_FindReplace.Flags & FR_REPLACE )
				bKeepDialog =
					OnReplace(	m_szFind,
								m_szReplace,
								VFALSE,
								bDown,
								bMatchCase,
								bWholeWord);

			if ( !bKeepDialog )
				Destroy();

			/* Message handled.*/
			return VFALSE;
		}

		/* Allow normal processing.*/
		return VTRUE;
	}

	/** This function is called when it is time to find the next string in
	your application that matched pszFind. bDown will be set to VTRUE if
	the user is searching "down" you document, VFALSE otherwise. If the
	user wants a case sensitive search, bMatchCase will be VTRUE, otherwise
	VFALSE. If you should match the whole word, bWholeWord will be VTRUE,
	otherwise it will be be VFALSE and you should match any part of the
	word. If you do not want to allow the user to specify down, match case,
	or whole word, you can specify this in the Create() call using the
	flags paramter. You should return VTRUE to continue finding the next
	string, VFALSE to close the dialog. The default implementation closes
	the dialog.*/
	virtual VBOOL			OnFindNext(	VSTRING_CONST	pszFind,
										VBOOL			bDown,
										VBOOL			bMatchCase,
										VBOOL			bWholeWord)
		{ return VFALSE; }

	/** Same as OnFindNext(), except used when replacing strings. Please see
	OnFindNext() for parameter information If the user wants to replace all
	text bReplaceAll will be VTRUE, otherwise it will be VFALSE.*/
	virtual VBOOL			OnReplace(	VSTRING_CONST	pszFind,
										VSTRING_CONST	pszReplace,
										VBOOL			bReplaceAll,
										VBOOL			bDown,
										VBOOL			bMatchCase,
										VBOOL			bWholeWord)
		{ return VFALSE; }

	/** Called just before the dialog box is shown. An override can
	adjust FINDREPLACE members as needed.*/
	virtual void			PreShow(FINDREPLACE& fr)
		{;}

	/** Embedded Members.*/
	FINDREPLACE				m_FindReplace;
	VCHAR					m_szFind[1024];
	VCHAR					m_szReplace[1024];
};

#endif /* VFINDREPLACEDIALOG*/
