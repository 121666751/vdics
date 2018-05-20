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

#ifndef VSTOCKBRUSH
#define VSTOCKBRUSH

#include "../vstandard.h"
#include "../structures/vrect.hpp"
#include "vdc.hpp"
#include "vgdistockobject.hpp"

/** VStockBrush is a VGDIStockObject that only knows how to deal with
Windows defined stock brushes.*/
class VStockBrush : public VGDIStockObject
{
public:
	/** Default  constructor initializes the class with a Windows stock
	brush object. See the Win32 GetStockObject() function for all possible
	values that can be passed for the nStockBrushID parameter. At the time
	of the last class update these were:

	BLACK_BRUSH - Black brush.

	WHITE_BRUSH - White brush.

	DKGRAY_BRUSH - Dark gray brush.

	DC_BRUSH - Windows 98, Windows NT 5.0 and later: Solid color brush.
	The default color is white. The color can be changed by using the
	SetDCBrushColor function.

	GRAY_BRUSH - Gray brush.

	HOLLOW_BRUSH - Hollow brush (equivalent to NULL_BRUSH).

	LTGRAY_BRUSH - Light gray brush.

	NULL_BRUSH - Null brush (equivalent to HOLLOW_BRUSH).*/
	VStockBrush(VINT nStockBrushID = BLACK_BRUSH)
		: VGDIStockObject(nStockBrushID)
		{ VASSERT_STOCK_BRUSH(nStockBrushID) }

	/** Fills a rectangle by using the brush. This function includes the left
	and top borders, but excludes the right and bottom borders of the
	rectangle. GDI fills a rectangle up to, but not including, the right
	column and bottom row, regardless of the current mapping mode. In
	Windows CE version 1.0, the brush cannot be a color brush. This function
	is the same in Windows CE version 2.0 as it is in Windows desktop
	platforms.*/
	void		FillRect(	VDC&			dc,
							VRect const&	r) const
		{ dc.FillRect(r, (HBRUSH)m_hGDIObject); }

	/** Return a reference to the brush GDI object handle.*/
	HBRUSH&		GetHandle() const
		{ return (HBRUSH&)VGDIStockObject::GetHandle(); }

	/** Get the brush information into a LOGBRUSH structure.*/
	VBOOL		GetInfo(LOGBRUSH& lb) const
		{ return VGDIObject::GetInfo(sizeof(LOGBRUSH), &lb); }
};

#endif /* VSTOCKBRUSH*/
