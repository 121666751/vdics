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

#ifndef VRECURSIVEDIRECTORYENUMERATOR
#define VRECURSIVEDIRECTORYENUMERATOR

#include "../vstandard.h"

/** VRecursiveDirectoryEnumeratorCallback is used to receive directory and
file notifications when a VRecursiveDirectoryEnumerator object is walking a
directory structure.*/
class VRecursiveDirectoryEnumeratorCallback
{
public:
	/** Override to receive directory or file information. Return VTRUE to
	continue enumeration, VFALSE to end it. Derived class can modify
	pszName, but MUST set it back to original string before returning.*/
	virtual VBOOL	OnRecursiveFound(	VSTRING					pszName,
										VSTRING_CONST			pszMask,
										WIN32_FIND_DATA const&	FindData,
										VBOOL					bDirectory,
										VVOIDPTR				pVoid1,
										VVOIDPTR				pVoid2) = 0;
};

/** VRecursiveDirectoryEnumerator iterates directories and files, calling
the VRecursiveDirectoryEnumeratorCallback::OnRecursiveFound() virtual
function each time a file system object is found.*/
class VRecursiveDirectoryEnumerator
{
public:
	/** Default constructor initializes internal members.*/
	VRecursiveDirectoryEnumerator()
		{ Reset(); }

	/** Virtual destructor does nothing.*/
	virtual ~VRecursiveDirectoryEnumerator()
		{;}

	/** Enumerate only subdirectories, never files, below the directory
	specified by pszDirectory. pVoid1 and pVoid2 are placeholders for any
	information you would like your derived
	VRecursiveDirectoryEnumeratorCallback class to receive when this class
	calls its OnRecursiveFound() virtual function.*/
	void			EnumDirectories(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszDirectory,
		VVOIDPTR								pVoid1 =
													NULL,
		VVOIDPTR								pVoid2 =
													NULL)
		{ EnumFiles(callback, pszDirectory, VTEXT("\1"), pVoid1, pVoid2); }

	/** Function to enumerate files and sub-directories in a single
	directory specified by pszDirectory. If pszMask is "\1", only
	subdirectories will be returned. pVoid1 and pVoid2 are placeholders for
	any information you would like your derived
	VRecursiveDirectoryEnumeratorCallback class to receive when this class
	calls its OnRecursiveFound() virtual function.*/
	void			EnumFiles(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszDirectory,
		VSTRING_CONST							pszMask =
													NULL,
		VVOIDPTR								pVoid1 =
													NULL,
		VVOIDPTR								pVoid2 =
													NULL)
	{
		Init(pszDirectory);
		EnumFilesInternal(callback, pszMask, pVoid1, pVoid2);
		SetErrorMode(m_nOldErrorMode);
		Reset();
	}

	/** Return the current recursion count.*/
	VUINT			GetRecursionCount() const
		{ return m_nRecursionCount; }

	/** Start recursing files at the directory specified by pszDirectory.
	pVoid1 and pVoid2 are placeholders for any information you would like
	your derived VRecursiveDirectoryEnumeratorCallback class to receive
	when this class calls its OnRecursiveFound() virtual function.*/
	void			Recurse(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszDirectory,
		VSTRING_CONST							pszMask =
													NULL,
		VVOIDPTR								pVoid1 =
													NULL,
		VVOIDPTR								pVoid2 =
													NULL)
	{
		Init(pszDirectory);
		RecurseInternal(callback, pszMask, pVoid1, pVoid2);
		SetErrorMode(m_nOldErrorMode);
		Reset();
	}

