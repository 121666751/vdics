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
Other Author(s) of this source code file: Laszlo Radanyi (lrd@plea.se)
*/

#ifndef VGDIOBJECTSELECTOR
#define VGDIOBJECTSELECTOR

#include "../vstandard.h"
#include "vdc.hpp"
#include "vgdiobject.hpp"

/** VGDIObjectSelector assists with the selection and un-selection of GDI
objects (like pens and brushes) into and out of a device context. It
verifies that objects selected into a DC during construction are
unselected from the DC during object destruction. This helps to prevent GDI
resource leaks and situations where GDI objects are deleted while they are
selected into a device context, which is a potential crash situation.
VGDIObjectSelector objects are usually short lived, and should remain in
scope for less time than the device context and the GDI object they select.
A typical usage of this class would be as follows, assuming you have a VDC
object named dc:

VPen pen1(VTRUE, PS_SOLID, 1, RGB(255,255,255));
VGDIObjectSelector os1(dc, pen1);
...(draw with pen1)...
VPen pen2(VTRUE, PS_DOT, 10, RGB(255,0,0));
VGDIObjectSelector os2(dc, pen2);
...(draw with pen2)...
VGDIObjectSelector os3(dc, GetStockObject(WHITE_BRUSH));
...(draw with white brush)...

See the constructor and destructor comments for more information about the
proper usage of this class, and the ways in which is attempts to verify
propery device context selection.*/
class VGDIObjectSelector
{
public:
	/** Initialize by selecting a VGDIObject into the devic context at dc.
	The VDC object at dc must remain in scope beyond the lifetime of this
	object. The object specified for gdiObject must be valid and must have
	already been initialized or created as needed.*/
	VGDIObjectSelector(	VDC&		dc,
						VGDIObject&	gdiObject)
		{ Init(dc, gdiObject.GetHandle()); }

	/** Initialize by selecting a Windows GDI object into the devic context
	at dc. The VDC object at dc must remain in scope beyond the lifetime of
	this object. The object specified for hObject must be valid and must
	have already been initialized or created as needed.*/
	VGDIObjectSelector(	VDC&	dc,
						HGDIOBJ	hObject)
		{ Init(dc, hObject); }

	/** Virtual destructor restores the previously selected object, which
	must still be a valid GDI object. This insures that the object that was
	selected into the device context by this object can safely be deleted.*/
	virtual ~VGDIObjectSelector()
	{
		/* The DC must always be valid! It must outlive this object in scope.*/
		VASSERT(m_pDC)
		VASSERT(m_pDC->GetHandle())

		/* Did we initialize properly? We did if we have an old object
		to select. It must still be valid (not deleted)!*/
		VBOOL bValidObject =
			(m_hOldObject && GetObjectType(m_hOldObject) != 0)
				? VTRUE : VFALSE;
		VASSERT(bValidObject)

		/* Select it now?*/
		if ( bValidObject )
			m_pDC->SelectObject(m_hOldObject);
	}

protected:
	/** Internal function initializes the class for all constructors.*/
	void	Init(	VDC&	dc,
					HGDIOBJ	hObject)
	{
		/* The DC must always be valid!*/
		VASSERT(dc.GetHandle())

		/* The GDI object must be valid!*/
		VBOOL bValidObject =
			(hObject && GetObjectType(hObject) != 0) ? VTRUE : VFALSE;
		VASSERT(bValidObject)

		/* Initialize and select hObject.*/
		m_pDC =			&dc;
		m_hOldObject =	(bValidObject) ? dc.SelectObject(hObject) : NULL;
	}

	/** Embedded Members.*/
	VDC*	m_pDC;
	HGDIOBJ	m_hOldObject;
};

#endif /* VGDIOBJECTSELECTOR*/
