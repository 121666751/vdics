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
#include <zcommon.h>
#include "clerks.h"
#include "clkinfo.h"

#include "main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmClerks *FmClerks;
//---------------------------------------------------------------------------
__fastcall TFmClerks::TFmClerks(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFmClerks::btnAddClick(TObject *Sender)
{   
    FmCkInfo->setMethod(1,"");
    FmCkInfo->ShowModal();
    showClerks();
}
//---------------------------------------------------------------------------
void __fastcall TFmClerks::Button4Click(TObject *Sender)
{
    Close();        
}
//---------------------------------------------------------------------------


void __fastcall TFmClerks::btnModClick(TObject *Sender)
{
    TListItem* li;
    li= txtClerkList->Selected;
    if(li==NULL) return ;

    String  clkno = li->Caption;
    FmCkInfo->setMethod(2,clkno.c_str());
    FmCkInfo->ShowModal();
    showClerks();
}
//---------------------------------------------------------------------------



int TFmClerks::showClerks()
{
    //TODO: Add your source code here
    String cmd =  AnsiString("select clkno,name,state,authority,area from clerks");

    int ret = FmIC->gdb.Query(cmd.c_str());
    if(ret)
    {
          MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
          return ret;
    }

    txtClerkList->Clear();
    TListItem* li;

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();      

    int priv;
    while(row!=NULL)
    {
        li = txtClerkList->Items->Add();
        li->Caption = row[0];
        li->SubItems->Insert(0,row[1]);
        if( atoi(row[2]) == 1 )
            li->SubItems->Insert(1,"正常");
        else
            li->SubItems->Insert(1,"不正常");

        priv = atoi(row[3]);
        if(priv &  PRIV_GRD_NOM )
            li->SubItems->Insert(2,"业务柜员");
        else if(priv &  PRIV_GRD_ADM)
            li->SubItems->Insert(2,"高级柜员");
        else if(priv &  PRIV_GRD_MID)
            li->SubItems->Insert(2,"中级柜员");
        else if(priv &  PRIV_GRD_CUS)
            li->SubItems->Insert(2,"自定义柜员");
        else
            li->SubItems->Insert(2,"未知");
            
        li->SubItems->Insert(3,row[4]);

        row = res.Fetch();
    }
      
    return 0;
}
void __fastcall TFmClerks::FormShow(TObject *Sender)
{
    showClerks();
}
//---------------------------------------------------------------------------

void __fastcall TFmClerks::btnDelClick(TObject *Sender)
{
    TListItem* li;
    li= txtClerkList->Selected;
    if(li==NULL) return ;

    String  clkno = li->Caption;

    String  show = "确实要删除柜员["+ clkno + "]?\t";

    if(MessageBox(Handle,show.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION) ==IDNO)
        return ;

    String cmd =  AnsiString("delete from clerks where clkno='")+clkno+"'";


    int ret = FmIC->gdb.Query(cmd.c_str());
    if(ret)
    {
          MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION); 
          return ;
    }
    showClerks();
}
//---------------------------------------------------------------------------

