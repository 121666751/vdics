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
 * 文件：	$Id: ManVch.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//--------------------------------------------------------------

#ifndef ManVchH
#define ManVchH
//---------------------------------------------------------------------------
/*
 * Copyrights (c) 2006 北京紫枫科技开发有限公司软件产品开发部
 * 
 * 版本 0.2
 * 
 * 作者：聂建军
 *
 * 摘要：人工补录票据信息界面
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
