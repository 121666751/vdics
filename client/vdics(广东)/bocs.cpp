/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
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
        MessageBox(Handle,"�Ƿ�������!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    if(txtArea->Text.Length()<6)
    {
        MessageBox(Handle,"�Ƿ���������!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    if(atoi(row[0])>0)
    {
        MessageBox(Handle,"������Ϣ�Ѵ���!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }


    ret = FmIC->gdb.VQuery(512,"insert into bocnets(exchno,pkgno,area,notice,merge,compare) values('%s','%s','%s',%d,%d,%d) ",txtNetno->Text.c_str(),pkgno.c_str(),txtArea->Text.c_str(),notice,merge,check);

    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    
    MessageBox(Handle,"���ӳɹ�!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
            li->SubItems->Insert(2,"��ƥ��");
        else
            li->SubItems->Insert(2,"ƥ��");

        if(atoi(row[4]) == 0 )
            li->SubItems->Insert(3,"��У��");
        else
            li->SubItems->Insert(3,"У��");
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

    String b = AnsiString("�Ƿ�Ҫɾ��������[")+accno+"]?\t";
    if(MessageBox(Handle,b.c_str(),"��ʾ",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

    int ret = FmIC->gdb.VQuery(512,"delete from bocnets where exchno='%s' and area='%s'",accno.c_str(),area.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }

    MessageBox(Handle,"ɾ���ɹ�!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

    cbMerge->Checked  =  (li->SubItems->Strings[2]=="ƥ��");
    cbCheck->Checked  =  (li->SubItems->Strings[3]=="У��");    
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
     //�ж��Ƿ�Ϊ��
        if(txtNetno->Text.Trim().Length()<6 || txtNetno->Text.Trim().Length()>12)     //�жϽ������Ƿ�Ϊ��
        {
                MessageBox(Handle,"�Ƿ��Ľ�����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                txtNetno->SetFocus();
                return;
         }
        if(txtNet2->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"�������㲻��Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                txtNet2->SetFocus();
                return;
        }
      
        if(txtArea->Text.Trim().Length()<6)
        {
                MessageBox(Handle,"�Ƿ��Ľ�������!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                txtArea->SetFocus();
                return;
        }

        int ret;
        MyRes res;
        MyRow row;

        TListItem *p = ListView->Selected;     //ѡȡѡ������������ݿ�ɾ��
        if(p == NULL)
        {
                MessageBox(Handle,"û��ѡ���޸���!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                return ;
        }

        int merge,compare,notice;
        String pkgno;

        if(p->SubItems->Strings[2] == "��ƥ��")
                merge = 0;
        else    merge = 2;

        if(p->SubItems->Strings[3] == "��У��")
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
                MessageBox(Handle,"���ݿ��ѯ����!0034\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        res = FmIC->gdb.Store();
        row = res.Fetch();
        if(row)
                notice = atoi(row[0]);
        else
                notice = 0;

    */


        //�����ݿ�ɾ��ѡ����
        ret = FmIC->gdb.VQuery(512,"delete from bocnets where exchno='%s' and area='%s'",p->Caption.c_str(),p->SubItems->Strings[1].c_str());
        if(ret)
        {
                MessageBox(Handle,"���ݿ��ѯ����!0031\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                return ;
        }
      
        //�жϽ�������ʹ����������ݿ��Ƿ��Ѱ���
        ret = FmIC->gdb.VQuery(512,"select count(*) from bocnets where area='%s' and exchno='%s'",txtArea->Text.Trim().c_str(),txtNetno->Text.Trim().c_str());

        if(ret)
        {
                MessageBox(Handle,"���ݿ��ѯ����!0032\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        res = FmIC->gdb.Store();
        row = res.Fetch();
        if(atoi(row[0]))                //������ݿ���������ҵ����ݣ�row��Ϊ�գ�����Ϊ��
        {
                AnsiString str;
                str.sprintf("������[%s]�ͽ�������[%s]�ظ�!0033\t",txtNetno->Text.Trim().c_str(),txtArea->Text.Trim().c_str());
                MessageBox(Handle,str.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
                ret = FmIC->gdb.VQuery(512,"insert into bocnets(exchno,pkgno,area,notice,merge,compare) values('%s','%s','%s',%d,%d,%d)",p->Caption.c_str(),p->SubItems->Strings[0].c_str(),p->SubItems->Strings[1].c_str(),notice,merge,compare);
                if(ret)
                {
                        MessageBox(Handle,"���ݿ��ѯ����!0036\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

        //��Ӹ��ĺ��¼�����ݿ�
        ret = FmIC->gdb.VQuery(512,"insert into bocnets(exchno,pkgno,area,notice,merge,compare) values('%s','%s','%s',%d,%d,%d)",txtNetno->Text.c_str(),txtNet2->Text.c_str(),txtArea->Text.c_str(),notice,merge,compare);
       
        if(ret)
        {
                MessageBox(Handle,"���ݿ��ѯ����!0035\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                return ;
        }


        MessageBox(Handle,"�޸ĳɹ�!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        flushData();
        txtNetno->Text = "";
        txtNet2->Text = "";
        txtArea->Text = "";
}
//---------------------------------------------------------------------------

