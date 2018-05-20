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

#ifndef VSPLITSUPPORT
#define VSPLITSUPPORT

#include "../vstandard.h"
#include "../gdi/vrecttracker.hpp"
#include "../gdi/vdc.hpp"
#include "../numbers/vbitmask.hpp"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"

/** Macros that can be used in classes that implement VSplitSupport.*/

/** Override repositions child windows. This function always set
VSplitSupport::Rect to 0, 0, cx, cy so this will have to be overridden
if anything other than these settings are required, or you can use the
option OPTION_NO_WM_SIZE) to skip this code.*/
#define VWCL_SPLIT_ON_SIZE(base_class_name)							\
virtual VLONG OnSize(VWPARAM nFlags, VINT ncx, VINT ncy)			\
{																	\
	if ( VSplitSupport::m_Options.IsNotSet(OPTION_NO_WM_SIZE) )		\
	{																\
		SetClientRectangle(0, 0, ncx, ncy);							\
		RepositionChildren(VREF_THIS);								\
	}																\
	return base_class_name::OnSize(nFlags, ncx, ncy);				\
}

/** Override handles mouse related messages.*/
#define VWCL_SPLIT_WINDOW_PROC(base_class_name)								\
virtual VLONG WindowProc(	HWND hWnd,										\
							VUINT nMessage,									\
							VWPARAM wParam,									\
							VLPARAM lParam)									\
{																			\
	VPoint pt(LOWORD(lParam), HIWORD(lParam));								\
	if ( nMessage == WM_LBUTTONDOWN && HandleOnLButtonDown(VREF_THIS, pt) )	\
		return 0;															\
	else if ( nMessage == WM_MOUSEMOVE && HandleOnMouseMove(VREF_THIS, pt) )\
		return 0;															\
	return base_class_name::WindowProc(hWnd, nMessage, wParam, lParam);		\
}

/** VSplitSupport is the base class for all split window classes and
provides the majority of split logic. It is important to note that this class
is NOT a window object, but is designed to interface with them. This allows
any window to become a splitter window. A split window is defined as any
window that has up to 2 child windows that are separated by a splitter area.
To create a window that has support for splitting, derive a class from a
VWindow base class and this class. Use the above macro's to send messages the
class needs from the derived code. See the various VWCL window classes that do
this for more information. This class supports vertical splits and horizontal
splits. A vertical split refers to the direction of the splitter area. This
would be top to bottom, with 2 child windows horizontal to each other, left
and right side. A horizontal split is just the opposite, with the split area
running horizontally, and the child windows arranged one on top of the
other.*/
class VSplitSupport
{
public:
	/** Default constructor initializes options, which can also be set
	with GetOptions().*/
	VSplitSupport(VUINT nOptions = OPTION_DEFAULT)
	{
		m_Options =				nOptions;
		m_pRectTracker =		NULL;
		m_hWndChildPane1 =		NULL;
		m_hWndChildPane2 =		NULL;
		m_nSplitPos =			-1;
		m_nMinSplit =			DEFAULT_MIN_SPLIT;
		m_nSplitterWidth =		DEFAULT_SPLITTER_WIDTH;
	}

	/** Virtual destructor deletes internally allocated objects.*/
	virtual ~VSplitSupport()
	{
		VDELETE_NULL(m_pRectTracker)
	}

	/** Local constants.*/
	enum
		{	SPLIT_NONE,
			SPLIT_HORIZONTAL,
			SPLIT_VERTICAL,
			DEFAULT_SPLITTER_WIDTH =	4,
			DEFAULT_MIN_SPLIT =			20,
		};

