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

#include "wsimages.h"
#include <zcommon.h>
#include <transimg.h>
#include "login.h"
#include "more.h"
#include "main.h"
#include "collection.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormWsImage *FormWsImage;
//---------------------------------------------------------------------------
__fastcall TFormWsImage::TFormWsImage(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormWsImage::FormShow(TObject *Sender)
{
    image->Align = alClient;
    image->Left = 1;
    image->Top = 1;
    image->AutoSize = false;
    image->Tag = 1;
    txtAmount->SetFocus();
    SetCtrlStyle(txtVchtype,ES_NUMBER);

}
//---------------------------------------------------------------------------
void __fastcall TFormWsImage::imageMouseUp(TObject *Sender,
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

void __fastcall TFormWsImage::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
     if(Key==13||Key==40||Key==34)  //回车 下箭头 pgdn
        PostMessage(Handle,WM_NEXTDLGCTL,0,0);
}
//---------------------------------------------------------------------------


void __fastcall TFormWsImage::btnSaveClick(TObject *Sender)
{  
    if( txtAmount->Text.IsEmpty() )
    {
        MessageBox(Handle,"金额不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtAmount->SetFocus();
        ModalResult = mrNone;
        return;
    } 

    if( txtVchtype->Text.IsEmpty() )
    {
        MessageBox(Handle,"类型不能为空!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtVchtype->SetFocus();
        ModalResult = mrNone;
        return;
    }

    if( type == 1 )
    {
        int ret;

        if( multiflag<3 &&  txtType->ItemIndex  == 1 && state==1)
        {
            if( checkExists(curID,txtAmount->Text.c_str())==false )
            {
                if(MessageBox(Handle,"金额不匹配,是否保存?\t","提示",MB_YESNO|MB_ICONINFORMATION) == IDNO)
                {
                    txtAmount->SetFocus();
                    ModalResult = mrNone;
                    return;
                }
            }
        }
        if(txtType->ItemIndex  == 0)
        {
            ret = FmIC->gdb.VQuery(512,"select model,mergestate,mergeid from authinput where model in (2,6,7) and mergestate=1 and sid=%d",curID);
            if(ret)
            {
                MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                return;
            }
            MyRes res2 = FmIC->gdb.Store();
            MyRow row2 = res2.Fetch();
            
            if( row2!=NULL )
            {
                long distid = atoi(row2[2]);
                ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=0 where sid=%d",distid);   
            }

            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,vchtype='%s',mergestate=0,envid=0,mainflag=0,model=2,clkno='%s' where sid=%d",RemoveThousandChar(txtAmount->Text.c_str()),txtVchtype->Text.c_str(),g_sys_opt,curID);
        }
        else if (txtType->ItemIndex  == 1 )
        {
            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,vchtype='%s',mergestate=0,mainflag=1,model=0,clkno='%s' where sid=%d",RemoveThousandChar(txtAmount->Text.c_str()),txtVchtype->Text.c_str(),g_sys_opt,curID);
        }
        else
            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,vchtype='%s',mergestate=0,mainflag=2,model=0,clkno='%s' where sid=%d",RemoveThousandChar(txtAmount->Text.c_str()),txtVchtype->Text.c_str(),g_sys_opt,curID);

        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }

        //如果是第一联则自动更新第二联的金额
        if(txtType->ItemIndex  == 1 && state==1 && multiflag<3)
        {
            ret = FmIC->gdb.VQuery(512,"select sid, mainflag,mergestate  from authinput where model not in(2,6,7) and  sid>%d order by sid limit 1 ",curID);
            if(ret)
            {
                txtAmount->SetFocus();
                ModalResult = mrNone;
                MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                return;
            }

            MyRes res = FmIC->gdb.Store();
            MyRow row = res.Fetch();
            if(row!=NULL)
            {
                int sid         = atoi(row[0]);
                int mainflag    = atoi(row[1]);
                int mergestate  = atoi(row[2]);

                if( mainflag ==2 && mergestate==0 )
                {
                    ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',vchtype='%s',state=1,clkno='%s' where sid=%d",RemoveThousandChar(txtAmount->Text.c_str()),txtVchtype->Text.c_str(),g_sys_opt.c_str(),sid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        txtAmount->SetFocus();
                        ModalResult = mrNone;
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }
                }
            }          
        }
  /*      if(FormWS->lstMerge->Items->Count>0 && (FormWS->cmbIdentify->ItemIndex == 3 || FormWS->cbMerge->ItemIndex == 2)  )
        {               
            ret =  FormWS->getNext();
            if(ret)
                ModalResult = mrOk;
            else
                ModalResult = mrNone;     
        }
      */
    }
}
//---------------------------------------------------------------------------


void TFormWsImage::MoneyChange(TEdit * txtMoney)
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

void TFormWsImage::MoneyKeyPress(TEdit* txtMoney, char & Key)
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

//---------------------------------------------------------------------------


void __fastcall TFormWsImage::txtAmountKeyPress(TObject *Sender,
      char &Key)
{
     MoneyKeyPress( txtAmount, Key);    
}
//---------------------------------------------------------------------------

void __fastcall TFormWsImage::txtAmountEnter(TObject *Sender)
{
     bPoint = false;    
}
//---------------------------------------------------------------------------

void __fastcall TFormWsImage::txtAmountChange(TObject *Sender)
{
    MoneyChange( txtAmount );    
}
//---------------------------------------------------------------------------

void __fastcall TFormWsImage::FormCreate(TObject *Sender)
{
     bFormatMoney = false;    
}
//---------------------------------------------------------------------------        
bool TFormWsImage::checkExists(int first, String amount)
{
    //TODO: Add your source code here
    bool firstck = false;
    bool secondck = false;


    int ret = FmIC->gdb.VQuery(512,"select count(*) from authinput where  sid=%d and  amount='%s' ",first,amount.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return false;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    if(atoi(row[0])>0)
        firstck = true;


    ret = FmIC->gdb.VQuery(512,"select sid, mainflag,mergestate,amount from authinput where sid>%d order by sid limit 1 ",first);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return false;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();
    if(row!=NULL)
    {
        int sid         = atoi(row[0]);
        int mainflag    = atoi(row[1]);
        int mergestate  = atoi(row[2]);

        if( mainflag ==2 && mergestate==0 )
        {
            if( strcmp(amount.c_str(),row[3]) == 0 )
                secondck = true;
        }
    }
    
    if(firstck == true || secondck == true) return true;
    else    return false;
}
