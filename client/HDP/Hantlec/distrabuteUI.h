/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 北京天创征腾信息科技有限公司    版权所有
 *
 * 此文件为天创征腾软件产品的一部分。
 * 天创征腾软件产品的所有部分,包括但不限于软件产品中所含的任何图象、照片、
 * 动画、文字和附加程序（applets）、加密卡、随附的印刷材料、源代码文件及
 * 本软件产品的任何副本的一切所有权和知识产权，均归版权人天创征腾拥有。
 *
 * 文件：	$Id: distrabuteUI.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
_DISTRABUTEUI_H__
#define __DISTRABUTEUI_H__

#ifdef __cplusplus
extern "C" {
#endif

HANDLE __stdcall InitDistUI( HWND Handle );
int __stdcall StartDst(int nDstMode, int nDstState, HANDLE hDistUI);
void __stdcall ReleaseDistUI( HANDLE Handle );

#ifdef __cplusplus
}
#endif

#endif //__DISTRABUTEUI_H__
 