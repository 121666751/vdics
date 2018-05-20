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

#ifndef VNUMBERSTRING
#define VNUMBERSTRING

#include "../vstandard.h"
#include "vstringmanipulator.hpp"
#include "vstring.hpp"

/** VNumberString is used to place numeric values into VString objects, and
compare strings with numeric values.*/
class VNumberString : public VStringManipulator
{
public:
	/** Constructor must be initialized with a VString object.*/
	VNumberString(VString& str)
		: VStringManipulator(str)
		{}
		
	/** Virtual destructor does nothing.*/
	virtual ~VNumberString()
		{;}

	/** Assign an integer to the string.*/
	VSTRING operator = (VINT nValue)
		{ return Load(nValue); }

	/** Assign an unsigned integer to the string.*/
	VSTRING operator = (VUINT nValue)
		{ return Load(nValue); }

	/** Assign a long number to the string.*/
	VSTRING operator = (VLONG nValue)
		{ return Load(nValue); }

	/** Assign an unsigned long number to the string.*/
	VSTRING operator = (VULONG nValue)
		{ return Load(nValue); }

	/** Concatenation of an integer.*/
	VSTRING operator += (VINT nValue)
		{ return Append(nValue); }

	/** Concatenation of an unsigned integer.*/
	VSTRING operator += (VUINT nValue)
		{ return Append(nValue); }

	/** Concatenation of a long number or integer.*/
	VSTRING operator += (VLONG nValue)
		{ return Append(nValue); }

	/** Concatenation of an unsigned long number or integer.*/
	VSTRING operator += (VULONG nValue)
		{ return Append(nValue); }

	/** Comparison of string contents with an integer.*/
	VBOOL operator == (VINT nValue) const
	{
		VASSERT(m_pString)

		VCHAR sz[64];
		
		if ( m_pString->CompareNoCase(VITOA(nValue, sz, 10)) == 0 )
			return VTRUE;
			
		return VFALSE;
	}

	/** Comparison of string contents with an unsigned integer.*/
	VBOOL operator == (VUINT nValue) const
	{
		VASSERT(m_pString)

		VCHAR sz[64];
		
		if ( m_pString->CompareNoCase(VUITOA((VULONG)nValue, sz, 10)) == 0 )
			return VTRUE;
			
		return VFALSE;
	}

	/** Comparison of string contents with a long number or integer.*/
	VBOOL operator == (VLONG nValue) const
	{
		VASSERT(m_pString)

		VCHAR sz[64];
		
		if ( m_pString->CompareNoCase(VLTOA(nValue, sz, 10)) == 0 )
			return VTRUE;
			
		return VFALSE;
	}

	/** Comparison of string contents with an unsigned long number
	or integer.*/
	VBOOL operator == (VULONG nValue) const
	{
		VASSERT(m_pString)

		VCHAR sz[64];
		
		if ( m_pString->CompareNoCase(VULTOA(nValue, sz, 10)) == 0 )
			return VTRUE;
			
		return VFALSE;
	}

	/** Append an integer to the string.*/
	VSTRING			Append(VINT nValue)
		{ return Append((VLONG)nValue); }

	/** Append an unsigned integer to the string.*/
	VSTRING			Append(VUINT nValue)
		{ return Append((VULONG)nValue); }

	/** Append a long integer or integer to the string.*/
	VSTRING			Append(VLONG nValue)
	{
		VASSERT(m_pString)

		VCHAR sz[64];
		return m_pString->Append(VLTOA(nValue, sz, 10));
	}

	/** Append an unsigned long integer or integer to the string.*/
	VSTRING			Append(VULONG nValue)
	{
		VASSERT(m_pString)

		VCHAR sz[64];
		return m_pString->Append(VULTOA(nValue, sz, 10));
	}

	/** Load an integer into the string.*/
	VSTRING			Load(VINT nValue)
		{ return Load((VLONG)nValue); }

	/** Load an unsigned integer into the string.*/
	VSTRING			Load(VUINT nValue)
		{ return Load((VULONG)nValue); }

	/** Load a long integer or integer into the string.*/
	VSTRING			Load(VLONG nValue)
	{
		VASSERT(m_pString)

		VCHAR sz[64];
		return m_pString->String(VLTOA(nValue, sz, 10));
	}

	/** Load an unsigned long integer or integer to the string.*/
	VSTRING			Load(VULONG nValue)
	{
		VASSERT(m_pString)

		VCHAR sz[64];
		return m_pString->String(VULTOA(nValue, sz, 10));
	}
};

#endif /* VNUMBERSTRING*/