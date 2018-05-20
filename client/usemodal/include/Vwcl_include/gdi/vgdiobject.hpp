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

#ifndef VGDIOBJECT
#define VGDIOBJECT

#include "../vstandard.h"

/** VGDIObject is a base class for wrapping all Windows GDI objects such as
fonts, pens, and brushes.*/
class VGDIObject
{
public:
	/** Standard  constructor initializes the class with an existing GDI
	object or NULL. If the internal GDI object should not be deleted by
	this class during destruction, pass VFALSE for bDeleteObject. Objects
	of this type are normally selected into one or more device context
	objects using the VGDIObjectSelector class.*/
	VGDIObject(	HGDIOBJ	hGDIObject =
							NULL,
				VBOOL	bDeleteObject =
							VTRUE)
	{
		/* Initialize.*/
		m_hGDIObject =		hGDIObject;
		m_bDeleteObject =	bDeleteObject;
	}

	/** Virtual destructor deletes the internal GDI object as needed.*/
	virtual ~VGDIObject()
		{ Delete(); }

	/** Delete the GDI object currently wrapped by this class, if any. This
	function will not delete objects that were marked as non-deletable
	during construction, or if the internal m_bDeleteObject flag is set to
	VFALSE. See the SetHandle() function for more information about why
	this function is implemented.*/
	virtual void	Delete()
	{
		if ( m_hGDIObject && m_bDeleteObject )
		{
			DeleteObject(m_hGDIObject);
			m_hGDIObject = NULL;
		}
	}

	/** Return a reference to the handle of the wrapped GDI object.*/
	HGDIOBJ&		GetHandle() const
		{ return (HGDIOBJ&)m_hGDIObject; }

	/** Function to set the internal object handle. This function will
	delete any previous GDI object held in this class. This can be used to
	re-use VGDIObject classes that wrap different logical GDI objects. In a
	pure OOP world, this would not be considered good behaviour. But in
	reality, this offers a great deal of functionality to objects that
	expose internal GDI objects that can be manipulated. If we did not
	allow this to occur, the end result would be many dynamically allocated
	objects used to similuate this behaviour. For instance, if class A
	allows customization of a VBrush object, without this ability that
	brush would have to be deleted and reallocated with the new settings.
	This adds an obvious problem when dealing with GDI objects in that it
	makes it more prone to allocation and deallocation errors, and forces
	the use of pointers when references would be preferred.*/
	VBOOL			SetHandle(HGDIOBJ hGDIObject)
	{
		/* Delete and current object wrapped by this class.*/
		Delete();

		/* hGDIObject should be valid.*/
		VBOOL bValidObject =
			(hGDIObject && GetObjectType(hGDIObject) != 0) ? VTRUE : VFALSE;
		VASSERT(bValidObject)

		/* Save handle.*/
		m_hGDIObject = (bValidObject) ? hGDIObject : NULL;

		return (m_hGDIObject) ? VTRUE : VFALSE;
	}

protected:
	/** Get object information. Only used by derived classes who know the
	data type they are trying to get.*/
	VBOOL			GetInfo(	VINT		nBufferSize,
								VVOIDPTR	pBuffer) const
	{
		VASSERT(nBufferSize)
		VASSERT(pBuffer)

		if (	(m_hGDIObject && nBufferSize && pBuffer) &&
				GetObject(m_hGDIObject, nBufferSize, pBuffer) )
			return VTRUE;

		return VFALSE;
	}

	/** Embedded Members.*/
	HGDIOBJ			m_hGDIObject;
	VBOOL			m_bDeleteObject;
};

#endif /* VGDIOBJECT*/
