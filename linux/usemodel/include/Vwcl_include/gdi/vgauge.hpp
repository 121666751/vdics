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

Primary Author of this source code file:	Todd Osborne
											(todd@vwcl.org)
Other Author(s) of this source code file:	Daniel Halan
											(daniel@image-research.se)
*/

#ifndef VGAUGE
#define VGAUGE

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../structures/vrect.hpp"
#include "vdc.hpp"
#include "v3drect.hpp"

/** VGauge fills a rect with color as progress is made. This class assumes
a value of 0 means the gauge should not display any filled area, and a
value of 100 is a completely filled gauage. This class uses a floating
point calculation to determine the filling of the gauge, and is thus very
accurate. The class is well suited for a progress bar, and supports calling
the OnDrawComplete() virtual function to allow drawing of text or other
objects after the Draw() function completes.

The gauge has two basic methods of use. In the first method, the filled
part of the gauge is determined by a percentage value of 0 to 100. You
specifiy the current percentage using the Percent() function and the class
fills the rectangle with that amount of filled area.

The other method is a wrapping of the first. In this method, you specify
the minimum and maximum values in a range using the GetMin(), GetMax() or
SetMinMax() functions, and the percentage is calulated during the Position()
call. This allows the gauge to function relative to some other range, not
simply the rectangle size.

You can use either method and intermix the two, as they both keep the class
internal members in a consistent state. VGauge is not derived from V3DRect,
but uses it extensively. You can call GetRect() to return a reference to the
internal 3D rectangle object and modify it as needed.*/
class VGauge
{
public:
	/** Default constructor can be initialized with V3DRect class options
	(which default to OPTION_GAUGE_DEFAULT), the rectangle fill color
	(defaults to gray, the pen color to use on the left and top edges of
	the rectangle (defaults to dark gray), the pen color to use on the right
	and bottom edges of the rectangle (defaults to white), the border width
	(defaults to 1 pixel), and the gauage fill color (defaults to blue).*/
	VGauge(	VUINT		nOptions =
							V3DRect::OPTION_GAUGE_DEFAULT,
			COLORREF	crFill =
							RGB(192,192,192),
			COLORREF	crLeftTop =
							RGB(127,127,127),
			COLORREF	crRightBottom =
							RGB(255,255,255),
			VUINT		nBorderWidth =
							1,
			COLORREF	crGaugeColor =
							RGB(0,0,255))
	{
		/* Initialize V3DRect object.*/
		m_r3D.GetFillColor() =			crFill;
		m_r3D.GetLeftTopColor() =		crLeftTop;
		m_r3D.GetRightBottomColor() =	crRightBottom;
		m_r3D.GetBorderWidth() =		nBorderWidth;
		
		/* Initialize class members.*/
		ResetMembers(crGaugeColor);
	}

	/** Draw the gauge.*/
	virtual void	Draw(VDC& dc)
	{
		/* Avoid wasting time.*/
		if ( m_r3D.IsNotEmpty() )
		{
			/* Save current rectangle.*/
			VRect rOriginal(m_r3D);

			/* Save current options.*/
			m_r3D.GetOptions().Save();

			/* Verify proper options bits are set.*/
			m_r3D.GetOptions().Append(	V3DRect::OPTION_SHRINK_RECT_ON_DRAW |
										V3DRect::OPTION_NO_FILL);

			/* Draw (this will shrink the rect).*/
			m_r3D.Draw(dc);

			/* Initialize rectangles now.*/
			VRect rInsideBorders(m_r3D);
			VRect rFilledArea;
			VRect rUnFilledArea;

			/* At 0%?*/
			if ( m_nPercent )
			{
				/* Initialize filled area.*/
				rFilledArea = rInsideBorders;

				/* At 100%? If yes, the gauge will consume the
				entire remaining rectangle.*/
				if ( m_nPercent != 100 )
				{
					if ( m_r3D.GetOptions().IsSet(
						V3DRect::OPTION_GAUGE_ORIENT_VERTICAL) )
					{
						VFLOAT nPixelsPerTick =
							(VFLOAT)((VFLOAT)rFilledArea.GetHeight()) / 100.0f;

						if ( m_r3D.GetOptions().IsSet(
							V3DRect::OPTION_GAUGE_TOP_TO_BOTTOM) )
							rFilledArea.GetBottom() =
								VMIN(rFilledArea.GetTop()
								+ ((VLONG)(nPixelsPerTick
								* (VFLOAT)m_nPercent)),
								m_r3D.GetBottom());
						else
							rFilledArea.GetTop() =
								VMAX(rFilledArea.GetBottom()
								- (VLONG)(nPixelsPerTick
								* (VFLOAT)m_nPercent),
								m_r3D.GetTop());
					}
					else
					{
						VFLOAT nPixelsPerTick =
							(VFLOAT)((VFLOAT)rFilledArea.GetWidth()) / 100.0f;

						if ( m_r3D.GetOptions().IsSet(
							V3DRect::OPTION_GAUGE_RIGHT_TO_LEFT) )
							rFilledArea.GetLeft() =
								VMAX(rFilledArea.GetRight()
								- (VLONG)(nPixelsPerTick
								* (VFLOAT)m_nPercent),
								m_r3D.GetLeft());
						else
							rFilledArea.GetRight() =
								VMIN(rFilledArea.GetLeft()
								+ (VLONG)(nPixelsPerTick
								* (VFLOAT)m_nPercent),
								m_r3D.GetRight());
					}
				}

				/* Display filled rectangle?*/
				if ( rFilledArea.IsNotEmpty() )
				{
					VBrush brush;

					if ( brush.CreateSolid(m_crGaugeColor) )
						brush.FillRect(dc, rFilledArea);
				}
			}

			/* Now fill the area we did not fill with the gauge.*/
			if ( rFilledArea.IsEmpty() )
			{
				m_r3D.Fill(dc);
				rUnFilledArea = rInsideBorders;
			}
			else if (	rUnFilledArea.Subtract(rInsideBorders, rFilledArea) &&
						rUnFilledArea.IsNotEmpty() )
			{
				m_r3D.Set(rUnFilledArea);
				m_r3D.Fill(dc);
			}

			/* Restore user options.*/
			m_r3D.GetOptions().Restore();

			/* Reset original rectangle, or if we should shrink on draw,
			set to rInsideBorders.*/
			if ( m_r3D.GetOptions().IsSet(
				V3DRect::OPTION_SHRINK_RECT_ON_DRAW) )
				m_r3D.Set(rInsideBorders);
			else
				m_r3D.Set(rOriginal);

			/* Call the OnDrawComplete() virtual function.*/
			OnDrawComplete(dc, rInsideBorders, rFilledArea, rUnFilledArea);
		}
	}

