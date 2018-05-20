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

#ifndef VCHECKBOX
#define VCHECKBOX

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vbutton.hpp"

/** VCheckBox encapsulates the standard Windows checkbox control.*/
class VCheckBox : public VButton
{
public:
	/** Default constructor initializes base class RTTI information.*/
	VCheckBox(VRTTI_TYPE nRTTI = VWCL_RTTI_CHECKBOX)
		: VButton(nRTTI)
		{;}

	/** Set the check state of a radio button or check box. See
	GetCheck() for possible values for nCheck.*/
	void		Check(VUINT nCheck = BST_CHECKED) const
		{ VWINDOW_SM1(BM_SETCHECK, nCheck); }

	/** Create a new checkbox control. See VButton::Create() for more
	information on possible style bits.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	r,
						VUINT			nID,
						VSTRING_CONST	pszText =
											NULL,
						VUINT			nStyle =
											WS_BORDER |
											WS_TABSTOP |
											WS_VISIBLE |
											BS_AUTOCHECKBOX)
		{ return VButton::Create(windowParent, r, nID, pszText, nStyle); }

	/** Retrieve the check state of the radio button or check box. The
	return value from a button created with the BS_AUTOCHECKBOX,
	BS_AUTORADIOBUTTON, BS_AUTO3STATE, BS_CHECKBOX, BS_RADIOBUTTON, or
	BS_3STATE style can be one of the following:

	BST_CHECKED - Button is checked.

	BST_INDETERMINATE - Button is grayed, indicating an indeterminate
	state (applies only if the button has the BS_3STATE or BS_AUTO3STATE
	style).

	BST_UNCHECKED - Button is unchecked.

	If the button has any other style, the return value is zero.*/
	VLONG		GetCheck() const
		{ return VWINDOW_SM0(BM_GETCHECK); }

	/** Same as GetCheck(), but returns VTRUE if checked, VFALSE if not.*/
	VBOOL		IsChecked() const
		{ return (GetCheck() == BST_CHECKED) ? VTRUE : VFALSE; }
};

#endif /* VCHECKBOX*/
