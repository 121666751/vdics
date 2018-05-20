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

Primary Author of this source code file:  Eric Herrmann (emh1@home.com)
Other Author(s) of this source code file: Todd Osborne (todd@vwcl.org)
*/

#ifndef VTOOLTIP
#define VTOOLTIP

#include "../vstandard.h"
#include "../windows/vwindow.hpp"

/** VToolTip allows association of a help string or text with child windows
of a window. You need one VToolTip for each dialog or window. Call AddTool()
on each child window to associate a specific help string with each child.*/
class VToolTip : public VWindow
{
public:
	/** Default constructor initializes the class.*/
	VToolTip()
		: VWindow(VWCL_RTTI_TOOLTIP)
	{
		/* Initialize members.*/
		m_hWndParent =		NULL;
	}

	/** Activate or deactivate the tooltip.*/
	void		Activate(VBOOL bActive = VTRUE) const
		{ VWINDOW_SM1(TTM_ACTIVATE, bActive); }

	/** Adds a new child item which may have help text associated with it.
	If nStringID is zero, then the message TTN_NEEDTEXT will be sent to the
	parent window in the form of a WM_NOTIFY (OnNotify) message to
	retrieve help text. Otherwise, the string resource indicated by the
	string resource nStringID will be displayed. Call this for each child
	you want to have a tooltip. Note: If the string is set using this
	function, or one that calls it, there is a 63 character limit imposed
	by the tooltip common control, and enforced by this function. It is
	possible for tooltips to displays longer string, but you will have to
	handle TTN_NEEDTEXT messages in the owner window to do so.*/
	VBOOL		AddTool(	HWND		hWndChild,
							VUINT		nStringID = 0,
							HINSTANCE	hResource = NULL) const
	{
		/* Create() must have been called already!*/
		VASSERT(GetSafeWindow())
		VASSERT(m_hWndParent)

		/* This must be known.*/
		VASSERT(hWndChild)

		if ( hWndChild )
		{
			TOOLINFO ti;
			VZEROSTRUCT(ti);

			/* Initialize structure.*/
			ti.cbSize = sizeof(ti);
			ti.uFlags =	TTF_IDISHWND | TTF_SUBCLASS;
			ti.hwnd =	m_hWndParent;
			ti.uId =	(VUINT)hWndChild;

			if ( nStringID )
			{
				ti.hinst =		(hResource)
								? hResource
								: VGetResourceHandle();
				ti.lpszText =	MAKEINTRESOURCE(nStringID);
			}
			else
				ti.lpszText = LPSTR_TEXTCALLBACK;

			return VWINDOW_SMB2(TTM_ADDTOOL, 0, &ti);
		}

		return VFALSE;
	}

	/** Same as above, but accept a child window ID instead of the handle.*/
	VBOOL		AddTool(	VUINT		nChildID,
							VUINT		nStringID = 0,
							HINSTANCE	hResource = NULL) const
	{
		VASSERT(m_hWndParent)
		VASSERT(nChildID)

		return AddTool(	GetDlgItem(m_hWndParent, nChildID),
						nStringID,
						hResource);
	}

	/** Create associates a tooltip with the parent window. Call this in
	the SetupWindow() override of the parent. nStyle can be one or more of
	the following values:

	TTF_ABSOLUTE - Version 4.70. Positions the tooltip window at the same
	coordinates provided by TTM_TRACKPOSITION. This flag must be used with
	the TTF_TRACK flag.

	TTF_CENTERTIP - Centers the tooltip window below the tool specified by
	the uId member.

	TTF_RTLREADING - Indicates that the tooltip text will be displayed in
	the opposite direction to the text in the parent window.

	TTF_TRACK - Version 4.70. Positions the tooltip window next to the tool
	to which it corresponds and moves the window according to coordinates
	supplied by the TTM_TRACKPOSITION messages. You must activate this type
	of tool using the TTM_TRACKACTIVATE message.

	TTF_TRANSPARENT - Version 4.70. Causes the tooltip control to forward
	mouse event messages to the parent window. This is limited to mouse
	events that occur within the bounds of the tooltip window.*/
	VBOOL		Create(VWindow const& windowParent, VUINT nStyle = 0)
	{
		/* Parent window must be valid!*/
		VASSERT(windowParent.GetSafeWindow())

		m_hWndParent = windowParent.GetHandle();
		VASSERT(m_hWndParent)

		/* Enforce documented usage.*/
		if ( nStyle & TTF_ABSOLUTE )
			nStyle |= TTF_TRACK;

		if ( VWindow::Create(	windowParent,
								VRECT_EMPTY,
								nStyle | WS_POPUP |
								TTS_NOPREFIX | TTS_ALWAYSTIP,
								TOOLTIPS_CLASS) )
		{
			::SetWindowPos(	GetHandle(),
							HWND_TOPMOST,
							0,
							0,
							0,
							0,
							SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

			return VTRUE;
		}

		return VFALSE;
	}

	/** Set delay times. nDuration may be one of the following:

	TTDT_AUTOPOP - Retrieve the length of time the tooltip window remains
	visible if the pointer is stationary within a tool's bounding rectangle.

	TTDT_INITIAL - Retrieve the length of time the pointer must remain
	stationary within a tool's bounding rectangle before the tooltip window
	appears.

	TTDT_RESHOW - Retrieve the length of time it takes for subsequent
	tooltip windows to appear as the pointer moves from one tool to another.

	nTime is the delay in milliseconds where 1000 = 1 second.*/
	void		DelayTime(VDWORD nDuration, VINT nTime) const
		{ VWINDOW_SM3(TTM_SETDELAYTIME, nDuration, nTime, 0); }

	/** Retrieve delay times. nDuration may be one of the following:

	TTDT_AUTOPOP - Retrieve the length of time the tooltip window remains
	visible if the pointer is stationary within a tool's bounding rectangle.

	TTDT_INITIAL - Retrieve the length of time the pointer must remain
	stationary within a tool's bounding rectangle before the tooltip window
	appears.

	TTDT_RESHOW - Retrieve the length of time it takes for subsequent
	tooltip windows to appear as the pointer moves from one tool to another.*/
	VINT		DelayTime(VDWORD nDuration) const
		{ return (VINT)VWINDOW_SM1(TTM_SETDELAYTIME, nDuration); }

protected:
	/** Embedded Members.*/
	HWND		m_hWndParent;
};

#endif
