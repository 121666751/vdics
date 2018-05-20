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

#ifndef VMEMORYBLOCK
#define VMEMORYBLOCK

#include "../vstandard.h"

/** VMemoryBlock encapsulates a dynamically allocated block of memory.*/
class VMemoryBlock
{
public:
	/** Default constructor can allocate a block of memory of the
	byte size specified by nLength.*/
	VMemoryBlock(VUINT nLength = 0)
		{ Init(nLength, NULL); }

	/** Construct object with existing VMemoryBlock object
	(copy constructor). This will allocate new memory and copy the
	memory from the existing object.*/
	VMemoryBlock(VMemoryBlock const& existing)
		{ Init(0, (VMemoryBlock*)&existing); }

	/** Virtual Destructor verifies internal memory is deallocated.*/
	virtual ~VMemoryBlock()
		{ Free(); }

	/** Allocate memory. If memory is currently allocated by this object,
	it will be free'd. On failure, any previous memory held by this object
	is still safe and valid. Returns a pointer to the memory on success,
	NULL on failure. If nLength is 0, this will have the same affect as
	calling Free(). If memory is allocated, it will be 0 filled.*/
	VSTRINGA		Alloc(VUINT nLength)
	{
		if ( !nLength )
		{
			Free();
			return NULL;
		}

		VSTRINGA pBlock = (VSTRINGA)malloc(nLength);

		if ( pBlock )
		{
			Free();
			m_pBlock =	pBlock;
			m_nLength =	nLength;
			VZEROMEMORY(m_pBlock, m_nLength);
		}

		return pBlock;
	}

	/** Same as Alloc(), but performs a memory allocation for strings. That
	is, it adds 1 character for the 0 terminator, and if UNICODE is being
	used, doubles the total byte count.*/
	VSTRINGA		AllocForString(VUINT nCharCount)
	{
		if ( nCharCount )
		{
			/* Double size for UNICODE?*/
			#ifdef VWCL_UNICODE
				nCharCount *= sizeof(VCHAR);
			#endif

			/* Add 1 for terminator.*/
			nCharCount += sizeof(VCHAR);
		}

		return Alloc(nCharCount);
	}

	/** Copy memory from another block to this one. Returns a pointer
	to the new memory on success, NULL on failure. On failure, any previous
	memory held by this object is still safe and valid. If the existing
	memory block has no memory allocated, this is not considered a problem,
	and this object will also have no memory allocated on success, and
	will return NULL even though it succeeded.*/
	VSTRINGA		Copy(VMemoryBlock const& existing)
	{
		if ( Alloc(existing.GetLength()) )
		{
			memcpy(m_pBlock, existing.Get(), existing.GetLength());
			return m_pBlock;
		}

		return NULL;
	}

	/** Free memory allocated by this object.*/
	void			Free()
	{
		if ( m_pBlock )
		{
			free(m_pBlock);
			Init(0, NULL);
		}
	}

	/** Return a pointer to the first byte of memory.*/
	VSTRINGA		Get() const
		{ return (VSTRINGA)m_pBlock; }

	/** Return the number of bytes allocated.*/
	VUINT			GetLength() const
		{ return m_nLength; }

	/** Return to take control of the memory buffer. Calling code must free
	the memory with the C runtime library function free() when done. After
	the buffer is stolen, this class will no longer maintain or use it.*/
	VSTRINGA		Steal()
	{
		VSTRINGA pBlock = m_pBlock;
		Init(0, NULL);

		return pBlock;
	}

	/** Transfer the contents of this memory block object to another. This
	will result in the object referenced by memBlockDest to be cleared. It
	will then take ownership of the memory of this object, and this object
	will be cleared.*/
	void			TransferTo(VMemoryBlock& memBlockDest)
	{
		memBlockDest.Free();
		memBlockDest.m_nLength =	m_nLength;
		memBlockDest.m_pBlock =		Steal();
	}

protected:
	/** Initialize member variables to default values, or copy from
	another object. Either nLength or pExisting can be given, but
	not both.*/
	void			Init(	VUINT				nLength,
							VMemoryBlock const* pExisting)
	{
		#ifdef VWCL_DEBUG
			if ( nLength )
				VASSERT(!pExisting)
		#endif

		#ifdef VWCL_DEBUG
			if ( pExisting )
				VASSERT(!nLength)
		#endif

		/* Initiailize members.*/
		m_pBlock =	NULL;
		m_nLength =	0;

		/* Copy existing object?*/
		if ( pExisting )
			Copy(VREF_PTR(pExisting));
		else if ( nLength )
			Alloc(nLength);
	}

	/** Embedded Members.*/
	VSTRINGA		m_pBlock;
	VUINT			m_nLength;
};

#endif	/* VMEMORYBLOCK*/
