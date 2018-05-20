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

#ifndef VCOMMANDPARAMETERPAIRLIST
#define VCOMMANDPARAMETERPAIRLIST

#include "../vstandard.h"
#include "../collections/vcommandparameterpair.hpp"
#include "../collections/vunsortedstringlinkedlist.hpp"
#include "../strings/vstring.hpp"

/** VCommandParameterPairList is used to store a collection of
VCommandParameterPair objects. Derived from VUnSortedStringLinkedList,
it gets most of its functionality from this class, but adds methods
for easier navigation of the list and insertion of objects into the
list. You should control sorting and case sensitivity using the
options provided by VUnSortedStringLinkedList and its base class(es).
The default for this class is not case sensitive.*/
class VCommandParameterPairList : public VUnSortedStringLinkedList
{
public:
	/** Constructor initializes class. When adding command and parameter
	pairs, it is considered valid to have more than one entry for each
	command. If you would rather have only one VCommandParameterPair object
	created for each command, pass a non-0 value for nParameterSeparator.
	This will cause only one command to appear in the linked list, but the
	parameter string will contain multiple strings, each separated by the
	nParameterSeparator character. These strings can easily be "walked" using
	the VDelimitedString class. The use of these overlapping parameters can
	also be set after construction by using the GetParameterSeparator()
	function.*/
	VCommandParameterPairList(VCHAR nParameterSeparator = 0)
		{ m_nParameterSeparator = nParameterSeparator; }

	/** Virtual destructor does nothing.*/
	virtual ~VCommandParameterPairList()
		{;}

	/** Add a new command and parameter pair. pszCommand is required, but the
	parameter is optional. NOTE: An important side affect of this function is
	that since it is virtual, you can override it in a derived class. You
	could then implement your own management of pszCommand and pszParameter. In
	other words, you can handle this function, and return NULL. This will
	prevent the pair from going into the internal linked list. You could for
	instance, handle your own processing and configuration settings during
	an override of this function, and not deal with a two part Read-Parse
	cycle.*/
	virtual VCommandParameterPair*	AddNewPair(	VSTRING_CONST pszCommand,
												VSTRING_CONST pszParameter)
	{
		VASSERT(VSTRLEN_CHECK(pszCommand))

		if ( !pszCommand )
			return NULL;

		VCommandParameterPair* pPair = NULL;

		/* Are we combining parameters?*/
		if ( m_nParameterSeparator )
		{
			if ( pszParameter )
			{
				/* Try to find an existing command with the same name.*/
				pPair = FindCommand(pszCommand);

				/* Did we find it? Add this parameter.*/
				if ( pPair )
				{
					VString sNewParameter(pszParameter);
					sNewParameter.Trim();

					if ( sNewParameter.IsNotEmpty() )
					{
						VString& strParameter = pPair->GetParameter();
						
						if ( strParameter.IsNotEmpty() )
						{
							strParameter += m_nParameterSeparator;
							strParameter += sNewParameter;
						}
						else
							strParameter.ReplaceBuffer(sNewParameter);
					}
				}
			}
		}

		/* Do we need to allocate a new pair object?*/
		if ( !pPair )
		{
			/* Allocate a new command-parameter pair object.*/
			pPair = VNEW VCommandParameterPair(pszCommand, pszParameter);

			if ( pPair )
				Add(pPair);
		}

		return pPair;
	}

	/** Return a reference to the value used to indicate if multiple
	parameters should be stored within a single VCommandParameterPair
	object. The default is VFALSE, which can create more than one
	VCommandParameterPair object when multiple commands with the same
	name are found. See the class constructor for more information.*/
	VCHAR&							GetParameterSeparator() const
		{ return (VCHAR&)m_nParameterSeparator; }

	/** Return the first VCommandParameterPair object in the list.*/
	VCommandParameterPair*			GetFirstPair() const
		{ return (VCommandParameterPair*)m_pFirstItem; }

	/** Given a command string, return the first VCommandParameterPair that
	contains this command, or NULL on failure. To control case sensitivity,
	use the methods provided by the VUnSortedStringLinkedList base class(es).
	The default for this class is not case sensitive.*/
	VCommandParameterPair*			FindCommand(VSTRING_CONST pszCommand) const
		{ return (VCommandParameterPair*)Find(pszCommand); }

	/** Given a command string, return the parameter string object that is
	associated with it, or NULL on failure. The parameter returned could be
	an empty string. If you want NULL returned in the case of an empty
	string, pass VTRUE for the bNullForEmpty parameter. This is the default.
	To control case sensitivity, use the methods provided by the
	VUnSortedStringLinkedList base class(es).The default for this class is
	not case sensitive.*/
	VString*						FindParameter(
		VSTRING_CONST	pszCommand,
		VBOOL			bNullForEmpty =
							VTRUE) const
	{
		VCommandParameterPair* pPair = FindCommand(pszCommand);

		if ( pPair && bNullForEmpty && pPair->GetParameter().IsEmpty() )
			pPair = NULL;

		return (pPair) ? &pPair->GetParameter() : NULL;
	}

protected:
	/** Embedded Members.*/
	VCHAR							m_nParameterSeparator;
};

#endif /* VCOMMANDPARAMETERPAIRLIST*/
