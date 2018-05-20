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

#ifndef VCRITICALLINKEDLISTMANAGER
#define VCRITICALLINKEDLISTMANAGER

#include "../vstandard.h"
#include "../collections/vlinkedlistmanager.hpp"
#include "vcriticalsection.hpp"

/** VCriticalLinkedListManager manages a linked list of VLinkedListItem
objects using VLinkedListManager, and is thread safe using critical sections
via the VCriticalSection class. This is a simple derivation of both base
classes to yield a threadsafe implementation of the linked list manager. It
is critical to note that the VLinkedListManager functions Find() and
GetCount() are not implemented in this class. The reason is that objects
returned by Find(), or the count returned by GetCount() could be invalid
after returning, since the critical section would no longer be locked. You
must Enter() the critical section first, call the Find() or GetCount()
VLinkedListManager functions, do something with the returned information,
and then Leave() the critical section. This cannot be encapsulated in this
class.*/
class VCriticalLinkedListManager :
	public VLinkedListManager, public VCriticalSection
{
public:
	/** Virtual destructor calls Free() to delete the objects contained
	in the linked list, in a thread-safe manner.*/
	virtual ~VCriticalLinkedListManager()
		{ Free(); }

	/** Adds an item to the end of the linked list unless
	bAddAtBeginningOfList is VTRUE.*/
	virtual void	Add(VLinkedListItem*	pItem,
						VBOOL				bAddAtBeginningOfList =
												VFALSE)
	{
		Enter();
		VLinkedListManager::Add(pItem, bAddAtBeginningOfList);
		Leave();
	}

	/** Delete pItem from the linked list. If bDeleteItem is VTRUE, delete
	will be called on pItem.*/
	virtual VBOOL	Delete(	VLinkedListItem*	pItem,
							VBOOL				bDeleteItem =
													VTRUE)
	{
		Enter();
		VLinkedListManager::Delete(pItem, bDeleteItem);
		Leave();
	}

	/** Empty the contents of the linked list, but do no delete objects
	currently in the list.*/
	virtual void	Empty()
	{
		Enter();
		VLinkedListManager::Empty();
		Leave();
	}

	/** Free all items's in linked list. This deletes the objects in the
	linked list. If you wish to only reset the linked list to contain no
	objects, but not delete the objects, call Empty().*/
	virtual void	Free()
	{
		Enter();
		VLinkedListManager::Free();
		Leave();
	}
};

#endif /* VCRITICALLINKEDLISTMANAGER*/
