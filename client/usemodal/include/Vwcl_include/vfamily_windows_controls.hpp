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

#ifndef VFAMILYWINDOWSCONTROLS
#define VFAMILYWINDOWSCONTROLS

#include "vstandard.h"

/* This header includes all VWCL classes that support the standard
Windows controls.*/
#if !defined(VWCL_TARGET_CONSOLE) && !defined(VWCL_NO_WIN32_API)
	#include "windows_controls/vbutton.hpp"
	#include "windows_controls/vcheckbox.hpp"
	#include "windows_controls/vclosexbutton.hpp"
	#include "windows_controls/vcombobox.hpp"
	#include "windows_controls/vedit.hpp"
	#include "windows_controls/vformattededit.hpp"
	#include "windows_controls/vgroupbox.hpp"
	#include "windows_controls/vlistbox.hpp"
	#include "windows_controls/vmenu.hpp"
	#include "windows_controls/vradiobutton.hpp"
	#include "windows_controls/vscrollbar.hpp"
	#include "windows_controls/vstatic.hpp"
#endif

#endif /* VFAMILYWINDOWSCONTROLS*/
