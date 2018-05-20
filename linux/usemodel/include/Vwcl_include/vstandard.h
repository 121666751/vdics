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

#ifndef VSTANDARD
#define VSTANDARD

/** This is the standard include file for the Virtual Windows Class Library
(VWCL). This header replaces the older VWCL.H and is used only to define basic
functionality, and implies nothing about the application being written. In
fact, the application can be C++ or a mix of C and C++. Previous releases of
VWCL (Prior to December 1997) used the preprocessor extensively to include or
exclude code. VWCL grew to such a size that this method proved extremely hard
to maintain. The new method requires the users of VWCL to manually include the
classes they need. Those classes, should they require others, will include
those other headers as needed. This makes the code more maintainable, and also
has the benefit of better code re-use and builds smaller and tighter
applications by only including required code. There are still some basic
preprocessor command that can, or should, be defined:

VWCL_DONT_REGISTER_CLASSES
Causes VWCL initialization code to not register standard VWCL window class(es).
This is most useful for dialog box based applications, or where VWCL is being
compiled into a shared DLL.

VWCL_EXPORT_DLL
Define this when building a DLL version of class library. A shared VWCL DLL
can only be loaded once per process. VWCL has no state or resource management
functions, so multiple DLL's that use VWCL dynamically linked into one EXE
would fail to locate resources outside of the calling programs resources.
A call to VGetApp().Init() from these DLL's would corrupt the EXE's global
VApplication object.

VWCL_IMPORT_DLL
Define this when using VWCL from a DLL (importing).

VWCL_NO_WIN32_API
Define when the Win32 API is not available. WINE (www.winehq.com), and
MainWin (www.mainsoft.com) are implementations of Win32 on UNIX platforms.

VWCL_NO_VWINDOW
Define when VWindow should not be included by VApplication and other
non-Windowing classes.

VWCL_TARGET_IIS_EXTENSION
Define when building a Internet Information Server extension application.

VWCL_TARGET_LINUX
Defines Linux as the target UNIX platform.

VWCL_TARGET_SOLARIS
Defines Sun Microsystems Solaris as the target UNIX platform.

VWCL_TARGET_SCO
Defines SCO UNIX as the target UNIX platform.

VWCL_TARGET_HPUX
Defines HPUX as the target UNIX platform.

VWCL_TARGET_BSD
Defines BSD as the target UNIX platform.

VWCL_TARGET_DEC
Defines DEC Digital Unix (Tru64) as the target UNIX platform.

VWCL_TARGET_MAC
Defines Apple Macintosh as the target platform.

The following directives will be defined by this header file and should
not be defined elsewhere:

VWCL_CONSOLE_APP
Causes VWCL to eliminate code not needed when a console app is the target.

VWCL_TARGET_UNIX
Instructs VWCL classes to compile for (generic) UNIX platforms.

VWCL_DEBUG
VWCL directive defined when a debug build is the target for all platforms.

VWCL_UNICODE
VWCL directive defined when UNICODE is being used.
*/

/********************************************************************
This section makes sure user did not already specify a directive this
header defines.
********************************************************************/
#ifdef VWCL_CONSOLE_APP
	#error VWCL_CONSOLE_APP should not yet be defined. \
	For Windows console mode, define _CONSOLE
#endif
#ifdef VWCL_TARGET_UNIX
	#error VWCL_TARGET_UNIX should not yet be defined. \
	Define VWCL_TARGET_LINUX, VWCL_TARGET_SOLARIS, VWCL_TARGET_SCO, \
	VWCL_TARGET_HPUX, VWCL_TARGET_BSD, or VWCL_TARGET_DEC for UNIX \
	compatiblity.
#endif
#ifdef VWCL_DEBUG
	#error VWCL_DEBUG should not yet be defined. \
	For Windows, define _DEBUG. For UNIX, do NOT define NDEBUG.
#endif
#ifdef VWCL_UNICODE
	#error VWCL_UNICODE should not yet be defined. \
	For Windows, define _UNICODE.
