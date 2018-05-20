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

#ifndef VMONTHCALENDARCONTROL
#define VMONTHCALENDARCONTROL

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"

/** VMonthCalendarControl encapsulates the Windows Month Calender
common control. The month calender control is only available with V4.70
and later of comctl32.dll. V4.70 was introduced with Internet Explorer 3.x
but is also available as 500comupd.exe (see Microsoft Knowledge Base
article Q186176: INFO: Redistribution of comctl32.dll).*/
class VMonthCalendarControl : public VCommonControl
{
public:
	/** Default constructor initializes RTTI information.*/
	VMonthCalendarControl(VRTTI_TYPE nRTTI = VWCL_RTTI_MONTH_CALENDAR_CONTROL)
		: VCommonControl(nRTTI)
		{;}

	/** Retrieves the color for a given portion of a month calendar control.
	nColor is a VINT value specifying which month calendar color to retrieve.
	This value can be one of the following:

	MCSC_BACKGROUND - The background color displayed between months.

	MCSC_MONTHBK - The background color displayed within the month.

	MCSC_TEXT - The color used to display text within a month.

	MCSC_TITLEBK - The background color displayed in the calendar's title.

	MCSC_TITLETEXT - The color used to display text within the
	calendar's title.

	MCSC_TRAILINGTEXT - Retrieve the color used to display header day and
	trailing day text. Header and trailing days are the days from the
	previous and following months that appear on the current month calendar.

	Returns a COLORREF value that represents the color setting for the
	specified portion of the month calendar control if successful. Otherwise,
	the return value is -1.*/
	COLORREF		Color(VINT nColor ) const
		{ return (COLORREF)VWINDOW_SM1(MCM_GETCOLOR, nColor); }

	/** Sets the color for a given portion of a month calendar control.
	iColor specifies which month calendar color to set. This value can
	be one of the following:

	MCSC_BACKGROUND - The background color displayed between months.

	MCSC_MONTHBK - The background color displayed within the month.

	MCSC_TEXT - The color used to display text within a month.

	MCSC_TITLEBK - The background color displayed in the calendar's title.

	MCSC_TITLETEXT - The color used to display text within the
	calendar's title.

	MCSC_TRAILINGTEXT - Retrieve the color used to display header day and
	trailing day text. Header and trailing days are the days from the
	previous and following months that appear on the current month calendar.

	The crColor value that represents the color that will be set for the
	specified area of the month calendar. Returns a COLORREF value that
	represents the previous color setting for the specified portion of the
	month calendar control, if successful. Otherwise this method returns
	-1.*/
	COLORREF		Color(	VINT		nColor,
							COLORREF	crColor) const
		{ return (COLORREF)VWINDOW_SM2(MCM_SETCOLOR, nColor, crColor); }

	/** Create the month calender control. Returns VTRUE on success, VFALSE on
	error. nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus updown specific ones including:

	MCS_DAYSTATE - The month calendar will send OnDayState notifications
	to request information about which days should be displayed in bold.
	For more information about supporting this style, see the OnDayState()
	documentation.

	MCS_MULTISELECT - The month calendar will allow the user to select a
	range of dates within the control. By default, the maximum range is one
	week. You can change the maximum range that can be selected by using
	the SetMaxSelCount function.

	MCS_NOTODAY - The month calendar control will not display the "today"
	date at the bottom of the control.

	MCS_NOTODAYCIRCLE - The month calendar control will not circle the
	"today" date.

	MCS_WEEKNUMBERS - The month calendar control will display week numbers
	(1-52) to the left of each row of days. Week 1 is defined as the first
	week that contains at least four days.*/
	VBOOL			Create(	VWindow const&	windowParent,
							VRect const&	r,
							VUINT			nID,
							VUINT			nStyle =
												WS_VISIBLE)
	{
		return VCommonControl::Create(	windowParent,
										r,
										WS_CHILD | nStyle,
										MONTHCAL_CLASS,
										NULL,
										nID);
	}

	/** Retrieves the first day of the week for the month calendar control.

	pbLocal is a pointer to a VBOOL value. If the value is non-zero,
	the control's setting does not match the setting in the control panel.
	If you are not interested in this value, you can pass NULL for this
	parameter. Returns an integer value that represents the first day of
	the week, where:
	0 is Monday
	1 is Tuesday
	2 is Wednesday
	3 is Thursday
	4 is Friday
	5 is Saturday
	6 is Sunday.*/
	VINT			FirstDayOfWeek(VBOOL* pbLocal = NULL) const
	{
		VDWORD nResult = VWINDOW_SM0(MCM_GETFIRSTDAYOFWEEK);

		if ( pbLocal )
			*pbLocal = (VBOOL)HIWORD(nResult);

		return LOWORD(nResult);
	}

