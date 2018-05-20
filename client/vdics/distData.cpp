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
#include <transimg.h>
#include <libmem.h>

#include <jpeg.hpp>
#include "distData.h"

#include "main.h"
#include "distimage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdistForm *distForm;

//---------------------------------------------------------------------------
__fastcall TdistForm::TdistForm(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TdistForm::btnExitClick(TObject *Sender)
{
    Close(); 
}
//---------------------------------------------------------------------------
void __fastcall TdistForm::FormShow(TObject *Sender)
{

    FmIC->getDS("date",date);
    FmIC->getDS("session",session);
    txtDate->Text = date;
    txtSession->Text = session;
    txtNetno->Clear();

    int ret = FmIC->gdb.VQuery(512,"select distinct exchno from dists  where date='%s' and session='%s' order by exchno",date,session);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    txtNetno->Items->Add("全部");
    while(row!=NULL)
    {
        txtNetno->Items->Add(row[0]);
        row = res.Fetch();
    }

    txtNetno->ItemIndex = 0 ;
    flushData();

}
//---------------------------------------------------------------------------


int TdistForm::flushData()
{
    //TODO: Add your source code here      
    String cond;      

    if(txtNetno->ItemIndex == 0 )
        cond =  " and  1= 1";
    else
        cond = AnsiString(" and exchno = '")+txtNetno->Text.c_str()+"'";

    if(txtMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(txtMerge->ItemIndex == 1 )
        cond = cond + " and  mergestate!=0  ";
    else
        cond = cond + " and mergestate=0  ";

    if(cbType->ItemIndex == 0 )
        cond = cond + " and  1=1  ";
    else if(cbType->ItemIndex == 1 )
        cond = cond + " and  isenv=0  ";
    else
        cond = cond + " and  isenv=1  ";

    cond = cond + AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";

    String str = "select sid,exchno,accno,vchno,vchtype,amount,mergestate,mergeid,isenv,modified,clkno  from dists where 1=1 ";
    str = str +cond;
    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
         return ret;
    }

   txtDetail->Items->BeginUpdate();
   txtDetail->Clear();
   TListItem* li;
   MyRes res = FmIC->gdb.Store();
   MyRow row = res.Fetch();
   while(row!=NULL)
   {
       li = txtDetail->Items->Add();
       li->Caption = row[0];
       li->SubItems->Insert(0,row[1]);
       li->SubItems->Insert(1,row[2]);
       li->SubItems->Insert(2,row[3]);
       li->SubItems->Insert(3,row[4]);
       li->SubItems->Insert(4,row[5]);

       if(atoi(row[6])== 0 )
           li->SubItems->Insert(5,"未匹配");
       else
           li->SubItems->Insert(5,"已匹配");

       li->SubItems->Insert(6,row[7]); 
     

       if(atoi(row[8])== 1 )
           li->SubItems->Insert(7,"信封");
       else
           li->SubItems->Insert(7,"票据");


       if(atoi(row[9])== 0 )
           li->SubItems->Insert(8,"普通模式");
       else
           li->SubItems->Insert(8,"交互模式");

       li->SubItems->Insert(9,row[10]);

       row = res.Fetch();
   }

   txtDetail->Items->EndUpdate();

   str = "select sum(amount) ,count(*) from dists where cdcode=0 ";
   str = str +cond;
   ret = FmIC->gdb.Query(str.c_str());
   if(ret)
   {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
   }

   res = FmIC->gdb.Store();
   row = res.Fetch();

   stBar->Panels->Items[0]->Text = String("交换号[")+txtNetno->Text.c_str()+"]";
   stBar->Panels->Items[1]->Text = AnsiString("借方总金额[")+ row[0]+"]";
   stBar->Panels->Items[2]->Text = AnsiString("借方总笔数[")+ row[1]+"]";

   str = "select sum(amount) ,count(*) from dists where cdcode=1 ";
   str = str +cond;
   ret = FmIC->gdb.Query(str.c_str());
   if(ret)
   {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
   }

   res = FmIC->gdb.Store();
   row = res.Fetch();

   stBar->Panels->Items[3]->Text = AnsiString("贷方总金额[")+ row[0]+"]";
   stBar->Panels->Items[4]->Text = AnsiString("贷方总笔数[")+ row[1]+"]";
   
   return 0;

}
void __fastcall TdistForm::btnRefreshClick(TObject *Sender)
{
    Screen->Cursor = crHourGlass;
    flushData();
    Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TdistForm::txtIdentifyCloseUp(TObject *Sender)
{
    flushData();
}
//---------------------------------------------------------------------------

void __fastcall TdistForm::txtSendCloseUp(TObject *Sender)
{
    flushData();
}
//---------------------------------------------------------------------------


void __fastcall TdistForm::txtDetailDblClick(TObject *Sender)
{
    TListItem* li;
    li= txtDetail->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select sid, accno,vchno,vchtype,amount ,mergestate,fname,ip ,0,date,session,isenv,modified,clkno from dists where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return ;

    char ip[24];
    char fname[255];


    memset(ip,0,sizeof(ip));
    memset(fname,0,sizeof(fname)); 

    distFormImage->txtAccno->Text = row[1];;
    distFormImage->txtVchno->Text = row[2];

    distFormImage->txtVchtype->Text = row[3];

    distFormImage->txtAmount->Text = row[4];

    distFormImage->cbType->ItemIndex = atoi(row[11]);

    if(atoi(row[5])!=0)
    {
        distFormImage->btnSave->Enabled = false;
        distFormImage->btnSave->Visible = false;
    }
    else
    {
        distFormImage->btnSave->Enabled = true;
        distFormImage->btnSave->Visible = true;
    }

    distFormImage->txtAmount->Enabled = false;

    sprintf(fname,"%s//%02d//dists//%s",row[9],atoi(row[10]),row[6]);
    MemT image;
    int sizex;
    int sizey;
    int panel;

    
    ret =  ti_getfile( fname, row[7],image);
    if(ret)
    {
        MessageBox(Handle,"获取图像错误","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }


    TMemoryStream* pms = new TMemoryStream();
    pms->Clear();
    pms->SetSize((int)image.size());
    ret = pms->Write(image.data(),image.size());
    pms->Position = 0;
    distFormImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();
    distFormImage->curID =  sid;
    ret = distFormImage->ShowModal();

    if(ret == mrOk )
    {       
        flushData(); 
    }
}
//---------------------------------------------------------------------------

void __fastcall TdistForm::txtDetailColumnClick(TObject *Sender,
      TListColumn *Column)
{
     int i,m,n,j;
    static bool od=true;

    txtDetail->Items->BeginUpdate();

    od=!od;
    n = Column->Index;
    m = txtDetail->Items->Count;
    txtDetail->Items->Add();

    Screen->Cursor = crHourGlass;

    //按序号进行排序
    if(n ==0 )
    {
         for(i=0;i< m-1;i++)
            for(j=i+1;j< m;j++)

            if(od)
            {
                if(txtDetail->Items->Item[i]->Caption.ToDouble()  >
                   txtDetail->Items->Item[j]->Caption.ToDouble())
                {
                    setitem(m,i,j);
                }
            }
            else
            {
                if(txtDetail->Items->Item[i]->Caption.ToDouble()<
                   txtDetail->Items->Item[j]->Caption.ToDouble())
                {
                    setitem(m,i,j);
                }
            }
    }

    if(n ==1 || n==2 || n == 4  || n ==5 )
    {
         for(i=0;i< m-1;i++)
            for(j=i+1;j< m;j++)

            if(od)
            {
                if(txtDetail->Items->Item[i]->SubItems->Strings[n-1].ToDouble()  >
                   txtDetail->Items->Item[j]->SubItems->Strings[n-1].ToDouble())
                {
                    setitem(m,i,j);
                }

            }
            else
            {
                if(txtDetail->Items->Item[i]->SubItems->Strings[n-1].ToDouble()<
                   txtDetail->Items->Item[j]->SubItems->Strings[n-1].ToDouble())
                {
                    setitem(m,i,j);
                }
            }
    }
    else
    {
        for(i=0;i< m-1;i++)
            for(j=i+1;j< m;j++)

            if(od)
            {
                if(sg(txtDetail->Items->Item[i]->SubItems->Text,n-1) >
                   sg(txtDetail->Items->Item[j]->SubItems->Text,n-1))
                {
                    setitem(m,i,j);
                }

            }
            else
            {
                if(sg(txtDetail->Items->Item[i]->SubItems->Text,n-1) <
                   sg(txtDetail->Items->Item[j]->SubItems->Text,n-1))
                {
                    setitem(m,i,j);
                }
            }
    }
    
    txtDetail->Items->Delete(m);
    txtDetail->Items->EndUpdate();
    Screen->Cursor = crDefault;
    return;    
}
//---------------------------------------------------------------------------


int TdistForm::setitem(int m, int i,int j)
{
    //TODO: Add your source code here
    txtDetail->Items->Item[m]=
    txtDetail->Items->Item[i];

    txtDetail->Items->Item[i]=
    txtDetail->Items->Item[j];

    txtDetail->Items->Item[j]=
    txtDetail->Items->Item[m];

    return 0;
}

AnsiString TdistForm::sg(AnsiString str, int n)
{
    int l,i;
    AnsiString qq(str);
    l=qq.Length();
    AnsiString p(str);
    char a[]="\r\n";
    int j=0,k=0;

    for(j=1;j< l;j++)
    {
        i=1;
        while(qq[j]!=a[0] && qq[j+1]!=a[1])
        {
            p[i++]=qq[j];
            j++;
        }
        k++;
        if((k-1)==n)
        break;
    }
    p[i]=NULL;

    return (p);
}

void __fastcall TdistForm::FindDlgFind(TObject *Sender)
{
    String find_str = FindDlg->FindText;
    FindDlg->CloseDialog();
    if( find_str.IsEmpty() ) return;

    int sub_no, start_index;
    start_index = txtDetail->Selected? txtDetail->Selected->Index:0;
    txtDetail->SetFocus();
    if(start_index == 0) txtDetail->Items->Item[0]->Focused = true;
    bool found = false;

    //从当前位置开始查询
    for( int i=start_index+1; i<txtDetail->Items->Count; i++ )
    {
        TListItem* pItem = txtDetail->Items->Item[i];
        if( FindResult(find_str,pItem,false) )
        {
            pItem->MakeVisible(false);
            pItem->Selected = true;
            pItem->Focused= true;
            found = true;
            break;
        }
    }

    //如果没有找到,从头开始查询
    if( !found )
    {
        for( int i=0; i<start_index; i++ )
        {
            TListItem* pItem = txtDetail->Items->Item[i];
            if( FindResult(find_str,pItem,false) )
            {
                pItem->MakeVisible(false);
                pItem->Selected = true;
                pItem->Focused = true;
                break;
            }
        }
    }        
}
//---------------------------------------------------------------------------


bool TdistForm::FindResult(String StrToFind, TListItem* pItem, bool bExact)
{
    //TODO: Add your source code here
     //按帐号进行查找
    String sub_str = pItem->SubItems->Strings[1];
    bool found = (sub_str.Pos(StrToFind)>0);

    if( found ) return true;

    //按凭证号进行查找
    sub_str = pItem->SubItems->Strings[2];
    found = (sub_str.Pos(StrToFind)>0);

    if( found ) return true;

    //按匹配流水进行查找
    sub_str = pItem->SubItems->Strings[6];
    found = (sub_str.Pos(StrToFind)>0);

    if( found ) return true;

    //按金额进行查询
    sub_str = pItem->SubItems->Strings[4];
  
    return (sub_str.Pos(StrToFind)>0);
}
void __fastcall TdistForm::N3Click(TObject *Sender)
{
     FindDlg->Execute();
}
//---------------------------------------------------------------------------


void __fastcall TdistForm::R1Click(TObject *Sender)
{
    flushData();    
}
//---------------------------------------------------------------------------

void __fastcall TdistForm::txtMergeCloseUp(TObject *Sender)
{
     flushData();    
}
//---------------------------------------------------------------------------

void __fastcall TdistForm::txtNetnoDropDown(TObject *Sender)
{
     txtNetno->Clear();

    int ret = FmIC->gdb.Query("select distinct exchno from dists order by exchno");
    if(ret)
    {
        MessageBox(Handle,"查询出错","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    txtNetno->Items->Add("全部");
    while(row!=NULL)
    {
        txtNetno->Items->Add(row[0]);
        row = res.Fetch();
    }
    txtNetno->ItemIndex = 0 ;

}
//---------------------------------------------------------------------------    

void __fastcall TdistForm::txtNetnoChange(TObject *Sender)
{
    flushData();    
}
//---------------------------------------------------------------------------

void __fastcall TdistForm::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
    flushData();    
}
//---------------------------------------------------------------------------

