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
 * �ļ���	$Id: totalmoney.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//------------------------------------------------------------------

#ifndef TotalMoneyH
#define TotalMoneyH
//---------------------------------------------------------------------------
/*
 * Copyrights (c) 2006 �����Ϸ�Ƽ��������޹�˾�����Ʒ������
 * 
 * �汾 0.2
 * 
 * ���ߣ�������
 *
 * ժҪ����ֽ����ƽ��棬������һ�����ʱѡ�����㣬��ȷ�������ƽ�����ܽ��
 *
 * ע�ͣ�������
 *
 * ������ڣ�2006��2��
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