#endif
#ifdef VODS
	#error VODS should not yet be defined.
#endif

/************************************************************************
This section prepares the compiler by defining and checking pre-processor
definitions.
************************************************************************/

/* Force strict type checking.*/
#ifndef STRICT
	#define STRICT
#endif

/* There are two standards for defining UNICODE. UNICODE and _UNICODE.
Some CRT headers only understand one of them, or break if the one they
expect is not defined. To cover these bases, we make sure that if either
is defined, they both are.*/
#ifdef UNICODE
	#ifndef _UNICODE
		#define _UNICODE
	#endif
#endif

#ifdef _UNICODE
	#ifndef UNICODE
		#define UNICODE
	#endif
#endif

/* Should VWCL_UNICODE be defined?*/
#ifdef _UNICODE
	#define VWCL_UNICODE
	/* MBCS should NOT be defined when UNICODE is in use. Since some
	compilers default to leaving it defined, we should warn the programmer.*/
	#ifdef _MBCS
		#error _MBCS should not be defined for UNICODE projects.
	#endif
	#ifdef MBCS
		#error MBCS should not be defined for UNICODE projects.
	#endif
#endif

/* Determine if a UNIX platform is the target.*/
#if defined VWCL_TARGET_LINUX
	#define VWCL_TARGET_UNIX
#elif defined VWCL_TARGET_SOLARIS
	#define VWCL_TARGET_UNIX
#elif defined VWCL_TARGET_SCO
	#define VWCL_TARGET_UNIX
#elif defined VWCL_TARGET_HPUX
	#define VWCL_TARGET_UNIX
#elif defined VWCL_TARGET_BSD
	#define VWCL_TARGET_UNIX
#elif defined VWCL_TARGET_DEC
	#define VWCL_TARGET_UNIX
#endif

/* Should VWCL_CONSOLE_APP be defined?*/
#ifndef VWCL_TARGET_UNIX
	#ifdef _CONSOLE
		#define VWCL_CONSOLE_APP
	#endif
#endif

/* Define other settings for console applications.*/
#ifdef VWCL_CONSOLE_APP
	#ifndef VWCL_DONT_REGISTER_CLASSES
		#define VWCL_DONT_REGISTER_CLASSES
	#endif
	#ifndef VWCL_NO_VWINDOW
		#define VWCL_NO_VWINDOW
	#endif
#endif

/* Should VWCL_DEBUG be defined?*/
#ifdef VWCL_TARGET_UNIX
	#ifndef NDEBUG
		#define VWCL_DEBUG
	#endif
#elif defined _DEBUG
	#define VWCL_DEBUG
#endif

/***********************************************************
This section includes standard and OS specific header files.
***********************************************************/

/* Include CRT standard headers.*/
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <time.h>
#include <wchar.h>

#ifdef VWCL_DEBUG
	#include <assert.h>
#endif

/* Include platform specific headers.*/
#ifdef VWCL_TARGET_UNIX
	/* UNIX specific headers.*/
	#include <unistd.h>
#else
	/* Windows specific headers.*/
	#include <windows.h>
	#include <windowsx.h>
	#include <commctrl.h>
	#include <commdlg.h>
	#include <direct.h>
	#include <io.h>
	#include <ole2.h>
	#include <olectl.h>
	#include <oledlg.h>
	#include <ocidl.h>
	#include <olectl.h>
	#include <process.h>
	/* Include Internet Information Server?*/
	#ifdef VWCL_TARGET_IIS_EXTENSION
		#include <httpext.h>
		#include <httpfilt.h>
	#endif
#endif

/* Include UNICODE and CRT mappings.*/
#include "vunicode_mapping.h"

/*****************************************************************
This section defines standard C/C++ data types as VWCL data types.
*****************************************************************/

