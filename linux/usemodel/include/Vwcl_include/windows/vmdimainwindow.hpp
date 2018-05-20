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

Primary Author of this source code file:	Thore B. Karlsen
											(sid6581@mail.utexas.edu)
Other Author(s) of this source code file:	Todd Osborne
											(todd@vwcl.org)
*/

#ifndef VMDIMAINWINDOW
#define VMDIMAINWINDOW

#include "../vstandard.h"
#include "../structures/vrect.hpp"
#include "../windows/vmainwindow.hpp"
#include "../windows/vtempwindow.hpp"
#include "../windows_controls/vmenu.hpp"

/* Forward Declaration(s).*/
class VMDIChildWindow;

/** VMDIMainWindow is a VMainWindow derived class that is used for
applications that present their interface in a Multiple Document
Interface (MDI). Objects of this type create and maintain a MDI
client window. This child window is the parent and container of
MDI child windows, which are encapsulated in the VMDIChildWindow
class.*/
class VMDIMainWindow : public VMainWindow
{
friend class VMDIChildWindow;

public:
	/** Default constructor initalizes members.*/
	VMDIMainWindow(VRTTI_TYPE nType = VWCL_RTTI_MDI_MAIN_WINDOW)
		: VMainWindow(nType)
	{
		/* Initialize members.*/
		m_hNonMDIAccelerators =			NULL;
		m_nMDIClientID =				0;
		m_nFirstMDIChildID =			0;
		m_nWindowMenuCascade =			0;
		m_nWindowMenuTileHorizontal =	0;
		m_nWindowMenuTileVertical =		0;
		m_nWindowMenuNext =				0;
		m_nWindowMenuPrevious =			0;
		m_nWindowMenuClose =			0;
		m_nWindowMenuCloseAll =			0;

		/* Initialize RTTI for m_wndMDIClient.*/
		m_wndMDIClient.GetRTTI() = VWCL_RTTI_MDI_CLIENT_WINDOW;
	}

	/** Activate the child MDI window after the active window.*/
	void				ActivateNext() const
	{
		VWindow* pWindow = (VWindow*)GetActiveChild();

		if ( pWindow )
			SendToClient(WM_MDINEXT, (VWPARAM)pWindow->GetHandle(), 0);
	}

	/** Activate the child MDI window before the active window.*/
	void				ActivatePrevious() const
	{
		VWindow* pWindow = (VWindow*)GetActiveChild();

		if ( pWindow )
			SendToClient(WM_MDINEXT, (VWPARAM)pWindow->GetHandle(), 1);
	}

	/** Arrange minimized MDI child window icons.*/
	void				ArrangeIcons() const
		{ SendToClient(WM_MDIICONARRANGE, 0, 0); }

	/** Cascade child MDI windows. nFlags can be either 0, or the only
	supported flag MDITILE_SKIPDISABLED, which prevents disabled MDI child
	windows from being tiled.*/
	void				Cascade(VUINT Flags = 0) const
		{ SendToClient(WM_MDICASCADE, Flags, 0); }

	/** Close all MDI child windows.*/
	void				CloseAll() const
	{
		if ( m_wndMDIClient.GetHandle() )
			::EnumChildWindows(	m_wndMDIClient.GetHandle(),
								(WNDENUMPROC)CloseEnumProc,
								(VLPARAM)this);
	}

