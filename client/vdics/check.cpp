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

#include "check.h"
#include "main.h"
#include <zcommon.h>
#include <transimg.h>
#include "login.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCheck *FormCheck;
//---------------------------------------------------------------------------
__fastcall TFormCheck::TFormCheck(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


int TFormCheck::setMethod(int type, int sid)
{
    //TODO: Add your source code here
     _type = type;
    _sid   = sid;
    _curSid = sid;
    lstFile->Clear();
    int ret =  FmIC->gdb.VQuery(512,"select distinct sid,fname from multis where envid=%d or sid=%d",sid,sid);
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
       li->ImageIndex = 1 ;
       row = res.Fetch();
    }      
}
void __fastcall TFormCheck::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormCheck::imageMouseUp(TObject *Sender,
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
void __fastcall TFormCheck::FormShow(TObject *Sender)
{
    image->Align = alClient;
    image->Left = 1;
    image->Top = 1;
    image->AutoSize = false;
    image->Tag = 1;
    txtAmount->SetFocus();
    SetCtrlStyle(txtAmount,ES_NUMBER);
    SetCtrlStyle(txtVchtype,ES_NUMBER);     

    if(lstFile->Items->Count>0) lstFile->Items->Item[0]->Selected = true;

}
//---------------------------------------------------------------------------
void __fastcall TFormCheck::lstFileSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
    TListItem* li;
    li= lstFile->Selected;
    if(li==NULL) return ;
    int sid = li->Caption.ToInt() ;
    _curSid = sid;

    for( int i = 0 ;i<lstFile->Items->Count;i++ )
    {
        lstFile->Items->Item[i]->ImageIndex = -1 ;
    }
    
    li->ImageIndex = 0 ;

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,substate,amount,model,fname,bname,right(pkgno,5),vchtype,mainflag,date,session  from multis where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
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

    txtVchtype->Text = row[8];
    chkMain->Checked = (atoi(row[9]) == 1);

    if(vchflag == 2 || vchflag ==6 || vchflag == 7)
    {
         chkMain->Visible = false;
         txtAmount->Visible = true;

         txtEnveAmount->Text = row[3];
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
    FormCheck->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    //主件金额
    ret = FmIC->gdb.VQuery(512,"select sum(amount) from multis where mainflag=1 and envid=%d",_sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    res = FmIC->gdb.Store();
    row = res.Fetch();

    char cmd[20];
    memset(cmd,0,sizeof(cmd));
    if( row[0]!="" && row[0]!=NULL)
        sprintf(cmd,"%.2f",atof(row[0]));
    else
        strcpy(cmd,"0");

    txtTotalAmount->Text = cmd;

    //信封金额
    ret = FmIC->gdb.VQuery(512,"select sum(amount) from multis where sid=%d",_sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    res = FmIC->gdb.Store();
    row = res.Fetch();
    memset(cmd,0,sizeof(cmd));
    if( row[0]!="" && row[0]!=NULL)
        sprintf(cmd,"%.2f",atof(row[0]));
    else
        strcpy(cmd,"0");
    txtEnveAmount->Text = cmd;

    String total  = RemoveThousandChar(txtEnveAmount->Text.c_str());
    String totald = RemoveThousandChar(txtTotalAmount->Text.c_str());

    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"%.2f",(total.ToDouble()-totald.ToDouble()));

    txtDifferAmount->Text = cmd;
    txtAmount->SetFocus();


    return ;
}
//---------------------------------------------------------------------------
void __fastcall TFormCheck::chkMainClick(TObject *Sender)
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
void __fastcall TFormCheck::btnSaveClick(TObject *Sender)
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

    if(amt.ToDouble()<0.001 &&mainflag == 1)
    {
        MessageBox(Handle,"金额错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    int ret = FmIC->gdb.VQuery(512,"update multis set amount='%s',mainflag=%d ,clkno='%s' where sid=%d",amt.c_str(),mainflag,g_sys_opt.c_str(),_curSid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }

    ret = FmIC->gdb.VQuery(512,"update multis set vchtype='%s' where envid=%d",txtVchtype->Text.c_str(),_sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }

    TListItem* li;
    li= lstFile->Selected;
    if(li==NULL) return ;

    int sid = li->Index+2;
    if(sid> lstFile->Items->Count )
    {
        ret = FmIC->gdb.VQuery(512,"select sum(amount) from multis where mainflag=1 and envid=%d",_sid);
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }
        MyRes res = FmIC->gdb.Store();
        MyRow row = res.Fetch();


        char cmd[20];
        memset(cmd,0,sizeof(cmd));
        if( row[0]!="" && row[0]!=NULL)
            sprintf(cmd,"%.2f",atof(row[0]));
        else
            strcpy(cmd,"0");

        txtTotalAmount->Text = cmd;

        //信封金额
        ret = FmIC->gdb.VQuery(512,"select sum(amount) from multis where sid=%d",_sid);
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }
        res = FmIC->gdb.Store();
        row = res.Fetch();
        memset(cmd,0,sizeof(cmd));
        if( row[0]!="" && row[0]!=NULL)
            sprintf(cmd,"%.2f",atof(row[0]));
        else
            strcpy(cmd,"0");
        txtEnveAmount->Text = cmd;

        String total  = RemoveThousandChar(txtEnveAmount->Text.c_str());
        String totald = RemoveThousandChar(txtTotalAmount->Text.c_str());

        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"%.2f",(total.ToDouble()-totald.ToDouble()));

        txtDifferAmount->Text = cmd;
        if( MessageBox(Handle,"处理完毕,是否核打?\t","提示",MB_YESNO|MB_ICONINFORMATION)==IDYES )
        {
            btnCheckClick(0);
        }
        return ;
    }

    image->AutoSize = false;
    image->Align = alClient;
    image->Proportional = true;
    image->Stretch = true;
    image->Tag = 0;     
    
    TListItem* nli;
    nli = lstFile->Items->Item[li->Index+1];
    nli->Selected = true;

    nli->WorkArea();

}
//---------------------------------------------------------------------------
void __fastcall TFormCheck::btnCheckClick(TObject *Sender)
{
    int ret = FmIC->gdb.VQuery(512,"select amount from multis where sid=%d",_sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    String amt = row[0];

    //单个信封主件总金额
    ret = FmIC->gdb.VQuery(512,"select sum(amount) from multis where mainflag=1 and envid=%d",_sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    res = FmIC->gdb.Store();
    row = res.Fetch();

    String amt2 = row[0];

    if(amt2=="" || amt2==NULL) amt2="0";
    if(amt=="" || amt==NULL) amt="0";

    if(amt.ToDouble() != amt2.ToDouble() )
    {
        MessageBox(Handle,"总金额不对!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    ret = FmIC->gdb.VQuery(512,"update multis set checkstate=1 where envid=%d",_sid);
    ret = FmIC->gdb.VQuery(512,"update multis set mainflag=0  where amount='0.00' and envid=%d",_sid);

    ret = FmIC->gdb.VQuery(512,"select sid,mainflag from multis where envid=%d order by sid",_sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
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
            FmIC->gdb.VQuery(512,"update multis set mainid=%d where sid=%d",lastmainid,id);
        }
        row = res.Fetch();
    }

    ModalResult = mrOk;

}
//---------------------------------------------------------------------------
void __fastcall TFormCheck::txtAmountKeyPress(TObject *Sender, char &Key)
{


    MoneyKeyPress( txtAmount, Key);
    if(Key == '\r')
    {
        txtVchtype->SetFocus();
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormCheck::txtAmountChange(TObject *Sender)
{
     MoneyChange( txtAmount );
}
//---------------------------------------------------------------------------

void __fastcall TFormCheck::txtAmountEnter(TObject *Sender)
{
     bPoint = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormCheck::FormCreate(TObject *Sender)
{
    bFormatMoney = false;
}
//---------------------------------------------------------------------------

void TFormCheck::MoneyChange(TEdit * txtMoney)
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
void TFormCheck::MoneyKeyPress(TEdit* txtMoney, char & Key)
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

void __fastcall TFormCheck::txtVchtypeKeyPress(TObject *Sender, char &Key)
{
    if(Key == '\r')
    {
        if(btnSave->Visible == true)
            btnSave->SetFocus();
        else
            btnExit->SetFocus();
    }    
}
//---------------------------------------------------------------------------

void __fastcall TFormCheck::txtAmountExit(TObject *Sender)
{
    if( RemoveThousandChar(txtAmount->Text).ToDouble()<0.001)
    {
        chkMain->Visible = false;
        chkMain->Checked = false;
    }
        
}
//---------------------------------------------------------------------------        


void __fastcall TFormCheck::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    image->AutoSize = false;
    image->Align = alClient;
    image->Proportional = true;
    image->Stretch = true;
    image->Tag = 0;          
}
//---------------------------------------------------------------------------

