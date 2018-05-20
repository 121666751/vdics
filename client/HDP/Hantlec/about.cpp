/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 �����촴������Ϣ�Ƽ����޹�˾    ��Ȩ����
 *
 * ���ļ�Ϊ�촴���������Ʒ��һ���֡�
 * �촴���������Ʒ�����в���,�����������������Ʒ���������κ�ͼ����Ƭ��
 * ���������ֺ͸��ӳ���applets�������ܿ����渽��ӡˢ���ϡ�Դ�����ļ���
 * �������Ʒ���κθ�����һ������Ȩ��֪ʶ��Ȩ�������Ȩ���촴����ӵ�С�
 *
 * �ļ���	$Id: about.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
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
    ���캯��
    ��ʾ�汾��Ϣ
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
            ver_str.printf("�汾��%d.%d.%d.%d",major,minor,release,build);
        }
        Version->Caption = ver_str;
        delete[] block;
    }
}
//--------------------------------------------------------------------- 
