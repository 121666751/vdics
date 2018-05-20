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
          MessageBox(Handle,"���ݿ��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        //�޸���չʾ���� -- FPF
        li->SubItems->Insert(0,row[1]);
        li->SubItems->Insert(1,row[2]);
        li->SubItems->Insert(2,row[3]);

        li->SubItems->Insert(3,row[4]);
        if(atoi(row[5])==0)
            li->SubItems->Insert(4,"�跽");
        else
            li->SubItems->Insert(4,"����");


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
        MessageBox(Handle,"����������Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtArea->SetFocus();
        return;
    }
    if(txtVchtype->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"ƾ֤���Ͳ���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtVchtype->SetFocus();
        return;
    }

    if(txtType->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"ƾ֤���Ͳ���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtType->SetFocus();
        return;
    }

    if(txtModel->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"�������Ͳ���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtModel->SetFocus();
        return;
    }

    if(txtName->Text.Trim().Length()<1)
    {
        MessageBox(Handle,"ƾ֤���Ʋ���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtName->SetFocus();
        return;
    }

    int ret = FmIC->gdb.VQuery(512,"select count(*) from vchtypes where vchtype='%s' and area='%s'",txtVchtype->Text.c_str(),txtArea->Text.c_str());
    if(ret)
    {
          MessageBox(Handle,"���ݿ��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
          return ;
    }


    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    if(atoi(row[0])!=0)
    {
        String s = AnsiString("��������[")+txtArea->Text.c_str()+"]ƾ֤����["+txtVchtype->Text.c_str()+"]�Ѵ���!\t";
        MessageBox(Handle,s.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    ret = FmIC->gdb.VQuery(512,"insert into vchtypes( vchtype,banktype,model,expcn,cdcode,area) values ('%s','%s',%d,'%s',%d,'%s')",txtVchtype->Text.c_str(),txtType->Text.c_str(),txtModel->Text.ToInt(),txtName->Text.c_str(),cbVchtype->ItemIndex,txtArea->Text.c_str());
    if(ret)
    {
          MessageBox(Handle,"���ݿ��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
          return ;
    }

    MessageBox(Handle,"���ӳɹ�!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

    String b = AnsiString("�Ƿ�Ҫɾ����������[") +area+"��ƾ֤����["+vchtype+"]?";
    if(MessageBox(Handle,b.c_str(),"��ʾ",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

    int ret = FmIC->gdb.VQuery(512,"delete from vchtypes where vchtype='%s' and area='%s'",vchtype.c_str(),area.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }

    MessageBox(Handle,"ɾ���ɹ�\t","��ʾ",MB_OK|MB_ICONINFORMATION);
    showVchtype();        
}
//---------------------------------------------------------------------------
void __fastcall TFmVchtype::btnModifClick(TObject *Sender)
{
     //�ж��Ƿ�Ϊ��
        if(txtArea->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"����������Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                txtArea->SetFocus();
                return;
         }
        if(txtVchtype->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"ƾ֤���Ͳ���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                txtVchtype->SetFocus();
                return;
        }

        if(txtType->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"ƾ֤���Ͳ���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                txtType->SetFocus();
                return;
        }

        if(txtModel->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"�������Ͳ���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                txtModel->SetFocus();
                return;
        }
        if(txtName->Text.Trim().Length()<1)
        {
                MessageBox(Handle,"ƾ֤���Ʋ���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                txtName->SetFocus();
                return;
        }

        int ret;
        MyRes res;
        MyRow row;

        TListItem *p = lstVchtype->Selected;     //ѡȡѡ������������ݿ�ɾ��
        if(p == NULL)
        {
                MessageBox(Handle,"û��ѡ���޸���!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        int ct;     //��Ǳ�־
        //ɾ��һ����¼
        if(p->SubItems->Strings[4] == "�跽")
                ct = 0;
        else
                ct = 1;
        ret = FmIC->gdb.VQuery(512,"delete from vchtypes where area='%s' and vchtype='%s' and banktype='%s' and model=%d and expcn='%s' and cdcode=%d",p->Caption.c_str(),p->SubItems->Strings[0].c_str(),p->SubItems->Strings[1].c_str(),p->SubItems->Strings[2].ToInt(),p->SubItems->Strings[3].c_str(),ct);
        if(ret)
        {
                MessageBox(Handle,"���ݿ��ѯ����!0011\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                return ;
        }

        //�жϽ�������ʹ����������ݿ��Ƿ��Ѱ���
        ret = FmIC->gdb.VQuery(512,"select count(*) from vchtypes where area='%s' and vchtype='%s'",txtArea->Text.Trim().c_str(),txtVchtype->Text.Trim().c_str());
        if(ret)
        {
                MessageBox(Handle,"���ݿ��ѯ����!0012\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        res = FmIC->gdb.Store();
        row = res.Fetch();
        if(atoi(row[0]))
        {
                AnsiString str;
                str.sprintf("��������[%s]�ʹ�������[%s]�ظ�!0013\t",txtArea->Text.Trim().c_str(),txtVchtype->Text.Trim().c_str());
                MessageBox(Handle,str.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);

                ret = FmIC->gdb.VQuery(512,"insert into  vchtypes (area,vchtype,banktype,model,expcn,cdcode) values('%s','%s','%s',%d,'%s',%d)",p->Caption.c_str(),p->SubItems->Strings[0].c_str(),p->SubItems->Strings[1].c_str(),p->SubItems->Strings[2].ToInt(),p->SubItems->Strings[3].c_str(),ct);
                if(ret)
                {
                        MessageBox(Handle,"���ݿ��ѯ����!0015\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                        return ;
                }
                showVchtype();         //ˢ��TListView�б�
                txtArea->Text = "";
                txtVchtype->Text = "";
                txtType->Text = "";
                txtModel->Text = "";
                txtName->Text = "";
                return ;
        }
        

        //����һ����¼������������ӵ����ݿ⣬ʵ�ָ���
        ret = FmIC->gdb.VQuery(512,"insert into  vchtypes (vchtype,banktype,model,expcn,cdcode,area) values('%s','%s',%d,'%s',%d,'%s')",txtVchtype->Text.Trim().c_str(),txtType->Text.Trim().c_str(),txtModel->Text.ToInt(),txtName->Text.Trim().c_str(),cbVchtype->ItemIndex,txtArea->Text.Trim().c_str());
        if(ret)
        {
                MessageBox(Handle,"���ݿ��ѯ����!0010\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        MessageBox(Handle,"�޸ĳɹ�!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        showVchtype();         //ˢ��TListView�б�
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
	TListItem * p = lstVchtype->Selected;    //ѡ����
	txtArea->Text = p->Caption;              //��������
	txtVchtype->Text = p->SubItems->Strings[0]; //��������
	txtType->Text = p->SubItems->Strings[1]; //��������
	txtModel->Text = p->SubItems->Strings[2];//��������
	txtName->Text = p->SubItems->Strings[3]; //ƾ֤����
	if(p->SubItems->Strings[4] == "�跽")
			cbVchtype->ItemIndex = 0;
	else
			cbVchtype->ItemIndex = 1;
}
//---------------------------------------------------------------------------