/** Target Dependant Signed Integer.*/
typedef	int				VINT;
/** Target Dependant Unsigned Integer.*/
typedef	unsigned int	VUINT;
/** Always char, never UNICODE.*/
typedef char			VCHARA;
/** Always wchar_t, only UNICODE.*/
typedef wchar_t			VCHARW;
/** Same as VCHARW when UNICODE is used, VCHARA otherwise.*/
#ifdef VWCL_UNICODE
	typedef VCHARW		VCHAR;
#else
	typedef VCHARA		VCHAR;
#endif
/** Always unsigned char, never UNICODE.*/
typedef unsigned char	VUCHAR;
/** Always the same as unsigned char.*/
typedef unsigned char	VBYTE;
/** Target Dependant Signed Short Integer.*/
typedef short			VSHORT;
/** Target Dependant Unsigned Short Integer.*/
typedef unsigned short	VUSHORT;
/** Target Dependant Signed Long Integer.*/
typedef long			VLONG;
/** Target Dependant Unsigned Long Integer.*/
typedef unsigned long	VULONG;
/** Target Dependant Float.*/
typedef float			VFLOAT;
/** Target Dependant Double.*/
typedef double			VDOUBLE;
/** Target Dependant Long Double.*/
typedef long double		VLONGDOUBLE;
/** Always a void*.*/
typedef void*			VVOIDPTR;
/** Always a void const*.*/
typedef const void*		VVOIDPTR_CONST;
/** Same as unsigned char, always a 1 byte (8 bit), unsigned.*/
typedef unsigned char	VBOOL;

/** Map Windows defined data types as new types, or uses existing
types when available.*/
#ifdef VWCL_NO_WIN32_API
	/** Same as unsigned char.*/
	typedef unsigned char	VWINBOOL;
	/** Same as unsigned short.*/
	typedef unsigned short	VWORD;
	/** Same as unsigned long.*/
	typedef unsigned long	VDWORD;
	/** Same as long.*/
	typedef long			VWPARAM;
	/** Same as long.*/
	typedef long			VLPARAM;
#else
	/** Same as Windows defined BOOL.*/
	typedef BOOL		VWINBOOL;
	/** Same as Windows defined WORD.*/
	typedef WORD		VWORD;
	/** Same as Windows defined DWORD.*/
	typedef DWORD		VDWORD;
	/** Same as Windows defined WPARAM.*/
	typedef WPARAM		VWPARAM;
	/** Same as Windows defined LPARAM.*/
	typedef LPARAM		VLPARAM;
#endif

/**********************************************************************
This section defines string types.
**********************************************************************/

#ifdef VWCL_UNICODE
	/** Same as wchar_t* (UNICODE supported if defined).*/
	typedef wchar_t*		VSTRING;
	/** Same as const wchar_t* (UNICODE supported if defined, const).*/
	typedef const wchar_t*	VSTRING_CONST;
#else
	/** Same as char* (UNICODE not supported).*/
	typedef char*			VSTRING;
	/** Same as const char* (UNICODE not supported, const).*/
	typedef const char*		VSTRING_CONST;
#endif

/** Same as char* (Always an ASCII string).*/
typedef char*			VSTRINGA;
/** Same as const char* (Always an ASCII string, const).*/
typedef const char*		VSTRING_CONSTA;

/** Same as wchar_t* (Always a UNICODE string).*/
typedef wchar_t*		VSTRINGW;
/** Same as const wchar_t* (Always a UNICODE string, const).*/
typedef const wchar_t*	VSTRING_CONSTW;

/*****************************************************************************
This section creates new VWCL macro's for working with strings and characters.
******************************************************************************/

/** This macro will always create a UNICODE string from a string literal,
even when UNICODE is not explicitly being used.*/
#define VTEXTW(str)			L ## str

/** This macro will always create an ASCII string from a string literal,
even when UNICODE is being used.*/
#define VTEXTA(str)			str

/** This macro will be either VTEXTW or VTEXTA. When VWCL_UNICODE
is defined, it will make a UNICODE character or string from a string
or character literal. When UNICODE is not being used, the macro becomes
VTEXTA, which does nothing.*/
#ifdef VWCL_UNICODE
	#define	VTEXT			VTEXTW
