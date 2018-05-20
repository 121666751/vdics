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
 * �ļ���	$Id: main.h,v 1.2 2008/03/30 10:40:36 Nie Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TFmSethosts : public TForm
{
__published:	// IDE-managed Components
    TButton *btnSaveHosts;
    TBevel *Bevel2;
    TBevel *Bevel1;
    TBevel *Bevel3;
    TButton *btnRefresh;
    TButton *btnExit;
    TLabel *Label10;
    TMemo *lstHost;
    TLabel *Label11;
    TLabel *Label12;
    TEdit *message_server;
    TLabel *Label13;
    TLabel *Label1;
    TEdit *ztic_db_server;
    TLabel *Label3;
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall local_db_hostChange(TObject *Sender);
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall btnSaveHostsClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    TStringList *p;
    TStringList *pIP;
    TStringList *pName;

public:		// User declarations
    __fastcall TFmSethosts(TComponent* Owner);
protected:
    int LoadHosts();
};
//---------------------------------------------------------------------------
extern PACKAGE TFmSethosts *FmSethosts;
//---------------------------------------------------------------------------
#endif