	/** Create the window. Most parameters are the same as the base class
	VMainWindow::Create() method, with the following exceptions:

	nMDIClientID is the ID that will be used when creating the MDI client
	window. It can be any non-zero value you like. The MDI client window
	serves as the parent or container window of MDI child windows.

	nFirstMDIChildID is the ID of the first MDI child window created.

	All other paramaters are the same as the base class VMainWindow::Create().
	Make sure the nFirstChildID and nClientID won't cause any problems with
	any other ID's you may expect to receive in VWindow::OnCommand() or
	WM_COMMAND. The return value is the same as VMainWindow::Create().*/
	VINT				Create(	VUINT			nMenuID,
								VRect const&	r =
													VRECT_EMPTY,
								TBBUTTON const*	pToolBarButtons =
													NULL,
								VUINT			nNumToolBarButtons =
													0,
								VUINT			nToolBarBitmapID =
													0,
								VBOOL			bInitialStatusBar =
													VTRUE,
								VUINT			nStyle =
													WS_OVERLAPPEDWINDOW,
								VBOOL			bEnterMessageLoop =
													VTRUE,
								VUINT			nMDIClientID =
													25000,
								VUINT			nFirstMDIChildID =
													25001)
	{
		/* This must be given.*/
		VASSERT(nMDIClientID)

		/* Initialize remaining members.*/
		m_hNonMDIAccelerators =	GetAccelerators();
		m_nFirstMDIChildID =	nFirstMDIChildID;
		m_nMDIClientID =		nMDIClientID;

		#ifdef VWCL_DEBUG
			/* You should set your accelerators before calling this
			function, as this class must store a copy of the original
			accelerators to restore when an MDI child window is not
			active. Use VWindow::GetAccelerators() to store a copy of
			the accelerators loaded using the LoadAccelerators() API
			call. Accelerators are only checked if a menu ID is given.*/
			if ( nMenuID )
				VASSERT(m_hNonMDIAccelerators)
		#endif

		/* Create the window now.*/
		return VMainWindow::Create(	nMenuID,
									r,
									pToolBarButtons,
									nNumToolBarButtons,
									nToolBarBitmapID,
									bInitialStatusBar,
									nStyle);
	}

	/** Return a pointer to the active MDI child window. If one is not
	active, this function will return NULL. This function is actually
	implemented in vwindow.cpp.*/
	VMDIChildWindow*	GetActiveChild() const;

	/** Return a reference to the MDI client window. The MDI client window
	is the parent (container) of MDI child windows.*/
	VWindow&			GetMDIClientWindow() const
		{ return (VWindow&)m_wndMDIClient; }

	/** Return a handle to the non-MDI accelerator table. These are used
	when no MDI child windows are active, and are what is normally set
	during initialization of the main window object.*/
	HACCEL				GetNonMDIAccelerators() const
		{ return m_hNonMDIAccelerators; }

	/** Handle a command from the "Window" menu. This function is normally
	called from the OnCommand() override of your main window class. If
	the command was handled by this function, the return value is VTRUE.
	If the command is not handled, or is not from the "Window" menu,
	VFALSE will be returned. See SetWindowMenuCommand() for more
	information.*/
	VBOOL				HandleWindowMenuCommand(VUINT nMenuID) const
	{
		if ( nMenuID == m_nWindowMenuCascade )
		{
			Cascade();
			return VTRUE;
		}

		if ( nMenuID == m_nWindowMenuTileHorizontal )
		{
			TileHorizontal();
			return VTRUE;
		}

		if ( nMenuID == m_nWindowMenuTileVertical )
		{
			TileVertical();
			return VTRUE;
		}

		if ( nMenuID == m_nWindowMenuNext )
		{
			ActivateNext();
			return VTRUE;
		}

		if ( nMenuID == m_nWindowMenuPrevious )
		{
			ActivatePrevious();
			return VTRUE;
		}

		if ( nMenuID == m_nWindowMenuClose )
		{
			if ( GetActiveChild() )
				((VWindow*)GetActiveChild())->Close();

			return VTRUE;
		}

		if ( nMenuID == m_nWindowMenuCloseAll )
		{
			CloseAll();
			return VTRUE;
		}

		return VFALSE;
	}

	/** Determine if at least one MDI child window is active.*/
	VBOOL				IsMDIChildActive() const
		{ return (GetActiveChild()) ? VTRUE : VFALSE; }

	/** Send a message to the MDI client window.*/
	VLONG				SendToClient(	VUINT	nMessage,
										VWPARAM	wParam,
										VLPARAM	lParam) const
	{
		if ( m_wndMDIClient.GetHandle() )
			return m_wndMDIClient.VWINDOW_SM2(nMessage, wParam, lParam);

		return 0;
	}

