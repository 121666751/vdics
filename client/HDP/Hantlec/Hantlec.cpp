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
 * �ļ���	$Id: Hantlec.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
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
         Application->Title = "����������ֿ���";
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

