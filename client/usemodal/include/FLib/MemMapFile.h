//////////////////////////////////////////////////////////////////
//																//
//		用途 : 内存映射的 - 文件操作 ( Memory Mapping File )	//
//		创建 : [Foolish] / 2001-4-19							//
//		更新 : 2002-7-25										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_MEMMAPFILE_H__
#define		__FOO_MEMMAPFILE_H__
#include <windows.h>
#pragma once

//===================================================================
//	Declaration
//===================================================================
class FCMemMapFile
{
public :
	FCMemMapFile () ;
	~FCMemMapFile () ;
	void	UnMap () ;

	/*************************************************************/
	/*				 覆盖只读文件会失败							 */
	/*************************************************************/
	void *	CreateFile (PCTSTR szFileName,
						DWORD dwMaxFileSize,
						bool bOverWrite = false,
						DWORD dwShare = FILE_SHARE_READ) ;

	/*************************************************************/
	/*			 读取文件长度为0的文件会失败					 */
	/*************************************************************/
	void *	ReadFile (PCTSTR szFileName,
					  DWORD dwShare = FILE_SHARE_READ) ;

	/*************************************************************/
	/*			 读取文件长度为0的文件会失败					 */
	//	dwMaxFileSize == 0 表示不改变文件大小					 */
	//	dwMaxFileSize小于文件长度，则不改变大小，否则以0填充	 */
	/*************************************************************/
	void *	ModifyFile (PCTSTR szFileName,
						DWORD dwMaxFileSize = 0,
						DWORD dwShare = FILE_SHARE_READ) ;

	HANDLE	GetFileHandle () const ;
	HANDLE	GetMappingHandle () const ;
	void *	GetBaseAddress () const ;
	void	FlushFile () const ;
	void	SetSize (DWORD dwFileSize) ; // 此函数的调用必须放在最后

protected :
	HANDLE		m_hFile ;
	HANDLE		m_hFileMap ;
	VOID	  * m_pMapAddress ;
} ;

//===================================================================
//	Implement
//===================================================================
inline FCMemMapFile::FCMemMapFile () {
	m_hFile = INVALID_HANDLE_VALUE ;
	m_hFileMap = NULL ;
	m_pMapAddress = NULL ;
}
inline FCMemMapFile::~FCMemMapFile () {
	this->UnMap () ;
}
inline HANDLE FCMemMapFile::GetFileHandle () const {
	return m_hFile ;
}
inline HANDLE FCMemMapFile::GetMappingHandle () const {
	return m_hFileMap ;
}
inline void * FCMemMapFile::GetBaseAddress () const {
	return m_pMapAddress ;
}
inline void FCMemMapFile::FlushFile () const {
	if (m_pMapAddress != NULL)
		::FlushViewOfFile (m_pMapAddress, 0) ;
}
inline void FCMemMapFile::UnMap () {
	if (m_pMapAddress != NULL)				::UnmapViewOfFile (m_pMapAddress) ;
	if (m_hFileMap != NULL)					CloseHandle (m_hFileMap) ;
	if (m_hFile != INVALID_HANDLE_VALUE)	CloseHandle (m_hFile) ;
	m_hFile = INVALID_HANDLE_VALUE ;
	m_hFileMap = NULL ;
	m_pMapAddress = NULL ;
}
inline void FCMemMapFile::SetSize (DWORD dwFileSize) {
	if (m_pMapAddress != NULL)		::UnmapViewOfFile (m_pMapAddress) ;
	if (m_hFileMap != NULL)			CloseHandle (m_hFileMap) ;
	if (m_hFile != INVALID_HANDLE_VALUE) {
		::SetFilePointer (m_hFile, dwFileSize, NULL, FILE_BEGIN) ;
		::SetEndOfFile (m_hFile) ;
		CloseHandle (m_hFile) ;
	}
	m_hFile = INVALID_HANDLE_VALUE ;
	m_hFileMap = NULL ;
	m_pMapAddress = NULL ;
}

#endif