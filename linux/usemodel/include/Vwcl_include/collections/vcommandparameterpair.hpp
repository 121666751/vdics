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

#ifndef VCOMMANDPARAMETERPAIR
#define VCOMMANDPARAMETERPAIR

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "vlinkedliststringitem.hpp"

/** Objects of this type represent a single command and parameter pair. This
is a common form of configuration file reading, and can be used in any
place where a list of objects that have a name also have an associated data
string attached to them. Derived from VLinkedListStringItem, the command name
is the same as the object name, and the class provides methods for getting
the parameter string as well as operations on it. Objects of this type are
normally stored in a linked list manager object, usually
VCommandParameterPairList. Keep in mind that objects of this type ARE
VString objects, so all methods of VString are available to objects of this
type.*/
class VCommandParameterPair : public VLinkedListStringItem
{
public:
	/** Default constructor can be initialized with the command and parameter
	names. These can also be set or modified later using normal VString
	class methods. If set during construction, the command and parameter will
	each have leading and trailing white space removed from them, using the
	VString::Trim() method.*/
	VCommandParameterPair(	VSTRING_CONST pszCommand =
											NULL,
							VSTRING_CONST pszParameter =
											NULL)
	{
		if ( pszCommand )
		{
			String(pszCommand);
			Trim();
		}

		if ( pszParameter )
		{
			m_strParameter = pszParameter;
			m_strParameter.Trim();
		}
	}

	/** Virtual destructor does nothing.*/
	virtual ~VCommandParameterPair()
		{;}

	/** Return the next object of this type in the linked list.*/
	VCommandParameterPair*	GetNextPair() const
		{ return (VCommandParameterPair*)m_pNextItem; }

	/** Return a reference to the parameter name string.*/
	VString&				GetParameter() const
		{ return (VString&)m_strParameter; }

	/** Return VTRUE or VFALSE based on the parameter value. The strings
	True, Yes, On and 1 are considered VTRUE, while everything else is
	considered VFALSE, as is NULL. The strings are not case sensitive. It
	is not valid to use this function for parameter strings that contain
	multiple parameters separated by a delimiter, such as those available
	using the VCommandParameterPairList class.*/
	VBOOL					IsParameterTrue() const
	{
		VSTRING_CONST TRUE_STRINGS[] =
			{
				VTEXT("TRUE"),
				VTEXT("YES"),
				VTEXT("ON"),
				VTEXT("1")
			};

		for ( VUINT i = 0; i < VARRAY_SIZE(TRUE_STRINGS); i++ )
		{
			if ( m_strParameter == TRUE_STRINGS[i] )
				return VTRUE;
		}

		return VFALSE;
	}

protected:
	/** Embedded Members.*/
	VString					m_strParameter;
};

#endif /* VCOMMANDPARAMETERPAIR*/
