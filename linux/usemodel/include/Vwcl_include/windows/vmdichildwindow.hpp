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

#ifndef VMDICHILDWINDOW
#define VMDICHILDWINDOW

#include "../vstandard.h"
#include "../windows/vwindow.hpp"
#include "../windows/vmdimainwindow.hpp"
#include "../windows_controls/vmenu.hpp"

/** VMDIChildWindow represents a Multiple Document Interface (MDI) child
window. Objects of this type are contained by parent windows such as
VMDIMainWindow. This class has built in facilities for self deleting
when the child window object (if dynamically allocated with new) is closed
or destroyed, eleviating the caller from having to maintain large list of
objects to be deleted. In addition, the message routine mechanisms of VWCL
provide for OnCommand() messages to be routed to MDI child windows, so that
they can provide their own processing of menu commands directed to the main
application window. To use this feature, you should override the OnCommand()
method defined in VWindow in your VMDIChildWindow derived classes.*/
class VMDIChildWindow : public VWindow
{
public:
	/** Constructor initalizes members. If the object should delete
	itself when the window is destroyed or closed, you can initialize the
	class with bSelfDelete set to VTRUE. This can also be controlled after
	the object has been created using the OPTION_SELF_DELETE option in the
	base VWindow class. Self deletion is only valid for objects created with
	new. Stack based objects cannot use the self-delete method, and attempting
	to do so would result in an access violation.
	pMenu is a pointer to a VMenu object that is used when windows of this
	type are active. NULL is allowed, and this will cause the class to
	not alter the main menu during activation. hAccelerators is a handle
	to accelerators used when objects of this type are active. It can
	also be NULL if accelerators are not used.*/
	VMDIChildWindow(VMDIMainWindow const&	windowParent,
					VMenu*					pMenu =
												NULL,
					HACCEL					hAccelerators =
												NULL,
					VBOOL					bSelfDelete =
												VFALSE)
		: VWindow(VWCL_RTTI_MDI_CHILD_WINDOW)
	{
		/* Initialize members.*/
		m_pMDIParent =		(VMDIMainWindow*)&windowParent;
		m_pMenu =			pMenu;
		m_hAccel =			hAccelerators;

		if ( bSelfDelete )
			GetOptions().Append(OPTION_SELF_DELETE);

		VASSERT(m_pMDIParent)
	}

	/** Return a handle to the accelerator table object used for
	translating keystrokes into commands. These accelerators are only
	use when a MDI child window of this object is active.*/
	HACCEL					Accelerators() const
		{ return (HACCEL)m_hAccel; }

	/** Set a handle to the accelerator table object used for
	translating keystrokes into commands. If your application uses
	accelerators, you should use this function to store a handle to them
	in this object. These accelerators are only use when a MDI child
	window of this object is active. These accelerators can also be
	specified during window creation. If a window of this type is the
	current MDI child window, the accelerators specified by hAccel will
	become active immediately in the user interface. If the default main
	window accelerators should be used, hAccel can be NULL.*/
	void					Accelerators(HACCEL hAccel)
	{
		VASSERT(m_pMDIParent)

		/* Save handle.*/
		m_hAccel = hAccel;

		/* Update UI now if we are the active child.*/
		if ( m_pMDIParent->GetActiveChild() == this )
			SetMenuAndAccelerators();
	}

	/** Activate this window.*/
	void					Activate() const
	{
		VASSERT(m_pMDIParent)
		m_pMDIParent->SendToClient(WM_MDIACTIVATE, (VWPARAM)GetHandle(), 0);
	}

	/** Activate the next MDI child object.*/
	void					ActivateNext() const
	{
		VASSERT(m_pMDIParent)
		m_pMDIParent->SendToClient(WM_MDINEXT, (VWPARAM)GetHandle(), 0);
	}

	/** Activate the previous MDI child object.*/
	void					ActivatePrevious() const
	{
		VASSERT(m_pMDIParent)
		m_pMDIParent->SendToClient(WM_MDINEXT, (VWPARAM)GetHandle(), 1);
	}

