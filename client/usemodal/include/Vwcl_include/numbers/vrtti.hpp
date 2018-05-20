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

#ifndef VRTTIHPP
#define VRTTIHPP

#include "../vstandard.h"

typedef VUINT VRTTI_TYPE;

/** VRTTI provides a very fast, compiler independant, trivial implementation
of Runtime Type Information. Objects normally derive this functionality,
which is nothing more than a numeric value assigned to derived classes.*/
class VRTTI
{
public:
	/** Default constructor can initialize the RTTI value. Defaults to 0.*/
	VRTTI(VRTTI_TYPE nRTTI = 0)
		{ m_nRTTI = nRTTI; }

	/** Virtual destructor does nothing.*/
	virtual ~VRTTI()
		{;}

	/** Return a reference to the runtime type value.*/
	VRTTI_TYPE&	GetRTTI() const
		{ return (VRTTI_TYPE&)m_nRTTI; }

protected:
	VRTTI_TYPE	m_nRTTI;
};

#endif /* VRTTIHPP*/
