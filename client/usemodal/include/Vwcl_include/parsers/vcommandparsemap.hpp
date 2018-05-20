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

#ifndef VCOMMANDPARSEMAP
#define VCOMMANDPARSEMAP

#include "../vstandard.h"
#include "../collections/vcommandparameterpair.hpp"
#include "../collections/vcommandparameterpairlist.hpp"
#include "../parsers/vdelimitedstring.hpp"
#include "../strings/vstring.hpp"

/** VCommandParseMap wraps parsing command strings into their
command-parameter parts. This is most useful when developing communications
protocols or CGI (Web) form processing. The idea is that you have a string
that contains Command=Parameter pairs. For instance: VString s contains the
following sequence: <SENDFILE=C:\TEST.HPP><DOIT=NOW>. Running this string
through the Parse() command would yield a list of VCommandParameterPair
objects. If 2 pointers to these objects were named pPair1 and pPair2, the
result would look like this:

Example:	pPair1 would be the string SENDFILE
			pPair1->GetParameter() would be the string C:\TEST.HPP
			pPair2 would be the string DOIT
			pPair2->GetParameter() would be the string NOW

Parameter values may be empty strings. In reality <=> are not used, but rather
non-printing ASCII values of 1, 2, and 3 are used to designate the start
of the command, the start of a parameter, and the end of a command. This
allows the command string to contain other printable codes. The actual
delimiters can be any 3 characters you wish, including the "human readable"
<=> which can be set by calling SetHumanReadable(), or each delimeter can
be set manually with SetDelimiters(). The marker in the string that defines
the start of a command is called the marker command and is represented by
default as MARKER_COMMAND (ASCII 1). The marker in the string that defines
the start of a parameter is called the marker parameter and is represented
by default as MARKER_PARAMETER  (ASCII 2). The marker in the string that
defines the end of the command-parameter pair is called the marker end and
is represented by default as MARKER_END (ASCII 3). MARKER_COMMAND_ANYTHING
is a special case, which means that when parsing a string, don't look for
the MARKER_COMMAND character, but rather ANYTHING is valid. This is mostly
used for parsing URL encoded strings from HTML forms where nothing
signifies the start of a name-value pair. In this case, = is used for the
parameter marker and & is used as the end marker.

This class gets most of its functionality from the VCommandParameterPairList
base class, which actually manages the list of command and parameter pairs.
See this class for more information on sorting, searching, etc.*/
class VCommandParseMap : public VCommandParameterPairList
{
public:
	/** Markers (delimeters, ASCII values).*/
	enum	{	MARKER_COMMAND =			VTEXTA('\1'),
				MARKER_PARAMETER =			VTEXTA('\2'),
				MARKER_END =				VTEXTA('\3'),
				MARKER_COMMAND_ANYTHING =	VTEXTA('\0'),
			};

	/** Default constructor initializes class members and can optionally
	place the class into "Human Readable" mode, where the marker command
	is <, the marker parameter is =, and the marker end is > ASCII value.
	It is considered valid to have more than one entry for each command. If
	you would rather have only one VCommandParameterPair object created for
	each command, pass a non-zero value for the nParameterSeparator. See the
	base class VCommandParameterPairList for more information about how this
	is used. The use of these overlapping parameters can also be set after
	construction by using the GetParameterSeparator() function. If used, it
	should be set before calling the Parse() method.*/
	VCommandParseMap(	VBOOL bHumanReadable =
								VFALSE,
						VCHAR nParameterSeparator =
								0)
		: VCommandParameterPairList(nParameterSeparator)
	{
		if ( bHumanReadable )
			SetHumanReadable();
		else
		{
			m_nMarkerCommand =		MARKER_COMMAND;
			m_nMarkerParameter =	MARKER_PARAMETER;
			m_nMarkerEnd =			MARKER_END;
		}
	}

	/** Virtual destructor does nothing.*/
	virtual ~VCommandParseMap()
		{;}

	/** Parse a string into command-parameter pairs. Commands are inside
	m_nMarkerCommand and m_nMarkerEnd delimeter tags and parameters follow
	the m_nMarkerParameter inside of each command. This function honors
	various settings for the marker beginning tag as defined by the class
	information at the top of this header file. See that section for more
	information. Returns the number of commands added to the list, 0 for none.
	If this method is called while a collection of commands exists, those
	objects stored will be deleted first. On error, previous command objects
	in the list will remain intact and not modified in any way. pszString
	will be modified while this function is running, but will be repaired
	before exit.*/
	VUINT								Parse(VSTRING pszString)
	{
		VASSERT(pszString)

		VUINT nLength = (pszString) ? VSTRLEN(pszString) : 0;

		if ( !nLength )
			return 0;

		/* As a check against something bad, check the first byte to
		make sure it is m_nMarkerCommand. If no marker command is used
		we have to skip this check.*/
		if (	m_nMarkerCommand != MARKER_COMMAND_ANYTHING &&
				pszString[0] != m_nMarkerCommand )
		{
			return 0;
		}

		/* Clear current list contents.*/
		Free();

		/* Use a VDelimitedString parser for parsing on end tags.*/
		VDelimitedString	ds(pszString, m_nMarkerEnd);
		VSTRING				pszCurrent;

		/* Process string into individual parts for now.*/
		while ( (pszCurrent = ds.Next()) )
		{
			/* Determine where the command starts.*/
			VSTRING pszCommand =	pszCurrent;
			VSTRING pszParameter =	NULL;

			if ( m_nMarkerCommand != MARKER_COMMAND_ANYTHING )
			{
				/* We expect this to be the marker!*/
				VASSERT(pszCommand[0] == m_nMarkerCommand)

				if ( pszCommand[0] == m_nMarkerCommand )
					pszCommand++;
			}

			/* Find where the parameter begins.*/
			VSTRING pszParam = VSTRCHR(pszCommand, m_nMarkerParameter);

			if ( pszParam )
			{
				/* Terminate here.*/
				pszParam[0] = VTEXT('\0');

				/* Save parameter pointer.*/
				pszParameter = pszParam + 1;
			}

			/* Add this command and parameter.*/
			AddNewPair(pszCommand, pszParameter);
		}

		return GetCount();
	}

	/** Set the marker command, parameter, and marker end delimiter
	characters. This should be done before calling Parse().*/
	void							SetDelimiters(	VCHAR nMarkerCommand,
													VCHAR nMarkerParameter,
													VCHAR nMarkerEnd)
	{
		m_nMarkerCommand =		nMarkerCommand;
		m_nMarkerParameter =	nMarkerParameter;
		m_nMarkerEnd =			nMarkerEnd;
	}

	/** Set the marker command, parameter, and marker end delimiter
	characters to <, =, and > respectively. This should be done before
	calling Parse().*/
	void							SetHumanReadable()
		{ SetDelimiters(VTEXT('<'), VTEXT('='), VTEXT('>')); }

protected:
	/** Embedded Members.*/
	VCHAR							m_nMarkerCommand;
	VCHAR							m_nMarkerParameter;
	VCHAR							m_nMarkerEnd;
};

#endif /** VCOMMANDPARSEMAP*/