	/** Sets the first day of the week for the month calendar control.

	nDay - An integer value representing which day is to be set as the
	first day of the week. This value must be one of the day numbers.
	See FirstDayOfWeek() for a description of the day numbers. Returns
	VTRUE if the previous first day of the week is set to a value other
	than that of LOCALE_IFIRSTDAYOFWEEK, which is the day indicated in the
	control panel setting. Otherwise, this function returns VFALSE.*/
	VBOOL			FirstDayOfWeek(VINT nDay) const
	{
		VDWORD nResult = (VDWORD)VWINDOW_SM2(MCM_SETFIRSTDAYOFWEEK, 0, nDay);

		if ( HIWORD(nResult) )
			VTRUE;

		return VFALSE;
	}

	/** Retrieves the maximum width of the "today" string in the month
	calendar control. This includes the label text and the date text. Returns
	the width of the "today" string, in pixels.*/
	VDWORD			GetMaxTodayWidth() const
		{ return (VDWORD)VWINDOW_SM0(MCM_GETMAXTODAYWIDTH); }

	/** Retrieves the minimum size required to display a full month in a
	month calendar control. Size information is presented in the form
	of a VRect object.

	The minimum required window size for the month calendar control depends
	on the currently selected font, control styles, system metrics, and
	regional settings. When an application changes anything that affects
	the minimum window size, or processes a WM_SETTINGCHANGE message, it
	should call GetMinReqRect() to determine the new minimum size.

	Note: The rectangle returned by GetMinReqRect() does not include the
	width of the "Today" string, if it is present. If the MCS_NOTODAY style
	is not set, your application should also retrieve the rectangle that
	defines the "Today" string width by calling the GetMaxTodayWidth()
	function. Use the larger of the two rectangles to ensure that the
	"Today" string is not clipped.

	The top and left members of rect will always be zero. The right and
	bottom members represent the minimum cx and cy required for the
	control.*/
	VBOOL			GetMinReqRect(VRect& rect) const
		{ return VWINDOW_SMB2(MCM_GETMINREQRECT, 0, rect.GetPointer()); }

	/** Retrieve date information (using SYSTEMTIME structures) that
	represents the high and low limits of a month calendar control's display.
	nFlag specifies the scope of the range limits to be retrieved. This value
	must be one of the following:

	GMR_DAYSTATE - Include preceding and trailing months of visible range
	that are only partially displayed.

	GMR_VISIBLE - Include only those months that are entirely displayed.

	Returns a VINT value that represents the range, in months, spanned by
	the two limits specified. The time members of these structures will not
	be modified.*/
	VINT			GetMonthRange(	SYSTEMTIME&	stMinRange,
									SYSTEMTIME&	stMaxRange,
									VDWORD		nFlags) const
	{
		SYSTEMTIME	stTimes[2];
		VINT		nCount =
			(VINT)VWINDOW_SM2(MCM_GETMONTHRANGE, nFlags, &stTimes);

		memcpy(&stMinRange, &stTimes[0], sizeof(SYSTEMTIME));
		memcpy(&stMaxRange, &stTimes[1], sizeof(SYSTEMTIME));

		return nCount;
	}

	/** Retrieve the minimum and maximum allowable dates set for the month
	calendar control. stMinRange is a reference to a SYSTEMTIME structure
	containing the date at the lowest end of the range. stMaxRange is a
	reference to a SYSTEMTIME structure containing the date at the highest
	end of the range. Returns VFALSE if no limits have been set, VTRUE
	otherwise. stMinRange and stMaxRange will have all members set to
	0. If this function returns with structures with all 0 members, it
	probably means that the range for that structure was not set.*/
	VBOOL			GetRange(	SYSTEMTIME& stMinRange,
								SYSTEMTIME& stMaxRange) const
	{
		VBOOL bResult = VFALSE;

		/* Initialize out parameters.*/
		VZEROSTRUCT(stMinRange);
		VZEROSTRUCT(stMaxRange);

		/* Initialize array.*/
		SYSTEMTIME stTimes[2];
		memset(stTimes, 0, sizeof(stTimes));

		VDWORD nRanges =
			(VDWORD)VWINDOW_SM2(MCM_GETRANGE, 0, &stTimes);

		if ( nRanges & GDTR_MIN )
		{
			memcpy(&stMinRange, &stTimes[0], sizeof(SYSTEMTIME));
			bResult = VTRUE;
		}

		if ( nRanges & GDTR_MAX )
		{
			memcpy(&stMaxRange, &stTimes[1], sizeof(SYSTEMTIME));
			bResult = VTRUE;
		}

		return bResult;
	}

