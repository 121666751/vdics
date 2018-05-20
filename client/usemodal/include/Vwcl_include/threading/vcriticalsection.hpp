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

#ifndef VCRITICALSECTION
#define VCRITICALSECTION

#include "../vstandard.h"

/** Prototype for TryEnterCriticalSection(). This function is resolved at
runtime to determine if the OS supports it.*/
typedef VWINBOOL
	(WINAPI* VTRY_ENTER_CRITICAL_SECTION)(LPCRITICAL_SECTION pCriticalSection);

/** VCriticalSection provides thread synchronization within the same process.
It is a simple wrapper around the Win32 critical section functions. Please
see the appropriate Win32 thread syncronization functions for more
information about critical sections and other syncronization methods.*/
class VCriticalSection
{
public:
	/** Default constructor initializes the critical section and
	internal member variables.*/
	VCriticalSection()
	{
		m_pfnTryEnterCriticalSection = (VTRY_ENTER_CRITICAL_SECTION)1;
		InitializeCriticalSection(&m_CriticalSection);
	}

	/** Virtual destructor deletes the internal critical section object.*/
	virtual ~VCriticalSection()
		{ DeleteCriticalSection(&m_CriticalSection); }

	/** Enter the critical section. This acquires the lock on the object,
	providing access to the protected code by only one thread in the
	process. This function will wait until another thread that has the
	critical section locked releases its lock. If no other thread has the
	critical section locked, this function will return immediately.*/
	void	Enter()
		{ EnterCriticalSection(&m_CriticalSection); }

	/** Leave the critical section, making it available for entry by another
	thread.*/
	void	Leave()
		{ LeaveCriticalSection(&m_CriticalSection); }

	/** Try to enter the critical section, returning immediately if not
	possible. If bWaitToEnterIfNeeded is VTRUE, then Enter() will be called
	before returning. This function is only supported when the underlying OS
	supports the feature. If the OS does not, this function acts like
	Enter() if bWaitToEnterIfNeeded is VTRUE, otherwise it returns VFALSE.
	In all cases, a VTRUE return means the critical section was entered,
	VFALSE means it was not.*/
	VBOOL	TryEnter(VBOOL bWaitToEnterIfNeeded = VFALSE)
	{
		/* Resolve function pointer if set to 1.*/
		if ( m_pfnTryEnterCriticalSection == (VTRY_ENTER_CRITICAL_SECTION)1 )
			m_pfnTryEnterCriticalSection = (VTRY_ENTER_CRITICAL_SECTION)
				GetProcAddress(	GetModuleHandle(VTEXT("KERNEL32")),
								"TryEnterCriticalSection");

		/* Use TryEnterCriticalSection() if available.*/
		if ( m_pfnTryEnterCriticalSection )
			return	(m_pfnTryEnterCriticalSection(&m_CriticalSection))
					? VTRUE : VFALSE;
		else if ( bWaitToEnterIfNeeded )
		{
			Enter();
			return VTRUE;
		}

		return VFALSE;
	}

protected:
	/** Embedded Members.*/
	CRITICAL_SECTION			m_CriticalSection;
	VTRY_ENTER_CRITICAL_SECTION	m_pfnTryEnterCriticalSection;
};

#endif	/* VCRITICALSECTION*/
