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

#include "vstandard.h"

/** This file provides portability of non-standard (but common) functions
and variables used in Windows and UNIX development.*/

/* Globals for use when UNIX is the target OS.*/
#ifdef VWCL_TARGET_UNIX
	VUINT		gn_vwcl_argc;
	VSTRING*	gpp_vwcl_argv;
#endif

/** Internal function to get the mode (type) of a file.*/
VUINT vwcl_port_GetFileMode(VSTRING_CONST pszName)
{
	if ( pszName )
	{
		VSTATSTRUCTURE fileSTAT;
		VZEROSTRUCT(fileSTAT);

		if ( VSTAT(pszName, &fileSTAT) == 0 )
			return fileSTAT.st_mode;
	}

	return 0;
}

/** Determine if a file exists.*/
VBOOL VDoesFileExist(VSTRING_CONST pszFileName)
{
	VUINT nMode = vwcl_port_GetFileMode(pszFileName);

	if ( nMode )
		return (!(nMode & S_IFDIR) && (nMode & S_IFREG)) ? VTRUE : VFALSE;

	return VFALSE;
}

/** Determine if a directory exists.*/
VBOOL VDoesDirectoryExist(VSTRING_CONST pszDirectoryName)
{
	VUINT nMode = vwcl_port_GetFileMode(pszDirectoryName);

	if ( nMode )
		return (nMode & S_IFDIR) ? VTRUE : VFALSE;

	return VFALSE;
}

/** Return the argc value normally passed to main(). This will work for
applications that use WinMain() as well.*/
VUINT VGetARGC()
{
	#ifdef VWCL_TARGET_UNIX
		return gn_vwcl_argc;
	#else
		return (VUINT)__argc;
	#endif
}

/** Return one of the argv values normally passed to main(). This will
work for applications that use WinMain() as well.*/
VSTRINGA VGetARGV(VUINT nIndex)
{
	/* Make sure caller is asking for a valid index.*/
	if ( nIndex < VGetARGC() )
	{
		#ifdef VWCL_TARGET_UNIX
			return gpp_vwcl_argv[nIndex];
		#else
			return __argv[nIndex];
		#endif
	}

	return NULL;
}

/** Set the command line parameters from main(). Windows applications
do not need to do this.*/
void VSetARGC_ARGV(VUINT nARGC, VSTRINGA* ppszARGV)
{
	#ifdef VWCL_TARGET_UNIX
		gn_vwcl_argc =	nARGC;
		gpp_vwcl_argv =	ppszARGV;
	#endif
}

/** This routine is used by all other xTOA functions and is the only one
that actually does the conversions.*/
VSTRING VConvertNumberToString(	VULONG	nValue,
								VSTRING	pszBuffer,
								VUINT	nRadix,
								VBOOL	bNegative)
{
	VSTRING pszOffset =		pszBuffer;
	VSTRING pszFirstDigit =	pszOffset;

	/* This must be known!*/
	VASSERT(pszBuffer)

	/* Radix (base) must be between 2 and 36!*/
	VASSERT(nRadix >= 2)
	VASSERT(nRadix <= 36)

	if ( !pszBuffer )
		return NULL;

	/* Are we really dealing with a negative value? Handle this case.*/
	if ( bNegative )
	{
		pszOffset[0] = VTEXT('-');
		pszOffset++;
		pszFirstDigit++;
		nValue = (VULONG)(-(VLONG)nValue);
	}

	do
	{
		VUINT nDigit = (VUINT)(nValue % nRadix);
		nValue /= nRadix;

		if ( nDigit > 9 )
			pszOffset[0] = (nDigit - 10 + VTEXT('a'));
		else
			pszOffset[0] = (nDigit + VTEXT('0'));

		pszOffset++;
	}
	while (nValue > 0);

	/* We now have the digit of the number in the buffer, but in reverse
	order. Reverse them.*/
	pszOffset[0] = VTEXT('\0');
	pszOffset--;

	do
	{
		VCHAR chTemp =		pszOffset[0];
		pszOffset[0] =		pszFirstDigit[0];
		pszFirstDigit[0] =	chTemp;
		pszOffset--;
		pszFirstDigit++;
	}
	while (pszFirstDigit < pszOffset);

	return pszBuffer;
}

/** Convert VINT to string.*/
VSTRING VITOA(VINT nValue, VSTRING pszBuffer, VUINT nRadix)
	{ return VLTOA((VLONG)nValue, pszBuffer, nRadix); }

/** Convert VUINT to string.*/
VSTRING VUITOA(VUINT nValue, VSTRING pszBuffer, VUINT nRadix)
	{ return VULTOA((VULONG)nValue, pszBuffer, nRadix); }

/** Convert VLONG to string.*/
VSTRING VLTOA(VLONG nValue, VSTRING pszBuffer, VUINT nRadix)
{
	return VConvertNumberToString(	(VULONG)nValue,
									pszBuffer,
									nRadix,
									(VBOOL)((nRadix == 10 && nValue < 0)
									? VTRUE : VFALSE));
}

/** Convert VULONG to string.*/
VSTRING VULTOA(VULONG nValue, VSTRING pszBuffer, VUINT nRadix)
	{ return VConvertNumberToString(nValue, pszBuffer, nRadix, VFALSE); }

/** Convert pszConvert to lowercase, returning a pointer to it.*/
VSTRING VSTRLWR(VSTRING pszConvert)
{
	if ( pszConvert )
	{
		VSTRING pszOffset = pszConvert;

		while ( pszOffset[0] != VTEXT('\0') )
		{
			if ( VISUPPER(pszOffset[0]) )
				pszOffset[0] = VTOLOWER(pszOffset[0]);

			pszOffset++;
		}
	}

	return pszConvert;
}

/** Convert pszConvert to uppercase, returning a pointer to it.*/
VSTRING VSTRUPR(VSTRING pszConvert)
{
	if ( pszConvert )
	{
		VSTRING pszOffset = pszConvert;

		while ( pszOffset[0] != VTEXT('\0') )
		{
			if ( VISLOWER(pszOffset[0]) )
				pszOffset[0] = VTOUPPER(pszOffset[0]);

			pszOffset++;
		}
	}

	return pszConvert;
}

VBOOL VIsLittleEndian()
{
	/* This is an optimization to only check once.*/
	static VBOOL bLittleEndian =	VFALSE;
	static VBOOL bBigEndian =		VFALSE;
	
	if ( !bLittleEndian && !bBigEndian )
	{
		VULONG n = 1L;

		if ( *((VCHARA*)&n) == 0x01 )
			bLittleEndian = VTRUE;
		else
			bBigEndian = VTRUE;
	}

	return bLittleEndian;
}

VBOOL VIsBigEndian()
	{ return (VIsLittleEndian()) ? VFALSE : VTRUE; }

