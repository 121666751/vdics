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
 * �ļ���	$Id: rescan.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//--------------------------------------------------------------

#ifndef rescanH
#define rescanH
//---------------------------------------------------------------------------
/*
 * Copyrights (c) 2006 �����Ϸ�Ƽ��������޹�˾�����Ʒ������
 * 
 * �汾 0.2
 * 
 * ���ߣ�������
 *
 * ժҪ���������������ƽ���
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
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <myplus.h>
//---------------------------------------------------------------------------
class TRescanForm : public TForm
{
__published:	// IDE-managed Components
    TBevel *Bevel1;
    TBevel *Bevel2;
    TComboBox *cbNetNo;
    TLabel *Label1;
    TBitBtn *btnOk;
    TBitBtn *BitBtn2;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall cbNetNoDropDown(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TRescanForm(TComponent* Owner);
    MyDb db;
    int GetNetnos();
};
//---------------------------------------------------------------------------
extern PACKAGE TRescanForm *RescanForm;
//---------------------------------------------------------------------------
#endif
