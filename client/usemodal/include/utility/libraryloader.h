#ifndef __LIBRARY_LOADER_H__
#define __LIBRARY_LOADER_H__

#include <windows.h>

class ZDllLoader
{
public:
	ZDllLoader() 			{ 	m_hDll = NULL; }
	virtual ~ZDllLoader() 	{	if( m_hDll ) FreeLibrary(m_hDll);	}
	
	int Load(const char* filename) {
		if( m_hDll ) FreeLibrary(m_hDll);
		m_hDll = LoadLibrary(filename);
		return (m_hDll)? 0: -1;
	}
	
	void* Addr(const char* procname) {
		return (m_hDll)? GetProcAddress(m_hDll,procname): NULL;
	}
	
	void* operator () (const char* procname) {
		return Addr(procname);
	}


protected:
	HMODULE m_hDll;
};

#endif //__LIBRARY_LOADER_H__