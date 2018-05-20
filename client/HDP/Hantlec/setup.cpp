/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 北京天创征腾信息科技有限公司    版权所有
 *
 * 此文件为天创征腾软件产品的一部分。
 * 天创征腾软件产品的所有部分,包括但不限于软件产品中所含的任何图象、照片、
 * 动画、文字和附加程序（applets）、加密卡、随附的印刷材料、源代码文件及
 * 本软件产品的任何副本的一切所有权和知识产权，均归版权人天创征腾拥有。
 *
 * 文件：	$Id: setup.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
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
    构造函数
*/
__fastcall TSetupDlg::TSetupDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
/**
    窗体键盘事件
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
    保存配置信息事件函数
*/
void __fastcall TSetupDlg::BtnSaveClick(TObject *Sender)
{
    if( access(txtPath->Text.c_str(),0) )
    {
    	MessageBox(Handle,"路径设置不正确!","错误",MB_OK+MB_ICONERROR);
		ModalResult = mrNone;
    }
    else
    {
    	ModalResult = mrOk;
    }
}
//---------------------------------------------------------------------------

/**
    主窗口显示函数
*/
void __fastcall TSetupDlg::FormShow(TObject *Sender)
{
    //设置输入框只能输入数字
    SetCtrlStyle(txtVchLen , ES_NUMBER);
    SetCtrlStyle(txtExnoLen, ES_NUMBER);
    SetCtrlStyle(txtAccLen, ES_NUMBER);
    SetCtrlStyle(txtVchtpLen, ES_NUMBER);
    SetCtrlStyle(txtAmountLen, ES_NUMBER);
}
//---------------------------------------------------------------------------

/**
    路径选择按钮点击响应函数
*/
void __fastcall TSetupDlg::SpeedButton1Click(TObject *Sender)
{
    AnsiString S;
    S = "";
    if (SelectDirectory("浏览文件夹", "", S))  txtPath->Text = S;

    SetCurrentDirectory(FmDistMan->originPath.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TSetupDlg::btnOkClick(TObject *Sender)
{
    //更改参数需要授权
   /* MessageBox(Handle,"改变参数需联社管理员授权\t","提示",MB_OK|MB_ICONINFORMATION);

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


