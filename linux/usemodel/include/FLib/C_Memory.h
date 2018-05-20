//////////////////////////////////////////////////////////////////
//																//
//		用途 : 内存操作											//
//		创建 : [Foolish] / 2001-6-5								//
//		更新 :													//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_MEMORY_H__
#define		__FOO_MEMORY_H__
#include <windows.h>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/****************************************************************/
	/*				创建一块共享内存								*/
	/*		如果存在pMemName, 则不能创建, 返回NULL					*/
	/****************************************************************/
	void *  fooMallocSharedMemory (PCTSTR	pMemName,
								   int		iBlockSize,
								   HANDLE	* hMapping) ; // 不可为NULL

	/****************************************************************/
	/*				释放一块共享内存								*/
	/****************************************************************/
	void  fooFreeSharedMemory (void * pBaseAddr, HANDLE hMapping) ;

	/****************************************************************/
	/*				读/写一块共享内存								*/
	/****************************************************************/
	BOOL  fooReadSharedMemory (PCTSTR	pMemName,
							   BYTE		* pReceive,
							   int		iExpectBytes) ;

	BOOL  fooWriteSharedMemory (PCTSTR	pMemName,
								BYTE	* pWrite,
								int		iWriteBytes) ;

	/****************************************************************/
	/*			判断是否存在名为pMemName的共享内存					*/
	/****************************************************************/
	bool  fooIsExistSharedMemory (PCTSTR pMemName) ;

//===================================================================
inline void  fooFreeSharedMemory (void * pBaseAddr, HANDLE hMapping) {
	if (pBaseAddr != NULL)
		::UnmapViewOfFile (pBaseAddr) ;
	if (hMapping != NULL)
		::CloseHandle (hMapping) ;
}
inline bool fooIsExistSharedMemory (PCTSTR pMemName) {
	HANDLE	hMap = NULL ;
	if (pMemName != NULL)
	{
		hMap = ::OpenFileMapping (FILE_MAP_READ, FALSE, pMemName) ;
		if (hMap != NULL)
			::CloseHandle (hMap) ;
	}
	return hMap ? true : false ;
}

#ifdef __cplusplus
}
#endif
#endif