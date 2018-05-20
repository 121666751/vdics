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
 * �ļ���	$Id: ManVch.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//--------------------------------------------------------------

#ifndef ManVchH
#define ManVchH
//---------------------------------------------------------------------------
/*
 * Copyrights (c) 2006 �����Ϸ�Ƽ��������޹�˾�����Ʒ������
 * 
 * �汾 0.2
 * 
 * ���ߣ�������
 *
 * ժҪ���˹���¼Ʊ����Ϣ����
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
#include <dstCommon.h>
//---------------------------------------------------------------------------
class TManVchAdd : public TForm
{
__published:	// IDE-managed Components
    TButton *ok;
    TButton *cancel;
    TBevel *Bevel1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TEdit *vchno;
    TEdit *exchno;
    TEdit *accno;
    TEdit *vchtype;
    TEdit *vchmoney;
    TLabel *Label6;
    TLabel *Label7;
    void __fastcall okClick(TObject *Sender);
    void __fastcall vchnoKeyPress(TObject *Sender, char &Key);
    void __fastcall exchnoKeyPress(TObject *Sender, char &Key);
    void __fastcall accnoKeyPress(TObject *Sender, char &Key);
    void __fastcall vchtypeKeyPress(TObject *Sender, char &Key);
    void __fastcall vchmoneyKeyPress(TObject *Sender, char &Key);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TManVchAdd(TComponent* Owner,DISTDATA* pData);
    DISTDATA* addData;
};
//---------------------------------------------------------------------------
extern PACKAGE TManVchAdd *ManVchAdd;
//---------------------------------------------------------------------------
#endif