	/** Base class override verifies window is destroyed properly.*/
	virtual void			Close(VBOOL bCloseNow = VTRUE)
		{ Destroy(); }

	/** Create the window. Returns VTRUE on success, VFALSE on failure.
	VWCL registers a window class for MDI child windows. This class is based
	entirely on the default class registered by VWindow. The only exception
	is that the window procedure is modified to point to this classes
	MDIHookProc(). This function is used during WM_CREATE messages to attach
	objects of this type to VWCL.

	Many applications may prefer to register a window class for each type
	of MDI document they display. If this is desired, you should register
	your custom window classes using the MDIHookProc() function from
	this class to verify that objects are properly associated with the
	VWCL runtime message routing mechanism. You must then pass your
	registered class name as the pszClassName parameter. When populating
	the WNDCLASS structure to register your own window class, you should
	set the lpfnWndProc member to MDIHookProc, as in
	wc.lpfnWndProc = VMDIChildWindow::MDIHookProc, where wc is a
	WNDCLASS structure.

	nStyle is one or more of the standard WS_XXX style bits, which are
	defined in VWindow::Create(). You are allowed to specify style
	bits which control how the window is initially shown. However,
	if a current MDI child window exists, and is maximized, the
	new window will also be created maximized. However, if you do not
	specify the WS_VISIBLE bit, the window will not be show, and you
	will have control to show it however you see fit when ready.*/
	VBOOL					Create(	VSTRING_CONST	pszWindowName =
														NULL,
									VSTRING_CONST	pszClassName =
														NULL,
									const VRect&	r =
														VRECT_EMPTY,
									VUINT			nStyle =
														WS_CAPTION |
														WS_CHILD |
														WS_OVERLAPPEDWINDOW |
														WS_VISIBLE)
	{
		/* This must be known!*/
		VASSERT(m_pMDIParent)

		/* This is the name we register, as needed.*/
		VSTRING_CONST pszVWCLClassName = VWINDOWCLASS_MDI_CHILD;

		/* Did the caller specify the window class?*/
		if ( !pszClassName )
		{
			pszClassName = pszVWCLClassName;

			/* Register window class, as needed.*/
			WNDCLASS wc;
			VZEROSTRUCT(wc);

			/* See if we have already done this.*/
			if ( !GetClassInfo(	VGetInstanceHandle(),
								pszClassName,
								&wc) )
			{
				/* Base the class on default VWCL class.*/
				GetClassInfo(	VGetInstanceHandle(),
								DefaultClassName(),
								&wc);

				/* Replace name and window procedure.*/
				wc.lpszClassName =	pszClassName;
				wc.lpfnWndProc =	MDIHookProc;

				/* Register the class.*/
				RegisterClass(&wc);
			}
		}

		/* This must be known by now.*/
		VASSERT(VSTRLEN_CHECK(pszClassName))

		/* Initialize MDICREATESTRUCT structure.*/
		MDICREATESTRUCT	mcs;
		VZEROSTRUCT(mcs);
		mcs.szClass	=	pszClassName;
		mcs.szTitle	=	pszWindowName;
		mcs.hOwner	=	VGetInstanceHandle();
		mcs.style	=	WS_CHILD | nStyle;
		mcs.lParam	=	(VLPARAM)this;

		/* Modify the user defined styles, for now.*/
		mcs.style &= ~(WS_VISIBLE | WS_MAXIMIZE);

		/* Adjust rectangle as needed.*/
		if ( r.IsEmpty() )
		{
			mcs.x =		CW_USEDEFAULT;
			mcs.y =		CW_USEDEFAULT;
			mcs.cx =	CW_USEDEFAULT;
			mcs.cy =	CW_USEDEFAULT;
		}
		else
		{
			mcs.x =		r.GetLeft();
			mcs.y =		r.GetTop();
			mcs.cx =	r.GetWidth();
			mcs.cy =	r.GetHeight();
		}

		/* Create the child window, passing the window object as lParam.
		This will ensure that the window procedure attaches the window to
		the object. We create the window invisible for now.*/
		if ( m_pMDIParent->SendToClient(WM_MDICREATE, 0, (VLPARAM)&mcs) )
		{
			/* Show it now?*/
			if ( nStyle & WS_VISIBLE )
			{
				/* Determine show state.*/
				VINT nShowHow = SW_SHOWNORMAL;

				/* Find out if currently active window is maximized. If it
				is, the new window should also be maximized.*/
				VBOOL bMaximized = VFALSE;

				m_pMDIParent->
					SendToClient(WM_MDIGETACTIVE, 0, (VLPARAM)&bMaximized);

				if ( bMaximized || (nStyle & WS_MAXIMIZE) )
					nShowHow = SW_SHOWMAXIMIZED;
				else if ( nStyle & WS_MINIMIZE )
					nShowHow = SW_SHOWMINIMIZED;

				/* Show the window now.*/
				Show(nShowHow);
				Update();

				/* Set the focus too.*/
				SetFocus();
			}

			/* Notify that we have been created?*/
			m_pMDIParent->OnMDIChildCreated(VREF_THIS);

			return VTRUE;
		}

		return VFALSE;
	}

