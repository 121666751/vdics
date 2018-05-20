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

#ifndef ZREGISTRY_H
#define ZREGISTRY_H

#include <windows.h>
#include <string>

/** ZRegistry supports reading from, and writing to, the system registry.*/
class ZRegistry
{
public:
	/** Default constructor can be initialized with an existing registry
	key, or NULL. If hKey is not NULL, this class assumes ownership of it,
	and will close the key as needed.*/
	ZRegistry(HKEY hKey = NULL)
		{ m_hKey = hKey; }

	/** Construct by opening the key identified by pszSubKey, within the
	parent of hKeyParent, using the access mode samDesired. If the key does
	not already exist, it will be created.*/
	ZRegistry(	const char*	pszSubKey,
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
	virtual ~ZRegistry()
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
	application name, create or open the registry setting. Returns TRUE on
	success, FALSE on failure. This object should not be wrapping an
	existing key before entry to this function. This function will open or
	create a key under HKEY_CURRENT_USER or HKEY_LOCAL_MACHINE, in the
	Software subkey, follwed by pszCompanyName and pszAppTitle. For
	instance, if the company name is Teds Excellent Software and the
	application name is TotallyBogus, and the root key is HKEY_CURRENT_USER,
	this function would create or open the
	HKEY_CURRENT_USER\Software\Teds Excellent Software\TotallyBogus key.
	If pszAppTitle is NULL, VGetAppTitle() will be used.*/
	BOOL			CreateAppKey(	const char*	pszCompanyName,
									const char*	pszAppTitle,
									HKEY			hKeyRoot =
														HKEY_CURRENT_USER)
	{
		//VASSERT(strlen(pszCompanyName))

		if ( pszCompanyName )
		{
			if ( !pszAppTitle )
				pszAppTitle = "notitle";

			//VASSERT(strlen(pszAppTitle))

			if ( pszAppTitle )
			{
				/* Build full path to key as
				Software\CompanyName\Application.*/
				std::string s("Software\\");
				s += pszCompanyName;
				s += '\\';
				s += pszAppTitle;

				return CreateKey(s.c_str(), hKeyRoot);
			}
		}

		return FALSE;
	}

	/** Create or open a key. If the key already exists, it will be opened.
	If not, it will be created. Returns TRUE on success, FALSE on failure.*/
	BOOL			CreateKey(	const char*	pszSubKey,
								HKEY			hKeyParent =
													HKEY_CURRENT_USER,
								REGSAM			samDesired =
													KEY_ALL_ACCESS)
	{
		//VASSERT(!m_hKey)
		//VASSERT(pszSubKey)
		//VASSERT(hKeyParent)

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
									? TRUE : FALSE;
		}

		return FALSE;
	}

	/** Delete the subkey. All sub-keys below pszKey, and pszKey itself will
	be recursively deleted, even on Windows NT which does not directly
	support this. This will only work on NT if bCheckForNT is TRUE. Returns
	TRUE on success, FALSE on failure.*/
	static BOOL	DeleteKey(	HKEY			hParentKey,
								const char*	pszKey,
								BOOL			bCheckForNT =
													TRUE)
	{
		//VASSERT(hParentKey)
		//VASSERT(pszKey)

		if ( hParentKey && pszKey )
		{
			BOOL bRecursive = FALSE;

			if ( bCheckForNT )
			{
				OSVERSIONINFO				vi;
				vi.dwOSVersionInfoSize =	sizeof(vi);
				GetVersionEx(&vi);

				/* Set recursive flag if NT is found.*/
				if ( vi.dwPlatformId == VER_PLATFORM_WIN32_NT )
					bRecursive = TRUE;
			}

			if ( bRecursive )
				return DeleteKeyNT(hParentKey, pszKey);
			else
				return	(RegDeleteKey(hParentKey, pszKey) == ERROR_SUCCESS)
						? TRUE : FALSE;
		}

		return FALSE;
	}

	/** Same as DeleteKey(), but recursively deletes keys on Windows NT, or
	other Win32 OS's.*/
	static BOOL	DeleteKeyNT(HKEY			hParentKey,
								const char*	pszKey)
	{
		//VASSERT(hParentKey)
		//VASSERT(pszKey)

		if ( hParentKey && pszKey )
		{
			/* Open the key to see if it has any sub-keys.*/
			ZRegistry reg(	pszKey,
							hParentKey,
							KEY_ENUMERATE_SUB_KEYS | DELETE);

			if ( reg.GetKey() )
			{
				char	szSubKey[256];
				DWORD	nSubKeyLength = sizeof(szSubKey);

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
					nSubKeyLength = sizeof(szSubKey);
					DeleteKeyNT(reg.GetKey(), szSubKey);
				}

				/* Close this key.*/
				reg.Close();

				/* Delete key now.*/
				return	(RegDeleteKey(hParentKey, pszKey) == ERROR_SUCCESS)
						? TRUE : FALSE;
			}
		}

