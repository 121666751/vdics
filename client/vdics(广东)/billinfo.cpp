//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <ztime.hpp>
#include <bcbext.h>
#include "main.h"
#include "billinfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBill *FormBill;
//---------------------------------------------------------------------------
__fastcall TFormBill::TFormBill(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormBill::btnOkClick(TObject *Sender)
{
    if(txtVchtype->Text.Trim().Length()<2 )
    {
         MessageBox(Handle,"凭证类型不对!\t","提示",MB_OK|MB_ICONINFORMATION);
         txtVchtype->SetFocus();
         ModalResult = mrNone;
    }

    if(txtAmount->Text.Trim().Length()<3 )
    {
         MessageBox(Handle,"金额号不对!\t","提示",MB_OK|MB_ICONINFORMATION);
         txtAmount->SetFocus();
         ModalResult = mrNone;
    }
    if(txtExchno->Text.Trim().Length()<5 )
    {
         MessageBox(Handle,"交换号不对!\t","提示",MB_OK|MB_ICONINFORMATION);
         txtExchno->SetFocus();
         ModalResult = mrNone;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormBill::FormShow(TObject *Sender)
{
    SetCtrlStyle(txtVchtype,ES_NUMBER);
    SetCtrlStyle(txtAccno,ES_NUMBER);
    SetCtrlStyle(txtVchno,ES_NUMBER);
    SetCtrlStyle(txtExchno,ES_NUMBER);

    ZDateTime zdate;
    zdate.SetBankDateStr( FmIC->gdate);
    txtDate->Date = StrToDate(zdate.DateString().c_str());  
}
//---------------------------------------------------------------------------

void TFormBill::MoneyChange(TEdit * txtMoney)
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
void TFormBill::MoneyKeyPress(TEdit* txtMoney, char & Key)
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

void __fastcall TFormBill::txtAmountChange(TObject *Sender)
{
     MoneyChange( txtAmount );    
}
//---------------------------------------------------------------------------

void __fastcall TFormBill::txtAmountEnter(TObject *Sender)
{
     bPoint = false;    
}
//---------------------------------------------------------------------------

void __fastcall TFormBill::FormCreate(TObject *Sender)
{
     bFormatMoney = false;    
}
//---------------------------------------------------------------------------

void __fastcall TFormBill::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
     if(Key==13||Key==40||Key==34)  //回车 下箭头 pgdn
        PostMessage(Handle,WM_NEXTDLGCTL,0,0);        
}
//---------------------------------------------------------------------------


void __fastcall TFormBill::txtAmountKeyPress(TObject *Sender, char &Key)
{
     MoneyKeyPress( txtAmount, Key);        
}
//---------------------------------------------------------------------------

