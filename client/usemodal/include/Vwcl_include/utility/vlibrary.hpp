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

#ifndef VLIBRARY
#define VLIBRARY

#include "../vstandard.h"

/** VLibrary supports dynamically loaded DLL's and other executable files.*/
class VLibrary
{
public:
	/** Default constructor initializes internal members and can optionally
	take ownership of a previously opened library if hLibrary is given. If
	not NULL, this class will free the library when appropriate.*/
	VLibrary(HINSTANCE hLibrary = NULL)
		{ m_hInstance = hLibrary; }

	/** Constructor that will attempt to load a library based on
	a file name.*/
	VLibrary(VSTRING_CONST pszFileName)
	{
		m_hInstance = NULL;
		Set(pszFileName);
	}

	/** Virtual destructor verifies the library wrapped by this object
	is free'd.*/
	virtual ~VLibrary()
		{ Free(); }

	/** Free the library currently wrapped by this object, as needed.*/
	void		Free()
	{
		if ( m_hInstance )
		{
			FreeLibrary(m_hInstance);
			m_hInstance = NULL;
		}
	}

	/** Return the instance handle to the library currently wrapped
	by this object.*/
	HINSTANCE	GetHandle() const
		{ return m_hInstance; }

	/** Make the library specified by hLibrary the current libray this
	object wraps. If this object is already associated with a current
	library, it will be free'd first. Calling this function with hLibrary
	set to NULL is the same as calling Free(). Returns the handle to the
	newly wrapped library, or NULL.*/
	HINSTANCE	Set(HINSTANCE hLibrary)
	{
		Free();
		m_hInstance = hLibrary;
		return m_hInstance;
	}

	/** Load the library specified by pszFileName and make it the current
	libray this object wraps. If this object is already associated with a
	current library, it will be free'd first. Calling this function with
	hLibrary set to NULL is the same as calling Free(). Returns the handle
	to the newly wrapped library, or NULL.*/
	HINSTANCE	Set(VSTRING_CONST pszFileName)
	{
		VASSERT(VSTRLEN_CHECK(pszFileName))
		return Set(LoadLibrary(pszFileName));
	}

protected:
	/** Embedded Members.*/
	HINSTANCE	m_hInstance;
};

#endif /* VLIBRARY*/
