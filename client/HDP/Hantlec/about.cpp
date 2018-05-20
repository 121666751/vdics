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
 * 文件：	$Id: about.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//---------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "about.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//---------------------------------------------------------------------
/**
    构造函数
    显示版本信息
*/
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
    char file[200];
    GetModuleFileName(NULL,file,sizeof(file));

    DWORD tmp;
    DWORD len = GetFileVersionInfoSize(file, &tmp);

    String ver_str;

    if( len )
    {
        unsigned char* block = new unsigned char[len+1];
        BOOL r = GetFileVersionInfo(file,NULL,len,block);
        VS_FIXEDFILEINFO *p;
        UINT buflen;
        r = VerQueryValue(block,"\\", &(void*)p,&buflen);
        if( r )
        {
            int major = HIWORD(p->dwFileVersionMS);
            int minor = LOWORD(p->dwFileVersionMS);
            int release = HIWORD(p->dwFileVersionLS);
            int build = LOWORD(p->dwFileVersionLS);
            ver_str.printf("版本：%d.%d.%d.%d",major,minor,release,build);
        }
        Version->Caption = ver_str;
        delete[] block;
    }
}
//--------------------------------------------------------------------- 
