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

#ifndef VDELIMITEDSTRING
#define VDELIMITEDSTRING

#include "../vstandard.h"

/** VDelimitedString supports walking a string of delimited values. This
class DOES modify the original string, which the caller owns. You should
copy string if this is not desired. The original string will be fixed only
if Next() is called until it returns NULL, or if FixLast() is called.*/
class VDelimitedString
{
public:
	/** Default constructor must be initialized with a C style string, or
	NULL. In addition, the delimiter character can be specified in
	nDelimiterChar. A comma separated string is the default.*/
	VDelimitedString(	VSTRING	pszString =
									NULL,
						VCHAR	nDelimiterChar =
									VTEXT(','))
	{
		m_pszString =				NULL;
		m_nLastTerminatedAtIndex =	INDEX_UNPARSED;
		Reset(pszString, nDelimiterChar);
	}

	/** Get the next, or first, element of the string and move offset past
	next delimiter. Returns NULL when the string contains no more
	delimiters. This function does modify the internal string by
	inserting 0 terminators as needed. This will be corrected on the next
	call to Next(), or if FixLast() is called.*/
	VSTRING	Next(VBOOL bFixLast = VTRUE)
	{
		VASSERT(m_nDelimiterChar)

		/* The result is where we left off last time, for now, or, if we
		have not done anything, the string position. If called after parsing
		is complete, return NULL.*/
		VSTRING pszResult =
			(m_pszString && m_nLastTerminatedAtIndex != INDEX_PARSED)
			? m_pszString + (m_nLastTerminatedAtIndex + 1)
			: NULL;

		if ( pszResult )
		{
			/* Fix last string, if needed.*/
			if ( bFixLast )
				FixLast();

			/* If we are now on a terminator, the string had a
			trailing separator char.*/
			if ( pszResult[0] == VTEXT('\0') )
			{
				pszResult =					NULL;
				m_nLastTerminatedAtIndex =	INDEX_PARSED;
			}
			else
			{
				VSTRING pszNext =
					VSTRCHR(pszResult, m_nDelimiterChar);

				if ( pszNext )
				{
					/* Terminate the string here.*/
					m_nLastTerminatedAtIndex =
						pszNext - m_pszString;
					
					m_pszString[m_nLastTerminatedAtIndex] =
						VTEXT('\0');
				}
				/* Done with parsing.*/
				else
					m_nLastTerminatedAtIndex = INDEX_PARSED;
			}
		}

		return pszResult;
	}

	/** Fix (un-terminate) the last termination this class performed
	in a previous call to Next().*/
	void	FixLast()
	{
		VASSERT(m_nDelimiterChar)

		if ( m_pszString && m_nLastTerminatedAtIndex >= 0 )
			m_pszString[m_nLastTerminatedAtIndex] = m_nDelimiterChar;
	}

	/** Reset internal buffers (to use the same class again with a different
	string). If pszString is NULL, this will reset with the same string as
	used before. You can also change the delimiter character with this
	function.*/
	void	Reset(	VSTRING	pszString =
								NULL,
					VCHAR	nDelimiterChar =
								VTEXT(','))
	{
		VASSERT(nDelimiterChar)

		m_nDelimiterChar =			nDelimiterChar;
		m_pszString =				(pszString) ? pszString : m_pszString;
		m_nLastTerminatedAtIndex =	INDEX_UNPARSED;
	}

protected:
	/** These constants are used internally by the class.*/
	enum	{	INDEX_UNPARSED =	-1,
				INDEX_PARSED =		-2,
			};

	/** Embedded Members.*/
	VCHAR		m_nDelimiterChar;
	VSTRING		m_pszString;
	VLONG		m_nLastTerminatedAtIndex;
};

#endif /* VDELIMITEDSTRING*/
