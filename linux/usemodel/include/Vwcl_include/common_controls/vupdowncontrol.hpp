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

Primary Author of this source code file:  Karl Edwall (karl@magicnotes.com)
Other Author(s) of this source code file:
*/

#ifndef VUPDOWNCONTROL
#define VUPDOWNCONTROL

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"

/** VUpDownControl encapsulates the Windows updown common control.*/
class VUpDownControl : public VCommonControl
{
public:
	/** Default constructor initializes RTTI information.*/
	VUpDownControl(VRTTI_TYPE nRTTI = VWCL_RTTI_UPDOWN_CONTROL)
		: VCommonControl(nRTTI)
		{;}

	/** Retrieves the current radix base (that is, either base 10 or 16)
	for the up-down control. The return value is the current base value.*/
	VUINT			Base() const
		{ return (VUINT)VWINDOW_SM0(UDM_GETBASE); }

	/** Set the base for a up down control. The base value determines whether
	the buddy window displays numbers in decimal or hexadecimal digits.
	Hexadecimal numbers are always unsigned, decimal numbers are signed.*/
	VUINT			Base(VINT nBase) const
		{ return (VUINT)VWINDOW_SM1(UDM_SETBASE, nBase); }

	/** Retrieves the handle to the current buddy window. The return value
	is the handle to the current buddy window. If this window is a VWindow
	derived class, you can use GetVWindowFromHandle() to get a VWindow
	pointer to it, or simply use the BuddyVWindow() method.*/
	HWND			Buddy() const
		{ return (HWND)VWINDOW_SM0(UDM_GETBUDDY); }

	/** Set the buddy window for a up down control. The buddy window
	is normally an edit control that receives its numeric input from
	changes in the updown control.*/
	void			Buddy(HWND hWndBuddy) const
		{ VWINDOW_SM1(UDM_SETBUDDY, hWndBuddy); }

	/** Same as Buddy(), but returns a VWindow pointer to the object. If
	the buddy window handle is not a VWindow object, NULL will be returned.*/
	VWindow*		BuddyVWindow() const
		{ return GetVWindowFromHandle(Buddy()); }

	/** Set the buddy window for a up down control. The buddy window
	is normally an edit control that receives its numeric input from
	changes in the updown control.*/
	void			BuddyVWindow(VWindow const& wndBuddy) const
		{ Buddy(wndBuddy.GetHandle()); }

	/** Create the updown control. Returns VTRUE on success, VFALSE on
	error. nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus updown specific ones including:

	UDS_ALIGNLEFT - Positions the up-down control next to the left edge of
	the buddy window. The buddy window is moved to the right, and its width
	is decreased to accommodate the width of the up-down control.

	UDS_ALIGNRIGHT - Positions the up-down control next to the right edge of
	the buddy window. The width of the buddy window is decreased to accommodate
	the width of the up-down control.

	UDS_ARROWKEYS - Causes the up-down control to increment and decrement the
	position when the UP ARROW and DOWN ARROW keys are pressed.

	UDS_AUTOBUDDY - Automatically selects the previous window in the z-order
	as the up-down control's buddy window.

	UDS_HORZ - Causes the up-down control's arrows to point left and right
	instead of up and down.

	UDS_HOTTRACK - Causes the control to exhibit "hot tracking" behavior.
	That is, it highlights the UP ARROW and DOWN ARROW on the control as the
	pointer passes over them. This style requires Microsoft® Windows® 98 or
	Windows® 2000. If the system is running Windows® 95 or Windows NT® 4.0,
	the flag is ignored. To check whether hot tracking is enabled,
	call SystemParametersInfo.

	UDS_NOTHOUSANDS - Does not insert a thousands separator between every
	three decimal digits.

	UDS_SETBUDDYINT - Causes the up-down control to set the text of the buddy
	window (using the WM_SETTEXT message) when the position changes. The text
	consists of the position formatted as a decimal or hexadecimal string.

	UDS_WRAP - Causes the position to "wrap" if it is incremented or
	decremented beyond the ending or beginning of the range.*/
	VBOOL			Create(	VWindow const&	windowParent,
							VRect const&	r,
							VUINT			nID,
							VUINT			nStyle =
												WS_VISIBLE)
	{
		return VCommonControl::Create(	windowParent,
										r,
										WS_CHILD | nStyle,
										UPDOWN_CLASS,
										NULL,
										nID);
	}

	/** Retrieves acceleration information for the up-down control. If the
	nAccelCount parameter is zero and the pAccel parameter is NULL, the return
	value is the number of accelerators currently set for the control.
	nAccelCount is the number of accelerators specified by the pAccels array.
	pAccels is the address of an array of UDACCEL structures that contain
	acceleration information. Elements should be sorted in ascending order
	based on the nSec member.*/
	VUINT			GetAccel(	VINT		nAccelCount,
								UDACCEL*	pAccel) const
		{ return (VUINT)VWINDOW_SM2(UDM_GETACCEL, nAccelCount, pAccel); }

	/** Call this function to retrieve the upper and lower limits (range) for
	the up down control. The member function GetRange32() retrieves the spin
	button control's range as a 32-bit integer. This version returns a 32-bit
	value containing the upper and lower limits. The low-order word is the
	upper limit for the control, and the high-order word is the lower limit.*/
	VDWORD			GetRange( ) const
		{ return (VDWORD)VWINDOW_SM0(UDM_GETRANGE); }

