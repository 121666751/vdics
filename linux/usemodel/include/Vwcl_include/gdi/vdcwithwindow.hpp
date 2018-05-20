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

#ifndef VDCWITHWINDOW
#define VDCWITHWINDOW

#include "../vstandard.h"
#include "../windows/vwindow.hpp"
#include "vdc.hpp"

/** VDCWithWindow is a VDC object that requires a window for proper usage.
It is primarily a base class for more specific device context classes.*/
class VDCWithWindow : public VDC
{
public:
	/** Standard constructor initializes by saving a pointer to the
	associated VWindow. The internal device context handle (HDC) is
	initialized to NULL and must be set to a valid value elsewhere.*/
	VDCWithWindow(VWindow const& window)
	{
		m_pWindow = (VWindow*)&window;
		VASSERT(m_pWindow)
	}

	/** Virtual destructor calls local Free() variant, which uses
	ReleaseDC(), not the base class DeleteDC().*/
	virtual ~VDCWithWindow()
		{ Free(); }

	/** Return the internal VWindow object reference.*/
	VWindow&		GetWindow() const
	{
		VASSERT(m_pWindow)
		return (VWindow&)VREF_PTR(m_pWindow);
	}

	/** Override uses ReleaseDC(), not the base class DeleteDC(), but still
	honors m_bFreeHDC member to skip if the device context is not owned by
	this object.*/
	virtual void	Free()
	{
		if ( m_bFreeHDC && m_hDC )
		{
			VASSERT(m_pWindow)
			ReleaseDC(m_pWindow->GetHandle(), m_hDC);
			m_hDC = NULL;
		}
	}

protected:
	/** Embedded Members.*/
	VWindow*		m_pWindow;
};

#endif /* VDCWITHWINDOW*/
