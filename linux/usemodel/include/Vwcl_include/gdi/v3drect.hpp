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

#ifndef V3DRECT
#define V3DRECT

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../structures/vrect.hpp"

#ifndef VWCL_NO_VWINDOW
	#include "../windows/vwindow.hpp"
#endif

#include "vdc.hpp"
#include "vbrush.hpp"
#include "vgdiobjectselector.hpp"
#include "vpen.hpp"
#include "vstockpen.hpp"

/** V3DRect is used to draw 3 Dimensional rectangles.*/
class V3DRect : public VRect
{
public:
	/** Construct with existing X, Y, and CX, CY coordinates, or default of
	0, 0, 0, 0. Can also be initialized with class options (which default
	to 0), the rectangle fill color (defaults to gray, the pen color to use
	on the left and top edges of the rectangle (defaults to dark gray), the
	pen color to use on the right and bottom edges of the rectangle
	(defaults to white), and the border width (defaults to 1 pixel).*/
	V3DRect(	VLONG		nx =
								0,
				VLONG		ny =
								0,
				VLONG		ncx =
								0,
				VLONG		ncy =
								0,
				VUINT		nOptions =
								0,
				COLORREF	crFill =
								RGB(192,192,192),
				COLORREF	crLeftTop =
								RGB(127,127,127),
				COLORREF	crRightBottom =
								RGB(255,255,255),
				VUINT		nBorderWidth =
								1)
		: VRect(nx, ny, ncx, ncy)
	{
		ResetMembers(	nOptions,
						crFill,
						crLeftTop,
						crRightBottom,
						nBorderWidth);
	}

	/** Construct with existing VRect object. Can also be initialized with
	class options (which default to 0), the rectangle fill color (defaults
	to gray, the pen color to use on the left and top edges of the rectangle
	(defaults to dark gray), the pen color to use on the right and bottom
	edges of the rectangle (defaults to white), and the border width
	(defaults to 1 pixel).*/
	V3DRect(	VRect const&	r,
				VUINT			nOptions =
									0,
				COLORREF		crFill =
									RGB(192,192,192),
				COLORREF		crLeftTop =
									RGB(127,127,127),
				COLORREF		crRightBottom =
									RGB(255,255,255),
				VUINT			nBorderWidth =
									1)
		: VRect(r)
	{
		ResetMembers(	nOptions,
						crFill,
						crLeftTop,
						crRightBottom,
						nBorderWidth);
	}

	/** Construct with existing RECT structure. Can also be initialized with
	class options (which default to 0), the rectangle fill color (defaults
	to gray, the pen color to use on the left and top edges of the rectangle
	(defaults to dark gray), the pen color to use on the right and bottom
	edges of the rectangle (defaults to white), and the border width
	(defaults to 1 pixel).*/
	V3DRect(	RECT const&	r,
				VUINT		nOptions =
								0,
				COLORREF	crFill =
								RGB(192,192,192),
				COLORREF	crLeftTop =
								RGB(127,127,127),
				COLORREF	crRightBottom =
								RGB(255,255,255),
				VUINT		nBorderWidth =
								1)
		: VRect(r)
	{
		ResetMembers(	nOptions,
						crFill,
						crLeftTop,
						crRightBottom,
						nBorderWidth);
	}

