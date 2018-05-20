//////////////////////////////////////////////////////////////////
//																//
//		用途 : x86-CPU操作										//
//		创建 : [Foolish] / 2001-3-15							//
//		更新 : 2002-4-21										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_X86CPU_H__
#define		__FOO_X86CPU_H__
#include <windows.h>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/************************************************************/
	/*				 测试CPU的主频								*/
	/*		在2000下, CPU主频可以直接从Registry中获得			*/
	/*	 H_L_M\HARDWARE\DESCRIPTION\System\CentralProcessor\0	*/
	/************************************************************/
	void *  fooStartCountCpuSpeed () ;
	DWORD  fooEndCountCpuSpeed (void * pRet) ; // pRet 为上一函数返回值

	/************************************************************/
	/*				 精确延时									*/
	/*		 主频为~MHz, 延迟时间为<微>秒数						*/
	/************************************************************/
	void  fooDelay (int iFrequency, DWORD dwDelayTime) ;

	/************************************************************/
	/*				 获得本机CPU的数目							*/
	/************************************************************/
	DWORD  fooGetCpuNumber () ;

	/************************************************************/
	/*		从pData开始的字节数组加iAdd, 并饱和在[0,255]		*/
	/*			iAdd 在 [-255,255] 中，可以为负值				*/
	/*					调节图象亮度时使用						*/
	/************************************************************/
	void  fooMMX_ADD (BYTE * pData, int iLength, int iAdd) ;

//===================================================================
__forceinline void  fooDelay (int iFreq, DWORD dwDelayTime) {
	ULARGE_INTEGER		uStart, uFlag ;
	__asm
	{
		rdtsc
		mov		uStart.LowPart, eax		// 低32位
		mov		uStart.HighPart, edx	// 高32位
	}
	uFlag.QuadPart = dwDelayTime * iFreq + uStart.QuadPart - 5 ;
	do {
		__asm
		{
			rdtsc
			mov		uStart.LowPart, eax		// 低32位
			mov		uStart.HighPart, edx	// 高32位
		}
	} while (uStart.QuadPart < uFlag.QuadPart) ;
}
//===================================================================
inline void  fooMMX_ADD (BYTE * pData, int iLength, int iAdd) {
	if (iLength <= 8) 
	{
		for ( ; iLength > 0 ; iLength--)
			*pData++ = max (min (*pData + iAdd, 255), 0) ;
		return ;
	}
	BYTE   byMM7[8] ;
	memset (byMM7, (iAdd > 0) ? iAdd : (-iAdd), 8) ;

	// 使pData 8Byte对齐
	DWORD	temp = 8 - ((DWORD)pData % 8) ;
	iLength -= temp ;
	for ( ; temp > 0 ; temp--)
		*pData++ = max (min (*pData + iAdd, 255), 0) ;

	__asm
	{
		mov		eax, iLength			// eax存放剩余字节数
		mov		esi, dword ptr pData	// esi存放数据
		lea		edi, byMM7
		movq	mm7, [edi]
		cmp		iAdd, 0
		jng		foo_sub
foo_add :
		cmp		eax, 32
		jb		not_enough
		movq	mm0, [esi + 0]
		movq	mm1, [esi + 8]
		movq	mm2, [esi + 16]
		movq	mm3, [esi + 24]
		paddusb	mm0, mm7
		paddusb	mm1, mm7
		paddusb	mm2, mm7
		paddusb	mm3, mm7
		movq	[esi + 0], mm0
		movq	[esi + 8], mm1
		movq	[esi + 16], mm2
		movq	[esi + 24], mm3
		sub		eax, 32
		add		esi, 32
		jmp		foo_add
foo_sub :
		cmp		eax, 32
		jb		not_enough
		movq	mm0, [esi + 0]
		movq	mm1, [esi + 8]
		movq	mm2, [esi + 16]
		movq	mm3, [esi + 24]
		psubusb	mm0, mm7
		psubusb	mm1, mm7
		psubusb	mm2, mm7
		psubusb	mm3, mm7
		movq	[esi + 0], mm0
		movq	[esi + 8], mm1
		movq	[esi + 16], mm2
		movq	[esi + 24], mm3
		sub		eax, 32
		add		esi, 32
		jmp		foo_sub
not_enough :
		mov		dword ptr temp, eax
		mov		dword ptr pData, esi
		emms
	}
	for ( ; temp > 0 ; temp--)
		*pData++ = max (min (*pData + iAdd, 255), 0) ;
}

#ifdef __cplusplus
}
#endif
#endif