	/** This enumeration is used for the nWhichCommand parameter to
	the SetWindowMenuCommand() method.*/
	enum	{			WINDOW_MENU_CASCADE,
						WINDOW_MENU_TILE_HORIZONTAL,
						WINDOW_MENU_TILE_VERTICAL,
						WINDOW_MENU_NEXT,
						WINDOW_MENU_PREVIOUS,
						WINDOW_MENU_CLOSE,
						WINDOW_MENU_CLOSE_ALL,
			};

	/** Set the menu command ID's for the "Window" menu. Setting these
	allows your main window override of OnCommand() to call the
	HandleWindowMenuCommand() method to automatically perform default
	processing for menu commands that are normally found on the
	"Window" menu. You should pass one of the defined commands from
	the above enumeration and your applications resource ID for the
	command to carry out this function. For example, if you would like
	this class to automatically handle the "Cascade" menu command, you
	would call this function as:

	SetWindowMenuCommand(WINDOW_MENU_CASCADE, ID_WINDOW_CASCADE);

	if you menu command resource ID is ID_WINDOW_CASCADE. Then in the
	OnCommand() override in your main window, you would call
	HandleWindowMenuCommand() to see if this class can provide the
	default processing for the command. If you would like to remove
	this processing, pass 0 for the nMenuID parameter.*/
	void				SetWindowMenuCommand(	VUINT	nWhichCommand,
												VUINT	nMenuID)
	{
		switch ( nWhichCommand )
		{
			case WINDOW_MENU_CASCADE:
				m_nWindowMenuCascade = nMenuID;
				break;

			case WINDOW_MENU_TILE_HORIZONTAL:
				m_nWindowMenuTileHorizontal = nMenuID;
				break;

			case WINDOW_MENU_TILE_VERTICAL:
				m_nWindowMenuTileVertical = nMenuID;
				break;

			case WINDOW_MENU_NEXT:
				m_nWindowMenuNext = nMenuID;
				break;

			case WINDOW_MENU_PREVIOUS:
				m_nWindowMenuPrevious = nMenuID;
				break;

			case WINDOW_MENU_CLOSE:
				m_nWindowMenuClose = nMenuID;
				break;

			case WINDOW_MENU_CLOSE_ALL:
				m_nWindowMenuCloseAll = nMenuID;
				break;
		}
	}

	/** Tile child MDI windows. nFlags can be one of the following
	values:

	MDITILE_HORIZONTAL - Tiles MDI child windows so that they are
	wide rather than tall.

	MDITILE_SKIPDISABLED - Prevents disabled MDI child windows from
	being tiled.

	MDITILE_VERTICAL - Tiles MDI child windows so that they are
	tall rather than wide.*/
	void				Tile(VUINT nFlags = 0) const
		{ SendToClient(WM_MDITILE, nFlags, 0); }

	/* Tile child MDI windows horizontally.*/
	void				TileHorizontal() const
		{ Tile(MDITILE_HORIZONTAL); }

	/* Tile child MDI windows vertically.*/
	void				TileVertical() const
		{ Tile(MDITILE_VERTICAL); }

protected:
	/** Enumeration procedure for closing all child windows.*/
	static BOOL CALLBACK	CloseEnumProc(HWND hWnd, VLPARAM lParam)
	{
		VASSERT(lParam)

		/* Skip icon title windows.*/
		if ( !::GetWindow(hWnd, GW_OWNER) )
			((VMDIMainWindow*)lParam)->
				SendToClient(WM_MDIDESTROY, (VWPARAM)hWnd, 0);

		/* Continue enumeration.*/
		return VTRUE;
	}

	/** Override passes commands to the active MDI child window. Since you
	will most likely override this function in your main window class, make
	sure you call this function for unhandled commands. The implementation
	of this function is in vwindow.cpp. This method is also responsible for
	some aspects of MDI child window management. See the implementation for
	more details.*/
	virtual VLONG		OnCommand(	VUINT	nNotifyCode,
									VUINT	nCommandID,
									HWND	hWndControl);

	/* This virtual function is called when a MDI child window has
	been created. The base class implementation does nothing.*/
	virtual void		OnMDIChildCreated(VMDIChildWindow& window)
		{;}

