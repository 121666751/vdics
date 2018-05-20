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

#ifndef VWCLHPP
#define VWCLHPP

/* This header file should NOT be considered a true part of VWCL. It is
really only an aid to development. When creating new projects that are
either based in VWCL, or use VWCL, it may be easier to include only this
file in your project. It will include EVERYTHING VWCL has to offer, and
may result in your application being bloated, or requiring you to link
with libraries that are not really needed. However, it will still only
include support for all VWCL classes under certain conditions. The family
header files will honor the preprocessor directives in vstandard.h, and
will, for instance, exclude things like windows and other GUI related
classes when a console application is being built. Once you determine your
applications true need of specific VWCL classes, you should remove your
reference to this header and substitute the headers you actually use.
Since VWCL classes always include headers they require, you only need to
look at your source code (not VWCL's) and include the VWCL classes you
actually use. VWCL will take care of including classes those classes need.
NOTE: This header does not include the 3rd Party files contained in
vfamily_3rd_party.hpp, since most developers do not have the header files
required to compile this. If you do, you should uncomment this include
statement.*/

#ifndef __cplusplus
	#error	Inclusion of vwcl.hpp requires a C++ compiler! \
			Only vstandard.h does not.
#endif

#include "vstandard.h"

/* Include common controls first, since VWCL classes may conditionally
compile support for them.*/
#include "vfamily_common_controls.hpp"
#include "vfamily_activex.hpp"
#include "vfamily_collections.hpp"
#include "vfamily_common_dialogs.hpp"
#include "vfamily_gdi.hpp"
#include "vfamily_html.hpp"
#include "vfamily_input_output.hpp"
#include "vfamily_networking.hpp"
#include "vfamily_numbers.hpp"
#include "vfamily_parsers.hpp"
#include "vfamily_splitters.hpp"
#include "vfamily_strings.hpp"
#include "vfamily_structures.hpp"
#include "vfamily_threading.hpp"
#include "vfamily_utility.hpp"
#include "vfamily_windows.hpp"
#include "vfamily_windows_controls.hpp"
#include "vfamily_windows_nt.hpp"

#endif /* VWCLHPP*/
