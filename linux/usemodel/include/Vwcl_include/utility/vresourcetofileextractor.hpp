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

#ifndef VRESOURCETOFILEEXTRACTOR
#define VRESOURCETOFILEEXTRACTOR

#include "../vstandard.h"
#include "../strings/vpathstring.hpp"
#include "../strings/vstring.hpp"

/** VResourceToFileExtractor extracts a file, which is embedded in resources,
to an external file. It will delete this file when done, and can operate on
files in a temporary location.*/
class VResourceToFileExtractor
{
public:
	/** Default constructor does nothing.*/
	VResourceToFileExtractor()
		{;}

	/** Virtual destructor deletes the extracted file. If the file should
	not be deleted, you can call GetFileName() and set the string reference
	returned to be an empty string using VString::Empty(), or you can
	override this destructor and do nothing.*/
	virtual ~VResourceToFileExtractor()
		{ DeleteFile(); }

	/** Delete the extracted file, as needed. This will also reset the
	internal file name to be an empty string.*/
	void		DeleteFile()
	{
		if ( m_strFileName.IsNotEmpty() )
		{
			/* Delete the file.*/
			VREMOVE(m_strFileName);

			/* It should not exist now! If it does, it probably means that
			another part of the application still has the file open, or
			has a lock on it. Make sure that this object goes out of scope
			AFTER code that uses the extracted file is done with it.*/
			VASSERT(!VDoesFileExist(m_strFileName))

			m_strFileName.Empty();
		}
	}

	/** Extract an embedded resource to a file. If pszFileName is NULL, the
	resource will be extracted and placed into a temporary file. Returns
	VTRUE on success, VFALSE on failure. pszType specifies the resource
	type name, like "JPEG", and pszName specifies the resource name itself,
	like "splash.jpg". If hResource is NULL, VGetResourceHandle() will be
	used. The previous extracted file, if any, will be deleted before
	extracting the new resource. On failure, the string returned from
	GetFileName() will be empty.*/
	VBOOL		Extract(VSTRING_CONST	pszType,
						VSTRING_CONST	pszName,
						VSTRING_CONST	pszFileName =
											NULL,
						HINSTANCE		hResource =
											NULL)
	{
		/* Delete any existing file.*/
		DeleteFile();
		VASSERT(m_strFileName.IsEmpty())

		VUINT nSize = 0;
		
		VVOIDPTR pvRes =
			VLoadAndLockResource(pszType, pszName, &nSize, hResource);

		if ( pvRes )
		{
			if ( !pszFileName )
			{
				/* Get temporary file name?*/
				VPathString ps(m_strFileName);
				ps.LoadTemporaryFileName();
			}
			else
				m_strFileName = pszFileName;

			/* Make sure we have a valid file name.*/
			if ( m_strFileName.IsNotEmpty() )
			{
				/* Open the file to write to, in binary mode.*/
				FILE* pFILE = VFOPEN(m_strFileName, VTEXT("wb"));

				if ( pFILE )
				{
					if ( fwrite(pvRes, 1, nSize, pFILE) )
						fclose(pFILE);
					else
					{
						fclose(pFILE);
						DeleteFile();
					}
				}
				else
					DeleteFile();
			}
		}

		return m_strFileName.IsNotEmpty();
	}

	/** Return a refence to the file name where the resource was extracted
	to. This will be an empty string if nothing was previously extracted.*/
	VString&	GetFileName() const
		{ return (VString&)m_strFileName; }

protected:
	/** Embedded Members.*/
	VString		m_strFileName;
};

#endif /* VRESOURCETOFILEEXTRACTOR*/
