//////////////////////////////////////////////////////////////////
//																//
//		用途 : 进/线程操作										//
//		创建 : [Foolish] / 2001-1-15							//
//		更新 : 2002-4-21										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_PROCESS_H__
#define		__FOO_PROCESS_H__
#include <windows.h>
#include <TCHAR.H>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/****************************************************************/
	/*				判断是否运行在9x系统下							*/
	/****************************************************************/
	bool  fooIsWindows9x () ;

	/****************************************************************/
	/*					创建进程									*/
	/*			参数见ShowWindow, SetPriorityClass					*/
	/****************************************************************/
	BOOL  fooCreateProcess (PCTSTR	szFileName,
							int		nCmdShow = SW_SHOWNORMAL,
							DWORD	dwPriority = NORMAL_PRIORITY_CLASS) ;

	/****************************************************************/
	/*					创建线程									*/
	/*		参数见SetThreadPriority,  CloseHandle可以立即结束线程	*/
	/****************************************************************/
	HANDLE  fooCreateThread (void	* pStartAddress,
							 void	* pParameter = NULL,
							 DWORD	dwPriority = THREAD_PRIORITY_NORMAL) ;

	/****************************************************************/
	/*					注入DLL到其他进程							*/
	/*			必须用_L()将szDllFile变为UNICODE字符串				*/
	/****************************************************************/
	BOOL  fooInjectDLL_W (DWORD dwProcessId, PCWSTR szDllFile) ;

	/****************************************************************/
	/*				得到当前进程的真实句柄							*/
	/*	::GetCurrentProcess() 获得的是伪句柄, 总是返回0x7FFFFFFF	*/
	/****************************************************************/
	HANDLE  fooGetRealProcessHandle () ;

	/****************************************************************/
	/*				重启动/关闭/登录系统							*/
	/****************************************************************/
	void  fooRebootSys () ;
	void  fooShutDownSys  () ;
	void  fooLogoSys () ;

	/****************************************************************/
	/*				获取pPrivilegeName权限	(win2000)				*/
	/*			pPrivilegeName在WINNT.h中定义 : SE_***				*/
	/****************************************************************/
	BOOL  fooEnablePrivilege (PCTSTR pPrivilegeName) ;

	/****************************************************************/
	/*		获取Debug权限以(调用OpenProcess)打开进程  (win2000)		*/
	/****************************************************************/
	BOOL  fooEnableDebugPrivilege () ;

	/****************************************************************/
	/*					禁止/允许任务管理器							*/
	/****************************************************************/
	void  fooEnableTaskDlg (BOOL bEnable) ;

//===================================================================
inline BOOL  fooEnableDebugPrivilege () {
	return fooEnablePrivilege (SE_DEBUG_NAME) ;
}
inline HANDLE  fooGetRealProcessHandle () {
	HANDLE		hProcess ;
	DuplicateHandle (::GetCurrentProcess (), ::GetCurrentProcess(), ::GetCurrentProcess(),
					 &hProcess, 0, FALSE, DUPLICATE_SAME_ACCESS) ;
	return hProcess ;
}
inline bool  fooIsWindows9x () {
	OSVERSIONINFO	osv ;
	::GetVersionEx (&osv) ;
	return (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) ;
}
inline void  fooRebootSys () {
	if (!::fooIsWindows9x ())
		::fooEnablePrivilege (SE_SHUTDOWN_NAME) ;
	::ExitWindowsEx (EWX_REBOOT, 0) ;
}
inline void  fooShutDownSys () {
	if (!::fooIsWindows9x ())
		::fooEnablePrivilege (SE_SHUTDOWN_NAME) ;
	ExitWindowsEx (EWX_POWEROFF, 0) ;
}
inline void  fooLogoSys () {
	ExitWindowsEx (EWX_LOGOFF, 0) ;
}

#ifdef __cplusplus
}
#endif
#endif