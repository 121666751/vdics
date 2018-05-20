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
 * 文件：	$Id: rescan.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com
 * 邮箱：   support@tchzt.com
 *
 ***************************************************************************
 */
//----------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "rescan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRescanForm *RescanForm;
//---------------------------------------------------------------------------
__fastcall TRescanForm::TRescanForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
/**
    窗体显示事件函数
*/
void __fastcall TRescanForm::FormShow(TObject *Sender)
{
    //打开数据库
    int ret = db.Open("local_db_host","cidsa","z412t620","vchcenter");
    if( ret )
    {
        MessageBox(Handle,"打开数据库出错","错误",MB_OK|MB_ICONERROR);
        Application->Terminate();
        return ;
    }

    //查询并显示网点号
    GetNetnos();
    cbNetNo->ItemIndex = 0;
    cbNetNo->SetFocus();
}
//---------------------------------------------------------------------------
/**
    查询显示网点号函数
*/
int TRescanForm::GetNetnos()
{
    //TODO: Add your source code here
    cbNetNo->Items->Clear();

    //查询网点号
    int ret = db.Query("select distinct pkgno from at_exchst where substring(state,2,1)='1' and substring(state,3,1)='0'");
    if( ret )
    {
        MessageBox(Handle,"查询出错","错误",MB_OK|MB_ICONERROR);
        Application->Terminate();
        return -1;
    }
    MyRes res = db.Store();
    int count = (int)res.Count();
    if( count <= 0 )
    {
        btnOk->Enabled = true;
        return 0;
    }
    btnOk->Enabled = true;

    //显示网点号
    MyRow row = res.Fetch();
    while( row!=NULL )
    {
        cbNetNo->Items->Add( row[0] );
        row=res.Fetch();
    }

    return 0;
}
//---------------------------------------------------------------------------

/**
    查询所有交换号
*/
void __fastcall TRescanForm::cbNetNoDropDown(TObject *Sender)
{
    GetNetnos();
}
//---------------------------------------------------------------------------
void __fastcall TRescanForm::FormCreate(TObject *Sender)
{
    //显示第一条记录
    cbNetNo->ItemIndex = 0;
}
//---------------------------------------------------------------------------