	/** Call this function to retrieve the upper and lower limits (range) for
	the up down control. The member function GetRange32() retrieves the spin
	button control's range as a 32-bit integer.*/
	void			GetRange(	VINT& nLower,
								VINT& nUpper ) const
	{
		VDWORD nResult = VWINDOW_SM0(UDM_GETRANGE);
		nLower = HIWORD(nResult);
		nUpper = LOWORD(nResult);
	}

	/** Call this function to retrieve the upper and lower limits (range)
	for the up down control. This function retrieves the spin button
	control's range as a 32-bit integer.*/
	void			GetRange32(	VINT& nLower,
								VINT& nUpper ) const
		{ VWINDOW_SM2(UDM_GETRANGE32, &nLower, &nUpper); }

	/** Retrieves the current position of the up-down control with 16-bit
	precision. Call this function to retrieve the current position of a up
	down control. When it processes the value returned, the control updates
	its current position based on the caption of the buddy window. The control
	returns an error if there is no buddy window or if the caption specifies
	an invalid or out-of-range value. You must examine the high-order word
	of the return value. If an error occurred, the high-order work will be
	non-zero. You can use the HIWORD macro to extract the high word value.*/
	VDWORD			Position() const
		{ return (VDWORD)VWINDOW_SM0(UDM_GETPOS); }

	/** Sets the current position for the up-down control with 16-bit
	precision. This function only supports 16-bit positions. If 32-bit
	values have been enabled using SetRange32(), use GetPos32() instead
	(requires V5.80 of Comctl32.dll). The return value is the previous
	position.*/
	VUINT			Position(VINT nPosition) const
		{ return (VUINT)VWINDOW_SMB3(UDM_SETPOS, 0, (VSHORT)nPosition, 0); }

#ifdef UDM_SETPOS32
	/** Sets the position of the up-down control with 32-bit precision.
	Requires version 5.80 and later of comctl32.dll ie. Windows 2000
	or IE5 to be installed. Returns the previous position.*/
	VUINT			Position32(VUINT nPosition) const
		{ return VWINDOW_SMB2(UDM_SETPOS32, 0, nPosition); }
#endif

#ifdef UDM_GETPOS32
	/** Returns the 32-bit position of the up-down control. Requires
	version 5.80 and later of comctl32.dll ie. Windows 2000 or IE5
	to be installed.*/
	VUINT			Position32() const
		{ return (VUINT)VWINDOW_SMB0(UDM_GETPOS32); }
#endif

	/** Set the acceleration for the up-down control. nAccelCount is the
	number of accelerators specified by the pAccels array. pAccels is
	the address of an array of UDACCEL structures that contain acceleration
	information. Elements should be sorted in ascending order based on the
	nSec member.*/
	VBOOL			SetAccel(	VINT		nAccelCount,
								UDACCEL*	pAccels) const
	{
		/* These must be known.*/
		VASSERT(nAccelCount)
		VASSERT(pAccels)

		return VWINDOW_SMB2(UDM_SETACCEL, nAccelCount, pAccels);
	}

	/** Set the upper and lower limits (range) for a up down control. The
	member function SetRange32() sets the 32-bit range for the up down
	control.
	NOTE: The default range for the spin button has the maximum set to
	zero (0) and the minimum set to 100. Since the maximum value is less
	than the minimum value, clicking the up arrow will decrease the position
	and clicking the down arrow will increase it. Use SetRange() to adjust
	these values.*/
	void			SetRange(	VINT nUpper,
								VINT nLower) const
		{ VWINDOW_SM3(UDM_SETRANGE, 0, (VSHORT)nUpper, (VSHORT)nLower); }

	/** Set the upper and lower limits (range) for a up down control. The
	member function SetRange32() sets the 32-bit range for the up down control.
	NOTE: The default range for the spin button has the maximum set to
	zero (0) and the minimum set to 100. Since the maximum value is less than
	the minimum value, clicking the up arrow will decrease the position and
	clicking the down arrow will increase it. Use SetRange() to adjust these
	values.*/
	void			SetRange32(	VINT nUpper,
								VINT nLower) const
		{ VWINDOW_SMB2(UDM_SETRANGE32, nLower, nUpper); }

	/** Returns the current UNICODE format used. See CCM_GETUNICODEFORMAT
	for more information. Returns VTRUE if using UNICODE, VFALSE otherwise.*/
	VBOOL			UnicodeFormat() const
		{ return (VWINDOW_SM0(UDM_GETUNICODEFORMAT) > 0) ? VTRUE : VFALSE; }

	/** Set the current UNICODE format used. See CCM_SETUNICODEFORMAT for
	more information.*/
	void			UnicodeFormat(VBOOL bUnicode) const
		{ VWINDOW_SMB1(UDM_SETUNICODEFORMAT, bUnicode); }

protected:
	/** The OnDeltaPosition() notification is called before the WM_VSCROLL
	or WM_HSCROLL message, which actually changes the control's position.
	Return VFALSE to prevent the change in the control's position, or VTRUE
	to allow the change. The base class allows the change.*/
	virtual VBOOL	OnDeltaPosition(NMUPDOWN& nmUpDown)
		{ return VTRUE; }

	/** Override of OnReflectedNotify() to route UpDown events to
	their proper handler.*/
	virtual VLONG	OnReflectedNotify(	NMHDR&		nmhdr,
										VLPARAM&	lCommonControlResult)
	{
		if (	nmhdr.code == UDN_DELTAPOS &&
				!OnDeltaPosition((NMUPDOWN&)nmhdr) )
			return 1;

		return VCommonControl::OnReflectedNotify(nmhdr, lCommonControlResult);
	}
};

#endif /* VUPDOWNCONTROL*/
