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
 * 文件：	$Id: setup.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//--------------------------------------------------------------
#ifndef setupH
#define setupH
//----------------------------------------------------------------------------
/*
 * Copyrights (c) 2006 北京紫枫科技开发有限公司软件产品开发部
 * 
 * 版本 0.2
 * 
 * 作者：聂建军
 *
 * 摘要：配置信息管理界面，用来修改配置信息
 *
 * 注释：杜永刚
 *
 * 完成日期：2006年2月
 */
//---------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
//----------------------------------------------------------------------------
class TSetupDlg : public TForm
{
__published:
    TPanel *Panel1;
    TLabel *Label1;
    TGroupBox *GroupBox1;
    TLabel *Label13;
    TEdit *txtPath;
    TSpeedButton *SpeedButton1;
    TEdit *txtPrefix;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *txtCalX;
    TEdit *txtCalY;
    TGroupBox *GroupBox2;
    TLabel *Label4;
    TEdit *txtDPI;
    TLabel *label222;
    TEdit *txtVchLen;
    TLabel *Label5;
    TEdit *txtExnoLen;
    TLabel *Label6;
    TLabel *Label7;
    TEdit *txtVchtpLen;
    TEdit *txtAccLen;
    TLabel *Label8;
    TEdit *txtAmountLen;
    TGroupBox *GroupBox3;
    TCheckBox *ckOcrRecognize;
    TCheckBox *chkInput;
    TCheckBox *ckManCtrlAmount;
    TGroupBox *GroupBox4;
    TLabel *Label9;
    TEdit *txtVchInterval;
    TLabel *Label10;
    TEdit *txtExnoInterval1;
    TLabel *Label11;
    TEdit *txtAccInterval1;
    TLabel *Label12;
    TEdit *txtVchtpInterval1;
    TLabel *Label14;
    TEdit *txtExnoInterval2;
    TEdit *txtAccInterval2;
    TEdit *txtVchtpInterval2;
    TEdit *txtAmountInterval1;
    TEdit *txtAmountInterval2;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TBitBtn *btnOk;
    TBitBtn *BitBtn2;
    TCheckBox *chkNewHDP;
    TCheckBox *ckColor;
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall BtnSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
private:
public:
	virtual __fastcall TSetupDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TSetupDlg *SetupDlg;
//----------------------------------------------------------------------------
#endif    
