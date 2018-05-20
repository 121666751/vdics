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

#include "mainimage.h"
#include <zcommon.h>
#include <transimg.h>
#include "login.h"
#include "more.h"
#include "main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMainImage *FormMainImage;
//---------------------------------------------------------------------------
__fastcall TFormMainImage::TFormMainImage(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMainImage::FormShow(TObject *Sender)
{
    image->Align = alClient;
    image->Left = 1;
    image->Top = 1;
    image->AutoSize = false;
    image->Tag = 1;
    SetCtrlStyle(txtVchtype,ES_NUMBER);    
}
//---------------------------------------------------------------------------
void __fastcall TFormMainImage::imageMouseUp(TObject *Sender,
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

    static double r1 = (double)p->Picture->Width/p->Width;
    static double r2 = (double)p->Picture->Height/p->Height;

    static double r =  (r1>r2)? r1 : r2;


    p->Align = alNone;
    p->Left = X - X*r;
    p->Top = Y - Y*r;
    p->AutoSize = true;
    p->Tag = 1;

}
//---------------------------------------------------------------------------



void __fastcall TFormMainImage::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
     if(Key==13||Key==40||Key==34)  //回车 下箭头 pgdn
        PostMessage(Handle,WM_NEXTDLGCTL,0,0);    
}
//---------------------------------------------------------------------------


void __fastcall TFormMainImage::btnSaveClick(TObject *Sender)
{
    if( txtVchtype->Text.IsEmpty() )
    {
        MessageBox(Handle,"凭证类型不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtVchtype->SetFocus();
        ModalResult = mrNone;
        return;
    } 
}
//---------------------------------------------------------------------------


void TFormMainImage::MoneyChange(TEdit * txtMoney)
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

void TFormMainImage::MoneyKeyPress(TEdit* txtMoney, char & Key)
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





void __fastcall TFormMainImage::FormCreate(TObject *Sender)
{
     bFormatMoney = false;    
}
//---------------------------------------------------------------------------

void __fastcall TFormMainImage::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    image->AutoSize = false;
    image->Align = alClient;
    image->Proportional = true;
    image->Stretch = true;
    image->Tag = 0;     
}
//---------------------------------------------------------------------------


