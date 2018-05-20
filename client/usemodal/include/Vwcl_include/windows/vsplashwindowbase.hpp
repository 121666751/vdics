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

#ifndef VSPLASHWINDOWBASE
#define VSPLASHWINDOWBASE

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../gdi/vdc.hpp"
#include "../structures/vrect.hpp"
#include "vwindow.hpp"
#include "vtempwindow.hpp"

/** VSplashWindowBase implements common features available for applications
displaying a splash (startup) screen. It is intended to be a base class for
other derivations that implement custom painting, such as VSplashWindow and
VSplashWindowEx. The base class handles runtime type setting, determines if
a splash screen should be shown at all, initializes the base VWindow object
to call OnPaint(), and provides a Draw() method which is called by OnPaint()
with the rectangle to render into. It also provides a method to destroy the
window when the user clicks on it, which means the class can be used as an
"about box" as well, where the screen is shown and destroyed when the user
clicks it. The window class it uses is #32770 (dialog box), which means this
class can be used before VApplication::Init() is called.*/
class VSplashWindowBase : public VWindow
{
public:
	/** Determine if a splash window should be created at all. If "NOLOGO"
	is specified on the application command line, a splash window should
	not be created. This is especially important to users that connect to
	Windows Terminal Server, or use applications over another form of slow
	speed multi-user connection. Since this method is static, it can be called
	before creating a derived class object.*/
	static VBOOL	CanShow()
	{
		/* Check for NOLOGO on command line.*/
		if ( VGetApp().IsCommandLineOption(VTEXTA("NOLOGO")) )
			return VFALSE;

		return VTRUE;
	}

	/** Delete the image and resources used, as needed. The base class
	implementation does nothing. This method is called when the window
	or class is destroyed. Derived classes should implement this method
	to release resources used by the class to display the graphic image.
	Since this is normally also called in the destructor, which cannot
	access derived class overrides, derived classes should also call
	their local version of this method in their destructor.*/
	virtual void	DeleteImage()
		{;}

	/** Override this method to perform customing drawing in a derived
	class. The base class does nothing.*/
	virtual void	Draw(	VDC&	dc,
							VRect&	rClient)
		{;}

protected:
	/** Protected constructor initializes RTTI information and sets
	the VWindow option to receive OnPaint() messages. Derived classes
	must pass a correct type for the nRTTI parameter. If the bSelfDelete
	parameter is VTRUE, the base class option flag OPTION_SELF_DELETE will
	be set.*/
	VSplashWindowBase(	VRTTI_TYPE	nRTTI,
						VBOOL		bSelfDelete =
										VFALSE)
		: VWindow(nRTTI)
	{
		/* We need OnPaint().*/
		GetOptions().Append(OPTION_NEED_ON_PAINT);

		if ( bSelfDelete )
			GetOptions().Append(OPTION_SELF_DELETE);
	}

	/** Protected virtual destructor should be overridden in derived classes
	to call the derived class version of DeleteImage().*/
	virtual ~VSplashWindowBase()
		{;}

	/** Protected Create() creates the window based on the dialog window
	class. This means that it can be used before VApplication::Init().
	Derived classes are required to pass the window size to this method,
	and this size should most likely be the exact size of the splash image.
	If there is a need to force the splash screen to be shown for a certain
	number of seconds, this can be specified by the nMinWaitSeconds
	parameter. Once created, the window will be centered, shown, and updated
	by this method. The window will be positioned, and will remain, at the
	top of the z-order until destroyed.*/
	VBOOL			Create(	VUINT	nWidth,
							VUINT	nHeight,
							VUINT	nMinWaitSeconds =
										0)
	{
		/* These must be known.*/
		VASSERT(nWidth)
		VASSERT(nHeight)

		/* Create the window same size as picture.*/
		if (	nWidth &&
				nHeight &&
				VWindow::Create(VTempWindow(GetDesktopWindow()),
								VRect(0, 0, nWidth, nHeight),
								WS_POPUP,
								VTEXT("#32770")) )
		{
			/* Center it.*/
			Center();
			
			/* Make it z-order top.*/
			SetWindowPos(	GetHandle(),
							HWND_TOPMOST,
							0,
							0,
							0,
							0,
							SWP_NOMOVE |
							SWP_NOREDRAW |
							SWP_NOSENDCHANGING |
							SWP_NOSIZE);
			
			/* Show and update now.*/
			Show();
			Update();

			/* Kill nMinWaitSecond's?*/
			if ( nMinWaitSeconds )
			{
				VUINT nNow = GetCurrentTime();
				while (	GetCurrentTime() < nNow + (nMinWaitSeconds * 1000) );
			}

			/* Success.*/
			return VTRUE;
		}

		/* Failure.*/
		return VFALSE;
	}

	/** Override of OnPaint() calculates the window client area and calls
	the Draw() virtual function. This method is not intended to be overridden
	by derived classes and is not declared as virtual.*/
	VBOOL			OnPaint(VDC& dc)
	{
		/* Get rectangle.*/
		VRect r;
		GetClientRect(r);
		
		/* Draw it and tell VWindow that drawing is complete now.*/
		Draw(dc, r);
		return VFALSE;
	}

	/** Override verifies image clean up before class goes out of scope.
	It calls the DeleteImage() virtual function. It then calls the base
	class version of this method.*/
	virtual void	PostDestroy()
	{
		DeleteImage();
		VWindow::PostDestroy();
	}

	/** Override calls Destroy() window when clicked and tells Windows not to
	erase the background since this class fills the background with an image.*/
	virtual VLONG	WindowProc(	HWND	hWnd,
								VUINT	nMessage,
								VWPARAM	wParam,
								VLPARAM	lParam)
	{
		switch ( nMessage )
		{
			case WM_ERASEBKGND:
				return VTRUE;

			case WM_LBUTTONUP:
				Destroy();
				return 0;
		}

		return VWindow::WindowProc(hWnd, nMessage, wParam, lParam);
	}
};

#endif /* VSPLASHWINDOWBASE*/