	/** Options that can be set during construction or with
	GetOptions(). Defaults to OPTIONS_DEFAULT.*/
	enum
		{	OPTION_NO_WM_SIZE	=					0x0001,
			OPTION_CAPTURE_MOUSE_OVER_SPLITTER =	0x0002,
			OPTION_ENABLE_USER_SPLIT =				0x0004,
			OPTION_SPLIT_VERTICAL =					0x0008,
			OPTION_SPLIT_HORIZONTAL =				0x0010,
			OPTION_DEFAULT =
				OPTION_CAPTURE_MOUSE_OVER_SPLITTER |
				OPTION_ENABLE_USER_SPLIT |
				OPTION_SPLIT_VERTICAL,
		};

	/** Add a split to this window. nSplitStyle must be either
	SPLIT_HORIZONTAL or SPLIT_VERTICAL. nInitPosition is the X or Y
	position on the screen where the area between the child windows
	is displayed. Handles to children are translated as follows:

	If a vertical split is done, m_hWndChildPane1 is the left pane and
	m_hWndChildPane2 is the right pane.

	If a horizontal split is done, m_hWndChildPane1 is the top pane and
	m_hWndChildPane2 is the bottom pane.

	This function sets the internal client rectangle object (m_rClient)
	to the client coordinates of window. This will be skipped if you have
	already initialized the rectangle with a call to GetRect(). This
	function also calls the virtual function AllocRectTracker(), which can
	be used to create a custom rectangle tracker object used for splitter
	movement.*/
	void			AddSplit(	VWindow const&	window,
								VBYTE			nSplitStyle =
													SPLIT_VERTICAL,
								VINT			nInitPosition =
													20,
								HWND			hWndChildPane1 =
													NULL,
								HWND			hWndChildPane2 =
													NULL)
	{
		/* Make sure required parameters are valid.*/
		VASSERT(window.GetSafeWindow())

		/* Allocate tracker as needed.*/
		if ( !m_pRectTracker )
			m_pRectTracker = AllocRectTracker();

		/* Get default client rectangle (real client area), unless
		m_rClient has already been set.*/
		if ( m_rClient.IsEmpty() )
			window.GetClientRect(m_rClient);

		if ( nSplitStyle == SPLIT_HORIZONTAL )
			m_Options.AppendExclusive(	OPTION_SPLIT_HORIZONTAL,
										OPTION_SPLIT_VERTICAL);
		else
			m_Options.AppendExclusive(	OPTION_SPLIT_VERTICAL,
										OPTION_SPLIT_HORIZONTAL);

		/* Make variable assignments.*/
		m_nSplitPos =		nInitPosition;
		m_hWndChildPane1 =	hWndChildPane1;
		m_hWndChildPane2 =	hWndChildPane2;

		/* Update UI now.*/
		RepositionChildren(window);
	}

	/** Return a reference to the child window at nPane. nPane must
	be either 1 or 2.*/
	HWND&			GetChildWindow(VBYTE nPane) const
	{
		VASSERT(nPane == 1 || nPane == 2)
		return (HWND&)((nPane == 1) ? m_hWndChildPane1 : m_hWndChildPane2);
	}

	/** Get the position of child windows. Derived classes should override
	this function to adjust the rectangles or offsets as needed. This base
	class function calculates the position based on the known client
	rectangle, which is held in m_rClient, the splitter width (defaults to
	DEFAULT_SPLITTER_WIDTH pixels, but can be changed using the
	GetSplitterWidth() method), and the current split position. In most cases,
	overrides of this function will call this function first, and then adjust
	the rectangle passed by reference after this base class function performs
	the initial calculations.*/
	virtual void	GetChildWindowRect(	VBYTE	nPane,
										VRect&	rWindow)
	{
		VASSERT(nPane == 1 || nPane == 2)

		VBOOL bSplitVertical = m_Options.IsSet(OPTION_SPLIT_VERTICAL);

		if ( nPane == 1 )
		{
			rWindow.Set(	m_rClient.GetLeft(),
							m_rClient.GetTop(),
							(bSplitVertical)
							? m_nSplitPos
							: m_rClient.GetRight(),
							(bSplitVertical)
							? m_rClient.GetBottom()
							: m_nSplitPos);

			/* Verify not outside of available rectangle.*/
			if ( bSplitVertical )
			{
				if ( rWindow.GetRight() > m_rClient.GetRight() )
					rWindow.GetRight() = m_rClient.GetRight();
			}
			else if ( rWindow.GetBottom() > m_rClient.GetBottom() )
				rWindow.GetBottom() = m_rClient.GetBottom();
		}
		else
		{
			rWindow.Set(	(bSplitVertical)
							? m_nSplitPos + m_nSplitterWidth
							: m_rClient.GetLeft(),
							(bSplitVertical)
							? m_rClient.GetTop()
							: m_nSplitPos + m_nSplitterWidth,
							m_rClient.GetRight(),
							m_rClient.GetBottom());

			/* Verify not outside of available rectangle.*/
			if ( bSplitVertical )
			{
				if ( rWindow.GetLeft() > m_rClient.GetRight() )
					rWindow.GetLeft() = m_rClient.GetRight();
			}
			else if ( rWindow.GetTop() < m_rClient.GetTop() )
				rWindow.GetTop() = m_rClient.GetTop();
		}
	}

