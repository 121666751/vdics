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

#ifndef VWINDOW
#define VWINDOW

#include "../vstandard.h"
#include "../vapplication.hpp"
#include "../collections/vlinkedlistmanager.hpp"
#include "../collections/vlinkedlistitem.hpp"
#include "../gdi/vdc.hpp"
#include "../numbers/vbitmask.hpp"
#include "../numbers/vrtti.hpp"
#include "../strings/vstring.hpp"
#include "../strings/vwindowstring.hpp"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"
#include "vwindowmessagefilter.hpp"

/* Forward declarations.*/
class VWindow;

/** Macro that is useful when a VWindow object reference is needed by a
function that expects a parent window but when there is no parent window,
like when creating top level application windows.*/
#define	VWINDOW_EMPTY								VWindow()

/** The following VWINDOW_SMxx macro's are used to wrap calls to
VWindow::SendMessage(), VWindow::SendMessageBOOL(),
VWindow::SendDialogItemMessage(), and VWindow::SendDialogItemMessageBOOL().
They are intended to replace many typecasts within the remainder of the
code, and to verify that those typecast's are to the correct type. They
also aid in debugging, since the compiler will choke if too many or too few
parameters are passed as part of macro expansion. The different macro's
are named for the function they call, and how many parameters are passed to
that function.*/

/** Macro used to call VWindow::SendMessage() for calls that
don't pass wParam or lParam values.*/
#define VWINDOW_SM0(message)						SendMessage(message)

/** Macro used to call VWindow::SendMessage() for calls that
pass wParam but no lParam values.*/
#define VWINDOW_SM1(message, wparam)				SendMessage(message, (VWPARAM)wparam)

/** Macro used to call VWindow::SendMessage() for calls that
pass wParam and lParam values.*/
#define VWINDOW_SM2(message, wparam, lparam)		SendMessage(message, (VWPARAM)wparam, (VLPARAM)lparam)

/** Macro used to call VWindow::SendMessage() for calls that
pass wParam and MAKELONG for 2 values to create the lParam.*/
#define VWINDOW_SM3(message, wparam, l1, l2)		SendMessage(message, (VWPARAM)wparam, (VLPARAM)MAKELONG(l1,l2))

/** Macro used to call VWindow::SendMessageBOOL() for calls
that don't pass wParam or lParam values.*/
#define VWINDOW_SMB0(message)						SendMessageBOOL(message)

/** Macro used to call VWindow::SendMessageBOOL() for calls
that pass wParam but no lParam values.*/
#define VWINDOW_SMB1(message, wparam)				SendMessageBOOL(message, (VWPARAM)wparam)

/** Macro used to call VWindow::SendMessageBOOL() for calls
that pass wParam and lParam values.*/
#define VWINDOW_SMB2(message, wparam, lparam)		SendMessageBOOL(message, (VWPARAM)wparam, (VLPARAM)lparam)

/** Macro used to call VWindow::SendMessageBOOL() for calls
that pass wParam and MAKELONG for 2 values to create the lParam.*/
#define VWINDOW_SMB3(message, wparam, l1, l2)		SendMessageBOOL(message, (VWPARAM)wparam, (VLPARAM)MAKELONG(l1,l2))

/** Macro used to call VWindow::SendDialogItemMessage() for calls
that don't pass wParam or lParam values.*/
#define VWINDOW_SDM0(id, message)					SendDialogItemMessage(id, message)

/** Macro used to call VWindow::SendDialogItemMessage() for calls
that pass wParam but no lParam values.*/
#define VWINDOW_SDM1(id, message, wparam)			SendDialogItemMessage(id, message, (VWPARAM)wparam)

/** Macro used to call VWindow::SendDialogItemMessage() for calls
that pass wParam and lParam values.*/
#define VWINDOW_SDM2(id, message, wparam, lparam)	SendDialogItemMessage(id, message, (VWPARAM)wparam, (VLPARAM)lparam)

/** Macro used to call VWindow::SendDialogItemMessage() for calls
that pass wParam and MAKELONG for 2 values to create the lParam.*/
#define VWINDOW_SDM3(id, message, wparam, l1, l2)	SendDialogItemMessage(id, message, (VWPARAM)wparam, (VLPARAM)MAKELONG(l1,l2))

/** Macro used to call VWindow::SendDialogItemMessageBOOL() for calls
that don't pass wParam or lParam values.*/
#define VWINDOW_SDMB0(id, message)					SendDialogItemMessageBOOL(id, message)

/** Macro used to call VWindow::SendDialogItemMessageBOOL() for calls
that pass wParam but no lParam values.*/
#define VWINDOW_SDMB1(id, message, wparam)			SendDialogItemMessageBOOL(id, message, (VWPARAM)wparam)

/** Macro used to call VWindow::SendDialogItemMessageBOOL() for calls
that pass wParam and lParam values.*/
#define VWINDOW_SDMB2(id, message, wparam, lparam)	SendDialogItemMessageBOOL(id, message, (VWPARAM)wparam, (VLPARAM)lparam)

/** Macro used to call VWindow::SendDialogItemMessageBOOL() for calls
that pass wParam and MAKELONG for 2 values to create the lParam.*/
#define VWINDOW_SDMB3(id, message, wparam, l1, l2)	SendDialogItemMessageBOOL(id, message, (VWPARAM)wparam, (VLPARAM)MAKELONG(l1,l2))

/** Define for unhandled messages.*/
#define VWINDOW_MESSAGE_NOT_HANDLED	-65535

/** VWindow is the base window object encapsulation. It can "be a window" or
can simply provide a wrapper for the Windows HWND data type. It supports
subclassing of non-VWCL windows so that VWCL can handle the message
routing, and provides an easier way to interface with all window objects.
It is derived from VLinkedListItem and managed by a VWCL linked list for
high speed message processing.*/
class VWindow : public VLinkedListItem
{
public:
	/** Eumeration of window options. The default is OPTION_NONE, but can be
	changed using the GetOptions() method. These options have the following
	meaning:

	OPTION_NONE - Nothing. This is the default.

	OPTION_NEED_ON_PAINT - Instructs VWindow that you want it to call your
	OnPaint() override.

	OPTION_DONT_CALL_SETUP_WINDOW - Instructs VWindow that SetupWindow()
	should not be called during Create() functions.

	OPTION_DOUBLE_BUFFERED_PAINT - This flag indicates that a window should
	be painted using a double-buffering technique. That allows the window
	to paint its entire contents to an off-screen device context in the
	OnPaint() handler. Once this handler is complete, the rendered content
	is then transfered to the visible device context in a very fast and
	flicker free BitBlt(). OPTION_NEED_ON_PAINT must also be used when
	a derived class needs to paint, but the double-buffering can support
	windows drawn by Windows or supporting libraries as well, so you need
	not override OnPaint() to take advantage of the double-buffering
	feature.
	
	OPTION_SELF_DELETE - This flag causes the object to delete itself in
	the PostDestroy() method. That is, PostDestroy() calls "delete this".
	This should only be used for windows that are dynamically allocated
	with new, and where the window is designed to manage its own lifetime.*/
	enum	{				OPTION_NONE =					0x0000,
							OPTION_NEED_ON_PAINT =			0x0001,
							OPTION_DONT_CALL_SETUP_WINDOW =	0x0002,
							OPTION_DOUBLE_BUFFERED_PAINT =	0x0004,
							OPTION_SELF_DELETE =			0x0008,
			};

