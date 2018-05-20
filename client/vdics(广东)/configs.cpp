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

#include "configs.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormConfig *FormConfig;
//---------------------------------------------------------------------------
__fastcall TFormConfig::TFormConfig(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::numExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TFormConfig::FormShow(TObject *Sender)
{
    SetCtrlStyle(txtFlowport,ES_NUMBER);
    SetCtrlStyle(txtEcmport,ES_NUMBER);
    SetCtrlStyle(txtSignport,ES_NUMBER);
    SetCtrlStyle(txtBrno,ES_NUMBER);
    SetCtrlStyle(txtAccno,ES_NUMBER);
    SetCtrlStyle(txtDcode,ES_NUMBER);
    SetCtrlStyle(txtMsgType,ES_NUMBER);
    SetCtrlStyle(txtMsgTypeEx,ES_NUMBER);
    showConfig();
    txtFlowip->SetFocus();
    cbPre->Checked = false;
    cbSign->Checked = false;     
    cbAmount->Checked = false;
    cbClear->Checked = false;

    int ret;
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('pretreat','%d')",0);
    if(ret)
    {
        MessageBox(Handle,"����Ԥ���������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('signflag','%d')",0);
    if(ret)
    {
        MessageBox(Handle,"������Ʊ�Ǽ��������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('amount','%d')",0);
    if(ret)
    {
        MessageBox(Handle,"�������ͽ���������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('clear','%d')",0);
    if(ret)
    {
        MessageBox(Handle,"����������Ϣ���ʧ��\t","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormConfig::btnExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------


int TFormConfig::showConfig()
{
    //TODO: Add your source code here
    int ret = FmIC->gdb.Query("select name,value from config");
    if(ret)
    {
        MessageBox(Handle,"��ѯ��������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    MyRes res =  FmIC->gdb.Store();
    MyRow row = res.Fetch();

    while(row!=NULL)
    {
        if (strcmp(row[0],"flowip") == 0 )
        {
            txtFlowip->Text = row[1];
        }
        else if(strcmp(row[0],"flowport") == 0)
        {
            txtFlowport->Text = row[1];
        }
        else if(strcmp(row[0],"ecmip") == 0)
        {
            txtEcmip->Text = row[1];
        }
        else if(strcmp(row[0],"ecmport") == 0)
        {
            txtEcmport->Text = row[1];
        }            
        else if(strcmp(row[0],"signip") == 0)
        {
            txtSignip->Text = row[1];
        }
        else if(strcmp(row[0],"signport") == 0)
        {
            txtSignport->Text = row[1];
        }
        else if(strcmp(row[0],"brno") == 0)
        {
            txtBrno->Text = row[1];
        }
        else if(strcmp(row[0],"msgtype") == 0)
        {
            txtMsgType->Text = row[1];
        } 

        else if(strcmp(row[0],"msgtypeex") == 0)
        {
            txtMsgTypeEx->Text = row[1];
        }
        else if(strcmp(row[0],"pretreat") == 0)
        {
            cbPre->Checked = ( atoi(row[1]) == 1);
        }
        else if(strcmp(row[0],"signflag") == 0)
        {
            cbSign->Checked = ( atoi(row[1]) == 1);
        }
        else if(strcmp(row[0],"amount") == 0)
        {
            cbAmount->Checked = ( atoi(row[1]) == 1);
        }
        else if(strcmp(row[0],"header") == 0)
        {
            txtAccno->Text= row[1];
        }
        else if(strcmp(row[0],"amount") == 0)
        {
            cbAmount->Checked = ( atoi(row[1]) == 1);             
        }  
        else if(strcmp(row[0],"dcode") == 0)
        {
            txtDcode->Text= row[1];
        }
        else if(strcmp(row[0],"mode") == 0)
        {
            cbMode->ItemIndex= atoi(row[1]);
        }

        else if(strcmp(row[0],"clear") == 0)
        {
            cbClear->Checked = ( atoi(row[1]) == 1);
        }
        row = res.Fetch();
    }

    return 0;
}

int TFormConfig::saveConfig()
{
    //TODO: Add your source code here
    int ret;

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('flowip','%s')",txtFlowip->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"�������̷����ַ����\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('flowport','%s')",txtFlowport->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"�������̷���˿ڴ���\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('msgtype','%s')",txtMsgType->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"�������̽��״������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('msgtypeex','%s')",txtMsgTypeEx->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"�������̽��״������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }


    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('ecmip','%s')",txtEcmip->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"����ECM�����ַ����\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('ecmport','%s')",txtEcmport->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"����ECM����˿ڴ���\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
  
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('signip','%s')",txtFlowip->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"������Ʊ�����ַ����\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('signport','%s')",txtFlowport->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"������Ʊ����˿ڴ���\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
  
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('brno','%s')",txtBrno->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"������к������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('header','%s')",txtAccno->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"���������˺Ŵ���\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('dcode','%s')",txtDcode->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"����跽��Ʊ�������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    int checked;

    if(cbPre->Checked == true)
        checked = 1;
    else
        checked = 0;
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('pretreat','%d')",checked);
    if(ret)
    {
        MessageBox(Handle,"����Ԥ���������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    if(cbSign->Checked == true)
        checked = 1;
    else
        checked = 0;

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('mode','%d')",cbMode->ItemIndex);
    if(ret)
    {
        MessageBox(Handle,"���洦��ģʽ����\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('signflag','%d')",checked);
    if(ret)
    {
        MessageBox(Handle,"������Ʊ�Ǽ��������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    if(cbAmount->Checked == true)
        checked = 1;
    else
        checked = 0;

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('amount','%d')",checked);
    if(ret)
    {
        MessageBox(Handle,"�������ͽ���������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    if(cbClear->Checked == true)
        checked = 1;
    else
        checked = 0;
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('clear','%d')",checked);
    if(ret)
    {
        MessageBox(Handle,"����������Ϣ���ʧ��\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    MessageBox(Handle,"��������ɹ�,���ؽ�ϵͳ\t","��ʾ",MB_OK|MB_ICONINFORMATION);

    return 0;
}
void __fastcall TFormConfig::btnSaveClick(TObject *Sender)
{
    saveConfig();    
}
//---------------------------------------------------------------------------


