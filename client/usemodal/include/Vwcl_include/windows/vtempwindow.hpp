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

#ifndef VTEMPWINDOW
#define VTEMPWINDOW

#include "../vstandard.h"
#include "vwindow.hpp"

/** VTempWindow is used to quickly wrap a HWND into a non-VWCL controlled
VWindow object. This is mostly useful when needing to pass a VWindow object
reference to a method that requires one, when the only thing you know is
the windows HWND. The VWindow::m_hWindow is set to NULL in the destructor
so the VWindow base class does not attempt to destroy the window. The window
handle (HWND) is not sub-classed or owned in any way by VWCL.*/
class VTempWindow : public VWindow
{
public:
	/** Default constructor is normally passed a windows handle (HWND).*/
	VTempWindow(HWND hWnd = NULL)
		: VWindow(VWCL_RTTI_TEMP_WINDOW)
		{ GetHandle() = hWnd; }

	/** Virtual destructor sets the internal handle (HWND) to NULL,
	preventing the base class for destroying it.*/
	virtual ~VTempWindow()
		{ GetHandle() = NULL; }
};

#endif	/* VTEMPWINDOW*/
