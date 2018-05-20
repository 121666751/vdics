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

#include "bocs.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormNets *FormNets;
//---------------------------------------------------------------------------
__fastcall TFormNets::TFormNets(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormNets::FormShow(TObject *Sender)
{
    SetCtrlStyle(txtNetno,ES_NUMBER);
    SetCtrlStyle(txtNet2,ES_NUMBER);
    SetCtrlStyle(txtArea,ES_NUMBER);
    flushData(); 
}
//---------------------------------------------------------------------------
void __fastcall TFormNets::BtnAddClick(TObject *Sender)
{
    if(txtNetno->Text.Length()<6 || txtNetno->Text.Trim().Length()>12)
    {
        MessageBox(Handle,"非法交换号!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    if(txtArea->Text.Length()<6)
    {
        MessageBox(Handle,"非法交换区域!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }


    int notice ;
    int merge;
    int check;
    String pkgno;


    if(chbCancel->Checked == true)
    {
        pkgno = txtNet2->Text.c_str();
        notice = 0;
    }
    else
    {
         pkgno = txtNetno->Text.c_str();
         notice = 1;
    }

    if(cbMerge->Checked == true)
    {
        merge = 1;
    }
    else
    {
        merge = 0;
    }

    if(cbCheck->Checked == true)
    {
        check = 1;
    }
    else
    {
        check = 0;
    }

    int ret = FmIC->gdb.VQuery(512,"select count(*) from bocnets where exchno='%s' and area='%s'",txtNetno->Text.c_str(),txtArea->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    if(atoi(row[0])>0)
    {
        MessageBox(Handle,"交换信息已存在!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }


    ret = FmIC->gdb.VQuery(512,"insert into bocnets(exchno,pkgno,area,notice,merge,compare) values('%s','%s','%s',%d,%d,%d) ",txtNetno->Text.c_str(),pkgno.c_str(),txtArea->Text.c_str(),notice,merge,check);

    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
    
    MessageBox(Handle,"增加成功!\t","提示",MB_OK|MB_ICONINFORMATION);
    flushData();    
}
//---------------------------------------------------------------------------


int TFormNets::flushData()
{
    //TODO: Add your source code here
    ListView->Clear();
    int ret = FmIC->gdb.Query("select exchno,pkgno,area,merge,compare from bocnets");
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    TListItem* li;
    while(row!=NULL)
    {
        li = ListView->Items->Add();
        li->Caption = row[0];
        li->SubItems->Insert(0,row[1]);
        li->SubItems->Insert(1,row[2]);
        if(atoi(row[3]) == 0 )
            li->SubItems->Insert(2,"不匹配");
        else
            li->SubItems->Insert(2,"匹配");

        if(atoi(row[4]) == 0 )
            li->SubItems->Insert(3,"不校验");
        else
            li->SubItems->Insert(3,"校验");
        row = res.Fetch();
    }
    return 0;
}
void __fastcall TFormNets::MeForceLookClick(TObject *Sender)
{
    TListItem* li;
    li = ListView->Selected ;
    if(li == NULL ) return ;
    String accno = li->Caption;

    String area = li->SubItems->Strings[1];

    String b = AnsiString("是否要删除交换号[")+accno+"]?\t";
    if(MessageBox(Handle,b.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

    int ret = FmIC->gdb.VQuery(512,"delete from bocnets where exchno='%s' and area='%s'",accno.c_str(),area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }

    MessageBox(Handle,"删除成功!\t","提示",MB_OK|MB_ICONINFORMATION);
    li->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TFormNets::Button3Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormNets::btnDeleteClick(TObject *Sender)
{
    flushData();
}
//---------------------------------------------------------------------------
void __fastcall TFormNets::ListViewClick(TObject *Sender)
{
    TListItem* li;
    li = ListView->Selected ;
    if(li == NULL ) return ;
    txtNetno->Text = li->Caption;
    txtNet2->Text = li->SubItems->Strings[0];
    if(txtNetno->Text == txtNet2->Text)     chbCancel->Checked = false;
    else   chbCancel->Checked = true;
    txtArea->Text = li->SubItems->Strings[1];

    cbMerge->Checked  =  (li->SubItems->Strings[2]=="匹配");
    cbCheck->Checked  =  (li->SubItems->Strings[3]=="校验");    
}
//---------------------------------------------------------------------------


void __fastcall TFormNets::N3Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TFormNets::chbCancelClick(TObject *Sender)
{
    if(chbCancel->Checked == true)
    {
        Label2->Visible = true;
        txtNet2->Visible = true;
    }
    else
    {
        txtNet2->Visible = false;
        Label2->Visible  = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormNets::btnModifClick(TObject *Sender)
{
     //判断是否为空
        if(txtNetno->Text.Trim().Length()<6 || txtNetno->Text.Trim().Length()>12)     //判断交换号是否为空
        {
                MessageBox(Handle,"非法的交换号!\t","提示",MB_OK|MB_ICONINFORMATION);
                txtNetno->SetFocus();
                return;
         }
        if(txtNet2->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"归属网点不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
                txtNet2->SetFocus();
                return;
        }
      
        if(txtArea->Text.Trim().Length()<6)
        {
                MessageBox(Handle,"非法的交换区域!\t","提示",MB_OK|MB_ICONINFORMATION);
                txtArea->SetFocus();
                return;
        }

        int ret;
        MyRes res;
        MyRow row;

        TListItem *p = ListView->Selected;     //选取选中项，并将从数据库删除
        if(p == NULL)
        {
                MessageBox(Handle,"没有选中修改项!\t","提示",MB_OK|MB_ICONINFORMATION);
                return ;
        }

        int merge,compare,notice;
        String pkgno;

        if(p->SubItems->Strings[2] == "不匹配")
                merge = 0;
        else    merge = 2;

        if(p->SubItems->Strings[3] == "不校验")
                compare = 0;
        else    compare = 1;

    if(chbCancel->Checked == true)
    {
        notice = 0;
        pkgno = txtNet2->Text;
    }else
    {
        notice = 1;
        pkgno = txtNetno->Text;
    }

//--------------------------------------------------------------------------------
  /*
        ret = FmIC->gdb.VQuery(512,"select notice from bocnets where exchno='%s' and pkgno='%s' and area='%s' and merge=%d and compare=%d",p->Caption.c_str(),p->SubItems->Strings[0].c_str(),p->SubItems->Strings[1].c_str(),merge,compare);
        if(ret)
        {
                MessageBox(Handle,"数据库查询错误!0034\t","提示",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        res = FmIC->gdb.Store();
        row = res.Fetch();
        if(row)
                notice = atoi(row[0]);
        else
                notice = 0;

    */


        //从数据库删除选中项
        ret = FmIC->gdb.VQuery(512,"delete from bocnets where exchno='%s' and area='%s'",p->Caption.c_str(),p->SubItems->Strings[1].c_str());
        if(ret)
        {
                MessageBox(Handle,"数据库查询错误!0031\t","提示",MB_OK|MB_ICONINFORMATION);
                return ;
        }
      
        //判断交换区域和打码类型数据库是否已包含
        ret = FmIC->gdb.VQuery(512,"select count(*) from bocnets where area='%s' and exchno='%s'",txtArea->Text.Trim().c_str(),txtNetno->Text.Trim().c_str());

        if(ret)
        {
                MessageBox(Handle,"数据库查询错误!0032\t","提示",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        res = FmIC->gdb.Store();
        row = res.Fetch();
        if(atoi(row[0]))                //如果数据库包含所查找的数据，row不为空，否则为空
        {
                AnsiString str;
                str.sprintf("交换号[%s]和交换区域[%s]重复!0033\t",txtNetno->Text.Trim().c_str(),txtArea->Text.Trim().c_str());
                MessageBox(Handle,str.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
                ret = FmIC->gdb.VQuery(512,"insert into bocnets(exchno,pkgno,area,notice,merge,compare) values('%s','%s','%s',%d,%d,%d)",p->Caption.c_str(),p->SubItems->Strings[0].c_str(),p->SubItems->Strings[1].c_str(),notice,merge,compare);
                if(ret)
                {
                        MessageBox(Handle,"数据库查询错误!0036\t","提示",MB_OK|MB_ICONINFORMATION);
                        return ;
                }
                flushData();
                txtNetno->Text = "";
                txtNet2->Text = "";
                txtArea->Text = "";
                return ;
        }

        if(!cbMerge->Checked)   merge = 0;      else    merge = 1;
        if(!cbCheck->Checked)   compare = 0;    else    compare = 1;

        //添加更改后记录到数据库
        ret = FmIC->gdb.VQuery(512,"insert into bocnets(exchno,pkgno,area,notice,merge,compare) values('%s','%s','%s',%d,%d,%d)",txtNetno->Text.c_str(),txtNet2->Text.c_str(),txtArea->Text.c_str(),notice,merge,compare);
       
        if(ret)
        {
                MessageBox(Handle,"数据库查询错误!0035\t","提示",MB_OK|MB_ICONINFORMATION);
                return ;
        }


        MessageBox(Handle,"修改成功!\t","提示",MB_OK|MB_ICONINFORMATION);
        flushData();
        txtNetno->Text = "";
        txtNet2->Text = "";
        txtArea->Text = "";
}
//---------------------------------------------------------------------------

