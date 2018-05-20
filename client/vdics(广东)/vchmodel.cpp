/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <zcommon.h>

#include "vchmodel.h"
#include "batscan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVchMode *VchMode;
//---------------------------------------------------------------------------
__fastcall TVchMode::TVchMode(TComponent* Owner)
    : TForm(Owner)
{
    mode=0;
}
//---------------------------------------------------------------------------


int TVchMode::SetData(vch_type_t oneVch,int flag)
{
    //TODO: Add your source code here
    mode=flag;
    memset(&curVch,0,sizeof(curVch));
    String stmp;

    vchname->Caption=String(oneVch.vchname)+"     ";
    if(mode==0||mode==1)
    {
        curVch=oneVch;
        stmp.printf("%4d",oneVch.dpi);
        txtDpi->Text=stmp;

        stmp.printf("%8.6f",oneVch.vch_sizex);
        txtVchX->Text=stmp;

        stmp.printf("%8.6f",oneVch.vch_sizey);
        txtVchY->Text=stmp;


        if(oneVch.format==1)  RadioJpg->Checked=true;
        else               RadioBmp->Checked=true;

        switch(oneVch.rotate)
        {
            case 0: Radio0->Checked=true;
                    break;
            case 1: Radio90->Checked=true;
                    break;
            case 2: Radio180->Checked=true;
                    break;
            case 3: Radio270->Checked=true;
                    break;
            default :ShowMessage("图像旋转角度设置无效");
                    debugout("rotate=%d\n",oneVch.rotate);
                    return 0;
        }

        if(oneVch.sideA)       chkSideA->Checked=true;
        else                   chkSideA->Checked=false;

        if(oneVch.sideB)       chkSideB->Checked=true;
        else                   chkSideB->Checked=false;

        if(oneVch.procA)       chkProcA->Checked=true;
        else                   chkProcA->Checked=false;

        if(oneVch.procB)       chkProcB->Checked=true;
        else                   chkProcB->Checked=false;

        switch(oneVch.colortype)
        {
            case 0: RadioRgb->Checked=true;
                    break;
            case 1: RadioGray->Checked=true;
                    break;
            case 2: RadioBw->Checked=true;
                    break;
            default :ShowMessage("扫描图像颜色设置无效!");
                    return 0;
        }

        cbScanType->ItemIndex =  oneVch.scantype;


    }
    else if(mode==2)
    {
        curVch=oneVch;
        txtDpi->Text="200";

        txtVchX->Text="3.2";
        txtVchY->Text="6.7";

        RadioJpg->Checked=true;

        Radio270->Checked=true;

        chkSideA->Checked=true;

        chkSideB->Checked=false;

        chkProcA->Checked=true;

        chkProcB->Checked=false;

        RadioRgb->Checked=true;

    }
        return 0;
}
void __fastcall TVchMode::CancelClick(TObject *Sender)
{
     return;
}
//---------------------------------------------------------------------------
void __fastcall TVchMode::ConformClick(TObject *Sender)
{
    if(mode==0)   return;
    if(chkSideA->Checked==false&&chkSideB->Checked==false)
    {
        MessageBox(Handle,"不能设置两面都不扫描!\t","提示",MB_OK+MB_ICONERROR);
        return;
    }
    else if(mode==1)
    {
        VCHTYPELIST::iterator  pos= FormScan->m_Ini.pList->myList.begin();
        while( pos != FormScan->m_Ini.pList->myList.end())
	    {
            if(strcmp(pos->vchname,curVch.vchname)==0) break;
            ++pos;
	    }

        pos->dpi=txtDpi->Text.ToInt();
        pos->vch_sizex=txtVchX->Text.ToDouble();
        pos->vch_sizey=txtVchY->Text.ToDouble();

        if(RadioBmp->Checked==true)                 pos->format=0;
        else                                        pos->format=1;

        if(chkSideA->Checked==true)                 pos->sideA=1;
        else                                        pos->sideA=0;

        if(chkSideB->Checked==true)                 pos->sideB=1;
        else                                        pos->sideB=0;

        if(chkProcA->Checked==true)                 pos->procA=1;
        else                                        pos->procA=0;

        if(chkProcB->Checked==true)                 pos->procB=1;
        else                                        pos->procB=0;

        if(RadioRgb->Checked==true)                 pos->colortype=0;
        else if(RadioGray->Checked==true)           pos->colortype=1;
        else                                        pos->colortype=2;

        if(Radio0->Checked==true)                   pos->rotate=0;
        else if(Radio90->Checked==true)             pos->rotate=1;
        else if(Radio180->Checked==true)            pos->rotate=2;
        else                                        pos->rotate=3;

        pos->scantype= cbScanType->ItemIndex;

    }

    else if(mode==2)
    {

        curVch.dpi=txtDpi->Text.ToInt();
        curVch.vch_sizex=txtVchX->Text.ToDouble();
        curVch.vch_sizey=txtVchY->Text.ToDouble();

        if(RadioBmp->Checked==true)                 curVch.format=0;
        else                                        curVch.format=1;

        if(chkSideA->Checked==true)                 curVch.sideA=1;
        else                                        curVch.sideA=0;

        if(chkSideB->Checked==true)                 curVch.sideB=1;
        else                                        curVch.sideB=0;

        if(chkProcA->Checked==true)                 curVch.procA=1;
        else                                        curVch.procA=0;

        if(chkProcB->Checked==true)                 curVch.procB=1;
        else                                        curVch.procB=0;

        if(RadioRgb->Checked==true)                 curVch.colortype=0;
        else if(RadioGray->Checked==true)           curVch.colortype=1;
        else                                        curVch.colortype=2;

        if(Radio0->Checked==true)                   curVch.rotate=0;
        else if(Radio90->Checked==true)             curVch.rotate=1;
        else if(Radio180->Checked==true)            curVch.rotate=2;
        else                                        curVch.rotate=3;

        curVch.scantype= cbScanType->ItemIndex;

        FormScan->m_Ini.pList->myList.push_back(curVch);

        FormScan->m_Ini.m_scannerInfo.vchtcount= FormScan->m_Ini.m_scannerInfo.vchtcount+1;
    }

}
//---------------------------------------------------------------------------
