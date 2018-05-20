/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//-------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "histquery.h"
#include <zcommon.h>
#include <bcbext.h>
#include <myplus.h>
#include "main.h"
#include "ConfigFile.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
//THistQueryDlg *HistQueryDlg;
//---------------------------------------------------------------------
__fastcall THistQueryDlg::THistQueryDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall THistQueryDlg::btnQueryClick(TObject *Sender)
{
    Query();
    txtNetNo->SetFocus();
}
//---------------------------------------------------------------------------
void THistQueryDlg::AddResult(String vchno, String netno, String accno, String tc, String amount)
{
    TListItem* li = VchList->Items->Add();
    li->Caption = vchno;
    li->ImageIndex = 0;

    li->SubItems->Add(netno);
    li->SubItems->Add(accno);
    li->SubItems->Add(tc);
    li->SubItems->Add(amount);
}
//---------------------------------------------------------------------------
/**
    ��ѯ���ݺ���
*/
void THistQueryDlg::Query()
{
    VchList->Items->Clear();
    int ret;

    String condition;

    //һ����ֲ�ѯ pbcdata��
    if(FmDistMan->_nDstMode == MD_FIRST )
    {
        if( !txtNetNo->Text.IsEmpty() )  condition = " and exchno='"+txtNetNo->Text+"' ";
        if( !txtAccNo->Text.IsEmpty() )  condition += " and accno like '%"+txtAccNo->Text+"' ";
        if( !txtAmount->Text.IsEmpty() ) condition += " and amount='"+txtAmount->Text +"'";

        if( condition.Length() <4 )
        {
            MessageBox(Handle,"����û�в鷶Χ","��ʾ",MB_OK|MB_ICONINFORMATION);
            return;
        }

        ret = FmDistMan->gdb.VQuery(512, "select vchno,exchno,accno,vchtype,amount \
                from pbcdata where 1=1 %s limit 30",condition.c_str() );
    }

    if(ret)
    {
        MessageBox(Handle,"��ѯ����","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmDistMan->gdb.Handle()));
        return;
    }

    MyRes res = FmDistMan->gdb.Store();
    MyRow row;
    row = res.Fetch();
    while( row!=NULL )
    {
        AddResult(row[0],row[1],row[2],row[3],row[4]);
        row=res.Fetch();
    }
}
//---------------------------------------------------------------------------
/**
    �������庯��
*/
void __fastcall THistQueryDlg::FormCreate(TObject *Sender)
{
    //���ÿؼ�ֻ����������
    SetCtrlStyle(txtNetNo, ES_NUMBER);
    SetCtrlStyle(txtAccNo,ES_NUMBER);
    SetCtrlStyle(txtAmount,ES_NUMBER);

    //�������ݳ���
    txtAccNo->MaxLength = FmDistMan->Distval.accLen;
    txtAmount->MaxLength = FmDistMan->Distval.amountLen;
    txtNetNo->MaxLength = FmDistMan->Distval.exnoLen;

    Left = 5;
    Top = 5;
}
//---------------------------------------------------------------------------
/**
    ��������¼�����
*/
void __fastcall THistQueryDlg::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    switch(Key)
    {
        case 'X':
        case 'x':
            ModalResult = mrCancel;
            break;
        case 13:
            PostMessage( Handle, WM_NEXTDLGCTL, 0, 0);
            break;
        case 33:
            PostMessage( Handle, WM_NEXTDLGCTL, 1, 0);
            break;
        case 'A':
        case 'a':
            LocateList();
            break;
    }
}
//---------------------------------------------------------------------------
void THistQueryDlg::LocateList()
{
    //TODO: Add your source code here
    if( VchList->Items->Count == 0 ) return;

    VchList->SetFocus();
    VchList->Items->Item[0]->Selected = true;
}
//---------------------------------------------------------------------------