	/** Retrieves the currently selected date. stTime is a refence to a
	SYSTEMTIME structure that will receive the currently selected date
	information. Returns VTRUE if successful, or VFALSE otherwise. This
	function will always fail when applied to month calendar controls
	that are set to the MCS_MULTISELECT style.*/
	VBOOL			GetSelection(SYSTEMTIME& stTime) const
		{ return VWINDOW_SMB2(MCM_GETCURSEL, 0, &stTime); }

	/** Retrieve date information that represents the upper and lower limits
	of the date range currently selected by the user. Returns VTRUE if
	successful, or VFALSE otherwise. GetSelectionRange() will fail if
	applied to a month calendar control that does not use the
	MCS_MULTISELECT style.*/
	VBOOL			GetSelectionRange(	SYSTEMTIME& stMinRange,
										SYSTEMTIME& stMaxRange ) const
	{
		VASSERT((Style() & MCS_MULTISELECT))

		SYSTEMTIME stTimes[2];

		if ( VWINDOW_SMB2(MCM_GETSELRANGE, 0, &stTimes) )
		{
			memcpy(&stMinRange, &stTimes[0], sizeof(SYSTEMTIME));
			memcpy(&stMaxRange, &stTimes[1], sizeof(SYSTEMTIME));

			return VTRUE;
		}

		return VFALSE;
	}

	/** Retrieves the date information for the date specified as "today"
	for the month calendar control. stToday is a reference to a SYSTEMTIME
	structure that will receive the date information.*/
	VBOOL			GetToday(SYSTEMTIME& stToday) const
		{ return VWINDOW_SMB2(MCM_GETTODAY, 0, &stToday); }

	/** Determine which portion of a month calendar control is at a given
	point on the screen.

	mcHitTest is a reference to a MCHITTESTINFO structure. Upon sending the
	message, the cbSize member will be set to the size of the MCHITTESTINFO
	structure, so you can skip this in your code. The pt member must be set
	to the point you want to hit test.

	Upon returning from this method, the members of the MCHITTESTINFO
	structure will be set and the function will return a VDWORD value that
	contains one or more of the following:

	MCHT_CALENDAR - The given point was within the calendar.

	MCHT_CALENDARBK - The given point was in the calendar's background.

	MCHT_CALENDARDATE - The given point was on a particular date within
	the calendar. The SYSTEMTIME structure at mcHitTest.st is set to the
	date at the given point.

	MCHT_CALENDARDATENEXT - The given point was over a date from the next
	month (partially displayed at the end of the currently displayed month).
	If the user clicks here, the month calendar will scroll its display to
	the next month or set of months.

	MCHT_CALENDARDATEPREV - The given point was over a date from the previous
	month (partially displayed at the end of the currently displayed month).
	If the user clicks here, the month calendar will scroll its display to
	the previous month or set of months.

	MCHT_CALENDARDAY - The given point was over a day abbreviation
	("Fri", for example). The SYSTEMTIME structure at mcHitTest.st is set to
	the corresponding date in the top row.

	MCHT_CALENDARWEEKNUM - The given point was over a week number
	(MCS_WEEKNUMBERS style only). The SYSTEMTIME structure at mcHitTest.st is
	set to the corresponding date in the leftmost column.

	MCHT_NEXT - The given point is in an area that will cause the month
	calendar to scroll its display to the next month or set of months. This
	flag is used to modify other hit test flags.

	MCHT_NOWHERE - The given point was not on the month calendar control, or
	it was in an inactive portion of the control.

	MCHT_PREV - The given point is in an area that will cause the month
	calendar to scroll its display to the previous month or set of months.
	This flag is used to modify other hit test flags.

	MCHT_TITLE - The given point was over a month's title.

	MCHT_TITLEBK - The given point was over the background of a month's
	title.

	MCHT_TITLEBTNNEXT - The given point was over the button at the top right
	corner of the control. If the user clicks here, the month calendar will
	scroll its display to the next month or set of months.

	MCHT_TITLEBTNPREV - The given point was over the button at the top left
	corner of the control. If the user clicks here, the month calendar will
	scroll its display to the previous month or set of months.

	MCHT_TITLEMONTH - The given point was in a month's title bar, over a
	month name.

	MCHT_TITLEYEAR - The given point was in a month's title bar, over the
	year value.

	MCHT_TODAYLINK - The given point was on the "today" link at the bottom
	of the month calendar control.

	The uHit member of the MCHITTESTINFO structure at mcHitTest will equal
	the return value.*/
	VUINT			HitTest(MCHITTESTINFO& mcHitTest) const
	{
		/* Verify proper structure initialization.*/
		POINT ptSave = mcHitTest.pt;
		VZEROSTRUCT(mcHitTest);

		/* Restore user value and structure size.*/
		mcHitTest.pt =		ptSave;
		mcHitTest.cbSize =	sizeof(MCHITTESTINFO);

		return (VDWORD)VWINDOW_SM2(MCM_HITTEST, 0, &mcHitTest);
	}

