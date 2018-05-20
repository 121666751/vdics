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
 * 文件：	$Id: Hantlec.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//-----------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("TotalMoney.cpp", FmTotalMoney);
USEFORM("selNet.cpp", FmSelNet);
USEFORM("about.cpp", AboutBox);
USEFORM("wait.cpp", FmWait);
USEFORM("wiseIPdlg.cpp", FmWiseIP);
USEFORM("main.cpp", FmDistMan);
USEFORM("histquery.cpp", HistQueryDlg);
USEFORM("setup.cpp", SetupDlg);
USEFORM("ManVch.cpp", ManVchAdd);
USEFORM("badclean.cpp", BadCleanDlg);
USEFORM("C2E.cpp", FormC2E);
USEFORM("login.cpp", FormLogin);
USEFORM("pocket.cpp", FmPocket);
USEFORM("vips.cpp", FmVips);
USEFORM("distData.cpp", distForm);
USEFORM("distimage.cpp", distFormImage);
USEFORM("state.cpp", FormST);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "帐务中心清分控制";
         Application->CreateForm(__classid(TFmDistMan), &FmDistMan);
         Application->CreateForm(__classid(TFmTotalMoney), &FmTotalMoney);
         Application->CreateForm(__classid(TFmSelNet), &FmSelNet);
         Application->CreateForm(__classid(TAboutBox), &AboutBox);
         Application->CreateForm(__classid(TFmWait), &FmWait);
         Application->CreateForm(__classid(TFmWiseIP), &FmWiseIP);
         Application->CreateForm(__classid(TSetupDlg), &SetupDlg);
         Application->CreateForm(__classid(TManVchAdd), &ManVchAdd);
         Application->CreateForm(__classid(TFormLogin), &FormLogin);
         Application->CreateForm(__classid(TFmPocket), &FmPocket);
         Application->CreateForm(__classid(TFmVips), &FmVips);
         Application->CreateForm(__classid(TdistFormImage), &distFormImage);
         Application->CreateForm(__classid(TdistForm), &distForm);
         Application->CreateForm(__classid(TFormST), &FormST);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------

