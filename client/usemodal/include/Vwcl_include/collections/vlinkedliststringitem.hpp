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

#ifndef VLINKEDLISTSTRINGITEM
#define VLINKEDLISTSTRINGITEM

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "vlinkedlistitem.hpp"

/** VLinkedListStringItem is an object that represents a string contained
in a linked list. It is a very small wrapper class that gets its
functionality from both the VString and VLinkedListItem base classes.*/
class VLinkedListStringItem : public VString, public VLinkedListItem
{
public:
	/** Default constructor can be initialized with an existing
	string, or NULL.*/
	VLinkedListStringItem(VSTRING_CONST pszString = NULL)
		: VString(pszString)
		{;}

	/** Return the next item in the list, or NULL.*/
	VLinkedListStringItem*	GetNextItem() const
		{ return (VLinkedListStringItem*)m_pNextItem; }
};

#endif /* VLINKEDLISTSTRINGITEM*/
