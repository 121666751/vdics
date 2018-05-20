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

#ifndef VCLIENTDC
#define VCLIENTDC

#include "../vstandard.h"
#include "../windows/vwindow.hpp"
#include "vdcwithwindow.hpp"

/** VClientDC wraps Device Contexts during NON OnPaint() and WM_PAINT
messages. Use VPaintDC for drawing during OnPaint() and WM_PAINT messages
only, and never use anything other than VPaintDC in response to OnPaint()
or WM_PAINT.*/
class VClientDC : public VDCWithWindow
{
public:
	/** Standard constructor initializes class by getting the DC for the
	window via the GetDC() function.*/
	VClientDC(VWindow const& window)
		: VDCWithWindow(window)
	{
		VASSERT(window.GetSafeWindow())
		m_hDC = GetDC(window.GetHandle());
		VASSERT(m_hDC)
	}
};

#endif /* VCLIENTDC*/
