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

#ifndef VPTRARRAY
#define VPTRARRAY

#include "../vstandard.h"

#define VPTRARRAY_ELEMENT			VULONG
#define VPTRARRAY_SKIP_INDICATOR	(VLONG)0xffffffff
typedef VLONG						VPTRARRAY_INDEX;

/** VPtrArray is a collection of unknown 4 byte types (pointers).*/
class VPtrArray
{
public:
	VPtrArray()
	{
		m_pData = NULL;
		m_nSize = 0;
	}

	virtual ~VPtrArray()
		{ RemoveAll(); }

	/** Same as At(nIndex).*/
	VVOIDPTR operator [] (VPTRARRAY_INDEX nIndex) const
		{ return At(nIndex); }

	/** Same as Add().*/
	VPTRARRAY_INDEX operator += (VVOIDPTR pData)
		{ return Add(pData); }

	/** Same as FindAndRemove().*/
	VPTRARRAY_INDEX operator -= (VVOIDPTR pData)
		{ return FindAndRemove(pData); }

	/** Add a new element at the end of the array. Returns index into
	array on success, -1 on failure.*/
	VPTRARRAY_INDEX	Add(VVOIDPTR pData)
	{
		if ( AllocCopy(	m_nSize + 1,
						VPTRARRAY_SKIP_INDICATOR,
						VPTRARRAY_SKIP_INDICATOR) )
		{
			m_pData[m_nSize - 1] = (VPTRARRAY_ELEMENT)pData;
			return m_nSize - 1;
		}

		return -1;
	}

	/** Return element at given index.*/
	VVOIDPTR		At(VPTRARRAY_INDEX nIndex) const
	{
		VASSERT(nIndex >= 0)
		VASSERT(nIndex < m_nSize)

		return	(nIndex >= 0 && nIndex < m_nSize)
				? (VVOIDPTR)m_pData[nIndex]
				: NULL;
	}

	/** Set element at given index. This does not allocate memory, that
	must already have been done by Add() or Size().*/
	void			At(	VPTRARRAY_INDEX	nIndex,
						VVOIDPTR		pData)
	{
		VASSERT(nIndex >= 0)
		VASSERT(nIndex < m_nSize)

		if ( nIndex >= 0 && nIndex < m_nSize )
			m_pData[nIndex] = (VPTRARRAY_ELEMENT)pData;
	}

	/** Return the index into array for first occurence of pData, or -1 if
	not found.*/
	VPTRARRAY_INDEX	Find(VVOIDPTR pData) const
	{
		for ( VPTRARRAY_INDEX i = 0; i < m_nSize; i++ )
		{
			if ( m_pData[i] == (VPTRARRAY_ELEMENT)pData )
				return i;
		}

		return -1;
	}

	/** Find and remove element from array. Returns VTRUE on success,
	VFALSE on failure.*/
	VBOOL			FindAndRemove(VVOIDPTR pData)
	{
		VINT nIndex = Find(pData);
		return (nIndex != -1) ? RemoveAt(nIndex) : VFALSE;
	}

	/** Insert a new element into the array at specified index, moving
	everything after it further into the array. Returns index on success,
	-1 on failure. If nIndex is -1 or an invalid value, this function will
	add the element to the end of the array.*/
	VPTRARRAY_INDEX	InsertAt(	VPTRARRAY_INDEX	nIndex,
								VVOIDPTR		pData)
	{
		if ( nIndex < 0 || nIndex > m_nSize )
			nIndex = m_nSize;

		/* Add item first to end.*/
		if ( Add(pData) != - 1 )
		{
			if ( nIndex < m_nSize - 1 )
			{
				/* Shift memory up in array.*/
				memmove(	&m_pData[nIndex + 1],
							&m_pData[nIndex],
							sizeof(VPTRARRAY_ELEMENT) *
							(m_nSize - nIndex - 1));

				/* Set pData where told to.*/
				m_pData[nIndex] = (VPTRARRAY_ELEMENT)pData;
			}

			return nIndex;
		}

		return -1;
	}

	/** Remove all elements from the array.*/
	void			RemoveAll()
		{ VDELETE_ARRAY_NULL(m_pData) m_nSize = 0; }

