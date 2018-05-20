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
 * 文件：	$Id: C2E.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//-----------------------------------------------------------

#ifndef C2EH
#define C2EH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <myplus.h>
//---------------------------------------------------------------------------
class TFormC2E : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPageControl *matchPage;
    TTabSheet *EnvTab;
    TListView *EnvList;
    TTabSheet *ChkTab;
    TListView *ChkList;
    TStatusBar *StatusBar1;
    TPanel *Panel4;
    TTabSheet *MergeTab;
    TListView *MergeList;
    TBitBtn *btnOk;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
    char _pkgno[20];
    char _session[4];
    MyDb db;
public:		// User declarations
    __fastcall TFormC2E(TComponent* Owner);
    int SetData(const char* pkgno, const char* session);
    int LoadData();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormC2E *FormC2E;
//---------------------------------------------------------------------------
#endif
