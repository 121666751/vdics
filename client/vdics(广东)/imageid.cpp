//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "imageid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormImageID *FormImageID;
//---------------------------------------------------------------------------
__fastcall TFormImageID::TFormImageID(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormImageID::btnOKClick(TObject *Sender)
{
    if(txtImageID->Text.Trim().Length()<10)
    {
        MessageBox(Handle,"Ӱ��ID����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        ModalResult = mrNone;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormImageID::FormShow(TObject *Sender)
{
    txtImageID->Text = "";
    txtImageID->SetFocus();
}
//---------------------------------------------------------------------------
