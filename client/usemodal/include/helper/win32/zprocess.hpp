#ifndef __zdev_win32_process_hpp__
#define __zdev_win32_process_hpp__

#include "../sys/zbase.hpp"
#include <windows.h>
#include <tlhelp32.h>

ZDEV_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
int win32_findProcessid_byName(string processName);

//////////////////////////////////////////////////////////////////////////
// 通过进程名得到进程ID. 
// 如果未找到，返回 -1
inline int win32_findProcessid_byName(string processName) {
	int pid = -1;
	HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	struct tagPROCESSENTRY32 processsnap; 
	processsnap.dwSize = sizeof(tagPROCESSENTRY32);
	for(::Process32First(snapshot,&processsnap); ::Process32Next(snapshot,&processsnap); ){
		if(!stricmp(processsnap.szExeFile, processName.c_str())) {
			pid = processsnap.th32ProcessID;
			break;
		}
	}
	::CloseHandle(snapshot);
	return pid;
}


ZDEV_NAMESPACE_END

#endif // __zdev_win32_process_hpp__