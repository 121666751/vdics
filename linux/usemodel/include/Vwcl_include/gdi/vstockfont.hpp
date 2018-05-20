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

#ifndef VSTOCKFONT
#define VSTOCKFONT

#include "../vstandard.h"
#include "vgdistockobject.hpp"

/** VStockFont is a VGDIStockObject that only knows how to deal with
Windows defined stock fonts.*/
class VStockFont : public VGDIStockObject
{
public:
	/** Default  constructor initializes the class with a Windows stock
	brush object. See the Win32 GetStockObject() function for all possible
	values that can be passed for the nStockFontID parameter. At the time
	of the last class update these were:

	ANSI_FIXED_FONT - Windows fixed-pitch (monospace) system font.

	ANSI_VAR_FONT - Windows variable-pitch (proportional space) system font.

	DEVICE_DEFAULT_FONT - Windows NT: Device-dependent font.

	DEFAULT_GUI_FONT - Default font for user interface objects such as
	menus and dialog boxes.

	OEM_FIXED_FONT - Original equipment manufacturer (OEM) dependent
	fixed-pitch (monospace) font.

	SYSTEM_FONT - System font. By default, the system uses the system font
	to draw menus, dialog box controls, and text.

	SYSTEM_FIXED_FONT - Fixed-pitch (monospace) system font. This stock
	object is provided only for compatibility with 16-bit Windows versions
	earlier than 3.0.*/
	VStockFont(VINT nStockFontID = ANSI_VAR_FONT)
		: VGDIStockObject(nStockFontID)
		{ VASSERT_STOCK_FONT(nStockFontID) }

	/** Return a reference to the font GDI object handle.*/
	HFONT&		GetHandle() const
		{ return (HFONT&)VGDIStockObject::GetHandle(); }

	/** Get the font information into a LOGFONT structure.*/
	VBOOL		GetInfo(LOGFONT& lf) const
		{ return VGDIObject::GetInfo(sizeof(LOGFONT), &lf); }
};

#endif /* VSTOCKFONT*/