	/** Return a reference the the rectangle this object uses which
	describes the client rectangle of the host window. Since this object
	is NOT a window itself, it must be told how large to draw its contents.
	This is a very powerful and deliberate feature of this class. In most
	cases, this rectangle is the entire client area of the host window.
	However, by using this function which returns the VRect reference,
	it can be adjusted to any size. It is important also to note that the
	preferred way of setting this rectangle to specified coordinates is
	not through modifying the VRect object directly, but by using the
	SetClientRectangle() function of this class.*/
	VRect&			GetClientRectangle() const
		{ return (VRect&)m_rClient; }

	/** Return a cursor to use for a horizontal split. The base class
	returns the standard North-South icon.*/
	virtual HCURSOR	GetCursorHorizontal()
		{ return LoadCursor(NULL, IDC_SIZENS); }

	/** Return a cursor to use for a vertical split. The base class
	returns the standard West-East icon.*/
	virtual HCURSOR	GetCursorVertical()
		{ return LoadCursor(NULL, IDC_SIZEWE); }

	/** Return a reference to the number of pixels from the edges of
	m_rClient to allow the user to move the splitter. This defaults to
	DEFAULT_MIN_SPLIT, which is 20 pixels. This means that if the left
	edge of m_rClient is the left edge of the host window, the user would
	not be allowed to move the splitter closer than 20 pixels to the right
	of the left edge. Same for all other edges.*/
	VUSHORT&		GetMinSplit() const
		{ return (VUSHORT&)m_nMinSplit; }

	/** Get which splitter the mouse is over, if any. Returns SPLIT_VERTICAL,
	SPLIT_HORIZONTAL, or SPLIT_NONE. This function also returns the
	splitter rectangle in rSplitter on output. If the user is not
	currently allowed to split the window (options bit
	OPTION_ENABLE_USER_SPLIT not set), this function will return
	SPLIT_NONE.*/
	VBYTE			GetMouseOverSplitter(	VPoint const&	pt,
											VRect&			rSplitter)
	{
		rSplitter.Empty();

		/* Is user allowed to split the window?*/
		if ( m_Options.IsSet(OPTION_ENABLE_USER_SPLIT) )
		{
			GetSplitterRect(rSplitter);

			if ( rSplitter.IsPointInside(pt) )
				return	(m_Options.IsSet(OPTION_SPLIT_VERTICAL))
						? SPLIT_VERTICAL
						: SPLIT_HORIZONTAL;
		}

		return SPLIT_NONE;
	}

	/** Return a pointer to the VRectTracker object this class uses when the
	user is splitting the windows. This object is allocated with a call to
	AllocRectTracker() during AddSplit(), and can be customized at that
	point, or by using the pointer returned here.*/
	VRectTracker*	GetRectTracker() const
		{ return (VRectTracker*)m_pRectTracker; }

