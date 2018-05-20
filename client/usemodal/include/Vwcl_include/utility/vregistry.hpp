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

#ifndef VREGISTRY
#define VREGISTRY

#include "../vstandard.h"
#include "../strings/vstring.hpp"

/** VRegistry supports reading from, and writing to, the system registry.*/
class VRegistry
{
public:
	/** Default constructor can be initialized with an existing registry
	key, or NULL. If hKey is not NULL, this class assumes ownership of it,
	and will close the key as needed.*/
	VRegistry(HKEY hKey = NULL)
		{ m_hKey = hKey; }

	/** Construct by opening the key identified by pszSubKey, within the
	parent of hKeyParent, using the access mode samDesired. If the key does
	not already exist, it will be created.*/
	VRegistry(	VSTRING_CONST	pszSubKey,
				HKEY			hKeyParent =
									HKEY_CURRENT_USER,
				REGSAM			samDesired =
									KEY_ALL_ACCESS)
	{
		m_hKey = NULL;
		CreateKey(pszSubKey, hKeyParent, samDesired);
	}

	/** Virtual destructor closes, as needed, the open registry key this
	class wraps.*/
	virtual ~VRegistry()
		{ Close(); }

	/** Close the key wrapped by this class, as needed.*/
	void			Close()
	{
		if ( m_hKey )
		{
			RegCloseKey(m_hKey);
			m_hKey = NULL;
		}
	}

	/** Given the root key (HKEY_CURRENT_USER or HKEY_LOCAL_MACHINE), and an
	application name, create or open the registry setting. Returns VTRUE on
	success, VFALSE on failure. This object should not be wrapping an
	existing key before entry to this function. This function will open or
	create a key under HKEY_CURRENT_USER or HKEY_LOCAL_MACHINE, in the
	Software subkey, follwed by pszCompanyName and pszAppTitle. For
	instance, if the company name is Teds Excellent Software and the
	application name is TotallyBogus, and the root key is HKEY_CURRENT_USER,
	this function would create or open the
	HKEY_CURRENT_USER\Software\Teds Excellent Software\TotallyBogus key.
	If pszAppTitle is NULL, VGetAppTitle() will be used.*/
	VBOOL			CreateAppKey(	VSTRING_CONST	pszCompanyName,
									VSTRING_CONST	pszAppTitle,
									HKEY			hKeyRoot =
														HKEY_CURRENT_USER)
	{
		VASSERT(VSTRLEN_CHECK(pszCompanyName))

		if ( pszCompanyName )
		{
			if ( !pszAppTitle )
				pszAppTitle = VGetAppTitle();

			VASSERT(VSTRLEN_CHECK(pszAppTitle))

			if ( pszAppTitle )
			{
				/* Build full path to key as
				Software\CompanyName\Application.*/
				VString s(VTEXT("Software\\"));
				s += pszCompanyName;
				s += VTEXT('\\');
				s += pszAppTitle;

				return	(s.GetErrorCount())
						? VFALSE
						: CreateKey(s, hKeyRoot);
			}
		}

		return VFALSE;
	}

	/** Create or open a key. If the key already exists, it will be opened.
	If not, it will be created. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			CreateKey(	VSTRING_CONST	pszSubKey,
								HKEY			hKeyParent =
													HKEY_CURRENT_USER,
								REGSAM			samDesired =
													KEY_ALL_ACCESS)
	{
		VASSERT(!m_hKey)
		VASSERT(pszSubKey)
		VASSERT(hKeyParent)

		if ( !m_hKey && pszSubKey && hKeyParent )
		{
			return (RegCreateKeyEx(	hKeyParent,
									pszSubKey,
									0,
									NULL,
									REG_OPTION_NON_VOLATILE,
									samDesired,
									NULL,
									&m_hKey,
									NULL) == ERROR_SUCCESS)
									? VTRUE : VFALSE;
		}

		return VFALSE;
	}

	/** Delete the subkey. All sub-keys below pszKey, and pszKey itself will
	be recursively deleted, even on Windows NT which does not directly
	support this. This will only work on NT if bCheckForNT is VTRUE. Returns
	VTRUE on success, VFALSE on failure.*/
	static VBOOL	DeleteKey(	HKEY			hParentKey,
								VSTRING_CONST	pszKey,
								VBOOL			bCheckForNT =
													VTRUE)
	{
		VASSERT(hParentKey)
		VASSERT(pszKey)

		if ( hParentKey && pszKey )
		{
			VBOOL bRecursive = VFALSE;

			if ( bCheckForNT )
			{
				OSVERSIONINFO				vi;
				vi.dwOSVersionInfoSize =	sizeof(vi);
				GetVersionEx(&vi);

				/* Set recursive flag if NT is found.*/
				if ( vi.dwPlatformId == VER_PLATFORM_WIN32_NT )
					bRecursive = VTRUE;
			}

			if ( bRecursive )
				return DeleteKeyNT(hParentKey, pszKey);
			else
				return	(RegDeleteKey(hParentKey, pszKey) == ERROR_SUCCESS)
						? VTRUE : VFALSE;
		}

		return VFALSE;
	}

