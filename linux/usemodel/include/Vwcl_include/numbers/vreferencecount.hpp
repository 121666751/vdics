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

#ifndef VREFERENCECOUNT
#define VREFERENCECOUNT

#include "../vstandard.h"

/** VReferenceCount maintains a generic reference count, useful for object
usage count tracking.*/
class VReferenceCount
{
public:
	/** Default constructor initializes with an object count of 0, unless
	nStartingCount is specified.*/
	VReferenceCount(VULONG nStartingCount = 0)
		{ Reset(nStartingCount); }

	/** Virtual destructor does nothing.*/
	virtual ~VReferenceCount()
		{;}

	/** Add 1 to the current reference count.
	Returns the new reference count.*/
	VULONG	Add()
		{ return ++m_nReferenceCount; }

	/** Return a reference to the current reference count.*/
	VULONG&	Count() const
		{ return (VULONG&)m_nReferenceCount; }

	/** Return a reference to the internal count variable.*/
	VULONG&	GetCount() const
		{ return (VULONG&)m_nReferenceCount; }

	/** Reset the count to nStartingPoint.*/
	void	Reset(VULONG nStartingCount = 0)
		{ m_nReferenceCount = nStartingCount; }

	/** Subtract 1 from the current reference count.
	Returns the new reference count.*/
	VULONG	Subtract()
	{
		VASSERT(m_nReferenceCount > 0)
		return --m_nReferenceCount;
	}

protected:
	/** Embedded Members.*/
	VULONG	m_nReferenceCount;
};

#endif /* VREFERENCECOUNT*/
