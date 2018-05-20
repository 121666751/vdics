//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "undo.h"

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormUndo *FormUndo;
//---------------------------------------------------------------------------
__fastcall TFormUndo::TFormUndo(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormUndo::E1Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormUndo::Button2Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------


int TFormUndo::flushUndo()
{
    char date[12];
    char session[4];
    memset(date,0,sizeof(date));
    memset(session,0,sizeof(session));
    FmIC->getDS("date",date);
    FmIC->getDS("session",session);

    int ret = FmIC->gdb.VQuery(512,"select sid,exchno,fname from vouchers where mergestate=0 and date='%s' and session='%s'",date,session);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    lstVouchers->Clear();
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    TListItem* li;
    while(row!=NULL)
    {
        li = lstVouchers->Items->Add();
        li->Caption = row[0];
        li->SubItems->Insert(0,row[1]);
        li->SubItems->Insert(1,row[2]);
        row = res.Fetch();
    }

    StatusBar->Panels->Items[0]->Text = String("单票未处理总数[")+res.Count()+"]";

    ret = FmIC->gdb.VQuery(512,"select sid,exchno,fname from multis where substate=0 and date='%s' and session='%s'",date,session);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    lstEnv->Clear();
    res = FmIC->gdb.Store();
    row = res.Fetch();

    while(row!=NULL)
    {
        li = lstEnv->Items->Add();
        li->Caption = row[0];
        li->SubItems->Insert(0,row[1]);
        li->SubItems->Insert(1,row[2]);
        row = res.Fetch();
    }

    StatusBar->Panels->Items[1]->Text = String("信封未处理总数[")+res.Count()+"]";

    ret = FmIC->gdb.VQuery(512,"select sid,exchno,fname from authinput where substate=0 and date='%s' and session='%s'",date,session);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    lstWs->Clear();
    res = FmIC->gdb.Store();
    row = res.Fetch();

    while(row!=NULL)
    {
        li = lstWs->Items->Add();
        li->Caption = row[0];
        li->SubItems->Insert(0,row[1]);
        li->SubItems->Insert(1,row[2]);
        row = res.Fetch();
    }

    StatusBar->Panels->Items[2]->Text = String("特色业务未处理总数[")+res.Count()+"]";
    return 0;
}
void __fastcall TFormUndo::btnRefreshClick(TObject *Sender)
{
    flushUndo();
}
//---------------------------------------------------------------------------

void __fastcall TFormUndo::R1Click(TObject *Sender)
{
    flushUndo();
}
//---------------------------------------------------------------------------

void __fastcall TFormUndo::FormShow(TObject *Sender)
{
    flushUndo();    
}
//---------------------------------------------------------------------------

