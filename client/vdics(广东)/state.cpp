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

    String sql = "select exchno,dramount,cramount,substring(state,2,1),substring(state,3,1),substring(state,4,1),substring(state,5,1),substring(state,7,1) from bocctrl  ";
    String cond=  AnsiString(" where date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";
    if(cmbPhase->ItemIndex == 0 )
    {
        cond = cond + " and 1 = 1 ";
    }
    
    else if(cmbPhase->ItemIndex == 1 )
    {
        if( cmbState->ItemIndex == 1)
            cond = cond + " and substring(state,2,1)='1'";
        else if( cmbState->ItemIndex == 2)
            cond = cond +  " and substring(state,2,1)!='1'";
    }

    else if(cmbPhase->ItemIndex == 2 )
    {
        if( cmbState->ItemIndex == 1)
            cond = cond + " and substring(state,3,1)='1'";
        else if( cmbState->ItemIndex == 2)
            cond = cond + " and substring(state,3,1)!='1'";
    }

    else if(cmbPhase->ItemIndex == 3 )
    {
        if( cmbState->ItemIndex == 1)
            cond = cond + " and substring(state,4,1)='1'";
        else if( cmbState->ItemIndex == 2)
            cond = cond + " and substring(state,4,1)!='1'";
    }

    else if(cmbPhase->ItemIndex == 4 )
    {
        if( cmbState->ItemIndex == 1)
            cond = cond + " and substring(state,5,1)='1'";
        else if( cmbState->ItemIndex == 2)
            cond = cond + " and substring(state,5,1)!='1'";
    }

    else if(cmbPhase->ItemIndex == 5 )
    {
        if( cmbState->ItemIndex == 1)
            cond = cond + " and substring(state,7,1)='1'";
        else if( cmbState->ItemIndex == 2)
            cond = cond + " and substring(state,7,1)!='1'";
    }

    sql = sql + cond;

    ret = FmIC->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    txtDetail->Clear();
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    TListItem* li;
    while(row!=NULL)
    {
       li = txtDetail->Items->Add();
       li->Caption = row[0];
       li->SubItems->Insert(0,row[1]);
       li->SubItems->Insert(1,row[2]);
       if(atoi(row[3]) == 1 )
            li->SubItems->Insert(2,"�����");
       else
            li->SubItems->Insert(2,"δ���");
       if(atoi(row[4]) == 1 )
            li->SubItems->Insert(3,"��ɨ��");
       else
            li->SubItems->Insert(3,"δɨ��");

       if(atoi(row[5]) == 1 )
            li->SubItems->Insert(4,"��ɨ��");
       else
            li->SubItems->Insert(4,"δɨ��");

       if(atoi(row[6]) == 1 )
            li->SubItems->Insert(5,"��ɨ��");
       else
            li->SubItems->Insert(5,"δɨ��");

       if(atoi(row[7]) == 1 )
            li->SubItems->Insert(6,"���ύ");
       else
            li->SubItems->Insert(6,"δ�ύ");

       row = res.Fetch();
    }

    sql = "select count(*) from bocctrl ";

    cond =  AnsiString(" where date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";
    sql = sql + cond;

    ret = FmIC->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    
    res = FmIC->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[0]->Text = String("��������[")+row[0]+"]";

    cond =  AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";
    sql = "select count(*) from bocctrl where substring(state,2,1)='1' ";

    sql = sql + cond;

    ret = FmIC->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    
    res = FmIC->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[1]->Text = String("�������������[")+row[0]+"]";

    sql = "select count(*) from bocctrl where substring(state,3,1)='1' ";

    sql = sql + cond;

    ret = FmIC->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    
    res = FmIC->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[2]->Text = String("��Ʊ��ɨ����������[")+row[0]+"]";

    sql = "select count(*) from bocctrl where substring(state,4,1)='1'";
    sql = sql + cond;
    
    ret = FmIC->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[3]->Text = String("�ŷ���ɨ����������[")+row[0]+"]";

    sql = "select count(*) from bocctrl where substring(state,5,1)='1'";

    sql = sql + cond;
    
    ret = FmIC->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[4]->Text = String("ί����ɨ����������[")+row[0]+"]";


    sql = "select count(*) from bocctrl where substring(state,7,1)='1'";
    ret = FmIC->gdb.Query(sql.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[5]->Text = String("�ŷ����ύ��������[")+row[0]+"]";
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
    char date[12];
    char session[4];
    memset(date,0,sizeof(date));
    memset(session,0,sizeof(session));
    FmIC->getDS("date",date);
    FmIC->getDS("session",session);
    txtDate->Text = date;
    txtSession->Text = session;
    flushState();
}
//---------------------------------------------------------------------------
void __fastcall TFormST::cmbPhaseChange(TObject *Sender)
{
    flushState();
}
//---------------------------------------------------------------------------
void __fastcall TFormST::E1Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TFormST::R1Click(TObject *Sender)
{
     flushState();    
}
//---------------------------------------------------------------------------

