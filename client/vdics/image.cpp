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

#include "image.h"
#include "single.h"
#include "main.h"
#include <zcommon.h>
#include <transimg.h>
#include "login.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormImage *FormImage;
//---------------------------------------------------------------------------
__fastcall TFormImage::TFormImage(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormImage::FormShow(TObject *Sender)
{
    image->Align = alClient;
    image->Left = 1;
    image->Top = 1;
    image->AutoSize = false;
    image->Tag = 1;
    txtAccno->SetFocus();

    SetCtrlStyle(txtAccno,ES_NUMBER);
    SetCtrlStyle(txtVchno,ES_NUMBER);
    SetCtrlStyle(txtVchtype,ES_NUMBER);   
}
//---------------------------------------------------------------------------
void __fastcall TFormImage::imageMouseUp(TObject *Sender,
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



void __fastcall TFormImage::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Key==13||Key==40||Key==34)  //�س� �¼�ͷ pgdn
        PostMessage(Handle,WM_NEXTDLGCTL,0,0);
}
//---------------------------------------------------------------------------



void __fastcall TFormImage::btnSaveClick(TObject *Sender)
{
    if( txtAmount->Text.IsEmpty() )
    {
        MessageBox(Handle,"����Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtAmount->SetFocus();
        ModalResult = mrNone;
        return;
    }

    if( txtAccno->Text.IsEmpty() )
    {
        MessageBox(Handle,"�˺Ų���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtAccno->SetFocus();
        ModalResult = mrNone;
        return;
    }

    if( txtVchno->Text.IsEmpty() )
    {
        MessageBox(Handle,"ƾ֤�Ų���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtVchno->SetFocus();
        ModalResult = mrNone;
        return;
    }

    if( txtVchtype->Text.IsEmpty() )
    {
        MessageBox(Handle,"ƾ֤���Ͳ���Ϊ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtVchtype->SetFocus();
        ModalResult = mrNone;
        return;
    }

    int modeflag = chkMode->Checked ?1:0;
    int ret =  FmIC->gdb.VQuery(512,"update vouchers set accno='%s',vchno='%s',vchtype='%s',amount='%s',state=1 ,clkno='%s' ,modeflag=%d where sid=%d",txtAccno->Text.c_str(),txtVchno->Text.c_str(),txtVchtype->Text.c_str(),RemoveThousandChar(txtAmount->Text.c_str()),g_sys_opt.c_str(),modeflag,curID);

	debugout("update vouchers set accno='%s',vchno='%s',vchtype='%s',amount='%s',state=1 ,clkno='%s' ,modeflag=%d where sid=%d",txtAccno->Text.c_str(),txtVchno->Text.c_str(),txtVchtype->Text.c_str(),RemoveThousandChar(txtAmount->Text.c_str()),g_sys_opt.c_str(),modeflag,curID);
	
	if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
        ModalResult = mrNone;
        return;
    }
    FmVoucher->mergeOne(curID,txtAccno->Text.c_str(),txtVchno->Text.c_str(),txtVchtype->Text.c_str(),RemoveThousandChar(txtAmount->Text.c_str()).c_str(),"","","","",0,FmVoucher->txtDate->Text.c_str(),FmVoucher->txtSession->Text.c_str());

    if(FmVoucher->txtDetail->Items->Count>0 && (FmVoucher->txtIdentify->ItemIndex == 3 || FmVoucher->txtIdentify->ItemIndex == 1)  )
    {
        ret =  FmVoucher->getNext();
        if(ret)
            ModalResult = mrOk;
        else
            ModalResult = mrNone;
    }
}
//---------------------------------------------------------------------------


void TFormImage::MoneyChange(TEdit * txtMoney)
{
    //TODO: Add your source code here
    if( txtMoney->Text.IsEmpty() ) txtMoney->Text = "0";
    if( bFormatMoney ) return;
    bFormatMoney = true;

    txtMoney->Text = RemoveThousandChar( txtMoney->Text );
    txtMoney->Text = AddThousandChar( txtMoney->Text );

    //�Զ���С����
    txtMoney->SelStart = txtMoney->Text.Pos(".")-1+nPopos;
    txtMoney->SelLength = 0;

    bFormatMoney = false;
}

void TFormImage::MoneyKeyPress(TEdit* txtMoney, char & Key)
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
void __fastcall TFormImage::txtAmountChange(TObject *Sender)
{
    MoneyChange( txtAmount );    
}
//---------------------------------------------------------------------------

void __fastcall TFormImage::txtAmountEnter(TObject *Sender)
{
     bPoint = false;    
}
//---------------------------------------------------------------------------

void __fastcall TFormImage::txtAmountKeyPress(TObject *Sender, char &Key)
{
    MoneyKeyPress( txtAmount, Key);    
}
//---------------------------------------------------------------------------

void __fastcall TFormImage::FormCreate(TObject *Sender)
{
     bFormatMoney = false;    
}
//---------------------------------------------------------------------------

void __fastcall TFormImage::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    image->AutoSize = false;
    image->Align = alClient;
    image->Proportional = true;
    image->Stretch = true;
    image->Tag = 0;         
}
//---------------------------------------------------------------------------

