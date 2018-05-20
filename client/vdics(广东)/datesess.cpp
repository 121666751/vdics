//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <ztime.hpp>
#include "datesess.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDate *FormDate;
//---------------------------------------------------------------------------
__fastcall TFormDate::TFormDate(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormDate::FormShow(TObject *Sender)
{
    FmIC->getDS("date",date);
    FmIC->getDS("session",session);

    txtSession->Text=session;
    ZDateTime zdate;
    zdate.SetBankDateStr(date);
    txtDate->Date = StrToDate(zdate.DateString().c_str());    
}
//---------------------------------------------------------------------------
void __fastcall TFormDate::BtnOkClick(TObject *Sender)
{
    String dd = txtDate->Date.FormatString("yyyymmdd");
    int ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('session',%s)",txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('date','%s')",dd.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }    
}
//---------------------------------------------------------------------------
