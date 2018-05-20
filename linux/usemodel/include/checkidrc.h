#ifndef __CHECKIDRC_H__
#define __CHECKIDRC_H__

#include <windows.h>
#include <utility/libraryloader.h>

inline void show_about(const char* caption)
{
    ZDllLoader dll;
    if( dll.Load("checkidrc.dll") ) return;

    typedef void (__stdcall *FUN)(const char*);
    FUN fun = (FUN)dll.Addr("ShowAbout");

    fun(caption);
    return;
}

#endif //!__CHECKIDRC_H__
 