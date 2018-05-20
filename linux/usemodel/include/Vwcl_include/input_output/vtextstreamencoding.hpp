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

#ifndef VTEXTSTREAMENCODING
#define VTEXTSTREAMENCODING

#include "../vstandard.h"

/** VTextStreamEncoding is used to determine the type of encoding
used by a string or file. It also has methods for reading UNICODE
and ASCII files, and handling URL encoding.*/
class VTextStreamEncoding
{
public:
	/** Class constants.*/
	enum
	{
		/** This constant represents the minimum number of Byte Order Mark
		bytes that must be supported for calling the
		GetEncodingFromByteOrderMark() method. The string passed to this
		method must be at least this many character long, although it can
		have 0 terminators if the entire length is not known.*/
		GEFBOM_MIN_LENGTH =	3,
	};

	/** File encoding constants understood by this class.*/
	enum
	{
		/** The encoding is plain ASCII. This will be ASCII codes 0-127.*/
		ASCII,
		/** The encoding is plain ASCII. This will be ASCII codes 0-255.*/
		ASCII_EXTENDED,
		/** The encoding is UNICODE UTF-8.*/
		UNICODE_UTF8,
		/** The encoding is UNICODE UTF-16 Big Endian.*/
		UNICODE_UTF16_BE,
		/** The encoding is UNICODE UTF-16 Little Endian.*/
		UNICODE_UTF16_LE,
		/** The encoding is unknown or not text.*/
		UNKNOWN,
		/** The stream could not be read.*/
		NOT_READ,
	};

	/** Given an array of VBYTE's, determine the encoding used. Returns
	one of the enumerated values above. pBytes does not have to be zero
	terminated, but it MUST be at least GEFBOM_MIN_LENGTH bytes in length.
	If you are interested in knowing how many bytes comprised the Byte Order
	Mark, pass a pointer to a VUINT type as the pByteCount parameter.*/
	static VUINT	GetEncodingFromByteOrderMark(	VBYTE*		pBytes,
													VUINT*		pByteCount =
																	NULL)
	{
		VASSERT(pBytes)

		/* Assume we won't figure out the type.*/
		VUINT nEncoding = UNKNOWN;

		/* Initialize out param?*/
		if ( pByteCount )
			*pByteCount = 0;

		if ( !pBytes )
			return nEncoding;

		VUINT nByteCount = 0;

		/* Look for UNICODE UTF-16 BE.*/
		if ( pBytes[0] == 0xfe && pBytes[1] == 0xff )
		{
			nByteCount =	2;
			nEncoding =		UNICODE_UTF16_BE;
		}
		/* Look for UNICODE UTF-16 LE.*/
		else if ( pBytes[0] == 0xff && pBytes[1] == 0xfe )
		{
			nByteCount =	2;
			nEncoding =		UNICODE_UTF16_LE;
		}
		/* Look for UNICODE UTF-8.*/
		else if ( pBytes[0] == 0xef && pBytes[1] == 0xbb && pBytes[2] == 0xbf )
		{
			nByteCount =	3;
			nEncoding =		UNICODE_UTF8;
		}
		else
		{
			/* This part of the routine is coded like this to calm the GCC
			compiler. If we compare pBytes[0] to a value in the range of
			0-255, like we do, it complains about the expression always
			returning 1, since the range of the data type is too small. This
			is bogus, and our code is correct, but to keep GCC from throwing
			errors all over the place we patch it here by upcasting the
			byte to a VUINT first.*/
			VUINT nFirstByte = pBytes[0];

			/* Look for ASCII.*/
			if ( nFirstByte >= 0x00 && nFirstByte <= 0x7f )
				nEncoding = ASCII;
			/* Look for Extended ASCII.*/
			else if ( nFirstByte >= 0x00 && nFirstByte <= 0xff )
				nEncoding = ASCII_EXTENDED;
		}

		/* Set out param?*/
		if ( pByteCount )
			*pByteCount = nByteCount;

		return nEncoding;
	}

