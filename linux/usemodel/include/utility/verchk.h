#ifndef __VERCHK_H__
#define __VERCHK_H__


#ifdef __cpluscplus
extern "C" {	
#endif //__cpluscplus


// **************************************************************************
// check the version file between local and center
// return:
// 0: version is equal, 1: local is newer, -1: center is newer, else: error
// **************************************************************************
long __stdcall cidCheckVersion();


// **************************************************************************
// download patchfile from center
// Parameter:
//		retPatchName	-alloced memory pointer by caller to store the 
//						 filename of the patch.
// return:
// 0: get success.  else: error
// **************************************************************************
long __stdcall cidGetPatch(char* retPatchName);


#ifdef __cpluscplus
}
#endif //__cpluscplus

#endif //__VERCHK_H__