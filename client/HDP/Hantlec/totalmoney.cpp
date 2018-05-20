/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//--------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TotalMoney.h"
#include <bcbext.h>
#include "main.h"
#include <zvchinfodef.h>
#include "ConfigFile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmTotalMoney *FmTotalMoney;
//---------------------------------------------------------------------------
__fastcall TFmTotalMoney::TFmTotalMoney(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
/**
    借方金额输入框信息函数
    如果输入的信息发生变化，对输入的数字进行格式处理
*/
void __fastcall TFmTotalMoney::txtTotalMoneyJFChange(TObject *Sender)
{
    MoneyChange( txtTotalMoneyJF );
}
//---------------------------------------------------------------------------
void __fastcall TFmTotalMoney::txtTotalMoneyJFKeyPress(TObject *Sender,
      char &Key)
{
    MoneyKeyPress( txtTotalMoneyJF, Key);
}
//---------------------------------------------------------------------------
void __fastcall TFmTotalMoney::FormCreate(TObject *Sender)
{
    bFormatMoney = false;

    //设置输入框的格式和字符类型
    SetCtrlStyle( txtTotalMoneyJF, ES_RIGHT );
    SetCtrlStyle( txtTotalMoneyDF, ES_RIGHT );
    SetCtrlStyle(txtpkgno, ES_NUMBER);
}
//---------------------------------------------------------------------------
/**
    窗体显示事件函数
    打开数据库，查询并显示网点号
*/
void __fastcall TFmTotalMoney::FormShow(TObject *Sender)
{
    txtTotalMoneyJF->SelectAll();
    txtpkgno->SetFocus();
    FmDistMan->InsertLog("修改金额");
    FmDistMan->InsertLog(txtTotalMoneyJF->Text.c_str());
    FmDistMan->InsertLog(txtTotalMoneyDF->Text.c_str());

    initNetno();
    txtpkgnoSelect(NULL);
    if(mode == 1) txtpkgno->Text = FmDistMan->curPkgNo;
}
//---------------------------------------------------------------------------

/**
    金额编辑框变化响应函数
*/
void TFmTotalMoney::MoneyChange(TEdit *txtMoney)
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
//---------------------------------------------------------------------------

/*
    金额编辑框输入响应函数
*/
void TFmTotalMoney::MoneyKeyPress(TEdit* txtMoney, char & Key)
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
void __fastcall TFmTotalMoney::txtTotalMoneyJFEnter(TObject *Sender)
{
    bPoint = false;
}
//---------------------------------------------------------------------------
/**
    贷方金额输入框信息函数
    如果输入的信息发生变化，对输入的数字进行格式处理
*/
void __fastcall TFmTotalMoney::txtTotalMoneyDFChange(TObject *Sender)
{
    MoneyChange( txtTotalMoneyDF );
}
//---------------------------------------------------------------------------
void __fastcall TFmTotalMoney::txtTotalMoneyDFKeyPress(TObject *Sender,
      char &Key)
{
    MoneyKeyPress( txtTotalMoneyDF, Key);
}
//---------------------------------------------------------------------------
void __fastcall TFmTotalMoney::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    FmDistMan->InsertLog("修改金额结果");
    FmDistMan->InsertLog(txtTotalMoneyJF->Text.c_str());
    FmDistMan->InsertLog(txtTotalMoneyDF->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFmTotalMoney::FormKeyDown(TObject *Sender, WORD &Key,
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
    }
}
//---------------------------------------------------------------------------
/**
    函数
    用来初始化网点号下拉列表
*/
int TFmTotalMoney::initNetno()
{
    //TODO: Add your source code here
    txtpkgno->Items->Clear();
    //查询可以清分的网点号
    int ret = FmDistMan->gdb.VQuery(512,"select distinct exchno from bocctrl where date='%s' and session='%s'",FmDistMan->date,FmDistMan->session);
    if(ret)
    {
        MessageBox(Handle,"查询数据库出错!\t","错误",MB_OK|MB_ICONERROR);
        return -2;
    }
    MyRes res = FmDistMan->gdb.Store();
    MyRow row = res.Fetch();
    while(row!=NULL)
    {

        txtpkgno->Items->Add( row[0] );
        row=res.Fetch();
    }
    txtpkgno->ItemIndex = 0;
    return 0;
}
//---------------------------------------------------------------------------

/**
    交换号下拉框下拉响应函数
*/
void __fastcall TFmTotalMoney::txtpkgnoDropDown(TObject *Sender)
{
//     initNetno();
     txtpkgnoSelect(NULL);
}
//---------------------------------------------------------------------------
/**
    选中下拉列表的事件函数函数
    选中网点号后，查询借贷方金额并显示
*/
void __fastcall TFmTotalMoney::txtpkgnoSelect(TObject *Sender)
{
    int ret =  FmDistMan->gdb.VQuery(512,"select dramount,cramount from bocctrl where exchno='%s' and date='%s' and session='%s'",txtpkgno->Text.Trim().c_str(),FmDistMan->date,FmDistMan->session );
    if(ret)
    {
        MessageBox(Handle,"查询数据库出错!\t","错误",MB_OK|MB_ICONERROR);
        return ;
    }
    MyRes res = FmDistMan->gdb.Store();
    MyRow row = res.Fetch();
    if(row!=NULL)
    {
        txtTotalMoneyJF->Text = row[0];
        txtTotalMoneyDF->Text = row[1];
    }
}
//---------------------------------------------------------------------------


