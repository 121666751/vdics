//////////////////////////////////////////////////////////////////
//																//
//		用途 : 文件查找											//
//		创建 : [Foolish] / 2001-3-27							//
//		更新 : 2002-4-24										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_FILEFIND_H__
#define		__FOO_FILEFIND_H__
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "stddefine.h"
#pragma once

//===================================================================
//	文件查找类
//===================================================================
class FCFileFind
{
public :
	FCFileFind () ;
	~FCFileFind () ;

	//	操作
	BOOL	FindFile (PCTSTR szPathName) ;	// 如 : "c:\\Pic\\jpg\\*.*"
	bool	IsNoPath   ( ) const ;	// 在FindFile后调用，为TRUE，则不存在路径。
	BOOL	FindNext ( ) ;
	bool	IsDirectory ( ) const ;
	bool	IsNoMore   ( ) const ;	// 在FindNext后调用，为TRUE，则无更多匹配。（批量查找）
	void	FindClose( ) ;
	bool	IsExistFile (PCTSTR szPathName) ; // TRUE则存在

	//	获取属性
	DWORD	GetSize  ( ) const ;
	DWORD	GetAttribute ( ) const ;
	PCTSTR	GetFullName ( ) const ;
	PCTSTR	GetFileName ( ) ;	// 不带路径和扩展名的文件名
	PCTSTR	GetExtName  ( ) ;	// 返回如 : ".bmp"
	PCTSTR	GetDriver   ( ) ;	// 返回如 : "E:"
	PCTSTR	GetPath		( ) ;	// 返回如 : "\Pic\bmp\"

protected :
	void	Initialize ( ) ;
	void	SpliteName ( ) ;
protected :
	HANDLE			m_hFileFind ;		// 查找文件句柄
	DWORD			m_dwFileAttribute ;	// 文件属性
	DWORD			m_dwFileSize ;		// 文件大小

	TCHAR	m_szFindPath[_MAX_PATH] ;	// 查找路径名

	TCHAR	m_szFullName[_MAX_PATH] ;	// 带路径文件名
	TCHAR	m_szDriver[_MAX_DRIVE] ;	// 当前盘
	TCHAR	m_szPath[_MAX_DIR] ;		// 当前路径名
	TCHAR	m_szFileName[_MAX_FNAME] ;	// 不带路径文件名, 不带扩展名
	TCHAR	m_szFileExt[_MAX_EXT] ;		// 文件扩展名
} ;

//===================================================================
//	Implement
//===================================================================
inline FCFileFind::FCFileFind () {
	this->Initialize ( ) ;
}
inline FCFileFind::~FCFileFind () {
	this->FindClose () ;
}
inline void  FCFileFind::Initialize ( ) {
	m_hFileFind		  = INVALID_HANDLE_VALUE ;
	m_dwFileAttribute = 0xFFFFFFFF ;
	m_dwFileSize      = -1 ;
	m_szFindPath[0]	  = TEXT('\0') ;
	m_szFileName[0]	  = TEXT('\0') ;
	m_szFileExt[0]	  = TEXT('\0') ;
	m_szFullName[0]   = TEXT('\0') ;
	m_szDriver[0]     = TEXT('\0') ;
	m_szPath[0]		  = TEXT('\0') ;
}
inline void  FCFileFind::FindClose ( ) {
	if (m_hFileFind != INVALID_HANDLE_VALUE)
		::FindClose (m_hFileFind) ;
	this->Initialize ( ) ;
}
inline PCTSTR  FCFileFind::GetFullName ( ) const {
	return m_szFullName ;
}
inline void  FCFileFind::SpliteName ( ) {
	_splitpath (m_szFullName, m_szDriver, m_szPath, m_szFileName, m_szFileExt) ;
}
inline PCTSTR  FCFileFind::GetExtName ( ) {
	this->SpliteName () ;
	return m_szFileExt ;
}
inline PCTSTR  FCFileFind::GetFileName ( ) {
	this->SpliteName () ;
	return m_szFileName ;
}
inline PCTSTR  FCFileFind::GetDriver ( ) {
	this->SpliteName () ;
	return m_szDriver ;
}
inline PCTSTR  FCFileFind::GetPath ( ) {
	this->SpliteName () ;
	return m_szPath ;
}
inline DWORD  FCFileFind::GetSize ( ) const {
	return m_dwFileSize ;
}
inline DWORD  FCFileFind::GetAttribute ( ) const {
	return m_dwFileAttribute ;
}
inline bool  FCFileFind::IsNoMore ( ) const {
	return (::GetLastError () == ERROR_NO_MORE_FILES) ;
}
inline bool  FCFileFind::IsExistFile (PCTSTR szPathName) {
	this->FindFile (szPathName) ;
	bool	bRet = (::GetLastError () != ERROR_FILE_NOT_FOUND) ;
	this->FindClose () ;
	return bRet ;
}
inline bool  FCFileFind::IsNoPath ( ) const {
	return (::GetLastError () == ERROR_PATH_NOT_FOUND) ;	
}
inline bool  FCFileFind::IsDirectory ( ) const {
	return (m_dwFileAttribute == FILE_ATTRIBUTE_DIRECTORY) ;
}

#endif