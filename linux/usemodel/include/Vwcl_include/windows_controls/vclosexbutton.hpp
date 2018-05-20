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

#ifndef VCLOSEXBUTTON
#define VCLOSEXBUTTON

#include "../vstandard.h"
#include "../gdi/v3drect.hpp"
#include "../gdi/vdc.hpp"
#include "../gdi/vgdiobjectselector.hpp"
#include "../gdi/vpen.hpp"
#include "../gdi/vstockpen.hpp"
#include "../gdi/vtempdc.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindowmessagefilter.hpp"
#include "vbutton.hpp"

/* Forward Declaration(s).*/
class VCloseXButton;

/** VCloseXButtonNotify is an abstract base class. It provides a single
function, OnClickXButton(), which must be overriden in a derived class,
or multiply inheritited into another class. This function is called by
the VCloseXButton class whenever a user clicks on the button part of
the control. You can provide your processing for these commands in your
override of OnCloseXButton(). A control of this type only draws in an
area not to exceed 12 pixels high. In addition, the client area must
be at least 12 pixels wide.*/
class VCloseXButtonNotify
{
friend class VCloseXButton;

protected:
	/** OnClickXButton(), which must be overriden in a derived class,
	or multiply inheritited into another class, is called by the
	VCloseXButton class whenever a user clicks on the button part of
	the control. You can provide your processing for these commands in
	your override of OnClickXButton().*/
	virtual void	OnClickXButton(VCloseXButton& wndButton) = 0;
};

/** VCloseXButton is an owner-drawn button control which closely resembles
a miniature toolbar placed above other windows. It is common to see controls
like this placed on top of controls that can be hidden, minimized, maximized,
etc. While hard to describe, an example can be seen in the Visual C++ IDE.
Just above the project management workspace is a control that allows the
workspace to be hidden. In ASCII, it looks something like this:
======================= X
The X is the close button, and would hide (but not really destroyed) the
project workspace. VCloseXButton can be used to display a graphical X,
or arrows pointing left, right, up, or down.*/
class VCloseXButton : public VButton, public VWindowMessageFilter
{
public:
	/** Standard constructor initializes class runtime type information
	and member variables. The default button type (style) is to show
	an X, which normally closes a window. The button type can be set
	with the ButtonType() method after creation, or after the control
	is created with Create().*/
	VCloseXButton(VRTTI_TYPE nRTTI = VWCL_RTTI_CLOSE_X_BUTTON)
		: VButton(nRTTI)
	{
		/* Initialize members.*/
		m_pNotify =		NULL;
		m_nButtonType =	BUTTON_TYPE_CLOSE;
	}

	/** This enumeration represents the possible button types that
	can be set or returned from the ButtonType() method. This determines
	how the button rectangle (the part the user can click) is drawn.
	BUTTON_TYPE_CLOSE will draw an X in the button.
	BUTTON_TYPE_POINT_LEFT will draw a left pointing arrow in the button.
	BUTTON_TYPE_POINT_RIGHT will draw a right pointing arrow in the button.
	BUTTON_TYPE_POINT_UP will draw an up pointing arrow in the button.
	BUTTON_TYPE_POINT_DOWN will draw a down pointing arrow in the button.*/
	enum	{				BUTTON_TYPE_CLOSE,
							BUTTON_TYPE_POINT_LEFT,
							BUTTON_TYPE_POINT_UP,
							BUTTON_TYPE_POINT_RIGHT,
							BUTTON_TYPE_POINT_DOWN,
			};

	/** Return the current button type. See the BUTTON_TYPE_XXX enumeration
	for more information about this return value.*/
	VBYTE					ButtonType() const
		{ return m_nButtonType; }

