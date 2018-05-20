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

#ifndef VTEXTFILEREADER
#define VTEXTFILEREADER

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "../strings/vstringmanipulator.hpp"
#include "vlinefilereader.hpp"

/** VTextFileReader reads an entire text file into a VString object. Since
it gets its file reading capabilities from the VLineFileReader base class,
and because that class uses VTextStreamEncoding, it can read and process
the file formats supported by these classes. This includes various UNICODE
and ASCII encodings. See these classes for more information. Use the
base class VLineFileReader methods to start reading and processing the file.
To optimize the reading process and performance, you can alter the granularity
of the VString object passed on the constructor, and the parameters and
other options in the VLineFileReader base class.*/
class VTextFileReader : public VLineFileReader, public VStringManipulator
{
public:
	/** Constructor must be initialized with a VString object. Text file
	context will be appended into this string.*/
	VTextFileReader(VString& str)
		: VStringManipulator(str)
	{
		/* Initialize members to defaults.*/
		m_strEndOfLineMarker = VTEXT("\r\n");
	}

	/** Virtual destructor does nothing.*/
	virtual ~VTextFileReader()
		{;}

	/** As each line is read and added to the string, a terminator character
	or string will be appended to each line. The default is a carriage return
	AND linefeed (CRLF). If you would like to use some other "end of line"
	indicator, this can be set by modifying the contents of the string
	returned as a reference from this method. An empty string is valid, which
	will cause no end of line markers to be placed into the string.*/
	VString&		GetEndOfLineMarker() const
		{ return (VString&)m_strEndOfLineMarker; }

protected:
	/** This function is called when reading a file once for each line
	in the file. If the file should continue to be read, return VTRUE from an
	override of this function, otherwise return VFALSE to stop processing.
	As lines are read, the string passed to this objects constructor will be
	updated with the line contents. The "end of line" marker will also be
	inserted into the string, if not empty. This can be controlled by the
	GetEndOfLineMarker() method. This method is an override of the base
	class VLineFileReader method.*/
	virtual VBOOL	OnLine(	VSTRING	pszString,
							VUINT	nLineNumber,
							VUINT	nCharCount)
	{
		VASSERT(m_pString)
		
		/* Add this line?*/
		if ( pszString )
			m_pString->Append(pszString, 0, nCharCount);

		/* Add end of line marker?*/
		if ( m_strEndOfLineMarker.IsNotEmpty() )
			m_pString->Append(	m_strEndOfLineMarker,
								0,
								m_strEndOfLineMarker.GetLength());

		/* Keep reading.*/
		return VTRUE;
	}
	
	/** Embedded Members.*/
	VString			m_strEndOfLineMarker;
};

#endif /* VTEXTFILEREADER*/
