//////////////////////////////////////////////////////////////////
//																//
//		用途 : 系统进/线程监视									//
//		创建 : [Foolish] / 2001-5-5				   				//
//		更新 : 2002-1-15										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_TOOLHELP_H__
#define		__FOO_TOOLHELP_H__
#include <windows.h>
#include <tlhelp32.h>
#pragma once

//===================================================================
//	Declaration
//===================================================================
class FCToolHelp
{
public :
	 FCToolHelp () ;
	~FCToolHelp () ;

	/****************************************************************/
	/*	dwProcess == 0 为当前进程									*/
	/*	只有在dwFlags为 TH32CS_SNAPHEAPLIST 或 TH32CS_SNAPMODULE	*/
	/*	时dwProcess才有用											*/
	/****************************************************************/
	bool  CreateSnapshot (DWORD dwFlags = TH32CS_SNAPALL,
						  DWORD dwProcessID = 0) ;

	BOOL  GetProcess (PROCESSENTRY32 * pProcess) ;
	BOOL  GetThread (THREADENTRY32 * pThread) ;
	BOOL  GetModule (MODULEENTRY32 * pModule) ;
	BOOL  ModuleFind (PCTSTR pszModName, MODULEENTRY32 * pModule = NULL) const ;

private :
	HANDLE	m_hSnapshot ;
	BOOL	m_bFirstProcess, m_bFirstThread, m_bFirstModule ;
	void	initialize () ;
} ;

//===================================================================
//	Implement
//===================================================================
inline FCToolHelp::FCToolHelp () {
	m_hSnapshot = INVALID_HANDLE_VALUE ;
	m_bFirstProcess = m_bFirstThread = m_bFirstModule = TRUE ;
}
inline FCToolHelp::~FCToolHelp() {
	this->initialize () ;
}
inline void  FCToolHelp::initialize () {
	if (m_hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle (m_hSnapshot) ;
	m_hSnapshot = INVALID_HANDLE_VALUE ;
	m_bFirstProcess = m_bFirstThread = m_bFirstModule = TRUE ;
}
inline bool  FCToolHelp::CreateSnapshot (DWORD dwFlags, DWORD dwProcessID) {
	this->initialize () ;
	m_hSnapshot = ::CreateToolhelp32Snapshot (dwFlags, dwProcessID) ;
	return (m_hSnapshot != INVALID_HANDLE_VALUE) ;
}
inline BOOL  FCToolHelp::GetProcess (PPROCESSENTRY32 ppe) {
	if (ppe == NULL)
		return FALSE ;
	else
		ppe->dwSize = sizeof (*ppe) ;
	BOOL	fOk ;
	if (m_bFirstProcess)
	{
		fOk = ::Process32First (m_hSnapshot, ppe) ;
		m_bFirstProcess = FALSE ;
	}
	else
		fOk = ::Process32Next (m_hSnapshot, ppe) ;
	if (fOk && (ppe->th32ProcessID == 0))
		fOk = this->GetProcess (ppe) ; // Remove the "[System Process]" (PID = 0)
	return fOk ;
}
inline BOOL  FCToolHelp::GetThread (PTHREADENTRY32 pte) {
	if (pte == NULL)
		return FALSE ;
	else
		pte->dwSize = sizeof (*pte) ;
	BOOL	fOk ;
	if (m_bFirstThread)
	{
		fOk = ::Thread32First (m_hSnapshot, pte) ;
		m_bFirstThread = FALSE ;
	}
	else
		fOk = ::Thread32Next (m_hSnapshot, pte) ;
	return fOk ;
}
inline BOOL  FCToolHelp::GetModule (PMODULEENTRY32 pme) {
	if (pme == NULL)
		return FALSE ;
	else
		pme->dwSize = sizeof (*pme) ;
	BOOL	fOk ;
	if (m_bFirstModule)
	{
		fOk = ::Module32First (m_hSnapshot, pme) ;
		m_bFirstModule = FALSE ;
	}
	else
		fOk = ::Module32Next (m_hSnapshot, pme) ;
	return fOk ;
}
inline BOOL  FCToolHelp::ModuleFind (PCTSTR pszModName, PMODULEENTRY32 pmeout) const {
	MODULEENTRY32	inner, * pme ;
	if (pmeout == NULL)
		pme = &inner ;
	else
		pme = pmeout ;
	BOOL  fFound = FALSE ;
	pme->dwSize = sizeof (*pme) ;
	for (BOOL fOk = ::Module32First (m_hSnapshot, pme) ; fOk ; fOk = ::Module32Next (m_hSnapshot, pme)) {
		fFound = (lstrcmpi (pme->szModule, pszModName) == 0) || 
				 (lstrcmpi (pme->szExePath, pszModName) == 0) ;
		if (fFound) break ;
	}
   return fFound ;
}

#endif