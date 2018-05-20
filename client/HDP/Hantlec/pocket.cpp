//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <bcbext.h>

#include "pocket.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmPocket *FmPocket;
//---------------------------------------------------------------------------
__fastcall TFmPocket::TFmPocket(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFmPocket::numExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

int TFmPocket::IsPockExist(char *distno, char *vchtype)
{
    int ret = 0;

    //加载数据库中pocket表中的记录
    ret = FmDistMan->gdb.VQuery(512, "select count(*) from pocket where distno='%s' and vchtype='%s'", distno, vchtype);

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

int TFmPocket::BindPocket()
{
    int ret;

    //加载数据库中pocket表中的记录
    ret = FmDistMan->gdb.Query("select sid, vchtype, pockno, pocdesc, distno from pocket");

    lstPocket->Clear();
    TListItem* li;
    
    MyRes res = FmDistMan->gdb.Store();
    MyRow row = res.Fetch();
    
    while(row!=NULL)
    {
        li = lstPocket->Items->Add();
        li->Caption = row[0];

        li->SubItems->Insert(0,row[1]);
        li->SubItems->Insert(1,row[2]);
        li->SubItems->Insert(2,row[3]);
        li->SubItems->Insert(3,row[4]);
        
        row = res.Fetch();
    }

    //绑定vchtypes集合
    ret = FmDistMan->gdb.Query("select distinct vchtype from vchtypes");
    if(!ret)
    {
        txtVchtype->Items->Clear();

        res = FmDistMan->gdb.Store();
        row = res.Fetch();
    
        while(row!=NULL)
        {
                txtVchtype->Items->Add(row[0]);
        
                row = res.Fetch();
        }

        txtVchtype->Items->Add("VP");

        txtVchtype->SetFocus();
    }

    txtVchtype->ItemIndex = 0;
    return ret;
}

int TFmPocket::showVchtype()
{
    //TODO: Add your source code here
    int ret = BindPocket();
    if(ret)
    {
          MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
          return ret;
    }
    return 0;
}
void __fastcall TFmPocket::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFmPocket::FormShow(TObject *Sender)
{
    showVchtype();   
}
//---------------------------------------------------------------------------
void __fastcall TFmPocket::btnAddClick(TObject *Sender)
{
    int ret;
    
    if(txtVchtype->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"凭证类型不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtVchtype->SetFocus();
        return;
    }
    if(txtPockno->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"口袋编号不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtPockno->SetFocus();
        return;
    }

    if(txtDistno->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"清分流程不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtDistno->SetFocus();
        return;
    }

    if(txtPockdesc->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"口袋描述不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtPockdesc->SetFocus();
        return;
    }

    if(IsPockExist( txtDistno->Text.c_str(), txtVchtype->Text.c_str()))
    {
        String b = AnsiString("凭证类型[") + txtVchtype->Text.c_str() +"]清分流程["+ txtDistno->Text.c_str() +"]已存在!";
        MessageBox(Handle,b.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    
    ret = FmDistMan->gdb.VQuery( 512, "insert into pocket(vchtype, pockno, pocdesc, distno) values( '%s', %d, '%s', '%s')", txtVchtype->Text.c_str(), atoi(txtPockno->Text.c_str()),  txtPockdesc->Text.c_str(), txtDistno->Text.c_str());
    if(ret)
    {
          MessageBox(Handle,"数据查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
          return ;
    }
    
    ret = BindPocket();
    if(ret)
    {
          MessageBox(Handle,"数据绑定错误!\t","提示",MB_OK|MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFmPocket::MeForceLookClick(TObject *Sender)
{
    TListItem* li;
    li = lstPocket->Selected ;
    if(li == NULL ) return ;
    String sid = li->Caption;

    String b = AnsiString("是否要删除编号[") +sid+"]的清分口袋?";
    if(MessageBox(Handle,b.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

    int ret = FmDistMan->gdb.VQuery(512,"delete from pocket where sid='%s'", sid.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }

    MessageBox(Handle,"删除成功\t","提示",MB_OK|MB_ICONINFORMATION);

    ret = BindPocket();
    if(ret)
    {
        MessageBox(Handle,"绑定查询错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }   
}
//---------------------------------------------------------------------------


