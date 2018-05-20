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
Other Author(s) of this source code file: Reiner Eiteljörge (rei@antaro.de)
*/

#ifndef VSTRINGHPP
#define VSTRINGHPP

#include "../vstandard.h"
#include "../input_output/vtextstreamencoding.hpp"
#include "../numbers/vreferencecount.hpp"
#include "../utility/vmemoryblock.hpp"
#include "vstringconverter.hpp"

/** VString encapsulates a dynamically allocated C style null-terminated
string, and provides many string related processing functions. In addition,
the class is capable of very high speed appending operations and is
generally more efficient and faster at performing routine string operations
than conventional C style code. This class can no longer be used to create
a dynamically allocated chunk of memory, use VMemoryBlock instead. Most
functions return a pointer to the internal C style string on success, NULL
on failure. On failure to append or set a string into this class, the previous
string is still valid and in a safe state.*/
class VString
{
public:
	/** Default constructor initializes the class.*/
	VString()
		{ Init(); }

	/** Construct with an existing UNICODE string. If UNICODE is not in use,
	the string will be converted to ASCII first. nMaxChars specifies the
	maximum number of characters in the string to copy from pszString. If
	0 is used, the entire string is copied.*/
	VString(	VSTRING_CONSTW	pszString,
				VUINT			nMaxChars =
									0)
	{
		Init();
		String(pszString, nMaxChars);
	}

	/** Construct with an existing ASCII string. If UNICODE is being used,
	the string will be converted to UNICODE first. nMaxChars specifies the
	maximum number of characters in the string to copy from pszString. If
	0 is used, the entire string is copied.*/
	VString(	VSTRING_CONSTA	pszString,
				VUINT			nMaxChars =
									0)
	{
		Init();
		String(pszString, nMaxChars);
	}

	/** Construct object with existing VString object (copy constructor).*/
	VString(VString const& str)
		{ Init(&str); }

	/** Construct object with existing VMemoryBlock object. See the
	StealMemoryBlock() method for more information.*/
	VString(VMemoryBlock& memBlock)
	{
		Init();
		StealMemoryBlock(memBlock);
	}

	/** Virtual Destructor verifies internal memory is
	deallocated (as needed).*/
	virtual ~VString()
		{ Empty(); }

	/** Returns non-const C-Style string pointer. See the String() method for
	information about dangers associated with using this operator.*/
	operator VSTRING () const
		{ return (VSTRING)String(); }

	/** Returns const C-Style string pointer. See the String() method for
	information about dangers associated with using this operator.*/
	operator VSTRING_CONST () const
		{ return (VSTRING_CONST)String(); }

	/** Assign a VMemoryBlock pointer to this object. See the
	StealMemoryBlock() method for more information.*/
	VSTRING operator = (VMemoryBlock& memBlock)
		{ return StealMemoryBlock(memBlock); }

	/** Assign an existing VString object (copy the string) to this object.*/
	VSTRING operator = (VString const& s)
		{ return String(s.String()); }

	/** Assign a UNICODE string pointer to this object. This will copy the string.*/
	VSTRING operator = (VSTRING_CONSTW pszString)
		{ return String(pszString); }

	/** Assign an ASCII string pointer to this object. This will copy the string.*/
	VSTRING operator = (VSTRING_CONSTA pszString)
		{ return String(pszString); }

	/** Assign a single UNICODE character to this object.*/
	VSTRING operator = (VCHARW nChar)
		{ return String(nChar); }

	/** Assign a single ASCII character to this object.*/
	VSTRING operator = (VCHARA nChar)
		{ return String(nChar); }

	/** Concatenation of a second UNICODE string.*/
	VSTRING operator += (VSTRING_CONSTW pszString)
		{ return Append(pszString); }

	/** Concatenation of a second ASCII string.*/
	VSTRING operator += (VSTRING_CONSTA pszString)
		{ return Append(pszString); }

	/** Concatenation of a single UNICODE character.*/
	VSTRING operator += (VCHARW nChar)
		{ return Append(nChar); }

	/** Concatenation of a single ASCII character.*/
	VSTRING operator += (VCHARA nChar)
		{ return Append(nChar); }

	/** UNICODE comparison (equality) without case sensitivity.*/
	VBOOL operator == (VSTRING_CONSTW pszString) const
	{
		if ( CompareNoCase(pszString) == 0 )
			return VTRUE;
			
		return VFALSE;
	}

	/** ASCII comparison (equality) without case sensitivity.*/
	VBOOL operator == (VSTRING_CONSTA pszString) const
	{
		if ( CompareNoCase(pszString) == 0 )
			return VTRUE;
			
		return VFALSE;
	}

	/** UNICODE Comparison (not equal) without case sensitivity.*/
	VBOOL operator != (VSTRING_CONSTW pszString) const
	{
		if ( CompareNoCase(pszString) != 0 )
			return VTRUE;
			
		return VFALSE;
	}

	/** ASCII Comparison (not equal) without case sensitivity.*/
	VBOOL operator != (VSTRING_CONSTA pszString) const
	{
		if ( CompareNoCase(pszString) != 0 )
			return VTRUE;
			
		return VFALSE;
	}

