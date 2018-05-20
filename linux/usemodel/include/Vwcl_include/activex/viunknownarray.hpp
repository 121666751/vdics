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

#ifndef VIUNKNOWNARRAY
#define VIUNKNOWNARRAY

#include "../vstandard.h"
#include "../collections/vptrarray.hpp"
#include "vunknown.hpp"

/** VIUnknownArray collects open COM object pointers. Added objects will be
released, in REVERSE ORDER, in ReleaseAll(), or when the VIUnknownArray
object is destructed. The thinking behind this class is simple. Many COM
objects return pointers to other COM objects. Many times to get a pointer
to a COM object you have to dig through multiple objects, getting pointers
to each one. When you are finally done with all of them, they must be
released, usually in the same order you retreived them. If an error occurs
anywhere in the chain, you still have to release the pointers you did
received. This class takes care of that. Simply call Add() each time you
get a pointer from a COM object. If your code encounters an error along
the way, you will not have to call Release() on each object. When this
object is destructed, all added COM objects will be released. This is a
collection of IUnknown object pointers, not the VWCL VUnknown class
object pointers. However, you can add VUnknown objects to this collection
by calling VUnknown::GetUnknown().*/
class VIUnknownArray
{
public:
	/** Virtual destructor verifies all contained unknown
	objects are released.*/
	virtual ~VIUnknownArray()
		{ ReleaseAll(); }

	/** Same as Add().*/
	VPTRARRAY_INDEX operator += (IUnknown const* pUnknown)
		{ return Add(pUnknown); }

	/** Add a new object at the end of the array. Returns index into
	array on success, -1 on failure.*/
	VPTRARRAY_INDEX	Add(IUnknown const* pUnknown)
	{
		VASSERT(pUnknown)
		return m_listObjects.Add((VVOIDPTR)pUnknown);
	}

	/** Return object at given index.*/
	IUnknown*		At(VPTRARRAY_INDEX nIndex) const
	{
		VASSERT(m_listObjects.At(nIndex))
		return (IUnknown*)m_listObjects.At(nIndex);
	}

	/** Return a reference to the internal VPtrArray object.*/
	VPtrArray&		GetCollection() const
		{ return (VPtrArray&)m_listObjects; }

	/** Release all objects in REVERSE ORDER, and remove from collection.*/
	void			ReleaseAll()
	{
		VPTRARRAY_INDEX nSize = m_listObjects.Size();

		while ( --nSize >= 0 )
			At(nSize)->Release();

		m_listObjects.RemoveAll();
	}

protected:
	/** Embedded Members.*/
	VPtrArray	m_listObjects;
};

#endif /* VIUNKNOWNARRAY*/
