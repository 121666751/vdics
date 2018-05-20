#ifndef __ZSOCKETPLUS_H__
#define __ZSOCKETPLUS_H__

#ifdef CID_SE // -- CHECK_ID__SINGLE_EDITION
/#include <zsingle.h>
typedef ZVirtualSocket ZSocket;

#else

#include <zsocket.hpp>

#endif	//CHECKID_SINGLE_EDITION

#endif	//__ZSOCKETPLUS_H__
