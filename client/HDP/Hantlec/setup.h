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
 * �ļ���	$Id: setup.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//--------------------------------------------------------------
#ifndef setupH
#define setupH
//----------------------------------------------------------------------------
/*
 * Copyrights (c) 2006 �����Ϸ�Ƽ��������޹�˾�����Ʒ������
 * 
 * �汾 0.2
 * 
 * ���ߣ�������
 *
 * ժҪ��������Ϣ������棬�����޸�������Ϣ
 *
 * ע�ͣ�������
 *
 * ������ڣ�2006��2��
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
