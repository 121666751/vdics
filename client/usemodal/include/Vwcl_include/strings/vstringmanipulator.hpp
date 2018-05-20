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

#ifndef VSTRINGMANIPULATOR
#define VSTRINGMANIPULATOR

#include "../vstandard.h"
#include "vstring.hpp"

/** VStringManipulator is a base class for other classes that provide
modifications to VString, but are themselves not a VString object.*/
class VStringManipulator
{
public:
	/** Constructor must be initialized with a VString object.*/
	VStringManipulator(VString& str)
	{
		m_pString = &str;
		VASSERT(m_pString)
	}

	/** Virtual destructor does nothing.*/
	virtual ~VStringManipulator()
		{;}

protected:
	/** Embedded Members.*/
	VString*		m_pString;
};

#endif /* VSTRINGMANIPULATOR*/