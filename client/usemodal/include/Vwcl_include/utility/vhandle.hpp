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

#ifndef VHANDLE
#define VHANDLE

#include "../vstandard.h"

/** VHandle stores a Windows HANDLE value, verifying it is closed in
destructor. Attempting to store either NULL or INVALID_HANDLE_VALUE will
cause this class to store NULL. This allows all Windows functions that use
handle values differently to use NULL as an invalid handle value. This
class should only be used by Windows objects whose HANDLE value can and
should be closed with a call to the Win32 API CloseHandle() function.*/
class VHandle
{
public:
	/** Default constructor can be initialized with a valid windows
	HANDLE of any type that can be closed with CloseHandle().*/
	VHandle(HANDLE hHandle = NULL)
	{
		m_hHandle = NULL;

		if ( hHandle )
			Set(hHandle, VFALSE);
	}

	/** Virtual destructor verifies the active handle, in any, is
	properly closed.*/
	virtual ~VHandle()
		{ Close(); }

	/** Close the currently wrapped handle, as needed.*/
	void		Close()
	{
		if ( m_hHandle )
		{
			CloseHandle(m_hHandle);
			m_hHandle = NULL;
		}
	}

	/** Return the handle that is currently open by this object.*/
	HANDLE		GetHandle() const
		{ return m_hHandle; }

	/** Set the handle this class is to manage. If a current handle exists,
	it will be closed only if bCloseCurrentHandle is VTRUE, which is the
	default. The return value is the handle managed by the class, or NULL.
	If hHandle is INVALID_HANDLE_VALUE, NULL will stored in this class
	instead. This makes all references to handles that are invalid return
	NULL, not the Windows mix of NULL or INVALID_HANDLE_VALUE.*/
	HANDLE		Set(	HANDLE	hHandle,
						VBOOL	bCloseCurrentHandle =
									VTRUE)
	{
		if ( bCloseCurrentHandle )
			Close();

		m_hHandle =
			(hHandle == INVALID_HANDLE_VALUE) ? NULL : hHandle;

		return m_hHandle;
	}

protected:
	/** Embedded Members.*/
	HANDLE		m_hHandle;
};

#endif /* VHANDLE*/
