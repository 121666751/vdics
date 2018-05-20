//////////////////////////////////////////////////////////////////
//																//
//		用途 : 注册表操作										//
//		创建 : [Foolish] / 2001-4-23							//
//		更新 : 2002-1-29										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef	 __FOO_REGISTRY_H__
#define	 __FOO_REGISTRY_H__
#include <windows.h>
#pragma once

//===================================================================
/********************************************************************/
/*			本类不区分大小写										*/
/********************************************************************/
class FCReg
{
public :
	FCReg  ( ) ;
	~FCReg ( ) ;
	operator HKEY ( ) const ;

	//	键值必须存在，要打开根键则szSubKey = NULL
	BOOL  OpenKey (HKEY hPreDefinedKey, PCTSTR szSubKey = NULL) ;

	//	如果键值存在则打开它 (必须先打开了一个键, 即先调用了OpenKey函数)
	BOOL  CreateKey (PCTSTR szSubKey) ;
	BOOL  CreateKeyAndAttach (PCTSTR szSubKey) ;

	//	找到返回TRUE，否则返回FALSE
	BOOL  EnumRegKey (TCHAR * szNameBuffer, bool bReset = false) ;
	BOOL  EnumRegValue (TCHAR * szNameBuffer, bool bReset = false) ;

	//	以下三个函数Value不存在则建立，存在则覆盖
	BOOL  SetValue (PCTSTR szValueName, DWORD dwData) ;
	BOOL  SetValue (PCTSTR szValueName, PCTSTR String) ;
	BOOL  SetValue (PCTSTR szValueName, BYTE * pBinary, DWORD dwCount) ;

	BOOL  QueryValueSize (PCTSTR szValueName, DWORD * dwSize) ;  // 字符串包括 NULL 字符
	BOOL  QueryValueType (PCTSTR szValueName, DWORD * dwType) ;

	BOOL  QueryValue (PCTSTR szValueName, DWORD * pDword) ;
	BOOL  QueryValue (PCTSTR szValueName, TCHAR * pString) ;
	BOOL  QueryValue (PCTSTR szValueName, BYTE * pBinary, DWORD * dwCount) ;

	void  FlushReg ( ) ;
	BOOL  DeleteSubKey (PCTSTR szSubKey) ;
	BOOL  DeleteValue (PCTSTR szValueName) ;
	void  CloseKey ( ) ;

protected :
	HKEY	m_hKey ;
} ;

