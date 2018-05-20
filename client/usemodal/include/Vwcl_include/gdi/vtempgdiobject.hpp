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

#ifndef VTEMPGDIOBJECT
#define VTEMPGDIOBJECT

#include "../vstandard.h"
#include "vgdiobject.hpp"

/** VTempGDIObject is a VGDIObject that never delete's the underlying
GDI object.*/
class VTempGDIObject : public VGDIObject
{
public:
	/** Standard  constructor initializes the class with an existing GDI
	object. A NULL hGDIObject is valid during construction.*/
	VTempGDIObject(HGDIOBJ hGDIObject)
		: VGDIObject((HGDIOBJ)hGDIObject, VFALSE)
		{;}
};

#endif /* VTEMPGDIOBJECT*/
