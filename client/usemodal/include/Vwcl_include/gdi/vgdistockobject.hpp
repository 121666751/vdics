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

#ifndef VGDISTOCKOBJECT
#define VGDISTOCKOBJECT

#include "../vstandard.h"
#include "vtempgdiobject.hpp"

/* Define these if not already done.*/
#ifndef DC_BRUSH
	#define DC_BRUSH 18
#endif

#ifndef DC_PEN
	#define DC_PEN 19
#endif

/** Macro's for debug assertion for invalid stock object ID's.*/
#define VASSERT_STOCK_PEN(nStockPenID)			VASSERT(nStockPenID == BLACK_PEN || nStockPenID == WHITE_PEN || nStockPenID == NULL_PEN || nStockPenID == DC_PEN)
#define VASSERT_STOCK_BRUSH(nStockBrushID)		VASSERT(nStockBrushID == BLACK_BRUSH || nStockBrushID == DKGRAY_BRUSH || nStockBrushID == GRAY_BRUSH || nStockBrushID == HOLLOW_BRUSH || nStockBrushID == LTGRAY_BRUSH || nStockBrushID == NULL_BRUSH || nStockBrushID == WHITE_BRUSH || nStockBrushID == DC_BRUSH)
#define VASSERT_STOCK_FONT(nStockFontID)		VASSERT(nStockFontID == ANSI_FIXED_FONT || nStockFontID == ANSI_VAR_FONT || nStockFontID == DEVICE_DEFAULT_FONT || nStockFontID == DEFAULT_GUI_FONT || nStockFontID == OEM_FIXED_FONT || nStockFontID == SYSTEM_FONT || nStockFontID == SYSTEM_FIXED_FONT)
#define VASSERT_STOCK_PALETTE(nStockPaletteID)	VASSERT(nStockPaletteID == DEFAULT_PALETTE)

/** VGDIStockObject is a VTempGDIObject that only knows how to initialize
with a stock Windows object ID. This class is primarily a base class for
other stock GDI object classes.*/
class VGDIStockObject : public VTempGDIObject
{
public:
	/** Default constructor initializes the class with a Windows stock
	object.*/
	VGDIStockObject(VINT nStockObjectID = BLACK_PEN)
		: VTempGDIObject(GetStockObject(nStockObjectID))
		{;}

	/** Get a stock object handle into this object.*/
	VBOOL			GetStock(VINT nStockObjectID)
		{ return SetHandle(GetStockObject(nStockObjectID)); }
};

#endif /* VGDISTOCKOBJECT*/
