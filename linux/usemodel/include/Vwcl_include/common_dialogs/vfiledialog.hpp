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

#ifndef VFILEDIALOG
#define VFILEDIALOG

#include "../vstandard.h"
#include "../parsers/vdelimitedstring.hpp"
#include "../strings/vstring.hpp"
#include "vcommondialog.hpp"

/* Some (older) compilers fail to define this.*/
#ifndef OFN_ENABLESIZING
	#define OFN_ENABLESIZING 0x00800000
#endif

/** VFileDialog is a common dialog wrapper which supports the File Open
and Save As dialog boxes. When opening files, it supports both single
and multiple file selections, and has excellent support for user defined
filters.*/
class VFileDialog : public VCommonDialog
{
public:
	/** Default constructur initializes base class with RTTI information.*/
	VFileDialog()
		: VCommonDialog(VWCL_RTTI_FILE_DIALOG)
		{;}

	/** Add a new filter that will be displayed to the user when the dialog
	is shown. pszDisplay is what is shown to the user when they click the
	types combobox. pszFileExtension is what is used by the common dialog
	to determine what file types to display to the user. The first
	extension added using this function will be the default displayed to
	the user. This can be changed by overriding the PreShow() function and
	altering the OPENFILENAME structure nFilterIndex member. This is a 1
	based index, so the first item you add using this function is 1, not 0.
	An example would be for Microsoft Word to show filters for DOC and DOT
	files, calls to this function would look like this:
	AddFilter("Word Documents", "doc");
	AddFilter("Word Templates", "dot");
	AddFilter("All Files", "*");*/
	void					AddFilter(	VSTRING_CONST pszDisplay,
										VSTRING_CONST pszFileExtension)
	{
		VASSERT(VSTRLEN_CHECK(pszDisplay))
		VASSERT(VSTRLEN_CHECK(pszFileExtension))

		/* Build filter string except don't terminate with final \n\n,
		that will be done later. We do append one trailing \n.*/
		m_strFilters += pszDisplay;
		m_strFilters += VTEXT("\n*.");
		m_strFilters += pszFileExtension;
		m_strFilters += VTEXT('\n');
	}

