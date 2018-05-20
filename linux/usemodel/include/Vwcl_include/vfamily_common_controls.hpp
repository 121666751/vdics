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

#ifndef VFAMILYCOMMONCONTROLS
#define VFAMILYCOMMONCONTROLS

#include "vstandard.h"

/* This header includes all VWCL classes that are based on a Windows
(Win32) Common Controls.*/
#if !defined(VWCL_TARGET_CONSOLE) && !defined(VWCL_NO_WIN32_API)
	#include "common_controls/vcommoncontrol.hpp"
	#include "common_controls/vdatetimecontrol.hpp"
	#include "common_controls/vdirectorytreeview.hpp"
	#include "common_controls/vheader.hpp"
	#include "common_controls/vimagelist.hpp"
	#include "common_controls/vlistview.hpp"
	#include "common_controls/vmonthcalendarcontrol.hpp"
	#include "common_controls/vprogress.hpp"
	#include "common_controls/vpropertypage.hpp"
	#include "common_controls/vpropertysheet.hpp"
	#include "common_controls/vrebar.hpp"
	#include "common_controls/vstatusbar.hpp"
	#include "common_controls/vtab.hpp"
	#include "common_controls/vtabwindow.hpp"
	#include "common_controls/vtoolbar.hpp"
	#include "common_controls/vtooltip.hpp"
	#include "common_controls/vtreeview.hpp"
	#include "common_controls/vupdowncontrol.hpp"
#endif

#endif /* VFAMILYCOMMONCONTROLS*/
