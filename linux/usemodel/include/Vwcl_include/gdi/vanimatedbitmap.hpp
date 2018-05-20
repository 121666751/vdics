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

Primary Author of this source code file:	Todd Osborne (todd@vwcl.org)
Other Author(s) of this source code file:	Helmut Pharo (pharo@onlinehome.de)
*/

#ifndef VANIMATEDBITMAP
#define VANIMATEDBITMAP

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../structures/vrect.hpp"
#include "vdc.hpp"
#include "vbitmap.hpp"
#include "vgdiobjectselector.hpp"

/** VAnimatedBitmap displays portions of a bitmap like a cartoon. This is a
very simple and lightweight class. The Win32 Common Controls provide a
better implementation of an animated bitmap, however this class exists for
cases where the common controls cannot be used, or when you desire a
simpler implementation. This class does not perform any type of palette
management, so it is best used for bitmaps with 16 colors or less. More
colors will work on video drivers that support more than 256 colors, but
this should not be assumed. The drawing code will shrink or stretch the
bitmap as needed to fill the rectangle which is set by using the VRect
object returned from GetRect(). If the size of the rectangle is exactly the
size of a frame in the bitmap, no stretching or shrinking will be performed.
This provides better performance.*/
class VAnimatedBitmap
{
public:
	/** Standard constructor can be initialized with class options and frame
	size and count. Before use however, the bitmap must be associated with
	the object by initializing it through the VBitmap class which can be
	returned by calling GetBitmap().*/
	VAnimatedBitmap(	VUINT nOptions =
								OPTION_DEFAULT,
						VUINT nFrameSizeX =
								32,
						VUINT nFrameSizeY =
								32,
						VLONG nFrameCount =
								0)
	{
		/* Initialize.*/
		m_nCurrentFrame =		0;
		m_nFrameIncrementer =	1;
		m_nFrameCount =			nFrameCount;
		m_sizeFrame.cx =		nFrameSizeX;
		m_sizeFrame.cy =		nFrameSizeY;
		m_Options.Bits(nOptions);
	}

	/** Virtual destructor does nothing.*/
	virtual ~VAnimatedBitmap()
		{;}

	/** Options that can be applied by using the GetOptions() function:

	OPTION_BITMAP_FRAMES_LEFT_TO_RIGHT - Means the bitmap associated with
	this object contains images read from left to right.

	OPTION_BITMAP_FRAMES_TOP_TO_BOTTOM - Means the bitmap associated with
	this object contains images read from top to bottom.

	OPTION_STEP_IT_REVERSE_AT_END - Causes the animation to be played
	backwards after the last frame is drawn.*/
	enum	{		OPTION_BITMAP_FRAMES_LEFT_TO_RIGHT =	0x0001,
					OPTION_BITMAP_FRAMES_TOP_TO_BOTTOM =	0x0002,
					OPTION_STEP_IT_REVERSE_AT_END =			0x0004,
					OPTION_DEFAULT =	OPTION_BITMAP_FRAMES_LEFT_TO_RIGHT |
										OPTION_STEP_IT_REVERSE_AT_END,
			};

	/** Return a reference to the current frame in the animation.*/
	VLONG&			CurrentFrame()
		{ return m_nCurrentFrame; }

	/** Draw the current frame in the animation to the device context held
	in dc.*/
	void			Draw(VDC& dc)
	{
		VASSERT(dc.GetHandle())
		VASSERT(m_Bitmap.GetHandle())

		/* Don't draw in empty rectangle.*/
		if ( m_Bitmap.GetHandle() && dc.GetHandle() && m_Rect.IsNotEmpty() )
		{
			VDC memDC;

			if ( dc.CreateCompatible(memDC) )
			{
				VGDIObjectSelector os(memDC, m_Bitmap);

				/* If rect is not equal to frame size, stretch or shrink
				to fit.*/
				if (	m_Rect.GetWidth() != m_sizeFrame.cx ||
						m_Rect.GetHeight() != m_sizeFrame.cy )
				{
					if ( m_Options.IsSet(OPTION_BITMAP_FRAMES_LEFT_TO_RIGHT) )
					{
						dc.StretchBlt(	VRECT_BREAK_WIDTH_HEIGHT(m_Rect),
										memDC,
										m_sizeFrame.cx * m_nCurrentFrame,
										0,
										m_sizeFrame.cx,
										m_sizeFrame.cy);
					}
					else
					{
						dc.StretchBlt(	VRECT_BREAK_WIDTH_HEIGHT(m_Rect),
										memDC,
										0,
										m_sizeFrame.cy * m_nCurrentFrame,
										m_sizeFrame.cx,
										m_sizeFrame.cy);
					}
				}
				else
				{
					if ( m_Options.IsSet(OPTION_BITMAP_FRAMES_LEFT_TO_RIGHT) )
					{
						dc.BitBlt(	VRECT_BREAK_WIDTH_HEIGHT(m_Rect),
									memDC,
									m_sizeFrame.cx * m_nCurrentFrame,
									0);
					}
					else
					{
						dc.BitBlt(	VRECT_BREAK_WIDTH_HEIGHT(m_Rect),
									memDC,
									0,
									m_sizeFrame.cy * m_nCurrentFrame);
					}
				}
			}
		}
	}

	/** Return a reference to the number of frames in the animation.*/
	VLONG&			GetFrameCount()
		{ return m_nFrameCount; }

	/** Return a reference to the current frame size (Default is 32 x 32).*/
	SIZE&			GetFrameSize()
		{ return m_sizeFrame; }

	/** Return a reference to the bitmap which contains the animation
	sequence to show.*/
	VBitmap&		GetBitmap()
		{ return m_Bitmap; }

	/** Return a reference to the options object.*/
	VBitmask&		GetOptions()
		{ return m_Options; }

	/** Return a reference to the rectangle object. This rectangle governs
	the area the animation is allowed to be drawn. If the size of this
	rectangle is exactly the size of a frame in the bitmap, no stretching
	or shrinking will be performed. This provides better performance.*/
	VRect&			GetRect()
		{ return m_Rect; }

	/** Step the current frame forward or backwards, and if pDC is not NULL,
	draws the new frame.*/
	void			StepIt(VDC* pDC = NULL)
	{
		/* Draw first otherwise the first frame doesn't get drawn, since
		m_nCurrentFrame is initiailized to 0.*/

		if ( pDC )
			Draw(VREF_PTR(pDC));

		VLONG nNext = m_nCurrentFrame + m_nFrameIncrementer;

		if ( nNext >= m_nFrameCount )
		{
			if (m_Options.IsSet(OPTION_STEP_IT_REVERSE_AT_END))
				m_nFrameIncrementer *= -1;
			/* Wrap backwards.*/
			else
				m_nCurrentFrame -= m_nFrameCount;
		}
		else if ( nNext < 0)
		{
			if (m_Options.IsSet(OPTION_STEP_IT_REVERSE_AT_END))
				m_nFrameIncrementer *= -1;
			/* Wrap Forward.*/
			else
				m_nCurrentFrame += m_nFrameCount;
		}

		/* Factored out from each of the previous cases.*/
		m_nCurrentFrame += m_nFrameIncrementer;
	}

protected:
	/** Embedded Members.*/
	VBitmap			m_Bitmap;
	VBitmask		m_Options;
	VRect			m_Rect;
	SIZE			m_sizeFrame;
	VLONG			m_nFrameCount;
	VLONG			m_nCurrentFrame;
	VLONG			m_nFrameIncrementer;
};

#endif /* VANIMATEDBITMAP*/
