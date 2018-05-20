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

Primary Author of this source code file:	Simon Capewell
											(simonc@bristol.crosswinds.net)
Other Author(s) of this source code file:	Todd Osborne
											(todd@vwcl.org)
*/

#ifndef VCOMMONDIALOG
#define VCOMMONDIALOG

#include "../vstandard.h"
#include "../windows/vdialog.hpp"

/** VCommonDialog is the base class for other common dialog classes.*/
class VCommonDialog : public VDialog
{
protected:
	/** Protected constructor, used only by derived classes, sets runtime
	type information and modal flag.*/
	VCommonDialog(VRTTI_TYPE nRTTI)
		: VDialog(nRTTI)
		{ m_bModal = VTRUE; }

	/** Override verifies that VDialog management of buttons like
	IDOK and IDCANCEL are not used.*/
	virtual VLONG	OnCommand(	VUINT	nNotifyCode,
								VUINT	nCommandID,
								HWND	hWndControl)
		{ return VWINDOW_MESSAGE_NOT_HANDLED; }
};

#endif /* VCOMMONDIALOG*/
