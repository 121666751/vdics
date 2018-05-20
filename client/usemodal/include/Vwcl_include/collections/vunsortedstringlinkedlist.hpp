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

#ifndef VUNSORTEDSTRINGLINKEDLIST
#define VUNSORTEDSTRINGLINKEDLIST

#include "../vstandard.h"
#include "vsortedstringlinkedlist.hpp"

/** VUnSortedStringLinkedList manages a linked list of unsorted strings. In
addition, this class can be used to store a list of sorted strings by
removing the base class option bit OPTION_UNSORTED.*/
class VUnSortedStringLinkedList : public VSortedStringLinkedList
{
public:
	/** Default constructor initializes options to be an unsorted list.*/
	VUnSortedStringLinkedList(VUINT nOptions = OPTION_UNSORTED)
		: VSortedStringLinkedList(nOptions)
		{;}

	/** Copy constructor.*/
	VUnSortedStringLinkedList(VUnSortedStringLinkedList const& original)
		: VSortedStringLinkedList(original)
		{;}

	/** Copy constructor used when given a base class object.*/
	VUnSortedStringLinkedList(VSortedStringLinkedList const& original)
		: VSortedStringLinkedList(original)
		{;}
};

#endif /* VUNSORTEDSTRINGLINKEDLIST*/
