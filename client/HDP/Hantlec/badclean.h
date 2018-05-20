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
 * 文件：	$Id: badclean.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//-----------------------------------------------------------------
#ifndef badcleanH
#define badcleanH
//----------------------------------------------------------------------------
/*
 * Copyrights (c) 2006 北京紫枫科技开发有限公司软件产品开发部
 * 
 * 版本 0.2
 * 
 * 作者：聂建军
 *
 * 摘要：错票清理界面，清除掉输入信息错误的票据信息，该票据可以重新清分
 *
 * 注释：杜永刚
 *
 * 完成日期：2006年2月
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
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
//----------------------------------------------------------------------------
class TBadCleanDlg : public TForm
{
__published:
    TLabel *Label2;
    TBevel *Bevel1;
    TButton *btnDel;
    TButton *btnExit;
    TLabel *Label1;
    TListView *VchList;
    TImageList *ImageList1;
    TGroupBox *GroupBox1;
    TRadioButton *RadioA;
    TRadioButton *RadioD;
    TRadioButton *RadioC;
    TRadioButton *RadioM;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnDelClick(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall RadioAClick(TObject *Sender);
    void __fastcall RadioDClick(TObject *Sender);
    void __fastcall RadioCClick(TObject *Sender);
    void __fastcall RadioMClick(TObject *Sender);
private:
public:
	virtual __fastcall TBadCleanDlg(TComponent* AOwner);
    int count;
    int countjf;
    int countdf;
    int countyc;

    HANDLE m_distUI;
};
//----------------------------------------------------------------------------
//extern PACKAGE TBadCleanDlg *BadCleanDlg;
//----------------------------------------------------------------------------
#endif    
