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

#ifndef VMAINDIALOG
#define VMAINDIALOG

#include "../vstandard.h"
#include "../gdi/vdc.hpp"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "vdialog.hpp"

/** VMainDialog provides support for applications whose main window is a
dialog box. This should be generally be limited to very simple applications.
Most full blown GUI applications should use VMainWindow, or one of its
derived classes. However, much can be achieved with this simpler method. If
split window support is needed, VWCL also provides the VSplitMainDialog
class. As an ever better optimization, define VWCL_DONT_REGISTER_CLASSES if
you are using this object as your main window. This will cause VWCL to not
register window classes, which is probably not needed if you are making use
of a dialog box as the main application window.*/
class VMainDialog : public VDialog
{
public:
	/** Default constructor should be initialized with an icon resource ID.
	If nIconID is 0, this class will load the default Windows application
	icon. The icon must be loaded from the application instance handle, not
	a resource handle. This is because Windows expects to find the icon in
	the applications executable file, and failure to put one there forces
	Windows to use icon substitutions. If VWCL_DONT_REGISTER_CLASSES is not
	defined, and nIconID is 0, this class will try to load the icon from the
	VWCL window class before using the Windows default icon.*/
	VMainDialog(	VUINT		nIconID =
									0,
					VRTTI_TYPE	nRTTI =
									VWCL_RTTI_MAIN_DIALOG)
		: VDialog(nRTTI)
	{
		/* Install this window as the main window.*/
		MainWindow(VREF_THIS);

		/* Initialize the icon from executable resources?*/
		if ( nIconID )
		{
			m_hIcon =
				LoadIcon(VGetInstanceHandle(), MAKEINTRESOURCE(nIconID));

			VASSERT(m_hIcon)
		}

		if ( !m_hIcon )
		{
			/* Get the application icon from registered class.*/
			WNDCLASS wc;
			VZEROSTRUCT(wc);

			#ifndef VWCL_DONT_REGISTER_CLASSES
				GetClassInfo(VGetInstanceHandle(), VWINDOWCLASS, &wc);
			#endif

			/* Save icon.*/
			m_hIcon =
				(wc.hIcon) ? wc.hIcon : LoadIcon(NULL, IDI_APPLICATION);
		}
	}

	/** Virtual destructor notifies VWCL that the main window is gone.*/
	virtual ~VMainDialog()
		{ SetMainWindowNULL(); }

protected:
	/** Override ends the dialog, and the application.*/
	virtual VLONG	OnClose()
	{
		Destroy();
		PostQuitMessage(0);
		return 0;
	}

	/** Override manages painting when iconized. This is required since
	Windows would not draw an icon for a dialog box by default. This really
	doesn't matter on newer versions of Windows, but for compatiblity sake,
	this code is here.*/
	virtual VBOOL	OnPaint(VDC& dc)
	{
		if ( m_hIcon && IsIconic() )
		{
			VWINDOW_SM1(WM_ICONERASEBKGND, dc.GetHandle());

			VRect r;
			GetClientRect(r);

			dc.DrawIcon((r.GetHalfWidth() - GetSystemMetrics(SM_CXICON)) / 2,
						(r.GetHalfHeight() - GetSystemMetrics(SM_CYICON) / 2),
						m_hIcon);
		}

		return VDialog::OnPaint(dc);
	}

	/** Override sets icon for dialog box. Derived classes should also call
	this function before returning.*/
	virtual VBOOL	SetupWindow()
	{
		/* Save icon in class data?*/
		if ( m_hIcon )
		{
			/* Make sure we get OnPaint() messages.*/
			GetOptions().Append(OPTION_NEED_ON_PAINT);

			/* Set icon into dialog window class.*/
			SetClassLong(GetHandle(), GCL_HICON, (VLONG)m_hIcon);
		}

		return VDialog::SetupWindow();
	}

	/** Override checks for WM_QUERYDRAGICON and returns icon for
	the application.*/
	virtual VLONG	WindowProc(	HWND	hWnd,
								VUINT	nMessage,
								VWPARAM	wParam,
								VLPARAM	lParam)
	{
		if ( nMessage == WM_QUERYDRAGICON )
			return (VLONG)m_hIcon;

		return VDialog::WindowProc(hWnd, nMessage, wParam, lParam);
	}

	/** Embedded Members.*/
	HICON			m_hIcon;
};

#endif /* VMAINDIALOG*/