	/** Return a reference to the current split position. If this value is
	then modified, call RepositionChildren() to have the change displayed
	to the user.*/
	VINT&			GetSplitPosition() const
		{ return (VINT&)m_nSplitPos; }

	/** Get the rect of the splitter (the area between the child windows).
	Overrides of this function normally call this base class function first
	and then modify the resulting splitter rectangle placed in rSplitter.*/
	virtual void	GetSplitterRect(VRect& rSplitter)
	{
		GetChildWindowRect(1, rSplitter);

		if ( m_Options.IsSet(OPTION_SPLIT_VERTICAL) )
		{
			rSplitter.Set(	rSplitter.GetRight(),
							m_rClient.GetTop() + 1,
							rSplitter.GetRight() + m_nSplitterWidth,
							m_rClient.GetBottom() - 1);
		}
		else
		{
			rSplitter.Set(	m_rClient.GetLeft() + 1,
							rSplitter.GetBottom(),
							m_rClient.GetRight() - 1,
							rSplitter.GetBottom() + m_nSplitterWidth);
		}
	}

	/** Return a reference to the splitter width. This defaults to
	DEFAULT_SPLITTER_WIDTH pixels during construction, but can be set at
	any time using this method. After changing this value, you should call
	RepositionChildren() to update the screen.*/
	VUINT&			GetSplitterWidth() const
		{ return (VUINT&)m_nSplitterWidth; }

	/** Returns VTRUE if the splitter orientation is SPLIT_VERTICAL,
	VFALSE otherwise.*/
	VBOOL			IsSplitterVertical() const
		{ return m_Options.IsSet(OPTION_SPLIT_VERTICAL); }

	/** Returns VTRUE if the splitter orientation is SPLIT_HORIZONTAL,
	VFALSE otherwise.*/
	VBOOL			IsSplitterHorizontal() const
		{ return !IsSplitterVertical(); }

	/** Call to start moving the splitter using the keyboard.*/
	void			KeyboardSplit(VWindow const& window)
	{
		VASSERT(window.GetSafeWindow())

		VPoint pt;

		if ( m_Options.IsSet(OPTION_SPLIT_VERTICAL) )
			pt.Set(m_nSplitPos, m_rClient.GetHalfHeight());
		else
			pt.Set(m_rClient.GetHalfWidth(), m_nSplitPos);

		/* Do the split now.*/
		DoSplit(window, pt);
	}

	/** Move the child windows around the splits. This should be called
	in WM_SIZE or OnSize() handler. RecalcLayout() is the correct place
	to call this method for VMainWindow derived classes.*/
	virtual void	RepositionChildren(VWindow const& window)
	{
		/* This function could be called before AddSplit() is called,
		so just check for a window, don't assert.*/
		if ( window.GetSafeWindow() )
		{
			for ( VBYTE i = 1; i <= 2; i++ )
			{
				HWND hWnd = GetChildWindow(i);

				if ( hWnd )
				{
					VRect r;
					GetChildWindowRect(i, r);

					MoveWindow(	hWnd,
								r.GetLeft(),
								r.GetTop(),
								1 + r.GetWidth(),
								1 + r.GetHeight(),
								VTRUE);

					InvalidateRect(hWnd, NULL, VTRUE);
					UpdateWindow(hWnd);
				}
			}
		}
	}

	/** This function is normally called from the OnSize() handler or
	WM_SIZE message. It is the preferred way to set the internal m_rClient
	cient rectangle object coordinates. This is a virtual function, and thus
	may be routed to a derived class, which may modify the internal
	rectangle coordinates in a different way. The base class function
	simply sets m_rClient to the coordinates specified as parameters.*/
	virtual void	SetClientRectangle(	VLONG nx,
										VLONG ny,
										VLONG ncx,
										VLONG ncy)
		{ m_rClient.Set(nx, ny, ncx, ncy); }

