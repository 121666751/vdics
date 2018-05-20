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
Other Author(s) of this source code file: Reiner Eiteljörge (rei@antaro.de)
*/

#ifndef VRESOURCESTRING
#define VRESOURCESTRING

#include "../vstandard.h"
#include "../utility/vmemoryblock.hpp"

/** VResourceString is used to load a string from resources. It has no
real ties to the VString class, other than VString can work with these
objects. The class is a very lightweight object with a minimal interface
used for loading strings and automatic implicit operators for string
extraction. It does support UNICODE and ASCII targets and internally
uses dynamic memory allocation. It is expected that objects of this type
will be used with VString for more complete string processing functions.*/
class VResourceString
{
public:
	/** Default constructor can load the string resource ID passed in as
	the nID parameter. nID can be any positive integer. It can be zero
	to indicate a string that should not be loaded. hResource identies
	the handle to the resource file to use. If this is NULL,
	VGetResourceHandle() will be used as the default.*/
	VResourceString(VUINT		nID =
									0,
					HINSTANCE	hResource =
									NULL)
		{ Load(nID, hResource); }

	/** Virtual destructor does nothing.*/
	virtual ~VResourceString()
		{;}

	/** Returns non-const C-Style string pointer.*/
	operator VSTRING () const
		{ return (VSTRING)GetString(); }

	/** Returns const C-Style string pointer.*/
	operator VSTRING_CONST () const
		{ return GetString(); }

	/** Return the internal string pointer.*/
	VSTRING_CONST	GetString() const
		{ return (VSTRING_CONST)m_StringBlock.Get(); }

	/** Load the string resource ID passed in as the nID parameter. nID can
	be any positive integer. It can be zero to indicate a string that should
	not be loaded. If 0, the string, if any, held by this class will be
	free'd. hResource identies the handle to the resource file to use. If
	this is NULL, VGetResourceHandle() will be used as the default. Returns
	a pointer to the internal string buffer on success, NULL on failure. On
	failure, the original string held by this object, if any, will remain
	intact.*/
	VSTRING_CONST	Load(	VUINT		nID =
											0,
							HINSTANCE	hResource =
											NULL)
	{
		if ( !nID )
			m_StringBlock.Free();
		else
		{
			/* Win32 string can be a max of 4k characters.*/
			VCHAR sz[4096 * sizeof(VCHAR)];

			/* Get resource handle, if needed.*/
			if ( !hResource )
				hResource = VGetResourceHandle();
			VASSERT(hResource)

			/* Load string into stack buffer and save character length.*/
			VUINT nLength =
				(hResource)
				? LoadString(hResource, nID, sz, VARRAY_SIZE(sz))
				: 0;

			if ( nLength && m_StringBlock.AllocForString(nLength) )
				memcpy(m_StringBlock.Get(), sz, nLength * sizeof(VCHAR));
			else
				return NULL;
		}

		return GetString();
	}

protected:
	/** Embedded Members.*/
	VMemoryBlock	m_StringBlock;
};

#endif /* VRESOURCESTRING*/