#else
	#define VTEXT			VTEXTA
#endif

/** Macro to return string length, or 0 if string is NULL.
Non-UNICODE version.*/
#define	VSTRLEN_CHECKA(str)	((str) ? VSTRLENA(str) : 0)

/** Macro to return string length, or 0 if string is NULL.
UNICODE version.*/
#define	VSTRLEN_CHECKW(str)	((str) ? VSTRLENW(str) : 0)

/** Macro is VSTRLEN_CHECKW when UNICODE is being used, VSTRLEN_CHECKA
otherwise.*/
#ifdef VWCL_UNICODE
	#define	VSTRLEN_CHECK	VSTRLEN_CHECKW
#else
	#define	VSTRLEN_CHECK	VSTRLEN_CHECKA
#endif

/***************************************************************************
This section creates new VWCL macro's, functions, and strings for debugging.
***************************************************************************/

/** Define VASSERT macro, which really does compile away to nothing for
release builds. This macro already defines the trailing ; so users should
NOT append a semicolon to the end of this macro. The reason for this is that
if the semicolon is there, it results in double semicolons during debug
builds and a single semicolon in release builds. This normally does not do
any harm, but some compilers have been reported to have problems with this.*/
#ifdef VWCL_DEBUG
	#define VASSERT(expr)	assert(expr);
#else
	#define VASSERT(expr)
#endif

/** Define VVERIFY which compiles to VASSERT(expression) in debug build, and
just expression in release build. This is useful when the expression should
be evaluated in the debug and release builds, but when the expression is still
required to be computed in release builds. Like VASSERT, you should not
include the trailing semicolon. It will be inserted as needed in release
builds.*/
#ifdef VWCL_DEBUG
	#define	VVERIFY(expr) VASSERT(expr)
#else
	#define VVERIFY(expr) expr;
#endif

/** OutputDebugString() wrapper. Sends output to OutputDebugString() on Windows,
stderr otherwise. If stderr is wanted, even on Windows platforms, use
VODS_CONSOLE, which always sends output to stderr. Both macros compile
to nothing in a non-debug build. Under a UNIX target, VODS is the same as
VODS_CONSOLE and always sends output to stderr. In all cases, you should not
include a trailing semicolon. This macro will insert one as needed.*/
#ifdef VWCL_DEBUG
	#define VODS_CONSOLE(string)	VFPRINTF(stderr, VTEXT("\n%s%s\n"), VTEXT("DEBUG: "), string);
	#ifdef VWCL_NO_WIN32_API
		#define VODS(string)		VODS_CONSOLE(string)
	#else
		#define VODS(string)		OutputDebugString(string);
	#endif
#else
	#define VODS_CONSOLE(string)
	#define VODS(string)
#endif

/** Macro compiles to VShowLastErrorMessage() global function for Win32 debug
builds, nothing otherwise. expr is the expression to evaluate. If expr
evaluates to a 0 value, VShowLastErrorMessage() will be called, otherwise it
will not. In essence, this works exactly like VASSERT, where you expect expr
to be non-0, but shows a messagebox or sends output to VODS. Like VASSERT, do
not end this macro with a semicolon, since that would leave a bogus extra one
that some compilers may have problems with.*/
#ifndef VWCL_NO_WIN32_API
	#ifdef VWCL_DEBUG
		/** Show the result from GetLastError() as a message box, or VODS if a
		console application. This function is implemented in
		utility/vdebugglobal.c. This function is not available in release builds,
		so it should rarely be called directly. Rather, the VSHOWLASTERRORMESSAGE
		macro should be used.*/
		#ifdef __cplusplus
			extern "C" {
		#endif
			VINT	VShowLastErrorMessage(HWND hWndParent);
		#ifdef __cplusplus
			}
		#endif
		#define VSHOWLASTERRORMESSAGE(expr, hwnd)	if ( !(expr) ) VShowLastErrorMessage(hwnd);
	#else
		#define VSHOWLASTERRORMESSAGE(expr, hwnd)
	#endif