	/** Toggle the orientation of the split. If a vertical split, make it
	horizontal, and vice-versa.*/
	void			ToggleOrientation(VWindow const& window)
	{
		m_Options.Toggle(OPTION_SPLIT_VERTICAL);
		m_Options.Toggle(OPTION_SPLIT_HORIZONTAL);

		if ( window.GetHandle() )
			RepositionChildren(window);
	}

protected:
	/** Allocate a VRectTracker object used when dragging and moving the
	splitter area. This base class function allocates a standard
	VRectTracker object, but derived code can allocate a custom object as
	desired. This function is called from the AddSplit() function as
	needed.*/
	virtual VRectTracker*	AllocRectTracker()
		{ return VNEW VRectTracker; }

	/** Do the split from the given coordinates in ptFrom, using the
	host window.*/
	virtual void			DoSplit(	VWindow const&	window,
										VPoint&			ptFrom)
	{
		VASSERT(window.GetSafeWindow())
		VASSERT(m_pRectTracker)

		if ( m_pRectTracker )
		{
			VRect	rSplitter;
			VINT	nDragWhatSplit = GetMouseOverSplitter(ptFrom, rSplitter);

			/* This mouse should already be positioned over a splitter.*/
			VASSERT(nDragWhatSplit != SPLIT_NONE)

			/* We will hold the position to set the cursor to here.*/
			VPoint ptCursor(ptFrom);

			/* Center mouse position in the center of the splitter
			and set cursor.*/
			if ( nDragWhatSplit == SPLIT_VERTICAL )
			{
				ptCursor.GetX() =
					rSplitter.GetLeft() + rSplitter.GetHalfWidth();

				if ( GetCursorVertical() )
					SetCursor(GetCursorVertical());
			}
			else
			{
				ptCursor.GetY() =
					rSplitter.GetTop() + rSplitter.GetHalfHeight();

				if ( GetCursorHorizontal() )
					SetCursor(GetCursorHorizontal());
			}

			/* Set the cursor position and set ptFrom to match.*/
			window.ClientToScreen(ptCursor);
			SetCursorPos(ptCursor.GetX(), ptCursor.GetY());
			window.ScreenToClient(ptCursor);
			ptFrom = ptCursor;

			/* Initialize the rectangle tracker rectangle.*/
			m_pRectTracker->GetRect().Set(rSplitter);

			/* Set the min and max range to use and enforce limits.*/
			if ( nDragWhatSplit == SPLIT_VERTICAL )
			{
				m_pRectTracker->GetLimitsRect().
					Set(m_nMinSplit,
						m_rClient.GetTop(),
						m_rClient.GetRight() - m_nMinSplit,
						m_rClient.GetBottom());

				m_pRectTracker->GetOptions() =
					VCapturedMouseTracker::OPTION_RECT_TRACKER_ENFORCE_LIMITS |
					VCapturedMouseTracker::OPTION_RECT_TRACKER_FIX_RECT_TOP |
					VCapturedMouseTracker::OPTION_RECT_TRACKER_FIX_RECT_BOTTOM;
			}
			else
			{
				m_pRectTracker->GetLimitsRect().
					Set(m_rClient.GetLeft(),
						m_nMinSplit + m_rClient.GetTop(),
						m_rClient.GetRight(),
						m_rClient.GetBottom() - m_nMinSplit);

				m_pRectTracker->GetOptions() =
					VCapturedMouseTracker::OPTION_RECT_TRACKER_ENFORCE_LIMITS |
					VCapturedMouseTracker::OPTION_RECT_TRACKER_FIX_RECT_LEFT |
					VCapturedMouseTracker::OPTION_RECT_TRACKER_FIX_RECT_RIGHT;
			}

			/* Start a rect tracker to track the split.*/
			if ( m_pRectTracker->Drag(window, ptFrom) )
			{
				m_nSplitPos =
					(nDragWhatSplit == SPLIT_VERTICAL)
					? m_pRectTracker->GetRect().GetLeft()
					: m_pRectTracker->GetRect().GetTop();

				RepositionChildren(window);
			}
		}

		/* Release the mouse capture if set to this window.*/
		window.IsMouseCaptured(VTRUE);
	}

