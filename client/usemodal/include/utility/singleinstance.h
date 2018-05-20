#ifndef __SINGLE_INSTANCE_H__
#define __SINGLE_INSTANCE_H__

#include <windows.h>

class ZSingleInstance
{
public:
	ZSingleInstance();
	virtual ~ZSingleInstance();

	int	init(const char* registString);
protected:
	HANDLE m_mutex;
};




//////////////////////////////////////////////////////////////
inline ZSingleInstance::ZSingleInstance() 
{ 
	m_mutex=NULL; 
}
	
inline ZSingleInstance::~ZSingleInstance() 
{ 
	if( m_mutex ) CloseHandle(m_mutex); 
}

inline int ZSingleInstance::init(const char* registString)
{
	if( !registString || !strlen(registString) ) return -1;
	
	m_mutex = CreateMutex(NULL,false,registString);
	
	if( (m_mutex==NULL) || (GetLastError()==ERROR_ALREADY_EXISTS) ) return -2;
	
	return 0;
}

#endif //__SINGLE_INSTANCE_H__