	/** Increase the error count and return the new error count.*/
	virtual VULONG	AddError()
		{ return m_ErrorCount.Add(); }

	/** Given an alphabetic character, alter its case. That is, make a
	lower case letter uppercase, and vice-versa. If this function modifies
	nChar, VTRUE will be returned. If nChar was not an alphabetic character,
	VFALSE will be returned.*/
	static VBOOL	AlternateChar(VCHAR& nChar)
	{
		VCHAR nAlternate = 0;

		if ( VISLOWER(nChar) )
			nAlternate = VTOUPPER(nChar);
		else if ( VISUPPER(nChar) )
			nAlternate = VTOLOWER(nChar);

		if ( nAlternate && nAlternate != nChar )
		{
			nChar = nAlternate;
			return VTRUE;
		}

		return VFALSE;
	}

	/** Append new string segment to existing string. If nMaxChars is 0,
	entire string is appended, otherwise only the first nMaxChars of the
	string is added. To support client not having to check for NULL strings
	before calling this function, it can deal with a NULL value for
	pszString. If this is the case, the function is a NOOP, and the error
	count is NOT increased. This is not considered an error. If appending
	a string of a known length, a small performance boost may be seen by
	specifying the known string length in the parameter nKnownStringLength.
	This value does NOT include the NULL terminator. It is valid to call
	this function with pszString set to itself, or within the existing
	string.*/
	VSTRING			Append(	VSTRING_CONST	pszString,
							VUINT			nMaxChars =
												0,
							VUINT			nKnownStringLength =
												0)
	{
		VBOOL bSuccess = VTRUE;

		/* Anything to do?*/
		if ( pszString )
		{
			/* Check for required duplication.*/
			VBOOL bDuplicateRequired;

			VString* pOverlap =
				DuplicateOverlap(pszString, bDuplicateRequired);

			/* Return error if we needed an overlap duplication made
			and couldn't.*/
			if ( bDuplicateRequired && !pOverlap )
				bSuccess = VFALSE;

			/* Still OK?*/
			if ( bSuccess )
			{
				/* Assign overlap to string?*/
				if ( pOverlap )
					pszString = pOverlap->String();

				/* Granularity cannot be 0! Set to default if it is.*/
				if ( !m_nGranularity )
					m_nGranularity = 64;

				/* Determine string length, if needed.*/
				if ( !nKnownStringLength )
					nKnownStringLength =
						(pOverlap)
						? pOverlap->GetLength()
						: VSTRLEN(pszString);

				/* Are we only interested in a certain number of
				characters? Save that memory.*/
				if ( nMaxChars && nMaxChars < nKnownStringLength )
					nKnownStringLength = nMaxChars;

				/* Do we have something to do?*/
				if ( nKnownStringLength )
				{
					/* Initialize buffer to current string.*/
					VSTRING pszBuffer = m_pszString;

					/* Determine required memory in bytes.*/
					VUINT nAllocLength =
						(nKnownStringLength * sizeof(VCHAR)) +
						sizeof(VCHAR);

					/* Determine new total string length.*/
					VUINT nNewStringLength =
						(m_nLength * sizeof(VCHAR)) + nAllocLength;

					/* Do we need to allocate more memory blocks?*/
					if (	!pszBuffer ||
							m_nAllocLength < nNewStringLength )
					{
						
						/* Determine number of blocks and total block size
						to allocate.*/
						VUINT nTotalBlocks =
							(nNewStringLength / m_nGranularity) + 1;

						VUINT nTotalAlloc =
							(nTotalBlocks * m_nGranularity);

						/* (Re)Alloc memory now?*/
						pszBuffer =
							(VSTRING)realloc(	m_pszString,
												nTotalAlloc);

						/* Did the re-allocation succeed? If not,
						m_pszString is still valid.*/
						if ( pszBuffer )
						{
							/* Make assignments and calculations.*/
							m_pszString =		pszBuffer;
							m_nAllocLength =	nTotalAlloc;
						}
					}

					/* Do we (still) have a buffer to write to?*/
					if ( pszBuffer )
					{
						/* Copy string now.*/
						memcpy(	m_pszString + m_nLength,
								pszString,
								nKnownStringLength * sizeof(VCHAR));

						/* Increment known length.*/
						m_nLength += nKnownStringLength;

						/* Zero terminate it.*/
						m_pszString[m_nLength] = VTEXT('\0');
					}
					else
					{
						AddError();
						bSuccess = VFALSE;
					}
				}
			}

			VDELETE_NULL(pOverlap)
		}

		return (bSuccess) ? m_pszString : NULL;
	}

	#ifdef VWCL_UNICODE
		/** Provides string conversion from ASCII to native UNICODE.*/
		VSTRING		Append(	VSTRING_CONSTA	pszString,
							VUINT			nMaxChars =
												0,
							VUINT			nKnownStringLength =
												0)
		{
			return Append(	(VSTRING_CONST)VStringConverter(pszString),
							nMaxChars,
							nKnownStringLength);
		}
	#else
		/** Provides string conversion from UNICODE to native ASCII.*/
		VSTRING		Append(	VSTRING_CONSTW	pszString,
							VUINT			nMaxChars =
												0,
							VUINT			nKnownStringLength =
												0)
		{
			return Append(	(VSTRING_CONST)VStringConverter(pszString),
							nMaxChars,
							nKnownStringLength);
		}
	#endif
		
