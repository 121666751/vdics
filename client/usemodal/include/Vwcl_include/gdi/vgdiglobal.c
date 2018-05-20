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

#define HIMETRIC_PER_INCH	2540	/* Number of HIMETRIC units per inch.*/
#define MAP_PIX_TO_LOGHIM(x,ppli)   MulDiv(HIMETRIC_PER_INCH, (x), (ppli))
#define MAP_LOGHIM_TO_PIX(x,ppli)   MulDiv((ppli), (x), HIMETRIC_PER_INCH)

HGLOBAL VLoadResource(	VSTRING_CONST	pszType,
						VSTRING_CONST	pszName,
						VUINT*			pnSize,
						HINSTANCE		hResource)
{
	HGLOBAL hResult = NULL;

	/* These must be known!*/
	VASSERT(pszType)
	VASSERT(pszName)

	/* Get resource handle if needed.*/
	if ( !hResource )
		hResource = VGetResourceHandle();
	VASSERT(hResource)

	/* Initialize out parameter.*/
	if ( pnSize )
		*pnSize = 0;

	if ( pszType && pszName && hResource )
	{
		/* Find the resource first.*/
		HRSRC hFound = FindResource(hResource, pszName, pszType);
		
		if ( hFound )
		{
			/* Get the size of this resource.*/
			VUINT nSize = SizeofResource(hResource, hFound);
			
			if ( nSize )
			{
				/* Load it now.*/
				hResult = LoadResource(hResource, hFound);
				
				if ( hResult && pnSize )
					*pnSize = nSize;
			}
		}
	}

	return hResult;
}

VVOIDPTR VLoadAndLockResource(	VSTRING_CONST	pszType,
								VSTRING_CONST	pszName,
								VUINT*			pnSize,
								HINSTANCE		hResource)
{
	HGLOBAL hResData = VLoadResource(pszType, pszName, pnSize, hResource);

	if ( hResData )
		return LockResource(hResData);

	return NULL;
}

void VMapCoords(HDC		hDC,
				SIZEL*	psizeL,
				VBOOL	bToPixels)
{
	/* Get the DC as needed.*/
	HDC hRealDC = (hDC) ? hDC :	GetDC(NULL);

	VASSERT(hRealDC)
	VASSERT(psizeL)
	
	if ( hRealDC && psizeL )
	{
		VINT nLogX = GetDeviceCaps(hRealDC, LOGPIXELSX);
		VINT nLogY = GetDeviceCaps(hRealDC, LOGPIXELSY);

		if ( bToPixels )
		{
			psizeL->cx = MAP_LOGHIM_TO_PIX(psizeL->cx, nLogX);
			psizeL->cy = MAP_LOGHIM_TO_PIX(psizeL->cy, nLogY);
		}
		else
		{
			psizeL->cx = MAP_PIX_TO_LOGHIM(psizeL->cx, nLogX);
			psizeL->cy = MAP_PIX_TO_LOGHIM(psizeL->cy, nLogY);
		}

		/* Did we get the DC? If yes, release it.*/
		if ( !hDC )
			ReleaseDC(NULL, hRealDC);
	}
}