	/** Read the first bytes of a file to determine the file encoding. It will
	return one of the enumerated values above.This version accepts an already
	open FILE pointer. This method will reset the FILE stream to the original
	seek position after it completes its processing. If you would rather this
	method NOT reset the file pointer, leaving it at the first byte position
	after the Byte Order Mark, you can pass VFALSE for the
	bRestoreFilePosition parameter.*/
	static VUINT	GetFileEncoding(FILE*	pFILE,
									VBOOL	bRestoreFilePosition =
												VTRUE)
	{
		VUINT nEncoding = NOT_READ;

		if ( pFILE )
		{
			/* Assume failure now.*/
			nEncoding = NOT_READ;

			/* Save current file position.*/
			fpos_t posOriginal;
			
			if ( fgetpos(pFILE, &posOriginal) == 0 )
			{
				/* Seek to beginning of stream.*/
				fpos_t posTemp = 0;
				fsetpos(pFILE, &posTemp);
				
				/* We will read, at most, GEFBOM_MIN_LENGTH bytes.*/
				VBYTE nBytes[GEFBOM_MIN_LENGTH];
				VZEROMEMORY(nBytes, VARRAY_SIZE(nBytes));
				
				/* Read up to GEFBOM_MIN_LENGTH bytes.*/
				if (	fread(	nBytes,
								sizeof(VBYTE),
								VARRAY_SIZE(nBytes),
								pFILE) )
				{
					/* We need to determine how many bytes were actually part
					of the BOM so that we can reset the file position.*/
					VUINT nBOMLength = 0;

					/* Decipher encoding.*/
					nEncoding =
						GetEncodingFromByteOrderMark(nBytes, &nBOMLength);

					/* If not valid, always reset file position.*/
					if ( !IsKnown(nEncoding) )
						bRestoreFilePosition = VTRUE;

					/* Set the file pointer past the BOM.*/
					if ( !bRestoreFilePosition )
					{
						posTemp = nBOMLength;
						fsetpos(pFILE, &posTemp);
					}
				}

				/* Restore file position?*/
				if ( bRestoreFilePosition )
					fsetpos(pFILE, &posOriginal);
			}
		}

		return nEncoding;
	}

	/** Read the first bytes of a file to determine the file encoding. It will
	return one of the enumerated values above. This version accepts a file
	name to read from.*/
	static VUINT	GetFileEncoding(VSTRING_CONST pszFileName)
	{
		VASSERT(VSTRLEN_CHECK(pszFileName))

		VUINT nEncoding = NOT_READ;

		if ( pszFileName )
		{
			FILE* pFILE = VFOPEN(pszFileName, VTEXT("rb"));

			if ( pFILE )
			{
				nEncoding = GetFileEncoding(pFILE, VFALSE);
				fclose(pFILE);
			}
		}

		return nEncoding;
	}

	/** Given an encoding type from the above enumeration, determine if the
	encoding is ASCII.*/
	static VBOOL	IsASCII(VUINT nEncodingType)
	{
		switch ( nEncodingType )
		{
			case ASCII:
			case ASCII_EXTENDED:
				return VTRUE;
		}

		return VFALSE;
	}

	/** Given an encoding type from the above enumeration, determine if the
	encoding is Big Endian.*/
	static VBOOL	IsBigEndian(VUINT nEncodingType)
	{
		switch ( nEncodingType )
		{
			case UNICODE_UTF16_BE:
				return VTRUE;
		}

		return VFALSE;
	}

	/** Given an encoding type from the above enumeration, determine if the
	encoding is Little Endian.*/
	static VBOOL	IsLittleEndian(VUINT nEncodingType)
	{
		switch ( nEncodingType )
		{
			case UNICODE_UTF16_LE:
				return VTRUE;
		}

		return VFALSE;
	}

	/** Test a single character to see if it should mark the end of a line
	read from a stream (ASCII Version). This is either a carriage return
	or line feed.*/
	static VBOOL	IsLineTerminator(VCHARA nChar)
	{
		switch ( nChar )
		{
			case VTEXTA('\n'):
			case VTEXTA('\r'):
				return VTRUE;
		}

		return VFALSE;
	}

	/** Test a single character to see if it should mark the end of a line
	read from a stream (UNICODE Version). This is either a carriage return
	or line feed.*/
	static VBOOL	IsLineTerminator(VCHARW nChar)
	{
		switch ( nChar )
		{
			case VTEXTW('\n'):
			case VTEXTW('\r'):
				return VTRUE;
		}

		return VFALSE;
	}

	/** Given an encoding type from the above enumeration, determine if the
	encoding could not be determined.*/
	static VBOOL	IsKnown(VUINT nEncodingType)
	{
		switch ( nEncodingType )
		{
			case UNKNOWN:
			case NOT_READ:
				return VFALSE;
		}

		return VTRUE;
	}

	/** Given an encoding type from the above enumeration, determine if the
	encoding is UNICODE.*/
	static VBOOL	IsUNICODE(VUINT nEncodingType)
	{
		switch ( nEncodingType )
		{
			case UNICODE_UTF8:
			case UNICODE_UTF16_BE:
			case UNICODE_UTF16_LE:
				return VTRUE;
		}

		return VFALSE;
	}

