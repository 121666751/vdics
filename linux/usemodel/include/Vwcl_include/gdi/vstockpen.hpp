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

#ifndef VSTOCKPEN
#define VSTOCKPEN

#include "../vstandard.h"
#include "vdc.hpp"
#include "vgdistockobject.hpp"

/** VStockPen is a VGDIStockObject that only knows how to deal with Windows
defined stock pens.*/
class VStockPen : public VGDIStockObject
{
public:
	/** Default constructor initializes the class with a Windows stock black
	pen object. See the Win32 GetStockObject() function for all possible
	values that can be passed for the nStockPenID parameter. At the time of
	the last class update these were:

	BLACK_PEN - A black pen.

	WHITE_PEN - A white pen.

	DC_PEN - Windows 98, Windows NT 5.0 and later: Solid pen color. The
	default color is white. The color can be changed by using the
	SetDCPenColor function.*/
	VStockPen(VINT nStockPenID = BLACK_PEN)
		: VGDIStockObject(nStockPenID)
		{ VASSERT_STOCK_PEN(nStockPenID) }

	/** Return a reference to the brush pen object handle.*/
	HPEN&		GetHandle() const
		{ return (HPEN&)VGDIStockObject::GetHandle(); }

	/** Get the pen information into a LOGPEN structure.*/
	VBOOL		GetInfo(LOGPEN& lp) const
		{ return VGDIObject::GetInfo(sizeof(LOGPEN), &lp); }
};

#endif /* VSTOCKPEN*/
