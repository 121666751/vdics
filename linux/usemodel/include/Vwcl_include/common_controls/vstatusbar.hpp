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

Primary Author of this source code file:  Todd Osborne (todd@vwcl.org)
Other Author(s) of this source code file:
*/

#ifndef VSTATUSBAR
#define VSTATUSBAR

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../strings/vstring.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "../utility/vmemoryblock.hpp"
#include "vcommoncontrol.hpp"

/** VStatusBar encapsulates the Windows status bar common control.*/
class VStatusBar : public VCommonControl
{
public:
	/** Default constructor initializes RTTI information and sets default
	text to the word "Ready".*/
	VStatusBar(VRTTI_TYPE nRTTI = VWCL_RTTI_STATUSBAR)
		: VCommonControl(nRTTI)
		{ m_strDefault = VTEXT("Ready"); }

	/** Create the statusbar control. Returns VTRUE on success, VFALSE on
	error. nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus statusbar specific ones including:

	CCS_BOTTOM - Causes the control to position itself at the bottom of the
	parent window�s client area and sets the width to be the same as the
	parent window�s width. Status bar controls have this style by default.

	CCS_NODIVIDER - Prevents a two-pixel highlight from being drawn at the
	top of the control.

	CCS_NOHILITE - Prevents a one-pixel highlight from being drawn at the
	top of the control.

	CCS_NOMOVEY - Causes the control to resize and move itself horizontally,
	but not vertically, in response to a WM_SIZE message. If the
	CCS_NORESIZE style is used, this style does not apply.

	CCS_NOPARENTALIGN - Prevents the control from automatically moving to
	the top or bottom of the parent window. Instead, the control keeps its
	position within the parent window despite changes to the size of the
	parent window. If the CCS_TOP or CCS_BOTTOM style is also used, the
	height is adjusted to the default, but the position and width remain
	unchanged.

	CCS_NORESIZE - Prevents the control from using the default width and
	height when setting its initial size or a new size. Instead, the control
	uses the width and height specified in the request for creation or
	sizing.

	CCS_TOP - Causes the control to position itself at the top of the
	parent window�s client area and sets the width to be the same as the
	parent window�s width. The default position of a status window is
	along the bottom of the parent window, but you can specify the CCS_TOP
	style to have it appear at the top of the parent window�s client area.
	You can specify the SBARS_SIZEGRIP style to include a sizing grip at
	the right end of the status window. Combining the CCS_TOP and
	SBARS_SIZEGRIP styles is not recommended, because the resulting sizing
	grip is not functional even though the system draws it in the status
	window.*/
	VBOOL			Create(	VWindow const&	windowParent,
							VRect const&	r,
							VUINT			nID,
							VSTRING_CONST	pszText =
												NULL,
							VUINT			nStyle =
												WS_BORDER |
												WS_VISIBLE |
												SBARS_SIZEGRIP |
												CCS_BOTTOM |
												CCS_NOMOVEY)
	{
		return VCommonControl::Create(	windowParent,
										r,
										WS_CHILD | nStyle,
										STATUSCLASSNAME,
										(pszText)
										? pszText
										: GetDefaultString(),
										 nID);
	}

	/** Return a reference to default string object used to display when
	SetText() string is NULL.*/
	VString&		GetDefaultString() const
		{ return (VString&)m_strDefault; }

	/** Retrieves the current widths of the horizontal and vertical borders
	of a status window. Returns VTRUE if successful, or VFALSE otherwise.
	pnBorders is an integer array that has three elements. The first
	element receives the width of the horizontal border, the second
	receives the width of the vertical border, and the third receives the
	width of the border between rectangles. The borders determine the
	spacing between the outside edge of the window and the rectangles
	within the window that contain text. The borders also determine the
	spacing between rectangles.*/
	VBOOL			GetBorders(VINT* pnBorders) const
	{
		VASSERT(pnBorders)
		return VWINDOW_SMB2(SB_GETBORDERS, 0, pnBorders);
	}

	/** Return the number of parts in the status bar.*/
	VUSHORT			GetPartCount() const
		{ return GetParts(0); }

	/** Retrieves a count of the parts in a status window. The function also
	retrieves the coordinate of the right edge of the specified number of
	parts. Returns the number of parts in the window if successful, or zero
	otherwise. nParts specifies the number of parts for which to retrieve
	coordinates. If this parameter is greater than the number of parts in
	the window, the function retrieves coordinates for existing parts only.
	pnRightCoord is the address of an integer array that has the same number
	of elements as parts specified by nParts. Each element in the array
	receives the client coordinate of the right edge of the corresponding
	part. If an element is set to -1, the position of the right edge for
	that part extends to the right edge of the window. To retrieve the
	current number of parts, set this parameter to zero, or use
	GetPartCount(). nParts is 0 based.*/
	VUSHORT			GetParts(	VUSHORT	nParts,
								VINT*	pnRightCoord =
											NULL) const
		{ return (VUSHORT)VWINDOW_SM2(SB_GETPARTS, nParts, pnRightCoord); }

