//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <bcbext.h>
#include <math.h>
#include <stdio.h>
#include <zcommon.h>
#include "clkinfo.h"
#include "main.h"
#include <bcbext.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmCkInfo *FmCkInfo;
//---------------------------------------------------------------------------
__fastcall TFmCkInfo::TFmCkInfo(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFmCkInfo::btnExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------




void __fastcall TFmCkInfo::txtRadioAClick(TObject *Sender)
{
    txtPrivList->Enabled = false;
    clear();
    setPriv(PRIV_OPT_VOU+PRIV_OPT_ENV);
}
//---------------------------------------------------------------------------

void __fastcall TFmCkInfo::txtRadioBClick(TObject *Sender)
{
     txtPrivList->Enabled = false;
     clear();
     setPriv(PRIV_OPT_PBC+PRIV_OPT_DST+PRIV_OPT_SCN+PRIV_OPT_VOU+PRIV_OPT_ENV+PRIV_OPT_UPD+PRIV_OPT_REP);
}
//---------------------------------------------------------------------------

void __fastcall TFmCkInfo::txtRadioCClick(TObject *Sender)
{
     txtPrivList->Enabled = false;
     clear();
     setPriv( PRIV_OPT_CLK+PRIV_OPT_DEP+PRIV_OPT_VCH+PRIV_OPT_CFG);
}
//---------------------------------------------------------------------------

void __fastcall TFmCkInfo::txtRadioDClick(TObject *Sender)
{
     txtPrivList->Enabled = true;
     clear();

}
//---------------------------------------------------------------------------


int TFmCkInfo::clear()
{
    //TODO: Add your source code here
    for( int i = 0;i< txtPrivList->Items->Count; i++)
    {
        txtPrivList->Checked[i] = false;
    }
}

int TFmCkInfo::setPriv(int priv)
{
    //TODO: Add your source code here
    clear();
    for( int i = 0;i< txtPrivList->Items->Count; i++)
    {
        if(priv& int(powl(2,(i+5))) )
            txtPrivList->Checked[i] = true;
    }

}
void __fastcall TFmCkInfo::FormShow(TObject *Sender)
{
    SetCtrlStyle(txtClkno,ES_NUMBER);
    SetCtrlStyle(txtArea,ES_NUMBER);   

    if(_type == 0 )
    {
        txtClkno->Enabled  = false;
        txtName->Enabled  = false;
        txtID->Enabled  = false;
        txtPswdOne->Enabled  = false;
        txtPswdTwo->Enabled  = false;
        txtSex->Enabled = false;
        txtArea->Enabled = false;
    }
    else if(_type == 1 )
    {


        txtClkno->Enabled  = true;
        txtName->Enabled  = true;
        txtID->Enabled  = true;
        txtPswdOne->Enabled  = true;
        txtPswdTwo->Enabled  = true;
        txtSex->Enabled = true;
        txtArea->Enabled = true;
        clear();

        txtClkno->SetFocus();
        txtClkno->Text = "";
        txtName->Text = "";
        txtID->Text = "";
        txtPswdOne->Text = "";
        txtPswdTwo->Text = "";
        txtSex->ItemIndex = 0;

        txtRadioA->Checked = false;
        txtRadioB->Checked = false;
        txtRadioC->Checked = false;
        txtRadioD->Checked = false;

        _priv = 1;
    }
    else if(_type == 2 )
    {
        txtClkno->Enabled  = false;
        txtName->Enabled  = true;
        txtID->Enabled  = true;
        txtPswdOne->Enabled  = true;
        txtPswdTwo->Enabled  = true;
        txtSex->Enabled = true;
        txtArea->Enabled = true;
    }

    if(_type != 1 )
    {
        String cmd =  AnsiString("select clkno,name,personalid,authority ,sex,area from clerks where clkno='")+_clkno+"'";
        int ret = FmIC->gdb.Query(cmd.c_str());
        if(ret)
        {
            MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error[%s]\n",mysql_error(FmIC->gdb.Handle()));
            return ;
        }

        MyRes res = FmIC->gdb.Store();
        MyRow row = res.Fetch();

        txtClkno->Text  = row[0];
        txtName->Text   = row[1];
        txtID->Text     = row[2];
        txtArea->Text     = row[5];
        txtPswdOne->Text = "";
        txtPswdTwo->Text = "";
        _priv = atoi(row[3]);
        txtSex->ItemIndex  =  atoi(row[4]);

    }

    if(_priv & PRIV_GRD_NOM)
        txtRadioA->Checked = true;
    else  if(_priv & PRIV_GRD_ADM )
        txtRadioC->Checked = true;
    else  if(_priv & PRIV_GRD_MID)
        txtRadioB->Checked = true;
    else  if(_priv & PRIV_GRD_CUS)
        txtRadioD->Checked = true;

    if(_priv & 1 )
        ckModify->Checked = true;
    setPriv(_priv);

    txtPrivList->Enabled = false;

}
//---------------------------------------------------------------------------


int TFmCkInfo::setMethod(int type,char *clkno)
{
    //TODO: Add your source code here
    memset(_clkno,0,sizeof(_clkno));
    _type = type ;
    strcpy(_clkno,clkno);

    return 0;
}
void __fastcall TFmCkInfo::btnSaveClick(TObject *Sender)
{
    if(txtClkno->Text.Length()<1)
    {
        MessageBox(Handle,"柜员号有误!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtClkno->SetFocus();
        return;
    }

    if(txtName->Text.Length()<1)
    {
        MessageBox(Handle,"姓名有误!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtName->SetFocus();
        return;
    }
    if(txtArea->Text.Length()<1)
    {
        MessageBox(Handle,"交换区域有误!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtArea->SetFocus();
        return;
    }
    if(txtPswdOne->Text != txtPswdTwo->Text )
    {
        MessageBox(Handle,"密码有误!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtPswdOne->SetFocus();
        txtPswdOne->Clear();
        txtPswdTwo->Clear();
        return;
    }

    int priv = getPriv();
    if(priv<=17)
    {
         MessageBox(Handle,"没有选择权限!\t","提示",MB_OK|MB_ICONINFORMATION);
         return;
    }

    char cmd[512];
    memset(cmd,0,sizeof(cmd));


    if(_type == 1)
    {        
        sprintf(cmd ,"insert into clerks( clkno,name,personalid,authority,state,sex,password,area) values('%s','%s','%s',%d,1,%d,PASSWORD('%s'),'%s')",txtClkno->Text.c_str(),txtName->Text.c_str(),txtID->Text.c_str(),priv,txtSex->ItemIndex,txtPswdOne->Text.c_str(),txtArea->Text.c_str());
    }
    else
    {
        if(ckModify->Checked == true)
            sprintf(cmd ,"update clerks set name='%s',personalid='%s',authority=%d ,sex=%d ,password=PASSWORD('%s'),area='%s' where clkno='%s'",txtName->Text.c_str(),txtID->Text.c_str(),priv,txtSex->ItemIndex,txtPswdOne->Text.c_str(),txtArea->Text.c_str(),txtClkno->Text.c_str());
        else
            sprintf(cmd ,"update clerks set name='%s',personalid='%s',authority=%d ,sex=%d ,area='%s' where clkno='%s'",txtName->Text.c_str(),txtID->Text.c_str(),priv,txtSex->ItemIndex,txtArea->Text.c_str(),txtClkno->Text.c_str());

    }

    int ret = FmIC->gdb.Query(cmd);
    if(ret)
    {
          MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
          return ;
    }       

    ModalResult = mrOk;
}
//---------------------------------------------------------------------------


int TFmCkInfo::getPriv()
{
    //TODO: Add your source code here
    int priv = 0;

    if( ckModify->Checked == true )
    {
        priv |= PRIV_MOD_PWD;
    }

    if(txtRadioA->Checked == true)
    {
        priv |= PRIV_GRD_NOM;
    }
    else if(txtRadioC->Checked == true)
    {
        priv |= PRIV_GRD_ADM;
    }
    else if(txtRadioB->Checked == true)
    {
        priv |= PRIV_GRD_MID;
    }
    else if(txtRadioD->Checked == true)
    {
        priv |= PRIV_GRD_CUS;
    }

    for( int i = 0;i< txtPrivList->Items->Count; i++)
   {
        if( txtPrivList->Checked[i] ==true)
            priv |= int(pow(2,i+5));
    }
    return priv;
}
void __fastcall TFmCkInfo::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
      if(Key == 13)
        PostMessage(Handle,WM_NEXTDLGCTL,0,0);    
}
//---------------------------------------------------------------------------


void __fastcall TFmCkInfo::txtClknoExit(TObject *Sender)
{
    int ret = FmIC->gdb.VQuery(512,"select count(*) from clerks where clkno='%s'",txtClkno->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error[%s]\n",mysql_error(FmIC->gdb.Handle()));
        return ;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    if(atoi(row[0]) !=0)
    {
        String b = AnsiString("柜员[")+ txtClkno->Text.c_str() +  "]已存在!\t";
        MessageBox(Handle,b.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        txtClkno->Text = "";
        txtClkno->SetFocus();
        return ;
    }
}
//---------------------------------------------------------------------------