	/** Same as DeleteKey(), but recursively deletes keys on Windows NT, or
	other Win32 OS's.*/
	static VBOOL	DeleteKeyNT(HKEY			hParentKey,
								VSTRING_CONST	pszKey)
	{
		VASSERT(hParentKey)
		VASSERT(pszKey)

		if ( hParentKey && pszKey )
		{
			/* Open the key to see if it has any sub-keys.*/
			VRegistry reg(	pszKey,
							hParentKey,
							KEY_ENUMERATE_SUB_KEYS | DELETE);

			if ( reg.GetKey() )
			{
				VCHAR	szSubKey[256];
				VDWORD	nSubKeyLength = VARRAY_SIZE(szSubKey);

				/* If this key has sub-key(s), delete them.*/
				while ( RegEnumKeyEx(	reg.GetKey(),
										0,
										szSubKey,
										&nSubKeyLength,
										NULL,
										NULL,
										NULL,
										NULL) == ERROR_SUCCESS )
				{
					nSubKeyLength = VARRAY_SIZE(szSubKey);
					DeleteKeyNT(reg.GetKey(), szSubKey);
				}

				/* Close this key.*/
				reg.Close();

				/* Delete key now.*/
				return	(RegDeleteKey(hParentKey, pszKey) == ERROR_SUCCESS)
						? VTRUE : VFALSE;
			}
		}

	   return VFALSE;
	}

	/** Same as above, but recursively delete pszKey, which is a child of
	the currently open key.*/
	VBOOL			DeleteSubKeyNT(VSTRING_CONST pszSubKey) const
		{ return DeleteKeyNT(m_hKey, pszSubKey); }

	/** Delete a subkey of this (open) registry key. All sub-keys below
	pszSubKey, and pszSubKey itself will be recursively deleted, even on
	Windows NT, which does not directly support this. This will only work on
	NT if bCheckForNT is VTRUE. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			DeleteSubKey(	VSTRING_CONST	pszSubKey,
									VBOOL			bCheckForNT =
														VTRUE) const
		{ return DeleteKey(m_hKey, pszSubKey, bCheckForNT); }

	/** Delete a value that is a child of the current (open) key.*/
	VBOOL			DeleteValue(VSTRING_CONST pszValue) const
	{
		VASSERT(m_hKey)

		if ( m_hKey )
			return	(RegDeleteValue(m_hKey, pszValue) == ERROR_SUCCESS)
					? VTRUE : VFALSE;

		return VFALSE;
	}

	/** Return a reference to the current (open) key.*/
	HKEY&			GetKey() const
		{ return (HKEY&)m_hKey; }

	/** Open an existing key. Unlike the CreateKey() functions, this method
	does not create the specified key if the key does not exist in the
	registry. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			OpenKey(	VSTRING_CONST	pszSubKey,
								HKEY			hKeyParent =
													HKEY_CURRENT_USER,
								REGSAM			samDesired =
													KEY_ALL_ACCESS)
	{
		VASSERT(!m_hKey)
		VASSERT(pszSubKey)
		VASSERT(hKeyParent)

		if ( !m_hKey && pszSubKey && hKeyParent )
		{
			return (RegOpenKeyEx(	hKeyParent,
									pszSubKey,
									0,
									samDesired,
									&m_hKey) == ERROR_SUCCESS)
									? VTRUE : VFALSE;
		}

		return VFALSE;
	}

	/** Read the BINARY key value and store the result in pData.*/
	VBOOL			ReadBinary(	VSTRING_CONST	pszValueKey,
								VVOIDPTR		pData,
								VUINT			nLength) const
	{
		VASSERT(m_hKey)
		VASSERT(pszValueKey)
		VASSERT(pData)
		VASSERT(nLength)

		if ( m_hKey && pszValueKey && pData && nLength )
		{
			VDWORD nSize = nLength;

			return (RegQueryValueEx(	m_hKey,
										pszValueKey,
										0,
										NULL,
										(BYTE*)pData,
										&nSize) == ERROR_SUCCESS)
										? VTRUE : VFALSE;
		}

		return VFALSE;
	}