	/** Appends a single ASCII character to the string.*/
	VSTRING			Append(VCHARA nChar)
	{
		VCHARA sz[2] = {nChar, VTEXTA('\0')};
		return Append((VSTRING_CONST)VStringConverter(sz), 1);
	}

	/** Appends a single UNICODE character to the string.*/
	VSTRING			Append(VCHARW nChar)
	{
		VCHARW sz[2] = {nChar, VTEXTW('\0')};
		return Append((VSTRING_CONST)VStringConverter(sz), 1);
	}

	/** Append a carriage return and line feed to the string.*/
	VSTRING			AppendCRLF()
		{ return Append(VTEXT("\r\n"), 0, 2); }

	/** Returns a character reference at specified index into the string.*/
	VCHAR&			CharAt(VUINT nIndex) const
	{
		VASSERT(nIndex < GetLength())
		return m_pszString[nIndex];
	}

	/** Case sensitive comparison. Returns zero if the strings are
	identical, -1 if this VString object is less than pszString, or 1
	if this VString object is greater than pszString. If either string is
	NULL or empty, 2 will be returned.*/
	VINT			Compare(VSTRING_CONST pszString) const
	{
		return	(GetLength() && pszString)
				? VSTRCMP(m_pszString, pszString)
				: 2;
	}

	#ifdef VWCL_UNICODE
		/** Provides string conversion from ASCII to native UNICODE.*/
		VINT		Compare(VSTRING_CONSTA pszString) const
			{ return Compare((VSTRING_CONST)VStringConverter(pszString)); }
	#else
		/** Provides string conversion from UNICODE to native ASCII.*/
		VINT		Compare(VSTRING_CONSTW pszString) const
			{ return Compare((VSTRING_CONST)VStringConverter(pszString)); }
	#endif

	/** Compare without case sensitibity. Returns zero if the strings are
	identical (ignoring case), -1 if this VString object is less than
	pszString (ignoring case), or 1 if this VString object is greater than
	pszString (ignoring case). If either string is NULL or empty, 2 will be
	returned.*/
	VINT			CompareNoCase(VSTRING_CONST pszString) const
	{
		return	(GetLength() && pszString)
				? VSTRCMP_NOCASE(m_pszString, pszString)
				: 2;
	}

	#ifdef VWCL_UNICODE
		/** Provides string conversion from ASCII to native UNICODE.*/
		VINT		CompareNoCase(VSTRING_CONSTA pszString) const
		{
			return CompareNoCase((VSTRING_CONST)VStringConverter(pszString));
		}
	#else
		/** Provides string conversion from UNICODE to native ASCII.*/
		VINT		CompareNoCase(VSTRING_CONSTW pszString) const
		{
			return CompareNoCase((VSTRING_CONST)VStringConverter(pszString));
		}
	#endif

	/** Determining if the string ends with a specific character. Returns
	VTRUE if it does, VFALSE otherwise.*/
	VBOOL			DoesEndWith(VCHAR nChar) const
	{
		VUINT nLength = GetLength();

		if ( nLength && m_pszString[nLength - 1] == nChar )
			return VTRUE;

		return VFALSE;
	}

	/** Free the internal buffer and reset internal variables. If bFreeBuffer
	is VTRUE, the internal buffer will be free'd, otherwise this function
	will only reset internal members so the previously allocated memory can
	be re-used. If the internal buffer is kept for future use, this class
	guards against calls that make use of it. In other words, even if the
	internal pointer m_pszString is not NULL, calls to GetLength() or the
	comparison functions for instance will behave like it is NULL.*/
	VSTRING			Empty(	VBOOL bFreeBuffer =
									VTRUE,
							VBOOL bResetErrorCount =
									VTRUE)
	{
		/* Free string if allocated and told to free the buffer.*/
		if ( m_pszString && bFreeBuffer )
		{
			free(m_pszString);
			m_pszString = NULL;
		}

		/* Reset internal variables.*/
		m_nLength = 0;

		if ( bFreeBuffer )
			m_nAllocLength = 0;

		if ( bResetErrorCount )
			m_ErrorCount.Reset();

		return m_pszString;
	}

	/** Append a character to the end of the string if the string does not
	already end with that character. This is mostly used when building a
	string that contains a file path that should end with a trailing
	VFILE_PATH_SEP_CHAR character. If the string already ends with the
	character specified by nChar, this function will be a NOOP. This
	function will only succeed on a string that already has length. It will
	not set the first character of an otherwise empty string to nChar. If
	you are only interested in determining if the string ends with a specific
	character, use DoesEndWith() instead.*/
	void			EndWith(VCHAR nChar = VFILE_PATH_SEP_CHAR)
	{
		VUINT nLength = GetLength();

		if ( nLength && m_pszString[nLength - 1] != nChar )
			Append(nChar);
	}

