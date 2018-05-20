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

#ifndef VNTEVENTLOG
#define VNTEVENTLOG

#include "../vstandard.h"
#include "../strings/vpathstring.hpp"
#include "../strings/vstring.hpp"
#include "../utility/vregistry.hpp"

/** VNTEventLog supports writing to the Windows NT Event Log.*/
class VNTEventLog
{
public:
	/** The maximum number of strings that can be passed to LogEvent().*/
	enum		{	LOG_MAX_INSERT_STRINGS = 16,
				};

	/** Default constructor initializes internal members.*/
	VNTEventLog()
		{ m_hEventSource = NULL; }

	/** Virtual destructor verifies internal handles are released.*/
	virtual ~VNTEventLog()
		{ Close(); }

	/** Close, and un-register, the internal event source handle, as needed.*/
	virtual void			Close()
	{
		if ( m_hEventSource )
		{
			DeregisterEventSource(m_hEventSource);
			m_hEventSource = NULL;
		}
	}

	/** Return the event source handle used for writing to the event log.*/
	HANDLE					GetHandle() const
		{ return m_hEventSource; }

	/** Return the top level registry key for services.*/
	static VSTRING_CONST	GetRegistryKey()
	{
		return
		VTEXT("System\\CurrentControlSet\\Services\\EventLog\\Application\\");
	}

	/** Log an event. nEventID is the message table string ID for the
	message to log. That message can contain embedded string insertion
	points using markers such as %1 %2 etc. For each of these insertion
	marks, this function can insert a string from the variable argument
	list. All parameters must be strings! See the documentation on the
	message compiler for more information.*/
	virtual void			LogEvent(	VDWORD	nEventID,
										VBOOL	bError =
													VTRUE,
										VUINT	nInsertStringCount =
													0,
													...)
	{
		if ( m_hEventSource )
		{
			/* Initialize variable arguments list.*/
			va_list vl;
			va_start(vl, nInsertStringCount);

			/* Build array of strings from argument list.*/
			VSTRING_CONST	pszStrings[LOG_MAX_INSERT_STRINGS];
			VUINT			nStringCount = 0;

			if ( nInsertStringCount > LOG_MAX_INSERT_STRINGS )
				nInsertStringCount = LOG_MAX_INSERT_STRINGS;

			while ( nStringCount < nInsertStringCount )
				pszStrings[nStringCount++] = va_arg(vl, VSTRING_CONST);

			ReportEvent(	m_hEventSource,
							(bError)
							? EVENTLOG_ERROR_TYPE
							: EVENTLOG_INFORMATION_TYPE,
							0,
							nEventID,
							NULL,
							nStringCount,
							0,
							(nInsertStringCount) ? pszStrings : NULL,
							NULL);

			/* Clean up variable argument list.*/
			va_end(vl);
		}
	}

	/** Register the event source. If pszApplicationName is NULL,
	VGetAppTitle() will be used instead. hMessageResources is the handle to
	the DLL or EXE where the message table resources are. These are the
	strings that appear in the NT Event Log and are normally stored in a
	DLL or executable file. The strings are built by the message compiler,
	and the NT Event Log stores string indexes, not actual strings.
	hMessageResources should NEVER be NULL for a DLL, as this would cause
	the parent EXE's path to be written as the message table source! The
	return value is VTRUE if an event source is registered, VFALSE
	otherwise. If a source was already registered by a previous call to
	this function, and this function has an error, the previous source will
	remain valid. The previous source will not be released (if open) by
	this function until there is a good chance the function will success.
	If hMessageResources is NULL, VGetResourceHandle() will be used to
	extract the resource libraries file name. If pszApplicationName is
	NULL, VGetAppTitle() will be used.*/
	VBOOL					Register(	VSTRING_CONST	pszApplicationName =
															NULL,
										HINSTANCE		hMessageResources =
															NULL)
	{
		VSTRING_CONST pszName =
			(pszApplicationName) ? pszApplicationName : VGetAppTitle();
		VASSERT(VSTRLEN_CHECK(pszName))

		VString sPath;
		VPathString ps(sPath);

		if ( ps.LoadModulePath(VFALSE, VFALSE, hMessageResources) )
			return Register(pszName, sPath);

		return VFALSE;
	}

	/** Same as above, but allows you to specify the applicaton name and
	executable file name where the message compiler strings are.*/
	VBOOL					Register(	VSTRING_CONST pszApplicationName,
										VSTRING_CONST pszMessageFile)
	{
		/* These must both be known!*/
		VASSERT(VSTRLEN_CHECK(pszApplicationName))
		VASSERT(VSTRLEN_CHECK(pszMessageFile))

		/* Build registry key string for source.*/
		VString sKey(GetRegistryKey());

		if ( sKey.IsNotEmpty() && sKey.Append(pszApplicationName) )
		{
			/* Set required registry key.*/
			VRegistry reg;

			if ( reg.CreateKey(sKey, HKEY_LOCAL_MACHINE, KEY_WRITE) )
			{
				/* Write event message file location.*/
				if ( reg.WriteString(	VTEXT("EventMessageFile"),
										pszMessageFile) )
				{
					/* Set the supported types flags and register
					event source.*/
					if ( reg.WriteDWORD(	VTEXT("TypesSupported"),
											EVENTLOG_ERROR_TYPE |
											EVENTLOG_WARNING_TYPE |
											EVENTLOG_INFORMATION_TYPE) )
					{
						/* De-Register any old event source.*/
						Close();

						m_hEventSource =
							RegisterEventSource(NULL, pszApplicationName);
					}
				}
			}
		}

		return (m_hEventSource) ? VTRUE : VFALSE;
	}

protected:
	/** Embedded Members.*/
	HANDLE					m_hEventSource;
};

#endif /* VNTEVENTLOG*/
