//////////////////////////////////////////////////////////////////
//																//
//		用途 : 磁盘操作											//
//		创建 : [Foolish] / 2001-1-15							//
//		更新 : 2002-4-21										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_DISK_H__
#define		__FOO_DISK_H__
#include <windows.h>
#include <mmsystem.h>
#include "StdDefine.h"
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/****************************************************************/
	/*				获取磁盘空间									*/
	/*		例如 : szDrive == _T("c:\\") , 可以为子目录				*/
	/****************************************************************/
	BOOL  fooGetDiskSpace (PCTSTR	szDrive,
						   __int64	* TotalSize,
						   __int64	* FreeSize) ;

	/****************************************************************/
	/*				获取磁盘信息									*/
	/*		例如 : szDrive == _T("c:\\") , 必须为根目录				*/
	/****************************************************************/
	BOOL  fooGetVolumeInfo (PCTSTR		szDrive,
							VOLUMEINFO	* pVolInfo) ; // 在StdDefine.h中

	/****************************************************************/
	/*				枚举磁盘										*/
	/*	磁盘的序列号是在分区后系统产生的8字节随机数字, 当进行软盘复	*/
	/*	制时，两个软盘的序列号也被复制为一样的.						*/
	/*		iModeNum 为从0开始的序列, 返回FALSE则枚举完毕			*/
	/****************************************************************/
	BOOL  fooEnumVolume (int iModeNum, VOLUMEINFO * pVolInfo) ;

	/****************************************************************/
	/*				打开/关闭CDROM的门								*/
	/****************************************************************/
	void  fooOpenCdromDoor () ;
	void  fooCloseCdromDoor () ;
	bool  fooIsDiskInsert () ;

//===================================================================
inline void  fooOpenCdromDoor () {
	::mciSendString (TEXT("set cdaudio door open"), NULL, 0, 0) ;
}
inline void  fooCloseCdromDoor () {
	::mciSendString (TEXT("set cdaudio door closed"), NULL, 0, 0) ;
}
inline bool  fooIsDiskInsert () {
	TCHAR	string[256] ;
	::mciSendString (TEXT("status cdaudio ready"), string, 256, 0) ;
	return (lstrcmpi (string, TEXT("true")) == 0) ? true : false ;
}

#ifdef __cplusplus
}
#endif
#endif