	/** Show the dialog box. Returns VTRUE on success, VFALSE if the user
	cancelled the dialog. If bOpenDialog is VTRUE, a file open dialog will
	be shown, otherwise a Save As dialog will be. nFlags corresponds to the
	OPENFILENAME structure Flags member. See this structure for more
	information. If the user should be presented with a default filename, it
	can be specified in pszFileName. If pszFileName is NULL, the last known
	filename will be used. If pszInitialDirectory not NULL, files in this
	directory will be shown, otherwise the last used directory will be used.
	pszDefaultExtension if not NULL will cause this extension to be appended
	automatically if the user did not enter a file extension. pszCaption, if
	not NULL, will be used as the dialog title. If NULL, the default of
	Open or Save As will be used.*/
	VBOOL					DoModal(	VWindow const&	windowParent,
										VBOOL			bOpenDialog =
															VTRUE,
										VDWORD			nFlags =
															OFN_HIDEREADONLY,
										VSTRING_CONST	pszFileName =
															NULL,
										VSTRING_CONST	pszInitialDirectory =
															NULL,
										VSTRING_CONST	pszDefaultExtension =
															NULL,
										VSTRING_CONST	pszCaption =
															NULL)
	{
		/* Big buffer to support multiple file selections (many).*/
		VCHAR sz[16 * 1024];

		/* User specified starting directory? In not, use default.*/
		if ( !pszInitialDirectory )
		{
			if (	m_strCurrentDirectory.IsEmpty() &&
					GetCurrentDirectory(VARRAY_SIZE(sz), sz) )
				m_strCurrentDirectory = sz;

			pszInitialDirectory = m_strCurrentDirectory;
		}

		/* User specified filename? If not, use current one, if any.*/
		if ( pszFileName )
			VSTRCPY(sz, pszFileName);
		else if ( m_strSingleSelectFileName.IsNotEmpty() )
			VSTRCPY(sz, m_strSingleSelectFileName);
		else
			sz[0] = VTEXT('\0');

		/* Initialize structure.*/
		OPENFILENAME ofn;
		VZEROSTRUCT(ofn);
		ofn.lStructSize =		sizeof(ofn);
		ofn.hwndOwner =			windowParent.GetHandle();
		ofn.lpstrFile =			sz;
		ofn.nMaxFile =			VARRAY_SIZE(sz);
		ofn.lpstrInitialDir =	pszInitialDirectory;
		ofn.lpstrTitle =		pszCaption;
		ofn.lpstrDefExt =		pszDefaultExtension;
		ofn.lCustData =			(VDWORD)this;
		ofn.lpfnHook =			HookProc;
		ofn.Flags =				nFlags |
								OFN_EXPLORER |
								OFN_NOCHANGEDIR |
								OFN_PATHMUSTEXIST |
								OFN_ENABLEHOOK |
								OFN_ENABLESIZING;

		/* Filters? Copy the string before modifying it.*/
		VString sFilters(m_strFilters);

		if ( sFilters.IsNotEmpty() )
		{
			/* Add trailing newline to make it valid.*/
			sFilters += VTEXT('\n');

			/* Set into ofn.*/
			ofn.lpstrFilter =	sFilters.MakeFilter();
			ofn.nFilterIndex =	1;
		}

		/* Determine correct options bits.*/
		if ( bOpenDialog )
			ofn.Flags |= OFN_FILEMUSTEXIST;
		else
		{
			/* When saving, prompt user if they are overwriting a file,
			and don't allow multiple selections for saving.*/
			ofn.Flags |= OFN_OVERWRITEPROMPT;
			ofn.Flags &= ~OFN_ALLOWMULTISELECT;
		}

		/* Call virtual function before showing the dialog.*/
		PreShow(ofn);

		/* Show the dialog.*/
		if ( ((bOpenDialog) ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn)) )
		{
			/* Reset selection filenames.*/
			m_strSingleSelectFileName.Empty();
			m_strMultiSelectFileNames.Empty();

			/* Clear directory string.*/
			m_strCurrentDirectory.Empty();

			if ( ofn.Flags & OFN_ALLOWMULTISELECT )
			{
				/* If sz is a file, the user only selected 1 name.*/
				if ( VDoesFileExist(sz) )
				{
					/* Save file name and current directory.*/
					m_strMultiSelectFileNames = m_strCurrentDirectory = sz;
					m_strCurrentDirectory.ReplaceLastChar(VFILE_PATH_SEP_CHAR);
				}
				else
				{
					/* Save the index into sz where the first file name
					is found. The first string inside sz is the directory.*/
					VUINT nFirstFileNameIndex = 0;

					/* sz now has 0 terminators scattered throughout it. The
					end of the string is indicated by 2 0's. Replace all
					single 0's with tabs to separate strings. This makes it
					easier to parse later.*/
					for ( VUINT i = 0; i < VARRAY_SIZE(sz); i++ )
					{
						if ( sz[i] == VTEXT('\0') )
						{
							/* Save directory?*/
							if ( !nFirstFileNameIndex )
							{
								/* If sz ends with a file path separator,
								remove it.*/
								size_t nLength = VSTRLEN(sz);

								if ( sz[nLength - 1] == VFILE_PATH_SEP_CHAR )
									nLength--;
								
								/* Save directory.*/
								m_strCurrentDirectory.String(sz, 0, nLength);
								
								/* Save first file index.*/
								nFirstFileNameIndex = i;
							}

							/* Replace with tab character.*/
							sz[i] = VTEXT('\t');

							/* Time to exit loop?*/
							if ( sz[i + 1] == VTEXT('\0') )
								break;
						}
					}
					
					/* Save file names.*/
					m_strMultiSelectFileNames = &sz[nFirstFileNameIndex + 1];
				}
				
				/* Initialize parser to handle tab delimited strings.*/
				m_dsMultiSelectFileNames.Reset(	m_strMultiSelectFileNames,
												VTEXT('\t'));
			}
			/* Save single selection string and path.*/
			else if ( m_strSingleSelectFileName.String(sz) )
				m_strCurrentDirectory.String(sz, 0, ofn.nFileOffset - 1);

			/* Success?*/
			if ( m_strCurrentDirectory.IsNotEmpty() )
			{
				if ( ofn.Flags & OFN_ALLOWMULTISELECT )
					return m_strMultiSelectFileNames.IsNotEmpty();

				return m_strSingleSelectFileName.IsNotEmpty();
			}
		}

		return VFALSE;
	}

	/** Return a reference to the last used directory. This string should
	never end in the final slash or backslash.*/
	VString&				GetDirectory() const
		{ return (VString&)m_strCurrentDirectory; }

	/** Return a reference to the filename selected. This function should
	only be used when the OFN_ALLOWMULTISELECT is NOT used as an option
	when opening files. It is safe to use when saving files since
	OFN_ALLOWMULTISELECT is not allowed. If multiple file selections are
	used, you should call GetNextFileName() in a loop until it returns
	VFALSE to receive each selected filename.*/
	VString&				GetFileName() const
		{ return (VString&)m_strSingleSelectFileName; }

	/** When multiple selection Open dialogs are allowed, using the
	OFN_ALLOWMULTISELECT flag, this function should be called in a loop to
	retreive each selected file name. This will be the entire path to the
	selected file. This function should be called until it returns VFALSE,
	even if you stop opening files, so that the internal string can be
	updated. When VFALSE is returned, or on error, the string at strBuffer
	is not modified.*/
	VBOOL					GetNextFileName(VString& strBuffer)
	{
		/* Get file name from next item in delimited parser.*/
		VSTRING_CONST pszFileName = m_dsMultiSelectFileNames.Next();

		if ( pszFileName )
		{
			/* Initialize with path first.*/
			strBuffer = m_strCurrentDirectory;

			/* Append path separator character.*/
			strBuffer.EndWith(VFILE_PATH_SEP_CHAR);

			/* Append the filename.*/
			strBuffer += pszFileName;

			return VTRUE;
		}

		return VFALSE;
	}

