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

#ifndef VLINKEDLISTITEM
#define VLINKEDLISTITEM

#include "../vstandard.h"

/** VLinkedListItem is the base class for all objects that are part of a
linked list. It simply defines a trivial class that has a public pointer
to the next item of the same, or derived, type and a virtual destructor.*/
class VLinkedListItem
{
public:
	/** Default constructor initializes internal member variable.*/
	VLinkedListItem()
		{ m_pNextItem = NULL; }

	/** Virtual destructor does nothing. Override can do anything.*/
	virtual ~VLinkedListItem()
		{;}

	/** The next item in the linked list. This is public so that programs
	that create hundreds or thousands of these do not have to soak up the
	overhead of extra member funcions. Derived classes can implement
	accessor functions as needed.*/
	VLinkedListItem* m_pNextItem;
};

#endif /* VLINKEDLISTITEM*/
