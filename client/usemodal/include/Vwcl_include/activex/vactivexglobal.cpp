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

#include "../vstandard.h"
#include "../numbers/vreferencecount.hpp"
#include "../utility/vhandle.hpp"
#include "vunknown.hpp"

/* Initialize static members.*/
VReferenceCount VUnknown::m_TotalObjectReferenceCount;

VBOOL VCoStringFromString(	VSTRING*		ppszCoString,
							VSTRING_CONST	pszString)
{
	VASSERT(ppszCoString)
	VASSERT(pszString)

	/* Initialize out parameter.*/
	*ppszCoString = NULL;

	if ( pszString )
	{
		/* Allocate memory required for pszString using CoTaskMemAlloc().*/
		*ppszCoString =
			(VSTRING)CoTaskMemAlloc(VSTRLEN(pszString) + sizeof(VCHAR));

		if ( *ppszCoString )
		{
			VSTRCPY(*ppszCoString, pszString);
			return VTRUE;
		}
	}

	return VFALSE;
}

VBOOL VIsValidStructuredStorageName(VSTRING_CONST pszName)
{
	VASSERT(VSTRLEN_CHECK(pszName))

	/* Get and check string length.*/
	VINT nLen = VSTRLEN_CHECK(pszName);

	if ( !nLen || nLen > 31 )
		return VFALSE;

	/* Check for invalid characters.*/
	for ( VINT i = 0; i < nLen; i++ )
	{
		/* Look for bad char.*/
		if ( VSTRCHR(VTEXT(".\\/:!"), pszName[i]) )
			return VFALSE;
	}

	/* Success.*/
	return VTRUE;
}

/* Internal function to stream IPicture from global memory.*/
IPicture* VIPictureFromGlobal(	HGLOBAL	hGlobal,
								VDWORD	nSize)
{
	VASSERT(hGlobal)
	VASSERT(nSize)

	IPicture* pPicture = NULL;

	if ( hGlobal )
	{
		if ( nSize )
		{
			/* Create IStream object from global memory.*/
			IStream* pStream = NULL;
			
			if ( SUCCEEDED(CreateStreamOnHGlobal(
				hGlobal,
				VTRUE,
				&pStream)) )
			{
				/* Create IPicture from stream.*/
				OleLoadPicture(	pStream,
								nSize,
								VFALSE,
								IID_IPicture,
								(VVOIDPTR*)&pPicture);
				
				/* Release the stream now.*/
				pStream->Release();
			}
		}
	}

	return pPicture;
}

IPicture* VLoadIPictureFromFile(VSTRING_CONST pszFileName)
{
	/* This must be known.*/
	VASSERT(VSTRLEN_CHECK(pszFileName))
	
	/* Initialize result.*/
	IPicture* pPicture = NULL;

	if ( pszFileName )
	{
		/* Open the file.*/
		VHandle hFile(CreateFile(	pszFileName,
									GENERIC_READ,
									FILE_SHARE_READ,
									NULL,
									OPEN_EXISTING,
									0,
									NULL));

		if ( hFile.GetHandle() )
		{
			/* Get the size.*/
			VDWORD nSize = GetFileSize(hFile.GetHandle(), NULL);
			
			if ( nSize && nSize != 0xffffffff )
			{
				/* Allocate memory based on file size.*/
				HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
				
				if ( hGlobal )
				{
					/* Lock memory now.*/
					VVOIDPTR pvData = GlobalLock(hGlobal);

					if ( pvData )
					{
						VDWORD nBytesRead = 0;
						
						/* Read file and store in global memory.*/
						if ( ReadFile(	hFile.GetHandle(),
										pvData,
										nSize,
										&nBytesRead,
										NULL) )
						{
							pPicture = VIPictureFromGlobal(hGlobal, nSize);
						}
						
						GlobalUnlock(hGlobal);
					}

					GlobalFree(hGlobal);
				}
			}
		}
	}

	return pPicture;
}

IPicture* VLoadIPictureFromResources(	VSTRING_CONST	pszType,
										VSTRING_CONST	pszName,
										HINSTANCE		hResource)
{
	/* Initialize result.*/
	IPicture* pPicture = NULL;

	/* Try to load the resource.*/
	VUINT nSize = 0;
	
	VVOIDPTR pvResData =
		VLoadAndLockResource(pszType, pszName, &nSize, hResource);

	if ( pvResData )
	{
		/* We have to copy this to workable memory.*/

		/* Allocate memory based on file size.*/
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
		
		if ( hGlobal )
		{
			/* Lock memory now.*/
			VVOIDPTR pvCopy = GlobalLock(hGlobal);

			if ( pvCopy )
			{
				/* Copy memory now.*/
				memcpy(pvCopy, pvResData, nSize);

				/* Get picture.*/
				pPicture = VIPictureFromGlobal(hGlobal, nSize);

				GlobalUnlock(hGlobal);
			}

			GlobalFree(hGlobal);
		}
	}

	return pPicture;
}