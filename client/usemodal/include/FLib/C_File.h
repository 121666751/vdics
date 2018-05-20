//////////////////////////////////////////////////////////////////
//																//
//		用途 : 文件操作											//
//		创建 : [Foolish] / 1999-10-9							//
//		更新 :													//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_FILE_H__
#define		__FOO_FILE_H__
#include <windows.h>
#pragma once

#include "stddefine.h"
#ifdef __cplusplus
extern "C" {
#endif

	/****************************************************************/
	/*		为NULL则在系统Temp目录下创建, Close() 时自动删除		*/
	/*		如 : szPathName == "c:\\Pic\\jpg\\"						*/
	/****************************************************************/
	HANDLE  fooCreateTempFile (PCTSTR szPathName = NULL) ;

	/****************************************************************/
	/*		得到文件长度 (win98)									*/
	/****************************************************************/
	DWORD  fooGetFileSize (PCTSTR szFileName) ;

	/****************************************************************/
	/*		增加/删除文件属性										*/
	/****************************************************************/
	BOOL  fooDelFileAttributes (PCTSTR szFileName, DWORD dwAttribute) ;
	BOOL  fooAddFileAttributes (PCTSTR szFileName, DWORD dwAttribute) ;

//===================================================================
inline DWORD fooGetFileSize (PCTSTR szFileName) {
	WIN32_FILE_ATTRIBUTE_DATA	file_data ;
	return (::GetFileAttributesEx (szFileName, GetFileExInfoStandard, &file_data) == 0)
		    ? -1 : file_data.nFileSizeLow ;
}
inline BOOL  fooDelFileAttributes (PCTSTR szFileName, DWORD dwAttribute) {
	DWORD	dwAtt = ::GetFileAttributes (szFileName) ;
	if (dwAtt == -1)	return FALSE ;
	dwAtt &= ~dwAttribute ;
	return ::SetFileAttributes (szFileName, dwAtt) ;
}
inline BOOL  fooAddFileAttributes (PCTSTR szFileName, DWORD dwAttribute) {
	DWORD	dwAtt = ::GetFileAttributes (szFileName) ;
	if (dwAtt == -1)	return FALSE ;
	dwAtt |= dwAttribute ;
	return ::SetFileAttributes (szFileName, dwAtt) ;
}

#ifdef __cplusplus
}
#endif
#endif