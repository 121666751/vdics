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

#ifndef VFAMILYCOMMONDIALOGS
#define VFAMILYCOMMONDIALOGS

#include "vstandard.h"

/* This header includes all VWCL classes that implement a Windows
Common Dialog box.*/
#if !defined(VWCL_TARGET_CONSOLE) && !defined(VWCL_NO_WIN32_API)
	#include "common_dialogs/vbrowseforfolderdialog.hpp"
	#include "common_dialogs/vcolordialog.hpp"
	#include "common_dialogs/vcommondialog.hpp"
	#include "common_dialogs/vfiledialog.hpp"
	#include "common_dialogs/vfindreplacedialog.hpp"
	#include "common_dialogs/vfontdialog.hpp"
	#include "common_dialogs/vpagesetupdialog.hpp"
	#include "common_dialogs/vprintdialog.hpp"
#endif

#endif /* VFAMILYCOMMONDIALOGS*/