	/** Fast Runtime Type Information supported on ALL compilers.
	Implemented in the VRTTI base class for VWindow and derived classes.*/
	enum	{				VWCL_RTTI_UNKNOWN,
							VWCL_RTTI_BUTTON,
							VWCL_RTTI_CHECKBOX,
							VWCL_RTTI_COMBOBOX,
							VWCL_RTTI_DIALOG,
							VWCL_RTTI_EDIT,
							VWCL_RTTI_GROUPBOX,
							VWCL_RTTI_LISTBOX,
							VWCL_RTTI_LISTVIEW,
							VWCL_RTTI_MAIN_DIALOG,
							VWCL_RTTI_MAIN_WINDOW,
							VWCL_RTTI_RADIO_BUTTON,
							VWCL_RTTI_SCROLLBAR,
							VWCL_RTTI_SPLASH_WINDOW,
							VWCL_RTTI_SPLIT_MAIN_DIALOG,
							VWCL_RTTI_SPLIT_MAIN_WINDOW,
							VWCL_RTTI_SPLIT_WINDOW,
							VWCL_RTTI_STATIC,
							VWCL_RTTI_STATUSBAR,
							VWCL_RTTI_TAB,
							VWCL_RTTI_TAB_WINDOW,
							VWCL_RTTI_TOOLBAR,
							VWCL_RTTI_TREEVIEW,
							VWCL_RTTI_WINDOW,
							VWCL_RTTI_SPLIT_DIALOG,
							VWCL_RTTI_MINI_WINDOW,
							VWCL_RTTI_DIRECTORY_TREEVIEW,
							VWCL_RTTI_FORMATTED_EDIT,
							VWCL_RTTI_PROPERTY_SHEET,
							VWCL_RTTI_PROPERTY_PAGE,
							VWCL_RTTI_PROGRESS,
							VWCL_RTTI_HIERARCHICAL_DATABASE_TREEVIEW,
							VWCL_RTTI_PAGE_SETUP_DIALOG,
							VWCL_RTTI_PRINT_DIALOG,
							VWCL_RTTI_BROWSE_FOR_FOLDER_DIALOG,
							VWCL_RTTI_COLOR_DIALOG,
							VWCL_RTTI_FILE_DIALOG,
							VWCL_RTTI_FIND_DIALOG,
							VWCL_RTTI_FONT_DIALOG,
							VWCL_RTTI_REBAR,
							VWCL_RTTI_TOOLTIP,
							VWCL_RTTI_TEMP_WINDOW,
							VWCL_RTTI_MDI_MAIN_WINDOW,
							VWCL_RTTI_MDI_CHILD_WINDOW,
							VWCL_RTTI_MDI_CLIENT_WINDOW,
							VWCL_RTTI_MDI_SPLIT_MAIN_WINDOW,
							VWCL_RTTI_DATETIME_CONTROL,
							VWCL_RTTI_MONTH_CALENDAR_CONTROL,
							VWCL_RTTI_UPDOWN_CONTROL,
							VWCL_RTTI_CLOSE_X_BUTTON,
							VWCL_RTTI_MDI_SPLIT_CHILD_WINDOW,
							VWCL_RTTI_OUTLOOK_BAR,
							VWCL_RTTI_HEADER,
							VWCL_RTTI_SPLASH_WINDOW_EX,
			};

	/** Standard constructor initializes class runtime type information
	and member variables.*/
	VWindow(VRTTI_TYPE nRTTI = VWCL_RTTI_WINDOW);

	/** Virtual destructor destroys window (if not already done)
	and releases internal variables.*/
	virtual ~VWindow();

	/** Attach an existing Windows window to a VWindow object, subclassing
	it, and routing messages through the object. After a window has been
	attached, the SetupWindow() virtual function will be called. This
	function will return VFALSE only if SetupWindow() also returns VFALSE.
	Normally SetupWindow() will only be called if an attachment was needed
	to make the window owned by VWCL. However, if SetupWindow() should
	always be called, by VTRUE for the bAlwaysCallSetupWindow paremter. This
	will NOT override the option OPTION_DONT_CALL_SETUP_WINDOW. If this
	option is set, SetupWindow() will not be called.*/
	VBOOL					Attach(	HWND	hWnd,
									VBOOL	bAlwaysCallSetupWindow =
												VFALSE);

