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
#include <libmem.h>
#include <transimg.h>

#include "pbcimg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDet *FormDet;
//---------------------------------------------------------------------------
__fastcall TFormDet::TFormDet(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


int TFormDet::SetData(pbc_node_t node)
{
    //TODO: Add your source code here
    _node = node;
    return 0;
}
void __fastcall TFormDet::FormShow(TObject *Sender)
{
    txtInfo->Cells[0][0] = "项目";
    txtInfo->Cells[1][0] = "帐号";
    txtInfo->Cells[2][0] = "凭证号";
    txtInfo->Cells[3][0] = "金额";
    txtInfo->Cells[4][0] = "文件名";
    txtInfo->Cells[5][0] = "流水号";
    txtInfo->Cells[6][0] = "户名";


    txtInfo->Cells[0][1] = "值";
    txtInfo->Cells[1][1] = _node.accno;
    txtInfo->Cells[2][1] = _node.vchno;
    txtInfo->Cells[3][1] = _node.amount;
    txtInfo->Cells[4][1] = _node.fname;
    txtInfo->Cells[5][1] = _node.sid;
    txtInfo->Cells[6][1] = _node.accname;


    ShowImage();
    txtImage->Align = alNone;
    txtImage->Left = 1;
    txtImage->Top = 1;
    txtImage->AutoSize = true;
    txtImage->Tag = 1;
}
//---------------------------------------------------------------------------


int TFormDet::ShowImage()
{
    //TODO: Add your source code here
    txtImage->Picture->Bitmap->Assign(NULL) ;
    char fname[256];
    memset(fname,0,sizeof(fname));
    sprintf(fname,"%s//%02d//%05d//%s",_node.date,atoi(_node.session),atoi(_node.exchno),_node.fname);
    int pannel;
    MemT pRGB;
    int sizeX;
    int sizeY;
    int size;

    //获取图像
    int ret = ti_getimage(fname,_node.ip,0,pRGB,sizeX,sizeY,pannel);
    if(ret)
    {
        return ret;
    }

    unsigned char * bmp;

    bmp =ti_RGB2BMP(pRGB.data(),sizeX,sizeY,0,0,0,0,sizeX,sizeY,size);

    TMemoryStream* pms = new TMemoryStream();
    pms->Clear();
    pms->SetSize(size);
    ret = pms->Write(bmp,size);
    pms->Position = 0;
    txtImage->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();
    //释放
    ret = ti_RGB2BMPFREE(bmp);
    bmp = NULL;

    return 0;
}
void __fastcall TFormDet::TBExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormDet::numExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormDet::txtImageMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TImage* p = (TImage*)Sender;

    if(p->Tag)
    {
        p->Tag = 0;
        p->AutoSize = false;
        p->Align = alClient;
        p->Proportional = true;
        p->Stretch = true;
        return;
    }

    static double r1 = (double)p->Picture->Width/p->Width;
    static double r2 = (double)p->Picture->Height/p->Height;

    static double r =  (r1>r2)? r1 : r2;


    p->Align = alNone;
    p->Left = X - X*r;
    p->Top = Y - Y*r;
    p->AutoSize = true;
    p->Tag = 1;
}
//---------------------------------------------------------------------------