	/** Override destroys window using proper MDI child technique.*/
	virtual void			Destroy()
	{
		VASSERT(m_pMDIParent)

		/* Let the owner handle the destruction.*/
		m_pMDIParent->SendToClient(WM_MDIDESTROY, (VWPARAM)GetHandle(), 0);
	}

	/** Returns a reference to the parent VMDIMainWindow object.*/
	VMDIMainWindow&			GetOwner()
	{
		VASSERT(m_pMDIParent)
		return VREF_PTR(m_pMDIParent);
	}

	/** Base class override identifies us as a MDI child window.*/
	VBOOL					IsVMDIChildWindowType()
		{ return VTRUE; }

	/** Base class override to maximize this window. This will also maximize
	other MDI child windows.*/
	virtual void			Maximize()
	{
		VASSERT(m_pMDIParent)
		m_pMDIParent->SendToClient(WM_MDIMAXIMIZE, (VWPARAM)GetHandle(), 0);
	}

	/** Store a pointer to the menu to use when a window of this type is
	active. This object must remain in scope at least as long as this object.
	If a window of this type is the current MDI child window, the menu
	specified by pMenu will become active immediately in the user interface.
	If the default main window menu should be used, pMenu can be NULL.*/
	void					Menu(VMenu* pMenu)
	{
		VASSERT(m_pMDIParent)

		/* Save pointer.*/
		m_pMenu = pMenu;

		/* Update UI now if we are the active child.*/
		if ( m_pMDIParent->GetActiveChild() == this )
			SetMenuAndAccelerators();
	}

	/** Return a pointer to the menu to use when a window of this type is
	active. This pointer must have been set during creation or by the use
	of the Menu() mutator function.*/
	VMenu*					Menu() const
		{ return (VMenu*)m_pMenu; }

	/** Default window procedure for MDI child windows. All we do here is
	process the WM_CREATE message and attach the window. Once that is
	done, we leave everything up to VWCL and the overloaded functions. If you
	register your own window class for an MDI child, you should specify this
	function as the lpfnWndProc member of the WNDPROC structure.*/
	static LRESULT CALLBACK	MDIHookProc(	HWND	hWnd,
											VUINT	nMessage,
											WPARAM	wParam,
											LPARAM	lParam)
	{
		if ( nMessage == WM_CREATE )
		{
			MDICREATESTRUCT* pMDI =
				(MDICREATESTRUCT*)((CREATESTRUCT*)lParam)->lpCreateParams;
			VASSERT(pMDI)

			VWindow* pWindow = (VWindow*)pMDI->lParam;
			VASSERT(pWindow)

			return (pWindow->Attach(hWnd)) ? 0 : -1;
		}

		return DefMDIChildProc(hWnd, nMessage, wParam, lParam);
	}

