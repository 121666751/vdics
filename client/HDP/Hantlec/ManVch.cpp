/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//----------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "bcbext.h"
#include "ManVch.h"
#include "main.h"
#include "stdio.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TManVchAdd *ManVchAdd;
//---------------------------------------------------------------------------
/**
    构造函数
*/
__fastcall TManVchAdd::TManVchAdd(TComponent* Owner,DISTDATA* pData)
        : TForm(Owner)
{
    addData = pData;
}
//---------------------------------------------------------------------------
/**
    保存事件函数
*/
void __fastcall TManVchAdd::okClick(TObject *Sender)
{
    if(vchmoney->Text.ToInt()<10.0)
    {
        MessageBox(Handle,"金额太少了","提示",MB_OK|MB_ICONINFORMATION);
        vchmoney->SetFocus();
        return;
    }
    if(strlen(vchno->Text.Trim().c_str())>0)
        sprintf(addData->vchno,"%06s",vchno->Text.Trim().c_str());
    else
        strcpy(addData->vchno,"000000");
    if(strlen( accno->Text.Trim().c_str())>0)
        sprintf( addData->codeacc,"%012s",accno->Text.Trim().c_str());
    else
        strcpy(addData->codeacc,"000000000000");
    if(strlen(vchtype->Text.Trim().c_str())<=0)
    {
        strcpy(addData->pbcvt,"00");
    }
    else
    {
        strcpy(addData->pbcvt,vchtype->Text.Trim().c_str());
    }
    sprintf(addData->amount,"%010s" ,vchmoney->Text.Trim().c_str());
    sprintf(addData->exchno,"%08s",exchno->Text.Trim().c_str());

    ModalResult = mrOk;
}
//---------------------------------------------------------------------------

/**
    凭证号编辑框输入响应函数
*/
void __fastcall TManVchAdd::vchnoKeyPress(TObject *Sender, char &Key)
{
    //如果是回车符,自动切换doa交换号编辑框
    if(Key=='\r')
    {
        //长度检查
        if(vchno->Text.Trim().Length()!=FmDistMan->Distval.vchLen)
        {
            MessageBox(Handle,"凭证号码有误","提示",MB_OK|MB_ICONINFORMATION);
            return ;
        }
        exchno->SetFocus();
    }
}
//---------------------------------------------------------------------------

/**
    交换号编辑框输入相应函数
*/
void __fastcall TManVchAdd::exchnoKeyPress(TObject *Sender, char &Key)
{
    //如果输入回车符,自动切换到帐号编辑框
    if(Key=='\r')
        accno->SetFocus();
}
//---------------------------------------------------------------------------

/**
    帐号编辑框输入响应函数
*/
void __fastcall TManVchAdd::accnoKeyPress(TObject *Sender, char &Key)
{
    //如果输入是回车符,自动切换到凭证种类编辑框
    if(Key=='\r')
    {
        //长度检查
        if(accno->Text.Trim().Length()!=FmDistMan->Distval.accLen)
        {
            MessageBox(Handle,"帐号有误","提示",MB_OK|MB_ICONINFORMATION);
            return;
        }
        vchtype->SetFocus();
    }
}
//---------------------------------------------------------------------------
void __fastcall TManVchAdd::vchtypeKeyPress(TObject *Sender, char &Key)
{
    //检查凭证类型格式是否输入正确
    if(Key=='\r')
    {
        //长度检查
        if(vchtype->Text.Trim().Length()!=FmDistMan->Distval.vchtpLen)
        {
            MessageBox(Handle,"凭证类型有误","提示",MB_OK|MB_ICONINFORMATION);
            return;
        }
        vchmoney->SetFocus();
    }
}
//---------------------------------------------------------------------------

/**
    金额编辑框输入响应函数
*/
void __fastcall TManVchAdd::vchmoneyKeyPress(TObject *Sender, char &Key)
{
    //如果输入回车符,则自动切换到确定按钮
    if(Key=='\r')
    {
        ok->SetFocus();
    }
}
//---------------------------------------------------------------------------
/**
    窗体显示事件函数
*/
void __fastcall TManVchAdd::FormShow(TObject *Sender)
{
    //设置输入框只能输入数字
    vchno->SetFocus();
    SetCtrlStyle(vchno,ES_NUMBER);
    SetCtrlStyle(exchno,ES_NUMBER);
    SetCtrlStyle(accno,ES_NUMBER);
    SetCtrlStyle(vchtype,ES_NUMBER);
    SetCtrlStyle(vchmoney,ES_NUMBER);
}
//---------------------------------------------------------------------------

