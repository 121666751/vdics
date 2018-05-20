#ifndef __ZSTANDARD_H__
#define __ZSTANDARD_H__

// ----------------------------------------------------------------------------------------------------------
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
#include <string>
using namespace std;*/

#ifdef _WIN32
	#define bzero(ptr,n) memset(ptr,0,n)
	#define vsnprintf _vsnprintf
	#define snprintf  _snprintf
	#define stat    _stat

    #include <windows.h>

#else
	#define VWCL_TARGET_LINUX
	#define VWCL_NO_WIN32_API

	#include <unistd.h>
	#include <stdarg.h>
	#include <strings.h>

#endif

/* define sleep() function  */
#ifdef _WIN32
	#define z_sleep(a) _sleep(a*1000)
	#define msleep(a) _sleep(a)
#else
	#define z_sleep(a) sleep(a)
	#define msleep(a) usleep(a*1000)
#endif




#ifndef _SYS_TYPES_H
	typedef unsigned char	u_char;
	typedef unsigned short	u_short;
	typedef unsigned int	u_int;
	typedef unsigned long	u_long;
#endif /* _SYS_TYPES_H */

#ifndef NULL
  #ifdef __cplusplus
    #define NULL    0
  #else
    define NULL    ((void *)0)
  #endif
#endif

// ----------------------------------------------------------------------------------------------------------
#define ZPCTSTR	const char*
#define RETURN_IF(a) {int b=a; if(b) return b;}

////////////////////////////////////////////////////////////////////////////////
// Common useful functions
/** Internal function to get the mode (type) of a file.*/
inline int ZGetFileMode(const char* pszName)
{
	if ( pszName )
	{
		struct stat fileSTAT;
		bzero(&fileSTAT,sizeof(fileSTAT));

		if ( stat(pszName, &fileSTAT) == 0 )
			return fileSTAT.st_mode;
	}

	return 0;
}

/** Determine if a file exists.*/
inline bool ZDoesFileExist(const char* pszFileName)
{
	int nMode = ZGetFileMode(pszFileName);

	if ( nMode )
		return (!(nMode & S_IFDIR) && (nMode & S_IFREG)) ? true : false;

	return false;
}

inline int ZGetFileSize( const char* sFileName )
{
	struct stat buf;
	if( stat(sFileName, &buf) ) return 0;
	return buf.st_size;
}


/** Determine if a directory exists.*/
inline bool ZDoesDirectoryExist(const char* pszDirectoryName)
{
	int nMode = ZGetFileMode(pszDirectoryName);

	if ( nMode )
		return (nMode & S_IFDIR) ? true : false;

	return false;
}

#ifdef _WIN32

#ifndef __BORLANDC__
#include <shlobj.h>

inline BOOL ZGetFolder(const HWND hwndOwner, const char* lpszTitle, char* retSelectedFolder)
{
	char pszDisplayName[MAX_PATH];
	LPITEMIDLIST lpID;
	BROWSEINFOA bi;

	bi.hwndOwner = hwndOwner;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = pszDisplayName;
	bi.lpszTitle = lpszTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = NULL;
	bi.iImage = NULL;
	lpID = SHBrowseForFolderA(&bi);

	if (lpID != NULL){
		BOOL b = SHGetPathFromIDList(lpID, retSelectedFolder);

		LPMALLOC pMalloc;

		if ( SUCCEEDED(SHGetMalloc(&pMalloc)) )
			pMalloc->Free(lpID);

		return b;
	}

	return FALSE;
}
#endif //__BORLANDC__

inline int ZDosCmd(const char* appName, const char* cmdLine)
{
    PROCESS_INFORMATION info;
    STARTUPINFO startinfo;
    memset( &startinfo, 0, sizeof( STARTUPINFO));
    startinfo.cb = sizeof( STARTUPINFO);
    startinfo.dwFlags = STARTF_USESHOWWINDOW;
    startinfo.wShowWindow = SW_SHOW;
    BOOL ret = CreateProcess(appName,
                (char*)cmdLine,
                0,
                0,
                0,
                CREATE_DEFAULT_ERROR_MODE,
                0,
                0,
                &startinfo,
                &info);
    if( !ret )
    {
		int i = GetLastError();
        return -1;
    }
    WaitForSingleObject(info.hProcess, INFINITE);
    return 0;
}

#endif //_WIN32

// ----------------------------------------------------------------------------------------------------------
#endif //__ZSTANDARD_H__