	/** Read the DWORD key value. Returns -1 if not read (VDWORD must be
	cast to VINT). If nDefault is set, it will be returned instead of -1 on
	error.*/
	VDWORD			ReadDWORD(	VSTRING_CONST	pszValueKey,
								VDWORD			nDefault =
													-1) const
	{
		VASSERT(m_hKey)
		VASSERT(pszValueKey)

		if ( m_hKey && pszValueKey )
		{
			VDWORD nValue;
			VDWORD nSize = sizeof(nValue);

			return (RegQueryValueEx(	m_hKey,
										pszValueKey,
										0,
										NULL,
										(BYTE*)&nValue,
										&nSize) == ERROR_SUCCESS)
										? nValue : nDefault;
		}

		return nDefault;
	}

	/** Read the string key value. On success, the resulting string will be
	set into the VString object, and a pointer to it be returned. On
	failure, NULL will be returnd and strBuffer will not be modified.*/
	VSTRING			ReadString(	VSTRING_CONST	pszValueKey,
								VString&		strBuffer) const
	{
		VASSERT(m_hKey)

		if ( m_hKey )
		{
			VCHAR	sz[1024];
			VDWORD	nSize = sizeof(sz);

			if ( RegQueryValueEx(	m_hKey,
									pszValueKey,
									0,
									NULL,
									(BYTE*)sz,
									&nSize) == ERROR_SUCCESS )
				return strBuffer.String(sz);
		}

		return NULL;
	}

	/** Write the BINARY key value.*/
	VBOOL			WriteBinary(	VSTRING_CONST	pszValueKey,
									VVOIDPTR		pData,
									VUINT			nLength) const
	{
		VASSERT(m_hKey)
		VASSERT(pData)
		VASSERT(nLength)

		if ( m_hKey && pData && nLength )
		{
			return (RegSetValueEx(	m_hKey,
									pszValueKey,
									0,
									REG_BINARY,
									(BYTE*)pData, nLength) == ERROR_SUCCESS)
									? VTRUE : VFALSE;
		}

		return VFALSE;
	}

	/** Write the DWORD key value, or if it is 0, write nDefault.*/
	VBOOL			WriteDWORD(	VSTRING_CONST	pszValueKey,
								VDWORD			nValue,
								VDWORD			nDefault =
													0) const
	{
		VASSERT(m_hKey)

		if ( m_hKey )
		{
			return (RegSetValueEx(	m_hKey,
									pszValueKey,
									0,
									REG_DWORD,
									(BYTE*)((nValue) ? &nValue : &nDefault),
									sizeof(nValue)) == ERROR_SUCCESS)
									? VTRUE : VFALSE;
		}

		return VFALSE;
	}

	/** Write the string key value, or if NULL or 0 length, delete the value.*/
	VBOOL			WriteString(	VSTRING_CONST pszValueKey,
									VSTRING_CONST pszValue) const
	{
		VASSERT(m_hKey)

		if ( m_hKey )
		{
			VDWORD nLength = VSTRLEN_CHECK(pszValue);

			if ( nLength )
				return (RegSetValueEx(	m_hKey,
										pszValueKey,
										0,
										REG_SZ,
										(BYTE*)pszValue,
										nLength * sizeof(VCHAR)) ==
										ERROR_SUCCESS)
										? VTRUE : VFALSE;

			return DeleteValue(pszValueKey);
		}

		return VFALSE;
	}

	/** Create a subkey and write a default value to it. (Good for OLE
	object registration).*/
	VBOOL			WriteSubKey(	VSTRING_CONST pszSubKey,
									VSTRING_CONST pszValue) const
	{
		VASSERT(m_hKey)
		VASSERT(pszSubKey)

		if ( m_hKey && pszSubKey )
		{
			VRegistry reg;

			if ( reg.CreateKey(pszSubKey, m_hKey, KEY_WRITE) )
			{
				reg.WriteString((VSTRING_CONST)NULL, pszValue);
				return VTRUE;
			}
		}

		return VFALSE;
	}
protected:
	/** Embedded Members.*/
	HKEY			m_hKey;
};

#endif /* VREGISTRY*/