	/** Return a reference to the gauge color. (Defaults to RGB(0,0,255)
	which is blue).*/
	COLORREF&		GetGaugeColor() const
		{ return (COLORREF&)m_crGaugeColor; }

	/** Return a reference to the specified minimum value.*/
	VUINT&			GetMin() const
		{ return (VUINT&)m_nMinPos; }

	/** Return a reference to the specified maximum value.*/
	VUINT&			GetMax() const
		{ return (VUINT&)m_nMaxPos; }

	/** Return a reference to the internal V3DRect use for drawing. You can
	customize this object to suit your needs. It must contain a valid
	rectangle for drawing to work properly. If this rectangle is changed, you
	should call the V3DRect::Refresh() method to update the display.*/
	V3DRect&		GetRect() const
		{ return (V3DRect&)m_r3D; }

	/** Set the gauge percentage (0 to 100) is valid. If nPercent is over
	100, it will be set to 0. If pDC is not NULL, the gauge will be redrawn
	to reflect the change.*/
	void			Percent(VBYTE	nPercent,
							VDC*	pDC =
										NULL)
	{
		/* Do nothing?*/
		if ( nPercent != m_nPercent )
		{
			/* Reset?*/
			m_nPercent = (nPercent > 100) ? 0 : nPercent;

			/* Calculate position.*/
			m_nPosition =
				(VUINT)(((m_nMaxPos - m_nMinPos) *
				((VDOUBLE)m_nPercent /
				(VDOUBLE)100)));

			if ( pDC )
				Draw(VREF_PTR(pDC));
		}
	}

	/** Return the current gauge percentage.*/
	VBYTE			Percent() const
		{ return m_nPercent; }

	/** Set the position value, which is in the range specified
	by SetMinMax().*/
	void			Position(	VUINT	nPosition,
								VDC*	pDC =
											NULL)
	{
		if ( nPosition != m_nPosition )
		{
			if ( (nPosition < m_nMinPos) || (nPosition > m_nMaxPos) )
			{
				m_nPercent =	0;
				m_nPosition =	m_nMinPos;
			}
			else
			{
				m_nPosition = nPosition;
				m_nPercent =
					(nPosition)
					? (VUINT)(((VDOUBLE)(m_nPosition - m_nMinPos) /
					(VDOUBLE)(m_nMaxPos - m_nMinPos)) * 100)
					: 0;
			}

			if ( pDC )
				Draw(VREF_PTR(pDC));
		}
	}

	/** Return the current position value.*/
	VUINT			Position() const
		{ return m_nPosition; }

	/** Sets the minimum and maximum values. Used only when the Position()
	function is used.*/
	void			SetMinMax(	VUINT nMinPos,
								VUINT nMaxPos)
	{
		m_nMinPos = nMinPos;
		m_nMaxPos = nMaxPos;
	}

	/** Step the percentage by 1. If the percentage is over 100, it will be
	set to 0. If pDC is not NULL, the gauge will be redrawn to reflect the
	change.*/
	void			StepIt(VDC* pDC = NULL)
		{ Percent(m_nPercent + 1, pDC); }

protected:
	/** This virtual function is called after the drawing of the rectangle
	and gauge is complete. The rectangles passed to this function are
	defined as follows:

	rInsideBoreders - The area of the rectangle inside the border area
	defined by the base V3DRect class.

	rFilledArea - The portion of the rectangle consumed by the filled
	region, based on percentage or position.

	rUnFilledArea - The area not filled by rFilledArea.

	This is an appropriate time to draw custom text or other objects into
	the available rectangle area.*/
	virtual void	OnDrawComplete(	VDC&	dc,
									VRect&	rInsideBorders,
									VRect&	rFilledArea,
									VRect&	rUnFilledArea)
		{;}

	/** Local function to initialize members.*/
	void			ResetMembers(COLORREF crGaugeColor)
	{
		m_crGaugeColor =	crGaugeColor;
		m_nPercent =		0;
		m_nPosition =		0;
		m_nMinPos =			0;
		m_nMaxPos =			100;
	}

	/** Embedded Members.*/
	V3DRect			m_r3D;
	COLORREF		m_crGaugeColor;
	VBYTE			m_nPercent;
	VUINT			m_nPosition;
	VUINT			m_nMinPos;
	VUINT			m_nMaxPos;
};

#endif /* VGAUGE*/
