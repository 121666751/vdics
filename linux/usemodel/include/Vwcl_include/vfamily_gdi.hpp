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

#ifndef VFAMILYGDI
#define VFAMILYGDI

#include "vstandard.h"

/* This header includes all VWCL classes that implement some type interaction
with the Windows Graphics Device Interface (GDI).*/
#if !defined(VWCL_TARGET_CONSOLE) && !defined(VWCL_NO_WIN32_API)
	#include "gdi/v3drect.hpp"
	#include "gdi/vanimatedbitmap.hpp"
	#include "gdi/vbitmap.hpp"
	#include "gdi/vbuffereddc.hpp"
	#include "gdi/vbrush.hpp"
	#include "gdi/vclientdc.hpp"
	#include "gdi/vdc.hpp"
	#include "gdi/vdcwithwindow.hpp"
	#include "gdi/vfont.hpp"
	#include "gdi/vgauge.hpp"
	#include "gdi/vgdiobject.hpp"
	#include "gdi/vgdiobjectselector.hpp"
	#include "gdi/vgdistockobject.hpp"
	#include "gdi/vlinetracker.hpp"
	#include "gdi/vpaintdc.hpp"
	#include "gdi/vpalette.hpp"
	#include "gdi/vpen.hpp"
	#include "gdi/vrecttracker.hpp"
	#include "gdi/vstockbrush.hpp"
	#include "gdi/vstockfont.hpp"
	#include "gdi/vstockpalette.hpp"
	#include "gdi/vstockpen.hpp"
	#include "gdi/vtempdc.hpp"
	#include "gdi/vtempgdiobject.hpp"
	#include "gdi/vwindowdc.hpp"
#endif

#endif /* VFAMILYGDI*/
