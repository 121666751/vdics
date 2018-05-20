//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <bcbext.h>

#include "vchtype.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmVchtype *FmVchtype;
//---------------------------------------------------------------------------
__fastcall TFmVchtype::TFmVchtype(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFmVchtype::numExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------


int TFmVchtype::showVchtype()
{
    //TODO: Add your source code here
    int ret = FmIC->gdb.Query("select area, vchtype,banktype,model,expcn,cdcode from vchtypes");
    if(ret)
    {
          MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
          return ret;
    }
    lstVchtype->Clear();
    TListItem* li;

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    int priv;
    while(row!=NULL)
    {
        li = lstVchtype->Items->Add();
        li->Caption = row[0];
        //修改行展示错误 -- FPF
        li->SubItems->Insert(0,row[1]);
        li->SubItems->Insert(1,row[2]);
        li->SubItems->Insert(2,row[3]);

        li->SubItems->Insert(3,row[4]);
        if(atoi(row[5])==0)
            li->SubItems->Insert(4,"借方");
        else
            li->SubItems->Insert(4,"贷方");


        row = res.Fetch();
    }

    txtVchtype->SetFocus();
    return 0;
}
void __fastcall TFmVchtype::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFmVchtype::FormShow(TObject *Sender)
{
    SetCtrlStyle(txtVchtype,ES_NUMBER);
    SetCtrlStyle(txtModel,ES_NUMBER);
    showVchtype();   
}
//---------------------------------------------------------------------------
void __fastcall TFmVchtype::btnAddClick(TObject *Sender)
{
    if(txtArea->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"交换区域不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtArea->SetFocus();
        return;
    }
    if(txtVchtype->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"凭证类型不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtVchtype->SetFocus();
        return;
    }

    if(txtType->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"凭证类型不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtType->SetFocus();
        return;
    }

    if(txtModel->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"版面类型不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtModel->SetFocus();
        return;
    }

    if(txtName->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"凭证名称不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtName->SetFocus();
        return;
    }

    int ret = FmIC->gdb.VQuery(512,"select count(*) from vchtypes where vchtype='%s' and area='%s'",txtVchtype->Text.c_str(),txtArea->Text.c_str());
    if(ret)
    {
          MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
          return ;
    }


    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    if(atoi(row[0])!=0)
    {
        String s = AnsiString("交换区域[")+txtArea->Text.c_str()+"]凭证类型["+txtVchtype->Text.c_str()+"]已存在!\t";
        MessageBox(Handle,s.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    ret = FmIC->gdb.VQuery(512,"insert into vchtypes( vchtype,banktype,model,expcn,cdcode,area) values ('%s','%s',%d,'%s',%d,'%s')",txtVchtype->Text.c_str(),txtType->Text.c_str(),txtModel->Text.ToInt(),txtName->Text.c_str(),cbVchtype->ItemIndex,txtArea->Text.c_str());
    if(ret)
    {
          MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
          return ;
    }

    MessageBox(Handle,"增加成功!\t","提示",MB_OK|MB_ICONINFORMATION);
    txtArea->Clear();
    txtVchtype->Clear();
    txtType->Clear();
    txtModel->Clear();
    txtName->Clear();
    showVchtype(); 
}
//---------------------------------------------------------------------------
void __fastcall TFmVchtype::MeForceLookClick(TObject *Sender)
{
    TListItem* li;
    li = lstVchtype->Selected ;
    if(li == NULL ) return ;
    String area = li->Caption;
    String vchtype = li->SubItems->Strings[0];

    String b = AnsiString("是否要删除交换区域[") +area+"的凭证类型["+vchtype+"]?";
    if(MessageBox(Handle,b.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

    int ret = FmIC->gdb.VQuery(512,"delete from vchtypes where vchtype='%s' and area='%s'",vchtype.c_str(),area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }

    MessageBox(Handle,"删除成功\t","提示",MB_OK|MB_ICONINFORMATION);
    showVchtype();        
}
//---------------------------------------------------------------------------
void __fastcall TFmVchtype::btnModifClick(TObject *Sender)
{
     //判断是否为空
        if(txtArea->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"交换区域不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
                txtArea->SetFocus();
                return;
         }
        if(txtVchtype->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"凭证类型不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
                txtVchtype->SetFocus();
                return;
        }

        if(txtType->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"凭证类型不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
                txtType->SetFocus();
                return;
        }

        if(txtModel->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"版面类型不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
                txtModel->SetFocus();
                return;
        }
        if(txtName->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"凭证名称不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
                txtName->SetFocus();
                return;
        }

        int ret;
        MyRes res;
        MyRow row;

        TListItem *p = lstVchtype->Selected;     //选取选中项，并将从数据库删除
        if(p == NULL)
        {
                MessageBox(Handle,"没有选中修改项!\t","提示",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        int ct;     //借记标志
        //删除一条记录
        if(p->SubItems->Strings[4] == "借方")
                ct = 0;
        else
                ct = 1;
        ret = FmIC->gdb.VQuery(512,"delete from vchtypes where area='%s' and vchtype='%s' and banktype='%s' and model=%d and expcn='%s' and cdcode=%d",p->Caption.c_str(),p->SubItems->Strings[0].c_str(),p->SubItems->Strings[1].c_str(),p->SubItems->Strings[2].ToInt(),p->SubItems->Strings[3].c_str(),ct);
        if(ret)
        {
                MessageBox(Handle,"数据库查询错误!0011\t","提示",MB_OK|MB_ICONINFORMATION);
                return ;
        }

        //判断交换区域和打码类型数据库是否已包含
        ret = FmIC->gdb.VQuery(512,"select count(*) from vchtypes where area='%s' and vchtype='%s'",txtArea->Text.Trim().c_str(),txtVchtype->Text.Trim().c_str());
        if(ret)
        {
                MessageBox(Handle,"数据库查询错误!0012\t","提示",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        res = FmIC->gdb.Store();
        row = res.Fetch();
        if(atoi(row[0]))
        {
                AnsiString str;
                str.sprintf("交换区域[%s]和打码类型[%s]重复!0013\t",txtArea->Text.Trim().c_str(),txtVchtype->Text.Trim().c_str());
                MessageBox(Handle,str.c_str(),"提示",MB_OK|MB_ICONINFORMATION);

                ret = FmIC->gdb.VQuery(512,"insert into  vchtypes (area,vchtype,banktype,model,expcn,cdcode) values('%s','%s','%s',%d,'%s',%d)",p->Caption.c_str(),p->SubItems->Strings[0].c_str(),p->SubItems->Strings[1].c_str(),p->SubItems->Strings[2].ToInt(),p->SubItems->Strings[3].c_str(),ct);
                if(ret)
                {
                        MessageBox(Handle,"数据库查询错误!0015\t","提示",MB_OK|MB_ICONINFORMATION);
                        return ;
                }
                showVchtype();         //刷新TListView列表
                txtArea->Text = "";
                txtVchtype->Text = "";
                txtType->Text = "";
                txtModel->Text = "";
                txtName->Text = "";
                return ;
        }
        

        //增加一条记录，将新数据添加到数据库，实现更新
        ret = FmIC->gdb.VQuery(512,"insert into  vchtypes (vchtype,banktype,model,expcn,cdcode,area) values('%s','%s',%d,'%s',%d,'%s')",txtVchtype->Text.Trim().c_str(),txtType->Text.Trim().c_str(),txtModel->Text.ToInt(),txtName->Text.Trim().c_str(),cbVchtype->ItemIndex,txtArea->Text.Trim().c_str());
        if(ret)
        {
                MessageBox(Handle,"数据库查询错误!0010\t","提示",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        MessageBox(Handle,"修改成功!\t","提示",MB_OK|MB_ICONINFORMATION);
        showVchtype();         //刷新TListView列表
        txtArea->Text = "";
        txtVchtype->Text = "";
        txtType->Text = "";
        txtModel->Text = "";
        txtName->Text = "";    
}
//---------------------------------------------------------------------------



void __fastcall TFmVchtype::lstVchtypeClick(TObject *Sender)
{
	if(lstVchtype->Selected == NULL)
		return ;
	TListItem * p = lstVchtype->Selected;    //选中项
	txtArea->Text = p->Caption;              //交换区域
	txtVchtype->Text = p->SubItems->Strings[0]; //打码类型
	txtType->Text = p->SubItems->Strings[1]; //流程类型
	txtModel->Text = p->SubItems->Strings[2];//版面类型
	txtName->Text = p->SubItems->Strings[3]; //凭证名称
	if(p->SubItems->Strings[4] == "借方")
			cbVchtype->ItemIndex = 0;
	else
			cbVchtype->ItemIndex = 1;
}
//---------------------------------------------------------------------------

