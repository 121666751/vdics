//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "state.h"
#include "main.h"
#include <myplus.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormST *FormST;
//---------------------------------------------------------------------------
__fastcall TFormST::TFormST(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

int TFormST::flushState()
{
    //TODO: Add your source code here
    int ret;

    String sql = "select exchno,dramount,cramount,substring(state,2,1),substring(state,3,1),substring(state,4,1),substring(state,5,1),substring(state,7,1) from bocctrl ";
    String cond="";
    if(cmbPhase->ItemIndex == 0 )
    {
        cond = " where 1 = 1 ";      
    }
    
    else if(cmbPhase->ItemIndex == 1 )
    {
        if( cmbState->ItemIndex == 1)
            cond = " where substring(state,2,1)='1'";
        else if( cmbState->ItemIndex == 2)
            cond = " where substring(state,2,1)!='1'";
    }

    else if(cmbPhase->ItemIndex == 2 )
    {
        if( cmbState->ItemIndex == 1)
            cond = " where substring(state,3,1)='1'";
        else if( cmbState->ItemIndex == 2)
            cond = " where substring(state,3,1)!='1'";
    }

    else if(cmbPhase->ItemIndex == 3 )
    {
        if( cmbState->ItemIndex == 1)
            cond = " where substring(state,4,1)='1'";
        else if( cmbState->ItemIndex == 2)
            cond = " where substring(state,4,1)!='1'";
    }

    else if(cmbPhase->ItemIndex == 4 )
    {
        if( cmbState->ItemIndex == 1)
            cond = " where substring(state,5,1)='1'";
        else if( cmbState->ItemIndex == 2)
            cond = " where substring(state,5,1)!='1'";
    }

    else if(cmbPhase->ItemIndex == 5 )
    {
        if( cmbState->ItemIndex == 1)
            cond = " where substring(state,7,1)='1'";
        else if( cmbState->ItemIndex == 2)
            cond = " where substring(state,7,1)!='1'";
    }

    sql = sql + cond;

    ret = FmDistMan->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    txtDetail->Clear();
    MyRes res = FmDistMan->gdb.Store();
    MyRow row = res.Fetch();

    TListItem* li;
    while(row!=NULL)
    {
       li = txtDetail->Items->Add();
       li->Caption = row[0];
       li->SubItems->Insert(0,row[1]);
       li->SubItems->Insert(1,row[2]);
       if(atoi(row[3]) == 1 )
            li->SubItems->Insert(2,"已清分");
       else
            li->SubItems->Insert(2,"未清分");
       if(atoi(row[4]) == 1 )
            li->SubItems->Insert(3,"已扫描");
       else
            li->SubItems->Insert(3,"未扫描");

       if(atoi(row[5]) == 1 )
            li->SubItems->Insert(4,"已扫描");
       else
            li->SubItems->Insert(4,"未扫描");

       if(atoi(row[6]) == 1 )
            li->SubItems->Insert(5,"已扫描");
       else
            li->SubItems->Insert(5,"未扫描");

       if(atoi(row[7]) == 1 )
            li->SubItems->Insert(6,"已提交");
       else
            li->SubItems->Insert(6,"未提交");

       row = res.Fetch();
    }

    sql = "select count(*) from bocctrl ";

    ret = FmDistMan->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    
    res = FmDistMan->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[0]->Text = String("网点总数[")+row[0]+"]";

    sql = "select count(*) from bocctrl where substring(state,2,1)='1'";

    ret = FmDistMan->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    
    res = FmDistMan->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[1]->Text = String("已清分网点总数[")+row[0]+"]";

    sql = "select count(*) from bocctrl where substring(state,3,1)='1'";
    ret = FmDistMan->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    
    res = FmDistMan->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[2]->Text = String("单票已扫描网点总数[")+row[0]+"]";

    sql = "select count(*) from bocctrl where substring(state,4,1)='1'";
    ret = FmDistMan->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    res = FmDistMan->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[3]->Text = String("信封已扫描网点总数[")+row[0]+"]";

    sql = "select count(*) from bocctrl where substring(state,5,1)='1'";
    ret = FmDistMan->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    res = FmDistMan->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[4]->Text = String("委收已扫描网点总数[")+row[0]+"]";


    sql = "select count(*) from bocctrl where substring(state,7,1)='1'";
    ret = FmDistMan->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    res = FmDistMan->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[5]->Text = String("信封已提交网点总数[")+row[0]+"]";
    return 0;
}
void __fastcall TFormST::btnRefreshClick(TObject *Sender)
{
    flushState();    
}
//---------------------------------------------------------------------------
void __fastcall TFormST::Button2Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormST::FormShow(TObject *Sender)
{
    flushState();
}
//---------------------------------------------------------------------------
void __fastcall TFormST::cmbPhaseChange(TObject *Sender)
{
    flushState();
}
//---------------------------------------------------------------------------
