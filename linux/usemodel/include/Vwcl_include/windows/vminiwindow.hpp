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

#ifndef VMINIWINDOW
#define VMINIWINDOW

#include "../vstandard.h"
#include "../gdi/vbrush.hpp"
#include "../gdi/vdc.hpp"
#include "../gdi/vgdiobjectselector.hpp"
#include "../gdi/vstockfont.hpp"
#include "../numbers/vrtti.hpp"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"
#include "vwindow.hpp"

/** VMiniWindow displays a window with a small caption bar which defaults
to 12 pixels high. These windows are all client area, and have no real
caption bar. The base class draws the caption in dark blue with white text
in the caption using the ANSI_VAR_FONT font.*/
class VMiniWindow : public VWindow
{
public:
	/** Default constructor initializes the RTTI information in the VWindow
	base class. It also sets the VWindow option OPTION_NEED_ON_PAINT to
	that the OnPaint() notification is sent. This is where the drawing of
	the caption area takes place.*/
	VMiniWindow(VRTTI_TYPE nRTTI = VWCL_RTTI_MINI_WINDOW)
		: VWindow(nRTTI)
	{
		GetOptions().Append(OPTION_NEED_ON_PAINT);
	}

	/** Override gets the caption rectangle area. This function uses a
	12 pixel caption area. Override to change.*/
	virtual void	GetCaptionRect(VRect& r)
	{
		GetClientRect(r);
		r.GetBottom() = 12;
	}

	/** Determine if the point at pt is in the dragging area. This is the
	fake caption area that we draw. Since the caption is drawn by us, and
	the window does not have the WS_CAPTION style set, we have to be able
	to determine when the user should be able to drag the window by
	implementing this caption area. This function will return VTRUE is pt
	is in the fake caption area, VFALSE otherwise.*/
	VBOOL			IsPointInClientArea(VPoint const& pt)
	{
		VRect r;
		GetCaptionRect(r);

		return r.IsPointInside(pt);
	}

protected:
	/** Override enforces correct window styles for mini-caption support.*/
	virtual VBOOL	PreCreateWindow(CREATESTRUCT& CreateStruct)
	{
		CreateStruct.style &= ~WS_CAPTION;
		return VWindow::PreCreateWindow(CreateStruct);
	}

	/** Override paints the custom caption area.*/
	virtual VBOOL	OnPaint(VDC& dc)
	{
		/* Get the caption rectangle.*/
		VRect r;
		GetCaptionRect(r);

		VBrush br;

		/* Create a dark blue brush to use for caption background
		and select into the DC.*/
		if ( br.CreateSolid(0, 0, 127) )
		{
			br.FillRect(dc, r);

			/* Get and draw text.*/
			VString s;

			if ( GetText(s) )
			{
				VINT nOldBkMode =			dc.BackgroundMode(TRANSPARENT);
				COLORREF crOldTextColor =	dc.TextColor(RGB(255,255,255));

				/* Select a white color and set text options.*/
				VStockFont			font(ANSI_VAR_FONT);
				VGDIObjectSelector	os(dc, font);
				dc.DrawText(s, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

				/* Reset DC.*/
				dc.BackgroundMode(nOldBkMode);
				dc.TextColor(crOldTextColor);
			}
		}

		return VWindow::OnPaint(dc);
	}

	/** Window procedure looks for WM_NCHITTEST and calls
	IsPointInClientArea() as appropriate.*/
	virtual VLONG	WindowProc(	HWND	hWnd,
								VUINT	nMessage,
								VWPARAM	wParam,
								VLPARAM	lParam)
	{
		if ( nMessage == WM_NCHITTEST )
		{
			/* Translate coordinates from screen to client.*/
			VPoint pt(LOWORD(lParam), HIWORD(lParam));
			ScreenToClient(pt);

			/* Call virtual function to deterine if point is in a
			"drag area". If so, tell Windows the mouse is in a caption
			area (so it can be dragged).*/
			if ( IsPointInClientArea(pt) )
				return HTCAPTION;
		}

		return VWindow::WindowProc(hWnd, nMessage, wParam, lParam);
	}
};

#endif /* VMINIWINDOW*/