	/** Start recursing ONLY directories starting at pszDirectory. Only
	directories will be sent to the callback function, never files. pVoid1
	and pVoid2 are placeholders for any information you would like your
	derived VRecursiveDirectoryEnumeratorCallback class to receive when this
	class calls its OnRecursiveFound() virtual function.*/
	void			RecurseDirectories(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszDirectory,
		VVOIDPTR								pVoid1 =
													NULL,
		VVOIDPTR								pVoid2 =
													NULL)
	{
		Init(pszDirectory);
		RecurseInternal(callback, VTEXT("\1"), pVoid1, pVoid2);
		SetErrorMode(m_nOldErrorMode);
		Reset();
	}

private:
	/** Internal function to enumerate files and sub-directories in a single
	directory. Returns VTRUE if completed, VFALSE otherwise.*/
	VBOOL			EnumFilesInternal(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszMask,
		VVOIDPTR								pVoid1,
		VVOIDPTR								pVoid2)
	{
		/* Get items in this directory.*/
		WIN32_FIND_DATA	fd;
		VBOOL bUserCancelled =	VFALSE;
		VINT					nDirectoryLen = VSTRLEN(m_szBuffer);
		
		VBOOL bDirectoriesOnly =
			(pszMask && VSTRCMP(pszMask, VTEXT("\1")) == 0) ? VTRUE : VFALSE;

		/* Append path separator.*/
		VSTRCPY(&m_szBuffer[nDirectoryLen], VFILE_PATH_SEP);

		/* Append search pattern.*/
		VSTRCPY(	&m_szBuffer[nDirectoryLen + 1],
					(!pszMask || bDirectoriesOnly)
					? VTEXT("*.*")
					: pszMask);

		/* Find the first file.*/
		HANDLE hFind = FindFirstFile(m_szBuffer, &fd);

		while ( !bUserCancelled && hFind != INVALID_HANDLE_VALUE )
		{
			if ( IsValidName(fd.cFileName) )
			{
				VBOOL bIsFile =
					(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					? VFALSE
					: VTRUE;

				/* Don't call OnRecursiveFound() with a file if only
				directories are asked for, otherwise call virtual function
				for notification after building full path.*/
				if (	(bIsFile && !bDirectoriesOnly) ||
						(!bIsFile && bDirectoriesOnly) )
				{
					VSTRCPY(&m_szBuffer[nDirectoryLen + 1], fd.cFileName);

					bUserCancelled =
						!callback.OnRecursiveFound(	m_szBuffer,
													pszMask,
													fd,
													!bIsFile,
													pVoid1,
													pVoid2);
				}
			}

			/* Find next and keep looping.*/
			if ( !bUserCancelled )
			{
				if ( !FindNextFile(hFind, &fd) )
					break;
			}
		}

		if ( hFind != INVALID_HANDLE_VALUE )
			FindClose(hFind);

		/* Fix directory buffer to pre-modified state.*/
		m_szBuffer[nDirectoryLen] = '\0';

		/* bUserCancelled should be VTRUE if we completed all enumerations.*/
		return (bUserCancelled) ? VFALSE : VTRUE;
	}

	/** Internal function called before starting public function calls.*/
	void			Init(VSTRING_CONST pszDirectory)
	{
		VASSERT(VSTRLEN_CHECK(pszDirectory))

		/* Reset recursion count to 0.*/
		m_nRecursionCount = 0;

		/* Copy to local buffer and get length.*/
		VINT nLen = VSTRLEN(VSTRCPY(m_szBuffer, pszDirectory));
		VASSERT(nLen)

		/* If pszDirectory ends with \ or /, remove it.*/
		if (	m_szBuffer[nLen - 1] == VTEXT('\\') ||
				m_szBuffer[nLen - 1] == VTEXT('/') )
			m_szBuffer[nLen - 1] = VTEXT('\0');

		/* Don't show errors to user when accessing file system.*/
		m_nOldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
	}

	/** Determine if a string is a valid file or directory name.*/
	VBOOL			IsValidName(VSTRING_CONST pszName)
	{
		VASSERT(VSTRLEN_CHECK(pszName))

		return (	VSTRCMP(pszName, VTEXT(".")) == 0 ||
					VSTRCMP(pszName, VTEXT("..")) == 0)
					? VFALSE : VTRUE;
	}

	/** Internal recursive function.*/
	void			RecurseInternal(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszMask,
		VVOIDPTR								pVoid1,
		VVOIDPTR								pVoid2)
	{
		/* Save directory length.*/
		VINT nDirectoryLen = VSTRLEN(m_szBuffer);

		#ifdef VWCL_DEBUG
			/* Verify input param is a directory.*/
			VDWORD nAttr = GetFileAttributes(m_szBuffer);
			VASSERT(nAttr != 0xFFFFFFFF)
			VASSERT(nAttr & FILE_ATTRIBUTE_DIRECTORY)
		#endif

		/* Enumerate files and subdirectories in this directory.*/
		if ( EnumFilesInternal(callback, pszMask, pVoid1, pVoid2) )
		{
			/* Get subdirectories in this directory for recursion.*/
			WIN32_FIND_DATA fd;

			/* Append wildcard search pattern.*/
			VSTRCPY(&m_szBuffer[nDirectoryLen], VTEXT("\\*.*"));

			/* Find the first file.*/
			HANDLE hFind = FindFirstFile(m_szBuffer, &fd);

			while ( hFind != INVALID_HANDLE_VALUE )
			{
				/* Is this a directory? If yes, recurse it.*/
				if (	(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
						IsValidName(fd.cFileName) )
				{
					/* Recurse after building full path.*/
					m_nRecursionCount++;
					VSTRCPY(&m_szBuffer[nDirectoryLen + 1], fd.cFileName);
					RecurseInternal(callback, pszMask, pVoid1, pVoid2);
				}

				/* Find next and keep looping.*/
				if ( !FindNextFile(hFind, &fd) )
					break;
			}

			if ( hFind != INVALID_HANDLE_VALUE )
				FindClose(hFind);
		}

		/* Fix directory buffer to pre-modified state.*/
		m_szBuffer[nDirectoryLen] = '\0';
	}

	/** Reset internal members.*/
	void			Reset()
	{
		m_nRecursionCount = m_nOldErrorMode = 0;
		VZEROMEMORY(m_szBuffer, VARRAY_SIZE(m_szBuffer));
	}

	/** Embedded Members.*/
	VUINT			m_nRecursionCount;
	VUINT			m_nOldErrorMode;
	VCHAR			m_szBuffer[1024 + 1];
};

#endif /* VRECURSIVEDIRECTORYENUMERATOR*/