	/** Find first occurence of substring in string. Returns index into
	string if found, -1 otherwise. If the search should not be case
	sensitive, pass VFALSE for the bCaseSensitive parameter.*/
	VINT			FindFirst(	VSTRING_CONST	pszSubString,
								VBOOL			bCaseSensitive =
													VTRUE) const
	{
		VASSERT(VSTRLEN_CHECK(pszSubString))

		if ( GetLength() && pszSubString )
		{
			if ( bCaseSensitive )
			{
				VSTRING pszFound = VSTRSTR(m_pszString, pszSubString);

				if ( pszFound )
					return pszFound - m_pszString;
			}
			else
			{
				/* Create lowercase version of string and pszSubString.*/
				VString sThis(VREF_THIS);
				sThis.LowerCase();

				VString sSub(pszSubString);
				sSub.LowerCase();

				return sThis.FindFirst(sSub, VTRUE);
			}
		}

		return -1;
	}

	/** Find first occurence of character in string. Returns index into
	string if found, -1 otherwise. If the search should not be case
	sensitive, pass VFALSE for the bCaseSensitive parameter.*/
	VINT			FindFirst(	VCHAR	nChar,
								VBOOL	bCaseSensitive =
											VTRUE) const
	{
		if ( GetLength() )
		{
			VSTRING pszFound = VSTRCHR(m_pszString, nChar);
			
			/* Do we need to look for case insensitive version?*/
			if ( !bCaseSensitive && AlternateChar(nChar) )
			{
				VSTRING pszFound2 = VSTRCHR(m_pszString, nChar);

				/* Determine closest match.*/
				if ( (!pszFound) || (pszFound2 && pszFound2 < pszFound) )
					pszFound = pszFound2;
			}

			if ( pszFound )
				return pszFound - m_pszString;
		}

		return -1;
	}

	/** Find the first character that is not nChar. Returns index to character
	on success, -1 if the character did not exist in the string. If the search
	should not be case sensitive, pass VFALSE for the bCaseSensitive
	parameter.*/
	VINT			FindFirstNot(	VCHAR	nChar,
									VBOOL	bCaseSensitive =
												VTRUE) const
	{
		VUINT nLength = GetLength();

		if ( nLength )
		{
			VCHAR nCharOtherCase = nChar;

			if ( !bCaseSensitive || !AlternateChar(nCharOtherCase) )
				bCaseSensitive = VFALSE;

			for ( VUINT i = 0; i < nLength; i++ )
			{
				if ( m_pszString[i] != nChar )
					return i;

				/* Try case insensitive?*/
				if ( !bCaseSensitive && m_pszString[i] != nCharOtherCase )
					return i;
			}
		}

		return -1;
	}

	/** Find last occurence of substring in string. Returns index into
	string if found, -1 otherwise. If the search should not be case
	sensitive, pass VFALSE for the bCaseSensitive parameter.*/
	VINT			FindLast(	VSTRING_CONST	pszSubString,
								VBOOL			bCaseSensitive =
													VTRUE) const
	{
		VASSERT(VSTRLEN_CHECK(pszSubString))

		if ( GetLength() && pszSubString )
		{
			if ( bCaseSensitive )
			{
				VSTRING pszFound;

				for ( VUINT i = GetLength(); i > 0; i-- )
				{
					pszFound = VSTRSTR(&m_pszString[i - 1], pszSubString);

					if ( pszFound )
						return pszFound - m_pszString;
				}
			}
			else
			{
				/* Create lowercase version of string and pszSubString.*/
				VString sThis(VREF_THIS);
				sThis.LowerCase();

				VString sSub(pszSubString);
				sSub.LowerCase();

				return sThis.FindLast(sSub, VTRUE);
			}
		}

		return -1;
	}

	/** Find last occurence of character in string. Returns index into
	string if found, -1 otherwise. If the search should not be case
	sensitive, pass VFALSE for the bCaseSensitive parameter.*/
	VINT			FindLast(	VCHAR	nChar,
								VBOOL	bCaseSensitive =
											VTRUE) const
	{
		if ( GetLength() )
		{
			VASSERT(m_pszString)

			VSTRING pszFound = VSTRRCHR(m_pszString, nChar);

			/* Do we need to look for case insensitive version?*/
			if ( !bCaseSensitive && AlternateChar(nChar) )
			{
				VSTRING pszFound2 = VSTRRCHR(m_pszString, nChar);

				/* Determine closest match.*/
				if ( pszFound2 > pszFound )
					pszFound = pszFound2;
			}

			if ( pszFound )
				return pszFound - m_pszString;
		}

		return -1;
	}

	/** Find the last character that is not nChar. Returns index to character
	on success, -1 if the character did not exist in the string. If the search
	should not be case sensitive, pass VFALSE for the bCaseSensitive
	parameter.*/
	VINT			FindLastNot(VCHAR	nChar,
								VBOOL	bCaseSensitive =
											VTRUE) const
	{
		VUINT nLength = GetLength();

		if ( nLength )
		{
			VCHAR nCharOtherCase = nChar;

			if ( !bCaseSensitive || !AlternateChar(nCharOtherCase) )
				bCaseSensitive = VFALSE;

			for ( VUINT i = nLength; i > 0; i-- )
			{
				if ( m_pszString[i - 1] != nChar )
					return i - 1;

				/* Try case insensitive?*/
				if ( !bCaseSensitive && m_pszString[i - 1] != nCharOtherCase )
					return i - 1;
			}
		}

		return -1;
	}

