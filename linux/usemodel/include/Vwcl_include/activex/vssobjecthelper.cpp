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

#include "../vstandard.h"
#include "vunknown.hpp"
#include "vssobjecthelper.hpp"
#include "vssstoragehelper.hpp"

VULONG VSSObjectHelper::Release()
{
	VASSERT(m_pWrappedObject)

	/* First AddRef() the object for our own use.*/
	AddRef();

	/* Call release on the object now and save result.*/
	VULONG nResult = m_pWrappedObject->Release();

	/* Should the object be completely released? If still 1, yes.*/
	if ( nResult == 1 )
	{
		/* If this is a storage, delete child elements VStorage derived
		class allocated.*/
		if ( GetRTTI() == VSS_RTTI_STORAGE )
		{
			VPtrArray& array =
				((VSSStorageHelper*)this)->GetCollection();

			VPTRARRAY_INDEX	nSize =	array.Size();

			for ( VPTRARRAY_INDEX i = 0; i < nSize; i++ )
				VDELETE (VSSObjectHelper*)array.At(i);
		}

		/* Should we auto-commit this element?*/
		if ( m_bAutoCommit )
			Commit();

		VString strOldName;

		/* Get the current name now if we are going to rename it.*/
		if ( m_pParentStorage && m_strRename.IsNotEmpty() )
			GetName(strOldName, VFALSE);

		/* Do the final release now and set nResult to 0.*/
		m_pWrappedObject->Release();
		nResult = 0;

		/* Do we need to rename this element before permanently going away?*/
		if ( strOldName.IsNotEmpty() )
			m_pParentStorage->RenameElement(
				VStringConverter(strOldName),
				VStringConverter(m_strRename));

		/* Reset internal member variables.*/
		ResetMembers(NULL, NULL);
	}

	/* Return the result.*/
	return nResult;
}
