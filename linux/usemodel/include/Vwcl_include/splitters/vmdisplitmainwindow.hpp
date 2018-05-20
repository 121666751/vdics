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

#ifndef VMDISPLITMAINWINDOW
#define VMDISPLITMAINWINDOW

#include "../vstandard.h"
#include "../windows/vmdimainwindow.hpp"
#include "vsplitsupport.hpp"

/** VMDISplitMainWindow is a main application window with split window
support. It supports displaying Multiple Document Interface (MDI)
child windows in one of the panes. It derives its functionality from
both VMDIMainWindow and VSplitSupport, and implements the VSplitSupport
macro's to route messages to this class. See VMDIMainWindow and
VSplitSupport documentation for more information.*/
class VMDISplitMainWindow : public VMDIMainWindow, public VSplitSupport
{
public:
	/** Default constructor initializes RTTI information and
	split support options in base classes.*/
	VMDISplitMainWindow(VUINT		nSplitSupportOptions =
										VSplitSupport::OPTION_DEFAULT,
						VRTTI_TYPE	nRTTI =
										VWCL_RTTI_MDI_SPLIT_MAIN_WINDOW)
		: VMDIMainWindow(nRTTI), VSplitSupport(nSplitSupportOptions)
		{;}

protected:
	/** Macros implement message mapping to VSplitSupport base class.*/
	VWCL_SPLIT_WINDOW_PROC(VMDIMainWindow)

	/** VMDIMainWindow override sets client rectangle in VSplitSupport to r
	and calls VSplitSupport::RepositionChildren() to re-arrange the
	position of attached windows.*/
	virtual void	RecalcLayout(VRect rect)
	{
		SetClientRectangle(VRECT_BREAK(rect));
		RepositionChildren(VREF_THIS);
	}
};

#endif /* VMDISPLITMAINWINDOW*/