	/** Return the number of bytes, not characters, allocated
	to this object.*/
	VUINT			GetByteAlloc() const
		{ return m_nAllocLength; }

	/** Returns the number of times a particular character is found in
	the string. If the search should not be case sensitive, pass VFALSE
	for the bCaseSensitive parameter.*/
	VUINT			GetCharCount(	VCHAR	nChar,
									VBOOL	bCaseSensitive =
												VTRUE) const
	{
		VUINT nCount =		0;
		VUINT nLength =		GetLength();
		VCHAR nAlternate =	nChar;
		VCHAR nThisChar =	0;
		
		/* Save case insensitive version.*/
		if ( !AlternateChar(nAlternate) )
			bCaseSensitive = VFALSE;

		for ( VUINT i = 0; i < nLength; i++ )
		{
			VASSERT(m_pszString)

			nThisChar = CharAt(i);

			if ( nThisChar == nChar )
				nCount++;
			else if ( !bCaseSensitive && nThisChar == nAlternate )
				nCount++;
		}

		return nCount;
	}

	/** Return the internal error count. This value is incremented anytime
	a memory allocation fails, or externally by calls to AddError().*/
	VULONG			GetErrorCount() const
		{ return m_ErrorCount.GetCount(); }

	/** Return a reference to the granularity size. This is how much memory,
	during appends, is allocated each time an append or allocation operation
	would require more memory than what is currently allocated. Any value,
	other than 0, is valid as the granularity setting. The larger the value,
	the more memory will be allocated each time a new block is required, but
	fewer allocations are needed. A smaller number decreases total memory
	usage, at the expense of more frequent reallocations. The default value
	is 64 bytes, and this will be used if set to 0.*/
	VUINT&			GetGranularity() const
		{ return (VUINT&)m_nGranularity; }

	/** Get the length of the string.*/
	VUINT			GetLength() const
	{
		if ( m_nLength )
		{
			/* If m_nLength is set, this better be valid!*/
			VASSERT(m_pszString)
			return m_nLength;
		}

		return 0;
	}

	/** Is the string empty?*/
	VBOOL			IsEmpty() const
		{ return (GetLength()) ? VFALSE : VTRUE; }

	/** Is the string not empty?*/
	VBOOL			IsNotEmpty() const
		{ return (GetLength()) ? VTRUE : VFALSE; }

	/** Determine if this string is a match for the wildcard specified by
	pszWildCard. At this time, the only valid wildcard character is *. For
	example, if this string contains the string "Test" and pszWildCard is
	"Te*" then this function will return VTRUE, otherwise it will return
	VFALSE. If the match should be case sensitive, pass VTRUE for the
	bCaseSensitive parameter.*/
	VBOOL			IsWildCardMatch(	VSTRING_CONST	pszWildCard,
										VBOOL			bCaseSensitive =
															VFALSE) const
	{
		VSTRING_CONST pszAster =
			(pszWildCard) ? VSTRCHR(pszWildCard, VTEXT('*')) : NULL;

		if ( pszAster && GetLength() )
		{
			/* Is the asterisk the first character? If so, this is match.*/
			if ( pszAster == pszWildCard )
				return VTRUE;

			/* We have more work to do.*/

			/* Create string with part of pszWildCard up to, but not
			including, the asterisk.*/
			VString sMatchPart(pszWildCard, pszAster - pszWildCard);

			if (	sMatchPart.IsNotEmpty() &&
					FindFirst(sMatchPart, bCaseSensitive) == 0 )
				return VTRUE;
		}

		return VFALSE;
	}

	/** Substring extraction. Returns a new VString object with the first
	nCount of this strings characters in it. If this string is less than
	nCount in length, the entire string will be returned.*/
	VString			Left(VUINT nCount) const
	{
		VString str;

		if ( nCount && GetLength() )
			str.String(m_pszString, 0, nCount);

		return str;
	}

	/** Make all characters lower case.*/
	VSTRING			LowerCase()
	{
		if ( GetLength() )
		{
			VASSERT(m_pszString)
			return VSTRLWR(m_pszString);
		}

		return NULL;
	}

	/** Make the string a GetOpenFileName() or GetSaveFileName() filter.
	That is, replace all occurances of \n in the string with \0 (zero
	terminators).*/
	VSTRING			MakeFilter()
		{ return ReplaceCharacters(VTEXT('\n'), VTEXT('\0')); }

	/** Substring extraction. Returns a new VString object with part of the
	existing string extracted. This function will start at the nFirst
	character in the string, and copy nCount number of characters to the
	new string. If nCount is 0, the balance of the string after nFirst will
	be copied. The index at nFirst must be withing the string length of this
	string to be valid, otherwise an empty string will be returned.*/
	VString			Mid(	VUINT nFirst,
							VUINT nCount =
									0) const
	{
		VString str;

		if ( nFirst < GetLength() )
			str.String(m_pszString + nFirst, 0, nCount);

		return str;
	}

