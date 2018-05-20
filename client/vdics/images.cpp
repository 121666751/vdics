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

#include "images.h"
#include "main.h"
#include <zcommon.h>
#include <transimg.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormImages *FormImages;
//---------------------------------------------------------------------------
__fastcall TFormImages::TFormImages(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormImages::FormShow(TObject *Sender)
{
    image->Align = alClient;
    image->Left = 1;
    image->Top = 1;
    image->AutoSize = false;
    image->Tag = 1;    
}
//---------------------------------------------------------------------------
void __fastcall TFormImages::imageMouseUp(TObject *Sender,
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
void __fastcall TFormImages::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------


int TFormImages::setMethod(int type, int id)
{
    //TODO: Add your source code here
    _type = type;
    _sid   = id;
    lstFile->Clear();
    int ret ;
    if(type == 0 )     //信封
    {
        ret =  FmIC->gdb.VQuery(512,"select sid,fname from multis where envid=%d",id);
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return 0;
        }
        MyRes res =  FmIC->gdb.Store();
        MyRow row =  res.Fetch();
        TListItem* li;
        while(row!=NULL)
        {
            li = lstFile->Items->Add();
            li->Caption = row[0];
            li->SubItems->Insert(0,row[1]);
            row = res.Fetch();
        }
    }
    else    //主票
    {
        ret = FmIC->gdb.VQuery(512,"select sid,fname ,envid from multis where  mainid=%d and (model !=2 and model!=6 and model!=7)",id);
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return 0;
        }

        int envid;
        MyRes res =  FmIC->gdb.Store();
        MyRow row =  res.Fetch();
        TListItem* li;
        while(row!=NULL)
        {
            li = lstFile->Items->Add();
            li->Caption = row[0];
            li->SubItems->Insert(0,row[1]);
            envid = atoi(row[2]);
            row = res.Fetch();
        }

        ret = FmIC->gdb.VQuery(512,"select sid,fname ,envid from multis where  sid=%d",envid);
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return 0;
        }

        res =  FmIC->gdb.Store();
        row =  res.Fetch();
        while(row!=NULL)
        {
            li = lstFile->Items->Add();
            li->Caption = row[0];
            li->SubItems->Insert(0,row[1]);
            row = res.Fetch();
        }
    }
}
void __fastcall TFormImages::lstFileSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
    TListItem* li;
    li= lstFile->Selected;
    if(li==NULL) return ;
    int sid = li->Caption.ToInt() ;

   int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,substate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session  from multis where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return ;

    char ip[24];

    char amount[16];
    char fname[80];
    char bname[80];
    char pkgno[12];
    int  submitstate;
    int  rotate;
    int  vchflag;
    int  vchtype;
      
    memset(ip,0,sizeof(ip));

    memset(amount,0,sizeof(amount));
    memset(fname,0,sizeof(fname));
    memset(bname,0,sizeof(bname));
    memset(pkgno,0,sizeof(pkgno));
    strcpy(ip,row[0]);
    rotate = atoi(row[1]);
    submitstate = atoi(row[2]);     

    strcpy(amount,row[3]);
    vchflag = atoi(row[4]);

    strcpy(fname,row[5]);
    strcpy(bname,row[6]);
    strcpy(pkgno,row[7]);
    vchtype = atoi(row[8]);


    MemT image;
    int sizex;
    int sizey;
    int panel;


    char tname[128];
    memset(tname,0,sizeof(tname));
    sprintf(tname,"%s//%02d//%05d//%s",row[9],atoi(row[10]),atoi(row[7]),fname);

    ret =  ti_getimage( tname, ip, rotate,   image, sizex, sizey,panel );
    if(ret)
    {
        MessageBox(Handle,"获取图像错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    unsigned char * bmp;
    int size;

    bmp =ti_RGB2BMP(image.data(),sizex,sizey,0,0,0,0,sizex,sizey,size);
    
    TMemoryStream* pms = new TMemoryStream();
    pms->Clear();
    pms->SetSize(size);
    ret = pms->Write(bmp,size);
    ti_RGB2BMPFREE(bmp);
    pms->Position = 0;
    FormImages->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();
    return ;    
}
//---------------------------------------------------------------------------
