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

#ifndef VPATHSTRING
#define VPATHSTRING

#include "../vstandard.h"
#include "vstringmanipulator.hpp"
#include "vstring.hpp"

/** VPathString is used to place a module (program, library, etc.) path into
a VString object.*/
class VPathString : public VStringManipulator
{
public:
	/** Constructor must be initialized with a VString object.*/
	VPathString(VString& str)
		: VStringManipulator(str)
		{}

	/** Append the current working directory to the string. If
	bTerminateWithSeparator is specified, The returned string will have a
	trailing VFILE_PATH_SEP_CHAR character appended to it. Otherwise it
	will contain only the path without the trailing slash or backslash.*/
	VSTRING			AppendCurrentDirectory(
		VBOOL	bTerminateWithSeparator =
					VTRUE,
		VBOOL	bAppend =
					VTRUE)
	{
		VASSERT(m_pString)

		VCHAR sz[1024];
		VGETCWD(sz, VARRAY_SIZE(sz));

		if ( bAppend )
			m_pString->Append(sz);
		else
			m_pString->String(sz);

		if ( bTerminateWithSeparator )
			m_pString->EndWith();

		return m_pString->String();
	}

	/** Append the path of the module in hInstance into the string. If
	bPathOnly is specified, only the path, including the last slash or
	backlash will be saved, otherwise the entire path will be. If hInstance
	is NULL, VGetInstanceHandle() will be used. On Non-Win32, this function
	will load only the path of the calling program and an alternate module
	cannot be specified. If bTerminateWithSeparator is VFALSE, the string
	will not be terminated with a trailing slash or backslash. This function
	will return a pointer to the internal string object on success, or NULL
	on failure. If NULL is returned, the previous string held in this object
	is still valid, but it will not be the path requested since the
	function returned NULL indicating an error.*/
	#ifdef VWCL_NO_WIN32_API
		VSTRING		AppendModulePath(
			VBOOL	bPathOnly =
						VTRUE,
			VBOOL	bTerminateWithSeparator =
						VTRUE,
			VBOOL	bAppend = VTRUE)
	#else
		VSTRING		AppendModulePath(
			VBOOL		bPathOnly =
							VTRUE,
			VBOOL		bTerminateWithSeparator =
							VTRUE,
			HINSTANCE	hInstance =
							NULL,
			VBOOL		bAppend =
							VTRUE)
	#endif
	{
		VASSERT(m_pString)

		VCHAR sz[4096] = {'\0'};

		#ifdef VWCL_NO_WIN32_API
			VSTRINGA pszBase = VGetARGV(0);

			if ( pszBase && realpath(pszBase, sz) )
			{
				/* Check to make sure the file that was found is
				really there.*/
				if ( !VDoesFileExist(sz) )
					sz[0] = VTEXT('\0');
			}
		#else
			GetModuleFileName(	(hInstance)
								? hInstance
								: VGetInstanceHandle(),
								sz,
								VARRAY_SIZE(sz));
		#endif

		if ( sz[0] != VTEXT('\0') )
		{
			/* Terminate after path?*/
			VSTRING pszEnd =
				(bPathOnly) ? VSTRRCHR(sz, VFILE_PATH_SEP_CHAR) : NULL;

			if  ( pszEnd )
				pszEnd[((bTerminateWithSeparator) ? 1 : 0)] = VTEXT('\0');

			return (bAppend) ? m_pString->Append(sz) : m_pString->String(sz);
		}

		return NULL;
	}

	/** Append a temporary file name to the string. On Win32 platforms, this
	will actually create the file, and then close it. On UNIX platforms, the
	file name will be just a string, not a physical file.*/
	VSTRING			AppendTemporaryFileName(VBOOL bAppend = VTRUE)
	{
		VCHAR sz[1024];

		#ifdef VWCL_NO_WIN32_API
			if ( VTMPNAM(sz) )
				return	(bAppend)
						? m_pString->Append(sz)
						: m_pString->String(sz);
		#else
			if ( GetTempPath(VARRAY_SIZE(sz), sz) )
			{
				VString s(sz);

				if ( GetTempFileName(s, VTEXT("VTF"), 0, sz) )
					return	(bAppend)
							? m_pString->Append(sz)
							: m_pString->String(sz);
			}
		#endif

		return NULL;
	}

	/** Load the current working directory into the string. If
	bTerminateWithSeparator is specified, The returned string will have a
	trailing VFILE_PATH_SEP_CHAR character appended to it. Otherwise it
	will contain only the path without the trailing slash or backslash.*/
	VSTRING			LoadCurrentDirectory(
		VBOOL	bTerminateWithSeparator = VTRUE)
	{
		return AppendCurrentDirectory(bTerminateWithSeparator, VFALSE);
	}

	/** Load the path of the module in hInstance into the string. If
	bPathOnly is specified, only the path, including the last slash or
	backlash will be saved, otherwise the entire path will be. If hInstance
	is NULL, VGetInstanceHandle() will be used. On Non-Win32, this function
	will load only the path of the calling program and an alternate module
	cannot be specified. If bTerminateWithSeparator is VFALSE, the string
	will not be terminated with a trailing slash or backslash. This function
	will return a pointer to the internal string object on success, or NULL
	on failure. If NULL is returned, the previous string held in this object
	is still valid, but it will not be the path requested since the
	function returned NULL indicating an error.*/
	#ifdef VWCL_NO_WIN32_API
		VSTRING		LoadModulePath(	VBOOL	bPathOnly =
												VTRUE,
									VBOOL	bTerminateWithSeparator =
												VTRUE)
		{
			return AppendModulePath(bPathOnly,
									bTerminateWithSeparator,
									VFALSE);
		}
	#else
		VSTRING		LoadModulePath(	VBOOL		bPathOnly =
													VTRUE,
									VBOOL		bTerminateWithSeparator =
													VTRUE,
									HINSTANCE	hInstance =
													NULL)
		{
			return AppendModulePath(bPathOnly,
									bTerminateWithSeparator,
									hInstance,
									VFALSE);
		}
	#endif

	/** Load a temporary file name into the string. On Win32 platforms, this
	will actually create the file, and then close it. On UNIX platforms, the
	file name will be just a string, not a physical file.*/
	VSTRING			LoadTemporaryFileName()
		{ return AppendTemporaryFileName(VFALSE); }
};

#endif /* VPATHSTRING*/