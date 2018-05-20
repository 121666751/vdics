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
Inspired by the work of Don Box's (dbox@develop.com) String816 class.
*/

#ifndef VSTRINGCONVERTER
#define VSTRINGCONVERTER

#include "../vstandard.h"

/** VStringConverter can be used anytime you need to convert ASCII to
UNICODE (wide) strings. The class is designed to work on as "as needed"
basis. This is, it only performs the conversion once, and then only
when asked. The design behind this class is that it can be initialized
with any type of string, UNICODE or ASCII, and can return any type of
string required by a function or method. The GetWide() and GetASCII()
methods are never really needed, as appropriate operators are designed
to return whatever string type is required, calling these methods to
handle the conversion.

VStringConverter is NOT a VString object, and knows nothing about VString.
It works on the lowest common denominator of strings, the native ASCII
and UNICODE data types. VString can provide these data types, so anywhere
you see a reference to a string pointer, VString objects can be used.

VStringConverter objects MUST go out of scope before, or at least not be
used after, the string they are providing conversions for has gone out
of scope or been destroyed. The class must hold a pointer to the string
to convert, and if this pointer is voided outside the scope of this class,
an attempt by this class to use it will certainly result in a segmentation
fault. VStringConverter is ideally suited to convert strings when used
as a parameter to a function or method.*/
class VStringConverter
{
public:
    /** Constructor must be initialized with an ASCII string.*/
	VStringConverter(VSTRING_CONSTA psz)
		{ Init(NULL, psz, VFALSE); }

    /** Constructor must be initialized with a UNICODE (wide) string.*/
    VStringConverter(VSTRING_CONSTW psz)
		{ Init(psz, NULL, VTRUE); }

    /** Destructor verifies internal memory allocations are released.*/
	virtual ~VStringConverter()
    {
		if ( m_bIsWide )
			free(m_pszA);
		else
			free(m_pszW);
    }

    /** Operator returns a const UNICODE string.*/
	operator VSTRING_CONSTW ()
		{ return GetWide(); }

    /** Operator returns a non-const UNICODE string.*/
    operator VSTRINGW ()
		{ return GetWide(); }

    /** Operator returns a const ASCII string.*/
    operator VSTRING_CONSTA ()
		{ return GetASCII(); }

    /** Operator returns a non-const ASCII string.*/
    operator VSTRINGA ()
		{ return GetASCII(); }

	/** Duplicate to ASCII, as needed.*/
	VSTRINGA	GetASCII()
	{
		/* This can only be called once per class. Other times
		we return the result of previous conversion. It is also
		only valid to use this function when the object was
		constructed with a WIDE string.*/
		if ( !m_pszA && m_bIsWide && m_pszW )
		{
			size_t nCount = VSTRLENW(m_pszW);

			if ( nCount )
			{
				nCount++;
				m_pszA = (VSTRINGA)malloc(nCount * sizeof(wchar_t));

				if ( m_pszA )
					wcstombs(m_pszA, m_pszW, nCount);
			}
		}
		
		return m_pszA;
	}

	/** Duplicate to wide, as needed.*/
	VSTRINGW	GetWide()
	{
		/* This can only be called once per class. Other times
		we return the result of previous conversion. It is also
		only valid to use this function when the object was
		constructed with an ASCII string.*/
		if ( !m_pszW && !m_bIsWide && m_pszA )
		{
			size_t nCount = VSTRLENA(m_pszA);

			if ( nCount )
			{
				nCount++;
				m_pszW = (VSTRINGW)malloc(nCount * sizeof(wchar_t));

				if ( m_pszW )
					mbstowcs(m_pszW, m_pszA, nCount);
			}
		}
		
		return m_pszW;
	}

private:
    /** Internal function to initialize the class.*/
	void		Init(	VSTRING_CONSTW	pszW,
						VSTRING_CONSTA	pszA,
						VBOOL			bIsWide)
	{
		/* Initialize members.*/
		m_pszW =	(VSTRINGW)pszW;
		m_pszA =	(VSTRINGA)pszA;
		m_bIsWide = bIsWide;
	}

	/** Embedded Members.*/
	VSTRINGW	m_pszW;
    VSTRINGA	m_pszA;
    VBOOL		m_bIsWide;
};

#endif /* VSTRINGCONVERTER*/