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
 * �ļ���	$Id: wiseIPdlg.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
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
