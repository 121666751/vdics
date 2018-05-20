/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <bcbext.h>
#include <zcommon.h>

#include "main.h"
#include "warrant.h"
#include "login.h"
#include "Password.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormWarrant *FormWarrant;
//---------------------------------------------------------------------------
__fastcall TFormWarrant::TFormWarrant(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormWarrant::FormShow(TObject *Sender)
{
    SetCtrlStyle(txtClkno,ES_NUMBER);
     txtClkno->Text = "";
     txtPassword->Text = "";
     txtClkno->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TFormWarrant::btnOKClick(TObject *Sender)
{
    if( strcmp(txtClkno->Text.c_str(), g_sys_opt.c_str())==0)
    {
        MessageBox(Handle,"��Ȩ��Ա�Ͳ�����Աһ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtClkno->Text = "";
        txtPassword->Text = "";
        txtClkno->SetFocus();
        ModalResult = mrNone;
        return;    
    }
    String cmd =  AnsiString("select state,authority,area from clerks where clkno='")+txtClkno->Text.c_str()+"' and password = PASSWORD('"+txtPassword->Text.c_str()+"')";

    int ret = FmIC->gdb.Query(cmd.c_str());
    if(ret)
    {
          MessageBox(Handle,"���ݿ��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
          ModalResult = mrNone;
          return ;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    if(row==NULL)
    {
        MessageBox(Handle,"��Ա�������!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtClkno->Text = "";
        txtPassword->Text = "";
        txtClkno->SetFocus();
        ModalResult = mrNone;
        return;
    }
    else
    {
        int state = atoi(row[0]);
        if(state!=1)
        {
            MessageBox(Handle,"��Ա״̬����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
            ModalResult = mrNone;
            return;
        }
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormWarrant::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{

    if(Key == 13)
        PostMessage(Handle,WM_NEXTDLGCTL,0,0);
}
//---------------------------------------------------------------------------