//===================================================================
inline  FCReg::FCReg () {
	m_hKey = NULL ;
}
inline  FCReg::~FCReg () {
	this->CloseKey () ;
}
inline void  FCReg::CloseKey () {
	if (m_hKey != NULL)	{
		::RegCloseKey (m_hKey) ; m_hKey = NULL ;
	}
}
inline FCReg::operator HKEY() const {
	return m_hKey ;
}
inline BOOL  FCReg::SetValue (PCTSTR szValueName, DWORD dwData) {
	return (RegSetValueEx (m_hKey, szValueName, 0, REG_DWORD, (CONST BYTE *)&dwData, sizeof(DWORD)) == ERROR_SUCCESS) ;
}
inline BOOL  FCReg::SetValue (PCTSTR szValueName, PCTSTR String) {
	return (RegSetValueEx (m_hKey, szValueName, 0, REG_SZ, (CONST BYTE *)String, lstrlen(String) + 1) == ERROR_SUCCESS) ;
}
inline void  FCReg::FlushReg ( ) {
	::RegFlushKey (m_hKey) ;
}
inline BOOL  FCReg::SetValue (PCTSTR szValueName, BYTE * pBinary, DWORD dwCount) {
	return (RegSetValueEx (m_hKey, szValueName, 0, REG_BINARY, (CONST BYTE *)pBinary, dwCount) == ERROR_SUCCESS) ;
}
inline BOOL  FCReg::DeleteValue (PCTSTR szValueName) {
	return (::RegDeleteValue (m_hKey, szValueName) == ERROR_SUCCESS) ;
}
inline BOOL  FCReg::DeleteSubKey (PCTSTR szSubKey) {
	return  (::RegDeleteKey (m_hKey, szSubKey) == ERROR_SUCCESS) ;
}
inline BOOL  FCReg::QueryValueSize (PCTSTR szValueName, DWORD * dwSize) {
	return (::RegQueryValueEx (m_hKey, szValueName, NULL, NULL, NULL, dwSize) == ERROR_SUCCESS) ;
}
inline BOOL  FCReg::QueryValueType (PCTSTR szValueName, DWORD * dwType) {
	return (::RegQueryValueEx (m_hKey, szValueName, NULL, dwType, NULL, NULL) == ERROR_SUCCESS) ;
}
inline BOOL  FCReg::OpenKey (HKEY hKey, PCTSTR szSubKey) {
	this->CloseKey () ;
	return (::RegOpenKeyEx (hKey, (szSubKey == NULL) ? TEXT("") : szSubKey, 0, KEY_ALL_ACCESS, &m_hKey) == ERROR_SUCCESS) ;
}
inline BOOL  FCReg::CreateKey (PCTSTR szSubKey) {
	HKEY	hChild ; DWORD	dwDisposition ;
	if ((m_hKey == NULL) || (szSubKey == NULL))
		return FALSE ;
	if (::RegCreateKeyEx (m_hKey, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE,
						  KEY_ALL_ACCESS, NULL, &hChild, &dwDisposition) != ERROR_SUCCESS)
		return FALSE ;
	::RegCloseKey (hChild) ;
	return TRUE ;
}
inline BOOL  FCReg::CreateKeyAndAttach (PCTSTR szSubKey) {
	HKEY	hChild ; DWORD	dwDisposition ;
	if ((m_hKey == NULL) || (szSubKey == NULL))
		return FALSE ;
	if (::RegCreateKeyEx (m_hKey, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE,
						  KEY_ALL_ACCESS, NULL, &hChild, &dwDisposition) != ERROR_SUCCESS)
		return FALSE ;
	::RegCloseKey (m_hKey) ;
	m_hKey = hChild ;
	return TRUE ;
}
inline BOOL  FCReg::QueryValue (PCTSTR szValueName, DWORD * pDword) {
	DWORD	cbData = sizeof(DWORD) ;
	BYTE	* pBuffer = new BYTE [cbData] ;
	if (RegQueryValueEx (m_hKey, szValueName, NULL, NULL, pBuffer, &cbData) != ERROR_SUCCESS)
	{
		delete[] pBuffer ;
		return FALSE ;
	}
	*pDword = *((DWORD *)pBuffer) ;
	delete[] pBuffer ;
	return TRUE ;
}
inline BOOL  FCReg::QueryValue (PCTSTR szValueName, TCHAR * pString) {
	DWORD	dwTemp = MAX_PATH ;
	return (RegQueryValueEx (m_hKey, szValueName, NULL, NULL, (BYTE *)pString, &dwTemp) == ERROR_SUCCESS) ;
}
inline BOOL  FCReg::QueryValue (PCTSTR szValueName, BYTE * pBinary, DWORD * dwCount) {
	*dwCount = MAX_PATH ;
	return (RegQueryValueEx (m_hKey, szValueName, NULL, NULL, pBinary, dwCount) == ERROR_SUCCESS) ;
}
inline BOOL  FCReg::EnumRegKey (PTSTR  szNameBuffer, bool bReset) {
	static DWORD	dwIndex = 0 ;
	DWORD	temp = MAX_PATH ;
	TCHAR	buffer[MAX_PATH] = { TEXT('\0') } ;
	if (bReset)
		dwIndex = 0 ;
	::RegEnumKeyEx (m_hKey, dwIndex++, buffer, &temp, NULL, NULL, NULL, NULL) ;
	if (szNameBuffer != NULL)
		lstrcpy (szNameBuffer, buffer) ;
	return (buffer[0] != TEXT('\0')) ;
}
inline BOOL  FCReg::EnumRegValue (PTSTR  szNameBuffer, bool bReset) {
	static DWORD	dwIndex = 0 ;
	DWORD	temp = MAX_PATH ;
	TCHAR	buffer[MAX_PATH] = { TEXT('\0') } ;
	if (bReset)
		dwIndex = 0 ;
	::RegEnumValue (m_hKey, dwIndex++, buffer, &temp, NULL, NULL, NULL, NULL) ;
	if (szNameBuffer != NULL)
		lstrcpy (szNameBuffer, buffer) ;
	return (buffer[0] != TEXT('\0')) ;
}

#endif