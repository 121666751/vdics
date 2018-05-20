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
 * �ļ���	$Id: rescan.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com
 * ���䣺   support@tchzt.com
 *
 ***************************************************************************
 */
//----------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "rescan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRescanForm *RescanForm;
//---------------------------------------------------------------------------
__fastcall TRescanForm::TRescanForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
/**
    ������ʾ�¼�����
*/
void __fastcall TRescanForm::FormShow(TObject *Sender)
{
    //�����ݿ�
    int ret = db.Open("local_db_host","cidsa","z412t620","vchcenter");
    if( ret )
    {
        MessageBox(Handle,"�����ݿ����","����",MB_OK|MB_ICONERROR);
        Application->Terminate();
        return ;
    }

    //��ѯ����ʾ�����
    GetNetnos();
    cbNetNo->ItemIndex = 0;
    cbNetNo->SetFocus();
}
//---------------------------------------------------------------------------
/**
    ��ѯ��ʾ����ź���
*/
int TRescanForm::GetNetnos()
{
    //TODO: Add your source code here
    cbNetNo->Items->Clear();

    //��ѯ�����
    int ret = db.Query("select distinct pkgno from at_exchst where substring(state,2,1)='1' and substring(state,3,1)='0'");
    if( ret )
    {
        MessageBox(Handle,"��ѯ����","����",MB_OK|MB_ICONERROR);
        Application->Terminate();
        return -1;
    }
    MyRes res = db.Store();
    int count = (int)res.Count();
    if( count <= 0 )
    {
        btnOk->Enabled = true;
        return 0;
    }
    btnOk->Enabled = true;

    //��ʾ�����
    MyRow row = res.Fetch();
    while( row!=NULL )
    {
        cbNetNo->Items->Add( row[0] );
        row=res.Fetch();
    }

    return 0;
}
//---------------------------------------------------------------------------

/**
    ��ѯ���н�����
*/
void __fastcall TRescanForm::cbNetNoDropDown(TObject *Sender)
{
    GetNetnos();
}
//---------------------------------------------------------------------------
void __fastcall TRescanForm::FormCreate(TObject *Sender)
{
    //��ʾ��һ����¼
    cbNetNo->ItemIndex = 0;
}
//---------------------------------------------------------------------------



