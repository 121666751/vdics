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

#ifndef VBITMASK
#define VBITMASK

#include "../vstandard.h"

/** VBitmask supports getting, setting, appending, removing, saving, and
restoring bits or values from a VUINT numeric data type.*/
class VBitmask
{
public:
	/** Default constructor can initialize the class with an existing
	value. Defaults to 0.*/
	VBitmask(VUINT nBits = 0)
	{
		Bits(nBits);
		m_nSaveBits = 0;
	}

	/** Copy constructor copies the current value from an existing bitmask
	object.*/
	VBitmask(VBitmask const& bitmask)
		{ Bits(bitmask); }

	/** Virtual destructor does nothing.*/
	virtual ~VBitmask()
		{;}

	/** Assignment operator, same as calling Bits() to set the value.*/
	VUINT operator =(VUINT nBits)
		{ return Bits(nBits); }

	/** Assignment operator, same as calling Bits() to set the value
	when an existing bitmask reference is known.*/
	VUINT operator =(VBitmask const& bitmask)
		{ return Bits(bitmask); }

	/** Comparison operator, same as IsEqual().*/
	VUINT operator ==(VUINT nBits) const
		{ return IsEqual(VBitmask(nBits)); }

	/** Comparison operator, same as IsEqual() when an existing bitmask
	reference is known.*/
	VBOOL operator ==(VBitmask const& bitmask) const
		{ return IsEqual(bitmask); }

	/** Comparison operator, same as IsNotEqual().*/
	VBOOL operator !=(VUINT nBits) const
		{ return IsNotEqual(VBitmask(nBits)); }

	/** Comparison operator, same as IsNotEqual() when an existing bitmask
	reference is known.*/
	VBOOL operator !=(VBitmask const& bitmask) const
		{ return IsNotEqual(bitmask); }

	/** Append bits to the value, OR them together and return
	resulting value.*/
	VUINT		Append(VUINT nBits)
	{
		m_nBits |= nBits;
		return m_nBits;
	}

	/** Append bits to the value and remove the bits at nRemoveBits.*/
	VUINT		AppendExclusive(VUINT nBits,
								VUINT nRemoveBits)
	{
		Remove(nRemoveBits);
		return Append(nBits);
	}

	/** Append or remove bits to or from the value depending on the
	bAppend flag.*/
	VUINT		AppendOrRemove(	VUINT nBits,
								VBOOL bAppend)
		{ return (bAppend) ? Append(nBits) : Remove(nBits); }

	/** Return a reference to the internal VUINT bit bitmask or value.*/
	VUINT&		Bits() const
		{ return (VUINT&)m_nBits; }

	/** Set the internal VUINT bit bitmask or value and save bits from
	existing VBitmask (deep copy) object reference and returns
	resulting value.*/
	VUINT		Bits(VBitmask const& bitmask)
	{
		m_nBits =		bitmask.m_nBits;
		m_nSaveBits =	bitmask.m_nSaveBits;

		return m_nBits;
	}

	/** Set the internal VUINT bit bitmask or value from a VUINT
	numeric data type and return resulting value.*/
	VUINT		Bits(VUINT nBits)
	{
		m_nBits = nBits;
		return m_nBits;
	}

	/** Returns VTRUE if the value contained in this class is also
	the value contained in bitmask, VFALSE otherwise.*/
	VBOOL		IsEqual(VBitmask const& bitmask) const
		{ return (bitmask.m_nBits == m_nBits) ? VTRUE : VFALSE; }

	/** Returns VTRUE if the value contained in this class is also
	the value contained in nBits, VFALSE otherwise.*/
	VBOOL		IsEqual(VUINT nBits) const
		{ return (m_nBits == nBits) ? VTRUE : VFALSE; }

	/** Returns VTRUE if the value contained in this class is not
	the value contained in bitmask, VFALSE otherwise.*/
	VBOOL		IsNotEqual(VBitmask const& bitmask) const
		{ return !IsEqual(bitmask); }

	/** Returns VTRUE if the value contained in this class is not
	the value contained in nBits, VFALSE otherwise.*/
	VBOOL		IsNotEqual(VUINT nBits) const
		{ return !IsEqual(nBits); }

	/** Determine if bit(s) is set. Returns VTRUE if yes, VFALSE otherwise.*/
	VBOOL		IsSet(VUINT nBits) const
		{ return (m_nBits & nBits) ? VTRUE : VFALSE; }

	/** Determine if bit(s) is not set. Returns VTRUE if yes,
	VFALSE otherwise.*/
	VBOOL		IsNotSet(VUINT nBits) const
		{ return (IsSet(nBits)) ? VFALSE : VTRUE; }

	/** Remove bits from the value and return resulting value.*/
	VUINT		Remove(VUINT nBits)
	{
		m_nBits &= ~nBits;
		return m_nBits;
	}

	/** Remove bits from the value and append the bits at nAppendBits.*/
	VUINT		RemoveExclusive(VUINT nBits,
								VUINT nAppendBits)
	{
		Append(nAppendBits);
		return Remove(nBits);
	}

	/** Save the current value to be restored later with the
	Restore() function.*/
	void		Save()
		{ m_nSaveBits = m_nBits; }

	/** Restore a previously saved value. The old saved value remains in
	effect until the next Save() call is made, so it is possible to do
	multiple Restore()'s from a single Save(). Returns resulting value.*/
	VUINT		Restore()
	{
		m_nBits = m_nSaveBits;
		return m_nBits;
	}

	/** Toggle the specified bit. If it is on, turn it off, and vice-versa.*/
	VUINT		Toggle(VUINT nBit)
	{
		if ( IsSet(nBit) )
			Remove(nBit);
		else
			Append(nBit);

		return m_nBits;
	}

protected:
	/** Embedded Members.*/
	VUINT		m_nBits;
	VUINT		m_nSaveBits;
};

#endif /* VBITMASK*/
