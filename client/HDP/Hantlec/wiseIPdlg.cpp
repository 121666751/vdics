/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//-------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "wiseIPdlg.h"
#include "main.h"
#include "wait.h"
#include <dstcommon.h>
#include <zvchinfodef.h>
#include <bcbext.h>
#include "histquery.h"
#include "main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmWiseIP *FmWiseIP;
//---------------------------------------------------------------------------
__fastcall TFmWiseIP::TFmWiseIP(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
/**
    ����
    �����õ��ڴ���
*/
int TFmWiseIP::GetPocketNum()
{
    //TODO: Add your source code here
    int pocket = FmDistMan->GetPocketNum();
    Application->ProcessMessages();
    if( pocket > 0 ) return pocket;
    return ERROR_POCKET;
}
//---------------------------------------------------------------------------
/**
    ���崴���¼�����
*/
void __fastcall TFmWiseIP::FormCreate(TObject *Sender)
{
    img = new TJPEGImage;
     bFormatMoney = false;

    //���������ֻ����������
    SetCtrlStyle(txtVchNo, ES_NUMBER);
    SetCtrlStyle(txtNetNo, ES_NUMBER);
    SetCtrlStyle(txtAccount, ES_NUMBER);
    SetCtrlStyle(txtVchKind, ES_NUMBER);
  //  SetCtrlStyle(txtMoney, ES_NUMBER);
}
//---------------------------------------------------------------------------
/**
    �����ͷ��¼�����
    �ͷ�ͼ����Դ
*/
void __fastcall TFmWiseIP::FormDestroy(TObject *Sender)
{
    delete img;
}
//---------------------------------------------------------------------------
void __fastcall TFmWiseIP::FormShow(TObject *Sender)
{
    txtVchNo->SetFocus();
   
}
//---------------------------------------------------------------------------
/**
    ����
    ��ͼ�����ŵ����ʵı�����ʾ
*/
void TFmWiseIP::setVchImgSize(bool bStrech)
{
    //TODO: Add your source code here
    if( !img->Empty )
    {
        if( bStrech )
        {
            VchImage->Width = VchScrollBox->Width-4;
            VchImage->Height = VchScrollBox->Height-4;

            int a = img->Width*VchImage->Height;
            int b = img->Height*VchImage->Width;

            if(      a > b && b > 0 ) VchImage->Height = (int)( b/img->Width);
            else if( b > a && a > 0 ) VchImage->Width = (int)( a/img->Height);
        }
        else if( VchImage->Width < VchScrollBox->Width && VchImage->Height < VchScrollBox->Height )
        {
            VchImage->Width = img->Width;
            VchImage->Height = img->Height;
        }
    }
}
//---------------------------------------------------------------------------
/**
    ����
    ����Ϣ��ʾ����
    @ DISTDATA * pdata-�ṹ�壬ͼ������
    @ const char * fname-����Ĳ���������ֻ��õ���ͼ���ļ���
*/
void TFmWiseIP::SetResult(DISTDATA * pdata,const char * fname)
{
    VchImage->Picture->LoadFromFile(fname);

    txtVchNo->MaxLength = FmDistMan->Distval.vchLen;
    txtAccount->MaxLength = FmDistMan->Distval.accLen;
    txtMoney->MaxLength = FmDistMan->Distval.amountLen+4;
    txtNetNo->MaxLength = 6;
    
    txtVchNo->Text = pdata->vchno;
    txtNetNo->Text = FmDistMan->txt1PkgNo->Caption;
    txtAccount->Text = pdata->codeacc;

    char tmp[3];
    memset(tmp,0,sizeof(tmp));

    txtVchKind->Text = pdata->pbcvt;

    String b = pdata->amount;

    if(b.Length()<=0) b = "0";

    char bmoney[24];
    memset(bmoney,0,sizeof(bmoney));
    sprintf(bmoney,"%.2f",b.ToDouble()/100.00);
    
    txtMoney->Text = bmoney;

    txtMicrCode->Text = pdata->codeline;
    txtOcrCode->Text = pdata->ocr;

    txtVchNo->Text = txtVchNo->Text.Trim().c_str();
    txtAccount->Text = txtAccount->Text.Trim().c_str();
    txtNetNo->Text =  txtNetNo->Text.Trim().c_str();
    txtVchKind->Text = txtVchKind->Text.Trim().c_str();
    txtMoney->Text = txtMoney->Text.Trim().c_str();     

    if(FmDistMan->_nDstMode == MD_FIRST )
    {
         btnHistQuery->Caption="��������";
    }
    else
         btnHistQuery->Caption="��ʷ����";

    TimeLeft->Caption = "90";
    TimerDie->Enabled = true;
}
//---------------------------------------------------------------------------
/**
    ����
    ��������������ݱ��浽���ز���
    @ DISTDATA * pdata-�ṹ�壬����Ҫ���صĲ���
*/
void TFmWiseIP::GetResult(DISTDATA * pdata)
{
    //TODO: Add your source code here
    if( txtVchNo->Text.IsEmpty() )
    {
        strcpy( pdata->vchno, "000000" );
    }
    else
    {
        strcpy( pdata->vchno, txtVchNo->Text.c_str() );
    }
    if( txtNetNo->Text.IsEmpty() )
    {
        strcpy( pdata->exchno, "000000" );
    }
    else
    {
        strcpy( pdata->exchno, txtNetNo->Text.c_str() );
    }
    strcpy( pdata->codeacc, txtAccount->Text.c_str() );
    strcpy( pdata->pbcvt, txtVchKind->Text.c_str() );
    strcpy( pdata->amount , txtMoney->Text.c_str() );
}
//---------------------------------------------------------------------------
/**
    ����
    ���ַ������и�ʽ��
*/
void TFmWiseIP::makeBSTRStr(String & str, int tagLen)
{
    //TODO: Add your source code here
    int lt = str.Length();
    if( lt >= tagLen )
    {
        str = str.SubString(1,tagLen);
    }
    else
    {
        for( int i = 0; i< (tagLen-lt); i++)
        {
            str = str + " ";
        }
    }
}
//---------------------------------------------------------------------------
/**
    ��������¼�
*/
void __fastcall TFmWiseIP::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    switch( (int)Key )
    {
        case 13:
            PostMessage( Handle, WM_NEXTDLGCTL, 0, 0);
            break;
        case 27:
            ModalResult = mrCancel;
            break;
        case 33:
            PostMessage( Handle, WM_NEXTDLGCTL, 1, 0);
            break;
        case 'Q':
        case 'q':
            btnHistQueryClick(0);
            break;
    }
}
//---------------------------------------------------------------------------
/**
    ��ѯ��ʷ�����¼�
*/
void __fastcall TFmWiseIP::btnHistQueryClick(TObject *Sender)
{
    THistQueryDlg* dlg = new THistQueryDlg(this);
    dlg->ShowModal();

    TListItem* li = dlg->VchList->Selected;
    if( li )
    {
        txtVchNo->Text = li->Caption;
        txtNetNo->Text = li->SubItems->Strings[0];
        txtAccount->Text = li->SubItems->Strings[1];
        txtVchKind->Text = li->SubItems->Strings[2];
        String amount = li->SubItems->Strings[3];
     //   amount.Delete(amount.Pos("."),1);
        txtMoney->Text = amount;
    }

    delete dlg;
}
//---------------------------------------------------------------------------
/**
    ���ݱ����¼�����
    ����������ݽ��б���
*/
void __fastcall TFmWiseIP::btnOkClick(TObject *Sender)
{
    //������ݵĺϷ���
    if(txtVchNo->Text.Trim().Length()!=FmDistMan->Distval.vchLen)
    {
        MessageBox(Handle,"ƾ֤�Ų���!\t","��ֿ���",MB_OK|MB_ICONINFORMATION);
        txtVchNo->SetFocus();
        return;
    }

    if( isNumeric(txtVchNo->Text.Trim().c_str()) == false)
    {
        MessageBox(Handle,"ƾ֤�Ų���!\t","��ֿ���",MB_OK|MB_ICONINFORMATION);
        txtVchNo->SetFocus();
        return;
    }

    if(isNumeric(txtNetNo->Text.Trim().c_str()) == false || strncmp(txtNetNo->Text.Trim().c_str(),"00000000",5)==0 ||(txtNetNo->Text.Trim().Length()!=6))
    {
        MessageBox(Handle,"�����Ų���!\t","��ֿ���",MB_OK|MB_ICONINFORMATION);
        txtNetNo->SetFocus();
        return;
    }

    //�����ʺŲ�һ����14λ��
    if(txtAccount->Text.Trim().Length()<FmDistMan->Distval.accLen)
    {
        MessageBox(Handle,"�ʺŲ���!\t","��ֿ���",MB_OK|MB_ICONINFORMATION);
        txtAccount->SetFocus();
        return;
    }

    if( isNumeric(txtAccount->Text.Trim().c_str()) == false)
    {
        MessageBox(Handle,"�ʺŲ���!\t","��ֿ���",MB_OK|MB_ICONINFORMATION);
        txtAccount->SetFocus();
        return;
    }

    //���
    if( txtMoney->Text.Trim().Length()<=0)
    {
        MessageBox(Handle,"����!\t","��ֿ���",MB_OK|MB_ICONINFORMATION);
        txtMoney->SetFocus();
        return;
    }

    String amt =  RemoveThousandChar(txtMoney->Text.c_str());
    if( amt.ToDouble() <=0)
    {
        MessageBox(Handle,"����!\t","��ֿ���",MB_OK|MB_ICONINFORMATION);
        txtMoney->SetFocus();
        return;
    }





    if( FmDistMan->CheckMoney(txtNetNo->Text.Trim().c_str(),txtVchKind->Text.Trim().c_str(),amt.c_str(),txtAccount->Text.Trim().c_str(),txtVchNo->Text.Trim().c_str()) ==false )
    {
        String info = AnsiString("������[")+txtNetNo->Text.Trim().c_str()+"]��������Ϣ��Ʊ��,�Ƿ����?\t";
        if(MessageBox(Handle,info.c_str(),"��ֿ���",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO)
        {
            txtMoney->SetFocus();
            return;
        }
    }

    amt =  amt.Delete(amt.Pos("."),1);
    if( isNumeric(txtVchKind->Text.Trim().c_str()) == false)
    {
        MessageBox(Handle,"ƾ֤���಻��!\t","��ֿ���",MB_OK|MB_ICONINFORMATION);
        txtVchKind->SetFocus();
        return;
    }

    if(txtVchKind->Text.Trim().Length()!=FmDistMan->Distval.vchtpLen)
    {
        MessageBox(Handle,"ƾ֤���಻��!\t","��ֿ���",MB_OK|MB_ICONINFORMATION);
        txtVchKind->SetFocus();
        return;
    }

    //�õ����������
    strcpy(FmDistMan->pData->vchno,   txtVchNo->Text.Trim().c_str());
    strcpy(FmDistMan->pData->exchno,  txtNetNo->Text.Trim().c_str());
    strcpy(FmDistMan->pData->codeacc, txtAccount->Text.Trim().c_str());
    strcpy(FmDistMan->pData->pbcvt, txtVchKind->Text.Trim().c_str());

    strcpy(FmDistMan->pData->amount,amt.c_str());

    Close();
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
/**
    ����
    ����ַ������Ƿ�������
*/
bool TFmWiseIP::isNumeric(const char * str)
{
    //TODO: Add your source code here
    String tmpstr = str;
    int Len =  tmpstr.Length();      
    for(int i=0;i<Len;i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

void __fastcall TFmWiseIP::TimerDieTimer(TObject *Sender)
{
    if( TimeLeft->Visible == true)
     {
        int r = TimeLeft->Caption.Trim().ToInt();
        r = r-1;
        TimeLeft->Caption = r;

        if(r<0)
        {
            Close();
        }
     }
     TimeLeft->Visible = !TimeLeft->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFmWiseIP::FormClose(TObject *Sender, TCloseAction &Action)
{
     TimerDie->Enabled = false;    
}
//---------------------------------------------------------------------------




void __fastcall TFmWiseIP::txtMoneyKeyPress(TObject *Sender, char &Key)
{
    MoneyKeyPress( txtMoney, Key);    
}
//---------------------------------------------------------------------------

void __fastcall TFmWiseIP::txtMoneyEnter(TObject *Sender)
{
     bPoint = false;    
}
//---------------------------------------------------------------------------
void TFmWiseIP::MoneyKeyPress(TEdit* txtMoney, char & Key)
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

/**
    ���༭��仯��Ӧ����
*/
void TFmWiseIP::MoneyChange(TEdit *txtMoney)
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
void __fastcall TFmWiseIP::txtMoneyChange(TObject *Sender)
{
     MoneyChange( txtMoney );    
}
//---------------------------------------------------------------------------

