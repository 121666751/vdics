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

#ifndef VBUFFEREDDC
#define VBUFFEREDDC

#include "../vstandard.h"
#include "../structures/vrect.hpp"
#include "vbitmap.hpp"
#include "vdc.hpp"

/** VBufferedDC is used to draw in a double-buffered manner. This object is
attached to another DC during creation. All drawing and rendering then occurs
using this object. When destructed, this class will use the BitBlt() method
to transfer the drawn contents back to the original device context. This helps
to reduce flicker by having the relatively slow drawing code render to an
offscreen device context and then draw to the real DC with a single function
call. If for any reason the initialization of this class fails, drawing will
continue to operate (without buffering) because this class will revert to
being a simple temporary wrapper for the device context passed to one of the
constructors. This alleviates the need to have code to guard against GDI
failures when allocating buffered devices. Basically, if you could draw to the
DC passed to the constructor, you still can through this class, even if the
double-buffered mechanism is not working.

NOTE: It is expected that calling code will completely fill the rectangle
passed to the constructor. By default, a double-buffered device context is
filled with black. To achieve correct results, the entire rectangle must
be drawn to completion.*/
class VBufferedDC : public VDC
{
public:
	/** Constructor must be initialized with an existing device context.
	This device context will be used as the "real" DC. Any drawing that
	occurs using this class will ultimately be rendered to the DC identified
	by hDC. The rectangle of the real DC must also be given, and must not be
	empty.*/
	VBufferedDC(HDC				hDC,
				VRect const&	r)
		{ Init(hDC, r); }

	/** Constructor must be initialized with an existing device context
	object. This device context will be used as the "real" DC. Any drawing
	that occurs using this class will ultimately be rendered to the DC
	identified by dc. The rectangle of the real DC must also be given, and
	must not be empty.*/
	VBufferedDC(VDC const&		dc,
				VRect const&	r)
		{ Init(dc.GetHandle(), r); }

	/** Virtual destructor BitBlt()'s the entire contents of the
	double-buffered DC to the read device context passed into one of the
	constructors.*/
	virtual ~VBufferedDC()
	{
		if ( m_hRealDC && GetHandle() && m_hRealDC != GetHandle() )
		{
			/* Blit double-buffered DC back to original DC to
			complete drawing.*/
			::BitBlt(	m_hRealDC,
						VRECT_BREAK_WIDTH_HEIGHT(m_rReal),
						GetHandle(),
						0,
						0,
						SRCCOPY);

			/* Select old bitmap into our DC, so the new one wrapped by
			m_Bitmap can be deleted.*/
			if ( m_hOldBitmap )
				SelectObject(m_hOldBitmap);
		}
	}

	/** Get the "real" device context. This is the DC that was passed to
	one of the constructors, not the offscreen DC.*/
	HDC		GetRealDC() const
		{ return m_hRealDC; }

protected:
	/** Internal function to initialize the object.*/
	void	Init(	HDC				hDC,
					VRect const&	r)
	{
		/* This must be known!*/
		VASSERT(hDC);

		/* The rectangle cannot be empty.*/
		VASSERT(r.IsNotEmpty())
		
		/* Initialize members.*/
		m_hRealDC =		hDC;
		m_rReal =		r;
		m_hOldBitmap =	NULL;
		
		/* Check parameters.*/
		if ( m_hRealDC && m_rReal.IsNotEmpty() )
		{
			/* Create the memory DC.*/
			HDC hMemDC = CreateCompatibleDC(m_hRealDC);

			if ( hMemDC )
			{
				/* Assume ownership now.*/
				GetHandle() =	hMemDC;
				m_bFreeHDC =	VTRUE;

				/* Create bitmap.*/
				if ( m_Bitmap.SetHandle(CreateCompatibleBitmap(
					m_hRealDC,
					m_rReal.GetWidth(),
					m_rReal.GetHeight())) )
				{
					/* Select bitmap.*/
					m_hOldBitmap = SelectObject(m_Bitmap.GetHandle());
				}
				/* Release our DC on failure.*/
				else
					Free();
			}
		}

		/* On failure, we simply wrap hDC.*/
		if ( !GetHandle() )
		{
			GetHandle() =	m_hRealDC;
			m_bFreeHDC =	VFALSE;
		}
	}

	/** Embedded Members.*/
	VBitmap	m_Bitmap;
	VRect	m_rReal;
	HGDIOBJ	m_hOldBitmap;
	HDC		m_hRealDC;
};

#endif /* VBUFFEREDDC*/