	/** Set the current button type. See the BUTTON_TYPE_XXX enumeration for
	a list of possible values for nButtonType. This method can be called
	before the window control is created or after it is visible. If called
	while the control is visible, the control will be updated to show the
	new button type. If there is no change in the button type, this function
	with be a NOOP.*/
	void					ButtonType(VBYTE nButtonType)
	{
		VASSERT(nButtonType <= BUTTON_TYPE_POINT_DOWN)

		if ( nButtonType != m_nButtonType )
		{
			m_nButtonType = nButtonType;

			/* Draw now if already a visible window.*/
			VRect r;

			if ( GetXButtonRect(r) )
				InvalidateRectUpdate(r);
		}
	}

	/** Create the button control. All parameters are the same as
	VButton::Create() and VWindow::Create().*/
	VBOOL					Create(	VWindow&		windowParent,
									VRect const&	r,
									VUINT			nID,
									VUINT			nStyle =
														WS_VISIBLE)
	{
		/* Install the message filter in parent window.*/
		windowParent.InstallMessageFilter(VREF_THIS);

		if ( VButton::Create(	windowParent,
								r,
								nID,
								NULL,
								WS_CHILD | 
								BS_PUSHBUTTON |
								BS_OWNERDRAW|
								nStyle) )
		{
			return VTRUE;
		}

		/* Remove filter on failure.*/
		windowParent.InstallMessageFilter(VREF_THIS, VFALSE);

		return VFALSE;
	}

	/** Get the rectangle where all drawing can take place. This is not
	the client area of the control. A control of this type only draws in
	an area not to exceed 12 pixels high. In addition, the client area must
	be at least 12 pixels wide. If these conditions are not met, this method
	will return VFALSE. If sufficient room exists to draw the control, and
	the control is visible, this function returns VTRUE.*/
	VBOOL					GetAreaRect(VRect& rArea) const
	{
		if ( GetSafeWindow() && IsVisible() )
		{
			/* Start with client area.*/
			GetClientRect(rArea);

			/* Make sure we have room to draw in.*/
			if (	rArea.GetHeight() >= 12 &&
					rArea.GetWidth() >= 12 )
			{
				/* Center our drawing area in the Y axis. We limit
				ourself to drawing in a 12 pixel high area.*/
				rArea.GetTop() =	rArea.GetHalfHeight() - 6;
				rArea.GetBottom() = rArea.GetTop() + 12;

				return VTRUE;
			}
		}

		return VFALSE;
	}

	/** Return the area of the line that makes up the top set of horizontal
	lines drawn in the control. See GetAreaRect() for restrictions that could
	cause this function to return VFALSE. Returns VTRUE on success.*/
	VBOOL					GetLinesRect(VRect& rLines) const
	{
		/* Get drawing area rectangle.*/
		if ( GetAreaRect(rLines) )
		{
			rLines.SubtractRight(16);
			rLines.AddTop(2);
			rLines.GetBottom() = rLines.GetTop() + 3;

			return VTRUE;
		}

		return VFALSE;
	}

	/** Return the area of the button where the user can click. See
	GetAreaRect() for restrictions that could cause this function to
	return VFALSE. Returns VTRUE on success.*/
	VBOOL					GetXButtonRect(VRect& rXButton) const
	{
		VRect rArea;

		/* Get total area we draw in first.*/
		if ( GetAreaRect(rXButton) )
		{
			rXButton.GetLeft() = rXButton.GetRight() - 12;
			return VTRUE;
		}

		return VFALSE;
	}

	/** Return a pointer to the VCloseXButtonNotify object that will receive
	notification when the user clicks the button area of the control.*/
	VCloseXButtonNotify*	Notify() const
		{ return (VCloseXButtonNotify*)m_pNotify; }