	/** Retrieve the maximum date range that can be selected in the month
	calendar control. Returns an VINT value that represents the total number
	of days that can be selected for the control. You can change the maximum
	date range that can be selected by using the MaxSelectionCount() function.*/
	VINT			MaxSelectionCount() const
		{ return (VINT)VWINDOW_SM0(MCM_GETMAXSELCOUNT); }

	/** Set the maximum number of days that can be selected in the month
	calendar control. nMax represents the maximum number of days that can
	be selected. Returns VTRUE if successful, or VFALSE otherwise. This
	method will fail if applied to a month calendar control that does not
	use the MCS_MULTISELECT style.*/
	VBOOL			MaxSelectionCount(VUINT nMax ) const
	{
		VASSERT((Style() & MCS_MULTISELECT))
		return VWINDOW_SMB1(MCM_SETMAXSELCOUNT, nMax);
	}

	/** Retrieves the scroll rate for the month calendar control. The scroll
	rate is the number of months that the control moves its display when
	the user clicks a scroll button.

	If the month delta was previously set using the MonthDelta() function,
	returns a VINT value that represents the month calendar's current scroll
	rate. If the month delta was not previously set using the MonthDelta()
	function, or the month delta was reset to the default, returns an VINT
	value that represents the current number of months visible.*/
	VINT			MonthDelta( ) const
		{ return (VINT)VWINDOW_SM0(MCM_GETMONTHDELTA); }

	/** Sets the scroll rate for the month calendar control. The scroll rate
	is the number of months that the control moves its display when the
	user clicks a scroll button.

	nDelta represents the number of months to be set as the control's
	scroll rate. If this value is zero, the month delta is reset to the
	default, which is the number of months displayed in the control.

	Returns an VINT value that represents the previous scroll rate. If
	the scroll rate was not previously set, the return value is zero.

	The PAGE UP and PAGE DOWN keys, VK_PRIOR and VK_NEXT, change the
	selected month by one, regardless of the number of months displayed
	or the value set by MonthDelta().*/
	VINT			MonthDelta(VINT nDelta ) const
		{ return (VINT)VWINDOW_SM1(MCM_SETMONTHDELTA, nDelta); }

	/** Display the month calendar control to the minimum size that displays
	one month. Calling SizeMinReq() successfully displays the entire month
	calendar control for one month's calendar.*/
	VBOOL			SizeMinReq(VBOOL bRepaint = VTRUE) const
	{
		VRect r;

		if ( GetMinReqRect(r) )
		{
			VDWORD nFlags =
				SWP_NOZORDER |
				SWP_NOREPOSITION |
				SWP_NOMOVE |
				SWP_NOACTIVATE;

			if ( !bRepaint )
				nFlags |= SWP_NOREDRAW;

			SetWindowPos(	GetHandle(),
							NULL,
							VRECT_BREAK_WIDTH_HEIGHT(r),
							nFlags);

			return VTRUE;
		}

		return VFALSE;
	}

	/** Set the day states for all months that are currently visible within
	the month calendar control.

	nMonths indicates how many elements are in the array that pDayStateArray
	points to.

	pDayStateArray is an array of MONTHDAYSTATE values that define how the
	month calendar control will draw each day in its display. This array must
	contain as many elements as the value returned by GetMonthRange(). Keep
	in mind that this array must contain MONTHDAYSTATE values that correspond
	with all months currently in the control's display, in chronological
	order. This includes the two months only partially displayed before the
	first month and after the last month.

	Returns VTRUE if successful, or VFLASE otherwise.*/
	VBOOL			SetDayState(VINT					nMonths,
								MONTHDAYSTATE const*	pDayStateArray)
	{
		VASSERT(nMonths)
		VASSERT(pDayStateArray)
		return VWINDOW_SMB2(MCM_SETDAYSTATE, nMonths, pDayStateArray);
	}

