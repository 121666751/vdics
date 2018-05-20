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

#ifndef VLINEFILEREADER
#define VLINEFILEREADER

#include "../vstandard.h"
#include "../strings/vstringconverter.hpp"
#include "vtextstreamencoding.hpp"

/** VLineFileReader is used to read a text file, one line at a time. It
can read ASCII and UNICODE files, and calls the OnLine() virtual function
each time a line is read. It can process any text files where the encoding
is one supported by the VTextStreamEncoding class. VTextStreamEncoding is
quite capable of reading different formats, but in all cases, OnLine() will
be called with the string format used by the target. For instance, if you
are compiling for UNICODE and reading an ASCII file, the ASCII lines will
be converted to UNICODE before calling OnLine(). If you are compiling for
ASCII and reading a UNICODE file, the UNICODE lines will be converted to
ASCII before calling the ASCII version of OnLine().*/
class VLineFileReader
{
public:
	/** Virtual destructor does nothing.*/
	virtual ~VLineFileReader()
		{;}

	/** Start reading a file. For each line found, OnLine() will be called.
	If the specified file was opened, the function returns VTRUE, otherwise
	it returns VFALSE. If an error occurs, or the file encoding cannot be
	determined, VFALSE will also be returned. Each line in the file can be up
	to 16k characters in size. If you need to read more that 16k per line, you
	can specify the nMaxCharsPerLine parameter. OnLine() will be called with a
	NULL pointer for pszString when empty lines are encountered. If you would
	rather not have OnLine() called for empty lines, pass VFALSE for
	bCallOnLineForEmptyLines. The line number passed to OnLine() is only
	updated if OnLine() is called.*/
	VBOOL			Read(	VSTRING_CONST	pszFileName,
							VBOOL			bCallOnLineForEmptyLines =
												VTRUE,
							VUINT			nMaxCharsPerLine =
												0)
	{
		VASSERT(VSTRLEN_CHECK(pszFileName))

		FILE* pFILE =
			(pszFileName) ? VFOPEN(pszFileName, VTEXT("rb")) : NULL;

		if ( pFILE )
		{
			Read(pFILE, bCallOnLineForEmptyLines, nMaxCharsPerLine);
			fclose(pFILE);

			/* Success.*/
			return VTRUE;
		}

		return VFALSE;
	}

	/** Same as above, but allows you to specify an already open
	file pointer. The file stream passed as pFILE will be reset by this
	function to start at the beginning of the stream. On exit, the file
	position will be after the last read line, probably the end of file.*/
	VBOOL			Read(	FILE*	pFILE,
							VBOOL	bCallOnLineForEmptyLines =
										VTRUE,
							VUINT	nMaxCharsPerLine =
										0)
	{
		VASSERT(pFILE)

		if ( !pFILE )
			return VFALSE;

		/* Determine file encoding, leaving the file position indicator
		pointing past the Byte Order Mark, if used.*/
		VUINT nEncoding = VTextStreamEncoding::GetFileEncoding(pFILE, VFALSE);

		/* Did we determine the encoding?*/
		if ( !VTextStreamEncoding::IsKnown(nEncoding) )
			return VFALSE;

		/* Default characters per line?*/
		if ( !nMaxCharsPerLine )
			nMaxCharsPerLine = 16 * 1024;

		/* Determine memory requirements.*/
		VUINT nAllocSize = nMaxCharsPerLine;
		
		/* Double it for UNICODE files.*/
		if ( VTextStreamEncoding::IsUNICODE(nEncoding) )
			nAllocSize *= 2;
				
		/* Allocate memory.*/
		VMemoryBlock memBlock;

		if ( !memBlock.Alloc(nAllocSize) )
			return VFALSE;

		/* We keep track of line number.*/
		VUINT nLineNumber = 0;

		/* Store character count here.*/
		VUINT nCharCount;

		/* Assume failure.*/
		VBOOL bResult = VFALSE;

		/* Determine if this encoding is UNICODE or not.*/
		if ( VTextStreamEncoding::IsUNICODE(nEncoding) )
		{
			VSTRINGW psz = (VSTRINGW)memBlock.Get();
			
			/* Read a line.*/
			while (	VTextStreamEncoding::ReadLine(	pFILE,
													psz,
													nMaxCharsPerLine - 1,
													nEncoding,
													&nCharCount) )
			{
				/* We don't call OnLine() for empty lines unless
				bCallOnLineForEmptyLines is VTRUE.*/
				if ( nCharCount || bCallOnLineForEmptyLines )
				{
					/* Remember that we called OnLine() at least once.*/
					bResult = VTRUE;

					/* Offset line count.*/
					nLineNumber++;

					/* Call OnLine().*/
					if ( nCharCount )
					{
						if ( !OnLine(	VStringConverter(psz),
										nLineNumber,
										nCharCount) )
						{
							break;
						}
					}
					else
					{
						if ( !OnLine(NULL, nLineNumber, 0) )
							break;
					}
				}
			}
		}
		else
		{
			VSTRINGA psz = (VSTRINGA)memBlock.Get();
			
			/* Read a line.*/
			while (	VTextStreamEncoding::ReadLine(	pFILE,
													psz,
													nMaxCharsPerLine - 1,
													&nCharCount) )
			{
				/* We don't call OnLine() for empty lines unless
				bCallOnLineForEmptyLines is VTRUE.*/
				if ( nCharCount || bCallOnLineForEmptyLines )
				{
					/* Remember that we called OnLine() at least once.*/
					bResult = VTRUE;

					/* Offset line count.*/
					nLineNumber++;

					/* Call OnLine().*/
					if ( nCharCount )
					{
						if ( !OnLine(	VStringConverter(psz),
										nLineNumber,
										nCharCount) )
						{
							break;
						}
					}
					else
					{
						if ( !OnLine(NULL, nLineNumber, 0) )
							break;
					}
				}
			}
		}

		return bResult;
	}

protected:
	/** This function is called when reading a file once for each line
	in the file. If the file should continue to be read, return VTRUE from an
	override of this function, otherwise return VFALSE to stop processing.
	This function will be called for empty lines, if the
	bCallOnLineForEmptyLines parameter to Read() was set, and pszString will
	be NULL, otherwise pszString will be 0 terminated, with carriage returns
	and linefeeds removed. nLineNumber is 1 based, so the first line read will
	cause this value to be 1, not 0. nCharCount is the number of characters in
	pszString.*/
	virtual VBOOL	OnLine(	VSTRING	pszString,
							VUINT	nLineNumber,
							VUINT	nCharCount)
		{ return VTRUE; }
};

#endif /* VLINEFILEREADER*/
