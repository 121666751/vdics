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

#ifndef VSPLASHWINDOW
#define VSPLASHWINDOW

#include "../vstandard.h"
#include "../gdi/vbitmap.hpp"
#include "vsplashwindowbase.hpp"

/** VSplashWindow creates a simple (16 color) splash screen. No palette
management is done by this class, so attempting to show more than 16 colors
can result in an undesired quality display. VSplashWindowEx is a more
advanced window, but is also higher in overhead and requirements. The class
gets most of it functionality from the VSplashWindowBase class.*/
class VSplashWindow : public VSplashWindowBase
{
public:
	/** Default constructor initializes the base class. If objects
	of this type should be deleted when the window is destroyed,
	pass VTRUE for the bSelfDelete parameter. This means that the
	object must be allocated on the heap with the "new" operator,
	and cannot be a static object. This can also be set, or removed,
	using the VWindow defined option flag OPTION_SELF_DELETE after
	construction.*/
	VSplashWindow(	VBOOL		bSelfDelete =
									VFALSE,
					VRTTI_TYPE	nRTTI =
									VWCL_RTTI_SPLASH_WINDOW)
		: VSplashWindowBase(nRTTI, bSelfDelete)
	{
		/* Initialize members.*/
		m_pBitmap = NULL;
	}

	/** Virtual destructor verifies internal bitmap is destroyed.*/
	virtual ~VSplashWindow()
		{ DeleteImage(); }

	/** Create the splash window. pszBitmap is the string name of the bitmap
	resource to show, which is loaded from hResource, or VGetResourceHandle()
	if hResource is NULL. See the alternate CreateID() function to specifiy
	the bitmap using a bitmap resource ID rather than the string name. Normally
	the splash window is shown until destructed, which is normally performed in
	the main window SetupWindow() virtual function override, or at another
	point when initialization is complete. To force the splash window to be
	seen for at least x number of seconds, pass the number of seconds as the
	nMinWaitSeconds parameter.*/
	VBOOL			Create(	VSTRING_CONST	pszBitmap,
							HINSTANCE		hResource =
												NULL,
							VUINT			nMinWaitSeconds =
												0)
	{
		/* This must not be known yet!*/
		VASSERT(!m_pBitmap)

		/* This must be given!*/
		VASSERT(pszBitmap)

		/* Allocate bitmap object.*/
		m_pBitmap = VNEW VBitmap(pszBitmap, hResource);

		if ( m_pBitmap && m_pBitmap->GetHandle() )
		{
			/* Create the window same size as bitmap.*/
			if ( VSplashWindowBase::Create(	m_pBitmap->GetWidth(),
											m_pBitmap->GetHeight(),
											nMinWaitSeconds) )
			{
				return VTRUE;
			}
		}

		/* Cleanup on failure.*/
		DeleteImage();

		return VFALSE;
	}

	/** Same as Create(), but uses bitmap ID rather than string name.*/
	VBOOL			CreateID(	VUINT		nBitmapID,
								HINSTANCE	hResource =
												NULL,
								VUINT		nMinWaitSeconds =
												0)
	{
		VASSERT(nBitmapID)

		return Create(	MAKEINTRESOURCE(nBitmapID),
						hResource,
						nMinWaitSeconds);
	}

	/** Delete the bitmap object as needed.*/
	void			DeleteImage()
		{ VDELETE_NULL(m_pBitmap) }

	/** This override draws the bitmap, if one is loaded.*/
	virtual void	Draw(	VDC&	dc,
							VRect&	rClient)
	{
		if ( m_pBitmap )
			m_pBitmap->Draw(dc);
	}

protected:
	/** Embedded Members.*/
	VBitmap*		m_pBitmap;
};

#endif /* VSPLASHWINDOW*/
