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

#ifndef VFAMILYACTIVEX
#define VFAMILYACTIVEX

#include "vstandard.h"

/* This header includes all VWCL classes that implement ActiveX (COM)
interfaces, or aid the implementaion of ActiveX development.*/
#ifndef VWCL_NO_WIN32_API
	#include "activex/vactivexenum.hpp"
	#include "activex/vactivexinitializer.hpp"
	#include "activex/vactivexregistrar.hpp"
	#include "activex/vautomationobject.hpp"
	#include "activex/vclassfactory.hpp"
	#include "activex/venumformatetc.hpp"
	#include "activex/venummoniker.hpp"
	#include "activex/venumoleverb.hpp"
	#include "activex/venumstatdata.hpp"
	#include "activex/venumstatstg.hpp"
	#include "activex/venumunknown.hpp"
	#include "activex/venumvariant.hpp"
	#include "activex/viunknownarray.hpp"
	#include "activex/vssobjecthelper.hpp"
	#include "activex/vssstoragehelper.hpp"
	#include "activex/vssstreamhelper.hpp"
	#include "activex/vunknown.hpp"
#endif

#endif /* VFAMILYACTIVEX*/