#else
	#define VSHOWLASTERRORMESSAGE(expr, hwnd)
#endif

/** This string is defined, mostly for use in messagebox titles
during debug builds.*/
#define VWCL_DEBUG_MESSAGE_STRING	VTEXT("VWCL DEBUG MESSAGE")

/********************************************************************
This section defines general purpose macro's, strings, and functions.
********************************************************************/

/** Macro's return the low or high byte when given a 16 bit value.*/
#define VLOBYTE16(nValue)			((VBYTE)(nValue))
#define VHIBYTE16(nValue)			((VBYTE)(((VWORD)(nValue) >> 8) & 0xff))

/** Macro to create a 16 bit value from 2 bytes.*/
#define VMAKE16BIT(low, high)		((VWORD)(((VBYTE)(low))|((VWORD)((VBYTE)(high))) << 8))

/** Macro's for min() and max().*/
#define VMIN(a, b)					((a) <= (b) ? (a) : (b))
#define VMAX(a, b)					((a) >= (b) ? (a) : (b))

/** VREF_PTR macro derefences any pointer to a reference.*/
#define	VREF_PTR(p)					*p

/** Use VREF_THIS when you have a "this" pointer and need to convert it to
a reference.*/
#define VREF_THIS					*this

/** Macro to determine static array size.*/
#define VARRAY_SIZE(the_array)		sizeof(the_array) / sizeof(the_array[0])

/** Macro to zero fill a section of memory.*/
#define VZEROMEMORY(address, bytes)	memset((VVOIDPTR)address, (VINT)0, (size_t)bytes)

/** Macro to zero fill all members of a structure.*/
#define VZEROSTRUCT(zero_struct)	VZEROMEMORY(&zero_struct, sizeof(zero_struct))

/** Macro to define true and false values.*/
#define	VTRUE						1
#define	VFALSE						0

/** Macro to force operator new to return NULL and not throw an exception.
TODO: This is not yet implemented until the compiler vendors make ANSI
compliant C++ compilers.*/
#define	VNEW						new

/** For consistency with VNEW, we define VDELETE.*/
#define VDELETE						delete

/** VDELETE_NULL deletes an object and sets the pointer to NULL. Do NOT end
this macro with a ;*/
#define VDELETE_NULL(ptr)			{ delete ptr; ptr = NULL; }

/** When deleting array, use this version.*/
#define VDELETE_ARRAY				delete []

/** VDELETE_ARRAY_NULL deletes an array of objects and sets the pointer to
NULL. Do NOT end this macro with a ;*/
#define VDELETE_ARRAY_NULL(ptr)		{ delete [] ptr; ptr = NULL; }

/** Define the default file path separator character.*/
#ifdef VWCL_TARGET_UNIX
	#define	VFILE_PATH_SEP		VTEXT("/")
	#define	VFILE_PATH_SEP_CHAR	VTEXT('/')
#else
	#define	VFILE_PATH_SEP		VTEXT("\\")
	#define	VFILE_PATH_SEP_CHAR	VTEXT('\\')
#endif