	/** Remove a single element from the array. All other elements will be
	shifted down one slot. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			RemoveAt(VPTRARRAY_INDEX nIndex)
		{ return RemoveRange(nIndex, nIndex); }

	/** Remove multiple elements from the array. All other elements will be
	shifted down by the number of elements removed. Returns VTRUE on success,
	VFALSE on failure.*/
	VBOOL			RemoveRange(VPTRARRAY_INDEX nIndexStart,
								VPTRARRAY_INDEX nIndexEnd)
	{
		VASSERT(nIndexStart >= 0)
		VASSERT(nIndexEnd < m_nSize)
		VASSERT(nIndexEnd >= nIndexStart)

		if (	nIndexStart >= 0 &&
				nIndexEnd < m_nSize &&
				nIndexEnd >= nIndexStart )
		{
			VPTRARRAY_INDEX nSize = m_nSize;
			return (AllocCopy(	m_nSize - ((nIndexEnd - nIndexStart) + 1),
								nIndexStart,
								nIndexEnd) &&
								m_nSize != nSize) ? VTRUE : VFALSE;
		}

		return VFALSE;
	}

	/** Set the size of the array (number of elements). This function
	pre-allocates array memory so future InsertAt() functions can be
	called. Elements currently in array will be preserved, unless the array
	is skrinking, in which case element(s) at the end of the current array
	will be truncate. Returns VTRUE on success, VFALSE on failure. It is
	valid to call this function with a size of 0, which is the same as
	RemoveAll().*/
	VBOOL			Size(VPTRARRAY_INDEX nSize)
	{
		VASSERT(nSize >= 0)

		if ( nSize )
			return AllocCopy(	nSize,
								VPTRARRAY_SKIP_INDICATOR,
								VPTRARRAY_SKIP_INDICATOR);

		return VFALSE;
	}

	/** Return the currently allocated array size.*/
	VPTRARRAY_INDEX	Size() const
		{ return m_nSize; }

protected:
	/** Internal function to work with private data.*/
	VBOOL		AllocCopy(	VPTRARRAY_INDEX nNewSize,
							VPTRARRAY_INDEX nSkipIndexStart,
							VPTRARRAY_INDEX nSkipIndexEnd)
	{
		/* Setting size to 0?*/
		if ( !nNewSize )
		{
			RemoveAll();
			return VTRUE;
		}

		/* Allocate memory now.*/
		VPTRARRAY_ELEMENT* pNewData = VNEW VPTRARRAY_ELEMENT[nNewSize];

		/* Did memory allocation fail?*/
		if ( !pNewData )
		{
			VASSERT(VFALSE)
			return VFALSE;
		}

		/* Zero out new memory.*/
		VZEROMEMORY(pNewData, nNewSize * sizeof(VPTRARRAY_ELEMENT));

		/* Copy any previous memory?*/
		if ( m_nSize )
		{
			/* There must be data!*/
			VASSERT(m_pData)

			/* If either skip index is VPTRARRAY_SKIP_INDICATOR, we will
			copy everything .VPTRARRAY_SKIP_INDICATOR is used internally
			as a flag to indicate no exclusions.*/
			if (	nSkipIndexStart == VPTRARRAY_SKIP_INDICATOR ||
					nSkipIndexEnd == VPTRARRAY_SKIP_INDICATOR )
				memcpy(	pNewData,
						m_pData,
						VMIN(m_nSize, nNewSize) * sizeof(VPTRARRAY_ELEMENT));
			else
			{
				/* Copy up to nSkipIndexStart?*/
				if ( nSkipIndexStart )
					memcpy(	&pNewData[0],
							&m_pData[0],
							nSkipIndexStart * sizeof(VPTRARRAY_ELEMENT));

				/* Copy from nSkipIndexEnd + 1 to end.*/
				memcpy(	&pNewData	[nSkipIndexStart],
						&m_pData	[nSkipIndexEnd + 1],
						(m_nSize * sizeof(VPTRARRAY_ELEMENT)) -
						((nSkipIndexEnd + 1) * sizeof(VPTRARRAY_ELEMENT)));
			}
		}

		/* Kill any current memory.*/
		VDELETE_ARRAY_NULL(m_pData)

		/* Make new assignments.*/
		m_pData =	pNewData;
		m_nSize =	nNewSize;

		return VTRUE;
	}

protected:
	/** Embedded Members.*/
	VPTRARRAY_ELEMENT*		m_pData;
	VPTRARRAY_INDEX			m_nSize;
};

#endif /* VPTRARRAY*/
