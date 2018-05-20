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

#ifndef VWINDOWDC
#define VWINDOWDC

#include "../vstandard.h"
#include "../windows/vwindow.hpp"
#include "vdcwithwindow.hpp"

/** VWindowDC wraps Device Contexts (DC's) for drawing in non-client area
of window which includes the window frame and caption areas.*/
class VWindowDC : public VDCWithWindow
{
public:
	/** Standard constructor initializes class by getting the DC for the
	window via the GetWindowDC() function. If the window referenced by
	window is not associated with a Windows Window (VWindow::GetHandle()
	returns NULL), the device context wrapped by this object will be for
	the whole screen, not the window parameter.*/
	VWindowDC(VWindow const& window)
		: VDCWithWindow(window)
	{
		m_hDC = GetWindowDC(window.GetHandle());
		VASSERT(m_hDC)
	}
};


#endif /* VWINDOWDC*/