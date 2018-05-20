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

#ifndef VDATETIMECONTROL
#define VDATETIMECONTROL

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vmonthcalendarcontrol.hpp"
#include "vcommoncontrol.hpp"

/** VDateTimeControl encapsulates the Windows Date and Time Picker common
control. The date and time picker controls are only available with V4.70 and
later of comctl32.dll. V4.70 was introduced with Internet Explorer 3.x but
is also available in 500comupd.exe (see Microsoft Knowledge Base article
Q186176: INFO: Redistribution of comctl32.dll).*/
class VDateTimeControl : public VCommonControl
{
public:
	/** Default constructor initializes RTTI information.*/
	VDateTimeControl(VRTTI_TYPE nRTTI = VWCL_RTTI_DATETIME_CONTROL)
		: VCommonControl(nRTTI)
		{;}

	/** Create the date time control. Returns VTRUE on success, VFALSE on
	error. nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus date and time specific ones including:

	DTS_LONGDATEFORMAT - Displays the date in long format. The default format
	string for this style is defined by LOCALE_SLONGDATEFORMAT, which produces
	output like "Friday, April 19, 1996".

	DTS_RIGHTALIGN - The drop-down month calendar will be right-aligned with
	the control instead of left-aligned, which is the default.

	DTS_SHOWNONE - It is possible to have no date currently selected in the
	control. With this style, the control displays a check box that users can
	check once they have entered or selected a date. Until this check box is
	checked, the application will not be able to retrieve the date from the
	control because, in essence, the control has no date. This state can be
	set with the DTM_SETSYSTEMTIME message or queried with the
	DTM_GETSYSTEMTIME message.

	DTS_SHORTDATEFORMAT - Displays the date in short format. The default
	format string for this style is defined by LOCALE_SSHORTDATE, which
	produces output like "4/19/96".

	DTS_SHORTDATECENTURYFORMAT - Version 5.80 - Similar to the
	DTS_SHORTDATEFORMAT style, except the year is a four-digit field. The
	default format string for this style is based on LOCALE_SSHORTDATE.
	The output looks like: "4/19/1996".

	DTS_TIMEFORMAT - Displays the time. The default format string for this
	style is defined by LOCALE_STIMEFORMAT, which produces output like
	"5:31:42 PM".

	DTS_UPDOWN - Places an up-down control to the right of the DTP control
	to modify date-time values. This style can be used in place of the
	drop-down month calendar, which is the default style.

	NOTE: The DTS_XXXFORMAT styles that define the display format cannot
	be combined. If none of the format styles are suitable, use a
	DTM_SETFORMAT message to define a custom format. DTS_APPCANPARSE allows
	the owner to parse user input and take necessary action. It enables users
	to edit within the client area of the control when they press the F2 key.
	The control sends DTN_USERSTRING notification messages when users are
	finished.*/
	VBOOL					Create(	VWindow const&	windowParent,
									VRect const&	r,
									VUINT			nID,
									VUINT			nStyle =
														WS_VISIBLE)
	{
		return VCommonControl::Create(	windowParent,
										r,
										WS_CHILD | nStyle,
										DATETIMEPICK_CLASS,
										NULL,
										nID);
	}

	/** Return a reference to the month and calendar control.*/
	VMonthCalendarControl&	GetMonthCalendarControl( ) const
	{
		m_wndMonthCalendarControl.GetHandle() =
			(HWND)VWINDOW_SM0(DTM_GETMONTHCAL);

		return (VMonthCalendarControl&)m_wndMonthCalendarControl;
	}

	/** Retrieves the current minimum and maximum allowable system
	times for the date and time picker (DTP) control. Both
	SYSTEMTIME structures must be passed in as references, which
	will contain the result after calling this method.*/
	void					GetRange(	SYSTEMTIME& stMinRange,
										SYSTEMTIME& stMaxRange ) const
	{
		SYSTEMTIME st[2];
		memcpy(&st[0], &stMinRange, sizeof(SYSTEMTIME));
		memcpy(&st[1], &stMaxRange, sizeof(SYSTEMTIME));

		VWINDOW_SM2(DTM_GETRANGE, 0, &st);
	}

