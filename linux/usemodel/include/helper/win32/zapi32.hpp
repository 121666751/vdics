#ifndef __zdev_win32_api32_hpp__
#define __zdev_win32_api32_hpp__

#include "../sys/zbase.hpp"
#include <windows.h>

ZDEV_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
// in kernel32.dll
typedef HMODULE (WINAPI *__LoadLibrary)(LPCTSTR);
typedef HMODULE (WINAPI *__LoadLibraryA)(LPCSTR);
typedef HMODULE (WINAPI *__LoadLibraryW)(LPCWSTR);
typedef LPVOID (WINAPI *__GetProcAddress)(HMODULE, LPCSTR);
typedef BOOL (WINAPI *__FreeLibrary)(HMODULE);
typedef int (WINAPI *__CreateProcess)(LPCTSTR,LPTSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,BOOL,DWORD,LPVOID,LPCTSTR,LPSTARTUPINFOW,LPPROCESS_INFORMATION);
typedef int (WINAPI *__CreateProcessA)(LPCSTR,LPSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,BOOL,DWORD,LPVOID,LPCSTR,LPSTARTUPINFOW,LPPROCESS_INFORMATION);
typedef int (WINAPI *__CreateProcessW)(LPCWSTR,LPWSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,BOOL,DWORD,LPVOID,LPCWSTR,LPSTARTUPINFOW,LPPROCESS_INFORMATION);
typedef UINT (WINAPI *__WinExec)(LPCSTR,UINT);
typedef BOOL (WINAPI *__CreateDirectory)(LPCTSTR, LPSECURITY_ATTRIBUTES);
typedef BOOL (WINAPI *__CreateDirectoryA)(LPCSTR, LPSECURITY_ATTRIBUTES);
typedef BOOL (WINAPI *__CreateDirectoryW)(LPCWSTR, LPSECURITY_ATTRIBUTES);
typedef BOOL (WINAPI *__IsBadReadPtr)(CONST VOID*,UINT);
typedef LPVOID (WINAPI *__VirtualAlloc)(LPVOID lpAddress,DWORD dwSize,DWORD flAllocationType,DWORD flProtect);
typedef BOOL (WINAPI *__VirtualFree)(LPVOID lpAddress,DWORD dwSize,DWORD dwFreeType);	
typedef VOID (WINAPI *__RtlMoveMemory)(PVOID,CONST VOID*,SIZE_T);	

// in user32.dll
typedef int (WINAPI *__MessageBox)(HWND, LPCTSTR, LPCTSTR, UINT);
typedef int (WINAPI *__MessageBoxA)(HWND, LPCSTR, LPCSTR, UINT);
typedef int (WINAPI *__MessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT);


ZDEV_NAMESPACE_END

#endif // __zdev_win32_api32_hpp__