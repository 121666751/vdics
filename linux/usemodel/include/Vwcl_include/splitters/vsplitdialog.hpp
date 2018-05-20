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

#ifndef VSPLITDIALOG
#define VSPLITDIALOG

#include "../vstandard.h"
#include "../gdi/vdc.hpp"
#include "../numbers/vrtti.hpp"
#include "../windows/vdialog.hpp"
#include "vsplitsupport.hpp"

/** VSplitDialog is a dialog box with split window support. It derives its
functionality from both VDialog and VSplitSupport, and implements the
VSplitSupport macro's to route messages to this class. See VDialog and
VSplitSupport documentation for more information.*/
class VSplitDialog : public VDialog, public VSplitSupport
{
public:
	/** Default constructor initializes RTTI information and
	split support options in base classes.*/
	VSplitDialog(	VUINT		nSplitSupportOptions =
									VSplitSupport::OPTION_DEFAULT,
					VRTTI_TYPE	nRTTI =
									VWCL_RTTI_SPLIT_DIALOG)
		: VDialog(nRTTI), VSplitSupport(nSplitSupportOptions)
		{;}

protected:
	/** Macros implement message mapping to VSplitSupport base class.*/
	VWCL_SPLIT_ON_SIZE(VDialog)
	VWCL_SPLIT_WINDOW_PROC(VDialog)
};

#endif /* VSPLITDIALOG*/
