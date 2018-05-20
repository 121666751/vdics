/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <bcbext.h>
#include <zcommon.h>

#include "main.h"
#include "login.h"
#include "Password.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

String  g_sys_opt;

String  g_sys_area;

int     g_sys_priv;

String  g_sys_name;

TFormLogin *FormLogin;
//---------------------------------------------------------------------------
__fastcall TFormLogin::TFormLogin(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLogin::FormShow(TObject *Sender)
{
    SetCtrlStyle(txtClkno,ES_NUMBER);    
}
//---------------------------------------------------------------------------

void __fastcall TFormLogin::btnOKClick(TObject *Sender)
{
    String cmd =  AnsiString("select state,authority,area,name from clerks where clkno='")+txtClkno->Text.c_str()+"' and password = PASSWORD('"+txtPassword->Text.c_str()+"')";

    int ret = FmDistMan->gdb.Query(cmd.c_str());
    if(ret)
    {
          MessageBox(Handle,"数据库查询错误!","提示",MB_OK|MB_ICONINFORMATION);
          ModalResult = mrNone;
          return ;
    }

    MyRes res = FmDistMan->gdb.Store();
    MyRow row = res.Fetch();

    if(row==NULL)
    {
        MessageBox(Handle,"柜员密码错误!","提示",MB_OK|MB_ICONINFORMATION);
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
            MessageBox(Handle,"柜员状态不对!","提示",MB_OK|MB_ICONINFORMATION);
            ModalResult = mrNone;
            return;
        }
    }

    g_sys_opt   = txtClkno->Text.c_str();
    g_sys_priv  = atoi(row[1]);
    g_sys_area  = row[2];

    g_sys_name  = row[3];

    if( g_sys_priv&PRIV_MOD_PWD)
    {
        if( FormPWD->ShowModal()!=IDOK)
        {
            Application->Terminate();
        }
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormLogin::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{

    if(Key == 13)
        PostMessage(Handle,WM_NEXTDLGCTL,0,0);
}
//---------------------------------------------------------------------------


