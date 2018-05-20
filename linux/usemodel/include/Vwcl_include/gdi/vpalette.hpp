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

#ifndef VPALETTE
#define VPALETTE

#include "../vstandard.h"
#include "vgdiobject.hpp"

/** VPalette is a VGDIObject that encapulates a HPALETTE GDI object. This
class is used only for custom created palette GDI objects, not stock palette
objects. This class will delete the object during destruction. For Windows
defined stock palette objects, use VStockPalette instead.*/
class VPalette : public VGDIObject
{
public:
	/** Default constructor can initialize a palette from a LOGPALETTE
	structure pointer, or NULL, in which case you would need to call
	Create() to initialize the internal palette GDI object.*/
	VPalette(LOGPALETTE const* pLogPalette = NULL)
	{
		if ( pLogPalette )
			Create(VREF_PTR(pLogPalette));
	}

	/** Creates a logical color palette. LogPalette contains information
	about the colors in the logical palette. An application can determine
	whether a device supports palette operations by calling the
	GetDeviceCaps() function and specifying the RASTERCAPS constant. A
	palette selected into a device context can be realized by calling the
	Realize() function. Since Windows CE does not arbitrate between the
	palettes of the foreground and background applications, palettes are
	not automatically padded with Windows systems colors. Therefore, the
	number of color entries in the palette created by this function is
	always the same as the palNumEntries member of the LOGPALETTE structure.
	This function returns VTRUE if the new palette was created, VFALSE
	otherwise.*/
	VBOOL		Create(LOGPALETTE const& LogPalette)
		{ return SetHandle(CreatePalette(&LogPalette)); }

	/** Return a reference the the palette GDI object handle.*/
	HPALETTE&	GetHandle() const
		{ return (HPALETTE&)VGDIObject::GetHandle(); }

	/** Maps palette entries from the current logical palette to the system
	palette. An application can determine whether a device supports palette
	operations by calling the GetDeviceCaps() function and specifying the
	RASTERCAPS constant. The Realize() function modifies the palette for the
	device associated with the specified device context. If the device
	context is a memory DC, the color table for the bitmap selected into
	the DC is modified. If the device context is a display DC, the physical
	palette for that device is modified. A logical color palette is a buffer
	between color-intensive applications and the system, allowing these
	applications to use as many colors as needed without interfering with
	colors displayed by other windows. When an application's window has the
	focus and it calls the Realize() function, the system attempts to
	realize as many of the requested colors as possible. The same is also
	true for applications with inactive windows. Windows CE does not
	arbitrate between the palettes of the background and foreground
	applications. The foreground application has complete control of the
	system palette. Therefore, Windows CE does not perform any color
	matching operations for foreground applications; it simply overwrites
	the system palette entries with the hdc parameter's palette entries.
	Windows CE does not support Realize() for background applications.
	Realize() will fail if the device associated with the device context
	does not have a settable palette. Use GetDeviceCaps() to find out if
	the devicehas a settable palette before using Realize(). If the function
	succeeds, the return value is the number of entries in the logical
	palette mapped to the system palette. If the function fails, the return
	value is GDI_ERROR.*/
	VUINT		Realize(VDC& dc) const
		{ return RealizePalette(dc.GetHandle()); }
};

#endif /* VPALETTE*/
