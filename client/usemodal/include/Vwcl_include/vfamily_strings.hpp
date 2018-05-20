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

#ifndef VFAMILYSTRINGS
#define VFAMILYSTRINGS

#include "vstandard.h"

/* This header includes all VWCL classes that are strings or string related,
but that are not also parsers.*/
#include "strings/vpathstring.hpp"
#include "strings/vnumberstring.hpp"
#include "strings/vstring.hpp"
#include "strings/vstringconverter.hpp"
#include "strings/vstringmanipulator.hpp"
#ifndef VWCL_NO_WIN32_API
	#include "strings/vresourcestring.hpp"
	#include "strings/vwindowstring.hpp"
#endif

#endif /* VFAMILYSTRINGS*/
