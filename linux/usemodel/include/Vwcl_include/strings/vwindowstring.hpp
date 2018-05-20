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

#ifndef VWINDOWSTRING
#define VWINDOWSTRING

#include "../vstandard.h"
#include "../utility/vmemoryblock.hpp"
#include "vstringmanipulator.hpp"
#include "vstring.hpp"

/** VWindowString is used to place strings from windows into
VString objects.*/
class VWindowString : public VStringManipulator
{
public:
	/** Constructor must be initialized with a VString object. It can
	also be initialized with a window handle to get the text from,
	and can be instructed to append the text or replace it. The default
	is to replace the string in the str object with the text loaded
	from the window. See the Append() method for more information.*/
	VWindowString(	VString&	str,
					HWND		hWnd =
									NULL,
					VUINT		nMaxChars =
									0,
					VBOOL		bAppend =
									VFALSE)
		: VStringManipulator(str)
	{
		if ( hWnd )
			Append(hWnd, nMaxChars, bAppend);
	}

	/** Virtual destructor does nothing.*/
	virtual ~VWindowString()
		{;}

	/** Same as Load().*/
	VSTRING operator = (HWND hWnd)
		{ return Load(hWnd); }

	/** Same as Append().*/
	VSTRING operator += (HWND hWnd)
		{ return Append(hWnd); }

	/** Append the string from the contents of a window. Normally, the
	entire text of the window is retrieved, but if you are only
	interested in a certain number of characters, this can be specified in
	the nMaxChars value. This value specifies the number of characters
	to get, excluding the NULL terminating character. So if you want 2
	characters, pass 2 for nMaxChars. 3 total bytes will be allocated
	and 2 characters copied to the buffer.*/
	VSTRING	Append(	HWND	hWnd,
					VUINT	nMaxChars =
								0,
					VBOOL	bAppend =
								VTRUE)
	{
		VASSERT(m_pString)
		VASSERT(hWnd)
		VASSERT(IsWindow(hWnd))

		VSTRING pszResult = NULL;

		if ( hWnd )
		{
			/* Get the length of the window string.*/
			VUINT nLength = (VUINT)GetWindowTextLength(hWnd);

			if ( nLength )
			{
				if ( nMaxChars && nMaxChars <= nLength )
					nLength = nMaxChars;

				/* Allocate memory for string.*/
				VMemoryBlock memBlock;
				
				if ( memBlock.AllocForString(nLength) )
				{
					if ( GetWindowText(	hWnd,
										(VSTRING)memBlock.Get(),
										nLength + 1) )
					{
						if ( bAppend )
							pszResult =
								m_pString->Append(	(VSTRING)memBlock.Get(),
													nMaxChars,
													nLength);
						else
							pszResult = m_pString->StealMemoryBlock(memBlock);
					}
				}
			}
		}

		return pszResult;
	}

	/** Load the string from the contents of a window. Normally, the
	entire text of the window is retrieved, but if you are only
	interested in a certain number of characters, this can be specified in
	the nMaxChars value. This value specifies the number of characters
	to get, excluding the NULL terminating character. So if you want 2
	characters, pass 2 for nMaxChars. 3 total bytes will be allocated
	and 2 characters copied to the buffer.*/
	VSTRING	Load(	HWND	hWnd,
					VUINT	nMaxChars =
								0)
		{ return Append(hWnd, nMaxChars, VFALSE); }
};

#endif /* VWINDOWSTRING*/