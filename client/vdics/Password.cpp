//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <zcommon.h>
#include "Password.h"
#include "login.h"
#include "main.h"
#include <bcbext.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPWD *FormPWD;
//---------------------------------------------------------------------------
__fastcall TFormPWD::TFormPWD(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormPWD::btnOKClick(TObject *Sender)
{
    if(txtPwdOne->Text!= txtPwdTwo->Text)
    {
        MessageBox(Handle,"新密码不一致!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtPwdOld->Text = "";
        txtPwdOne->Text = "";
        txtPwdTwo->Text = "";
        txtPwdOld->SetFocus();
        ModalResult = mrNone;
        return;
    }

    if(txtPwdOld->Text== txtPwdTwo->Text)
    {
        MessageBox(Handle,"新旧密码一致!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtPwdOld->Text = "";
        txtPwdOne->Text = "";
        txtPwdTwo->Text = "";
        txtPwdOld->SetFocus();
        ModalResult = mrNone;
        return;
    }

    if(txtPwdTwo->Text.Length() == 0 )
    {
        MessageBox(Handle,"新密码不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtPwdOld->Text = "";
        txtPwdOne->Text = "";
        txtPwdTwo->Text = "";
        txtPwdOld->SetFocus();
        ModalResult = mrNone;
        return;
    }

    String pswd = "";
    String cmd =  AnsiString("select count(*) from clerks where clkno='")+g_sys_opt.c_str()+"' and password = PASSWORD('"+txtPwdOld->Text.c_str()+"')";

    int ret = FmIC->gdb.Query(cmd.c_str());
    if(ret)
    {
        MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtPwdOld->Text = "";
        txtPwdOne->Text = "";
        txtPwdTwo->Text = "";
        txtPwdOld->SetFocus();
        ModalResult = mrNone;
        return ;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    if( atoi(row[0])==0)
    {
        MessageBox(Handle,"柜员密码错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtPwdOld->Text = "";
        txtPwdOne->Text = "";
        txtPwdTwo->Text = "";
        txtPwdOld->SetFocus();
        ModalResult = mrNone;
        return;
    }

    if(g_sys_priv &  PRIV_MOD_PWD)
    {
        cmd =  AnsiString("update clerks set authority = authority-1 ,password=PASSWORD('")+txtPwdOne->Text.c_str()+"') where clkno='"+g_sys_opt.c_str()+"'" ;
    }
    else
        cmd =  AnsiString("update clerks set password=PASSWORD('")+txtPwdOne->Text.c_str()+"') where clkno='"+g_sys_opt.c_str()+"'" ;
    ret = FmIC->gdb.Query(cmd.c_str());
    if(ret)
    {
        MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtPwdOld->Text = "";
        txtPwdOne->Text = "";
        txtPwdTwo->Text = "";
        txtPwdOld->SetFocus();
        ModalResult = mrNone;
        return ;
    }

    MessageBox(Handle,"修改密码成功!\t","提示",MB_OK|MB_ICONINFORMATION);

}
//---------------------------------------------------------------------------
void __fastcall TFormPWD::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
     if(Key == 13)
         PostMessage(Handle,WM_NEXTDLGCTL,0,0);
}
//---------------------------------------------------------------------------

