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
        MessageBox(Handle,"保存预扣情况错误\t","提示",MB_OK|MB_ICONINFORMATION);
    }
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('signflag','%d')",0);
    if(ret)
    {
        MessageBox(Handle,"保存退票登记情况错误\t","提示",MB_OK|MB_ICONINFORMATION);
    }
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('amount','%d')",0);
    if(ret)
    {
        MessageBox(Handle,"保存上送金额情况错误\t","提示",MB_OK|MB_ICONINFORMATION);
    }
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('clear','%d')",0);
    if(ret)
    {
        MessageBox(Handle,"保存清算信息检查失败\t","提示",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"查询参数错误\t","提示",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"保存流程服务地址错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('flowport','%s')",txtFlowport->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"保存流程服务端口错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('msgtype','%s')",txtMsgType->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"保存流程交易代码错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('msgtypeex','%s')",txtMsgTypeEx->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"保存流程交易代码错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }


    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('ecmip','%s')",txtEcmip->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"保存ECM服务地址错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('ecmport','%s')",txtEcmport->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"保存ECM服务端口错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
  
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('signip','%s')",txtFlowip->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"保存退票服务地址错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('signport','%s')",txtFlowport->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"保存退票服务端口错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
  
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('brno','%s')",txtBrno->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"保存分行号码错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('header','%s')",txtAccno->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"保存区域账号错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('dcode','%s')",txtDcode->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"保存借方退票代码错误\t","提示",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"保存预扣情况错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    if(cbSign->Checked == true)
        checked = 1;
    else
        checked = 0;

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('mode','%d')",cbMode->ItemIndex);
    if(ret)
    {
        MessageBox(Handle,"保存处理模式错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('signflag','%d')",checked);
    if(ret)
    {
        MessageBox(Handle,"保存退票登记情况错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    if(cbAmount->Checked == true)
        checked = 1;
    else
        checked = 0;

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('amount','%d')",checked);
    if(ret)
    {
        MessageBox(Handle,"保存上送金额情况错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    if(cbClear->Checked == true)
        checked = 1;
    else
        checked = 0;
    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('clear','%d')",checked);
    if(ret)
    {
        MessageBox(Handle,"保存清算信息检查失败\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    MessageBox(Handle,"保存参数成功,请重进系统\t","提示",MB_OK|MB_ICONINFORMATION);

    return 0;
}
void __fastcall TFormConfig::btnSaveClick(TObject *Sender)
{
    saveConfig();    
}
//---------------------------------------------------------------------------