	   return FALSE;
	}

	/** Same as above, but recursively delete pszKey, which is a child of
	the currently open key.*/
	BOOL			DeleteSubKeyNT(const char* pszSubKey) const
		{ return DeleteKeyNT(m_hKey, pszSubKey); }

	/** Delete a subkey of this (open) registry key. All sub-keys below
	pszSubKey, and pszSubKey itself will be recursively deleted, even on
	Windows NT, which does not directly support this. This will only work on
	NT if bCheckForNT is TRUE. Returns TRUE on success, FALSE on failure.*/
	BOOL			DeleteSubKey(	const char*	pszSubKey,
									BOOL			bCheckForNT =
														TRUE) const
		{ return DeleteKey(m_hKey, pszSubKey, bCheckForNT); }

	/** Delete a value that is a child of the current (open) key.*/
	BOOL			DeleteValue(const char* pszValue) const
	{
		//VASSERT(m_hKey)

		if ( m_hKey )
			return	(RegDeleteValue(m_hKey, pszValue) == ERROR_SUCCESS)
					? TRUE : FALSE;

		return FALSE;
	}

	/** Return a reference to the current (open) key.*/
	HKEY&			GetKey() const
		{ return (HKEY&)m_hKey; }

	/** Open an existing key. Unlike the CreateKey() functions, this method
	does not create the specified key if the key does not exist in the
	registry. Returns TRUE on success, FALSE on failure.*/
	BOOL			OpenKey(	const char*	pszSubKey,
								HKEY			hKeyParent =
													HKEY_CURRENT_USER,
								REGSAM			samDesired =
													KEY_ALL_ACCESS)
	{
		//VASSERT(!m_hKey)
		//VASSERT(pszSubKey)
		//VASSERT(hKeyParent)

		if ( !m_hKey && pszSubKey && hKeyParent )
		{
			return (RegOpenKeyEx(	hKeyParent,
									pszSubKey,
									0,
									samDesired,
									&m_hKey) == ERROR_SUCCESS)
									? TRUE : FALSE;
		}

		return FALSE;
	}

	/** Read the BINARY key value and store the result in pData.*/
	BOOL			ReadBinary(	const char*	pszValueKey,
								void*		pData,
								UINT			nLength) const
	{
		//VASSERT(m_hKey)
		//VASSERT(pszValueKey)
		//VASSERT(pData)
		//VASSERT(nLength)

		if ( m_hKey && pszValueKey && pData && nLength )
		{
			DWORD nSize = nLength;

			return (RegQueryValueEx(	m_hKey,
										pszValueKey,
										0,
										NULL,
										(BYTE*)pData,
										&nSize) == ERROR_SUCCESS)
										? TRUE : FALSE;
		}

		return FALSE;
	}

	/** Read the DWORD key value. Returns -1 if not read (DWORD must be
	cast to VINT). If nDefault is set, it will be returned instead of -1 on
	error.*/
	DWORD			ReadDWORD(	const char*	pszValueKey,
								DWORD			nDefault =
													-1) const
	{
		//VASSERT(m_hKey)
		//VASSERT(pszValueKey)

		if ( m_hKey && pszValueKey )
		{
			DWORD nValue;
			DWORD nSize = sizeof(nValue);

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
	set into the std::string object, and a pointer to it be returned. On
	failure, NULL will be returnd and strBuffer will not be modified.*/
	std::string			ReadString(	const char*	pszValueKey,
								std::string&		strBuffer) const
	{
		//VASSERT(m_hKey)

		if ( m_hKey )
		{
			char	sz[1024];
			DWORD	nSize = sizeof(sz);

			if ( RegQueryValueEx(	m_hKey,
									pszValueKey,
									0,
									NULL,
									(BYTE*)sz,
									&nSize) == ERROR_SUCCESS )
			{
				strBuffer = sz;
				return strBuffer;
			}
		}

		return NULL;
	}

	/** Write the BINARY key value.*/
	BOOL			WriteBinary(	const char*	pszValueKey,
									void*		pData,
									UINT			nLength) const
	{
		//VASSERT(m_hKey)
		//VASSERT(pData)
		//VASSERT(nLength)

		if ( m_hKey && pData && nLength )
		{
			return (RegSetValueEx(	m_hKey,
									pszValueKey,
									0,
									REG_BINARY,
									(BYTE*)pData, nLength) == ERROR_SUCCESS)
									? TRUE : FALSE;
		}

		return FALSE;
	}

	/** Write the DWORD key value, or if it is 0, write nDefault.*/
	BOOL			WriteDWORD(	const char*	pszValueKey,
								DWORD			nValue,
								DWORD			nDefault =
													0) const
	{
		//VASSERT(m_hKey)

		if ( m_hKey )
		{
			return (RegSetValueEx(	m_hKey,
									pszValueKey,
									0,
									REG_DWORD,
									(BYTE*)((nValue) ? &nValue : &nDefault),
									sizeof(nValue)) == ERROR_SUCCESS)
									? TRUE : FALSE;
		}

		return FALSE;
	}

	/** Write the string key value, or if NULL or 0 length, delete the value.*/
	BOOL			WriteString(	const char* pszValueKey,
									const char* pszValue) const
	{
		//VASSERT(m_hKey)

		if ( m_hKey )
		{
			DWORD nLength = strlen(pszValue);

			if ( nLength )
				return (RegSetValueEx(	m_hKey,
										pszValueKey,
										0,
										REG_SZ,
										(BYTE*)pszValue,
										nLength * sizeof(char)) ==
										ERROR_SUCCESS)
										? TRUE : FALSE;

			return DeleteValue(pszValueKey);
		}

		return FALSE;
	}

	/** Create a subkey and write a default value to it. (Good for OLE
	object registration).*/
	BOOL			WriteSubKey(	const char* pszSubKey,
									const char* pszValue) const
	{
		//VASSERT(m_hKey)
		//VASSERT(pszSubKey)

		if ( m_hKey && pszSubKey )
		{
			ZRegistry reg;

			if ( reg.CreateKey(pszSubKey, m_hKey, KEY_WRITE) )
			{
				reg.WriteString((const char*)NULL, pszValue);
				return TRUE;
			}
		}

		return FALSE;
	}
protected:
	/** Embedded Members.*/
	HKEY			m_hKey;
};

#endif /* ZRegistry*/
