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

#include <bcbext.h>

#include "collectioncheck.h"
#include "main.h"
#include <zcommon.h>
#include <transimg.h>
#include "login.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormWsCheck *FormWsCheck;
//---------------------------------------------------------------------------
__fastcall TFormWsCheck::TFormWsCheck(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


int TFormWsCheck::setMethod(int type, int sid)
{
    //TODO: Add your source code here
     _type = type;
    _sid   = sid;
    _curSid = sid;
    lstFile->Clear();
    int ret =  FmIC->gdb.VQuery(512,"select sid,fname from authinput where envid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
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
void __fastcall TFormWsCheck::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormWsCheck::imageMouseUp(TObject *Sender,
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
void __fastcall TFormWsCheck::FormShow(TObject *Sender)
{
    image->Align = alClient;
    image->Left = 1;
    image->Top = 1;
    image->AutoSize = false;
    image->Tag = 1;
    txtAmount->SetFocus();
    SetCtrlStyle(txtAmount,ES_NUMBER);    
    if(lstFile->Items->Count>0) lstFile->Items->Item[0]->Selected = true;

}
//---------------------------------------------------------------------------
void __fastcall TFormWsCheck::lstFileSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
    TListItem* li;
    li= lstFile->Selected;
    if(li==NULL) return ;
    int sid = li->Caption.ToInt() ;
    _curSid = sid;

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,substate,amount,model,fname,bname,right(pkgno,5),vchtype,mainflag,date,session  from authinput where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
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
    txtAmount->Text = amount;

    vchflag = atoi(row[4]);

  
    strcpy(fname,row[5]);
    strcpy(bname,row[6]);
    strcpy(pkgno,row[7]);
    vchtype = atoi(row[8]);
    
    chkMain->Checked = (atoi(row[9]) == 1);

    if(vchflag == 2 || vchflag ==6 || vchflag == 7)
    {
         chkMain->Visible = false;
         txtAmount->Visible = true;
    }
    else
    {
         LabelAmount->Visible = true;
         txtAmount->Visible = true;
         chkMain->Visible = true;
         chkMain->Checked = true;    
    }


    MemT image;
    int sizex;
    int sizey;
    int panel;

    char tname[128];
    memset(tname,0,sizeof(tname));
    sprintf(tname,"%s//%02d//%05d//%s",row[10],atoi(row[11]),atoi(row[7]),fname);

    ret =  ti_getimage( tname, ip, rotate,   image, sizex, sizey,panel );
    if(ret)
    {
        MessageBox(Handle,"获取图像错误","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    unsigned char * bmp;
    int size;

    bmp =ti_RGB2BMP(image.data(),sizex,sizey,0,0,0,0,sizex,sizey,size);
    
    TMemoryStream* pms = new TMemoryStream();
    pms->Clear();
    pms->SetSize(size);
    ret = pms->Write(bmp,size);
    delete   bmp;
    pms->Position = 0;
    FormWsCheck->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    txtAmount->SetFocus();
    return ;        
}
//---------------------------------------------------------------------------
void __fastcall TFormWsCheck::chkMainClick(TObject *Sender)
{
    if(chkMain->Checked == false)
    {
        LabelAmount->Visible = false;
        txtAmount->Visible = false;
    }
    else
    {
        LabelAmount->Visible = true;
        txtAmount->Visible = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormWsCheck::btnSaveClick(TObject *Sender)
{
    int mainflag =0 ;
    if(chkMain->Visible == true && chkMain->Checked == true)
    {
        mainflag = 1;
    }
    String amt="0.00";
    if(txtAmount->Visible == true)
    {
        amt = RemoveThousandChar(txtAmount->Text);
    }

    if(amt.ToDouble()<0.01 && txtAmount->Visible == true)
    {
        MessageBox(Handle,"金额错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    int ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',mainflag=%d ,clkno='%s' where sid=%d",amt.c_str(),mainflag,g_sys_opt.c_str(),_curSid);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }

    TListItem* li;
    li= lstFile->Selected;
    if(li==NULL) return ;

    int sid = li->Index+2;
    if(sid> lstFile->Items->Count )
    {
        if( MessageBox(Handle,"处理完毕，是否核打?","提示",MB_YESNO|MB_ICONINFORMATION)==IDYES )
        {
            btnCheckClick(0);    
        }
        return ;
    }
    TListItem* nli;
    nli = lstFile->Items->Item[li->Index+1];
    nli->Selected = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormWsCheck::btnCheckClick(TObject *Sender)
{
    int ret = FmIC->gdb.VQuery(512,"select amount from authinput where sid=%d",_sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    String amt = row[0];

    //单个信封主件总金额
    ret = FmIC->gdb.VQuery(512,"select sum(amount) from authinput where mainflag=1 and envid=%d",_sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    res = FmIC->gdb.Store();
    row = res.Fetch();

    String amt2 = row[0];
    if(amt.ToDouble() != amt2.ToDouble() )
    {
        MessageBox(Handle,"总金额不对","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    ret = FmIC->gdb.VQuery(512,"update authinput set checkstate=1 where envid=%d",_sid);
    ret = FmIC->gdb.VQuery(512,"update authinput set mainflag=0  where amount='0.00' and envid=%d",_sid);

    ret = FmIC->gdb.VQuery(512,"select sid,mainflag from authinput where envid=%d order by sid",_sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    res = FmIC->gdb.Store();
    row = res.Fetch();
    int mainflag,id;
    int lastmainid=0;
    while(row!=NULL)
    {
        id = atoi(row[0]);
        mainflag = atoi(row[1]);
        if(mainflag ==1 )
        {
            lastmainid = id;
            
        }
        if(lastmainid>0 &&  mainflag!=1)
        {
            FmIC->gdb.VQuery(512,"update authinput set mainid=%d where sid=%d",lastmainid,id);
        }
        row = res.Fetch();  
    }

    ModalResult = mrOk; 
    
}
//---------------------------------------------------------------------------
void __fastcall TFormWsCheck::txtAmountKeyPress(TObject *Sender, char &Key)
{
    if(Key == '\r')
    {
        btnSave->SetFocus();
    }

    MoneyKeyPress( txtAmount, Key);    
}
//---------------------------------------------------------------------------


void __fastcall TFormWsCheck::txtAmountChange(TObject *Sender)
{
     MoneyChange( txtAmount );        
}
//---------------------------------------------------------------------------

void __fastcall TFormWsCheck::txtAmountEnter(TObject *Sender)
{
     bPoint = false;        
}
//---------------------------------------------------------------------------

void __fastcall TFormWsCheck::FormCreate(TObject *Sender)
{
    bFormatMoney = false;        
}
//---------------------------------------------------------------------------



void TFormWsCheck::MoneyChange(TEdit * txtMoney)
{
    //TODO: Add your source code here
    if( txtMoney->Text.IsEmpty() ) txtMoney->Text = "0";
    if( bFormatMoney ) return;
    bFormatMoney = true;

    txtMoney->Text = RemoveThousandChar( txtMoney->Text );
    txtMoney->Text = AddThousandChar( txtMoney->Text );

    //自动加小数点
    txtMoney->SelStart = txtMoney->Text.Pos(".")-1+nPopos;
    txtMoney->SelLength = 0;

    bFormatMoney = false;
}

void TFormWsCheck::MoneyKeyPress(TEdit* txtMoney, char & Key)
{
    //TODO: Add your source code here
     if( txtMoney->SelText == txtMoney->Text ) bPoint = false;

	if( Key == 13 || Key == 27 ) return;
    if( Key == 8 )
    {
        txtMoney->SelectAll();
        Key = 0;
        return;
    }

    if( Key == '.' )
    {
        txtMoney->SelStart = txtMoney->Text.Pos(".");
        bPoint = true;
        nPopos = 0 + (int)bPoint;
    }
 	if( Key < '0' || Key > '9' ) Key = 0;
    else nPopos = (nPopos+1) * (int)bPoint;
}