	/** When this string contains a HTTP parsed URL, convert + signs into
	spaces and %xx escape sequences into single character.*/
	VSTRING			ParseEncodedURL()
	{
		if ( GetLength() )
		{
			/* This method shortens the string, so update the length.*/
			VTextStreamEncoding::ParseEncodedURL(m_pszString);
			m_nLength = VSTRLEN_CHECK(m_pszString);

			return m_pszString;
		}

		return NULL;
	}

	/** Remove a section from the center of this string. nFirst is the index
	into this string of the first character to be removed. nRemoveCount is
	the number of characters to remove. For example, if this class holds
	the string "Testing" and you pass 1 as the first character to remove
	and 5 as the remove count, the resulting string would be "tg". Starting
	with the "e" in "Testing" which is at index 1 in the string, 5
	characters, including the "e" were removed. Returns internal string
	buffer on success, NULL on failure. On failure, the internal string is
	not modified. If nRemoveCount would extend beyond the length of the
	string, all characters after nFirst (inclusive) will be removed. If
	nFirst is 0 and nRemoveCount is the string length (or greater), the
	string will be emptied and NULL returned. This function never results
	in a memory allocation, since it shortens the string.*/
	VSTRING			RemoveSection(	VUINT nFirst,
									VUINT nRemoveCount =
											1)
	{
		VUINT nLength = GetLength();

		if ( nLength > nFirst )
		{
			/* Anything to do?*/
			if ( nRemoveCount )
			{
				/* Validate remove count.*/
				if ( nRemoveCount > nLength - nFirst )
					nRemoveCount = nLength - nFirst;

				/* Going to empty string?*/
				if ( nRemoveCount == nLength )
					Empty();
				else
				{
					/* Copy remainder of string to first offset.*/
					VSTRCPY(&m_pszString[nFirst],
							&m_pszString[nFirst + nRemoveCount]);

					/* Update length.*/
					m_nLength -= nRemoveCount;
				}
			}

			return m_pszString;
		}

		return NULL;
	}

	/** Set a pointer to an externally allocated C style string (allocated
	with malloc()) into object. This can be done to replace a string pointer
	with a previously "stolen" pointer from the StealBuffer() function. This
	class will then assume ownership of the string, deleting it as needed.
	The length of the allocated memory MUST be given to this class for the
	function to succeed. The C string essentially becomes a VString object,
	so the allocated memory size must be given, and it must be at least 1
	character longer than the string to be valid.*/
	VSTRING			ReplaceBuffer(	VSTRING	pszString,
									VUINT	nAllocatedMemorySize,
									VUINT	nKnownStringLength =
												0)
	{
		/* Free current string.*/
		Empty();

		/* Save pointer?*/
		if ( pszString )
		{
			#ifdef VWCL_DEBUG
				VUINT nRequired = (VUINT)VSTRLEN(pszString) + sizeof(VCHAR);
				VASSERT(nAllocatedMemorySize >= nRequired)
			#endif

			/* Set member variables.*/
			m_pszString =		pszString;
			m_nAllocLength =	nAllocatedMemorySize;

			m_nLength =
				(nKnownStringLength)
				? nKnownStringLength
				: VSTRLEN(pszString);
		}

		return m_pszString;
	}

	/** Steal the buffer from an existing VString object and place into
	this object. If strFrom is empty, this string will also be emptied.*/
	VSTRING			ReplaceBuffer(VString& strFrom)
	{
		if ( strFrom.IsEmpty() )
			Empty();
		else
		{
			VUINT nAllocatedMemorySize =	strFrom.m_nAllocLength;
			VUINT nKnownStringLength =		strFrom.GetLength();
			ReplaceBuffer(	strFrom.StealBuffer(),
							nAllocatedMemorySize,
							nKnownStringLength);
		}

		return m_pszString;
	}

	/** Replace all occurances of nFind in the string with nReplacement.*/
	VSTRING			ReplaceCharacters(	VCHAR nFind,
										VCHAR nReplacement)
	{
		VUINT nLength = GetLength();

		if ( nLength )
		{
			for ( VUINT i = 0; i < nLength; i++ )
			{
				if ( m_pszString[i] == nFind )
					m_pszString[i] = nReplacement;
			}
		}

		return m_pszString;
	}

	/** Replace the first occurance of nChar with nReplacement in the
	string. The default for nReplacement terminates the string. If the
	search (on the nChar parameter) should not be case sensitive, you
	can pass VFALSE for the bCaseSensitive parameter. Returns the index
	of the character that was replaced, or -1 if the character was not
	found in the string. If nReplacement is a 0 terminator, the string
	length will be updated to reflect this change.*/
	VINT			ReplaceFirstChar(	VCHAR	nChar,
										VCHAR	nReplacement =
													VTEXT('\0'),
										VBOOL	bCaseSensitive =
													VTRUE)
	{
		VINT nReplace = FindFirst(nChar, bCaseSensitive);

		if ( nReplace != -1 )
		{
			CharAt(nReplace) = nReplacement;

			/* If the string was terminated, update length.*/
			if ( nReplacement == VTEXT('\0') )
				m_nLength = nReplace;
		}
			
		return nReplace;
	}

