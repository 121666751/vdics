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

#ifndef VSTATIC
#define VSTATIC

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"

/** VStatic encapsulates the standard Windows static control.*/
class VStatic : public VWindow
{
public:
	/** Default constructor initializes base class RTTI information.*/
	VStatic(VRTTI_TYPE nRTTI = VWCL_RTTI_STATIC)
		: VWindow(nRTTI)
		{;}

	/** Create a new static control. nStyle is a combination of
	WS_xxx window style bits or'd with these button specific bits:

	SS_BLACKFRAME - Specifies a box with a frame drawn with the same color
	as window frames. The default is black.

	SS_BLACKRECT - Specifies a rectangle filled with the color used to
	draw window frames. The default is black.

	SS_CENTER - Designates a simple rectangle and displays the given text
	centered in the rectangle. The text is formatted before it is displayed.
	Words that would extend past the end of a line are automatically wrapped
	to the beginning of the next centered line.

	SS_GRAYFRAME - Specifies a box with a frame drawn with the same color
	as the screen background (desktop). The default is gray.

	SS_GRAYRECT - Specifies a rectangle filled with the color used to fill
	the screen background. The default is gray.

	SS_ICON - Designates an icon displayed in the dialog box. The given
	text is the name of an icon (not a filename) defined elsewhere in the
	resource file. The nWidth and nHeight parameters are ignored; the icon
	automatically sizes itself.

	SS_LEFT - Designates a simple rectangle and displays the given text
	flush-left in the rectangle. The text is formatted before it is
	displayed. Words that would extend past the end of a line are
	automatically wrapped to the beginning of the next flush-left line.

	SS_LEFTNOWORDWRAP - Designates a simple rectangle and displays the
	given text flush-left in the rectangle. Tabs are expanded, but words
	are not wrapped. Text that extends past the end of a line is clipped.

	SS_NOPREFIX - Unless this style is specified, Windows will interpret
	any ampersand (&) characters in the controlís text to be accelerator
	prefix characters. In this case, the ampersand (&) is removed and the
	next character in the string is underlined. If a static control is to
	contain text where this feature is not wanted, SS_NOPREFIX may be added.
	This static-control style may be included with any of the defined
	static controls. You can combine SS_NOPREFIX with other styles by using
	the bitwise OR operator. This is most often used when filenames or
	other strings that may contain an ampersand (&) need to be displayed in
	a static control in a dialog box.

	SS_RIGHT - Designates a simple rectangle and displays the given text
	flush-right in the rectangle. The text is formatted before it is
	displayed. Words that would extend past the end of a line are
	automatically wrapped to the beginning of the next flush-right line.

	SS_SIMPLE - Designates a simple rectangle and displays a single line
	of text flush-left in the rectangle. The line of text cannot be
	shortened or altered in any way. (The controlís parent window or
	dialog box must not process the WM_CTLCOLOR message.)

	SS_USERITEM - Specifies a user-defined item.

	SS_WHITEFRAME - Specifies a box with a frame drawn with the same color
	as the window background. The default is white.

	SS_WHITERECT - Specifies a rectangle filled with the color used to
	fill the window background. The default is white.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	rect,
						VUINT			nID,
						VSTRING_CONST	pszText = NULL,
						VUINT			nStyle = WS_VISIBLE | SS_LEFT)
	{
		VASSERT(windowParent.GetSafeWindow())

		return VWindow::Create(	windowParent,
								rect,
								WS_CHILD | nStyle,
								VTEXT("STATIC"),
								pszText,
								nID);
	}

	/** Retrieve a handle to the icon associated with a static control that
	has the SS_ICON style. The return value is a handle to the icon, or
	NULL if either the static control has no associated icon or if an error
	occurred.*/
	HICON		Icon() const
	{
		/* The SS_ICON style is required to contain icons.*/
		VASSERT(Style() & SS_ICON)

		return (HICON)VWINDOW_SM0(STM_GETICON);
	}

	/** Associate an icon with an icon static control which has the SS_ICON
	style. The return value is a handle to the icon previously associated
	with the icon control, or zero if an error occurs.*/
	HICON		Icon(HICON hIcon) const
	{
		/* The SS_ICON style is required to contain icons.*/
		VASSERT(Style() & SS_ICON)

		return (HICON)VWINDOW_SM1(STM_SETICON, hIcon);
	}

	/** Retrieve a handle to the image associated with a static control.
	nImageType specifies the type of image to retrieve. This parameter can
	be one of the following values: IMAGE_BITMAP, IMAGE_CURSOR,
	IMAGE_ENHMETAFILE, or IMAGE_ICON. The return value is a handle to the
	image, if any; otherwise, it is NULL.*/
	HANDLE		Image(VUINT nImageType = IMAGE_BITMAP) const
		{ return (HANDLE)VWINDOW_SM1(STM_GETIMAGE, nImageType); }

	/** Associate a new image (icon or bitmap) with a static control. See
	above Image() function for information on possible values for
	nImageType. hImage is a handle to the image to associate with the
	static control. The return value is a handle to the image previously
	associated with the static control, if any; otherwise, it is NULL.*/
	HANDLE		Image(	HANDLE	hImage,
						VUINT	nImageType =
									IMAGE_BITMAP) const
		{ return (HANDLE)VWINDOW_SM2(STM_SETIMAGE, nImageType, hImage); }
};

#endif /* VSTATIC*/