	/** This function is called in response to a Left Mouse Button Down
	message. Return VTRUE if message was handled. The base class
	implementation looks to see if the mouse is over the splitter rectangle
	area, and if so, starts a user initiated split adjustment. The function
	should return VFALSE if nothing occurred as a result of the mouse
	message.*/
	virtual VBOOL			HandleOnLButtonDown(VWindow const&	window,
												VPoint&			pt)
	{
		VRect r;

		if ( GetMouseOverSplitter(pt, r) != SPLIT_NONE )
		{
			DoSplit(window, pt);
			return VTRUE;
		}

		return VFALSE;
	}

	/** This function is called in response to a mouse movement in the host
	window. Return VTRUE if message was handled. The base class
	implementation looks to see if the mouse is over the splitter rectangle
	area, and if so, captures the mouse to prevent flicker if the
	OPTION_CAPTURE_MOUSE_OVER_SPLITTER option bit is set, and sets the
	cursor to display appropriate feedback to the user, if the cursor
	handles can be retrieved using the GetCursorHorizontal() and
	GetCursorVertical() functions. The function should return VFALSE if
	nothing occurred as a result of the mouse message.*/
	virtual VBOOL			HandleOnMouseMove(	VWindow const&	window,
												VPoint const&	pt)
	{
		VASSERT(window.GetSafeWindow())

		/* No known cursors? No option to capture the mouse?*/
		if ( m_Options.IsSet(OPTION_CAPTURE_MOUSE_OVER_SPLITTER) )
		{
			HCURSOR hVertical =		GetCursorVertical();
			HCURSOR hHorizontal =	GetCursorHorizontal();

			if ( hVertical || hHorizontal )
			{
				/* Check to see if we should be able to drag a splitter.
				If yes, set the cursor to the appropriate cursor.*/
				VRect	rSplitter;
				HCURSOR	hSetCursor =	NULL;
				VBYTE	nOverSplit =	GetMouseOverSplitter(pt, rSplitter);

				/* Set cursor value as needed.*/
				if ( nOverSplit == SPLIT_VERTICAL )
					hSetCursor = hVertical;
				else if ( nOverSplit == SPLIT_HORIZONTAL )
					hSetCursor = hHorizontal;

				/* Are we (still) over a splitter?*/
				if ( nOverSplit != SPLIT_NONE )
				{
					/* Do we have a cursor?*/
					if ( hSetCursor )
					{
						/* Capture mouse if not already done.*/
						if ( !window.IsMouseCaptured() )
							window.SetCapture();

						/* Set cursor if not already done.*/
						if ( GetCursor() != hSetCursor )
							SetCursor(hSetCursor);
					}

					/* Message handled.*/
					return VTRUE;
				}
				/* Release the mouse capture since the point is not
				over the splitter.*/
				else
					window.IsMouseCaptured(VTRUE);
			}
		}

		return VFALSE;
	}

	/** Embedded Members.*/

	/** Our options.*/
	VBitmask		m_Options;

	/** Our client area we work in, not necessarily the entire client
	area of the associated window.*/
	VRect			m_rClient;

	/** The VRectTracker object we use when moving the splitter.*/
	VRectTracker*	m_pRectTracker;

	/** The minimum number of pixels that the user can size a splitter to.
	Rect values will be added to, or subtracted from, m_nSplit to stop
	movement outside of specified limits.*/
	VUSHORT			m_nMinSplit;

	/** Child Pane Windows.*/
	HWND			m_hWndChildPane1;
	HWND			m_hWndChildPane2;

	/** Current X or Y position of split area.*/
	VINT			m_nSplitPos;

	/** The distance between child panes.*/
	VUINT			m_nSplitterWidth;
};

#endif /* VSPLITSUPPORT*/