	/** Set the "today" selection for the month calendar control. stToday is
	a reference to a SYSTEMTIME structure that contains the date to be set
	as the "today" selection for the control. If bDefault is VTRUE, the
	SYSTEMTIME structure referenced by st is not used and the control
	returns to the default setting. If the "today" selection is set to
	any date other than the default the control will not automatically
	update the "today" selection when the time passes midnight for the
	current day and will not automatically update its display based on
	locale changes.*/
	void			SetToday(SYSTEMTIME const&	stToday,
							VBOOL				bDefault =
													VFALSE)
		{ VWINDOW_SM2(MCM_SETTODAY, 0, ((bDefault) ? NULL : &stToday)); }

	/** Sets the currently selected date for the month calendar control.
	If the specified date is not in view, the control updates the display
	to bring it into view. stTime is a reference to a SYSTEMTIME structure
	that contains the date to be set as the current selection. Returns VTRUE
	if successful, or VFLASE otherwise. This method will fail if applied to
	a month calendar control that uses the MCS_MULTISELECT style.*/
	VBOOL			SetSelection(SYSTEMTIME const& stTime) const
	{
		VASSERT(!(Style() & MCS_MULTISELECT))
		return VWINDOW_SMB2(MCM_SETCURSEL, 0, &stTime);
	}

	/** Sets the minimum and maximum allowable dates for the month calendar
	control. stMinRange is a reference to a SYSTEMTIME structure containing
	the date at the lowest end of the range. stMaxRange is a reference to a
	SYSTEMTIME structure containing the date at the highest end of the
	range.*/
	VBOOL			SetRange(	SYSTEMTIME const& stMinRange,
								SYSTEMTIME const& stMaxRange) const
	{
		/* Initialize array.*/
		SYSTEMTIME stTimes[2];
		memcpy(&stTimes[0], &stMinRange, sizeof(SYSTEMTIME));
		memcpy(&stTimes[1], &stMaxRange, sizeof(SYSTEMTIME));

		return VWINDOW_SMB2(DTM_SETRANGE, GDTR_MIN | GDTR_MAX, &stTimes);
	}

	/** Set the selection for the month calendar control to a given date
	range. Returns VTRUE if successful, or VFALSE otherwise. This method
	will fail if applied to a month calendar control that does not use
	the MCS_MULTISELECT style.*/
	VBOOL			SetSelectionRange(	SYSTEMTIME const& stMinRange,
										SYSTEMTIME const& stMaxRange ) const
	{
		VASSERT((Style() & MCS_MULTISELECT))

		SYSTEMTIME stTimes[2];
		memcpy(&stTimes[0], &stMinRange, sizeof(SYSTEMTIME));
		memcpy(&stTimes[1], &stMaxRange, sizeof(SYSTEMTIME));

		return VWINDOW_SMB2(MCM_SETSELRANGE, 0, &stTimes);
	}

protected:
	/** Sent by the month calendar control to request information about how
	individual days should be displayed. This notification message is sent
	only by month calendar controls that use the MCS_DAYSTATE style. The
	base class does nothing.*/
	virtual void	OnDayState(NMDAYSTATE& nmDayState)
		{;}

	/** Sent by a month calendar control when the user makes an explicit
	date selection within a month calendar control.*/
	virtual void	OnSelect(NMSELCHANGE& nmSelChange)
	{
	}

	/** Sent by the month calendar control when the currently selected date
	or range of dates changes. For example, the control sends MCN_SELCHANGE
	when the user explicitly changes his or her selection within the current
	month or when the selection is implicitly changed in response to
	next/previous month navigation. This notification message is similar to
	OnSelect(), but it is sent in response to any selection change.
	OnSelect() is sent only for an explicit date selection.*/
	virtual void	OnSelectionChange(NMSELCHANGE& nmSelChange)
		{;}

	/** Override of OnReflectedNotify() to route events to their
	proper handler.*/
	virtual VLONG	OnReflectedNotify(	NMHDR&		nmhdr,
										VLPARAM&	lCommonControlResult)
	{
		switch ( nmhdr.code )
		{
			case MCN_GETDAYSTATE:
				OnDayState((NMDAYSTATE&)nmhdr);
				break;

			case MCN_SELCHANGE:
				OnSelectionChange((NMSELCHANGE&)nmhdr);
				break;

			case MCN_SELECT:
				OnSelect((NMSELCHANGE&)nmhdr);
				break;
		}

		return VCommonControl::OnReflectedNotify(nmhdr, lCommonControlResult);
	}
};

#endif /* VMONTHCALENDARCONTROL*/