	/** Retrieves the bounding rectangle of a part in a status window.
	Returns VTRUE if successful, or VFALSE otherwise. nPart is the
	zero-based index of the part whose bounding rectangle is to be
	retrieved. r is a VRect object that receives the bounding rectangle.
	nPart is 0 based.*/
	VBOOL			GetRect(	VRect&	r,
								VWPARAM	nPart =
											0) const
		{ return VWINDOW_SMB2(SB_GETRECT, nPart, r.GetPointer()); }

	/** Get the text at the part of the statusbar identified by nPart.
	Returns the string contained within the strBuffer string class on
	success, NULL on failure. strBuffer will not be modified on failure.
	nPart is 0 based.*/
	VSTRING			GetText(	VString&	strBuffer,
								VWPARAM		nPart =
												0) const
	{
		VWORD nLength = GetTextLength(nPart);

		if ( nLength )
		{
			VMemoryBlock memBlock;

			if ( memBlock.AllocForString(nLength) )
			{
				VWINDOW_SM2(SB_GETTEXT, nPart, memBlock.Get());
				strBuffer = memBlock;

				return strBuffer;
			}
		}

		return NULL;
	}

	/** Return the length of the text held in the nPart part of the
	statusbar. nPart is 0 based.*/
	VWORD			GetTextLength(VWPARAM nPart = 0) const
		{ return LOWORD(VWINDOW_SM1(SB_GETTEXTLENGTH, nPart)); }

#ifdef SB_GETICON
	/** Return the icon associated with the specified part of the
	status bar, or NULL if one is not a part of the display.*/
	HICON			Icon(VWPARAM nPart = 0) const
		{ return (HICON)VWINDOW_SM1(SB_GETICON, nPart); }
#endif

#ifdef SB_SETICON
	/** Set the icon associated with the specified part of the
	status bar, or NULL if one is not to be a part of the display.*/
	HICON			Icon(	HICON	hIcon,
							VWPARAM	nPart =
										0) const
		{ return (HICON)VWINDOW_SM2(SB_SETICON, nPart, hIcon); }
#endif

	/** Sets the minimum height of a status window's drawing area.
	nMinHeight is the minimum height, in pixels, of the window. The minimum
	height is the sum of wParam and twice the width, in pixels, of the
	vertical border of the status window. An application must send the
	WM_SIZE message to the status window to redraw the window. The wParam
	and lParam parameters of the WM_SIZE message should be set to zero.*/
	void			SetMinHeight(VWPARAM nMinHeight) const
		{ VWINDOW_SM1(SB_SETMINHEIGHT, nMinHeight); }

	/** Sets the number of parts in a status window and the coordinate of the
	right edge of each part. Returns VTRUE if successful, or VFALSE
	otherwise. nParts is  the number of parts to set (cannot be greater
	than 255). pnWidthArray is the address of an integer array that has the
	same number of elements as parts specified by nParts. Each element in
	the array specifies the position, in client coordinates, of the right
	edge of the corresponding part. If an element is -1, the position of
	the right edge for that part extends to the right edge of the window.
	nParts is 0 based.*/
	VBOOL			SetParts(	VUSHORT	nParts,
								VINT*	pnWidthArray) const
	{
		VASSERT(pnWidthArray)
		return VWINDOW_SMB2(SB_SETPARTS, nParts, pnWidthArray);
	}

	/** Set the text in nPart of pszText. If pszText is NULL, the default
	string will be used. If pszText already is being shown in the status
	bar, this function does nothing. Returns VTRUE if the statusbar text
	was updated, VFALSE otherwise. nPart is 0 based.*/
	virtual VBOOL	SetText(	VSTRING_CONST	pszText =
													NULL,
								VWPARAM			nPart =
													0)
	{
		/* If pszText is NULL, use default string.*/
		VSTRING_CONST pszNewText =
			(VSTRLEN_CHECK(pszText)) ? pszText : m_strDefault;

		/* Get current text and compare with pszText. We don't want flicker.*/
		VString sCurrent;

		if ( !GetText(sCurrent, nPart) || sCurrent != pszNewText )
		{
			if ( VWINDOW_SM2(SB_SETTEXT, nPart, pszNewText) )
			{
				Update();
				return VTRUE;
			}

			return VFALSE;
		}

		return VTRUE;
	}

	/** Returns VTRUE if the status bar is in simple mode, VFALSE otherwise.*/
	VBOOL			SimpleMode() const
		{ return VWINDOW_SMB0(SB_ISSIMPLE); }

	/** Specifies whether a status window displays simple text or displays
	all window parts set by a previous SetParts() call. Returns VFALSE if
	an error occurs. If bSimple is VTRUE, the window displays simple text.
	If it is VFALSE, it displays multiple parts. If the status window is
	being changed from nonsimple to simple, or vice versa, the window is
	immediately redrawn.*/
	VBOOL			SimpleMode(VBOOL bSimple) const
		{ return VWINDOW_SMB1(SB_SIMPLE, bSimple); }

protected:
	/** Embedded Members.*/
	VString			m_strDefault;
};

#endif /* VSTATUSBAR*/
