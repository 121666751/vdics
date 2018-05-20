#ifndef __xcdn_hpp__
#define __xcdn_hpp__

/**
 * 基本宏定义
 * WIN32:	_WIN32
 * BCB  :	__BCPLUSPLUS__
 * MSVC :	_MSC_VER
 */

#include "sys/zbase.hpp"
#include "sys/zfile.hpp"
#include "sys/zstring.hpp"

#include "sys/zproperties.hpp"

#include "sys/smart_ptr.hpp"
#include "sys/socket.hpp"

#ifdef _WIN32
    #include "win32/thread.hpp"	
	#include "win32/zwin32.hpp"
	//#include "win32/ztoken.hpp"
	#include "win32/zprocess.hpp"
	#include "win32/zapi32.hpp"
	#include "win32/ztcpip.hpp"
#else
    #include "linux/base.hpp"	
    #include "linux/thread.hpp"	
#endif


#endif //__xcdn_hpp__