	/** Set a pointer to the VCloseXButtonNotify object that will receive
	notification when the user clicks the button area of the control.*/
	void					Notify(VCloseXButtonNotify* pNotify)
		{ m_pNotify = pNotify; }

protected:
	/** Draw the button area (where the user can click) of the control.*/
	virtual void			DrawButton(	VDC&			dc,
										VRect const&	rArea,
										VBOOL			bButtonDown)
	{
		/* Initialize 3D rectangle for drawing close button.*/
		V3DRect r;

		/* Get X button rectangle.*/
		if ( GetXButtonRect(r) )
		{
			/* Set the drawing style(s) we want.*/
			r.GetOptions() = V3DRect::OPTION_SHRINK_RECT_ON_DRAW;

			/* Set the colors.*/
			r.GetLeftTopColor() =
				(bButtonDown) ? RGB(0,0,0) : RGB(255,255,255);

			r.GetRightBottomColor() =
				(bButtonDown) ? RGB(255,255,255) : RGB(0,0,0);

			/* Draw outside border.*/
			r.Draw(dc);

			/* Fix right edge for button style.*/
			if ( !bButtonDown )
			{
				VStockPen borderPen(BLACK_PEN);
				VGDIObjectSelector os1(dc, borderPen);

				/* Draw from upper right to lower right.*/
				dc.MoveTo(r.GetRight(),	r.GetTop() - 1);
				dc.LineTo(r.GetRight(), r.GetTop() + 1);
			}

			/* Draw inside border.*/
			if ( bButtonDown )
			{
				r.AddTop();
				r.GetLeftTopColor() = RGB(127,127,127);
				r.GetOptions().Append(	V3DRect::OPTION_NO_DRAW_RIGHT |
										V3DRect::OPTION_NO_DRAW_BOTTOM);
			}
			else
			{
				r.SubtractBottom();
				r.GetRightBottomColor() = RGB(127,127,127);
				r.GetOptions().Append(	V3DRect::OPTION_NO_DRAW_LEFT |
										V3DRect::OPTION_NO_DRAW_TOP);
			}

			/* Offset and draw inside border now.*/
			r.Draw(dc);

			/* Draw the "X".*/

			/* Shrink first to get away from edges.*/
			r.Deflate(2, 2);

			/* Offset left edge 1 pixel.*/
			r.SubtractLeft();

			if ( bButtonDown )
				r.Offset(0, -1);

			/* Allocate pen.*/
			VPen xPen;

			/* Create the pen and select into DC.*/
			xPen.Create(	PS_SOLID,
							(m_nButtonType == BUTTON_TYPE_CLOSE)
							? 2
							: 1);

			VGDIObjectSelector os2(dc, xPen);

			/* Create brush if needed.*/
			VBrush xBrush;
			VGDIObjectSelector* posBrush = NULL;

			if ( m_nButtonType != BUTTON_TYPE_CLOSE )
			{
				/* Create and select into DC.*/
				if ( xBrush.CreateSolid(RGB(0,0,0)) )
				{
					posBrush =
						VNEW VGDIObjectSelector(dc, xBrush);
				}
			}

			switch ( m_nButtonType )
			{
				case BUTTON_TYPE_CLOSE:
				{
					/* Draw from upper left to lower right.*/
					dc.MoveTo(r.GetLeft(),	r.GetTop());
					dc.LineTo(r.GetRight(), r.GetBottom());

					/* Draw from upper right to lower left.*/
					dc.MoveTo(r.GetRight(), r.GetTop());
					dc.LineTo(r.GetLeft(),	r.GetBottom());

					break;
				}

				case BUTTON_TYPE_POINT_LEFT:
				{
					r.SubtractRight();

					POINT ptPoly[] =
					{
						{r.GetRight(),	r.GetTop()},
						{r.GetRight(),	r.GetBottom()},
						{r.GetLeft(),	r.GetTop() + r.GetHalfHeight()}
					};

					dc.Polygon(ptPoly, VARRAY_SIZE(ptPoly));
					break;
				}

				case BUTTON_TYPE_POINT_RIGHT:
				{
					r.SubtractRight();

					POINT ptPoly[] =
					{
						{r.GetLeft(),	r.GetTop()},
						{r.GetRight(),	r.GetTop() + r.GetHalfHeight()},
						{r.GetLeft(),	r.GetBottom()}
					};

					dc.Polygon(ptPoly, VARRAY_SIZE(ptPoly));
					break;
				}

				case BUTTON_TYPE_POINT_UP:
				{
					r.SubtractBottom();

					POINT ptPoly[] =
					{
						{r.GetLeft(),					r.GetBottom()},
						{r.GetLeft() + r.GetHalfWidth(),r.GetTop()},
						{r.GetRight(),					r.GetBottom()}
					};

					dc.Polygon(ptPoly, VARRAY_SIZE(ptPoly));
					break;
				}

				case BUTTON_TYPE_POINT_DOWN:
				{
					r.SubtractBottom();

					POINT ptPoly[] =
					{
						{r.GetLeft(),					r.GetTop()},
						{r.GetRight(),					r.GetTop()},
						{r.GetLeft() + r.GetHalfWidth(),r.GetBottom()}
					};

					dc.Polygon(ptPoly, VARRAY_SIZE(ptPoly));
					break;
				}
			}

			/* Delete dynamically allocated selector.*/
			VDELETE_NULL(posBrush);
		}
	}