	/** Options for this class and known VWCL derivatives. Defaults to 0 (no
	options). The class options can be set during construction, or later
	with the GetOptions() function.*/
	enum
	{
		/** Draws a 1 pixel black border first around rectangle.*/
		OPTION_DRAW_BLACK_BORDER =			0x0001,
		/** Shrinks the rectangle as it draws in Draw().*/
		OPTION_SHRINK_RECT_ON_DRAW =		0x0002,
		/** Does not fill the rectangle during Draw().*/
		OPTION_NO_FILL =					0x0004,
		/** Does not draw the left side of the rectangle in Draw().*/
		OPTION_NO_DRAW_LEFT =				0x0008,
		/** Does not draw the top side of the rectangle in Draw().*/
		OPTION_NO_DRAW_TOP =				0x0010,
		/** Does not draw the right side of the rectangle in Draw().*/
		OPTION_NO_DRAW_RIGHT =				0x0020,
		/** Does not draw the bottom side of the rectangle in Draw().*/
		OPTION_NO_DRAW_BOTTOM =				0x0040,
		/** Draw a border in the VGauge class.*/
		OPTION_GAUGE_WANT_BORDER =			0x0080,
		/** Draw a 3D border in the VGauge class.*/
		OPTION_GAUGE_WANT_3D_BORDER =		0x0100,
		/** Gauge oriented vertically in the VGauge class.*/
		OPTION_GAUGE_ORIENT_VERTICAL =		0x0200,
		/** Gauge oriented horizontally in the VGauge class.*/
		OPTION_GAUGE_ORIENT_HORIZONTAL =	0x0400,
		/** Causes a horizontal gauage to draw progress from right to left.*/
		OPTION_GAUGE_RIGHT_TO_LEFT =		0x0800,
		/** Causes a vertical gauage to draw progress from top to bottom.*/
		OPTION_GAUGE_TOP_TO_BOTTOM =		0x0800,
		/** Default gauge options.*/
		OPTION_GAUGE_DEFAULT =				OPTION_NO_FILL |
											OPTION_GAUGE_WANT_3D_BORDER |
											OPTION_GAUGE_ORIENT_HORIZONTAL,
	};

	/** Draw and shrink rectangle as needed.*/
	virtual void	Draw(VDC& dc)
	{
		VASSERT(dc.GetHandle())

		/* Guard against wasting time.*/
		if ( IsEmpty() )
			return;

		/* Save old rectangle.*/
		VRect rOld(VREF_THIS);

		if ( m_Options.IsNotSet(OPTION_NO_FILL) )
			Fill(dc);

		VBOOL bBlackBorder = m_Options.IsSet(OPTION_DRAW_BLACK_BORDER);

		/* If we should draw a black border, do that now.*/
		if ( bBlackBorder )
		{
			VStockPen blackPen(BLACK_PEN);
			VGDIObjectSelector os(dc, blackPen);
			dc.RectangleEx(VREF_THIS, VDC::RECTANGLE_EX_DRAW_ALL);

			/* Adjust rectangle.*/
			Deflate(1, 1);
		}

		/* If m_nBorderWidth is 0, we won't draw anymore.*/
		if ( m_nBorderWidth )
		{
			/* Determine drawing options for VDC::RectangleEx().*/
			VUINT nDCOptionsLeftTop =		0;
			VUINT nDCOptionsRightBottom =	0;

			if ( m_Options.IsNotSet(OPTION_NO_DRAW_LEFT) )
				nDCOptionsLeftTop |= VDC::RECTANGLE_EX_DRAW_LEFT;

			if ( m_Options.IsNotSet(OPTION_NO_DRAW_TOP) )
				nDCOptionsLeftTop |= VDC::RECTANGLE_EX_DRAW_TOP;

			if ( m_Options.IsNotSet(OPTION_NO_DRAW_RIGHT) )
				nDCOptionsRightBottom |= VDC::RECTANGLE_EX_DRAW_RIGHT;

			if ( m_Options.IsNotSet(OPTION_NO_DRAW_BOTTOM) )
				nDCOptionsRightBottom |= VDC::RECTANGLE_EX_DRAW_BOTTOM;

			/* Create pens based on options. Set options to 0 if pen
			fails to create.*/
			VPen leftTopPen;
			VPen rightBottomPen;

			if ( nDCOptionsLeftTop )
			{
				if ( !leftTopPen.Create(PS_SOLID, 0, m_crLeftTop) )
					nDCOptionsLeftTop = 0;
			}

			if ( nDCOptionsRightBottom )
			{
				if ( !rightBottomPen.Create(PS_SOLID, 0, m_crRightBottom) )
					nDCOptionsRightBottom = 0;
			}

			/* Correct coordinates if certain options are set. This corrects
			an issue about how this object interfaces with the VDC function
			RectangleEx(). Without this hack, the borders would be drawn
			one pixel off on the left bottom side, and one pixel off on the
			right top side.*/
			if (	m_Options.IsSet(OPTION_NO_DRAW_TOP) ||
					m_Options.IsSet(OPTION_NO_DRAW_BOTTOM) )
			{
				AddBottom(1);
				SubtractTop(1);
			}

			/* Anything to draw?*/
			if ( nDCOptionsLeftTop || nDCOptionsRightBottom )
			{
				for ( VUINT i = 0; i < m_nBorderWidth; i++ )
				{
					/* Draw left and top sides?*/
					if ( nDCOptionsLeftTop )
					{
						/* Select pen and draw.*/
						VGDIObjectSelector os(dc, leftTopPen);
						dc.RectangleEx(	VREF_THIS,
										VDC::RECTANGLE_EX_3D_STYLE |
										nDCOptionsLeftTop);
					}

					/* Draw right and bottom sides?*/
					if ( nDCOptionsRightBottom )
					{
						/* Select pen and draw.*/
						VGDIObjectSelector os(dc, rightBottomPen);
						dc.RectangleEx(	VREF_THIS,
										VDC::RECTANGLE_EX_3D_STYLE |
										nDCOptionsRightBottom);
					}

					/* Shrink the rect by what we drew.*/
					if ( nDCOptionsLeftTop & VDC::RECTANGLE_EX_DRAW_LEFT )
						AddLeft();

					if ( nDCOptionsLeftTop & VDC::RECTANGLE_EX_DRAW_TOP )
						AddTop();

					if ( nDCOptionsRightBottom & VDC::RECTANGLE_EX_DRAW_RIGHT )
						SubtractRight(1);

					if ( nDCOptionsRightBottom & VDC::RECTANGLE_EX_DRAW_BOTTOM )
						SubtractBottom();

					/* If rect is empty, break out now.*/
					if ( IsEmpty() )
						break;
				}
			}
		}

		/* Should we re-inflate the rect?*/
		if ( m_Options.IsNotSet(OPTION_SHRINK_RECT_ON_DRAW) )
			Set(rOld);
	}

