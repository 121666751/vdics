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

#ifndef VTEMPDC
#define VTEMPDC

#include "../vstandard.h"
#include "vdc.hpp"

/** VTempDC is a VDC object that will never free the wrapped Windows device
context (HDC). This class is mostly useful when you need to pass a VDC
object reference to another VWCL class or function, but when you have only
a HDC device context handle that you, or VWCL, does not own, or wish to not
have deleted by a VDC class.*/
class VTempDC : public VDC
{
public:
	/** Standard constructor must be initialized with an existing (valid)
	device context (HDC).*/
	VTempDC(HDC hDC)
		: VDC(hDC, VFALSE)
		{ VASSERT(hDC) }

	/** Standard constructor must be initialized with an existing (valid)
	device context object (VDC). When we call the base class constructor,
	we cast away the const-ness because the base class could modify the
	sourceDC if we passed VTRUE for bFreeDC, but we don't, so sourceDC
	really is const.*/
	VTempDC(VDC& sourceDC)
		: VDC(sourceDC, VFALSE)
		{ VASSERT(sourceDC.GetHandle()) }

	/** Override of base class Free() function that does absolutely nothing.
	Thus, it is safe to call Free() on these objects and not have to worry
	about accidentally causing any harm to the internal HDC member.*/
	virtual void	Free()
		{;}
};

#endif /* VTEMPDC*/