	/** Given an encoding type, determine if byte swapping is required. This
	will be VTRUE if the encoded format is big endian on little endian
	systems, or little endian on big endian systems.*/
	static VBOOL	NeedsByteSwapping(VUINT nEncodingType)
	{
		if ( IsUNICODE(nEncodingType) && nEncodingType != UNICODE_UTF8 )
		{
			if ( VIsLittleEndian() )
				return IsBigEndian(nEncodingType);

			return IsLittleEndian(nEncodingType);
		}

		return VFALSE;
	}

	/** Given a HTTP parsed URL, convert + signs into spaces and %xx escape
	sequences into single character. The return value is the same pszURL
	string pointer passed to function. If pszURL is really a VString object,
	use the VString::ParseEncodedURL() instead of this one. This method will
	shorten pszURL if encoded data is found, so calling code should update
	their cache of the string length, if used.*/
	static VSTRING	ParseEncodedURL(VSTRING pszURL)
	{
		VASSERT(VSTRLEN_CHECK(pszURL))

		if ( pszURL )
		{
			register VINT	i, j;
			register VCHAR	digit;
			VSTRING			pszThisChar;

			for ( i = 0, j = 0; pszURL[j]; ++i, ++j )
			{
				pszURL[i] = pszURL[j];

				if ( pszURL[i] == '%' )
				{
					pszThisChar = &pszURL[j + 1];

					digit =
						(pszThisChar[0] >= VTEXT('A')
						? ((pszThisChar[0] & 0xdf) - VTEXT('A')) + 10
						: (pszThisChar[0] - VTEXT('0')));

					digit *= 16;

					digit +=
						(pszThisChar[1] >= VTEXT('A')
						? ((pszThisChar[1] & 0xdf) - VTEXT('A')) + 10
						: (pszThisChar[1] - VTEXT('0')));

					pszURL[i] = digit;

					j += 2;
				}
				else if ( pszURL[i] == VTEXT('+') )
					pszURL[i] = VTEXT(' ');
			}

			pszURL[i] = VTEXT('\0');
		}

		return pszURL;
	}

	/** Given a FILE stream pointer, read a single line from it. This method
	allows the file to have been opened in either TEXT or BINARY mode. This
	version of the method is used for reading ASCII files only. Returns VTRUE
	on success, VFALSE on failure (end of file). All new line characters read
	will be removed by this method, leaving only the string found at the line
	read. This method understands CRLF and NEWLINE line terminator characters.
	The file position will be offset to the start of the next line once
	this function completes, unless the size of the buffer pointed to by
	pszBuffer was not large enough to hold an entire line. In this case, the
	function will only read up to to maximum number of characters in the line,
	and the file position will be left in the middle of a line.

	You can think of this method as a safe version of fgets(). The major
	exception is that this routine can safely read TEXT and BINARY text,
	whereas fgets() only properly reads TEXT streams.
	
	pFILE is a pointer to an already open file stream. This can be a TEXT or
	BINARY mode stream. pszBuffer is a pointer to a ASCII character array, and
	nMaxChars is the size, in characters, of the pszBuffer array. nMaxChars
	includes the terminating 0, so if you want to read 2 bytes, pass 3 for
	this parameter, to make room for the terminator. If you are interested in
	the number of characters built into pszBuffer, you can pass a pointer to
	a VUINT value to receive this information as pnCharCount.*/
	static VBOOL	ReadLine(	FILE*		pFILE,
								VSTRINGA	pszBuffer,
								VUINT		nMaxChars,
								VUINT*		pnCharCount =
												NULL)
	{
		return InternalReadLine(pFILE,
								NULL,
								pszBuffer,
								nMaxChars,
								ASCII,
								pnCharCount);
	}

