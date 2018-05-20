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

#include "envimage.h"
#include <zcommon.h>
#include <transimg.h>
#include "login.h"
#include "more.h"
#include "main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEnvImage *FormEnvImage;
//---------------------------------------------------------------------------
__fastcall TFormEnvImage::TFormEnvImage(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormEnvImage::FormShow(TObject *Sender)
{
    image->Align = alClient;
    image->Left = 1;
    image->Top = 1;
    image->AutoSize = false;
    image->Tag = 1;
    txtAmount->SetFocus();
    SetCtrlStyle(txtVchType,ES_NUMBER);    
}
//---------------------------------------------------------------------------
void __fastcall TFormEnvImage::imageMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TImage* p = (TImage*)Sender;

    if(p->Tag)
    {
        p->Tag = 0;
        p->AutoSize = false;
        p->Align = alClient;
        p->Proportional = true;
        p->Stretch = true;
        return;
    }

    double r1 = (double)p->Picture->Width/p->Width;
    double r2 = (double)p->Picture->Height/p->Height;
    double r =  (r1>r2)? r1 : r2;

    p->Align = alNone;
    p->Left = X - X*r;
    p->Top = Y - Y*r;

    p->AutoSize = true;
    p->Tag = 1;

}
//--------------------------------------------------------------------------- 
void __fastcall TFormEnvImage::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
     if(Key==13||Key==40||Key==34)  //回车 下箭头 pgdn
        PostMessage(Handle,WM_NEXTDLGCTL,0,0);    
}
//--------------------------------------------------------------------------- 
void __fastcall TFormEnvImage::btnSaveClick(TObject *Sender)
{
    if( txtAmount->Text.IsEmpty() )
    {
        MessageBox(Handle,"金额不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtAmount->SetFocus();
        ModalResult = mrNone;
        return;
    }

    if( txtVchType->Text.IsEmpty() )
    {
        MessageBox(Handle,"凭证类型不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtVchType->SetFocus();
        ModalResult = mrNone;
        return;
    }

    int modeflag = chkMode->Checked?1:0;

    int mainflag = -1;
    if( type == 1 )
    {
        int vchflag = 0 ;
        if(txtType->ItemIndex  == 0 )
        {
            vchflag = 200;
        }
        else if (txtType->ItemIndex  == 1 )
        {
            vchflag = 6;
        }
        else if (txtType->ItemIndex  == 2 )
        {
            vchflag = 7;
        }

        else if (txtType->ItemIndex  == 3 )
        {
            vchflag = 0;
            mainflag = 0;
        }

        int ret = FmIC->gdb.VQuery(512,"update multis set amount='%s',state=1,model=%d,mainflag=%d,vchtype='%s',clkno='%s',modeflag=%d where sid=%d",RemoveThousandChar(txtAmount->Text.c_str()),vchflag,mainflag,txtVchType->Text.c_str(),g_sys_opt.c_str(),modeflag,curID);
        if(ret)
        {
            MessageBox(Handle,"查询出错\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }

        if(FmEnv->lstMerge->Items->Count>0 && (FmEnv->cmbIdentify->ItemIndex == 3 || FmEnv->cmbIdentify->ItemIndex == 1)  )
        {
            ret =  FmEnv->getNextScan();
            if(ret)
                ModalResult = mrOk;
            else
                ModalResult = mrNone;
        }
    }

    if( type == 4 )
    {
        int ret = FmIC->gdb.VQuery(512,"update multis set checkstate=1,amount='%s',state=1,vchtype='%s' ,clkno='%s',modeflag=%d where mainid=%d",RemoveThousandChar( txtAmount->Text.c_str()),txtVchType->Text.c_str(),g_sys_opt.c_str(),modeflag,curID);
        if(ret)
        {
            MessageBox(Handle,"查询出错\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }

        if(FmEnv->lstSingle->Items->Count>0 && (FmEnv->cmbIdentify->ItemIndex == 3 || FmEnv->cmbMerge->ItemIndex == 2)  )
        {
            ret =  FmEnv->getNext();
            if(ret)
                ModalResult = mrOk;
            else
                ModalResult = mrNone;
        }
    }



}
//---------------------------------------------------------------------------


void TFormEnvImage::MoneyChange(TEdit * txtMoney)
{
    //TODO: Add your source code here
    if( txtMoney->Text.IsEmpty() ) txtMoney->Text = "0";
    if( bFormatMoney ) return;
    bFormatMoney = true;

    txtMoney->Text = RemoveThousandChar( txtMoney->Text );
    txtMoney->Text = AddThousandChar( txtMoney->Text );

    //自动加小数点
    txtMoney->SelStart = txtMoney->Text.Pos(".")-1+nPopos;
    txtMoney->SelLength = 0;

    bFormatMoney = false;
}

void TFormEnvImage::MoneyKeyPress(TEdit* txtMoney, char & Key)
{
    //TODO: Add your source code here
     if( txtMoney->SelText == txtMoney->Text ) bPoint = false;

	if( Key == 13 || Key == 27 ) return;
    if( Key == 8 )
    {
        txtMoney->SelectAll();
        Key = 0;
        return;
    }

    if( Key == '.' )
    {
        txtMoney->SelStart = txtMoney->Text.Pos(".");
        bPoint = true;
        nPopos = 0 + (int)bPoint;
    }
 	if( Key < '0' || Key > '9' ) Key = 0;
    else nPopos = (nPopos+1) * (int)bPoint;
}

//---------------------------------------------------------------------------


void __fastcall TFormEnvImage::txtAmountKeyPress(TObject *Sender,
      char &Key)
{
     MoneyKeyPress( txtAmount, Key);    
}
//---------------------------------------------------------------------------

void __fastcall TFormEnvImage::txtAmountEnter(TObject *Sender)
{
     bPoint = false;    
}
//---------------------------------------------------------------------------

void __fastcall TFormEnvImage::txtAmountChange(TObject *Sender)
{
    MoneyChange( txtAmount );    
}
//---------------------------------------------------------------------------

void __fastcall TFormEnvImage::FormCreate(TObject *Sender)
{
     bFormatMoney = false;    
}
//---------------------------------------------------------------------------   

void __fastcall TFormEnvImage::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    image->AutoSize = false;
    image->Align = alClient;
    image->Proportional = true;
    image->Stretch = true;
    image->Tag = 0;        
}
//---------------------------------------------------------------------------

