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

#ifndef VPAINTDC
#define VPAINTDC

#include "../vstandard.h"
#include "../windows/vwindow.hpp"
#include "vdcwithwindow.hpp"

/** VPaintDC wraps Device Contexts (DC's) during OnPaint() and WM_PAINT
messages only. Use VPaintDC for drawing during OnPaint() and WM_PAINT
messages only, and never use anything other than VPaintDC in response to
OnPaint() or WM_PAINT.*/
class VPaintDC : public VDCWithWindow
{
public:
	/** Standard constructor initializes class by getting the DC for the
	window via the BeginPaint() function.*/
	VPaintDC(VWindow const& window)
		: VDCWithWindow(window)
	{
		VZEROSTRUCT(m_ps);
		m_hDC = BeginPaint(window.GetHandle(), &m_ps);
		VASSERT(m_hDC)
	}

	/** Virtual destructor verifies local override of Free() is performed.*/
	virtual ~VPaintDC()
		{ Free(); }

	/** Override calls EndPaint() to release DC, does not DeleteDC() or
	ReleaseDC(), which is invalid when handling OnPaint() and WM_PAINT.*/
	virtual void	Free()
	{
		if ( m_bFreeHDC && m_hDC )
		{
			VASSERT(m_pWindow)
			EndPaint(m_pWindow->GetHandle(), &m_ps);
			VZEROSTRUCT(m_ps);
			m_hDC = NULL;
		}
	}

	/** Return a reference to the internal PAINTSTRUCT structure.*/
	PAINTSTRUCT&	GetPAINTSTRUCT() const
		{ return (PAINTSTRUCT&)m_ps; }

	/** Return a pointer to the internal PAINTSTRUCT structure.*/
	PAINTSTRUCT*	GetPointer() const
		{ return (PAINTSTRUCT*)&m_ps; }

protected:
	/** Embedded Members.*/
	PAINTSTRUCT		m_ps;
};

#endif /* VPAINTDC*/
