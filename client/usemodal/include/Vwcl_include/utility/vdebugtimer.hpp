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

#ifndef VDEBUGTIMER
#define VDEBUGTIMER

#include "../vstandard.h"
#include "../strings/vnumberstring.hpp"
#include "../strings/vstring.hpp"
#include "vtimer.hpp"

/** VDebugTimer shows a message box, or writes output to VODS() for console
apps, displaying elapsed time when destructed. The macro
VDEBUG_ELAPSED_TIME is used to instantiate an object of this type. In
release builds the macro compiles to nothing. This is useful for timing
function calls and code tuning. When using this macro, do not place a
semicolon after the macro name. This would leave a bogus extra semicolon in
release builds, causing some compilers to have problems.*/
class VDebugTimer : public VTimer
{
public:
	/** Destructor shows elapsed time using VODS() or MessageBox(),
	depending on the target application.*/
	~VDebugTimer()
	{
		VUINT nMS;

		VString s(VTEXT("Elapsed Time is: "));
		VNumberString ns(s);
		
		ns += GetElapsedTime(&nMS);
		s += VTEXT(" second(s) and ");
		ns += nMS;
		s += VTEXT(" Milliseconds.");

		#ifdef VWCL_CONSOLE_APP
			VODS(s.String())
		#else
			#ifndef VWCL_NO_WIN32_API
				MessageBox(	GetActiveWindow(),
							s,
							VTEXT("VDebugTimer"),
							MB_ICONINFORMATION);
			#else
				VODS(s.String())
			#endif
		#endif
	}
};

/** VDEBUG_ELAPSED_TIME is a debug build only macro that creates a
VDebugTimer object which shows a messagebox, or writes output to VODS() for
console apps, displaying elapsed time when destructed. In release builds the
macro compiles to nothing. This is useful for timing function calls and
code tuning. When using this macro, do not place a semicolon after the
macro name. This would leave a bogus extra semicolon in release builds,
causing some compilers to have problems.*/
#ifdef VWCL_DEBUG
	#define VDEBUG_ELAPSED_TIME VDebugTimer vwcl_DEBUG_TIMER;
#else
	#define VDEBUG_ELAPSED_TIME
#endif

#endif /* VDEBUGTIMER*/