	/** Retrieves the currently selected time from a date and time picker
	(DTP) control and places it in a specified SYSTEMTIME structure. Returns
	GDT_VALID if the time information was successfully placed in stTimeDest.
	Returns GDT_NONE if the control was set to the DTS_SHOWNONE style and the
	control check box was not selected. Returns GDT_ERROR if an error
	occurs.*/
	VDWORD					GetTime(SYSTEMTIME& stTimeDest) const
		{ return (VDWORD)VWINDOW_SM2(DTM_GETSYSTEMTIME, 0, &stTimeDest); }

	/** Retrieves the color for a given portion of the month calendar within
	the date and time picker (DTP) control. nColor specifies which month
	calendar color to retrieve. This value can be one of the following:

	MCSC_BACKGROUND - Retrieve the background color displayed between months.

	MCSC_MONTHBK - Retrieve the background color displayed within the month.

	MCSC_TEXT - Retrieve the color used to display text within a month.

	MCSC_TITLEBK - Retrieve the background color displayed in the
	calendar's title.

	MCSC_TITLETEXT - Retrieve the color used to display text within the
	calendar's title.

	MCSC_TRAILINGTEXT - Retrieve the color used to display header day and
	trailing day text. Header and trailing days are the days from the previous
	and following months that appear on the current month calendar.

	Returns a COLORREF value that represents the color setting for the
	specified portion of the month calendar control if successful, otherwise,
	it returns -1.*/
	COLORREF				MonthCalendarColor(VINT nColor) const
		{ return (COLORREF)VWINDOW_SM1(DTM_GETMCCOLOR, nColor); }

	/** Sets the color for a given portion of the month calendar within the
	date and time picker (DTP) control. See the above function for information
	about the nColor parameter. crNewColor is a COLORREF which specifies the
	new color to use. Returns a COLORREF value that represents the previous
	color setting for the specified portion of the month calendar control if
	successful. Otherwise, this method returns -1.*/
	COLORREF				MonthCalendarColor(	VINT		nColor,
												COLORREF	crNewColor) const
		{ return (COLORREF)VWINDOW_SM2(DTM_SETMCCOLOR, nColor, crNewColor); }

	/** Retrieves the font that the date and time picker (DTP) control's
	child month and calendar control is currently using.*/
	HFONT					MonthCalendarFont( ) const
		{ return (HFONT)VWINDOW_SM0(DTM_GETMCFONT); }

	/** Sets the font to be used by the date and time picker (DTP) control's
	child month calendar control.*/
	void					MonthCalendarFont(	HFONT hFont,
												VBOOL bRedraw =
														TRUE) const
		{ VWINDOW_SM2(DTM_SETMCFONT, hFont, bRedraw); }

	/** Sets the display of a date and time picker (DTP) control based on a
	given format string. pszFormat is a zero-terminated format string that
	defines the desired display. Setting this parameter to NULL will reset
	the control to the default format string for the current style.

	It is acceptable to include extra characters within the format string to
	produce a more rich display. However, any nonformat characters must be
	enclosed within single quotes. For example, the format string
	"'Today is: 'hh':'m':'s ddddMMMdd', 'yyy" would produce output like
	"Today is: 04:22:31 Tuesday Mar 23, 1996".

	NOTE: A DTP control tracks locale changes when it is using the default
	format string. If you set a custom format string, it will not be updated
	in response to locale changes.*/
	VBOOL					SetFormat(VSTRING_CONST pszFormat = NULL) const
		{ return VWINDOW_SMB2(DTM_SETFORMAT, 0, pszFormat); }

	/** Sets the minimum and maximum allowable system times for the date
	and time picker (DTP) control. You can specify either a valid pointer
	for pstMinRange or pstMaxRange or a NULL value in which case the value
	will not be used.*/
	VBOOL					SetRange(	SYSTEMTIME const* pstMinRange,
										SYSTEMTIME const* pstMaxRange) const
	{
		SYSTEMTIME st[2];
		VZEROMEMORY(&st, sizeof(st));

		VDWORD nFlags = 0;

		if ( pstMinRange )
		{
			nFlags |= GDTR_MIN;
			memcpy(&st[0], pstMinRange, sizeof(SYSTEMTIME));
		}

		if( pstMaxRange )
		{
			nFlags |= GDTR_MAX;
			memcpy(&st[1], pstMaxRange, sizeof(SYSTEMTIME));
		}

		return VWINDOW_SMB2(DTM_SETRANGE, nFlags, &st);
	}

