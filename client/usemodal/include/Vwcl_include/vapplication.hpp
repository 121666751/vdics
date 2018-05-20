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

#ifndef VAPPLICATION
#define VAPPLICATION

#include "vstandard.h"
#include "strings/vstring.hpp"

/* Forward declaration(s).*/
class VApplication;

/* Set the VApplication application object. This function is implemented in
vapplication.cpp and is set when a VApplication object is created. If
VGetApp() is called before an application object is created, a default
application object will be created by VWCL, so if an application wants to
not use the default application object, it must instantiate its own
VApplication derived object before calling other VWCL classes or functions,
or must replace the VWCL global object with a call to VSetApp().*/
void VSetApp(VApplication const& appObject);

/* VApplication encapsulates startup code and support for
internationalization. VWCL maintains a global application object of this
class, and a reference can be returned to it with a call to the global
function VGetApp(). This object maintains the application name (title)
and if the application has the concept of a "current file" this class also
maintains the file name string. See the above VSetApp() global function for
more information on replacing the default VWCL application object.*/
class VApplication
{
public:
	/* Standard constructor initializes class variables as needed. The Init()
	function must always be called for all VWCL applications that use the
	VApplication global object, or for applications that implement their own
	VApplication class. See Init() for more information.*/
	VApplication();

	/* Virtual destructor unregisters VWCL window clases and perform other
	as-needed application cleanup.*/
	virtual ~VApplication();

	/* Return the application title string object.*/
	VString&				GetAppTitle() const
		{ return (VString&)m_strAppTitle; }

	/* Return a reference to the current file name string object.*/
	VString&				GetCurrentFile() const
		{ return (VString&)m_strCurrentFile; }

	/* Determine if the command specified by pszOption is a command line
	option to the program. This function will look at each command
	line parameter until it finds pszOption. If it finds it, a pointer to
	it will be returned. Since command line parameters are not normally
	UNICODE, this function only allows ASCII strings. The method will look
	past the first character of the option to determine if it qualifies. Since
	many command line parameters start with -, /, or \, looking past this
	first character allows this type of command line, as well as those without
	prefaces. The comparisons are not case sensitive by default, unless
	bCaseSensitive is set to VTRUE.*/
	VSTRINGA				IsCommandLineOption(
		VSTRINGA	pszOption,
		VBOOL		bCaseSensitive = VFALSE) const
	{
		for ( VUINT i = 1; i < VGetARGC(); i++ )
		{
			VSTRINGA pszThisCommand = VGetARGV(1);

			if ( pszThisCommand )
			{
				if ( bCaseSensitive )
				{
					if (	VSTRCMP_NOCASEA(pszThisCommand,
											pszOption) == 0 ||
							VSTRCMP_NOCASEA(pszThisCommand + 1,
											pszOption) == 0 )
					{
						return pszThisCommand;
					}
				}
				else if (	VSTRCMP_NOCASEA(pszThisCommand,
											pszOption) == 0 ||
							VSTRCMP_NOCASEA(pszThisCommand + 1,
											pszOption) == 0 )
				{
					return pszThisCommand;
				}
			}
		}

		return NULL;
	}

	/* Set the current file name and update the main window, or console
	window, caption. If pszFileName is NULL, "Untitled" will be used instead.
	Returns VTRUE if title was set, VFALSE otherwise. The application name is
	included in the caption, as well as pszFileName. If bDefault is VFALSE,
	the function will not use the word "Default" if pszFileName is NULL.
	bDefault is not used anytime pszFileName is not NULL.*/
	virtual VBOOL			SetCurrentFile(	VSTRING_CONST	pszFileName,
											VBOOL			bDefault = VTRUE);

	#ifndef VWCL_NO_WIN32_API
		#ifndef VWCL_CONSOLE_APP
			/* Get command show param (the ShowWindow() API SW_xxx Constant).*/
			VINT			GetCommandShow() const
				{ return m_nCommandShow; }
		#endif

		/* Get the application instance handle. This is normally only valid
		after calling Init(), but applications can call this method before
		if the instance handle of the executable is desired.*/
		virtual HINSTANCE	GetInstanceHandle()
		{
			if ( !m_hInstance )
				m_hInstance = GetModuleHandle(NULL);

			return m_hInstance;
		}

		/* Return a reference to the resource handle. By default this is the
		same as the instance handle, but can be set by modifying the handle
		returned from this function. This is normally only valid after calling
		Init(), but applications can call this method before if the instance
		handle of the executable is desired.*/
		virtual HINSTANCE&	GetResourceHandle()
		{
			if ( !m_hResource )
				m_hResource = GetInstanceHandle();

			return (HINSTANCE&)m_hResource;
		}
	#endif

	/* Initialize class library, application startup, and register window
	classes as needed. You can optionally provide a WNDCLASS structure that
	will be used to initialize the VWindow window class. pWindowClass can be
	NULL to use defaults. If NULL, you can still specify the background brush
	and icon if desired. However, if pWindowClass is not NULL, it is assumed
	to be the gospel, and the structure must be completely initialized. There
	are some modifications to this structure which Init() will make in the
	pWindowClass if they are 0 or NULL. These are as follows:
	pWindowClass->hIcon will be set to the icon loaded from nIconID only if
	nIconID is not 0.
	pWindowClass->hInstance will use the application instance handle.
	pWindowClass->lpfnWndProc will use the default VWindow shared window
	procedure.
	pWindowClass->hCursor will use the global arrow cursor.
	pWindowClass->lpszClassName will use the default class name retreived
	from VWindow::DefaultClassName().
	Functions are provided for when windowing applications are being built,
	and others for when a console app is the target.*/
	#ifndef VWCL_NO_WIN32_API
		#ifndef VWCL_CONSOLE_APP
			#ifndef VWCL_DONT_REGISTER_CLASSES
				void		Init(	HINSTANCE	hInstance,
									VINT		nCommandShow = SW_SHOWNORMAL,
									VUINT		nIconID = 0,
									HBRUSH		hBackgroundBrush =
										(HBRUSH)(COLOR_WINDOW + 1),
									WNDCLASS*	pWindowClass = NULL);
			#else
				void		Init(	HINSTANCE	hInstance,
									VINT		nCommandShow = SW_SHOWNORMAL);
			#endif
		#else
			void			Init(HINSTANCE hInstance = NULL)
			{
				/* Get this module's handle if not passed as parameter.*/
				m_hInstance = m_hResource =
					(hInstance) ? hInstance : GetInstanceHandle();
			}
		#endif
	#endif

protected:
	/* Embedded Members.*/
	#ifndef VWCL_NO_WIN32_API
		HINSTANCE			m_hInstance;
		HINSTANCE			m_hResource;
		#ifndef VWCL_CONSOLE_APP
			VINT			m_nCommandShow;
		#endif
	#endif

	/* Application title and currently opened file.*/
	VString					m_strAppTitle;
	VString					m_strCurrentFile;
};

#endif	/* VAPPLICATION*/
