//---------------------------------------------------------------------
#include <winsock2.h>
#include <vcl.h>
#pragma hdrstop

#include "SetupDlg.h"
#include <io.h>
#include <zcommon.h>

#include <filectrl.hpp>
#include "batscan.h"
#include "vchmodel.h"
#include "ScannerIni.h"
#include "vchname.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
TFmSetup *FmSetup;
//---------------------------------------------------------------------
__fastcall TFmSetup::TFmSetup(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TFmSetup::OKBtnClick(TObject *Sender)
{
    if( access(txtPath->Text.c_str(),0) ) {
    MessageBox(Handle,"路径设置不正确!\t","错误",MB_OK+MB_ICONERROR);
		ModalResult = mrNone;
    }
    else {
    	ModalResult = mrOk;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFmSetup::btnBrowseClick(TObject *Sender)
{
    AnsiString S;
    S = "";
    if (SelectDirectory("浏览文件夹", "", S))  txtPath->Text = S;
}
//---------------------------------------------------------------------------

void __fastcall TFmSetup::btnModify1Click(TObject *Sender)
{
  /*	if( MessageBox(Handle,"确实要更改扫描精度、扫描范围吗?","请确认...",
    	MB_YESNO+MB_ICONWARNING) == IDNO ) return;

        txtDpi->Enabled = true;
        txtVchX->Enabled = true;
        txtVchY->Enabled=true;
        txtDpi->SetFocus();   */
}
//---------------------------------------------------------------------------



void __fastcall TFmSetup::txtSerialKeyPress(TObject *Sender, char &Key)
{
	if( !isdigit(Key) ) Key=0;
}
//---------------------------------------------------------------------------

void __fastcall TFmSetup::btnchangemode(TObject *Sender)
{
   	if( MessageBox(Handle,"确实要更改校准值、偏移量吗?\t","请确认...",
    	MB_YESNO+MB_ICONWARNING) == IDNO ) return;

      txtCalX->Enabled=true;
      txtCalY->Enabled=true;
      txtOffSet->Enabled=true;
      txtCalX->SetFocus();

}
//---------------------------------------------------------------------------




void __fastcall TFmSetup::btnConFirmClick(TObject *Sender)
{
    if( access(txtPath->Text.c_str(),0) )
    {
    	MessageBox(Handle,"路径设置不正确!\t","错误",MB_OK+MB_ICONERROR);
		ModalResult = mrNone;
    }
    else
    {
    	ModalResult = mrOk;
    }
}
//---------------------------------------------------------------------------



void __fastcall TFmSetup::VchDetailListDblClick(TObject *Sender)
{
/*
    vch_type_t selectVch;
    memset(&selectVch,0,sizeof(vch_type_t));
    TListItem* li;
    li=VchDetailList->Selected;
    if(li==NULL)
    {
        return;
    }
    strcpy(selectVch.vchname,li->Caption.c_str());
    debugout("name=[%s]\n",selectVch.vchname);
    VCHTYPELIST::iterator  pos= FormScan->m_Ini.pList->myList.begin();
    while( pos != FormScan->m_Ini.pList->myList.end())
	{

        if(strcmp(pos->vchname,selectVch.vchname)==0) break;
        ++pos;
	}
    VchMode->SetData(*pos,0);
    VchMode->ShowModal();                    */
}
//---------------------------------------------------------------------------

void __fastcall TFmSetup::btnModifyClick(TObject *Sender)
{

    vch_type_t selectVch;
    String stmp;
    memset(&selectVch,0,sizeof(vch_type_t));
    TListItem* li;
    li=VchDetailList->Selected;
    if(li==NULL)
    {
        MessageBox(Handle,"请先选择凭证种类!\t","错误",MB_OK+MB_ICONERROR);
        return;
    }
    strcpy(selectVch.vchname,li->Caption.c_str());
    debugout("name=[%s]\n",selectVch.vchname);
    VCHTYPELIST::iterator  pos= FormScan->m_Ini.pList->myList.begin();
    while( pos != FormScan->m_Ini.pList->myList.end())
	{

        if(strcmp(pos->vchname,selectVch.vchname)==0) break;
        ++pos;
	}

    VchMode->SetData(*pos,1);
    VchMode->ShowModal();

    pos= FormScan->m_Ini.pList->myList.begin();
    VchDetailList->Items->Clear();
    while( pos != FormScan->m_Ini.pList->myList.end())
	{
        li =  VchDetailList->Items->Add();
		li->Caption=pos->vchname;
        stmp.printf("%5.2f x %5.2f",pos->vch_sizex,pos->vch_sizey);
        li->SubItems->Insert(0,stmp.c_str()) ;
        li->Data =(void*)pos->vchname;

        ++pos;
	}



}
//---------------------------------------------------------------------------


void __fastcall TFmSetup::btnClearClick(TObject *Sender)
{
    vch_type_t selectVch;
    memset(&selectVch,0,sizeof(vch_type_t));
    TListItem* li;
    li=VchDetailList->Selected;
    if(li==NULL)
    {
        MessageBox(Handle,"请先选择凭证种类!\t","错误",MB_OK+MB_ICONERROR);
        return;
    }
    strcpy(selectVch.vchname,li->Caption.c_str());
    debugout("name=[%s]\n",selectVch.vchname);
    VCHTYPELIST::iterator  pos= FormScan->m_Ini.pList->myList.begin();
    while( pos != FormScan->m_Ini.pList->myList.end())
	{
        if(strcmp(pos->vchname,selectVch.vchname)==0) break;
        ++pos;
	}
    FormScan->m_Ini.pList->myList.erase(pos);
    li->Delete();
    FormScan->m_Ini.m_scannerInfo.vchtcount= FormScan->m_Ini.m_scannerInfo.vchtcount-1;

}
//---------------------------------------------------------------------------

void __fastcall TFmSetup::btnAddClick(TObject *Sender)
{
    String stmp;
    Newvch->ShowModal();
    //if(Newvch->txtVchName->Text.Trim().c_str()==NULL)
    if(Newvch->txtVchName->Text.Trim()=="")
    {
        MessageBox(Handle,"请输入凭证名称!\t","错误",MB_OK+MB_ICONERROR);
        return;
    }
    TListItem* li;
    vch_type_t onevch;
    memset(&onevch,0,sizeof(vch_type_t));
    strcpy(onevch.vchname,Newvch->txtVchName->Text.Trim().c_str());
    for(int i=0;i<VchDetailList->Items->Count;i++)
    {
       li = VchDetailList->Items->Item[i];
       if((strcmp(li->Caption.c_str(),onevch.vchname)==0))
       {
           ShowMessage("凭证名称重复！请重新设置!\t");
           return;
       }
   }

    VchMode->SetData(onevch,2);
    VchMode->ShowModal();

    memset(&onevch,0,sizeof(vch_type_t));
    VCHTYPELIST::iterator  pos= FormScan->m_Ini.pList->myList.begin();
    VchDetailList->Items->Clear();
    while( pos != FormScan->m_Ini.pList->myList.end())
	{
        onevch= *pos;
        li =  VchDetailList->Items->Add();
		li->Caption=pos->vchname;
        stmp.printf("%5.2f x %5.2f",pos->vch_sizex,pos->vch_sizey);
        li->SubItems->Insert(0,stmp.c_str()) ;
        li->Data =(void*)pos->vchname;

        ++pos;
	}



}
//---------------------------------------------------------------------------


