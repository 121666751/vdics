
#ifndef __MTYPE_H
#define __MTYPE_H

#ifndef __OS_LINUX_VER
	#pragma warning(disable : 4786)
#endif

#include <vector>
#include <string>
#include <queue>

using namespace std;

typedef long LONG;
typedef int BOOL;
typedef float FLOAT;
typedef double DOUBLE;
typedef int INT;
typedef short SHORT;
typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef char TCHAR;
typedef UCHAR *PUCHAR;
typedef char *PSZ;
typedef char *LPTSTR;
typedef const char *LPCTSTR;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef FLOAT* PFLOAT;
typedef BYTE* LPBYTE;

typedef long	UTID;
typedef unsigned int UINT;
typedef unsigned int *PUINT;

/////////////////////////////////

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

/////////////////////////////////

#endif
