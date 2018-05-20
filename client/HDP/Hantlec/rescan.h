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
 * 文件：	$Id: rescan.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//--------------------------------------------------------------

#ifndef rescanH
#define rescanH
//---------------------------------------------------------------------------
/*
 * Copyrights (c) 2006 北京紫枫科技开发有限公司软件产品开发部
 * 
 * 版本 0.2
 * 
 * 作者：聂建军
 *
 * 摘要：重新清分网点控制界面
 *
 * 注释：杜永刚
 *
 * 完成日期：2006年2月
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
