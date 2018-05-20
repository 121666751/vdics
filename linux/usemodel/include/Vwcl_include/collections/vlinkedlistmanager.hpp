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

#ifndef VLINKEDLISTMANAGER
#define VLINKEDLISTMANAGER

#include "../vstandard.h"
#include "vlinkedlistitem.hpp"

/** VLinkedListManager manages a singly linked list of VLinkedListItem
objects. If using a linked list in a multithreaded application where
multiple threads can be accessing the linked list contents, use the
VCriticalLinkedListManager class instead of this one, as it provides
thread safe access to the list.*/
class VLinkedListManager
{
public:
	/** Same as Add(VLinkedListItem* pItem, VFALSE), to add the item at
	the end of the list.*/
	void operator += (VLinkedListItem* pItem)
		{ Add(pItem, VFALSE); }

	/** Same as Delete(VLinkedListItem* pItem, VTRUE), to delete the
	item from the list and delete the pItem object.*/
	void operator -= (VLinkedListItem* pItem)
		{ Delete(pItem, VTRUE); }

	/** Default constructor initializes internal members.*/
	VLinkedListManager()
		{ Empty(); }

	/** Virtual destructor calls Free() to delete the objects contained
	in the linked list.*/
	virtual ~VLinkedListManager()
		{ Free(); }

	/** Base class adds an item to the end of the linked list unless
	bAddAtBeginningOfList is VTRUE.*/
	virtual void		Add(VLinkedListItem*	pItem,
							VBOOL				bAddAtBeginningOfList =
													VFALSE)
	{
		VASSERT(pItem)

		if ( m_pFirstItem )
		{
			if ( bAddAtBeginningOfList )
			{
				pItem->m_pNextItem = m_pFirstItem;
				m_pFirstItem = pItem;
			}
			else
			{
				m_pLastItem->m_pNextItem = pItem;
				m_pLastItem = pItem;
			}
		}
		else
			m_pFirstItem = m_pLastItem = pItem;

		/* Increment count.*/
		m_nItemCount++;
	}

	/** Delete pItem from the linked list. If bDeleteItem is VTRUE, delete
	will be called on pItem.*/
	virtual VBOOL		Delete(	VLinkedListItem*	pItem,
								VBOOL				bDeleteItem =
														VTRUE)
	{
		VASSERT(pItem)

		/* Find the previous item in the list.*/
		VLinkedListItem*	pThisItem =		m_pFirstItem;
		VLinkedListItem*	pPreviousItem =	NULL;
		VBOOL				bResult =		VFALSE;

		while ( pThisItem )
		{
			if ( pThisItem == pItem )
			{
				bResult = VTRUE;

				/* Is this the first item?*/
				if ( pItem == m_pFirstItem )
					m_pFirstItem = pItem->m_pNextItem;

				/* Is this the last item?*/
				if ( pItem == m_pLastItem )
					m_pLastItem = pPreviousItem;

				/* Assign pPreviousItem to point to the next one.*/
				if ( pPreviousItem )
					pPreviousItem->m_pNextItem = pItem->m_pNextItem;

				/* Decrement count.*/
				m_nItemCount--;

				/* Done.*/
				break;
			}
			else
			{
				/* Save previous item and get next item for loop.*/
				pPreviousItem =	pThisItem;
				pThisItem =		pThisItem->m_pNextItem;
			}
		}

		/* Delete pItem now?*/
		if ( bDeleteItem )
			VDELETE_NULL(pItem)

		return bResult;
	}

	/** Empty the contents of the linked list, but do no delete objects
	currently in the list.*/
	virtual void		Empty()
	{
		m_pFirstItem = m_pLastItem =	NULL;
		m_nItemCount =					0;
	}

	/** Find the item in the linked list, or return NULL if not found.*/
	VLinkedListItem*	Find(VLinkedListItem const* pItem) const
	{
		if ( pItem )
		{
			VLinkedListItem* pThisItem = m_pFirstItem;

			while ( pThisItem )
			{
				if ( pThisItem == pItem )
					return pThisItem;

				pThisItem = pThisItem->m_pNextItem;
			}
		}

		return NULL;
	}

	/** Free all items's in linked list. This deletes the objects in the
	linked list. If you wish to only reset the linked list to contain
	no objects, but not delete the objects, call Empty().*/
	virtual void		Free()
	{
		VLinkedListItem* pItem =		m_pFirstItem;
		VLinkedListItem* pNextItem =	NULL;

		while ( pItem )
		{
			pNextItem = pItem->m_pNextItem;
			VDELETE_NULL(pItem)
			pItem = pNextItem;
		}

		Empty();
	}

	/** Return the number of items or objects held in the linked list.*/
	VUINT				GetCount() const
		{ return m_nItemCount; }

	/** Pointers to the first and last objects in the linked list.*/
	VLinkedListItem*	m_pFirstItem;
	VLinkedListItem*	m_pLastItem;

	/** The number of items in the list.*/
	VUINT				m_nItemCount;
};

#endif /* VLINKEDLISTMANAGER*/
