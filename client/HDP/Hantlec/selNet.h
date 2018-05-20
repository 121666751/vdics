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
 * 文件：	$Id: selNet.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//--------------------------------------------------------------

#ifndef selNetH
#define selNetH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <myplus.h>
#define NETNO_UNLIMIT_MARK "111111"
//---------------------------------------------------------------------------
class TFmSelNet : public TForm
{
__published:	// IDE-managed Components
    TBevel *Bevel1;
    TBitBtn *btnOk;
    TBitBtn *BitBtn2;
    TLabel *Label2;
    TComboBox *cbNetNo;
    TLabel *Label1;
    TSpeedButton *btnCheck;
    TSpeedButton *btnDaiji;
    TSpeedButton *btnAll;
    TCheckBox *cbTallyMode;
    TBevel *Bevel2;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall cbNetNoDropDown(TObject *Sender);
    void __fastcall btnCheckClick(TObject *Sender);
private:
    int GetNetNoEx();
    void ToggleTcBtn();
    void ToggleTallyMode();	// User declarations
public:		// User declarations
    __fastcall TFmSelNet(TComponent* Owner);
    int GetNetnos();
    int m_vchTc;
};
//---------------------------------------------------------------------------
extern PACKAGE TFmSelNet *FmSelNet;
//---------------------------------------------------------------------------
#endif
