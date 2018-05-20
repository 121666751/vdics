//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <bcbext.h>

#include "vips.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmVips *FmVips;
//---------------------------------------------------------------------------
__fastcall TFmVips::TFmVips(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFmVips::numExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

int TFmVips::IsVipsExist(char *accno)
{
    int ret = 0;

    //�������ݿ���Vips���еļ�¼
    ret = FmDistMan->gdb.VQuery(512, "select count(*) from vips where accno='%s'", accno);

    MyRes res = FmDistMan->gdb.Store();
    MyRow row = res.Fetch();

    if(row==NULL)
        return 0;
    else
    {
        int count = atoi(row[0]);
        
        if(count > 0)
                return 1;
        else
                return 0;
    }
    
    return ret;
}

int TFmVips::BindVips()
{
    int ret;

    //�������ݿ���Vips���еļ�¼
    ret = FmDistMan->gdb.Query("select sid, accno, accname, exchno from vips");

    lstVips->Clear();
    TListItem* li;
    
    MyRes res = FmDistMan->gdb.Store();
    MyRow row = res.Fetch();
    
    while(row!=NULL)
    {
        li = lstVips->Items->Add();
        li->Caption = row[0];

        li->SubItems->Insert(0,row[1]);
        li->SubItems->Insert(1,row[2]);
        li->SubItems->Insert(2,row[3]);
        
        row = res.Fetch();
    }

    return ret;
}

int TFmVips::showVchtype()
{
    //TODO: Add your source code here
    int ret = BindVips();
    if(ret)
    {
          MessageBox(Handle,"���ݿ��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
          return ret;
    }
    return 0;
}
void __fastcall TFmVips::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFmVips::FormShow(TObject *Sender)
{
    showVchtype();   
}
//---------------------------------------------------------------------------
void __fastcall TFmVips::btnAddClick(TObject *Sender)
{
    int ret;
    
    if(txtAccno->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"�˺Ų���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtAccno->SetFocus();
        return;
    }
    if(txtAccname->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"�ͻ����Ʋ���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtAccname->SetFocus();
        return;
    }

    if(txtExchno->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"�����Ų���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtExchno->SetFocus();
        return;
    }

    if(IsVipsExist( txtAccno->Text.c_str()))
    {
        String b = AnsiString("�˺�[") + txtAccno->Text.c_str() +"]�Ѵ���!";
        MessageBox(Handle,b.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    ret = FmDistMan->gdb.VQuery( 512, "insert into vips(accno, accname, exchno) values( '%s', '%s', '%s')", txtAccno->Text.c_str(), txtAccname->Text.c_str(),  txtExchno->Text.c_str());
    if(ret)
    {
          MessageBox(Handle,"���ݲ�ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
          return ;
    }
    
    ret = BindVips();
    if(ret)
    {
          MessageBox(Handle,"���ݰ󶨴���!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFmVips::MeForceLookClick(TObject *Sender)
{
    TListItem* li;
    li = lstVips->Selected ;
    if(li == NULL ) return ;
    String sid = li->Caption;

    String b = AnsiString("�Ƿ�Ҫɾ����ˮ��[") +sid+"���˺�?";
    if(MessageBox(Handle,b.c_str(),"��ʾ",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

    int ret = FmDistMan->gdb.VQuery(512,"delete from vips where sid='%s'", sid.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }

    MessageBox(Handle,"ɾ���ɹ�\t","��ʾ",MB_OK|MB_ICONINFORMATION);

    ret = BindVips();
    if(ret)
    {
        MessageBox(Handle,"�󶨲�ѯ����\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }   
}
//---------------------------------------------------------------------------


