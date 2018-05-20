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

#ifndef VRADIOBUTTON
#define VRADIOBUTTON

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vcheckbox.hpp"

/** VRadioButton encapsulates the standard Windows radio button control.*/
class VRadioButton : public VCheckBox
{
public:
	/** Default constructor initializes RTTI information in base classes.*/
	VRadioButton(VRTTI_TYPE nRTTI = VWCL_RTTI_RADIO_BUTTON)
		: VCheckBox(nRTTI)
		{;}

	/** Create a new radiobutton control. See VButton::Create() for more
	information on possible style bits.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	rect,
						VUINT			nID,
						VSTRING_CONST	pszText =
											NULL,
						VUINT			nStyle =
											WS_BORDER |
											WS_TABSTOP |
											WS_VISIBLE |
											BS_AUTORADIOBUTTON)
		{ return VButton::Create(windowParent, rect, nID, pszText, nStyle); }
};

#endif /* VRADIOBUTTON*/
