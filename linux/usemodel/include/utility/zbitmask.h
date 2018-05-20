#ifndef ZBITMASK
#define ZBITMASK

/** ZBITMASK supports getting, setting, appending, removing, saving, and
restoring bits or values from a int numeric data type.*/
class ZBitmask
{
public:
	/** Default constructor can initialize the class with an existing
	value. Defaults to 0.*/
	ZBitmask(int nBits = 0)
	{
		Bits(nBits);
		m_nSaveBits = 0;
	}

	/** Copy constructor copies the current value from an existing bitmask
	object.*/
	ZBitmask(ZBitmask const& bitmask)
		{ Bits(bitmask); }

	/** Virtual destructor does nothing.*/
	virtual ~ZBitmask()
		{;}

	/** Assignment operator, same as calling Bits() to set the value.*/
	int operator =(int nBits)
		{ return Bits(nBits); }

	/** Assignment operator, same as calling Bits() to set the value
	when an existing bitmask reference is known.*/
	int operator =(ZBitmask const& bitmask)
		{ return Bits(bitmask); }

	/** Comparison operator, same as IsEqual().*/
	int operator ==(int nBits) const
		{ return IsEqual(ZBitmask(nBits)); }

	/** Comparison operator, same as IsEqual() when an existing bitmask
	reference is known.*/
	bool operator ==(ZBitmask const& bitmask) const
		{ return IsEqual(bitmask); }

	/** Comparison operator, same as IsNotEqual().*/
	bool operator !=(int nBits) const
		{ return IsNotEqual(ZBitmask(nBits)); }

	/** Comparison operator, same as IsNotEqual() when an existing bitmask
	reference is known.*/
	bool operator !=(ZBitmask const& bitmask) const
		{ return IsNotEqual(bitmask); }

	/** Append bits to the value, OR them together and return
	resulting value.*/
	int		Append(int nBits)
	{
		m_nBits |= nBits;
		return m_nBits;
	}

	/** Append bits to the value and remove the bits at nRemoveBits.*/
	int		AppendExclusive(int nBits,
								int nRemoveBits)
	{
		Remove(nRemoveBits);
		return Append(nBits);
	}

	/** Append or remove bits to or from the value depending on the
	bAppend flag.*/
	int		AppendOrRemove(	int nBits,
								bool bAppend)
		{ return (bAppend) ? Append(nBits) : Remove(nBits); }

	/** Return a reference to the internal int bit bitmask or value.*/
	int&		Bits() const
		{ return (int&)m_nBits; }

	/** Set the internal int bit bitmask or value and save bits from
	existing ZBitmask (deep copy) object reference and returns
	resulting value.*/
	int		Bits(ZBitmask const& bitmask)
	{
		m_nBits =		bitmask.m_nBits;
		m_nSaveBits =	bitmask.m_nSaveBits;

		return m_nBits;
	}

	/** Set the internal int bit bitmask or value from a int
	numeric data type and return resulting value.*/
	int		Bits(int nBits)
	{
		m_nBits = nBits;
		return m_nBits;
	}

	/** Returns VTRUE if the value contained in this class is also
	the value contained in bitmask, VFALSE otherwise.*/
	bool		IsEqual(ZBitmask const& bitmask) const
		{ return (bitmask.m_nBits == m_nBits) ? VTRUE : VFALSE; }

	/** Returns VTRUE if the value contained in this class is also
	the value contained in nBits, VFALSE otherwise.*/
	bool		IsEqual(int nBits) const
		{ return (m_nBits == nBits) ? VTRUE : VFALSE; }

	/** Returns VTRUE if the value contained in this class is not
	the value contained in bitmask, VFALSE otherwise.*/
	bool		IsNotEqual(ZBitmask const& bitmask) const
		{ return !IsEqual(bitmask); }

	/** Returns VTRUE if the value contained in this class is not
	the value contained in nBits, VFALSE otherwise.*/
	bool		IsNotEqual(int nBits) const
		{ return !IsEqual(nBits); }

	/** Determine if bit(s) is set. Returns VTRUE if yes, VFALSE otherwise.*/
	bool		IsSet(int nBits) const
		{ return (m_nBits & nBits) ? VTRUE : VFALSE; }

	/** Determine if bit(s) is not set. Returns VTRUE if yes,
	VFALSE otherwise.*/
	bool		IsNotSet(int nBits) const
		{ return (IsSet(nBits)) ? VFALSE : VTRUE; }

	/** Remove bits from the value and return resulting value.*/
	int		Remove(int nBits)
	{
		m_nBits &= ~nBits;
		return m_nBits;
	}

	/** Remove bits from the value and append the bits at nAppendBits.*/
	int		RemoveExclusive(int nBits,
								int nAppendBits)
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
	int		Restore()
	{
		m_nBits = m_nSaveBits;
		return m_nBits;
	}

	/** Toggle the specified bit. If it is on, turn it off, and vice-versa.*/
	int		Toggle(int nBit)
	{
		if ( IsSet(nBit) )
			Remove(nBit);
		else
			Append(nBit);

		return m_nBits;
	}

protected:
	/** Embedded Members.*/
	int		m_nBits;
	int		m_nSaveBits;
};

#endif /* ZBITMASK*/