	/** Base class override to restore this window from a maximized or
	minimized state. This will also restore other MDI child windows.*/
	virtual void			Restore()
	{
		VASSERT(m_pMDIParent)
		m_pMDIParent->SendToClient(WM_MDIRESTORE, (VWPARAM)GetHandle(), 0);
	}

protected:
	/** This virtual function is called when this window is activated. The
	base class function does nothing.*/
	virtual void			OnActivate()
		{;}

	/** This virtual function is called when this window is deactivated. The
	base class function does nothing.*/
	virtual void			OnDeactivate()
		{;}

	/** Override calls the OnMDIChildDestroy() method of the VMDIMainWindow
	class.*/
	virtual void			OnDestroy()
	{
		VASSERT(m_pMDIParent)

		/* Notify that we are going to be destroyed.*/
		m_pMDIParent->OnMDIChildDestroy(VREF_THIS);
	}

	/** Local function to set menu and accelerators in main window, based
	on activation state and other menu and accelerator options.*/
	void					SetMenuAndAccelerators(
		VBOOL bMDIChildActive = VTRUE)
	{
		VASSERT(m_pMDIParent)

		/* Set accelerators. They are optional.*/
		if ( bMDIChildActive && m_hAccel )
			m_pMDIParent->GetAccelerators() = m_hAccel;
		else
			m_pMDIParent->GetAccelerators() =
				m_pMDIParent->GetNonMDIAccelerators();

		/* Menu's are optional, so deal with this.*/
		VMenu* pMenu = NULL;

		if ( bMDIChildActive && m_pMenu )
			pMenu = m_pMenu;
		else
			pMenu = &m_pMDIParent->GetMenu();

		if ( pMenu )
		{
			m_pMDIParent->SendToClient(	WM_MDISETMENU,
										(VWPARAM)pMenu->GetHandle(),
										0);

			/* Redraw menu bar.*/
			DrawMenuBar(m_pMDIParent->GetHandle());

			/* Update menu items.*/
			m_pMDIParent->DoMenuItemUpdates();
		}
	}

	/** This override is used to handle MDI messages that are routed to
	the OnActivate() and OnDeactivate() methods. This function also
	allows windows to handle default message processing through the
	DefMDIChildProc() function.*/
	virtual VLONG			WindowProc(	HWND	hWnd,
										VUINT	nMessage,
										VWPARAM	wParam,
										VLPARAM	lParam)
	{
		VASSERT(m_pMDIParent)

		if ( nMessage == WM_MDIACTIVATE )
		{
			/* Determine activation state and call virtual functions.*/
			if ( (HWND)lParam == hWnd )
			{
				/* Set our menu.*/
				SetMenuAndAccelerators();

				/* Force update now of MDI client window so that it properly
				draws active MDI client window, including non-client areas.*/
				VRect r;

				if ( GetUpdateRect(	m_pMDIParent->
										GetMDIClientWindow().GetHandle(),
									r.GetPointer(),
									VFALSE) )
				{
					m_pMDIParent->
						GetMDIClientWindow().InvalidateRectUpdate(r);
				}

				/* Call virtual functions in this class and parent.*/
				OnActivate();
				m_pMDIParent->OnMDIChildActivated(VREF_THIS);
			}
			else if ( wParam )
			{
				/* Only deactivate our menu if another MDI child will
				be setting its own menu soon. In other words, we only
				restore the parent menu when we are the last MDI child
				window and are going away.*/
				if ( !lParam )
					SetMenuAndAccelerators(VFALSE);

				/* Call virtual functions in this class and parent.*/
				OnDeactivate();
				m_pMDIParent->OnMDIChildDeactivated(VREF_THIS);
			}
		}

		return DefMDIChildProc(hWnd, nMessage, wParam, lParam);
	}

	/** Embedded Members.*/

	/* The owner MDI window.*/
	VMDIMainWindow*			m_pMDIParent;

	/* The menu to use when active.*/
	VMenu*					m_pMenu;

	/* The accelerators to use when active.*/
	HACCEL					m_hAccel;
};

#endif /* VMDICHILDWINDOW*/