/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
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
    函数
    用来得到口袋号
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
    窗体创建事件函数
*/
void __fastcall TFmWiseIP::FormCreate(TObject *Sender)
{
    img = new TJPEGImage;
     bFormatMoney = false;

    //设置输入框只能输入数字
    SetCtrlStyle(txtVchNo, ES_NUMBER);
    SetCtrlStyle(txtNetNo, ES_NUMBER);
    SetCtrlStyle(txtAccount, ES_NUMBER);
    SetCtrlStyle(txtVchKind, ES_NUMBER);
  //  SetCtrlStyle(txtMoney, ES_NUMBER);
}
//---------------------------------------------------------------------------
/**
    窗体释放事件函数
    释放图象资源
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
    函数
    把图象缩放到合适的比例显示
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
    函数
    把信息显示出来
    @ DISTDATA * pdata-结构体，图象数据
    @ const char * fname-传入的参数，是清分机得到的图象文件名
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
         btnHistQuery->Caption="人行数据";
    }
    else
         btnHistQuery->Caption="历史数据";

    TimeLeft->Caption = "90";
    TimerDie->Enabled = true;
}
//---------------------------------------------------------------------------
/**
    函数
    用来把输入的数据保存到返回参数
    @ DISTDATA * pdata-结构体，保存要返回的参数
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
    函数
    对字符串进行格式化
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
    窗体键盘事件
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
    查询历史数据事件
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
    数据保存事件函数
    把输入的数据进行保存
*/
void __fastcall TFmWiseIP::btnOkClick(TObject *Sender)
{
    //检查数据的合法性
    if(txtVchNo->Text.Trim().Length()!=FmDistMan->Distval.vchLen)
    {
        MessageBox(Handle,"凭证号不对!\t","清分控制",MB_OK|MB_ICONINFORMATION);
        txtVchNo->SetFocus();
        return;
    }

    if( isNumeric(txtVchNo->Text.Trim().c_str()) == false)
    {
        MessageBox(Handle,"凭证号不对!\t","清分控制",MB_OK|MB_ICONINFORMATION);
        txtVchNo->SetFocus();
        return;
    }

    if(isNumeric(txtNetNo->Text.Trim().c_str()) == false || strncmp(txtNetNo->Text.Trim().c_str(),"00000000",5)==0 ||(txtNetNo->Text.Trim().Length()!=6))
    {
        MessageBox(Handle,"交换号不对!\t","清分控制",MB_OK|MB_ICONINFORMATION);
        txtNetNo->SetFocus();
        return;
    }

    //磁码帐号不一定是14位的
    if(txtAccount->Text.Trim().Length()<FmDistMan->Distval.accLen)
    {
        MessageBox(Handle,"帐号不对!\t","清分控制",MB_OK|MB_ICONINFORMATION);
        txtAccount->SetFocus();
        return;
    }

    if( isNumeric(txtAccount->Text.Trim().c_str()) == false)
    {
        MessageBox(Handle,"帐号不对!\t","清分控制",MB_OK|MB_ICONINFORMATION);
        txtAccount->SetFocus();
        return;
    }

    //金额
    if( txtMoney->Text.Trim().Length()<=0)
    {
        MessageBox(Handle,"金额不对!\t","清分控制",MB_OK|MB_ICONINFORMATION);
        txtMoney->SetFocus();
        return;
    }

    String amt =  RemoveThousandChar(txtMoney->Text.c_str());
    if( amt.ToDouble() <=0)
    {
        MessageBox(Handle,"金额不对!\t","清分控制",MB_OK|MB_ICONINFORMATION);
        txtMoney->SetFocus();
        return;
    }





    if( FmDistMan->CheckMoney(txtNetNo->Text.Trim().c_str(),txtVchKind->Text.Trim().c_str(),amt.c_str(),txtAccount->Text.Trim().c_str(),txtVchNo->Text.Trim().c_str()) ==false )
    {
        String info = AnsiString("交换号[")+txtNetNo->Text.Trim().c_str()+"]无输入信息的票据,是否继续?\t";
        if(MessageBox(Handle,info.c_str(),"清分控制",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO)
        {
            txtMoney->SetFocus();
            return;
        }
    }

    amt =  amt.Delete(amt.Pos("."),1);
    if( isNumeric(txtVchKind->Text.Trim().c_str()) == false)
    {
        MessageBox(Handle,"凭证种类不对!\t","清分控制",MB_OK|MB_ICONINFORMATION);
        txtVchKind->SetFocus();
        return;
    }

    if(txtVchKind->Text.Trim().Length()!=FmDistMan->Distval.vchtpLen)
    {
        MessageBox(Handle,"凭证种类不对!\t","清分控制",MB_OK|MB_ICONINFORMATION);
        txtVchKind->SetFocus();
        return;
    }

    //得到输入的数据
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
    函数
    检查字符串里是否都是数字
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
    金额编辑框变化响应函数
*/
void TFmWiseIP::MoneyChange(TEdit *txtMoney)
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
void __fastcall TFmWiseIP::txtMoneyChange(TObject *Sender)
{
     MoneyChange( txtMoney );    
}
//---------------------------------------------------------------------------

