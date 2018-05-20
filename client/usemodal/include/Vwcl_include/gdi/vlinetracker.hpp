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

#ifndef VLINETRACKER
#define VLINETRACKER

#include "../vstandard.h"
#include "../structures/vpoint.hpp"
#include "../utility/vcapturedmousetracker.hpp"
#include "../windows/vwindow.hpp"
#include "vclientdc.hpp"
#include "vgdiobjectselector.hpp"
#include "vstockpen.hpp"

/** VLineTracker draws a line between an origin point and the current point
as the user moves the mouse. It gets most of its functionality from the
VCapturedMouseTracker base class, so please see this class for more
information.*/
class VLineTracker : public VCapturedMouseTracker
{
public:
	/** Default constructor initializes VCapturedMouseTracker base class
	and local members.*/
	VLineTracker(	VUINT	nOptions =
								OPTION_DEFAULT,
					VUSHORT	nKeyboardIncrement =
								5)
		: VCapturedMouseTracker(nOptions, nKeyboardIncrement)
		{ ResetMembers(); }

	/** Start tracking line movement in VWindow starting at ptFrom. On
	success, this function will return VTRUE and set the point in ptResult
	to the ending position. The function returns VFALSE on failure. If
	bEraseLineWhenDone is VTRUE, the line drawn between the original and
	ending point will be erased.*/
	VBOOL			Track(	VWindow const&	window,
							VPoint const&	ptFrom,
							VPoint&			ptResult,
							VBOOL			bEraseLineWhenDone =
												VTRUE)
	{
		/* Initialize out parameter.*/
		ptResult = ptFrom;

		/* Get a client DC to use.*/
		VClientDC dc(window);

		if ( !dc.GetHandle() )
			return VFALSE;

		/* Save pointer to DC.*/
		m_pClientDC = &dc;

		/* Call base class to start tracking.*/
		VBOOL bResult =
			VCapturedMouseTracker::Track(window, ptFrom, ptResult);

		/* Draw one last time to erase line? Only if the user moved
		the tracker.*/
		if ( ptFrom != ptResult )
		{
			if ( !bResult || bEraseLineWhenDone )
				Draw(ptFrom, ptResult);
		}

		/* Rest members.*/
		ResetMembers();

		return bResult;
	}

protected:
	/** Draw line using XOR pen. Override can do anything.*/
	virtual void	Draw(	VPoint const& ptFrom,
							VPoint const& ptTo)
	{
		VASSERT(m_pClientDC)
		VASSERT(m_pClientDC->GetHandle())

		/* Set up DC options.*/
		VINT				nOldROP = m_pClientDC->ROP2(R2_NOTXORPEN);
		VStockPen			pen(BLACK_PEN);
		VGDIObjectSelector	os(VREF_PTR(m_pClientDC), pen);

		/* Draw line from origin to current position.*/
		m_pClientDC->MoveTo(ptFrom);
		m_pClientDC->LineTo(ptTo);

		/* Clean up DC.*/
		m_pClientDC->ROP2(nOldROP);
	}

	/** This function gets called by base during tracking.*/
	virtual void	OnTrack(	VWindow const&	window,
								VPoint const&	ptFrom,
								VPoint&			ptCurrent)
	{
		/* Erase old line?*/
		if ( m_bLastToIsValid )
			Draw(ptFrom, m_ptLastTo);

		/* Draw new line.*/
		Draw(ptFrom, ptCurrent);

		/* Save last position.*/
		m_ptLastTo = ptCurrent;

		/* Update flag.*/
		m_bLastToIsValid = VTRUE;
	}

	/** Local function resets internal member variables.*/
	void			ResetMembers()
	{
		m_pClientDC =		NULL;
		m_bLastToIsValid =	VFALSE;
		m_ptLastTo.Empty();
	}

	/** Embedded Members.*/
	VClientDC*		m_pClientDC;
	VPoint			m_ptLastTo;
	VBOOL			m_bLastToIsValid;
};

#endif /* VLINETRACKER*/
