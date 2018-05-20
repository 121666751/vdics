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
 * 文件：	$Id: main.h,v 1.2 2008/03/30 10:40:36 Nie Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
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