#ifdef __cplusplus
	/* Forward Declaration(s).*/
	class						VApplication;

	/* Returns a reference to the VApplication application object.*/
	VApplication&				VGetApp();

	/* The following are only valid on Win32 platforms.*/
	#ifndef VWCL_NO_WIN32_API
		/** Handle dailog box message routing.*/
		#ifndef VWCL_CONSOLE_APP
			VBOOL				VTranslateDialogMessage(MSG const& msg);
		#endif

		/* The standard window class name that VWCL registers unless
		VWCL_DONT_REGISTER_CLASSES is defined.*/
		#define VWINDOWCLASS			VTEXT("VWindow")

		/* The standard window class name for MDI child windows that
		VWCL registers when creating MDI child windows.*/
		#define	VWINDOWCLASS_MDI_CHILD	VTEXT("VMDIChildWindow")

		/***************************************************
		This section defines global ActiveX helper functions
		implemented in vactivexglobal.cpp.
		****************************************************/
		/** Given a C string, allocate a new string with CoTaskMemAlloc()
		and copy it, putting the results in ppszCoString.*/
		VBOOL		VCoStringFromString(VSTRING*		ppszCoString,
										VSTRING_CONST	pszString);

		/** Determine if a string is a valid ActiveX structured storage name.
		This includes length and valid characters.*/
		VBOOL		VIsValidStructuredStorageName(VSTRING_CONST pszName);

		/** Given a file name to a picture format supported by the IPicture
		interface, load the picture and return the IPicture object interface
		pointer. Applications that use this must link with OLEPRO32.LIB. IPicture
		supports many popular graphic file formats, including GIF, JPG, ICO, WMP,
		EMF, BMP, and others. Returns IPicture object pointer on success, NULL
		on failure. The returned IPicture pointer must be released when done,
		using IPicture::Release().*/
		IPicture*	VLoadIPictureFromFile(VSTRING_CONST pszFileName);

		/** Given a resource type and name name of a picture format supported
		by the IPicture interface, load the picture and return the IPicture
		object interface pointer. Applications that use this must link with
		OLEPRO32.LIB. IPicture supports many popular graphic file formats,
		including GIF, JPG, ICO, WMP, EMF, BMP, and others. Returns IPicture
		object pointer on success, NULL on failure. The returned IPicture
		pointer must be released when done, using IPicture::Release().*/
		IPicture*	VLoadIPictureFromResources(	VSTRING_CONST	pszType,
												VSTRING_CONST	pszName,
												HINSTANCE		hResource =
																	NULL);
	#endif
#endif

