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

#ifndef VFORMATTEDEDIT
#define VFORMATTEDEDIT

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../numbers/vrtti.hpp"
#include "../strings/vstring.hpp"
#include "../structures/vrect.hpp"
#include "vedit.hpp"

/** VFormattedEdit is a VEdit window which requires subclassing of a
Windows EDIT control, and validates data as entered.*/
class VFormattedEdit : public VEdit
{
public:
	/** Formatting constants held in m_Options which can be retreived
	using GetFormatOptions() or can be set during construction. Some can
	be or'd, others are mutually exclusive. ECF_NUMBER can be or'd with
	ECF_DECIMAL and ECF_NEGATIVE. All others are mutually exclusive to
	each other.*/
	enum	{
					FORMAT_OPTION_NONE =					0,
					FORMAT_OPTION_ALLOW_NUMBERS =			0x0001,
					FORMAT_OPTION_ALLOW_DECIMAL =			0x0002,
					FORMAT_OPTION_ALLOW_NEGATIVE =			0x0004,
					FORMAT_OPTION_ONLY_ALLOW_LOWERCASE =	0x0008,
					FORMAT_OPTION_ONLY_ALLOW_UPPERCASE =	0x0010,
					FORMAT_OPTION_FORCE_LOWER_CASE =		0x0020,
					FORMAT_OPTION_FORCE_UPPER_CASE =		0x0040,

			};

	/** Default constructor can be initialized with formatting options.
	It also initializes base class.*/
	VFormattedEdit(	VUINT		nFormatOptions =
									FORMAT_OPTION_NONE,
					VRTTI_TYPE	nRTTI =
									VWCL_RTTI_FORMATTED_EDIT)
		: VEdit(nRTTI)
		{ m_FormatOptions = nFormatOptions; }

	/** Return a reference to the VString object which holds the characters
	that and invalid and thus not allowed to be entered. If this string is
	not empty, the above formatting options will not be used. You must
	decide to use either this string of invalid characters to choose the
	above formatting options and leave this string empty.*/
	VString&		GetInvalid() const
		{ return (VString&)m_strInvalid; }

	/** Get the VBitmask object which holds the above option bits. The
	setting of these bits does not override the invalid string above. It
	must be empty for these settings to have any effect.*/
	VBitmask&		GetFormatOptions() const
		{ return (VBitmask&)m_FormatOptions; }

	/** Determine if a character is valid. If needed, this function will
	convert the case of nChar to make it valid.*/
	virtual VBOOL	IsValidCharacter(VCHAR& nChar)
	{
		/* If the m_strInvalid member is not empty, we will
		check to make sure nChar is NOT in the string.*/
		if ( m_strInvalid.IsNotEmpty() )
			return (m_strInvalid.FindFirst(nChar) != -1) ? VTRUE : VFALSE;

		/* Do we need to convert the case first?*/
		if ( m_FormatOptions.IsSet(FORMAT_OPTION_FORCE_LOWER_CASE) )
			nChar = VTOLOWER(nChar);
		else if ( m_FormatOptions.IsSet(FORMAT_OPTION_FORCE_UPPER_CASE) )
			nChar = VTOUPPER(nChar);

		VString s;

		/* If we are checking for a decimal make sure there is not
		already one in the control, and for a negative, make sure the - is
		in the first place and does not already exist.*/
		if (	nChar == VTEXT('.') &&
				m_FormatOptions.IsSet(FORMAT_OPTION_ALLOW_DECIMAL) )
		{
			if ( !GetTextLength() )
				return VTRUE;
			else if ( GetText(s) && s.FindFirst(VTEXT('.')) == -1 )
				return VTRUE;
		}

		if (	nChar == VTEXT('-') &&
				m_FormatOptions.IsSet(FORMAT_OPTION_ALLOW_NEGATIVE) )
		{
			/* For the negative case, make sure we are inserting
			at the first position.*/
			if ( !GetTextLength() )
				return VTRUE;
			else
			{
				if (	(s.IsNotEmpty() ||
						GetText(s)) && s.FindFirst(VTEXT('-') != -1) )
				{
					VDWORD nStartPosition, nEndPosition;
					Selection(&nStartPosition, &nEndPosition);

					if ( !nStartPosition && !nEndPosition )
						return VTRUE;
				}
			}
		}

		/* Make sure nChar is between 0 and 9?*/
		if ( m_FormatOptions.IsSet(FORMAT_OPTION_ALLOW_NUMBERS) )
			return	(nChar >= VTEXT('0') && nChar <= VTEXT('9'))
					? VTRUE : VFALSE;

		/* Make sure nChar is between a and z?*/
		if ( m_FormatOptions.IsSet(FORMAT_OPTION_ONLY_ALLOW_LOWERCASE) )
			return	(nChar >= VTEXT('a') && nChar <= VTEXT('z'))
					? VTRUE : VFALSE;

		/* Make sure nChar is between A and Z?*/
		if ( m_FormatOptions.IsSet(FORMAT_OPTION_ONLY_ALLOW_UPPERCASE) )
			return	(nChar >= VTEXT('A') && nChar <= VTEXT('Z'))
					? VTRUE : VFALSE;

		return VFALSE;
	}

protected:
	/** Window procedure override looks for messages we are interested in.*/
	virtual VLONG	WindowProc(	HWND	hWnd,
								VUINT	nMessage,
								VWPARAM	wParam,
								VLPARAM	lParam)
	{
		switch ( nMessage )
		{
			case WM_CHAR:
			{
				VCHAR nChar = (VCHAR)wParam;

				/* Check for valid input if format option bits are
				present or invalid string is not empty.*/
				if (	(nChar != VK_BACK) &&
						(m_FormatOptions.Bits() ||
						m_strInvalid.String()) )
				{
					if ( !IsValidCharacter(nChar) )
					{
						MessageBeep(MB_ICONEXCLAMATION);
						return 0;
					}
				}

				break;
			}

			/* Do not allow paste unless this control has no format
			options or invalid text chars.*/
			case WM_PASTE:
			{
				if ( m_FormatOptions.Bits() || m_strInvalid.String() )
				{
					MessageBeep(MB_ICONEXCLAMATION);
					return 0;
				}

				break;
			}
		}

		/* If we made it this far, the character is valid.*/
		return VEdit::WindowProc(hWnd, nMessage, wParam, lParam);
	}

	/** Embedded Members.*/
	VString			m_strInvalid;
	VBitmask		m_FormatOptions;
};

#endif /* VFORMATTEDEDIT*/