	/** This virtual function is called when a MDI child window is
	activated. The base class does nothing. It is called after the
	VMDIChildWindow::OnActivate() has completed.*/
	virtual void		OnMDIChildActivated(VMDIChildWindow& window)
		{;}

	/** This virtual function is called when a MDI child window is
	deactivated. The base class does nothing. It is called after the
	VMDIChildWindow::OnDeactivate() has completed.*/
	virtual void		OnMDIChildDeactivated(VMDIChildWindow& window)
		{;}

	/** This virtual function is called just before a MDI child window
	is destroyed. The base class implementation does nothing.*/
	virtual void		OnMDIChildDestroy(VMDIChildWindow& window)
		{;}

	/** This virtual function is called after a MDI child window
	has been destroyed. The base class implementation does nothing.
	NOTE: The window represented by the VMDIChildWindow object passed
	to this function will have already been destroyed. The object is
	valid at this point, but cannot be used as if it were a valid
	window.*/
	virtual void		OnMDIChildDestroyed(VMDIChildWindow& window)
		{;}

	/** Override moves MDI client window to consume client area. An override
	of this function could be used to position the MDI client area anywhere
	within the client area of the main window.*/
	virtual void		RecalcLayout(VRect r)
	{
		if ( m_wndMDIClient.GetHandle() )
			m_wndMDIClient.Move(VRECT_BREAK_WIDTH_HEIGHT(r), VTRUE);
	}

	/** Override initializes MDI client window. If you desire to customize
	the MDI client window, and this can not be done after creation, you
	must override this function to do so. If you do, do not call this
	classes SetupWindow() method.*/
	virtual VBOOL		SetupWindow()
	{
		/* Initialize CLIENTCREATESTRUCT structure.*/
		CLIENTCREATESTRUCT ccs;
		VZEROSTRUCT(ccs);
		ccs.idFirstChild = m_nFirstMDIChildID;

		/* Create the MDI client window.*/
		HWND hWnd =	CreateWindowEx(	0,
									VTEXT("MDICLIENT"),
									NULL,
									WS_CHILD |
									WS_VISIBLE |
									WS_HSCROLL |
									WS_VSCROLL |
									MDIS_ALLCHILDSTYLES,
									0,0,0,0,
									GetHandle(),
									(HMENU)m_nMDIClientID,
									VGetInstanceHandle(),
									&ccs);

		if ( hWnd )
		{
			/* Assign hWnd to object, but don't attach it. No need to.*/
			m_wndMDIClient.GetHandle() = hWnd;

			return VMainWindow::SetupWindow();
		}

		return VFALSE;
	}

	/** Override processes default messages through DefFrameProc()
	rather than DefWindowProc(), as required for MDI parent windows.*/
	virtual VLONG		WindowProc(	HWND	hWnd,
									VUINT	nMessage,
									VWPARAM	wParam,
									VLPARAM	lParam)
	{
		/* Call base class first!*/
		VLONG nResult =
			VMainWindow::WindowProc(hWnd, nMessage, wParam, lParam);

		if ( nResult == VWINDOW_MESSAGE_NOT_HANDLED )
			return DefFrameProc(	hWnd,
									m_wndMDIClient.GetHandle(),
									nMessage,
									wParam,
									lParam);

		return nResult;
	}

	/** Embedded Members.*/

	/* The MDI client window.*/
	VTempWindow			m_wndMDIClient;

	/* ID of the first MDI child created.*/
	VUINT				m_nFirstMDIChildID;

	/* This ID assigned to the MDI client window.*/
	VUINT				m_nMDIClientID;

	/* The main accelerators (no active MDI).*/
	HACCEL				m_hNonMDIAccelerators;

	/* These are used for automatic handling of Window menu items.*/
	VUINT				m_nWindowMenuCascade;
	VUINT				m_nWindowMenuTileHorizontal;
	VUINT				m_nWindowMenuTileVertical;
	VUINT				m_nWindowMenuNext;
	VUINT				m_nWindowMenuPrevious;
	VUINT				m_nWindowMenuClose;
	VUINT				m_nWindowMenuCloseAll;
};

#endif /* VMDIMAINWINDOW*/
