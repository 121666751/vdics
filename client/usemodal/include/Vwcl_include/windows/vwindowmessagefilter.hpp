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

#ifndef VWINDOWMESSAGEFILTER
#define VWINDOWMESSAGEFILTER

#include "../vstandard.h"
#include "../collections/vlinkedlistitem.hpp"
#include "../numbers/vreferencecount.hpp"

/* Forward declarations.*/
class VWindow;

/** VWindowMessageFilter is used to inspect messages sent to a VWindow
object before that object has a chance itself to handle the message. It is
not derived from VWindow, so it can be used any time message routing from a
VWindow object to an arbtitrary object of another type is needed. See the
OnMessageFilter() method and VWindow::InstallMessageFilter() for more
information.*/
class VWindowMessageFilter : private VLinkedListItem
{
friend class VWindow;

public:
	/** Destructor implemented in vwindow.cpp.*/
	~VWindowMessageFilter();

protected:
	/** See VWindow::InstallMessageFilter() for more information. An override
	of this function is getting messages sent to it BEFORE its associated
	VWindow. This is essentially an override of VWindow::WindowProc() but is
	kept separate to avoid confusion. If the message filter completely
	handles the message, and the associated VWindow should not be allowed
	to process the message, you should return VFALSE. Returning VTRUE
	allows the associated VWindow to handle the message. If the return value
	sent back to Windows is significant, you must set the nResult parameter
	to the significant value, AND set the bUseResult to VTRUE. This is a
	pure virtual function that must be overriden in a derived class.
	IT IS NOT VALID for an override of this function to delete a VWindow
	object, whether the window parameter or another. This message should be
	considered a notification only, not a time to respond by deleting
	VWindow objects. It is valid to remove this filter from a VWindow
	during this notification.*/
	virtual VBOOL	OnMessageFilter(	VWindow&	window,
										VUINT		nMessage,
										VWPARAM		wParam,
										VLPARAM		lParam,
										VLONG&		nResult,
										VBOOL&		bUseResult) = 0;

private:
	/** Used only by VWindow internally.*/
	VReferenceCount	m_ReferenceCount;
};

#endif	/* VWINDOWMESSAGEFILTER*/