protected:
	/** Hook procedure attaches this window to VWCL.*/
	static VUINT CALLBACK	HookProc(	HWND	hWnd,
										VUINT	nMessage,
										VWPARAM	wParam,
										VLPARAM	lParam)
	{
		if ( nMessage == WM_INITDIALOG )
		{
			VASSERT(lParam)
			VFileDialog* pThis =
				(VFileDialog*)((OPENFILENAME*)lParam)->lCustData;
			VASSERT(pThis)
			VASSERT(!pThis->GetSafeWindow())

			pThis->Attach(hWnd);

			return 1;
		}
		return 0;
	}

	/** Override looks for notification messages to call virtual functions.*/
	virtual VLONG			OnNotify(	VUINT	nIDControl,
										NMHDR&	nmhdr)
	{
		OFNOTIFY& notify = (OFNOTIFY&)nmhdr;

		switch ( notify.hdr.code )
		{
			case CDN_INITDONE:
				OnInitDone();
				break;

			case CDN_SELCHANGE:
				OnFileNameChange();
				break;

			case CDN_FOLDERCHANGE:
				OnFolderChange();
				break;

			case CDN_SHAREVIOLATION:
			{
				VUINT nResult = OnShareViolation(notify.pszFile);

				if ( nResult == OFN_SHAREWARN )
				{
					SetDialogResult(OFN_SHAREWARN);
					return 1;
				}

				return nResult;
			}

			case CDN_HELP:
				OnHelp();
				break;

			case CDN_FILEOK:
				if ( OnFileNameCheck(notify.lpOFN->lpstrFile) )
					return 0;
				SetDialogResult(1);
				return 1;

			case CDN_TYPECHANGE:
				OnTypeChange(notify.lpOFN->nFilterIndex);
				break;
		}

		return VCommonDialog::OnNotify(nIDControl, nmhdr);
	}

	/** Override this function to handle the WM_NOTIFY CDN_INITDONE message.
	The notification message is sent when the system has finished arranging
	controls in the Open or Save As dialog box to make room for the
	controls of the child dialog box.*/
	virtual void			OnInitDone()
		{;}

	/** Override this function to handle the WM_NOTIFY CDN_SELCHANGE message.
	The notification message is sent when the user selects a new file or
	folder in the file list of the Open or Save As dialog box.*/
	virtual void			OnFileNameChange()
		{;}

	/** Override this function if you want to provide custom validation of
	filenames that are entered in the dialog box. Return VTRUE to accept
	the filename and close the dialog, or VFALSE to reject it and keep the
	dialog open. If you decide this filename is invalid, you should notify
	the user to provide this feedback.*/
	virtual VBOOL			OnFileNameCheck(VSTRING_CONST pszFileName)
		{ return VTRUE; }

	/** Override this function to handle the WM_NOTIFY CDN_FOLDERCHANGE
	message. The notification message is sent when a new folder is opened
	in the Open or Save As dialog box.*/
	virtual void			OnFolderChange()
		{;}

	/** Override this function to handle the WM_NOTIFY CDN_HELP message. The
	notification message is sent when the user clicks the Help button in
	the Open or Save As dialog box.*/
	virtual void			OnHelp()
		{;}

	/** Override this function to provide custom handling of share violations.
	Valid return values are:

	OFN_SHAREFALLTHROUGH - The filename is returned from the dialog box.

	OFN_SHARENOWARN - No further action needs to be taken.

	OFN_SHAREWARN - The user receives the standard warning message for
	this error.*/
	virtual VUINT			OnShareViolation(VSTRING_CONST pszFileName)
		{ return OFN_SHAREWARN; }

	/** Override this function to handle the WM_NOTIFY CDN_TYPECHANGE
	message. The notification message is sent when the user selects a new
	file type from the list of file types in the Open or Save As dialog
	box. nFilterIndex is the index into the filters that were added to the
	dialog, and are 1, not 0, based.*/
	virtual void			OnTypeChange(VUINT nFilterIndex)
		{;}

	/** Called just before the dialog box is shown. An override can
	adjust OPENFILENAME members as needed.*/
	virtual void			PreShow(OPENFILENAME& ofn)
		{;}

	/** Embedded Members.*/
	VString					m_strFilters;
	VString					m_strSingleSelectFileName;
	VString					m_strMultiSelectFileNames;
	VString					m_strCurrentDirectory;
	VDelimitedString		m_dsMultiSelectFileNames;
};

#endif /* VFILEDIALOG*/