	/** Given a FILE stream pointer, read a single line from it. This method
	allows the file to have been opened in either TEXT or BINARY mode. This
	version of the method is used for reading UNICODE files only. Returns VTRUE
	on success, VFALSE on failure (end of file). All new line characters read
	will be removed by this method, leaving only the string found at the line
	read. This method understands CRLF and NEWLINE line terminator characters.
	The file position will be offset to the start of the next line once
	this function completes, unless the size of the buffer pointed to by
	pszBuffer was not large enough to hold an entire line. In this case, the
	function will only read up to to maximum number of characters in the line,
	and the file position will be left in the middle of a line.

	You can think of this method as a safe version of fgetws(). The major
	exception is that this routine can safely read UNICODE text, whereas
	fgetws() only reads MBCS characters. This method also converts the
	endianess of the file to the endianess of the target platform.
	
	pFILE is a pointer to an already open file stream. This can be a TEXT or
	BINARY mode stream. pszBuffer is a pointer to a wide character array, and
	nMaxChars is the size, in characters (not bytes), of the pszBuffer array.
	nMaxChars includes the terminating 0, so if you want to read 2 bytes, pass
	3 for this parameter, to make room for the terminator.

	The nEncodingType parameter must be a UNICODE encoding type defined in
	the above enumeration. If you are interested in the number of characters
	built into pszBuffer, you can pass a pointer to a VUINT value to receive
	this information as pnCharCount.*/
	static VBOOL	ReadLine(	FILE*		pFILE,
								VSTRINGW	pszBuffer,
								VUINT		nMaxChars,
								VUINT		nEncodingType,
								VUINT*		pnCharCount =
												NULL)
	{
		return InternalReadLine(pFILE,
								pszBuffer,
								NULL,
								nMaxChars,
								nEncodingType,
								pnCharCount);
	}

protected:
	/** Internal function to read UNICODE or ASCII file.*/
	static VBOOL	InternalReadLine(	FILE*		pFILE,
										VSTRINGW	pszBufferW,
										VSTRINGA	pszBufferA,
										VUINT		nMaxChars,
										VUINT		nEncodingType,
										VUINT*		pnCharCount)
	{
		/* These must be known.*/
		VASSERT(pFILE)
		VASSERT(nMaxChars)
		/* One of these must be set!*/
		VASSERT(pszBufferW || pszBufferA)
		/* If UNICODE, don't pass an ASCII string!*/
		VASSERT(pszBufferA || IsUNICODE(nEncodingType))

		/* Initialize out parameters.*/
		if ( pszBufferW )
			pszBufferW[0] = VTEXTW('\0');

		if ( pszBufferA )
			pszBufferA[0] = '\0';

		if ( pnCharCount )
			*pnCharCount = 0;

		/* Validate parameters.*/
		if ( !pFILE || !nMaxChars )
			return VFALSE;

		/* One of these must be set!*/
		if ( !pszBufferW && !pszBufferA )
			return VFALSE;

		if ( pszBufferW && !IsUNICODE(nEncodingType) )
			return VFALSE;

		/* Save current file position.*/
		fpos_t posOriginal;

		if ( fgetpos(pFILE, &posOriginal) != 0 )
			return VFALSE;

		/* Save some flags.*/
		VBOOL bNeedsByteSwap =			NeedsByteSwapping(nEncodingType);
		VBOOL bMachineIsLittleEndian =	VIsLittleEndian();

		VBOOL bErrorOut =	VFALSE;
		VUINT nTotalChars =	0;
		VUINT nBytesRead;
		VBYTE nBytes[32];
						
		/* Calculate character size. First base it on buffer type.*/
		VUINT nCharSize = (pszBufferW) ? sizeof(VCHARW) : sizeof(VCHARA);

		/* UTF-8 is single byte.*/
		if ( nEncodingType == UNICODE_UTF8 )
			nCharSize = 1;

		/* Read raw bytes from the stream.*/
		while ( !bErrorOut &&
				(nBytesRead = fread(nBytes,
									1,
									VARRAY_SIZE(nBytes),
									pFILE)) )
		{
			/* nBytesRead MUST ALWAYS be evenly divisible by 2 when reading
			double byte UNICODE files.*/
			VASSERT(nCharSize == 1 || nBytesRead % 2 == 0)

			/* Protect against incorrectly written UNICODE files.*/
			if ( nCharSize == 1 || nBytesRead % 2 == 0 )
			{
				VBOOL bBreakMainLoop =	VFALSE;
				VUINT nBufferOffSet =	nTotalChars;
				
				/* Process bytes.*/
				for ( VUINT i = 0; i < nBytesRead; i += nCharSize )
				{
					/* Make a UNICODE character from each 2 bytes?*/
					if ( pszBufferW )
					{
						if ( nEncodingType == UNICODE_UTF8 )
						{
							if ( bMachineIsLittleEndian )
								pszBufferW[nBufferOffSet] =
									VMAKE16BIT(nBytes[i], 0);
							else
								pszBufferW[nBufferOffSet] =
									VMAKE16BIT(0, nBytes[i]);
						}
						else
						{
							/* Byte swap now if needed.*/
							if ( bNeedsByteSwap )
								pszBufferW[nBufferOffSet] =
									VMAKE16BIT(nBytes[i + 1], nBytes[i]);
							else
								pszBufferW[nBufferOffSet] =
									VMAKE16BIT(nBytes[i], nBytes[i + 1]);
						}
					}
					/* Add byte to ASCII buffer.*/
					else
						pszBufferA[nBufferOffSet] = nBytes[i];

					/* See if we found a line terminator.*/
					if (	(pszBufferW &&
							IsLineTerminator(pszBufferW[nBufferOffSet])) ||
							(pszBufferA &&
							IsLineTerminator(pszBufferA[nBufferOffSet])) )
					{
						/* Reset file pointer to after this position.*/
						fpos_t posNew =
							posOriginal +
							(nCharSize * nBufferOffSet) +
							nCharSize;
						fsetpos(pFILE, &posNew);

						/* Eat remaining terminator, if any. We will eat,
						at most, 1 more character, since any remaining ones
						will be on the next line, not the one we are
						processing. If the next terminating character is
						the same one we just processed above, it will be
						considered to be on a new line, and skipped by this
						routine. In other words, we deal with Microsoft
						style CRLF pairs, as well as normal UNIX style
						terminators with only the newline.*/
						nBytesRead = fread(	nBytes,
											1,
											nCharSize,
											pFILE);
						
						if ( nBytesRead == nCharSize )
						{
							VBOOL bShouldOffSet = VFALSE;

							if ( pszBufferW )
							{
								VCHARW nThisChar;

								if ( nEncodingType == UNICODE_UTF8 )
								{
									if ( bMachineIsLittleEndian )
										nThisChar =
											VMAKE16BIT(nBytes[0], 0);
									else
										nThisChar =
											VMAKE16BIT(0, nBytes[0]);
								}
								else
								{
									/* Byte swap now if needed.*/
									if ( bNeedsByteSwap )
										nThisChar =
											VMAKE16BIT(nBytes[1], nBytes[0]);
									else
										nThisChar =
											VMAKE16BIT(nBytes[0], nBytes[1]);
								}

								/* Was this a terminator?
								Was it different from the last one we saw?*/
								if (	IsLineTerminator(nThisChar) &&
										nThisChar !=
										pszBufferW[nBufferOffSet] )
								{
									bShouldOffSet = VTRUE;
								}
							}
							else
							{
								/* Was this a terminator?
								Was it different from the last one we saw?*/
								if (	IsLineTerminator((VCHARA)nBytes[0]) &&
										nBytes[0] !=
										pszBufferA[nBufferOffSet] )
								{
									bShouldOffSet = VTRUE;
								}
							}

							if ( bShouldOffSet )
								posNew += nCharSize;
						}

						/* Set final file position.*/
						fsetpos(pFILE, &posNew);

						/* Set flag to exit loop.*/
						bBreakMainLoop = VTRUE;

						/* Exit this loop.*/
						break;
					}
					/* Don't go out of bounds.*/
					else if ( ++nBufferOffSet == nMaxChars - 1 )
					{
						bBreakMainLoop = VTRUE;
						break;
					}
				}
					
				/* Save total number of characters.*/
				nTotalChars = nBufferOffSet;

				/* Are we done?*/
				if ( bBreakMainLoop || nBytesRead < VARRAY_SIZE(nBytes) )
					break;
			}
			else
			{
				nTotalChars =	0;
				bErrorOut =		VTRUE;
			}
		}

		/* Did we read anything?*/
		if ( nTotalChars )
		{
			/* This should not be set!*/
			VASSERT(!bErrorOut)

			/* Terminate the string.*/
			if ( pszBufferW )
				pszBufferW[nTotalChars] = VTEXTW('\0');
			else
				pszBufferA[nTotalChars] = VTEXTA('\0');

			/* Set out parameter?*/
			if ( pnCharCount )
				*pnCharCount = nTotalChars;
		}
		/* Did we error out or hit the end of the file?*/
		else if ( bErrorOut || feof(pFILE) || ferror(pFILE) )
		{
			/* Zero fill memory.*/
			if ( pszBufferW )
				VZEROMEMORY(pszBufferW, nMaxChars * nCharSize);
			else
				VZEROMEMORY(pszBufferA, nMaxChars * nCharSize);

			/* Set to original file position.*/
			fsetpos(pFILE, &posOriginal);

			/* Done.*/
			return VFALSE;
		}

		/* Success.*/
		return VTRUE;
	}
};

#endif /* VTEXTSTREAMENCODING*/