/* The following sections are for C and C++ programming.*/
#ifdef __cplusplus
	extern "C" {
#endif

/****************************************************************************
This section defines global number to string conversion functions. The
functions are implemented in vwclport.c. In all cases, the parameters are as
follows:

nValue - The number to convert.
pszBuffer - Where to place the output string. It must be large enough to hold
the string representation of the number.

nRadix - Base of value, must be in the range 2 - 36. Most of the time this
is base 10.

All functions return a pointer to pszBuffer on success, NULL on failure.
****************************************************************************/

/** Convert a VINT to string.*/
VSTRING				VITOA(VINT nValue, VSTRING pszBuffer, VUINT nRadix);
/** Convert a VUINT to string.*/
VSTRING				VUITOA(VUINT nValue, VSTRING pszBuffer, VUINT nRadix);
/** Convert a VLONG to string.*/
VSTRING				VLTOA(VLONG nValue, VSTRING pszBuffer, VUINT nRadix);
/** Convert a VULONG to string.*/
VSTRING				VULTOA(VULONG nValue, VSTRING pszBuffer, VUINT nRadix);

/***************************************************************************
This section defines string processing functions that should be a part of
the ANSI C runtime libraries, but aren't. These functions are implemented
in vwclport.c.
***************************************************************************/

/** Convert pszConvert to lowercase, returning a pointer to it.*/
VSTRING				VSTRLWR(VSTRING pszConvert);

/** Convert pszConvert to uppercase, returning a pointer to it.*/
VSTRING				VSTRUPR(VSTRING pszConvert);

/************************************************************************
This section defines global file and directory functions. The methods are
implemented in vwclport.c.
************************************************************************/

/** Determine if a file, not a directory, exists.*/
VBOOL				VDoesFileExist(VSTRING_CONST pszFileName);

/** Determine if a directory, not a file, exists.*/
VBOOL				VDoesDirectoryExist(VSTRING_CONST pszDirectoryName);

/***********************************************************************
This section defines global portability functions. The methods are
implemented in vwclport.c.
***********************************************************************/

/** Returns VTRUE if the machine is little endian architecture, VFALSE if
the machine is big endian.*/
VBOOL				VIsLittleEndian();

/** Returns VTRUE if the machine is big endian architecture, VFALSE if
the machine is little endian.*/
VBOOL				VIsBigEndian();

/************************************************************************
This section defines global functions for getting and setting the program
state, options, etc.
************************************************************************/

/** Return a static string buffer to the applications title, or name. If
building a standard Windows GUI VWCL application, this functions will
already be implemented for you in VApplication. However, if you are picking
and choosing classes from VWCL to use in your application, you may have to
implement this C function because classes that require this will have to
have them to compile and link properly.*/
VSTRING_CONST		VGetAppTitle();

/** Return the argc values passed to main() or WinMain(). For UNIX targets,
this must be set with VSetARGC_ARGV().*/
VUINT				VGetARGC();

/** Return one string from the argv array passed to main() or WinMain(). For
UNIX targets, this must be set with VSetARGC_ARGV(). nIndex specifies which
string from the array to return. It is valid to pass an invalid index for this
parameter since the function will return NULL if nIndex exceeds the number of
command line arguments. Because command line arguments are normally passed only
in ASCII, not UNICODE, this method will always return ASCII strings.*/
VSTRINGA			VGetARGV(VUINT nIndex);

/** This function must be used on UNIX targets to save the argc and argv passed
to main(). For Windows targets it is optional, and if fact does nothing.
VGetARGC() and VGetARGV() are still valid on Windows, they just don't have to
be set. On these platforms, the compiler stores these in global variables
which VWCL uses rather than the user specified ones. Because command line
arguments are normally passed only in ASCII, not UNICODE, this method only
accepts ASCII strings.*/
void				VSetARGC_ARGV(VUINT nARGC, VSTRINGA* ppszARGV);

#ifndef VWCL_NO_WIN32_API
	/** If building a standard Windows GUI VWCL application, these functions
	will already be implemented for you in vapplication.cpp and other global
	modules. However, if you are picking and choosing classes from VWCL to
	use in your application, you may have to implement these C style functions
	because classes that require these will have to have them to compile and
	link properly.*/

	/** Return the show command (ShowWindow() SW_xxx constant passed on the
	command line).*/
	VINT		VGetCommandShow();

	/** Return the global instance handle of the application or DLL.*/
	HINSTANCE	VGetInstanceHandle();

	/** Return the instance handle where resources are held.*/
	HINSTANCE	VGetResourceHandle();
#endif

/*******************************************************
This section defines global Windows GDI helper functions
implemented in vgdiglobal.c, and macro's for GDI.
*******************************************************/
#ifndef VWCL_NO_WIN32_API
	/** Map coordinates to or from pixels to or from himetric. The
	macro's VPixelsToHIMETRIC and VPixelsFromHIMETRIC help with
	calling this function.*/
	void	VMapCoords(HDC hDC, SIZEL* psizeL, VBOOL bToPixels);

	/** Load a custom resource. Returns a pointer to the global memory
	on success, NULL on failure. If the size of the resource is required,
	pass a valid VUINT pointer for the pnSize parameter.*/
	HGLOBAL	VLoadResource(	VSTRING_CONST	pszType,
							VSTRING_CONST	pszName,
							VUINT*			pnSize,
							HINSTANCE		hResource);

	/** Load a custom resource and lock it into memory. Returns a pointer
	to the memory on success, NULL on failure. If the size of the resource
	is required, pass a valid VUINT pointer for the pnSize parameter.*/
	VVOIDPTR VLoadAndLockResource(	VSTRING_CONST	pszType,
									VSTRING_CONST	pszName,
									VUINT*			pnSize,
									HINSTANCE		hResource);

	#define	VPixelsToHIMETRIC(hDC, psizeL)		VMapCoords(hDC, psizeL, VFALSE)
	#define	VPixelsFromHIMETRIC(hDC, psizeL)	VMapCoords(hDC, psizeL, VTRUE)
#endif
#ifdef __cplusplus
	}
#endif

#endif /* VSTANDARD*/