	/** Center the window relative to the entire screen.*/
	void					Center() const
	{
		VASSERT(GetSafeWindow())

		VRect r;
		GetRect(r);
		SetWindowPos(	m_hWindow,
    					HWND_TOP,
    					(GetSystemMetrics(SM_CXSCREEN) / 2) -
						r.GetHalfWidth(),
    					(GetSystemMetrics(SM_CYSCREEN) / 2) -
						r.GetHalfHeight(),
						0,
						0,
						SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	/** Check or Un-Check a button, radio button, or check box that is a
	child of this window. nID is the resource or child window ID of the
	child window to check.*/
	void					CheckButton(VUINT nID,
										VBOOL bCheck =
												VTRUE) const
	{
		VASSERT(GetSafeWindow())
		CheckDlgButton(m_hWindow, nID, (bCheck) ? BST_CHECKED : BST_UNCHECKED);
	}

	/** Converts the client coordinates of a specified point to screen
	coordinates. The ClientToScreen function replaces the client
	coordinates in the VPoint object with the screen coordinates. The screen
	coordinates are relative to the upper-left corner of the screen.*/
	void					ClientToScreen(VPoint& pt) const
	{
		VASSERT(GetSafeWindow())
		::ClientToScreen(m_hWindow, pt.GetPointer());
	}

	/** Causes the window to be closed. This sends the WM_CLOSE message, which
	VWCL routes to the OnClose() method. There are two methods to close a
	window. You can choose to close it right now, or delayed. The difference
	is how the message is sent. Immediate closing uses SendMessage(), while
	the delayed close will use PostMessage(), allowing the caller to finish
	processing a function before the message will be received.*/
	virtual void			Close(VBOOL bCloseNow = VTRUE)
	{
		if ( bCloseNow )
			VWINDOW_SM0(WM_CLOSE);
		else
			PostMessage(WM_CLOSE, 0, 0);
	}

	/** Lowest common denominator of window creation functions.

	windowParent::m_hWindow can be NULL in which case this function will
	attempt to create top level window. You can use the VWINDOW_EMPTY macro
	to specify a VWindow object that is NULL

	The rectangle specified by r is used to determine window placement
	and position. The left and right members determine the window position,
	while the right and bottom members determine the width and height. If r
	is an empty rectangle (use VRECT_EMPTY), and WS_CHILD style bit is not
	set (in nStyle), the function will allow Windows to determine the window
	size and position.

	nStyle is a	combination of WS_xxx bits that Windows defines for window
	styles. At the time of this writing these are:

	WS_BORDER - Creates a window that has a border.

	WS_CAPTION - Creates a window that has a title bar (implies the WS_BORDER
	style). Cannot be used with the WS_DLGFRAME style.

	WS_CHILD - Creates a child window. Cannot be used with the WS_POPUP style.

	WS_CLIPCHILDREN - Excludes the area occupied by child windows when you
	draw within the parent window. Used when you create the parent window.
	VWCL appends this style automatically for windows that do not have
	groupbox controls as children.

	WS_CLIPSIBLINGS - Clips child windows relative to each other, that is,
	when a particular child window receives a paint message, the
	WS_CLIPSIBLINGS style clips all other overlapped child windows out of
	the region of the child window to be updated. (If WS_CLIPSIBLINGS is not
	given and child windows overlap, when you draw within the client area of
	a child window, it is possible to draw within the client area of a
	neighboring child window.) For use with the WS_CHILD style only. VWCL
	appends this style automatically for all child windows.

	WS_DISABLED - Creates a window that is initially disabled.

	WS_DLGFRAME - Creates a window with a double border but no title.

	WS_GROUP - Specifies the first control of a group of controls in which
	the user can move from one control to the next with the arrow keys. All
	controls defined with the WS_GROUP style FALSE after the first control
	belong to the same group. The next control with the WS_GROUP style starts
	the next group (that is, one group ends where the next begins).

	WS_HSCROLL - Creates a window that has a horizontal scroll bar.

	WS_MAXIMIZE - Creates a window of maximum size.

	WS_MAXIMIZEBOX - Creates a window that has a Maximize button.

	WS_MINIMIZE - Creates a window that is initially minimized. For use
	with the WS_OVERLAPPED style only.

	WS_MINIMIZEBOX - Creates a window that has a Minimize button.

	WS_OVERLAPPED - Creates an overlapped window. An overlapped window
	usually has a caption and a border.

	WS_OVERLAPPEDWINDOW - Creates an overlapped window with the WS_OVERLAPPED,
	WS_CAPTION, WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX, and WS_MAXIMIZEBOX
	styles.

	WS_POPUP - Creates a pop-up window. Cannot be used with the WS_CHILD
	style.

	WS_POPUPWINDOW - Creates a pop-up window with the WS_BORDER, WS_POPUP, and
	WS_SYSMENU styles. The WS_CAPTION style must be combined with the
	WS_POPUPWINDOW style to make the Control menu visible.

	WS_SYSMENU - Creates a window that has a Control-menu box in its title
	bar. Used only for windows with title bars.

	WS_TABSTOP - Specifies one of any number of controls through which the
	user can move by using the TAB key. The TAB key moves the user to the
	next control specified by the WS_TABSTOP style.

	WS_THICKFRAME - Creates a window with a thick frame that can be used
	to size the window.

	WS_VISIBLE - Creates a window that is initially visible.

	WS_VSCROLL - Creates a window that has a vertical scroll bar.

	pszClassName is the string name of the registered Windows class to
	create. If NULL, VWINDOWCLASS is assumed, but this can be user defined
	using the DefaultClassName() function.

	pszWindowName is a pointer to a string that will be the window caption,
	or title.

	nIDOrMenu can either be a window ID or a HMENU if the window is to have
	an associated menu.

	After creation, the SetupWindow() virtual function will be called unless
	the OPTION_DONT_CALL_SETUP_WINDOW option is set.*/
	VBOOL					Create(	VWindow const&	windowParent,
									VRect const&	r,
									VUINT			nStyle,
									VSTRING_CONST	pszClassName =
														NULL,
									VSTRING_CONST	pszWindowName =
														NULL,
									VUINT			nIDorMenu =
														0);

	#ifndef VWCL_DONT_REGISTER_CLASSES
		/** The default name VWCL uses to register windows classes is
		VWINDOWCLASS. You can change this by calling this function and
		setting the name to whatever you desire. This would allow, for
		instance, a DLL built using VWCL to register a name that it uses
		internally to reference a class and not effect the calling
		executables ability to register and use its own VWCL or default
		class name. This function is not available if
		VWCL_DONT_REGISTER_CLASSES is defined. This function MUST be called
		before VApplication::Init(), since that function registers the
		class name.*/
		static void				DefaultClassName(VSTRING_CONST pszClassName);

		/** Get the default name VWCL uses to register windows classes. This
		will be either a user defined name set with DefaultClassName(string)
		or VWINDOWCLASS. This function will never return NULL. This function
		is not available if VWCL_DONT_REGISTER_CLASSES is defined.*/
		static VSTRING_CONST	DefaultClassName();
	#endif

	/** The Destroy function destroys the specified window, as needed. The
	function sends WM_DESTROY and WM_NCDESTROY messages to the window to
	deactivate it and remove the keyboard focus from it. The function also
	destroys the window's menu, flushes the thread message queue, destroys
	timers, removes clipboard ownership, and breaks the clipboard viewer
	chain (if the window is at the top of the viewer chain). If the
	specified window is a parent or owner window, Destroy() automatically
	destroys the associated child or owned windows when it destroys the
	parent or owner window. The function first destroys child or owned
	windows, and then it destroys the parent or owner window. Destroy()
	also destroys modeless dialog boxes created by the CreateDialog() or
	VDialog::Create() functions.*/
	virtual void			Destroy()
	{
		if ( GetSafeWindow() )
			DestroyWindow(m_hWindow);
	}

	/** Detach an existing Windows window from an associated VWindow object,
	removing any subclassing, and restore to previous state. This function
	DOES NOT set the m_hWindow HWND to NULL, thus allowing the use of the
	VWindow object after Detach() has been called. Messages are not routed,
	but VWindow methods can still be called. It is not possible to detach a
	VWCL created window. Only windows that are subclassed from another
	window, like controls for instance, can be detached. Detaching a VWCL
	created window (created with VWindow::Create()) would leave the window
	without a window procedure, something that cannot be done.*/
	void					Detach();

	/** Translates the text of a specified control in a dialog box or child
	window into an integer value. nID is the resource or child window ID of
	the child window to retrieve the value for. pbSuccess is a pointer to a
	Boolean variable that receives a function success/failure value. VTRUE
	indicates success, VFALSE indicates failure. This parameter is optional
	and can be NULL. In that case, the function returns no information about
	success or failure. bSigned specifies whether the function should
	examine the text for a minus sign at the beginning and return a signed
	integer value if it finds one. VTRUE specifies that this should be done,
	VFALSE that it should not. The DialogItemINT() function retrieves the
	text of the given control by sending the control a WM_GETTEXT message.
	The function translates the retrieved text by stripping any extra
	spaces at the beginning of the text and then converting the decimal
	digits. The function stops translating when it reaches the end of the
	text or encounters a nonnumeric character. If the bSigned parameter is
	VTRUE, the DialogItemINT() function checks for a minus sign at the
	beginning of the text and translates the text into a signed integer
	value. Otherwise, the function creates an unsigned integer value. The
	DialogItemINT() function returns zero if the translated value is greater
	than INT_MAX (for signed numbers) or UINT_MAX (for unsigned numbers).*/
	VINT					DialogItemINT(	VUINT		nID,
											VWINBOOL*	pbSuccess =
															NULL,
											VBOOL		bSigned =
															VFALSE) const
	{
		VASSERT(GetSafeWindow())
		return GetDlgItemInt(m_hWindow, nID, pbSuccess, bSigned);
	}

	/** Sets the text of a control in a dialog box or child window to the
	string representation of a specified integer value. nID is the resource
	or child window ID of the child window to set the text into.*/
	void					DialogItemINT(	VUINT	nID,
											VINT	nValue,
											VBOOL	bSigned =
														VFALSE) const
	{
		VASSERT(GetSafeWindow())
		SetDlgItemInt(m_hWindow, nID, nValue, bSigned);
	}

	/** Return the unsigned integer value of the child window at nID. This
	is the same as DialogItemINT(), but does not allow the function to
	return a negative value.*/
	VUINT					DialogItemUINT(	VUINT		nID,
											VWINBOOL*	pbSuccess =
															NULL) const
		{ return (VUINT)DialogItemINT(nID, pbSuccess, VFALSE); }

	/** Sets the text of a control in a dialog box or child window to the
	string representation of a specified unsigned integer value. nID is the
	resource or child window ID of the child window to set the text into.*/
	void					DialogItemUINT(	VUINT nID,
											VUINT nValue) const
		{ DialogItemINT(nID, (VINT)nValue, VFALSE); }

	/** Enable or disable all child windows.*/
	void					EnableChildren(VBOOL bEnable = VTRUE) const;

	/** The Enabled function enables or disables mouse and keyboard input.
	When input is disabled, the window does not receive input such as mouse
	clicks and key presses. When input is enabled, the window receives all
	input. If the enabled state of a window is changing, a WM_ENABLE message
	is sent before the Enabled function returns. If a window is already
	disabled, all its child windows are implicitly disabled, although they
	are not sent a WM_ENABLE message. A window must be enabled before it can
	be activated. For example, if an application is displaying a modeless
	dialog box and has disabled its main window, the application must enable
	the main window before destroying the dialog box. Otherwise, another
	window will receive the keyboard focus and be activated. If a child
	window is disabled, it is ignored when the system tries to determine
	which window should receive mouse messages. By default, a window is
	enabled when it is created. To create a window that is initially
	disabled, an application can specify the WS_DISABLED style in the
	Create() function. After a window has been created, an application can
	use Enabled to enable or disable the window. A disabled control cannot
	receive the keyboard focus, nor can a user gain access to it.*/
	void					Enabled(VBOOL bEnable) const
	{
		VASSERT(GetSafeWindow())
		EnableWindow(m_hWindow, bEnable);
	}

	/** Returns VTRUE if the window is enabled, VFALSE otherwise.*/
	VBOOL					Enabled() const
	{
		VASSERT(GetSafeWindow())
		return (IsWindowEnabled(m_hWindow)) ? VTRUE : VFALSE;
	}

	/** Same as above, but enables or disables a child window. nID is the
	resource or child window ID of the child window to enable or disable.*/
	void					EnableChild(	VUINT nID,
											VBOOL bEnable =
													VTRUE) const
	{
		VASSERT(GetSafeWindow())
		EnableWindow(GetDialogItem(nID), bEnable);
	}

	/** Enter a message loop. This is normally called from a "main window"
	class, but could be called anywhere in an application when it is ready
	to enter the application message loop. See the SDK documentation on
	GetMessage() to determine what the return value means.*/
	VWPARAM					EnterMessageLoop();

	/** Find a child window when the name of the child window class is known.*/
	HWND					FindChildWindowByClass(
		VSTRING_CONST pszClass) const;

	/** Retrieve the font with which the window is currently drawing its text.*/
	HFONT					Font() const
		{ return (HFONT)VWINDOW_SM0(WM_GETFONT); }

	/** Set the font that the window is to use when drawing text. hFont is a
	handle to the font. If this parameter is NULL, the control uses the
	default system font to draw text. bRedraw specifies whether the
	window should be redrawn immediately upon setting the font.*/
	void					Font(	HFONT hFont,
									VBOOL bRedraw =
											VTRUE) const
	{
		VASSERT(GetSafeWindow())
		VWINDOW_SM3(WM_SETFONT, hFont, bRedraw, 0);
	}

	/** Returns a reference to the accelerator table object used for
	translating keystrokes into commands. If your application uses
	accelerators, you should use this function to store a handle to them
	in this window. NOTE: This is only used by windows that enter a message
	loop using the EnterMessageLoop() method. Accelerators are not allowed
	to be specified for arbritrary windows, and doing so will result in
	nothing happening. Normally, this function is used to store accelerators
	for main window objects.*/
	HACCEL&					GetAccelerators() const
		{ return (HACCEL&)m_hAccel; }

	/** Get the rectangle of the window caption (title) area in the
	non-client area. This base class function only works when the window
	has the WS_CAPTION style bit set. Derived classes may not have this
	requirement.*/
	virtual void			GetCaptionRect(VRect& r)
	{
		if ( Style() & WS_CAPTION )
		{
			/* Get size of frame around window.*/
			VUINT nStyle =
				Style();

			VINT ncxIcon =
				GetSystemMetrics(SM_CXSIZE);

			VINT nCX =
				GetSystemMetrics(	(nStyle & WS_THICKFRAME)
									? SM_CXSIZEFRAME
									: SM_CXFIXEDFRAME);

			VINT nCY =
				GetSystemMetrics(	(nStyle & WS_THICKFRAME)
									? SM_CYSIZEFRAME
									: SM_CYFIXEDFRAME);

			/* Compute rectangle.*/
			GetRect(r);
			r.Offset(-r.GetLeft(), -r.GetTop());
			r.AddLeft(nCX);
			r.AddRight(-nCX);
			r.AddTop(nCY);
			r.GetBottom() = r.GetTop() +
							GetSystemMetrics(SM_CYCAPTION) -
							GetSystemMetrics(SM_CYBORDER);
		}
		else
			r.Empty();
	}

	/** Retrieves the coordinates of the window's client area. The client
	coordinates specify the upper-left and lower-right corners of the client
	area. Because client coordinates are relative to the upper-left corner
	of a window's client area, the coordinates of the upper-left corner are
	always 0,0.*/
	void					GetClientRect(VRect& r) const
	{
		VASSERT(GetSafeWindow())
		::GetClientRect(m_hWindow, r.GetPointer());
	}

	/** Get the rectangle of a child window into a VRect object. On output, r
	will contain the rectangle of the child window in coordinates relative
	to the client area of this window. Returns VTRUE on success, VFALSE
	on failure.*/
	VBOOL					GetChildRect(	VUINT	nID,
											VRect&	r) const
	{
		r.Empty();

		HWND hWnd = GetDialogItem(nID);

		if ( hWnd )
		{
			if ( GetWindowRect(hWnd, r.GetPointer()) )
			{
				MapWindowPoints(NULL,
								m_hWindow,
								(POINT*)r.GetPointer(),
								2);
				return VTRUE;
			}
		}

		return VFALSE;
	}

	/** Return a reference to the MSG structure for the message currently
	being processed.*/
	static MSG&				GetCurrentMessage();

	/** Get the HWND of a child window when the window or resource ID is
	known. nID is the resource or child window ID of the child window to
	retrieve.*/
	HWND					GetDialogItem(VUINT nID) const
	{
		VASSERT(GetSafeWindow())
		return GetDlgItem(m_hWindow, nID);
	}

	/** Retrieves the title or text associated with a control in a dialog
	box or child window. nID is the resource or child window ID of the
	child window to retrieve the text for. The text is retreived into a
	VString object, strBuffer. Normally, the entire text of the child
	window is retrieved, but if you are only interested in a certain number
	of bytes, this can be specified in the nMaxChars value. This value
	specifies the number of characters to get, including the NULL
	terminating character. So if you want 2 characters, pass 2 for nMaxChars.
	3 total bytes will be allocated and 2 characters copied to the buffer.
	The return value is the internal string pointer within strBuffer on
	success, NULL on failure. strBuffer will not be modified on failure.*/
	VSTRING					GetDialogItemText(	VUINT		nID,
												VString&	strBuffer,
												VUINT		nMaxChars =
																0) const
	{
		VWindowString ws(strBuffer);
		return ws.Load(GetDialogItem(nID), nMaxChars);
	}

	/** Same as GetDialogItemText() except it appends the window text to
	an existing string, if any.*/
	VSTRING					GetDialogItemTextAppend(VUINT		nID,
													VString&	strBuffer,
													VUINT		nMaxChars =
																	0) const
	{
		VWindowString ws(strBuffer);
		return ws.Append(GetDialogItem(nID), nMaxChars);
	}

	/** Return a reference to the VWindow's associated Windows HWND. Most
	useful when passing a VWindow object to a Windows function that requires
	a HWND where VWCL does not provide a wrapper for the API call. This is a
	reference so that specialized functions can set this handle to NULL.
	This is not recommended unless advanced knowledge of the inner-workings
	of VWCL are well known.*/
	HWND&					GetHandle() const
		{ return (HWND&)m_hWindow; }

	/** Return the control ID of this window. This number is specified
	during the call to Create(). It will be 0 before the window is
	created, or after it is destroyed.*/
	VUINT					GetID() const
		{ return m_nID; }

	/** Return a reference to the internal linked list object VWCL uses to
	keep track of VWindow objects. The actual list used is private and not
	a VLinkedListManager type, so it is never valid to delete or add
	VWindow objects to this list. It is only valid to walk this list, in a
	read-only manner. VWindow objects add and delete themselves from this
	list during construction and destruction.*/
	static VLinkedListManager&	GetLinkedList();

	/** Retrieve the windows options object. See enumeration at top of class
	for more information.*/
	VBitmask&				GetOptions() const
		{ return (VBitmask&)m_WindowOptions; }

	/** Retrieves the dimensions of the bounding rectangle of the window. The
	dimensions are given in screen coordinates that are relative to the
	upper-left corner of the screen.*/
	void					GetRect(VRect& r) const
	{
		VASSERT(GetSafeWindow())
		GetWindowRect(m_hWindow, r.GetPointer());
	}

	/** Return a reference to the runtime type value.*/
	VRTTI_TYPE&				GetRTTI() const
		{ return (VRTTI_TYPE&)m_RTTI.GetRTTI(); }

	/** Test to see if this object has an associated, and valid HWND,
	attached to it. Returns HWND on success, NULL on failure.*/
	HWND					GetSafeWindow() const
		{ return (IsWindow()) ? m_hWindow : NULL; }

	/** Retrieves the title or text associated with this window. The text is
	retreived into a VString object, strBuffer. Normally, the entire text
	of the window is retrieved, but if you are only interested in a certain
	number of bytes, this can be specified in the nMaxChars value. This
	value specifies the number of characters to get, including the NULL
	terminating character. So if you want 2 characters, pass 2 for
	nMaxChars. 3 total bytes will be allocated and 2 characters copied to
	the buffer. The return value is the internal string pointer within
	strBuffer on success, NULL on failure. strBuffer will not be modified
	on failure.*/
	VSTRING					GetText(VString&	strBuffer,
									VUINT		nMaxChars =
													0) const
	{
		VWindowString ws(strBuffer);
		return ws.Load(m_hWindow, nMaxChars);
	}

	/** Same as GetText() except it appends the window text to an existing
	string, if any.*/
	VSTRING					GetTextAppend(	VString&	strBuffer,
											VUINT		nMaxChars =
															0) const
	{
		VWindowString ws(strBuffer);
		return ws.Append(m_hWindow, nMaxChars);
	}

	/** Determine the length, in characters, of the text associated with the
	window. The length does not include the terminating null character. For
	an edit control, the text to be copied is the content of the edit
	control. For a combobox, the text is the content of the edit control
	(or static-text) portion of the combo box. For a button, the text is the
	button name. For other windows, the text is the window title. To
	determine the length of an item in a listbox, an application can use
	the VListBox::GetItemTextLength() function. Under certain conditions,
	the function returns a value that is larger than the actual length of
	the text. This occurs with certain mixtures of ANSI and Unicode, and is
	due to the system allowing for the possible existence of DBCS characters
	within the text. The return value, however, will always be at least as
	large as the actual length of the text; you can thus always use it to
	guide buffer allocation. This behavior can occur when an application
	uses both ANSI functions and common dialogs, which use Unicode.*/
	VUINT					GetTextLength() const
		{ return (VUINT)VWINDOW_SM0(WM_GETTEXTLENGTH); }

	/** Retrieves a pointer to the parent window, which must be a VWindow
	object. If the function fails, the return value is NULL. This function
	can also be used to get the parent window for an object when only the
	HWND is known. Just pass a non-NULL value for hWnd and that window
	handle will be used instead of m_hWindow.*/
	VWindow*				GetVParent(HWND hWnd = NULL) const
	{
		if ( !hWnd )
			hWnd = m_hWindow;

		if ( hWnd )
			hWnd = GetParent(hWnd);

		return GetVWindowFromHandle(hWnd);
	}

	/** Get a VWindow pointer when a HWND is known. Returns NULL on failure.
	This could fail if the requested window identified by hWnd is not a VWCL
	window, a window not subclassed by VWCL, or where VWCL has not yet
	completed its ownership code, or after a VWCL window has been destroyed
	or un-attached.*/
	static VWindow*			GetVWindowFromHandle(HWND hWnd);

	/** Hide this window, and activate another.*/
	void					Hide() const
		{ Show(SW_HIDE); }

	/** Install a notification message filter. This is a
	VWindowMessageFilter object that receives messages intended for this
	window before this window acts on the message. This allows
	VWindowMessageFilter derived objects to respond to messages intended
	for this window. This allows non-VWindow classes to receive
	notifications about messages intended for this window. It is valid to
	call this function several times for each VWindowMessageFilter object
	acting as a filter, as this object uses a reference count. In other
	words, you can install the same message filter object multiple times
	without harm. You should however remove the filter when no longer
	needed by calling this function will the bInstall parameter set to
	VFALSE. If the VWindow associated with the messageFilter becomes
	invalid, this class is smart enough to detect this and will
	automatically remove the it from the notification chain. This also
	occurs during normal destroying of VWindow objects, so if the message
	filter is normally removed when the associated window is destroyed,
	there is no reason to manualy un-install the filter. A filter that
	implements this type of processing must override the OnMessageFilter()
	function of the VWindowMessageFilter class. If bRemoveAll is VTRUE, the
	message filter messageFilter is removed from the message filter list,
	even if its reference count in not 0. bRemoveAll forces the removal of
	all VWindowMessageFilter objects from the VWindow object, and is
	normally only used by VWCL internally.*/
	void					InstallMessageFilter(
		VWindowMessageFilter&	messageFilter,
		VBOOL					bInstall =
									VTRUE,
		VBOOL					bRemoveAll =
									VFALSE);

	/** Invalidate the entire window, but do not force an update immediately.*/
	void					Invalidate(VBOOL bErase = VTRUE) const
		{ InvalidateRectUpdate(NULL, bErase, VFALSE); }

	/** Invalidate the entire window, and force an update immediately.*/
	void					InvalidateUpdate(VBOOL bErase = VTRUE) const
		{ InvalidateRectUpdate(NULL, bErase, TRUE); }

	/** Invalidate part of the entire window, but do not force an update
	immediately. The area to invalidate is defined by the coordinates of r.*/
	void					InvalidateRect(	VRect const&	r,
											VBOOL			bErase =
																VTRUE) const
		{ InvalidateRectUpdate(&r, bErase, VFALSE); }

	/** Invalidate part of the entire window, and force an update
	immediately. The area to invalidate is defined by the coordinates of r.*/
	void					InvalidateRectUpdate(
		VRect const&	r,
		VBOOL			bErase =
							VTRUE,
		VBOOL			bUpdateNow =
							VTRUE) const
		{ InvalidateRectUpdate(&r, bErase, bUpdateNow); }

	/** Invalidate part of the entire window, and force an update
	immediately. The area to invalidate is defined by the coordinates of
	pRect. If pRect is NULL, this function invalidates and entire window
	client area.*/
	void					InvalidateRectUpdate(
		VRect const*	pRect =
							NULL,
		VBOOL			bErase =
							VTRUE,
		VBOOL			bUpdateNow =
							VTRUE) const
	{
		VASSERT(GetSafeWindow())

		::InvalidateRect(	m_hWindow,
							(pRect) ? pRect->GetPointer() : NULL,
							bErase);

		if ( bUpdateNow )
			Update();
	}

	/** Returns VTRUE if VWCL is managing this window, VFALSE otherwise.
	VWCL is considered to be managing a window if the window was created
	with VWindow::Create() or a derived class-function, or if a window has
	been subclassed by replacing the window procedure with
	VWindow::WindowProcShared(), such as by using the Attach() method.*/
	VBOOL					IsAttached()
	{
		if ( m_pfnOldWndProc )
			return VTRUE;

		if ( Long(	(IsVDialogType())
					? DWL_DLGPROC
					: GWL_WNDPROC) == (VLONG)WindowProcShared )
			return VTRUE;

		return VFALSE;
	}

	/** Returns VTRUE if the mouse is captured by this window, VFALSE
	otherwise. If the mouse capture should be released if owned by this
	window, pass VTRUE for the bReleaseIfOwned parameter. If this windows
	owns the capture, and it is released by bReleaseIfOwned, the return
	value will be VFALSE.*/
	VBOOL					IsMouseCaptured(
		VBOOL bReleaseIfOwned = VFALSE) const
	{
		VBOOL bResult =
			(GetCapture() == m_hWindow) ? VTRUE : VFALSE;

		if ( bResult && bReleaseIfOwned )
		{
			ReleaseCapture();
			bResult = VFALSE;
		}

		return bResult;
	}

	/** Determine if a button, radio button, or check box that is a child of
	this window is checked on. nID is the resource or child window ID of
	the child window to check. Returns VTRUE if the button is checked,
	VFALSE if not.*/
	VBOOL					IsButtonChecked(VUINT nID) const
	{
		VASSERT(GetSafeWindow())

		return	(::IsDlgButtonChecked(m_hWindow, nID) == BST_CHECKED)
				? VTRUE : VFALSE;
	}

	/** Determine whether the window is minimized (iconic). Returns VTRUE
	if it is minimized, VFALSE if not.*/
	VBOOL					IsIconic() const
	{
		VASSERT(GetSafeWindow())
		return (::IsIconic(m_hWindow)) ? VTRUE : VFALSE;
	}

	/** Returns VTRUE if this object (or derived object) is a VDialog type.
	Obviously this base class is not a dialog box, so it returns VFALSE.
	Dialog boxes should return VTRUE.*/
	virtual VBOOL			IsVDialogType()
		{ return VFALSE; }

	/** Returns VTRUE if the window is visible, VFALSE otherwise. This does
	not mean the window is actually visible to the user, it could still be
	obscured by other windows and applications. To be visible, this window
	and its parent (if applicable) must have the WS_VISIBLE bit set. If you
	wish to only check to see if this window "would" be visible (has the
	WS_VISIBLE bit set), use IsVisibleEx() instead.*/
	VBOOL					IsVisible() const
	{
		VASSERT(GetSafeWindow())
		return (IsWindowVisible(m_hWindow)) ? VTRUE : VFALSE;
	}

	/** Returns VTRUE if this window would be visible, that is, has the
	WS_VISIBLE style bit set. It may not actually be visible to the user,
	since the parent window may not be visible, or the window is obscured
	by another window or application.*/
	VBOOL					IsVisibleEx() const
		{ return (Style() & WS_VISIBLE) ? VTRUE : VFALSE; }

	/** Returns VTRUE if this object (or derived object) is a VMDIChildWindow
	type. Obviously this base class is not a MDI child window, so it returns
	VFALSE. MDI child windows should return VTRUE.*/
	virtual VBOOL			IsVMDIChildWindowType()
		{ return VFALSE; }

	/** Determine if this VWindow object has a Windows window (HWND)
	associated with it. Returns VTRUE if it does, VFALSE if not. This
	function will also return VFALSE if there is an assocated HWND, but
	Windows reports that it is not a valid HWND.*/
	VBOOL					IsWindow() const
	{
		return	(m_hWindow)
				? (::IsWindow(m_hWindow))
				? VTRUE
				: VFALSE
				: VFALSE;
	}

	/** Retrieves information about the window. The function also retrieves
	the 32-bit (long) value at the specified offset into the extra window
	memory of a window. See the SDK documentation for GetWindowLong() for
	more information on possible values that can be passed for nIndex.*/
	VLONG					Long(VINT nIndex) const
	{
		VASSERT(GetSafeWindow())
		return GetWindowLong(m_hWindow, nIndex);
	}

	/** Changes an attribute of the window. The function also sets a 32-bit
	(long) value at the specified offset into the extra window memory of a
	window. See the SDK documentation for SetWindowLong() for more
	information on possible values that can be passed for nIndex. The
	returned VLONG is the previous long value that was set at nIndex.*/
	VLONG					Long(	VINT	nIndex,
									VLONG	nNewLong) const
	{
		VASSERT(GetSafeWindow())
		return SetWindowLong(m_hWindow, nIndex, nNewLong);
	}

	/** Return a reference to the main window object set with
	MainWindow(window).*/
	static VWindow&			MainWindow();

	/** Set a reference to the main window.*/
	static void				MainWindow(VWindow& mainWindow);

	/** Maximize this window to consume entire available screen real estate.*/
	virtual void			Maximize()
		{ Show(SW_SHOWMAXIMIZED); }

	/** Minimizes the window and activates the next top-level window in
	the Z order.*/
	virtual void			Minimize()
		{ Show(SW_SHOWMINIMIZED); }

	/** Retrieves the handle to the menu assigned to the window.*/
	HMENU					Menu() const
	{
		VASSERT(GetSafeWindow())
		return GetMenu(m_hWindow);
	}

	/** Assigns a new menu to the specified window. If the hMenu parameter
	is NULL, the window's current menu is removed. The window is redrawn
	to reflect the menu change. The Menu() function replaces the previous
	menu, if any, but it does not destroy it. An application should call
	the DestroyMenu() function to accomplish this task.*/
	void					Menu(HMENU hMenu) const
	{
		VASSERT(GetSafeWindow())
		SetMenu(m_hWindow, hMenu);
	}

	/** Show a message box owned by this window, which displays pszText to
	the user with the caption of pszCaption. See the Windows SDK for
	possible values for nType.*/
	VINT					MessageBox(
		VSTRING_CONST	pszText,
		VSTRING_CONST	pszCaption,
		VUINT			nType =
							MB_ICONINFORMATION | MB_OK) const
	{
		VINT nResult;
		HWND hWndFocus = GetFocus();

		nResult = ::MessageBox(m_hWindow, pszText, pszCaption, nType);

		if ( hWndFocus )
			::SetFocus(hWndFocus);

		return nResult;
	}

	/** Same as above but uses the application name as the message box
	caption.*/
	VINT					MessageBox(
		VSTRING_CONST	pszText,
		VUINT			nType =
							MB_ICONINFORMATION | MB_OK) const
		{ return MessageBox(pszText, VGetAppTitle(), nType); }

	/** Changes the position and dimensions of the window. For a top-level
	window, the position and dimensions are relative to the upper-left
	corner of the screen. For a child window, they are relative to the
	upper-left corner of the parent window's client area. bRedraw specifies
	whether the window is to be repainted. If this parameter is VTRUE, the
	window receives a WM_PAINT message. If the parameter is VFALSE, no
	repainting of any kind occurs. This applies to the client area, the
	non-client area (including the title bar and scroll bars), and any part
	of the parent window uncovered as a result of moving a child window. If
	this parameter is VFALSE, the application must explicitly invalidate or
	redraw any parts of the window and parent window that need redrawing.*/
	void					Move(	VINT	x,
									VINT	y,
									VINT	cx,
									VINT	cy,
									VBOOL	bRedraw =
												VTRUE) const
	{
		VASSERT(GetSafeWindow())
		MoveWindow(m_hWindow, x, y, cx, cy, bRedraw);
	}

	/** Same as above, but moves the window to the left and top position of
	the r VRect parameter, and makes the window the width of the r, and the
	makes the window the height of the r.*/
	void					Move(	VRect const&	r,
									VBOOL			bRedraw =
														VTRUE) const
	{
		VASSERT(GetSafeWindow())
		MoveWindow(m_hWindow, VRECT_BREAK_WIDTH_HEIGHT(r), bRedraw);
	}

	/** Places (posts) a message in the message queue associated with the
	thread that created this window and then returns without waiting for
	the thread to process the message.*/
	void					PostMessage(	VUINT	nMessage,
											VWPARAM wParam,
											VLPARAM lParam) const
	{
		VASSERT(GetSafeWindow())
		::PostMessage(m_hWindow, nMessage, wParam, lParam);
	}

	/** Retrieves a handle to the parent window. If the function succeeds,
	the return value is a handle to the parent window. If the window has no
	parent window, the return value is NULL.*/
	HWND					Parent() const
	{
		VASSERT(GetSafeWindow())
		return GetParent(m_hWindow);
	}

	/** Changes the parent window. hWndParent is a handle to the new parent
	window. If this parameter is NULL, the desktop window becomes the new
	parent window. Windows NT 5.0 and later: If this parameter is
	HWND_MESSAGE, the child window becomes a message-only window. If the
	function succeeds, the return value is a handle to the previous parent
	window. If the function fails, the return value is NULL.*/
	HWND					Parent(HWND hWndParent) const
	{
		VASSERT(GetSafeWindow())
		return SetParent(m_hWindow, hWndParent);
	}

	/** Override to allow user to call IsDialogMessage() or perform other
	processing before the message is dispatched. Return VTRUE if message
	was processed, VFALSE if not.*/
	virtual VBOOL			PreTranslateMessage(MSG const& msg)
		{ return VFALSE; }

	/** Releases the mouse capture from a window in the current thread and
	restores normal mouse input processing. A window that has captured the
	mouse receives all mouse input, regardless of the position of the
	cursor, except when a mouse button is clicked while the cursor hot spot
	is in the window of another thread.*/
	void					ReleaseCapture() const
	{
		VASSERT(GetSafeWindow())
		::ReleaseCapture();
	}

	/** Activates and displays the window. If the window is minimized or
	maximized, the system restores it to its original size and position. An
	application should specify call this function when restoring a minimized
	window.*/
	virtual void			Restore()
		{ Show(SW_RESTORE); }

	/** Converts the screen coordinates of a specified point on the screen
	to client coordinates of this window.*/
	void					ScreenToClient(VPoint& pt) const
	{
		VASSERT(GetSafeWindow())
		::ScreenToClient(m_hWindow, pt.GetPointer());
	}

	/** Send a message to the specified child window or control. nID is the
	resource or child window ID of the child window to send the message to.*/
	VLONG					SendDialogItemMessage(	VUINT	nID,
													VUINT	nMessage,
													VWPARAM	wParam =
																0,
													VLPARAM	lParam =
																0) const
	{
		VASSERT(GetSafeWindow())
		return SendDlgItemMessage(m_hWindow, nID, nMessage, wParam, lParam);
	}

	/** Same as above, but returns VTRUE or VFALSE for messages that return
	a boolean result.*/
	VBOOL					SendDialogItemMessageBOOL(	VUINT	nID,
														VUINT	nMessage,
														VWPARAM	wParam =
																	0,
														VLPARAM	lParam =
																	0) const
	{
		return (SendDlgItemMessage(	m_hWindow,
									nID,
									nMessage,
									wParam,
									lParam))
									? VTRUE : VFALSE;
	}

	/** Sends the specified message to this window. The function calls the
	window procedure for the specified window and does not return until the
	window procedure has processed the message. The PostMessage() function,
	in contrast, posts a message to a thread's message queue and returns
	immediately. The return value specifies the result of the message
	processing and depends on the message sent. If the window was created
	by the calling thread, the window procedure is called immediately as a
	subroutine. If the specified window was created by a different thread,
	the system switches to that thread and calls the appropriate window
	procedure. Messages sent between threads are processed only when the
	receiving thread executes message retrieval code. The sending thread is
	blocked until the receiving thread processes the message.*/
	VLONG					SendMessage(	VUINT	nMessage,
											VWPARAM	wParam =
														0,
											VLPARAM	lParam =
														0) const
	{
		VASSERT(GetSafeWindow())
		return (VLONG)::SendMessage(m_hWindow, nMessage, wParam, lParam);
	}

	/** Same as above, but returns VTRUE or VFALSE for messages that
	return a boolean result.*/
	VBOOL					SendMessageBOOL(	VUINT	nMessage,
												VWPARAM	wParam =
															0,
												VLPARAM	lParam =
															0) const
		{ return (SendMessage(nMessage, wParam, lParam)) ? VTRUE : VFALSE; }

	/** Sets the mouse capture to this window. Once a window has captured the
	mouse, all mouse input is directed to that window, regardless of whether
	the cursor is within the borders of that window. Only one window at a
	time can capture the mouse. If the mouse cursor is over a window created
	by another thread, the system will direct mouse input to the specified
	window only if a mouse button is down. The return value is the handle of
	the window that had previously captured the mouse. If there is no such
	window, the return value is NULL. Only the foreground window can capture
	the mouse. When a background window attempts to do so, the window
	receives messages only for mouse events that occur when the cursor hot
	spot is within the visible portion of the window. Also, even if the
	foreground window has captured the mouse, the user can still click
	another window, bringing it to the foreground. When the window no longer
	requires all mouse input, the thread that created the window should call
	the ReleaseCapture() function to release the mouse. This function cannot
	be used to capture mouse input meant for another process. Calling
	SetCapture() on Windows 95 causes the window that is losing the mouse
	capture to receive a WM_CAPTURECHANGED message.*/
	HWND					SetCapture() const
	{
		VASSERT(GetSafeWindow())
		return ::SetCapture(m_hWindow);
	}

	/** Sets the text of a control in a dialog box or child window to the
	string pszText. nID is the resource or child window ID of the child
	window to set the text into.*/
	void					SetDialogItemText(	VUINT			nID,
												VSTRING_CONST	pszText) const
	{
		VASSERT(GetSafeWindow())
		SetDlgItemText(m_hWindow, nID, pszText);
	}

	/** Some dialog boxes and child windows require a message result to be
	placed in their window data. VWCL does not normally do this, but since
	it is required for some messages, this function can be used to do it.*/
	void					SetDialogResult(VLONG nResult) const
		{ Long(DWL_MSGRESULT, nResult); }

	/** Sets the keyboard focus to this window. The window must be associated
	with the calling thread's message queue. If the function succeeds, the
	return value is the handle to the window that previously had the
	keyboard focus. If the window is not valid or associated with the
	calling thread's message queue, the return value is NULL. The SetFocus()
	function sends a WM_KILLFOCUS message to the window that loses the
	keyboard focus and a WM_SETFOCUS message to the window that receives the
	keyboard focus. It also activates either the window that receives the
	focus or the parent of the window that receives the focus. If a window
	is active but does not have the focus, any key pressed will produce the
	WM_SYSCHAR, WM_SYSKEYDOWN, or WM_SYSKEYUP message. If the VK_MENU key
	is also pressed, the lParam parameter of the message will have bit 30
	set. Otherwise, the messages produced do not have this bit set. By using
	the AttachThreadInput() function, a thread can attach its input
	processing to another thread. This allows a thread to call SetFocus() to
	set the keyboard focus to a window associated with another thread's
	message queue. On Windows CE, do not use the SetFocus() function to set
	the keyboard focus to a window associated with another thread's message
	queue. There is one exception. If a window in one thread is the child of
	the window on the other thread, or if the windows are siblings of the
	same parent, the thread associated with one window can set the focus to
	the other window even though it belongs to a different thread. In this
	case, there is no need to call AttachThreadInput() first.*/
	HWND					SetFocus() const
	{
		VASSERT(GetSafeWindow())
		return ::SetFocus(m_hWindow);
	}

	/** Set the internal reference to the main window object to NULL.*/
	static void				SetMainWindowNULL();

	/** Changes the text of the window's title bar (if it has one). If this
	window is a control, the text of the control is changed.*/
	void					SetText(VSTRING_CONST pszText) const
	{
		VASSERT(GetSafeWindow())
		::SetWindowText(m_hWindow, pszText);
	}

	/** Sets the window's show state. nCmdShow specifies how the window is to
	be shown. This parameter is ignored the first time an application calls
	Show(), if the program that launched the application provides a
	STARTUPINFO structure. Otherwise, the first time Show() is called, the
	value should be the value obtained by the WinMain() function in its
	nCmdShow parameter, which is stored by VWCL in the VApplication object
	and can be retrieved using VGetCommandShow() which is equivalent to the
	nCmdShow passed to WinMain. See the Win32 SDK documentation for
	ShowWindow() to learn more information about possible values for the
	nCmdShow parameter. This class also provides the Hide(), Maximize(),
	Minimize(), and Restore() functions that are useful, easier to
	understand, interfaces to this function.*/
	void					Show(VINT nCmdShow = SW_SHOWNORMAL) const
	{
		VASSERT(GetSafeWindow())
		ShowWindow(m_hWindow, nCmdShow);
	}

	/** Show or hide all child windows.*/
	void					ShowChildren(VBOOL bShow = VTRUE) const;

	/** Retrieve the WS_xxx style bits for this window.*/
	VLONG					Style() const
		{ return Long(GWL_STYLE); }

	/** Set the WS_xxx style bits for this window. The return value is the
	previous window style bits.*/
	VLONG					Style(VUINT nStyle) const
		{ return Long(GWL_STYLE, nStyle); }

	/** Append the WS_xxx style bits for this window. The return value is
	the previous window style bits.*/
	VLONG					StyleAppend(VUINT nStyle) const
		{ return Style(Style() | nStyle); }

	/** Remove the WS_xxx style bits for this window. The return value is
	the previous window style bits.*/
	VLONG					StyleRemove(VUINT nStyle) const
	{
		VLONG nNewStyle = Style();
		nNewStyle &= ~nStyle;

		return Style(nNewStyle);
	}

	/** Retrieve the WS_EX_xxx extended style bits for this window.*/
	VLONG					StyleEx() const
		{ return Long(GWL_EXSTYLE); }

	/** Set the WS_EX_xxx extended style bits for this window. The return
	value is the previous window ex-style bits.*/
	VLONG					StyleEx(VUINT nStyle) const
		{ return Long(GWL_EXSTYLE, nStyle); }

	/** Append the WS_EX_xxx extended style bits for this window. The
	return value is the previous window ex-style bits.*/
	VLONG					StyleAppendEx(VUINT nStyle) const
		{ return StyleEx(StyleEx() | nStyle); }

	/** Remove the WS_EX_xxx extended style bits for this window. The
	return value is the previous window ex-style bits.*/
	VLONG					StyleRemoveEx(VUINT nStyle) const
	{
		VLONG nNewStyle = StyleEx();
		nNewStyle &= ~nStyle;

		return StyleEx(nNewStyle);
	}

	/** Updates the client area of the window by sending a WM_PAINT message
	to the window if the window's update region is not empty. The function
	sends a WM_PAINT message directly to the window procedure of the
	specified window, bypassing the application queue. If the update region
	is empty, no message is sent.*/
	void					Update() const
	{
		VASSERT(GetSafeWindow())
		UpdateWindow(m_hWindow);
	}

	/** Window procedure. Override to perform message processing not
	encapsulated by VWindow or derived classes. The return value is the
	result of the message processing and depends on the message sent. Every
	message received by a window must be handled. If not handled by an
	override of this class, a derived implementation should call this
	function and return the value to the caller. The base class
	implementation calls the Windows provided default handler for messages,
	or if this object subclasses another window, it will call that windows'
	window procedure.*/
	virtual VLONG			WindowProc(	HWND	hWnd,
										VUINT	nMessage,
										VWPARAM	wParam,
										VLPARAM	lParam);

	/** Shared window procedure used by all VWindow and derived, and
	subclassed, window objects. This function resolves the VWindow pointer
	and calls the virtual WindowProc() function. It also verifies VWCL
	window management integrity and performs other needed housekeeping
	chores.*/
	static VLONG CALLBACK	WindowProcShared(	HWND	hWnd,
												VUINT	nMessage,
												VWPARAM	wParam,
												VLPARAM	lParam);

protected:
friend class VApplication;

	/** Handler for WM_CLOSE. The default implementation does nothing, but
	causes Windows to destroy the window. Return 0 if the message was
	handled, VWINDOW_MESSAGE_NOT_HANDLED to have Windows perform the normal
	processing. If your override of this function causes the window to be
	destroyed, you should return 0.*/
	virtual VLONG			OnClose()
		{ return VWINDOW_MESSAGE_NOT_HANDLED; }

	/** Handler for WM_COMMAND, which is sent when the user selects a
	command item from a menu, when a control sends a notification message
	to its parent window, or when an accelerator keystroke is translated.
	nNotifyCode specifies the notification code if the message is from a
	control. If the message is from an accelerator, this parameter is 1. If
	the message is from a menu, this parameter is 0. nCommandID specifies
	the identifier of the menu item, control, or accelerator. hWndControl
	is the handle to the control sending the message if the message is from
	a control. Otherwise, this parameter is NULL. Return 0 to indicate that
	you have handled this message, or VWINDOW_MESSAGE_NOT_HANDLED to have
	Windows do the default processing.*/
	virtual VLONG			OnCommand(	VUINT	nNotifyCode,
										VUINT	nCommandID,
										HWND	hWndControl)
		{ return VWINDOW_MESSAGE_NOT_HANDLED; }

	/** Handler for WM_DESTROY. The WM_DESTROY message is sent when a window
	is being destroyed. It is sent to the window procedure of the window
	being destroyed after the window is removed from the screen. This
	message is sent first to the window being destroyed and then to the
	child windows (if any) as they are destroyed. During the processing of
	the message, it can be assumed that all child windows still exist. If
	the window being destroyed is part of the clipboard viewer chain (set
	by calling the SetClipboardViewer() function), the window must remove
	itself from the chain by processing the ChangeClipboardChain() function
	before returning from the WM_DESTROY message. This default
	implementation does nothing.*/
	virtual void			OnDestroy()
		{;}

	/** Handler for WM_PAINT. Will only be called with a valid VDC object,
	so there is no need to check dc.GetHandle(). Return VTRUE if VWCL
	should continue routing the message to Windows so that subclassed
	windows can perform their own painting. If VFALSE is returned, it is
	assumed a derived class override has performed all required painting. It
	is perfectly valid for a derived function to call either base class(es)
	functions and/or WindowProc() to have base code do its drawing before,
	after, or mixed in with, derived class painting.
	THIS FUNCTION WILL ONLY BE CALLED IF THE OPTION BIT OPTION_NEED_ON_PAINT
	OPTION BIT IS SET! In addition to only being called with a valid device
	context in the dc parameter, this method will never be called if the
	client rectangle of the window is empty.*/
	virtual VBOOL			OnPaint(VDC& dc)
		{ return VTRUE; }

	/** Handler for WM_SIZE. Return 0 if message was handled,
	VWINDOW_MESSAGE_NOT_HANDLED otherwise. nFlags specifies the type of
	resizing requested. This parameter can be one of the following values:

	SIZE_MAXHIDE - Message is sent to all pop-up windows when some
	other window is maximized.

	SIZE_MAXIMIZED - The window has been maximized.

	SIZE_MAXSHOW - Message is sent to all pop-up windows when some
	other window has been restored to its former size.

	SIZE_MINIMIZED - The window has been minimized.

	SIZE_RESTORED- The window has been resized, but neither the
	SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.*/
	virtual VLONG			OnSize(	VWPARAM	nFlags,
									VINT	ncx,
									VINT	ncy)
		{ return VWINDOW_MESSAGE_NOT_HANDLED; }

	/** Handler for WM_NOTIFY. Return 0 if message was handled,
	VWINDOW_MESSAGE_NOT_HANDLED otherwise.*/
	virtual VLONG			OnNotify(	VUINT	nIDControl,
										NMHDR&	nmhdr)
		{ return VWINDOW_MESSAGE_NOT_HANDLED; }

	/** Support for reflected WM_NOTIFY messages. Derived class must return
	0 if message was handled, -1 if handled and parent should NOT be
	notified, or 1 if message was not handled and parent should be
	notified. If -1 if returned, derived classes must also set the
	lCommonControlResult to the return value expected by the
	common control.*/
	virtual VLONG			OnReflectedNotify(
		NMHDR&		nmhdr,
		VLPARAM&	lCommonControlResult)
		{ return VWINDOW_MESSAGE_NOT_HANDLED; }

	/** Called after WM_NCDESTROY completes. Safe to do a "delete this" on
	this object if it was dynamically allocated. The base class function
	will use "delete this" only if the OPTION_SELF_DELETE option bit is set.*/
	virtual void			PostDestroy()
	{
		if ( m_WindowOptions.IsSet(OPTION_SELF_DELETE) )
			VDELETE this;
	}

	/** Override to change the window creation parameters before a window
	is created.*/
	virtual VBOOL			PreCreateWindow(CREATESTRUCT& CreateStruct)
		{ return TRUE; }

	/** Called after Create() or Attach() has successfully created or
	subclassed a Windows window and associated the window handle with the
	VWindow object. This is the best function to override in derived
	classes to perform window initialization after creation or attachment.
	If an override returns VFALSE, the window will be destroyed or detached.
	This function is called for all VWindow objects, including dialog boxes
	and subclassed controls. The base class implementation verifies that
	correct clipping window style bits are used, so all overrides should
	eventually call this base class implementation.*/
	virtual VBOOL			SetupWindow();

	/** Embedded Members.*/

	/** Runtime Type information.*/
	VRTTI					m_RTTI;

	/** Window options.*/
	VBitmask				m_WindowOptions;

	/** This is the ID of this control specified when created.*/
	VUINT					m_nID;

	/** Handle to the Windows window this object encapsulates.*/
	HWND					m_hWindow;

	/** For windows that process message loops, the accelerators can
	be stored here.*/
	HACCEL					m_hAccel;

	/** If VWCL is managing a non-VWindow window, this will point to
	the subclassed window procedure.*/
	WNDPROC					m_pfnOldWndProc;

	/** Used only when message filter objects are installed
	into this window.*/
	VLinkedListManager*		m_pInstalledFilters;
};

#endif	/* VWINDOW*/
