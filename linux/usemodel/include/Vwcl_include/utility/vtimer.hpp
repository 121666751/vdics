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

#ifndef VTIMER
#define VTIMER

#include "../vstandard.h"

/** VTimer is a simple object for keeping track of elapsed time.*/
class VTimer
{
public:
	/** Default constructor initializes the starting time.*/
	VTimer()
		{ Reset(); }

	/** Virtual destructor does nothing.*/
	virtual ~VTimer()
		{;}

	/** Return the elapsed time since object construction or last call to
	Reset(). The return value is number of seconds elapsed. If milliseconds
	are also desired, pass a non-NULL value for pnMilliseconds.*/
	VUINT	GetElapsedTime(VUINT* pnMilliseconds = NULL) const
	{
		struct timeb timeNow;
		::ftime(&timeNow);

		/* Does caller want milliseconds?*/
		if ( pnMilliseconds )
		{
			/* Add 1000 if now ms is less than original.*/
			if ( timeNow.millitm < m_timeStart.millitm )
				timeNow.millitm += 1000;

			*pnMilliseconds = timeNow.millitm - m_timeStart.millitm;
		}

		return timeNow.time - m_timeStart.time;
	}

	/** Reset start time to current time.*/
	void	Reset()
		{ ::ftime(&m_timeStart); }

protected:
	/** Embedded Members.*/
	struct timeb	m_timeStart;
};

#endif /* VTIMER*/