	/** Replace the last occurance of nChar with nReplacement in the string.
	The default for nReplacement terminates the string.  If the search
	(on the nChar parameter) should not be case sensitive, you can pass
	VFALSE for the bCaseSensitive parameter. Returns the index of the
	character that was replaced, or -1 if the character was not found in
	the string. If nReplacement is a 0 terminator, the string length will be
	updated to reflect this change.*/
	VINT			ReplaceLastChar(	VCHAR	nChar,
										VCHAR	nReplacement =
													VTEXT('\0'),
										VBOOL	bCaseSensitive =
													VTRUE)

	{
		VINT nReplace = FindLast(nChar, bCaseSensitive);

		if ( nReplace != -1 )
		{
			CharAt(nReplace) = nReplacement;
			
			/* If the string was terminated, update length.*/
			if ( nReplacement == VTEXT('\0') )
				m_nLength = nReplace;
		}

		return nReplace;
	}

	/** Substring extraction. Returns a new VString object with the nCount
	rightmost characters of this string. If nCount is longer that the length
	of this string, the entire string will be returned.*/
	VString			Right(VUINT nCount) const
	{
		VString str;

		VUINT nLength = GetLength();

		if ( nCount < nLength )
			return Mid(nLength - nCount);
		else
			str = VREF_THIS;

		return str;
	}

	/** Steal the C-Style string buffer. Calling code takes ownership of
	string pointer and must free it when done using the CRT function free().
	On exit, this class is a newly initialized state.*/
	VSTRING			StealBuffer()
	{
		/* Save return buffer after emptying the current object.*/
		VSTRING pszString = Empty(VFALSE);

		/* Reset internal members.*/
		Init(this);

		return pszString;
	}

	/** Move the contents of a VMemoryBlock object to this string. This will
	cause the memory block object to have no allocated memory, as this string
	will now own that memory, which MUST be a zero terminated string, and must
	be either ASCII or UNICODE depending on the target being built.*/
	VSTRING			StealMemoryBlock(VMemoryBlock& memBlock)
	{
		/* Free current string.*/
		Empty();

		/* Set member variables.*/
		m_nAllocLength =	memBlock.GetLength();
		m_pszString =		(VSTRING)memBlock.Steal();
		m_nLength =			VSTRLEN_CHECK(m_pszString);
		
		return m_pszString;
	}

	/** Get the internal string buffer. It is important to note that a valid
	buffer pointer might be returned from this function, even though the
	VString class knows the specified length is 0. The reason is that
	internal buffers can be reused to avoid unneeded memory allocations.
	For instance, if you call the Empty() method and tell it not to release
	the internal buffer, the internal m_nLength member is set to 0 but the
	internal string pointer m_pszString is not touched. Therefore, calling
	this function would return the internal pointer.*/
	VSTRING			String() const
		{ return m_pszString; }

	/** Save pszString in class and returns pointer to the buffer if a string
	is held. It should be noted that for string that are not likely to change
	size, this function differs from Append() by not allocating more memory
	than is required. This does not affect future appends which can still be
	done, it merely means that for strings not likely to change, it uses a
	more efficient storage scheme. If pszString is not NULL and nMaxChars is
	given, only the number of characters specified will be copied to the
	string. A very slight performance boost can be realized if the length of
	pszString is known when this function is called, and can be specified as
	the nKnownStringLength parameter. If this function fails to allocate
	memory, it will return NULL, however the previous string held by this
	class will be left intact and safe.*/
	VSTRING			String(	VSTRING_CONST	pszString,
							VUINT			nMaxChars =
												0,
							VUINT			nKnownStringLength =
												0)
	{
		VBOOL bSuccess = VTRUE;

		/* Same as empty?*/
		if ( !pszString )
			return Empty();

		/* Check for required duplication.*/
		VBOOL bDuplicateRequired;

		VString* pOverlap =
			DuplicateOverlap(pszString, bDuplicateRequired);

		/* Return error if we needed an overlap duplication made
		and couldn't.*/
		if ( bDuplicateRequired && !pOverlap )
			bSuccess = VFALSE;

		/* Still OK?*/
		if ( bSuccess )
		{
			/* Assign overlap to string?*/
			if ( pOverlap )
				pszString = pOverlap->String();

			/* Determine memory allocation length. pszString
			can be NULL.*/
			if ( !nKnownStringLength )
			{
				nKnownStringLength =	(pOverlap)
										? pOverlap->GetLength()
										: VSTRLEN(pszString);
			}

			/* Are we only interested in a certain number
			of characters? Save that memory.*/
			if ( nMaxChars && nMaxChars < nKnownStringLength )
				nKnownStringLength = nMaxChars;

			/* Anything to do?*/
			if ( nKnownStringLength )
			{
				VMemoryBlock memBlock;

				if ( memBlock.AllocForString(nKnownStringLength) )
				{
					/* Copy string to memory block.*/
					memcpy(	memBlock.Get(),
							pszString,
							nKnownStringLength * sizeof(VCHAR));
					
					/* Steal from memory block to this string object.*/
					StealMemoryBlock(memBlock);
				}
				else
				{
					AddError();
					bSuccess = VFALSE;
				}
			}
			else
				Empty();
		}

		VDELETE_NULL(pOverlap)

		return (bSuccess ) ? m_pszString : NULL;
	}

