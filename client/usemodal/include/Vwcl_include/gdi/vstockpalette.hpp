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

#ifndef VSTOCKPALETTE
#define VSTOCKPALETTE

#include "../vstandard.h"
#include "vgdistockobject.hpp"

/** VStockPalette is a VGDIStockObject that only knows how to deal with
Windows defined stock palettes.*/
class VStockPalette : public VGDIStockObject
{
public:
	/** Default  constructor initializes the class with a Windows stock
	palette object. See the Win32 GetStockObject() function for all
	possible values that can be passed for the nStockPaletteID parameter.
	At the time of the last class update these were:

	DEFAULT_PALETTE - This palette consists of the static colors in the
	system palette.*/
	VStockPalette(VINT nStockPaletteID = DEFAULT_PALETTE)
		: VGDIStockObject(nStockPaletteID)
		{ VASSERT_STOCK_PALETTE(nStockPaletteID) }

	/** Return a reference to the palette GDI object handle.*/
	HPALETTE&	GetHandle() const
		{ return (HPALETTE&)VGDIStockObject::GetHandle(); }

	/** Maps palette entries from the current logical palette to the system
	palette. An application can determine whether a device supports palette
	operations by calling the GetDeviceCaps function and specifying the
	RASTERCAPS constant. The Realize() function modifies the palette for
	the device associated with the specified device context. If the device
	context is a memory DC, the color table for the bitmap selected into
	the DC is modified. If the device context is a display DC, the physical
	palette for that device is modified. A logical color palette is a buffer
	between color-intensive applications and the system, allowing these
	applications to use as many colors as needed without interfering with
	colors displayed by other windows. When an application's window has the
	focus and it calls the RealizePalette() function, the system attempts
	to realize as many of the requested colors as possible. The same is
	also true for applications with inactive windows. Windows CE does not
	arbitrate between the palettes of the background and foreground
	applications. The foreground application has complete control of the
	system palette. Therefore, Windows CE does not perform any color
	matching operations for foreground applications; it simply overwrites
	the system palette entries with the hdc parameter's palette entries.
	Windows CE does not support Realize() for background applications.
	Realize will fail if the device associated with the device context does
	not have a settable palette. Use GetDeviceCaps() to find out if the
	device has a settable palette before using Realize(). If the function
	succeeds, the return value is the number of entries in the logical
	palette mapped to the system palette. If the function fails, the return
	value is GDI_ERROR.*/
	VUINT		Realize(VDC& dc) const
		{ return RealizePalette(dc.GetHandle()); }
};


#endif /* VSTOCKPALETTE*/
