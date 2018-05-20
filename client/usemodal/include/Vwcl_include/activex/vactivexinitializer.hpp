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

#ifndef VACTIVEXINITIALIZER
#define VACTIVEXINITIALIZER

#include "../vstandard.h"

/* Include OLE/ActiveX headers.*/
#include <ole2.h>
#include <objbase.h>

/** VActiveXInitializer is used to initialize support for COM and/or OLE.
It initializes this support in the constructor and releases support in
the destructor. All initialization and release is done on as "as-needed"
basis. Normally a class of this type must exist throughout the scope of
your application, if your application uses COM or OLE. Each thread in your
application that uses COM or OLE should have an object of this type
instantiated throughout the thread lifetime.*/
class VActiveXInitializer
{
public:
	/** Default constructor initializes only COM by default, but can be
	used to initialize full blown OLE support. See the Win32 API documentation
	for the differences between CoInitialize() and OleInitialize().*/
	VActiveXInitializer(VBOOL bCOMOnly = VTRUE)
	{
		m_hrCOMInitialized = E_UNEXPECTED;
		m_hrOLEInitialized = E_UNEXPECTED;

		if ( bCOMOnly )
			m_hrCOMInitialized = CoInitialize(NULL);
		else
			m_hrOLEInitialized = OleInitialize(NULL);
	}

	/* Virtual destructor releases COM or OLE support.*/
	virtual ~VActiveXInitializer()
	{
		if ( IsOLE() )
			OleUninitialize();
		else if ( IsCOM() )
			CoUninitialize();
	}

	/* Determine if COM is initialized. If OLE is initialized, COM will
	be as well. However, if only COM is initialized, full blown OLE
	will not be.*/
	VBOOL	IsCOM() const
	{
		if (	SUCCEEDED(m_hrCOMInitialized) ||
				SUCCEEDED(m_hrOLEInitialized) )
		{
			return VTRUE;
		}

		return VFALSE;
	}

	/* Determine if full blown OLE is initialized. It can be assumed that
	COM will also be enabled if this method returns VTRUE.*/
	VBOOL	IsOLE() const
		{ return SUCCEEDED(m_hrOLEInitialized); }

protected:
	HRESULT	m_hrCOMInitialized;
	HRESULT	m_hrOLEInitialized;
};

#endif /* VACTIVEXINITIALIZER*/
