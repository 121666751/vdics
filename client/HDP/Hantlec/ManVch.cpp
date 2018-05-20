/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
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
    ���캯��
*/
__fastcall TManVchAdd::TManVchAdd(TComponent* Owner,DISTDATA* pData)
        : TForm(Owner)
{
    addData = pData;
}
//---------------------------------------------------------------------------
/**
    �����¼�����
*/
void __fastcall TManVchAdd::okClick(TObject *Sender)
{
    if(vchmoney->Text.ToInt()<10.0)
    {
        MessageBox(Handle,"���̫����","��ʾ",MB_OK|MB_ICONINFORMATION);
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
    ƾ֤�ű༭��������Ӧ����
*/
void __fastcall TManVchAdd::vchnoKeyPress(TObject *Sender, char &Key)
{
    //����ǻس���,�Զ��л�doa�����ű༭��
    if(Key=='\r')
    {
        //���ȼ��
        if(vchno->Text.Trim().Length()!=FmDistMan->Distval.vchLen)
        {
            MessageBox(Handle,"ƾ֤��������","��ʾ",MB_OK|MB_ICONINFORMATION);
            return ;
        }
        exchno->SetFocus();
    }
}
//---------------------------------------------------------------------------

/**
    �����ű༭��������Ӧ����
*/
void __fastcall TManVchAdd::exchnoKeyPress(TObject *Sender, char &Key)
{
    //�������س���,�Զ��л����ʺű༭��
    if(Key=='\r')
        accno->SetFocus();
}
//---------------------------------------------------------------------------

/**
    �ʺű༭��������Ӧ����
*/
void __fastcall TManVchAdd::accnoKeyPress(TObject *Sender, char &Key)
{
    //��������ǻس���,�Զ��л���ƾ֤����༭��
    if(Key=='\r')
    {
        //���ȼ��
        if(accno->Text.Trim().Length()!=FmDistMan->Distval.accLen)
        {
            MessageBox(Handle,"�ʺ�����","��ʾ",MB_OK|MB_ICONINFORMATION);
            return;
        }
        vchtype->SetFocus();
    }
}
//---------------------------------------------------------------------------
void __fastcall TManVchAdd::vchtypeKeyPress(TObject *Sender, char &Key)
{
    //���ƾ֤���͸�ʽ�Ƿ�������ȷ
    if(Key=='\r')
    {
        //���ȼ��
        if(vchtype->Text.Trim().Length()!=FmDistMan->Distval.vchtpLen)
        {
            MessageBox(Handle,"ƾ֤��������","��ʾ",MB_OK|MB_ICONINFORMATION);
            return;
        }
        vchmoney->SetFocus();
    }
}
//---------------------------------------------------------------------------

/**
    ���༭��������Ӧ����
*/
void __fastcall TManVchAdd::vchmoneyKeyPress(TObject *Sender, char &Key)
{
    //�������س���,���Զ��л���ȷ����ť
    if(Key=='\r')
    {
        ok->SetFocus();
    }
}
//---------------------------------------------------------------------------
/**
    ������ʾ�¼�����
*/
void __fastcall TManVchAdd::FormShow(TObject *Sender)
{
    //���������ֻ����������
    vchno->SetFocus();
    SetCtrlStyle(vchno,ES_NUMBER);
    SetCtrlStyle(exchno,ES_NUMBER);
    SetCtrlStyle(accno,ES_NUMBER);
    SetCtrlStyle(vchtype,ES_NUMBER);
    SetCtrlStyle(vchmoney,ES_NUMBER);
}
//---------------------------------------------------------------------------