	/** Sets the date and time picker (DTP) control to a given date and time.
	If pstTimeNew is a valid pointer it sets the DTP control according to the
	data within the SYSTEMTIME structure. If pstTimeNew is NULL, it sets the
	DTP control to "no date" and clear its check box.*/
	VBOOL					SetTime(SYSTEMTIME* pstTimeNew = NULL) const
	{
		VDWORD nFlags = GDT_NONE;

		if (pstTimeNew )
			nFlags = GDT_VALID;

		return VWINDOW_SMB2(DTM_SETSYSTEMTIME, nFlags, pstTimeNew);
	}

protected:
	/** This virtual function is called by a date and time picker (DTP)
	control when the user closes the drop-down month calendar. The month
	calendar is closed when the user chooses a date from the month calendar
	or clicks the drop-down arrow while the calendar is open. The base
	class does nothing.*/
	virtual void			OnCloseUp(NMHDR& nmhdr)
		{;}

	/** This virtual function is called by a date and time picker (DTP)
	control whenever a change occurs. The base class does nothing.*/
	virtual void			OnDateTimeChange(NMDATETIMECHANGE& nmChange)
		{;}

	/** This virtual function is called by a date and time picker (DTP)
	control when the user activates the drop-down month calendar. The
	base class does nothing.*/
	virtual void			OnDropDown(NMHDR& nmhdr)
		{;}

	/** This virtual function is called by a date and time picker (DTP)
	control to request text to be displayed in a callback field. Handling
	this message allows the owner of the control to provide a custom string
	that the control will display. This base class does nothing.*/
	virtual void			OnFormat(NMDATETIMEFORMAT& nmFormat)
		{;}

	/** This virtual function is called by a date and time picker (DTP)
	control to retrieve the maximum allowable size of the string that will
	be displayed in a callback field. The owner of the control must calculate
	the maximum possible width of the text that will be displayed in the
	callback field, and set the szMax member of the NMDATETIMEFORMATQUERY
	structure. Handling this message prepares the control to adjust for the
	maximum size of the string that will be displayed in a particular callback
	field. This enables the control to properly display output at all times,
	reducing flicker within the control's display. The base class does
	nothing.*/
	virtual void			OnFormatQuery(NMDATETIMEFORMATQUERY& nmQuery)
		{;}

	/** This virtual function is called by a date and time picker (DTP)
	control when the user types in a callback field. Handling this
	message allows the owner of the control to provide specific responses
	to keystrokes within the callback fields of the control. The base
	class function does nothing.*/
	virtual void			OnKeyDown(NMDATETIMEWMKEYDOWN& nmKeyDown)
		{;}

	/** This virtual function is called by a date and time picker (DTP)
	control when a user finishes editing a string in the control. This
	notification message is only sent by DTP controls that are set to
	the DTS_APPCANPARSE style. Handling this message allows the owner
	to provide custom responses to strings entered into the control by
	the user. The owner must be prepared to parse the input string and
	take action if necessary. This base class does nothing.*/
	virtual void			OnUserString(NMDATETIMESTRING& nmString)
		{;}

	/** Override of the OnReflectedNotify to route events
	to their proper handler.*/
	virtual VLONG			OnReflectedNotify(
		NMHDR&		nmhdr,
		VLPARAM&	lCommonControlResult)
	{
		switch ( nmhdr.code )
		{
			case DTN_CLOSEUP:
				OnCloseUp(nmhdr);
				break;

			case DTN_DATETIMECHANGE:
				OnDateTimeChange((NMDATETIMECHANGE&)nmhdr);
				return 0;

			case DTN_DROPDOWN:
				OnDropDown(nmhdr);
				return 0;

			case DTN_FORMAT:
				OnFormat((NMDATETIMEFORMAT&)nmhdr);
				return 0;

			case DTN_FORMATQUERY:
				OnFormatQuery((NMDATETIMEFORMATQUERY&)nmhdr);
				return 0;

			case DTN_USERSTRING:
				OnUserString((NMDATETIMESTRING&)nmhdr);
				return 0;

			case DTN_WMKEYDOWN:
				OnKeyDown((NMDATETIMEWMKEYDOWN&)nmhdr);
				return 0;

		}

		return VCommonControl::OnReflectedNotify(nmhdr, lCommonControlResult);
	}

	/** Embedded Members.*/
	VTempWindow				m_wndMonthCalendarControl;
};

#endif /* VDATETIMECONTROL*/
