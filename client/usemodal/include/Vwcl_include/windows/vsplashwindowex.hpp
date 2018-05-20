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

#ifndef VSPLASHWINDOWEX
#define VSPLASHWINDOWEX

#include "../vstandard.h"
#include "../gdi/vbrush.hpp"
#include "vsplashwindowbase.hpp"

/** VSplashWindowEx creates a more advanced splash screen than what
VSplashWindow is capable of. It uses the OLE defined IPicture object
to decompress and display images in thousands of colors, and in various
formats including BMP,JPG, GIF, ICO, EMF and WMF. It gets most of it
functionality from the VSplashWindowBase class. Since it uses OLE, full
blown OLE must be initialized before using this class. VActiveXInitializer
can perform this required initialization. In addition, you must link your
application with OLE32.LIB and OLEPRO32.LIB to use this class.*/
class VSplashWindowEx : public VSplashWindowBase
{
public:
	/** Default constructor initializes the base class. If objects
	of this type should be deleted when the window is destroyed,
	pass VTRUE for the bSelfDelete parameter. This means that the
	object must be allocated on the heap with the "new" operator,
	and cannot be a static object. This can also be set, or removed,
	using the VWindow defined option flag OPTION_SELF_DELETE after
	construction.*/
	VSplashWindowEx(VBOOL		bSelfDelete =
									VFALSE,
					VRTTI_TYPE	nRTTI =
									VWCL_RTTI_SPLASH_WINDOW_EX)
		: VSplashWindowBase(nRTTI, bSelfDelete)
	{
		/* Initialize members.*/
		m_pPicture =		NULL;
		m_crFillColor =		0;
		m_bUseFillColor =	VFALSE;
	}

	/** Virtual destructor verifies internal picture is destroyed.*/
	virtual ~VSplashWindowEx()
		{ DeleteImage(); }

	/** Create the splash window by loading a graphic image from a file.
	pszFileName is the file name of the image to load. Normally the splash
	window is shown until destructed, which is normally performed in the
	main window SetupWindow() virtual function override, or at another point
	when initialization is complete. To force the splash window to be seen
	for at least x number of seconds, pass the number of seconds as the
	nMinWaitSeconds parameter.*/
	VBOOL			CreateFromFile(	VSTRING_CONST	pszFileName,
									VUINT			nMinWaitSeconds =
														0)
	{
		return CompleteCreate(	VLoadIPictureFromFile(pszFileName),
								nMinWaitSeconds);
	}

	/** Create the splash window by loading a graphic image from resources.
	pszType is the custom resource type, like "JPEG". pszName is the name of
	the resource, like "SPLASH". hResource is the handle to the library to
	load from. If NULL, VGetResourceHandle() will be used instead. Normally
	the splash window is shown until destructed, which is normally performed
	in the main window SetupWindow() virtual function override, or at another
	point when initialization is complete. To force the splash window to be
	seen for at least x number of seconds, pass the number of seconds as the
	nMinWaitSeconds parameter.*/
	VBOOL			CreateFromResources(VSTRING_CONST	pszType,
										VSTRING_CONST	pszName,
										HINSTANCE		hResource =
															NULL,
										VUINT			nMinWaitSeconds =
															0)
	{
		return CompleteCreate(
			VLoadIPictureFromResources(pszType, pszName, hResource),
			nMinWaitSeconds);
	}

	/** Delete the picture object, as needed.*/
	void			DeleteImage()
	{
		if ( m_pPicture )
		{
			m_pPicture->Release();
			m_pPicture = NULL;
		}
	}

	/** This override draws the picture, if one is loaded.*/
	virtual void	Draw(	VDC&	dc,
							VRect&	rClient)
	{
		if ( !m_pPicture )
			return;

		/* Should we fill with color first?*/
		if ( m_bUseFillColor )
		{
			VBrush brush;

			if ( brush.CreateSolid(m_crFillColor) )
				dc.FillRect(rClient, brush.GetHandle());
		}

		/* Get width and height in himetric.*/
		SIZEL size;
		size.cx = rClient.GetWidth();
		size.cy = rClient.GetHeight();
		VPixelsToHIMETRIC(dc.GetHandle(), &size);

		/* Display picture.*/
		m_pPicture->Render(	dc.GetHandle(),
							VRECT_BREAK_WIDTH_HEIGHT(rClient),
							0,
							size.cy,
							size.cx,
							-size.cy,
							rClient.GetPointer());
	}

	/** If the splash window is being drawn using a transparent image format,
	such as GIF, and the transparency is being used, the background fill color
	should be set using this method. This is the color that will show up
	"behind" the image.*/
	void			SetTransparentBackColor(COLORREF crFillColor)
	{
		m_crFillColor =		crFillColor;
		m_bUseFillColor =	VTRUE;
	}

protected:
	/** Internal function to complete window creation.*/
	VBOOL				CompleteCreate(	IPicture*	pPicture,
										VUINT		nMinWaitSeconds)
	{
		/* This must not be known yet!*/
		VASSERT(!m_pPicture)

		m_pPicture = pPicture;

		if ( m_pPicture )
		{
			/* Get width and height of picture.*/
			SIZEL size;
			m_pPicture->get_Width(&size.cx);
			m_pPicture->get_Height(&size.cy);

			/* Convert himetric to pixels.*/
			VPixelsFromHIMETRIC(NULL, &size);

			if ( size.cx && size.cy )
			{
				/* Create the window same size as picture.*/
				if ( VSplashWindowBase::Create(
					size.cx,
					size.cy,
					nMinWaitSeconds) )
				{
					return VTRUE;
				}
			}
		}

		/* Cleanup on failure.*/
		DeleteImage();

		return VFALSE;
	}

	/** Embedded Members.*/
	IPicture*			m_pPicture;
	COLORREF			m_crFillColor;
	VBOOL				m_bUseFillColor;
};

#endif /* VSPLASHWINDOWEX*/
