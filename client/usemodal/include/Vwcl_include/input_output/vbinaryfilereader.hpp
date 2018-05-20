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

#ifndef VBINARYFILEREADER
#define VBINARYFILEREADER

#include "../vstandard.h"
#include "../utility/vmemoryblock.hpp"

/** VBinaryFileReader reads an entire binary file
into a VMemoryBlock object.*/
class VBinaryFileReader
{
public:
	/** Default constructor does nothing.*/
	VBinaryFileReader()
		{;}

	/** Virtual destructor does nothing.*/
	virtual ~VBinaryFileReader()
		{;}

	/** Read an entire binary file with an already open FILE stream. Returns
	the number of bytes read on success, 0 on failure. The file contents are
	read into the VMemoryBlock object, which will not be modified on failure.
	This version of the method requires that the number of bytes to be read
	be passed as the nLength parameter. The number of bytes actually read may
	be smaller than nLength. On success, the memory block at memBlock will
	contain EXACTLY the number of bytes actually read, which may be smaller
	than the nLength parameter. The file position within pFILE will not be
	modified before reading the content, and will be reset to the position
	on exit where the position was on entry to this method.*/
	size_t	Read(	VMemoryBlock&	memBlock,
					FILE*			pFILE,
					size_t			nLength)
	{
		VASSERT(pFILE)
		VASSERT(nLength)

		/* Validate parameters.*/
		if ( !pFILE || !nLength )
			return 0;

		/* Local memory block used for now.*/
		VMemoryBlock memBlockLocal;

		/* Allocate memory for content.*/
		if ( !memBlockLocal.Alloc(nLength) )
			return 0;

		/* Remember current file position.*/
		fpos_t posOriginal;

		if ( fgetpos(pFILE, &posOriginal) != 0 )
			return 0;

		/* Read file into memory.*/
		size_t nRead = fread(	memBlockLocal.Get(),
								1,
								nLength,
								pFILE);

		/* Did we read anything?*/
		if ( nRead )
		{
			/* Allocate exactly enough memory for what we read and
			copy from our temporary block to the parameter block.*/
			if ( memBlock.Alloc(nRead) )
				memcpy(memBlock.Get(), memBlockLocal.Get(), nRead);
			/* Indicate failure now.*/
			else
				nRead = 0;
		}

		/* Always set to original position.*/
		fsetpos(pFILE, &posOriginal);

		return nRead;
	}

	/** Read an entire binary file when given a file name string. Returns
	the number of bytes read on success, 0 on failure. The file contents are
	read into the VMemoryBlock object, which will not be modified on failure.
	On success, the memory block at memBlock will contain EXACTLY the number
	of bytes actually read.*/
	size_t	Read(	VMemoryBlock&	memBlock,
					VSTRING_CONST	pszFileName)
	{
		VASSERT(VSTRLEN_CHECK(pszFileName))

		/* Initialize result.*/
		size_t nRead = 0;

		/* Determine the file size.*/
		VSTATSTRUCTURE fileSTAT;
		VZEROSTRUCT(fileSTAT);

		if (	VSTAT(pszFileName, &fileSTAT) == 0 &&
				fileSTAT.st_size )
		{
			/* Open the file in binary mode.*/
			FILE* pFILE = VFOPEN(pszFileName, VTEXT("rb"));

			if ( pFILE )
			{
				/* Read the file now.*/
				nRead = Read(memBlock, pFILE, fileSTAT.st_size);

				/* Close the file.*/
				fclose(pFILE);
			}
		}

		return nRead;
	}
};

#endif /* VBINARYFILEREADER*/
