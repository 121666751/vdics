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
 * 文件：	$Id: totalmoney.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//------------------------------------------------------------------

#ifndef TotalMoneyH
#define TotalMoneyH
//---------------------------------------------------------------------------
/*
 * Copyrights (c) 2006 北京紫枫科技开发有限公司软件产品开发部
 * 
 * 版本 0.2
 * 
 * 作者：聂建军
 *
 * 摘要：清分金额控制界面，用来在一次清分时选择网点，并确定并控制借贷方总金额
 *
 * 注释：杜永刚
 *
 * 完成日期：2006年2月
 */
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <myplus.h>
//---------------------------------------------------------------------------
class TFmTotalMoney : public TForm
{
__published:	// IDE-managed Components
    TBevel *Bevel1;
    TBevel *Bevel2;
    TLabel *Label1;
    TComboBox *txtpkgno;
    TLabel *txtMoneyKind;
    TEdit *txtTotalMoneyJF;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *txtTotalMoneyDF;
    TBevel *Bevel3;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    void __fastcall txtTotalMoneyJFChange(TObject *Sender);
    void __fastcall txtTotalMoneyJFKeyPress(TObject *Sender, char &Key);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall txtTotalMoneyJFEnter(TObject *Sender);
    void __fastcall txtTotalMoneyDFChange(TObject *Sender);
    void __fastcall txtTotalMoneyDFKeyPress(TObject *Sender, char &Key);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall txtpkgnoDropDown(TObject *Sender);
    void __fastcall txtpkgnoSelect(TObject *Sender);
private:	// User declarations
    bool bFormatMoney;
    int nPopos;
    bool bPoint;
    int initNetno();
public:		// User declarations
    __fastcall TFmTotalMoney(TComponent* Owner);
    int mode;
protected:
    void MoneyChange(TEdit *txtMoney);
    void MoneyKeyPress(TEdit* txtMoney, char & Key);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmTotalMoney *FmTotalMoney;
//---------------------------------------------------------------------------
#endif
