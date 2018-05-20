#ifndef __zdev_win32_token_hpp__
#define __zdev_win32_token_hpp__

#include "../sys/zbase.hpp"
#include <windows.h>

ZDEV_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
BOOL win32_EnablePrivilege(PTSTR szPriv);
BOOL win32_EnableDebugPrivilege();
//////////////////////////////////////////////////////////////////////////
// 提升当前进程的权限
inline BOOL win32_EnablePrivilege(PTSTR szPriv) {
	HANDLE hToken   = NULL;
    BOOL   fSuccess = FALSE;
    __try {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount             = 1;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        if (!::LookupPrivilegeValue(NULL, szPriv, &tp.Privileges[0].Luid)) break;
        if(!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) break;
		if(!::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) break;
        fSuccess = TRUE;
    } __finally {
		if (hToken != NULL)	::CloseHandle(hToken);
	}
    return(fSuccess);	
}

inline BOOL win32_EnableDebugPrivilege() {
	return win32_EnablePrivilege(SE_DEBUG_NAME);
}

ZDEV_NAMESPACE_END

#endif // __zdev_win32_token_hpp__