	/** Fill the rectangle with the background color. This function does not
	check the OPTION_NO_FILL option flag. Calling code should perform this
	check as needed. In addition, this function fills the entire rectangle
	with the fill color, ignoring other options. The rectangle should be
	sized accordingly before calling this function.*/
	void			Fill(VDC& dc) const
	{
		VASSERT(dc.GetHandle())

		VBrush fillBrush;

		if ( fillBrush.CreateSolid(m_crFill) )
			fillBrush.FillRect(dc, VREF_THIS);
	}

	/** Return are reference to the border width value.*/
	VUINT&			GetBorderWidth() const
		{ return (VUINT&)m_nBorderWidth; }

	/** Return a reference to the fill color value.*/
	COLORREF&		GetFillColor() const
		{ return (COLORREF&)m_crFill; }

	/** Return a reference to the left and top color value.*/
	COLORREF&		GetLeftTopColor() const
		{ return (COLORREF&)m_crLeftTop; }

	/** Return a reference to the options object.*/
	VBitmask&		GetOptions() const
		{ return (VBitmask&)m_Options; }

	/** Return a reference to the right and bottom color value.*/
	COLORREF&		GetRightBottomColor() const
		{ return (COLORREF&)m_crRightBottom; }

	/** Given a window to paint on, redraw the rectangle.*/
	#ifndef VWCL_NO_VWINDOW
		void		Refresh(VWindow const& window)
			{ window.InvalidateRectUpdate(VREF_THIS, VFALSE); }
	#endif

protected:
	/** Local function sets internal member variables.*/
	void			ResetMembers(	VUINT		nOptions,
									COLORREF	crFill,
									COLORREF	crLeftTop,
									COLORREF	crRightBottom,
									VINT		nBorderWidth)
	{
		m_Options =			nOptions;
		m_crFill =			crFill;
		m_crLeftTop =		crLeftTop;
		m_crRightBottom =	crRightBottom;
		m_nBorderWidth =	nBorderWidth;
	}

	/** Embedded Members.*/
	VBitmask		m_Options;
	COLORREF		m_crFill;
	COLORREF		m_crLeftTop;
	COLORREF		m_crRightBottom;
	VUINT			m_nBorderWidth;
};

#endif /* V3DRECT*/
