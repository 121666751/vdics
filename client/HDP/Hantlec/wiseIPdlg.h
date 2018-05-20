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
 * 文件：	$Id: wiseIPdlg.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */

//---------------------------------------------------------------------------
#ifndef wiseIPdlgH
#define wiseIPdlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <myplus.h>

//---------------------------------------------------------------------------
struct DISTDATA;

class TFmWiseIP : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TBitBtn *btnOk;
    TScrollBox *VchScrollBox;
    TImage *VchImage;
    TPanel *Panel3;
    TLabel *Label3;
    TEdit *txtVchNo;
    TLabel *Label4;
    TEdit *txtNetNo;
    TLabel *Label5;
    TEdit *txtAccount;
    TLabel *Label6;
    TEdit *txtVchKind;
    TLabel *Label7;
    TEdit *txtMoney;
    TBevel *Bevel1;
    TLabel *Label8;
    TEdit *txtMicrCode;
    TEdit *txtOcrCode;
    TBitBtn *btnHistQuery;
    TLabel *TimeLeft;
    TTimer *TimerDie;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall btnHistQueryClick(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall TimerDieTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall txtMoneyKeyPress(TObject *Sender, char &Key);
    void __fastcall txtMoneyEnter(TObject *Sender);
    void __fastcall txtMoneyChange(TObject *Sender);
private:	// User declarations
    TJPEGImage* img;
    bool ImageStrech;
    bool NeedEndorse;

    bool bFormatMoney;
    int nPopos;
    bool bPoint;
public:
  
    bool ManCheckResult;
    int FeedSource;		// Feeder Type 0 - Hand Drop ; 1 - Main Hopper ; 2 - Merge Hopper(not use)
    int FeedCount;      // Feed Count limit;
    String TcsName;
    __fastcall TFmWiseIP(TComponent* Owner);

    void SetResult(DISTDATA * pdata,const char *fname);
    char fname[256];
    void GetResult(DISTDATA * pdata);
    void GetCurImage(int DataNumber);
    bool isNumeric(const char * str);
    void MoneyKeyPress(TEdit* txtMoney, char & Key);
protected:
    char * StateText(int state);
    int GetPocketNum();
    void setVchImgSize(bool bStrech);
    void makeBSTRStr(String & str, int tagLen);
    void MoneyChange(TEdit *txtMoney);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmWiseIP *FmWiseIP;
//---------------------------------------------------------------------------
#endif
