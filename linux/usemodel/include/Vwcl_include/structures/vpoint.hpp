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

#ifndef VPOINT
#define VPOINT

#include "../vstandard.h"

/** Macro to create an empty VPoint object (coordinates of 0, 0).*/
#define VPOINT_EMPTY				VPoint(0,0)

/** Macro to retrieve the X and Y coordinates of a VPoint object.*/
#define VPOINT_BREAK(p)				p.GetX(), p.GetY()

/** VPoint provides a wrapper with enhancements to the POINT structure.*/
class VPoint
{
public:
	/** Construct with existing X and Y coordinates, or default of 0, 0.*/
	VPoint(	VLONG nXCoordinate =
					0,
			VLONG nYCoordinate =
					0)
		{ Set(nXCoordinate, nYCoordinate); }

	/** Construct by copying coordinates from an existing VPoint object.*/
	VPoint(VPoint const& pt)
		{ Set(pt); }

	/** Construct by converting coordinates from an existing POINT structure.*/
	VPoint(POINT const& pt)
		{ Set(pt); }

	/** Virtual destructor does nothing.*/
	virtual ~VPoint()
		{;}

	/** Same as calling IsEqual() to determine if an existing VPoint
	object has the same coordinates as this point.*/
	VBOOL operator	==(VPoint const& pt) const
		{ return IsEqual(pt); }

	/** Same as calling !IsEqual() to determine if an existing VPoint
	object does not have the same coordinates as this point.*/
	VBOOL operator	!=(VPoint const& pt) const
		{ return !IsEqual(pt); }

	/** Same as calling Set() to set the coordinates of this object to
	those of an existing VPoint object.*/
	void operator =(VPoint const& pt)
		{ Set(pt); }

	/** Add or subtract nDistance to or from the X coordinate.*/
	void		AddX(VLONG nDistance = 1)
		{ m_Point.x += nDistance; }

	/** Add or subtract nDistance to or from the Y coordinate.*/
	void		AddY(VLONG nDistance = 1)
		{ m_Point.y += nDistance; }

	/** Set the coordinates of this point to 0, 0.*/
	void		Empty()
		{ m_Point.x = m_Point.y = 0; }

	/** Set the point to be the current cursor position.*/
	void		GetCursorPosition()
		{ ::GetCursorPos(&m_Point); }

	/** Return a POINT structure reference to this object.*/
	POINT&		GetPOINT() const
		{ return (POINT&)m_Point; }

	/** Return the a pointer to the internal POINT structure.*/
	POINT*		GetPointer() const
		{ return (POINT*)&m_Point; }

	/** Return a reference to the X coordinate of this point.*/
	VLONG&		GetX() const
		{ return (VLONG&)m_Point.x; }

	/** Return a reference to the Y coordinate of this point.*/
	VLONG&		GetY() const
		{ return (VLONG&)m_Point.y; }

	/** Returns VTRUE if the coordinates of this point are 0, 0,
	VFALSE otherwise.*/
	VBOOL		IsEmpty() const
		{ return (!m_Point.x && !m_Point.y) ? VTRUE : VFALSE; }

	/** Returns VTRUE if the coordinates of this point are not 0, 0,
	VFALSE otherwise.*/
	VBOOL		IsNotEmpty() const
		{ return (IsEmpty()) ? VFALSE : VTRUE; }

	/** Returns VTRUE if an existing VPoint object has the same
	coordinates as this point, VFALSE otherwise.*/
	VBOOL		IsEqual(VPoint const& pt) const
	{
		return	(pt.GetX() == m_Point.x && pt.GetY() == m_Point.y)
				? VTRUE
				: VFALSE;
	}

	/** Offset the X coordinate of this point by nDistanceX, and the
	Y coordinate by nDistanceY. Either can be negative, which will subtract.*/
	void		Offset(VLONG nDistanceX, VLONG nDistanceY)
	{
		m_Point.x += nDistanceX;
		m_Point.y += nDistanceY;
	}

	/** Set the coordinates of this point to be the values nXCoordinate
	and nYCoordinate.*/
	void		Set(VLONG nXCoordinate,
					VLONG nYCoordinate)
	{
		m_Point.x = nXCoordinate;
		m_Point.y = nYCoordinate;
	}

	/** Set the coordinates of this point to be the values from an
	existing VPoint object.*/
	void		Set(VPoint const& pt)
		{ Set(pt.GetPOINT()); }

	/** Set the coordinates of this point to be the values from an
	existing POINT structure.*/
	void		Set(POINT const& pt)
		{ Set(pt.x, pt.y); }

	/** Subtract nDistance to or from the X coordinate.*/
	void		SubtractX(VLONG nDistance = 1)
		{ AddX(-nDistance); }

	/** Subtract nDistance to or from the Y coordinate.*/
	void		SubtractY(VLONG nDistance = 1)
		{ AddY(-nDistance); }

protected:
	/** Embedded Members.*/
	POINT		m_Point;
};

#endif /* VPOINT*/
