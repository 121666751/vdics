/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 �����촴������Ϣ�Ƽ����޹�˾    ��Ȩ����
 *
 * ���ļ�Ϊ�촴����������Ʒ��һ���֡�
 * �촴����������Ʒ�����в���,������������������Ʒ���������κ�ͼ����Ƭ��
 * ���������ֺ͸��ӳ���applets�������ܿ����渽��ӡˢ���ϡ�Դ�����ļ���
 * ��������Ʒ���κθ�����һ������Ȩ��֪ʶ��Ȩ�������Ȩ���촴����ӵ�С�
 *
 * �ļ���	$Id: wait.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com
 * ���䣺   support@tchzt.com
 *
 ***************************************************************************
 */
//--------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "wait.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmWait *FmWait;
//---------------------------------------------------------------------------
__fastcall TFmWait::TFmWait(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------