	#ifdef VWCL_UNICODE
		/** Provides string conversion from ASCII to native UNICODE.*/
		VSTRING		String(	VSTRING_CONSTA	pszString,
							VUINT			nMaxChars =
												0,
							VUINT			nKnownStringLength =
												0)
		{
			return String(	(VSTRING_CONST)VStringConverter(pszString),
							nMaxChars,
							nKnownStringLength);
		}
	#else
		/** Provides string conversion from UNICODE to native ASCII.*/
		VSTRING		String(	VSTRING_CONSTW	pszString,
							VUINT			nMaxChars =
												0,
							VUINT			nKnownStringLength =
												0)
		{
			return String(	(VSTRING_CONST)VStringConverter(pszString),
							nMaxChars,
							nKnownStringLength);
		}
	#endif

	/** Assign a single UNICODE character to this string.*/
	VSTRING			String(VCHARW nChar)
	{
		VCHARW sz[2] = {nChar, VTEXTW('\0')};
		return String(sz, 0, 1);
	}

	/** Assign a single ASCII character to this string.*/
	VSTRING			String(VCHARA nChar)
	{
		VCHARA sz[2] = {nChar, VTEXTA('\0')};
		return String(sz, 0, 1);
	}

	/** Remove all white-space characters from left and right
	side of string. This is the same as calling TrimLeft() and TrimRight().*/
	VSTRING			Trim()
	{
		TrimLeft();
		return TrimRight();
	}

	/** Remove all white-space characters from left side of string. This
	method will never result in a new memory allocation.*/
	VSTRING			TrimLeft()
	{
		VUINT nLength = GetLength();

		if ( nLength )
		{
			/* Flag to indicate when a non-space char was found.*/
			VBOOL bAllSpaces = VTRUE;

			for ( VUINT i = 0; i < nLength; i++ )
			{
				if ( !VISSPACE(m_pszString[i]) )
				{
					bAllSpaces = VFALSE;

					/* Update length.*/
					m_nLength -= i;

					/* Include the 0 terminator in the move.*/
					memmove(m_pszString,
							m_pszString + i,
							(m_nLength * sizeof(VCHAR)) + sizeof(VCHAR));

					/* Time to exit.*/
					break;
				}
			}

			/* All spaces? Empty string.*/
			if ( bAllSpaces )
				Empty();
		}

		return m_pszString;
	}

	/** Remove all white-space characters from right side of string. This
	method will never result in a new memory allocation.*/
	VSTRING			TrimRight()
	{
		VUINT nLength = GetLength();

		if ( nLength )
		{
			/* Flag to indicate when a non-space char was found.*/
			VBOOL bAllSpaces = VTRUE;

			for ( VUINT i = nLength; i > 0; i-- )
			{
				if ( !VISSPACE(m_pszString[i - 1]) )
				{
					bAllSpaces = VFALSE;

					/* Terminate here.*/
					m_pszString[i] = VTEXT('\0');

					/* Update length.*/
					m_nLength = i;

					/* Time to exit.*/
					break;
				}
			}

			/* All spaces? Empty string.*/
			if ( bAllSpaces )
				Empty();
		}

		return m_pszString;
	}

	/** Make all characters upper case.*/
	VSTRING			UpperCase()
	{
		if ( GetLength() )
		{
			VASSERT(m_pszString)
			return VSTRUPR(m_pszString);
		}

		return NULL;
	}

protected:
	/** Copy internal string if pszString is within us.*/
	VString*		DuplicateOverlap(	VSTRING_CONST	pszString,
										VBOOL&			bDuplicateRequired)
	{
		bDuplicateRequired = VFALSE;

		/* Deal with appending from an overlapping string. That is, a
		string that is part of us already.*/
		if (	pszString &&
				m_pszString &&
				pszString >= m_pszString &&
				pszString <= m_pszString + m_nLength )
		{
			bDuplicateRequired = VTRUE;

			VString* pDuplicate = VNEW VString(pszString);

			if ( pDuplicate && pDuplicate->IsNotEmpty() )
				return pDuplicate;

			VDELETE_NULL(pDuplicate)

			/* Incremement error count.*/
			AddError();
		}

		return NULL;
	}

	/** Initialize member variables to default values, or copy from
	another object.*/
	void			Init(VString const* pExisting = NULL)
	{
		/* Initiailize members.*/
		m_pszString = NULL;
		m_nAllocLength = m_nLength = 0;

		/* Copy the granularity as needed.*/
		m_nGranularity =
			(pExisting) ? pExisting->m_nGranularity : 64;

		/* Copy string too?*/
		if ( pExisting && pExisting->IsNotEmpty() )
			String(pExisting->String());
	}

	/** Embedded Members.*/
	VReferenceCount	m_ErrorCount;
	VSTRING			m_pszString;
	VUINT			m_nLength;
	VUINT			m_nAllocLength;
	VUINT			m_nGranularity;
};

#endif	/* VSTRINGHPP*/
