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

#ifndef VENUMSTATSTG
#define VENUMSTATSTG

#include "../vstandard.h"
#include "vunknown.hpp"
#include "vactivexenum.hpp"

/** VEnumSTATSTG is an implementation of the IEnumSTATSTG interface which is
used to enumerate through an array of STATSTG structures, which contain
statistical information about an open storage, stream, or byte array object.
VEnumSTATSTG has the same methods as all enumerator interfaces:
Next(), Skip)(), Reset(), and Clone(). For general information on these
methods, refer to IEnumXXX in the Win32 documentation. IEnumSTATSTG is
implemented to enumerate the elements of a storage object. If you are using
the compound file implementation of the storage object, a pointer to which
is available through a call to StgCreateDocfile(), IEnumSTATSTG is
implemented on that object, and a pointer is returned through a call to
IStorage::EnumElements(). If you are doing a custom implementation of a
storage object, you need to implement IEnumSTATSTG to fill in a
caller-allocated array of STATSTG structures, each of which contains
information about the nested elements in the storage object. It is
important to note that this class makes a private copy of the STATSTG
struct(s) that are passed to the Create() function of this class. This
ensures the data will be available at least as long as this object is in
scope. It also means that client code does not have to keep the STATSTG
structs available in memory. It is perfectly valid to initialize this object
with STATSTG struct(s) on the stack, and then essentially forget about them,
leaving their lifetime managed by this class.*/
class VEnumSTATSTG : public IEnumSTATSTG, public VActiveXEnum
{
public:
	/** Create and initialize an instance of this object. If a memory error
	occurs, NULL will be returned. pSTATSTGs must be a valid pointer to a
	STATSTG struct, or a pointer to an array of them. nCountOfElements is
	the number of STATSTG structs pointed to by pSTATSTGs, if this
	represents an array. The STATSTG structs must already have been
	initialized by client code, and are copied into the new objects
	private data.*/
	static VEnumSTATSTG*	Create(	STATSTG*	pSTATSTGs,
									VULONG		nCountOfElements = 1)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CREATE_NON_OBJECT(VEnumSTATSTG, pSTATSTGs)
	}

	/** Implement IUnknown methods.*/
	VWCL_ACTIVEX_IMPLEMENT_IUNKNOWN_METHODS(VUnknown)

	/** Creates another enumerator that contains the same enumeration state
	as the current one. Using this function, a client can record a
	particular point in the enumeration sequence, and then return to that
	point at a later time. The new enumerator supports the same interface
	as the original one.*/
	STDMETHODIMP			Clone(IEnumSTATSTG** ppIEnumSTATSTG)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CLONE_NON_OBJECT(VEnumSTATSTG, m_pSTATSTGs, ppIEnumSTATSTG)
	}

	/** Retrieves the next nCountOfElements items in the enumeration
	sequence. If there are fewer than the requested number of elements left
	in the sequence, it retrieves the remaining elements. The number of
	elements actually retrieved is returned through pnCountOfElementsFetched
	(unless the caller passed in NULL for that parameter). pSTATSTGs must
	be a valid pointer to a STATSTG struct, or a pointer to an array of
	them. nCountOfElements is the number of STATSTG structs pointed to by
	pSTATSTGs, if this represents an array. The STATSTG structs do not need
	to have been initialized by client code as they are initialized by this
	function.*/
	STDMETHODIMP			Next(	VULONG		nCountOfElements,
									STATSTG*	pSTATSTGs,
									VULONG*		pnCountOfElementsFetched)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_NEXT_NON_OBJECT(m_pSTATSTGs, pSTATSTGs, sizeof(STATSTG))
		return hResult;
	}

	/** Resets the enumeration sequence to the beginning. COM SPEC: A call
	to this method, resetting the sequence, does not guarantee that the
	same set of objects will be enumerated after the reset, because it
	depends on the collection being enumerated. A static collection is
	reset to the beginning, but it can be too expensive for some
	collections, such as files in a directory, to guarantee this condition.*/
	STDMETHODIMP			Reset()
		{ return VActiveXEnum::Reset(); }

	/** Skips over the next nCountOfElements elements in the enumeration
	sequence. Return S_OK if the number of elements skipped is
	nCountOfElements, otherwise S_FALSE.*/
	STDMETHODIMP			Skip(VULONG nCountOfElements)
		{ return VActiveXEnum::Skip(nCountOfElements); }

protected:
	/** Protected constructor used for cloning and private initialization.*/
	VEnumSTATSTG(	STATSTG*	pSTATSTGs,
					VULONG		nCountOfElements,
					VULONG		nCurrentIndex)
		: VActiveXEnum(IID_IEnumSTATSTG, nCountOfElements, nCurrentIndex)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CONSTRUCTOR_NON_OBJECT(m_pSTATSTGs, STATSTG, pSTATSTGs)

		/* A deep copy of a STATSTG struct requires that we also
		copy STATSTG::pwcsName.*/
		if ( !IsInErrorState() )
		{
			for ( VULONG i = 0; i < nCountOfElements; i++ )
			{
				if ( pSTATSTGs[i].pwcsName )
				{
					m_pSTATSTGs[i].pwcsName =
						VNEW VCHARW[VSTRLENW(pSTATSTGs[i].pwcsName) + 1];

					if ( m_pSTATSTGs[i].pwcsName )
						VSTRCPYW(	m_pSTATSTGs[i].pwcsName,
									pSTATSTGs[i].pwcsName);
					else
					{
						Error(VTRUE);
						return;
					}
				}
			}
		}
	}

	/** Protected destructor free's class allocated STATSTG array
	and associated memory.*/
	virtual ~VEnumSTATSTG()
	{
		/* Free allocated STATSTG::pwcsName memory.*/
		for ( VULONG i = 0; i < m_nCountOfElements; i++ )
			VDELETE_NULL(m_pSTATSTGs[i].pwcsName)

		/* Free allocated STATSTG structs.*/
		VWCL_ACTIVEX_ENUM_IMPLEMENT_DESTRUCTOR_NON_OBJECT(m_pSTATSTGs)
	}

	/** Array of STATSTG structures that we copy and own.*/
	STATSTG*				m_pSTATSTGs;
};

#endif /* VENUMSTATSTG*/