	/** Draw the horizontal lines of the control.*/
	virtual void			DrawLines(	VDC&			dc,
										VRect const&	rArea)
	{
		/* Initialize 3D rectangle for drawing lines.*/
		V3DRect r;

		/* Get line area rectangle.*/
		if ( GetLinesRect(r) )
		{
			/* Set the colors.*/
			r.GetLeftTopColor() =		RGB(255,255,255);
			r.GetRightBottomColor() =	RGB(127,127,127);

			/* Draw the top line.*/
			r.Draw(dc);

			/* Offset and draw the bottom line.*/
			r.Offset(0, 4);
			r.Draw(dc);
		}
	}

	/** Fill the background of the total control, not just the area. This
	base class implementation fills the background with the brush that
	is registered for the parent window background. Override to fill with
	any color you desire.*/
	virtual void			FillBackground(	VDC&			dc,
											VRect const&	rClient)
	{
		HBRUSH hBR = (HBRUSH)GetClassLong(Parent(), GCL_HBRBACKGROUND);

		if ( hBR )
			dc.FillRect(rClient, hBR);
	}

	/** This VWindowMessageFilter override looks for messags this control
	needs to know about, but where the messages are sent to the parent
	window.*/
	virtual VBOOL			OnMessageFilter(	VWindow&	window,
												VUINT		nMessage,
												VWPARAM		wParam,
												VLPARAM		lParam,
												VLONG&		nResult,
												VBOOL&		bUseResult)
	{
		switch ( nMessage )
		{
			/* Do we need to draw the control now?*/
			case WM_DRAWITEM:
			{
				DRAWITEMSTRUCT* pDIS = (DRAWITEMSTRUCT*)lParam;
				VASSERT(pDIS)

				/* Make sure this is intended for us!*/
				if ( pDIS->hwndItem == GetHandle() )
				{
					/* Get temporary DC object.*/
					VTempDC dc(pDIS->hDC);

					/* Initialize area we draw in.*/
					VRect rArea(pDIS->rcItem);

					/* Fill with parent background brush. This should fill
					the whole control, not just our 12 pixel area.*/
					FillBackground(dc, rArea);

					/* Draw the X button.*/
					DrawButton(	dc,
								rArea,
								(pDIS->itemState & ODS_SELECTED)
								? VTRUE
								: VFALSE);

					/* Draw the lines.*/
					DrawLines(dc, rArea);

					/* We handled the message.*/
					return VFALSE;
				}

				/* We did not handle the message.*/
				return VTRUE;
			}

			/* Look for closing of window to notify.*/
			case WM_COMMAND:
			{
				if (	m_pNotify &&
						(HWND)lParam == GetHandle() )
				{
					/* Call OnClickXButton() virtual function
					in notification object.*/
					m_pNotify->OnClickXButton(VREF_THIS);

					/* We handled the message.*/
					return VFALSE;
				}

				break;
			}
		}

		/* Allow parent to handle message.*/
		return VTRUE;
	}

	/** Embedded Members.*/
	VCloseXButtonNotify*	m_pNotify;
	VBYTE					m_nButtonType;
};

#endif /* VCLOSEXBUTTON*/