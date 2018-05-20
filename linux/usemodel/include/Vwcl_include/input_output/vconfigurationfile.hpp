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

#ifndef VCONFIGURATIONFILE
#define VCONFIGURATIONFILE

#include "../vstandard.h"
#include "../collections/vcommandparameterpairlist.hpp"
#include "vlinefilereader.hpp"

/** VConfigurationFile is used to read configuration files. This is similar
to both the Windows style INI file reading, and the UNIX style CONF file
reading, though not exact implementations of either. This class is used to
process configuration files that are plain text, where options are
expressed as command and parameter pairs. For instance, if this line
appears in a configuration file:

FONT=Sans Serif

FONT is considered the command, and Sans Serif is the parameter. Commands
are always required, but parameters are optional. So this line is also
valid, though quite meaningless:

FONT

Commands and parameters are separated by the equal (=) sign. White space is
removed from the command and parameters. Only white space that appears left
and right are removed. Commands and parameters are allowed to have internal
spaces. In other words, this is also valid:

OPTIONS 1 = My Test Option

which means that the command is OPTION 1 and the parameter is My Test
Option. This class keeps these command and parameter pairs as a linked list
of VCommandParameterPair objects. It is normally valid to have more than one
command in the list. For example, this results in 2 VCommandParameterPair
objects being created:

COMMAND1=PARAMETER1
COMMAND1=PARAMETER2

This could be difficult to manage in your code. Support for overlapping, or
sharing, of parameters is provided by the VCommandParameterPairList base
class. If you wish to use parameter overlap, you should set this option using
the GetParameterSeparator() of VCommandParameterPairList and setting the
reference returned to your string separator character. The default is to
disable this functionality. You would normally want to set this value before
calling the the Read() function to read the configuration file.*/
class VConfigurationFile :	public VCommandParameterPairList,
							public VLineFileReader
{
public:
	/** Constructor initializes class. When reading a configuration file, it
	is considered valid to have more than one entry for each command. If you
	would rather have only one VCommandParameterPair object created for each
	command, pass a non-zero value for the nParameterSeparator. See the base
	class VCommandParameterPairList for more information about how this is
	used. The use of these overlapping parameters can also be set after
	construction by using the GetParameterSeparator() function. If used, it
	should be set before calling the Read() method.*/
	VConfigurationFile(VCHAR nParameterSeparator = 0)
		: VCommandParameterPairList(nParameterSeparator)
		{;}

	/** Virtual destructor does nothing.*/
	virtual ~VConfigurationFile()
		{;}

	/** Read the specified configuration file. This will reset the internal
	linked list of pair objects.*/
	VBOOL						Read(VSTRING_CONST pszFileName)
	{
		VASSERT(VSTRLEN_CHECK(pszFileName))

		/* Clear linked list.*/
		Free();

		/* Let base class do the real work of reading the file. We instruct
		it to not call our OnLine() method for emtpy lines.*/
		return VLineFileReader::Read(pszFileName, VFALSE);
	}

	/** Same as above, but allows you to specify an already open
	file pointer. Returns VTRUE if the base class Read() function
	called this classes OnLine() method at least once.*/
	VBOOL						Read(FILE* pFILE)
	{
		VASSERT(pFILE)

		/* Clear linked list.*/
		Free();

		/* Let base class do the real work of reading the file. We instruct
		it to not call our OnLine() method for emtpy lines.*/
		return VLineFileReader::Read(pFILE, VFALSE);
	}

protected:
	/** This function is called by the base class for each line in the file.
	The base class implementation parses the line as needed. This function
	validates and parses the command and parameter strings, and then calls
	AddNewPair() to add the command and parameter to the list.*/
	virtual VBOOL			OnLine(	VSTRING	pszString,
									VUINT	nLineNumber,
									VUINT	nCharCount)
	{
		if ( pszString && nCharCount )
		{
			/* Look for comment line. These start with # or ;. Also
			check for = sign which is invalid.*/
			if (	pszString[0] != VTEXT('#') &&
					pszString[0] != VTEXT(';') &&
					pszString[0] != VTEXT('=') )
			{
				/* Initialize parameter value.*/
				VSTRING pszParameter = NULL;

				/* See if we can find an = sign.*/
				VSTRING pszEqual = VSTRCHR(pszString, VTEXT('='));

				if ( pszEqual )
				{
					/* Terminate string here.*/
					pszEqual[0] = VTEXT('\0');

					/* Offset parameter.*/
					pszParameter = pszEqual + 1;
				}
				
				/* Add new pair now.*/
				AddNewPair(pszString, pszParameter);
			}
		}

		/* Next line.*/
		return VTRUE;
	}
};

#endif /* VCONFIGURATIONFILE*/
