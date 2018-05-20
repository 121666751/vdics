/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 �����촴������Ϣ�Ƽ����޹�˾    ��Ȩ����
 *
 * ���ļ�Ϊ�촴���������Ʒ��һ���֡�
 * �촴���������Ʒ�����в���,�����������������Ʒ���������κ�ͼ����Ƭ��
 * ���������ֺ͸��ӳ���applets�������ܿ����渽��ӡˢ���ϡ�Դ�����ļ���
 * �������Ʒ���κθ�����һ������Ȩ��֪ʶ��Ȩ�������Ȩ���촴����ӵ�С�
 *
 * �ļ���	$Id: setup.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//---------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <io.h>
#include <bcbext.h>
#include <filectrl.hpp>
#include "main.h"
#include "setup.h"

#include <ztpbm.h>
#include <commonresource.h>
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TSetupDlg *SetupDlg;
//---------------------------------------------------------------------
/**
    ���캯��
*/
__fastcall TSetupDlg::TSetupDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
/**
    ��������¼�
*/
void __fastcall TSetupDlg::FormKeyPress(TObject *Sender, char &Key)
{
    if( Key == 13 )
    {
        PostMessage(Handle,WM_NEXTDLGCTL,0,0);
    }
}
//---------------------------------------------------------------------------
/**
    ����������Ϣ�¼�����
*/
void __fastcall TSetupDlg::BtnSaveClick(TObject *Sender)
{
    if( access(txtPath->Text.c_str(),0) )
    {
    	MessageBox(Handle,"·�����ò���ȷ!","����",MB_OK+MB_ICONERROR);
		ModalResult = mrNone;
    }
    else
    {
    	ModalResult = mrOk;
    }
}
//---------------------------------------------------------------------------

/**
    ��������ʾ����
*/
void __fastcall TSetupDlg::FormShow(TObject *Sender)
{
    //���������ֻ����������
    SetCtrlStyle(txtVchLen , ES_NUMBER);
    SetCtrlStyle(txtExnoLen, ES_NUMBER);
    SetCtrlStyle(txtAccLen, ES_NUMBER);
    SetCtrlStyle(txtVchtpLen, ES_NUMBER);
    SetCtrlStyle(txtAmountLen, ES_NUMBER);
}
//---------------------------------------------------------------------------

/**
    ·��ѡ��ť�����Ӧ����
*/
void __fastcall TSetupDlg::SpeedButton1Click(TObject *Sender)
{
    AnsiString S;
    S = "";
    if (SelectDirectory("����ļ���", "", S))  txtPath->Text = S;

    SetCurrentDirectory(FmDistMan->originPath.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TSetupDlg::btnOkClick(TObject *Sender)
{
    //���Ĳ�����Ҫ��Ȩ
   /* MessageBox(Handle,"�ı�������������Ա��Ȩ\t","��ʾ",MB_OK|MB_ICONINFORMATION);

    char _outputLogin[12];
    memset(&_outputLogin,0,sizeof(_outputLogin));

    SetCurrentDirectory(FmDistMan->originPath.c_str());
    int ret = zt_pub_warrant(zt_pub_getDepno(),"system_setup_p",  _outputLogin);
    if(ret)
    {
        return ;
    }*/
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TSetupDlg::BitBtn2Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------


