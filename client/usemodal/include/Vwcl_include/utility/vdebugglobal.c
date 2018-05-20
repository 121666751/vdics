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

#include "../vstandard.h"

/** This function is not needed in non-debug builds. Normally it is excluded
from release builds, but just to save a few hundred bytes, lets go ahead and
protect the developer who may have forgot to exclude this file.*/
#ifdef VWCL_DEBUG
	VINT VShowLastErrorMessage(HWND hWndParent)
	{
		VCHAR	szString[4096] =	{VTEXT('\0')};
		VDWORD	nLastError =		GetLastError();

		if ( nLastError )
			FormatMessage(	FORMAT_MESSAGE_FROM_SYSTEM,
							NULL,
							nLastError,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							szString,
							VARRAY_SIZE(szString),
							NULL);

		if ( szString[0] != VTEXT('\0') )
		{
			#ifdef VWCL_CONSOLE_APP
				VODS(szString)
			#else
				return MessageBox(	hWndParent,
									szString,
									VWCL_DEBUG_MESSAGE_STRING,
									MB_ICONINFORMATION);
			#endif
		}

		return IDOK;
	}
#endif

