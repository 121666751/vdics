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

#ifndef VBITMAP
#define VBITMAP

#include "../vstandard.h"
#include "vdc.hpp"
#include "vgdiobject.hpp"
#include "vgdiobjectselector.hpp"

/** VBitmap encapsulates the processing of Windows HBITMAP objects.*/
class VBitmap : public VGDIObject
{
public:
	/** Default constructor can initialize the class to defaults, or load
	a bitmap from resources.*/
	VBitmap(	VSTRING_CONST	pszBitmap =
									NULL,
				HINSTANCE		hResource =
									NULL)
	{
		VZEROSTRUCT(m_Bitmap);

		if ( pszBitmap )
			Load(pszBitmap, hResource);
	}

	/** Standard constructor initializes the class with an existing handle
	to a bitmap.*/
	VBitmap(HBITMAP hBitmap)
	{
		VZEROSTRUCT(m_Bitmap);
		Set(hBitmap);
	}

	/** Draw the bitmap to a device context. dc is the device context to
	draw the bitmap into. nXPosition is the left position in the device
	context to draw the upper left corner of the bitmap. nYPosition is the
	top position in the device context to draw the upper top corner of the
	bitmap. nXBitmapPosition is the left position within the bitmap to
	start drawing. nYBitmapPosition is the top position within the bitmap
	to start drawing. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL		Draw(	VDC&	dc,
						VINT	nXPosition =
									0,
						VINT	nYPosition =
									0,
						VINT	nXBitmapPosition =
									0,
						VINT	nYBitmapPosition =
									0)
	{
		if ( m_hGDIObject && dc.GetHandle() )
		{
			VDC memDC;

			if ( dc.CreateCompatible(memDC) )
			{
				VGDIObjectSelector os(memDC, VREF_THIS);

				dc.BitBlt(	nXPosition,
							nYPosition,
							GetWidth() - nXBitmapPosition,
							GetHeight() - nYBitmapPosition,
							memDC,
							nXBitmapPosition,
							nYBitmapPosition);

				return VTRUE;
			}
		}

		return VFALSE;
	}

	/** Return a reference to the BITMAP struct pointer.*/
	BITMAP&		GetBITMAP() const
		{ return (BITMAP&)m_Bitmap; }

	/** Return a reference to the bits within the internal BITMAP structure.*/
	VVOIDPTR&	GetBits() const
		{ return (VVOIDPTR&)m_Bitmap.bmBits; }

	/** Return a reference to the bits per pixel value of this bitmap.*/
	VWORD&		GetBitPerPixel() const
		{ return (VWORD&)m_Bitmap.bmBitsPixel; }

	/** Get the bitmap information into a BITMAP structure. If pBitmap is
	NULL, the internal structure will be used.*/
	VBOOL		GetInfo(BITMAP* pBitmap = NULL) const
	{
		return VGDIObject::GetInfo(	sizeof(BITMAP),
									(pBitmap)
									? pBitmap
									: (VVOIDPTR)&m_Bitmap);
	}

	/** Return a reference to the bitmap GDI object handle.*/
	HBITMAP&	GetHandle() const
		{ return (HBITMAP&)VGDIObject::GetHandle(); }

	/** Return a reference to the bitmap height.*/
	VLONG&		GetHeight() const
		{ return (VLONG&)m_Bitmap.bmHeight; }

	/** Return a reference to the bitmap planes.*/
	VWORD&		GetPlanes() const
		{ return (VWORD&)m_Bitmap.bmPlanes; }

	/** Return a pointer to the BITMAP struct pointer.*/
	BITMAP*		GetPointer() const
		{ return (BITMAP*)&m_Bitmap; }

	/** Return a reference to the bitmap width.*/
	VLONG&		GetWidth() const
		{ return (VLONG&)m_Bitmap.bmWidth; }

	/** Return a reference to the width bytes.*/
	VLONG&		GetWidthBytes() const
		{ return (VLONG&)m_Bitmap.bmWidthBytes; }

	/** Load the object with a bitmap from resources using the bitmap
	resource string name.*/
	VBOOL		Load(	VSTRING_CONST	pszBitmap,
						HINSTANCE		hResource =
											NULL)
	{
		return Set(LoadBitmap(	(hResource)
								? hResource
								: VGetResourceHandle(),
								pszBitmap));
	}

	/** Load the object with a bitmap from resources using the bitmap
	resource ID.*/
	VBOOL		LoadID(	VUINT		nBitmapID,
						HINSTANCE	hResource =
										NULL)
	{
		VASSERT(nBitmapID)
		return Load(MAKEINTRESOURCE(nBitmapID), hResource);
	}

protected:
	/** Set the internal HBITMAP object and initialize the bitmap structure.*/
	VBOOL		Set(HBITMAP hBitmap)
	{
		if ( SetHandle(hBitmap) )
		{
			GetInfo();
			return VTRUE;
		}

		VZEROSTRUCT(m_Bitmap);

		return VFALSE;
	}

	/** Embedded Members.*/
	BITMAP		m_Bitmap;
};

#endif /* VBITMAP*/
