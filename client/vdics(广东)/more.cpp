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
#include <transimg.h>
#include <libmem.h>
#include <bcbext.h>
#include "more.h"
#include "main.h"

#include <bcbext.h>

#include "envimage.h"
#include "images.h"

#include "image.h"
#include "check.h"
#include "login.h"
#include "collection.h"
#include "distimage.h"
#include "warrant.h"
#include "pubfun.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmEnv *FmEnv;

//---------------------------------------------------------------------------
void utf8_to_ansi(char* src, char* &dest) 
{
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, src, strlen(src), NULL, 0);//测试转换
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, src, strlen(src), wszString, wcsLen);  //utf8 转Unicode
	wszString[wcsLen] = '\0';

	int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	dest = new char[ansiLen + 1];        //unicode版对应的strlen是wcslen
	::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), dest, ansiLen, NULL, NULL);
	dest[ansiLen] = '\0';
	delete [] wszString;
}
//---------------------------------------------------------------------------
__fastcall TFmEnv::TFmEnv(TComponent* Owner)
    : TForm(Owner)
{
    locked = false;
}
//---------------------------------------------------------------------------
void __fastcall TFmEnv::btnExitClick(TObject *Sender)
{
    if(MessageBox(Handle,"确实要退出?\t","提示",MB_YESNO|MB_ICONINFORMATION) == IDNO) return;
    Close();
}
//---------------------------------------------------------------------------


int TFmEnv::flushData1()
{
    flushdate();      
//------------------------------------------------------------------------
    //TODO: Add your source code here
    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"'";

    if(cmbNetno->ItemIndex == 0 )
    {
        cond = cond + " and 1= 1";
    }
    else
    {
    //--------------------------------------------------------------------20150511
        if(cmbNetno->Text.Length() != 6)
                cond = cond + AnsiString(" and pkgno = '")+cmbNetno->Text.c_str()+"'";
        else
   //---------------------------------------------------------------------20150511
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";
    }

    cond = cond + " and area='"+g_sys_area.c_str()+"'";

    if(cmbIdentify->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbIdentify->ItemIndex == 1 )
        cond = cond + " and state=1  ";
    else if(cmbIdentify->ItemIndex == 2 )
        cond = cond + " and ( state =0 or state = 2)  ";
    else
        cond = cond + " and ( state=3 or state=4 or state=5) ";
    if(cmbMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbMerge->ItemIndex == 1 )
        cond = cond + " and mergestate=1  ";
    else
        cond = cond + " and mergestate!=1  ";

    if(cmbNetState->ItemIndex == 1 )
        cond = cond + " and substate=1  order by sid";
    else
        cond = cond + " and substate!=1  order by sid ";

    String str = "select sid,exchno,fname,model,vchtype, amount,state,mergestate ,mainflag,mainid,sendstate,imageid,senddesc from multis where 1=1 ";
    str = str +cond;

    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
         return ret;
    }

    lstMerge->Items->BeginUpdate();
    lstMerge->Clear();
    TListItem* li;

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    while(row!=NULL)
    {
       li = lstMerge->Items->Add();
       li->Caption = row[0];

//----------------------------------------------------------------------20150511
        if(cmbNetno->ItemIndex != 0)
                li->SubItems->Insert(0,cmbNetno->Text);
        else
//----------------------------------------------------------------------20150511
                li->SubItems->Insert(0,row[1]);
       li->SubItems->Insert(1,row[2]);
       if((atoi(row[3]) == 2) || (atoi(row[3]) == 200))
            li->SubItems->Insert(2,"信封票据");
       else if(atoi(row[3]) == 6)
            li->SubItems->Insert(2,"代收票据");
       else if(atoi(row[3]) == 7)
            li->SubItems->Insert(2,"代付票据");
       else
            li->SubItems->Insert(2,"");
       
       li->SubItems->Insert(3,row[4]);
       li->SubItems->Insert(4,row[5]);
       if(atoi(row[6]) == 0 )
            li->SubItems->Insert(5,"未识别");
       else  if(atoi(row[6]) == 1 )
            li->SubItems->Insert(5,"已识别");
       else  if(atoi(row[6]) == 3 )
            li->SubItems->Insert(5,"处理失败");
       else  if(atoi(row[6]) == 5 )
            li->SubItems->Insert(5,"识别失败");
       else
            li->SubItems->Insert(5,"识别中");

       if(atoi(row[7]) == 1 )
            li->SubItems->Insert(6,"已匹配");
       else
            li->SubItems->Insert(6,"未匹配");

      if(atoi(row[8]) == 1 )
       {
            li->SubItems->Insert(7,"主件");
       }
       else
            li->SubItems->Insert(7,"附件");

       li->SubItems->Insert(8,row[9]);

       if(atoi(row[10])== 1 )
           li->SubItems->Insert(9,"已送");
       else if(atoi(row[10])== 0)
           li->SubItems->Insert(9,"未送");
       else if(atoi(row[10])== 3)
           li->SubItems->Insert(9,"上送失败");
       else if(atoi(row[10])== 2)
           li->SubItems->Insert(9,"上送中");
       else
           li->SubItems->Insert(9,"未知错误");

       li->SubItems->Insert(10,row[11]);
 //------------20150415------------显示上送错误原因
        char *tmp = NULL;
        try
        {
                utf8_to_ansi(row[12], tmp);
        }
        catch(...)
        {
		strcpy(tmp,row[12]);
        }
       li->SubItems->Insert(11,tmp);
       delete tmp;

 //----------------------------------------
       row = res.Fetch();
   }

   str = "select count(*),sum(amount) from multis where  ( model=2 or model=6 or model=7 )  ";

   str = str +cond;
   ret = FmIC->gdb.Query(str.c_str());
   if(ret)
   {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
        return ret;
   }



   res =  FmIC->gdb.Store();
   row = res.Fetch();

   StatusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";
   StatusBar->Panels->Items[1]->Text = String("信封总笔数[")+row[0]+"]";
   StatusBar->Panels->Items[2]->Text = String("信封总金额[")+row[1]+"]";


   lstMerge->Items->EndUpdate();
   return 0;
}


int TFmEnv::flushData2()
{
    //TODO: Add your source code here
    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"'";

    if(cmbNetno->ItemIndex == 0 )
    {
        cond = cond + " and 1= 1";
    }
    else
    {
    //---------------------------------------------------------------------------20150511
        if(cmbNetno->Text.Length() != 6)
                cond = cond + AnsiString(" and pkgno = '")+cmbNetno->Text.c_str()+"'";
        else
    //---------------------------------------------------------------------------20150511
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";
    }

    cond = cond + " and area='"+g_sys_area.c_str()+"'";
     
    if(cmbIdentify->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbIdentify->ItemIndex == 1 )
        cond = cond + " and state=1  ";
    else if(cmbIdentify->ItemIndex == 2 )
        cond = cond + " and ( state =0 or state = 2)  ";
    else
        cond = cond + " and state=3  ";
    if(cmbMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbMerge->ItemIndex == 1 )
        cond = cond + " and   mergestate=1  ";
    else
        cond = cond + " and   mergestate!=1  ";

    if(cmbNetState->ItemIndex == 1 )
        cond = cond + " and substate=1  ";
    else
        cond = cond + " and substate!=1  ";

    String str = "select sid,exchno,fname,vchtype, amount,mergestate,mergeid from multis where ( model=2 or model=6 or model=7 or model=200) ";
    str = str +cond;
    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
         return ret;
    }

   lstEnvs->Items->BeginUpdate();
   lstEnvs->Clear();
   TListItem* li;
   MyRes res = FmIC->gdb.Store();
   MyRow row = res.Fetch();
   while(row!=NULL)
   {
       li = lstEnvs->Items->Add();
       li->Caption = row[0];

//----------------------------------------------------------------------20150511
        if(cmbNetno->ItemIndex != 0)
                li->SubItems->Insert(0,cmbNetno->Text);
        else
//----------------------------------------------------------------------20150511

       li->SubItems->Insert(0,row[1]);

       li->SubItems->Insert(1,row[2]);
       li->SubItems->Insert(2,row[3]);
       li->SubItems->Insert(3,row[4]);
       
       if(atoi(row[5]) == 1 )
            li->SubItems->Insert(4,"已匹配");
       else
            li->SubItems->Insert(4,"未匹配");
       li->SubItems->Insert(5,row[6]);
      
       row = res.Fetch();
   }


    str = "select count(*),sum(amount) from multis where  ( model=2 or model=6 or model=7 )  ";

    str = str +cond;
    ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }



   res =  FmIC->gdb.Store();
   row = res.Fetch();

   StatusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";
   StatusBar->Panels->Items[1]->Text = String("信封总笔数[")+row[0]+"]";
   StatusBar->Panels->Items[2]->Text = String("信封总金额[")+row[1]+"]";

   lstEnvs->Items->EndUpdate();
   return 0;
}

int TFmEnv::flushData3()
{
    //TODO: Add your source code here
     //TODO: Add your source code here
    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"'";

    if(cmbNetno->ItemIndex == 0 )
    {
        cond = cond + " and 1= 1";
    }
    else
    {   //-----------------------------------------------------------------------20150511
        if(cmbNetno->Text.Length() != 6)
                cond = cond + AnsiString(" and pkgno = '")+cmbNetno->Text.c_str()+"'";
        else
        //-----------------------------------------------------------------------20150511
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";
    }

    cond = cond + " and area='"+g_sys_area.c_str()+"'";
     
    if(cmbIdentify->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbIdentify->ItemIndex == 1 )
        cond = cond + " and state=1  ";
    else if(cmbIdentify->ItemIndex == 2 )
        cond = cond + " and ( state =0 or state = 2)  ";
    else
        cond = cond = cond + " and state=3  ";
    if(cmbMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbMerge->ItemIndex == 1 )
        cond = cond + " and mergestate=1  ";
    else
        cond = cond + " and mergestate!=1  ";

    if(cmbNetState->ItemIndex == 1 )
        cond = cond + " and substate=1  ";
    else
        cond = cond + " and substate!=1  ";

    String str = "select sid,exchno,fname,vchtype, amount,mergestate,mergeid from multis where mainflag=1  ";
    str = str +cond;
    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
         return ret;
    }

   lstMain->Items->BeginUpdate();
   lstMain->Clear();
   TListItem* li;
   MyRes res = FmIC->gdb.Store();
   MyRow row = res.Fetch();
   while(row!=NULL)
   {
       li = lstMain->Items->Add();
       li->Caption = row[0];

//----------------------------------------------------------------------20150511
        if(cmbNetno->ItemIndex != 0)
                li->SubItems->Insert(0,cmbNetno->Text);
        else
//----------------------------------------------------------------------20150511
       li->SubItems->Insert(0,row[1]);

       li->SubItems->Insert(1,row[2]);
       li->SubItems->Insert(2,row[3]);
       li->SubItems->Insert(3,row[4]);
       if(atoi(row[5]) == 1 )
            li->SubItems->Insert(4,"已匹配");
       else 
            li->SubItems->Insert(4,"未匹配");

       li->SubItems->Insert(5,row[6]);

       row = res.Fetch();
   }

    str = "select count(*),sum(amount) from multis where mainflag=1 ";

    str = str +cond;
    ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }



   res =  FmIC->gdb.Store();
   row = res.Fetch();

   StatusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";
   StatusBar->Panels->Items[1]->Text = String("主票总笔数[")+row[0]+"]";
   StatusBar->Panels->Items[2]->Text = String("主票总金额[")+row[1]+"]";
   
   lstMain->Items->EndUpdate();
   return 0;
}

int TFmEnv::flushData4()
{
    //TODO: Add your source code here
     //TODO: Add your source code here
    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";

    if(cmbNetno->ItemIndex == 0 )
    {
        cond = cond + " and 1= 1";
    }
    else
    {   //----------------------------------------------------------------------20150511
        if(cmbNetno->Text.Length() != 6)
                cond = cond + AnsiString(" and pkgno = '")+cmbNetno->Text.c_str()+"'";
        else
        //----------------------------------------------------------------------20150511
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";
    }

    cond = cond + " and area='"+g_sys_area.c_str()+"'";

    if(cmbIdentify->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbIdentify->ItemIndex == 1 )
        cond = cond + " and state=1  ";
    else if(cmbIdentify->ItemIndex == 2 )
        cond = cond + " and ( state =0 or state = 2)  ";
    else
        cond = cond + " and state=3  ";
    if(cmbMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbMerge->ItemIndex == 1 )
        cond = cond + " and checkstate=1  ";
    else
        cond = cond + " and checkstate!=1  ";

    if(cmbNetState->ItemIndex == 1 )
        cond = cond + " and substate=1  ";
    else
        cond = cond + " and substate!=1  ";

    String str = "select sid,exchno,fname,vchtype, amount,checkstate,mergeid from multis where mainflag=1 and multiflag=0";
    str = str +cond;
    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
         return ret;
    }

   lstSingle->Items->BeginUpdate();
   lstSingle->Clear();
   TListItem* li;
   MyRes res = FmIC->gdb.Store();
   MyRow row = res.Fetch();
   while(row!=NULL)
   {
       li = lstSingle->Items->Add();
       li->Caption = row[0];

//----------------------------------------------------------------------20150511
        if(cmbNetno->ItemIndex != 0)
                li->SubItems->Insert(0,cmbNetno->Text);
        else
//----------------------------------------------------------------------20150511
       li->SubItems->Insert(0,row[1]);

       li->SubItems->Insert(1,row[2]);
       li->SubItems->Insert(2,row[3]);
       li->SubItems->Insert(3,row[4]);
       if(atoi(row[5]) == 0 )
            li->SubItems->Insert(4,"未核打");
       else
            li->SubItems->Insert(4,"已核打");

       row = res.Fetch();
   }

   str = "select count(*),sum(amount) from multis where   mainflag=1 and multiflag=0";

    str = str +cond;
    ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }



   res =  FmIC->gdb.Store();
   row = res.Fetch();

   StatusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";
   StatusBar->Panels->Items[1]->Text = String("单主件总笔数[")+row[0]+"]";
   StatusBar->Panels->Items[2]->Text = String("单主件总金额[")+row[1]+"]";
   
   lstSingle->Items->EndUpdate();
   return 0;
}
int TFmEnv::flushData5()
{
    //TODO: Add your source code here
     //TODO: Add your source code here
    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";

    if(cmbNetno->ItemIndex == 0 )
    {
        cond = cond + " and 1= 1";
    }
    else
    {
        //----------------------------------------------------------------------20150511
        if(cmbNetno->Text.Length() != 6)
                cond = cond + AnsiString(" and pkgno = '")+cmbNetno->Text.c_str()+"'";
        else
        //----------------------------------------------------------------------20150511
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";
    }

    cond = cond + " and area='"+g_sys_area.c_str()+"'";

    if(cmbIdentify->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbIdentify->ItemIndex == 1 )
        cond = cond + " and state=1  ";
    else if(cmbIdentify->ItemIndex == 2 )
        cond = cond + " and ( state =0 or state = 2)  ";
    else
        cond = cond + " and state=3  ";
    if(cmbMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbMerge->ItemIndex == 1 )
        cond = cond + " and checkstate=1  ";
    else
        cond = cond + " and checkstate!=1  ";

    if(cmbNetState->ItemIndex == 1 )
        cond = cond + " and substate=1  ";
    else
        cond = cond + " and substate!=1  ";

    String str = "select sid,exchno,fname, vchtype,amount,mainflag,envid,checkstate from multis where multiflag=1 and (model=2 or model=6 or model=7 or model=200) ";
    str = str +cond;
    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
         return ret;
    }

     debugout("sql [%s]\n",str.c_str());
     
   lstMulti->Items->BeginUpdate();
   lstMulti->Clear();
   TListItem* li;
   MyRes res = FmIC->gdb.Store();
   MyRow row = res.Fetch();
   while(row!=NULL)
   {
       li = lstMulti->Items->Add();
       li->Caption = row[0];

//----------------------------------------------------------------------20150511
        if(cmbNetno->ItemIndex != 0)
                li->SubItems->Insert(0,cmbNetno->Text);
        else
//----------------------------------------------------------------------20150511
       li->SubItems->Insert(0,row[1]);

       li->SubItems->Insert(1,row[2]);
       li->SubItems->Insert(2,row[3]);
       li->SubItems->Insert(3,row[4]);

       if(atoi(row[5]) == 1 )
            li->SubItems->Insert(4,"主件");
       else
            li->SubItems->Insert(4,"附件");
       if(atoi(row[7]) == 1 )     
            li->SubItems->Insert(5,"已核打");
       else
            li->SubItems->Insert(5,"未核打");

      
       row = res.Fetch();
   }

    str = "select count(*),sum(amount) from multis where multiflag=1 and (model=2 or model=6 or model=7) ";

    str = str +cond;
    ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }



   res =  FmIC->gdb.Store();
   row = res.Fetch();

   StatusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";
   StatusBar->Panels->Items[1]->Text = String("多主件总笔数[")+row[0]+"]";
   StatusBar->Panels->Items[2]->Text = String("多主件总金额[")+row[1]+"]";
   

   lstMulti->Items->EndUpdate();
   return 0;
}

int TFmEnv::flushData6()
{
    //TODO: Add your source code here
     
    String cond;
    
    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";

   
    if(cmbNetno->ItemIndex == 0 )
    {
        cond = cond + " and 1= 1";
    }
    else
    {
        //----------------------------------------------------------------------20150511
        if(cmbNetno->Text.Length() != 6)
                cond = cond + AnsiString(" and pkgno = '")+cmbNetno->Text.c_str()+"'";
        else
        //----------------------------------------------------------------------20150511
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";
    }
    cond = cond + " and area='"+g_sys_area.c_str()+"'";
    
    if(cmbMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbMerge->ItemIndex == 1 )
        cond = cond = cond + " and  mergestate!=0  ";
    else
        cond = cond = cond + " and mergestate=0  ";


    String str;

    if(FmIC->gmode !=2 )
        str = "select sid,exchno,accno,vchno, vchtype,amount ,mergestate ,mergeid ,envelop from pbcdata where 1=1 ";
    else
        str = "select sid,exchno,accno,vchno, vchtype,amount ,mergestate ,mergeid ,isenv from dists where 1=1 ";
    str = str +cond;
    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
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

//----------------------------------------------------------------------20150511
        if(cmbNetno->ItemIndex != 0)
                li->SubItems->Insert(0,cmbNetno->Text);
        else
//----------------------------------------------------------------------20150511
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

       if(FmIC->gmode !=0 )
       {
        if(atoi(row[8])>= 1 )
           li->SubItems->Insert(7,"信封");
        else
           li->SubItems->Insert(7,"票据");
       }
       li->Data = row[0];
       row = res.Fetch();
   }

    if(FmIC->gmode !=2 )
        str = "select count(*),sum(amount) from pbcdata where 1=1 ";
    else
        str = "select count(*),sum(amount) from dists where 1=1 ";
    str = str +cond;
    ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }  


   res =  FmIC->gdb.Store();
   row = res.Fetch();

   StatusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";

   if(FmIC->gmode !=2 )
   {
        StatusBar->Panels->Items[1]->Text = String("人行总笔数[")+row[0]+"]";
        StatusBar->Panels->Items[2]->Text = String("人行总金额[")+row[1]+"]";
   }
   else
   {
        StatusBar->Panels->Items[1]->Text = String("清分总笔数[")+row[0]+"]";
        StatusBar->Panels->Items[2]->Text = String("清分总金额[")+row[1]+"]";
   }
   txtDetail->Items->EndUpdate();
   return 0;

}

void __fastcall TFmEnv::R1Click(TObject *Sender)
{ 
    Screen->Cursor = crHourGlass;
    try
    {
        if( pageCtrl->TabIndex == 0 )
        {
            Label7->Caption   = "匹配状态";
            flushData1();
        }
        else if( pageCtrl->TabIndex == 1 )
        {
            Label7->Caption   = "匹配状态";
            flushData2();
        }
        else if( pageCtrl->TabIndex == 2 )
        {
            Label7->Caption   = "匹配状态";
            flushData3();
        }
        else if( pageCtrl->TabIndex == 3 )
        {
            Label7->Caption   = "核打状态";
            flushData4();
        }
        else if( pageCtrl->TabIndex == 4 )
        {             
            Label7->Caption   = "核打状态";
            flushData5();
        }
        else if( pageCtrl->TabIndex == 5 )
        {
            Label7->Caption   = "匹配状态";
            flushData6();
        }
        else
        {
            Label7->Caption   = "匹配状态";
            flushData1();
        }
    }
    catch(...)
    {
        Screen->Cursor = crDefault;
    }

    if( cmbNetno->ItemIndex==0 )
    {
        btnMerge->Enabled = false;
        btnSubmit->Enabled = false;
        btnReSubmit->Enabled = false;      
    }
    else 
    {
        if(cmbNetState->ItemIndex == 0 || cmbNetState->ItemIndex == 2)
        {
            btnMerge->Enabled = true;
            btnSubmit->Enabled = true;
            btnReSubmit->Enabled = false;
        }
        else
        {
            btnMerge->Enabled = false;
            btnSubmit->Enabled = false;
            btnReSubmit->Enabled = true;
        }
    }
    Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TFmEnv::FormShow(TObject *Sender)
{
    memset(date,0,sizeof(date));
    memset(session,0,sizeof(session));
    FmIC->getDS("date",date);
    FmIC->getDS("session",session);
    txtDate->Text = date;
    txtSession->Text = session;
    pageCtrl->ActivePageIndex = 0;
    flushExchno(0);
/*------------20150417--------------------   */
 //   flushdate();
 //   flush_sendstate();
/*-----------------------------------------  */
    flushData1();

     if(FmIC->gmode !=2 )
     {
        TabSheet6->Caption = "人行数据";
     }
     else
        TabSheet6->Caption = "清分数据";


    int ret = FmIC->gdb.Query("select concat(area,vchtype),model from vchtypes");

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return ;
    }

    while(row!=NULL)
    {
        _vtMap[row[0]] = row[1];

 //       String s; s.sprintf("%s,%s",row[0],row[1]);ShowMessage(s);

        row = res.Fetch();
    }

}
//---------------------------------------------------------------------------


int TFmEnv::flushExchno(int type)
{
    //TODO: Add your source code here
    cmbNetno->Clear();
    int ret;


    if(type == 0 )
        ret = FmIC->gdb.VQuery(512,"select distinct bocctrl.exchno from bocctrl,multis where right(left(bocctrl.state,7),1)='0' and ( multis.exchno=bocctrl.exchno or bocctrl.exchno not in (select distinct exchno from multis)) and bocctrl.area='%s' and bocctrl.date='%s' and bocctrl.session='%s' order by multis.exchno",g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    else if(type == 1 )
        ret = FmIC->gdb.VQuery(512,"select distinct bocctrl.exchno from bocctrl,multis where right(left(bocctrl.state,7),1)='1' and ( multis.exchno=bocctrl.exchno or bocctrl.exchno not in (select distinct exchno from multis)) and bocctrl.area='%s' and bocctrl.date='%s' and bocctrl.session='%s' order by multis.exchno",g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    else
        ret = FmIC->gdb.VQuery(512,"select distinct bocctrl.exchno from bocctrl,multis where right(left(bocctrl.state,7),1)='2' and ( multis.exchno=bocctrl.exchno or bocctrl.exchno not in (select distinct exchno from multis)) and bocctrl.area='%s' and bocctrl.date='%s' and bocctrl.session='%s' order by multis.exchno",g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
  
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return 0;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    cmbNetno->Items->Add(" 全    部");
    while(row!=NULL)
	{
//		String s = row[0];
//		char * netno =  s.SubString(7,6).c_str();
//		cmbNetno->Items->Add(netno);
		cmbNetno->Items->Add(row[0]);
        row = res.Fetch();
    }

    cmbNetno->ItemIndex = 0 ;

    btnMerge->Enabled = false;
    btnSubmit->Enabled = false;
    btnReSubmit->Enabled = false;

}
void __fastcall TFmEnv::cmbNetStateCloseUp(TObject *Sender)
{
    if( locked == true)
    {
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,6),'0',right(state,9)) where exchno='%s' and right(left(state,7),1)!='1' and date='%s' and session=%s and area='%s'",curNetno.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str(),g_sys_area.c_str());
        locked = false;
    }
    if(cmbNetState->ItemIndex == 0 )
        flushExchno(0);
    else if(cmbNetState->ItemIndex == 1 )
        flushExchno(1);
    else
        flushExchno(2);
    R1Click(NULL);
    if( cmbNetState->ItemIndex == 1)
    {
        btnMerge->Enabled = false;
        btnSubmit->Enabled = false;
        btnReSubmit->Enabled = false;
    }
    else
    {
        btnMerge->Enabled  = true;
        btnSubmit->Enabled = true;
        btnReSubmit->Enabled = false;
    }      
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::cmbNetnoCloseUp(TObject *Sender)
{
    if( locked == true)
    {
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,6),'0',right(state,9)) where exchno='%s' and right(left(state,7),1)!='1' and area='%s' and date='%s' and session='%s'",lastNetno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
        locked = false;
    }
  

    R1Click(NULL);
    if( cmbNetno->ItemIndex==0 )
    {
        btnMerge->Enabled = false;
        btnSubmit->Enabled = false;
        btnReSubmit->Enabled = false;      
    }
    else 
    {
        if(cmbNetState->ItemIndex == 0 || cmbNetState->ItemIndex == 2)
        {
            btnMerge->Enabled = true;
            btnSubmit->Enabled = true;
            btnReSubmit->Enabled = false;
        }
        else
        {
            btnMerge->Enabled = false;
            btnSubmit->Enabled = false;
            btnReSubmit->Enabled = true;
        }
    }

    if(cmbNetno->ItemIndex!=0  && (cmbNetState->ItemIndex == 0 || cmbNetState->ItemIndex == 2))
    {
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,6),'2',right(state,9)) where exchno='%s' and right(left(state,7),1)!='1' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
        locked = true;
    }
    curNetno = cmbNetno->Text;
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::cmbNetnoDropDown(TObject *Sender)
{
    lastNetno =  curNetno;
    flushExchno(cmbNetState->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::btnSubmitClick(TObject *Sender)
{
    if(cmbNetno->ItemIndex == 0 ) return ;
    if( mergeEnvs(0) != 0 ) return;


    int ret ;
    if( FmIC->gmode ==2 )
    {
        ret = FmIC->gdb.VQuery(512,"select count(*) from pbcdata where exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }

        MyRes res = FmIC->gdb.Store();
        MyRow row = res.Fetch();

        int pt = atoi(row[0]);

        ret = FmIC->gdb.VQuery(512,"select count(*) from dists where exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }

        res = FmIC->gdb.Store();
        row = res.Fetch();

        int dt = atoi(row[0]);

        if( pt > 0 && dt == 0 )
        {
            MessageBox(Handle,"网点没有进行清分,不能提交!\t","提示",MB_OK|MB_ICONINFORMATION);
            return ;
        }

        ret = FmIC->gdb.VQuery(512,"select sum(amount) from multis where exchno='%s' and area='%s' and date='%s' and session='%s' and model in (2,6,7)",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }

        res = FmIC->gdb.Store();
        row = res.Fetch();

        String amt_env = row[0];

        ret = FmIC->gdb.VQuery(512,"select sum(amount) from multis where exchno='%s' and area='%s' and date='%s' and session='%s' and mainflag=1",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }

        res = FmIC->gdb.Store();
        row = res.Fetch();

        String amt_mainid = row[0];
        if( amt_env!=amt_mainid )
        {
            MessageBox(Handle,"信封与主件金额不符,不能提交!\t","提示",MB_OK|MB_ICONINFORMATION);
            return ;
        }
    }



    ret = FmIC->gdb.VQuery(512,"select count(*) from multis where multiflag =1 and checkstate=0 and exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    int ct = atoi(row[0]);
    if(ct > 0 )
    {
      
        pageCtrl->ActivePageIndex = 4 ;
        cmbIdentify->ItemIndex = 1;
        cmbMerge->ItemIndex = 2;

        //flushData5();
        pageCtrlChange(NULL);
        MessageBox(Handle,"多主件业务没有核打完毕，不能提交!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    if( strcmp(g_sys_area.c_str(),BRNO_BJ)== 0 )
    {
    	/*
        ret = FormWS->subMit(cmbNetno->Text.c_str());
        if(ret)
        {
            FormWS->cmbNetno->SetTextBuf(cmbNetno->Text.c_str()) ;
            FormWS->boolExchno = false;
            FormWS->ShowModal();
            return;
        }
      */
    }
    
    ret = FmIC->gdb.VQuery(512,"select pkgno ,merge from bocnets where exchno='%s' and area='%s'",cmbNetno->Text,g_sys_area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    int codeline;
    res  = FmIC->gdb.Store();
    row  = res.Fetch();

    if(row == NULL)
        codeline = 0;
    else
    {
        codeline = atoi(row[1]);
    }

    ret = FmIC->gdb.VQuery(512,"select count(*) from multis where mergestate=0 and (model=2 or model=6 or model=7) and exchno='%s' and area='%s' and  date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    res  = FmIC->gdb.Store();
    row  = res.Fetch();

    ct = atoi(row[0]);
    if(ct > 0 )
    {
        String sw = AnsiString("共[")+ct +"]个信封没有匹配完,是否提交?\t";
        if(MessageBox(Handle,sw.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION) == IDNO)
        {
            pageCtrl->ActivePageIndex  = 1 ;
            cmbMerge->ItemIndex = 2;
            flushData2();
            return ;
        }
    }


    if(FmIC->gmode !=2 )
        ret = FmIC->gdb.VQuery(512,"select count(*) from pbcdata where mergestate=0  and exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    else
         ret = FmIC->gdb.VQuery(512,"select count(*) from dists where isenv=1 and mergestate=0  and exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());

    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    res  = FmIC->gdb.Store();
    row  = res.Fetch();

    int dt = atoi(row[0]);
    if(dt > 0 && codeline == 1 )
    {
        String bShow;
        if(FmIC->gmode !=2)
        {
            bShow  =  "人行数据没有匹配完,是否强制提交?\t";

        }
        else
        {
            bShow  =  "清分数据没有匹配完,是否强制提交?\t";
        }

        if(MessageBox(Handle,bShow.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION) == IDNO)
        {
            pageCtrl->ActivePageIndex  = 5 ;
            cmbMerge->ItemIndex = 2;
            flushData6();
            return ;
        }
    }

    if( (ct > 0 || (dt>0 && codeline ==1)) && FmIC->gmode ==2 )
    {
    	
    		if(!FmIC->m_bSend)
    		{
    			MessageBox(Handle,"该柜员没有强制提交!\t","提示",MB_OK|MB_ICONINFORMATION);
    			return;
    		}
    		/*
        MessageBox(Handle,"强制提交需要授权!\t","提示",MB_OK|MB_ICONINFORMATION);

        if( FormWarrant->ShowModal() == mrCancel)
        {
            MessageBox(Handle,"授权失败,不能强制提交!\t","提示",MB_OK|MB_ICONINFORMATION);
            return;
        }
        */
    }

    
    locked = false;
    FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,6),'1',right(state,9)) where exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    FmIC->gdb.VQuery(512,"update multis set substate=1 where exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());

    MessageBox(Handle,"提交成功!\t","提示",MB_OK|MB_ICONINFORMATION);

    btnMerge->Enabled = false;
    btnSubmit->Enabled = false;

    if( strcmp(g_sys_area.c_str(),BRNO_BJ)== 0 )
    {
        ret = FmIC->gdb.VQuery(512,"select count(*) from authinput where checkstate!=1 and exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }
        res  = FmIC->gdb.Store();
        row  = res.Fetch();

        int ct = atoi(row[0]);
        if(ct>0)
        {
          //  FormWS->cmbNetno->SetTextBuf(cmbNetno->Text.c_str()) ;
           // FormWS->boolExchno = false;
           // FormWS->ShowModal();
            return;
        }
    }       
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::lstMergeDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstMerge->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int curid = lstMerge->ItemIndex;
     
    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,substate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,state,mergestate,modeflag from multis where sid=%d",sid);
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
    int  mergestate;
    int  rotate;
    int  vchflag;
    String  vchtype;
    int  state;
    int  modeflag = 0 ;

    memset(ip,0,sizeof(ip));

    memset(amount,0,sizeof(amount));
    memset(fname,0,sizeof(fname));
    memset(bname,0,sizeof(bname));
    memset(pkgno,0,sizeof(pkgno));
    strcpy(ip,row[0]);
    rotate = atoi(row[1]);
    submitstate = atoi(row[2]);

    strncpy(amount,row[3],sizeof(amount)-1);
    vchflag = atoi(row[4]);

    strcpy(fname,row[5]);
    strcpy(bname,row[6]);
    strncpy(pkgno,row[7],sizeof(pkgno)-1);
    vchtype = row[8];
    state = atoi(row[11]);
    mergestate = atoi(row[12]);
    modeflag = atoi(row[13]);

    FormEnvImage->txtType->Visible = true;
    FormEnvImage->Label1->Visible = true;

    if(submitstate!=0 || state == 0 ) FormEnvImage->btnSave->Visible = false;
    else                FormEnvImage->btnSave->Visible = true;


    if( strlen(amount)<=0) strcpy(amount,"0");

    FormEnvImage->txtVchType->Text  = vchtype;
    FormEnvImage->txtAmount->Text = amount;

    FormEnvImage->chkMode->Checked = (modeflag == 1);

    FormEnvImage->curID  = sid; 

    if(vchflag == 2 )           FormEnvImage->txtType->ItemIndex =  0 ;
    else if(vchflag == 6)       FormEnvImage->txtType->ItemIndex =  1 ;
    else if(vchflag == 7)       FormEnvImage->txtType->ItemIndex =  2 ;
    else   FormEnvImage->txtType->ItemIndex =  3 ;     

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
    pms->Position = 0;
    FormEnvImage->type  =  1;

    FormEnvImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    ti_RGB2BMPFREE(bmp);
    ret = FormEnvImage->ShowModal();

    if((cmbIdentify->ItemIndex == 3 || cmbIdentify->ItemIndex == 1)  )
    {
        R1Click(NULL);
    }
    else
    {
        li->SubItems->Strings[2] =  FormEnvImage->txtType->Text.c_str();
        li->SubItems->Strings[3] =  FormEnvImage->txtVchType->Text.c_str();
        li->SubItems->Strings[4] =  RemoveThousandChar(FormEnvImage->txtAmount->Text.c_str());
        
    }

    if(curid > lstMerge->Items->Count)
        curid = 0;

    if( lstMerge->Items->Count>curid ) lstMerge->Items->Item[curid]->Selected = true;
}
//---------------------------------------------------------------------------



void __fastcall TFmEnv::lstEnvsDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstEnvs->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int curid = lstEnvs->ItemIndex;

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
    String  vchtype;

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
    vchtype = row[8];

    FormEnvImage->btnSave->Visible = false;

    if( strlen(amount)<=0) strcpy(amount,"0");


    FormEnvImage->txtVchType->Text  = vchtype;
    FormEnvImage->txtAmount->Text = amount;
    if(vchflag == 2 )           FormEnvImage->txtType->ItemIndex =  0 ;
    else if(vchflag == 6)       FormEnvImage->txtType->ItemIndex =  1 ;
    else if(vchflag == 7)       FormEnvImage->txtType->ItemIndex =  2 ;
    else   FormEnvImage->txtType->ItemIndex =  3 ;


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
    pms->Position = 0;
    FormEnvImage->type  =  0;
    FormEnvImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    ti_RGB2BMPFREE(bmp);

    ret = FormEnvImage->ShowModal();

    if(curid > lstEnvs->Items->Count)
        curid = 0;

    if( lstEnvs->Items->Count>curid ) lstEnvs->Items->Item[curid]->Selected = true;


}
//---------------------------------------------------------------------------


void __fastcall TFmEnv::lstMainDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstMain->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int curid = lstMain->ItemIndex;

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,substate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session ,modeflag from multis where sid=%d",sid);
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
    String  vchtype;
    int  modeflag;
      
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
    vchtype = row[8];
    modeflag= atoi(row[11]);

    if( strlen(amount)<=0) strcpy(amount,"0");
    

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

    FormImages->setMethod(1,sid);
    ret = FormImages->ShowModal();

    if(curid > lstMain->Items->Count)
        curid = 0;

    if( lstMain->Items->Count>curid ) lstMain->Items->Item[curid]->Selected = true;
 

}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::lstMultiDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstMulti->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int curid = lstMulti->ItemIndex;

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,substate,amount,mainflag,fname,bname,right(pkgno,5),vchtype,date,session  from multis where sid=%d",sid);
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
    int  mainflag;
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
    mainflag = atoi(row[4]);

    strcpy(fname,row[5]);
    strcpy(bname,row[6]);
 
    if(submitstate!=0)
    {
        FormCheck->btnSave->Visible = false;
        FormCheck->btnCheck->Visible = false;
    }
    else
    {
        FormCheck->btnSave->Visible = true;
        FormCheck->btnCheck->Visible = true;
    }


    FormCheck->txtAmount->Text  = amount;
    FormCheck->txtAmount->Visible = true;
    FormCheck->chkMain->Visible = false;
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
    FormCheck->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    FormCheck->setMethod(1,sid);
    ret = FormCheck->ShowModal();
    if(ret == mrOk )
    {
        li->SubItems->Strings[5] = "已核";
        li->Delete();
        if(lstMulti->Items->Count>0 && cmbMerge->ItemIndex == 2)
        {
            
            TListItem* nli;
            nli = lstMulti->Items->Item[0];
            nli->Selected = true;
            lstMultiDblClick(NULL);
        }
        
    }

    R1Click(NULL);

    if(curid > lstMulti->Items->Count)
        curid = 0;

    if( lstMulti->Items->Count>curid ) lstMulti->Items->Item[curid]->Selected = true; 
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::lstSingleDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstSingle->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int curid = lstSingle->ItemIndex;

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,substate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,envid ,state,modeflag from multis where sid=%d",sid);
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
    String  vchtype;
    int  envid;
    int  state;
    int  modeflag;

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
    vchtype = row[8];
    envid = atoi(row[11]);
    state  = atoi(row[12]);
    modeflag  = atoi(row[13]);

    if(submitstate!=0 || state == 0 )  FormEnvImage->btnSave->Visible = false;
    else                FormEnvImage->btnSave->Visible = true;

    if( strlen(amount)<=0) strcpy(amount,"0");

    FormEnvImage->txtVchType->Text  = vchtype;
    FormEnvImage->txtAmount->Text = amount;

    FormEnvImage->txtType->Visible = false;
    FormEnvImage->Label1->Visible = false;
    FormEnvImage->chkMode->Checked = (modeflag == 1);


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
    pms->Position = 0;
    FormEnvImage->curID =  sid;
    FormEnvImage->type  =  4;
    FormEnvImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    ti_RGB2BMPFREE(bmp);
    ret = FormEnvImage->ShowModal();

    R1Click(NULL);

    if(curid > lstSingle->Items->Count)
        curid = 0;

    if( lstSingle->Items->Count>curid )lstSingle->Items->Item[curid]->Selected = true; 
}
//---------------------------------------------------------------------------


void __fastcall TFmEnv::FormClose(TObject *Sender, TCloseAction &Action)
{
    if(locked == true)    FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,6),'0',right(state,9)) where exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
}
//---------------------------------------------------------------------------


void __fastcall TFmEnv::MeForceLookClick(TObject *Sender)
{
    TListItem* li;
    int ret = 0;


    if( pageCtrl->TabIndex == 0 )
    {
        li= lstMerge->Selected;
        if(li==NULL) return ;

        int curid = lstMerge->ItemIndex;

        if(MessageBox(Handle,"是否要删除选中记录?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;


        for(int i =lstMerge->Items->Count -1 ;i>=0 ;i--)
        {
            li = lstMerge->Items->Item[i];
            if(li->Selected  == true)
            {
                int sid = li->Caption.ToInt();

                ret = FmIC->gdb.VQuery(512,"select substate,mergeid  from multis where sid=%d",sid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }
                MyRes res  = FmIC->gdb.Store();
                MyRow row  = res.Fetch();

                if(row==NULL) return ;

                int substate = atoi(row[0]) ;
                if(substate !=0)
                {
                    MessageBox(Handle,"记录已提交,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                long aissid = atoi(row[1]);

                ret = FmIC->gdb.VQuery(512,"delete from multis where sid=%d",sid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                WriteToLogFile(g_sys_opt.c_str(),"删除票据",sid,"");

                if(aissid!=0)
                {
                    ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0 ,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }

                    ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }
                }

                li->Delete();
            }
        }

        if(curid > lstMerge->Items->Count)
            curid = 0;

        if( lstMerge->Items->Count>curid ) lstMerge->Items->Item[curid]->Selected = true;
        
    }
    else if( pageCtrl->TabIndex == 1 )
    {
        li= lstEnvs->Selected;
        if(li==NULL) return ;

        if(MessageBox(Handle,"是否要删除选中记录?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

        int curid = lstEnvs->ItemIndex;

        for(int i =lstEnvs->Items->Count -1;i>=0 ;i--)
        {
            li = lstEnvs->Items->Item[i];
            if(li->Selected  == true)
            {
                int sid = li->Caption.ToInt();

                ret = FmIC->gdb.VQuery(512,"select substate,mergeid  from multis where sid=%d",sid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }
                MyRes res  = FmIC->gdb.Store();
                MyRow row  = res.Fetch();

                if(row==NULL) return ;

                int substate = atoi(row[0]) ;
                if(substate !=0)
                {
                    MessageBox(Handle,"网点已提交,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                long aissid = atoi(row[1]);

                ret = FmIC->gdb.VQuery(512,"delete from multis where (envid=%d or sid=%d) and envid>0",sid,sid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                WriteToLogFile(g_sys_opt.c_str(),"删除信封",sid,"");
                if(aissid!=0)
                {
                    ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0 ,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }

                    ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0 ,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }
                }
                li->Delete();
            }
        }

        if(curid > lstEnvs->Items->Count)
            curid = 0;

        if( lstEnvs->Items->Count>curid ) lstEnvs->Items->Item[curid]->Selected = true;
    }
    else if( pageCtrl->TabIndex == 2 )
    {
        li= lstMain->Selected;
        if(li==NULL) return ;

        if(MessageBox(Handle,"是否要删除选中记录?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

        int curid = lstMain->ItemIndex;     

        for(int i = lstMain->Items->Count ;i>=0 ;i--)
        {
            li = lstMain->Items->Item[i];
            if(li->Selected  == true)
            {
                int sid = li->Caption.ToInt();               

                ret = FmIC->gdb.VQuery(512,"select substate,mergeid,envid  from multis where sid=%d",sid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }
                MyRes res  = FmIC->gdb.Store();
                MyRow row  = res.Fetch();

                if(row==NULL) return ;

                int substate = atoi(row[0]) ;

                int envid = atoi(row[2]) ;
                if(substate !=0)
                {
                    MessageBox(Handle,"网点已提交,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                long aissid = atoi(row[1]);

                ret = FmIC->gdb.VQuery(512,"delete from multis where envid=%d and envid>0",envid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }


                WriteToLogFile(g_sys_opt.c_str(),"删除信封",envid,"");
                
                if(aissid!=0)
                {
                    ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }

                    ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }
                }
                li->Delete();
            }
        }
        if(curid > lstMain->Items->Count)
            curid = 0;

        if( lstMain->Items->Count>curid ) lstMain->Items->Item[curid]->Selected = true;
    }

    else if( pageCtrl->TabIndex == 3 )
    {
        li= lstSingle->Selected;
        if(li==NULL) return ;

        if(MessageBox(Handle,"是否要删除选中记录?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

        int curid = lstSingle->ItemIndex;  

        for(int i = lstSingle->Items->Count -1 ;i>=0;i--)
        {
            li = lstSingle->Items->Item[i];
            if(li->Selected  == true)
            {
                int sid = li->Caption.ToInt();              

                ret = FmIC->gdb.VQuery(512,"select substate,mergeid,envid  from multis where sid=%d",sid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }
                MyRes res  = FmIC->gdb.Store();
                MyRow row  = res.Fetch();

                if(row==NULL) return ;

                int substate = atoi(row[0]) ;

                int envid = atoi(row[2]) ;
                if(substate !=0)
                {
                    MessageBox(Handle,"网点已提交,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                long aissid = atoi(row[1]);

                ret = FmIC->gdb.VQuery(512,"delete from multis where envid=%d and envid>0",envid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                WriteToLogFile(g_sys_opt.c_str(),"删除信封",envid,"");
                if(aissid!=0)
                {
                    ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0 ,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }
                    ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0 ,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }
                }

                li->Delete();
            }
        }

        if(curid > lstSingle->Items->Count)
            curid = 0;

        if( lstSingle->Items->Count>curid ) lstSingle->Items->Item[curid]->Selected = true;
    }

    else if( pageCtrl->TabIndex == 4 )
    {
        li= lstMulti->Selected;
        if(li==NULL) return ;

        if(MessageBox(Handle,"是否要删除选中记录?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

        int curid = lstMulti->ItemIndex; 

        for(int i =0;i<lstMulti->Items->Count ;i++)
        {
            li = lstMulti->Items->Item[i];
            if(li->Selected  == true)
            {
                int sid = li->Caption.ToInt();              

                ret = FmIC->gdb.VQuery(512,"select substate,mergeid,envid  from multis where sid=%d",sid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }
                MyRes res  = FmIC->gdb.Store();
                MyRow row  = res.Fetch();

                if(row==NULL) return ;

                int substate = atoi(row[0]) ;

                int envid = atoi(row[2]) ;
                if(substate !=0)
                {
                    MessageBox(Handle,"网点已提交,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                long aissid = atoi(row[1]);

                WriteToLogFile(g_sys_opt.c_str(),"删除信封",envid,"");
                ret = FmIC->gdb.VQuery(512,"delete from multis where envid=%d and envid>0",envid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                if(aissid!=0)
                {
                    ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }

                    ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }
                }
                li->Delete();
            }
        }
        if(curid > lstSingle->Items->Count)
            curid = 0;

        if( lstSingle->Items->Count>curid ) lstSingle->Items->Item[curid]->Selected = true;
    }
    else
        return;
    MessageBox(Handle,"删除成功","提示",MB_OK|MB_ICONINFORMATION);
    
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::cmbIdentifyCloseUp(TObject *Sender)
{
    R1Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::cmbMergeCloseUp(TObject *Sender)
{
    R1Click(NULL);    
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::btnRefreshClick(TObject *Sender)
{
    R1Click(0);
}
//---------------------------------------------------------------------------



void __fastcall TFmEnv::pageCtrlChange(TObject *Sender)
{
    try
    {
        if( pageCtrl->TabIndex == 0 )
        {
            Label7->Caption   = "匹配状态";
            cmbMerge->Items->Clear();
            cmbMerge->Items->Add("全    部");
            cmbMerge->Items->Add("已匹配");
            cmbMerge->Items->Add("未匹配");
        }
        else if( pageCtrl->TabIndex == 1 )
        {
            Label7->Caption   = "匹配状态";
            cmbMerge->Items->Clear();
            cmbMerge->Items->Add("全    部");
            cmbMerge->Items->Add("已匹配");
            cmbMerge->Items->Add("未匹配");
            pageCtrl->ActivePageIndex = 1;
        }
        else if( pageCtrl->TabIndex == 2 )
        {
            Label7->Caption   = "匹配状态";
            cmbMerge->Items->Clear();
            cmbMerge->Items->Add("全    部");
            cmbMerge->Items->Add("已匹配");
            cmbMerge->Items->Add("未匹配");
            pageCtrl->ActivePageIndex = 2;
        }
        else if( pageCtrl->TabIndex == 3 )
        {
            Label7->Caption   = "核打状态";
            cmbMerge->Items->Clear();
            cmbMerge->Items->Add("全    部");
            cmbMerge->Items->Add("已核打");
            cmbMerge->Items->Add("未核打");
            pageCtrl->ActivePageIndex = 3;
        }
        else if( pageCtrl->TabIndex == 4 )
        {
            Label7->Caption   = "核打状态";
            cmbMerge->Items->Clear();
            cmbMerge->Items->Add("全    部");
            cmbMerge->Items->Add("已核打");
            cmbMerge->Items->Add("未核打");
        }
        else if( pageCtrl->TabIndex == 5 )
        {
            Label7->Caption   = "匹配状态";
            cmbMerge->Items->Clear();
            cmbMerge->Items->Add("全    部");
            cmbMerge->Items->Add("已匹配");
            cmbMerge->Items->Add("未匹配");
        }
    }
    catch(...)
    {
        Screen->Cursor = crDefault;
    }
     cmbMerge->ItemIndex  =  0;
    
    R1Click(NULL);
    Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::lstEnvsColumnClick(TObject *Sender,
      TListColumn *Column)
{
    int i,m,n,j;
    static bool od=true;

    Screen->Cursor = crHourGlass;
    lstEnvs->Items->BeginUpdate();

    od=!od;
    n = Column->Index;
    m = lstEnvs->Items->Count;
    lstEnvs->Items->Add();      
    if(n==3 || n==4 || n==6)
    {
        for(i=0;i< m-1;i++)
        for(j=i+1;j< m;j++)

        if(od)
        {
            if(lstEnvs->Items->Item[i]->SubItems->Strings[n-1].ToDouble()  >
                   lstEnvs->Items->Item[j]->SubItems->Strings[n-1].ToDouble())
                {
                    setitem(m,i,j);
                }
        }
        else
        {
            if(lstEnvs->Items->Item[i]->SubItems->Strings[n-1].ToDouble()  <
                   lstEnvs->Items->Item[j]->SubItems->Strings[n-1].ToDouble())
            {
               setitem(m,i,j);
            }
        }
    }

    Screen->Cursor = crDefault;
    lstEnvs->Items->Delete(m);
    lstEnvs->Items->EndUpdate();
    return;    
}
//---------------------------------------------------------------------------


int TFmEnv::setitem(int m, int i, int j)
{
    //TODO: Add your source code here
    lstEnvs->Items->Item[m]=
    lstEnvs->Items->Item[i];

    lstEnvs->Items->Item[i]=
    lstEnvs->Items->Item[j];

    lstEnvs->Items->Item[j]=
    lstEnvs->Items->Item[m];

    return 0;
}

void __fastcall TFmEnv::B1Click(TObject *Sender)
{
    TListItem* li;
    int ret = 0;


    li= lstMerge->Selected;
    if(li==NULL) return ;

   	
   	if(!FmIC->m_bNetDelete)
   	{
   		MessageBox(Handle,"该柜员没有网点删除权限!\t","提示",MB_OK|MB_ICONINFORMATION);
   		return;
   	}

    int sid = li->Caption.ToInt();

    ret = FmIC->gdb.VQuery(512,"select exchno  from multis where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return ;

    String netno =  row[0];

    String sw = AnsiString("是否要删除网点[")+netno +"]扫描信息?\t";
    if(MessageBox(Handle,sw.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;
    ret = FmIC->gdb.VQuery(512,"select max(substate)  from multis where exchno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();
    int substate = atoi(row[0]) ;
    if(substate !=0)
    {
        MessageBox(Handle,"网点已提交,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
		
/*
    MessageBox(Handle,"网点删除需要授权!\t","提示",MB_OK|MB_ICONINFORMATION);

    if( FormWarrant->ShowModal() == mrCancel)
    {
        MessageBox(Handle,"授权失败,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
*/
    ret = FmIC->gdb.VQuery(512,"delete from multis where exchno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }


     ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0,mergeid=-1 where mergestate=2 and exchno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
     if(ret)
     {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
     }

     ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=-1 where mergestate=2 and  exchno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
     if(ret)
     {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
     }
    MessageBox(Handle,"删除成功!\t","提示",MB_OK|MB_ICONINFORMATION);
    R1Click(0);
}
//---------------------------------------------------------------------------



void __fastcall TFmEnv::btnReSubmitClick(TObject *Sender)
{
    if(cmbNetno->ItemIndex == 0 ) return ;
    if (mergeEnvs(1)) return ;

    int ret = FmIC->gdb.VQuery(512,"select count(*) from multis where multiflag =1 and checkstate=0 and exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    int ct = atoi(row[0]);
    if(ct > 0 )
    {
        MessageBox(Handle,"多主件业务没有核打完毕，不能提交!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    ret = FmIC->gdb.VQuery(512,"select pkgno ,merge from bocnets where exchno='%s' and area='%s' ",cmbNetno->Text,g_sys_area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    int codeline;
    res  = FmIC->gdb.Store();
    row  = res.Fetch();

    if(row == NULL)
        codeline = 0;
    else
    {
        codeline = atoi(row[1]);
    }


    ret = FmIC->gdb.VQuery(512,"select count(*) from multis where mergestate=0 and (model=2 or model=6 or model=7) and exchno='%s' and area='%s' and date='%s' and session='%'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    
    res  = FmIC->gdb.Store();
    row  = res.Fetch();

    ct = atoi(row[0]);
    /*if(ct > 0 )
    {
        if(FmIC->gmode !=2)
        {
            String sw = AnsiString("共[")+ct +"]个信封没有匹配完,是否提交?\t";
            if(MessageBox(Handle,sw.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION) == IDNO)
            return ;
        }
        else
        {
            String sw = AnsiString("共[")+ct +"]个信封没有匹配完,不能提交!\t";
            MessageBox(Handle,sw.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
            return ;
        }
    }

    ret = FmIC->gdb.VQuery(512,"select count(*) from pbcdata where mergestate=0  and exchno='%s' and area='%s'",cmbNetno->Text,g_sys_area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    res  = FmIC->gdb.Store();
    row  = res.Fetch();

    ct = atoi(row[0]);
    if(ct > 0 )
    {
        String bShow;
        if(FmIC->gmode !=2)
        {
            bShow  =  "人行数据没有匹配完,是否提交?\t";
            if(MessageBox(Handle,bShow.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION) == IDNO)
            {
                pageCtrl->ActivePageIndex  = 5 ;
                cmbMerge->ItemIndex = 2;
                flushData6();
                return ;
            }
        }
        else
        {
            bShow  =  "清分数据没有匹配完,不能提交!\t";
            MessageBox(Handle,bShow.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
            pageCtrl->ActivePageIndex  = 5 ;
            cmbMerge->ItemIndex = 2;
            flushData6();
            return ;
        }
    }
    */
    if(ct > 0 )
    {
        String sw = AnsiString("共[")+ct +"]个信封没有匹配完,是否提交?\t";
        if(MessageBox(Handle,sw.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION) == IDNO)
        {
            pageCtrl->ActivePageIndex  = 2 ;
            cmbMerge->ItemIndex = 2;
            flushData2();
            return ;
        }
    }


    if(FmIC->gmode !=2 )
        ret = FmIC->gdb.VQuery(512,"select count(*) from pbcdata where mergestate=0  and exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    else
        ret = FmIC->gdb.VQuery(512,"select count(*) from dists where isenv=1 and mergestate=0  and exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());

    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    res  = FmIC->gdb.Store();
    row  = res.Fetch();

    int dt = atoi(row[0]);
    if(dt > 0 && codeline == 1 )
    {
        String bShow;
        if(FmIC->gmode !=2)
        {
            bShow  =  "人行数据没有匹配完,是否强制提交?\t";

        }
        else
        {
            bShow  =  "清分数据没有匹配完,是否强制提交?\t";
        }

        if(MessageBox(Handle,bShow.c_str(),"提示",MB_YESNO|MB_ICONINFORMATION) == IDNO)
        {
            pageCtrl->ActivePageIndex  = 5 ;
            cmbMerge->ItemIndex = 2;
            flushData6();
            return ;
        }
    }

    if( (ct > 0 || (dt>0 && codeline ==1)) && FmIC->gmode ==2 )
    {
    	
    		if(!FmIC->m_bSend)
    		{
    			MessageBox(Handle,"该柜员没有强制提交!\t","提示",MB_OK|MB_ICONINFORMATION);
    			return;
    		}
    	/*
        MessageBox(Handle,"强制提交需要授权!\t","提示",MB_OK|MB_ICONINFORMATION);

        if( FormWarrant->ShowModal() == mrCancel)
        {
            MessageBox(Handle,"授权失败,不能强制提交!\t","提示",MB_OK|MB_ICONINFORMATION);
            return;
        }
       */
    }
    
    locked = false;
    FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,4),'1',right(state,11)) where exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    FmIC->gdb.VQuery(512,"update multis set substate=1 where exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    cmbNetnoDropDown(NULL);

    MessageBox(Handle,"提交成功!\t","提示",MB_OK|MB_ICONINFORMATION);

    btnMerge->Enabled = false;
    btnSubmit->Enabled = false;    
}
//---------------------------------------------------------------------------


int TFmEnv::mergeEnvs(int type)
{
    //TODO: Add your source code here
     //检查所有票据是否OCR完成
    //检查清分所有票据是否OCR完成
    //检查是否有连续两个信封（代收代付）
    //检查envs 信封+代收代付总笔数，是否和清分数据一致，如一致进行自动勾兑 ,mergeid,mergestate
    //以信封、代收代付凭证为分割，自动对信封、代收代付凭证后第一张票据赋mainflag,envid,vchtype
    //自动对下一信封、代收代付凭证前的凭证赋 envid,vchtype
    //对于多笔凭证类业务，核打时输入金额和是否主件选择项，更新mainflag
    //判别是否识别完

    if(cmbNetno->ItemIndex == 0 ) return  -1;

    FmIC->gdb.VQuery(512,"update multis set mainflag=-1,envid=sid where  ( model=2 or model=6 or model=7 or model=200) and  exchno='%s' and substate!=1 and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
 
    int ret = FmIC->gdb.VQuery(512,"select count(*) from multis where state!=1 and exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return  -1 ;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    if(!res.Count())
        MessageBox(Handle,"匹配失败!1001\t","提示",MB_OK|MB_ICONINFORMATION);

    if(atoi(row[0])>0)
    {
        MessageBox(Handle,"扫描数据没有识别完或者识别有误,不能进行匹配!\t","提示",MB_OK|MB_ICONINFORMATION);
        pageCtrl->ActivePageIndex = 0 ;
        cmbIdentify->ItemIndex = 3;
        pageCtrlChange(NULL);
        flushData1();
        return -1;
    }

    Screen->Cursor = crHourGlass;
    //检查第一个图像必须是信封
    ret = FmIC->gdb.VQuery(512,"select sid,model from multis where  substate!=1 and exchno='%s' and area='%s' and date='%s' and session='%s' order by sid limit 1",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        Screen->Cursor = crDefault;
        return  -1;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();
    if(row == NULL)
    {
        Screen->Cursor = crDefault;
        return  0;
    }
    int ktc = atoi(row[1]);
    if(ktc!=2 && ktc!=6 && ktc!=7 && ktc!=200)
    {
        MessageBox(Handle,"未匹配记录第一个图像不是信封,可能扫描错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        pageCtrl->ActivePageIndex = 0 ;
        cmbIdentify->ItemIndex = 1;
        flushData1();
        Screen->Cursor = crDefault;
        return  -1;
    }

    //检查是否有两个连续分隔页
    ret = FmIC->gdb.VQuery(512,"select sid,model from multis where exchno='%s' and substate!=1  and area='%s' and date='%s' and session='%s' order by sid",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        Screen->Cursor = crDefault;
        return ret  ;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();
    bool last=false ;
    while(row!=NULL)
    {
        int vf = atoi(row[1]);
        if(vf == 2 || vf ==6 || vf ==7 || vf == 200)
        {
            if(last == true )
            {
                MessageBox(Handle,"匹配错误，存在连续两个信封!\t","提示",MB_OK|MB_ICONINFORMATION);
                pageCtrl->ActivePageIndex = 0 ;
                cmbIdentify->ItemIndex = 1;
                flushData1();
                Screen->Cursor = crDefault;
                return -1;
            }
            last = true;
        }
        else    last = false;
        row = res.Fetch();
    }
    ret = FmIC->gdb.VQuery(512,"select sid,model,vchtype,mainflag,amount,multiflag from multis where exchno='%s' and substate!=1 and area='%s' and date='%s' and session='%s' order by sid",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        Screen->Cursor = crDefault;
        return  -1;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();

    long sid;
    long envid;
    long mainid;
    long aissid;
    String vchtypes;
    int vchflag;
    int mainflag;
    int multiflag;
    String amt;
    while(row!=NULL)
    {
        sid = atoi(row[0]);
        vchflag = atoi(row[1]);
        mainflag = atoi(row[3]);
        if(mainflag == 1 ) mainid = sid;
        amt = row[4];
        multiflag = atoi(row[5]);
        if(vchflag == 2 || vchflag ==6 || vchflag == 7 || vchflag == 200)                                      //如果是分隔页
        {
            //信封的信封号为本身
            FmIC->gdb.VQuery(512,"update multis set envid=%d where sid=%d",sid,sid);
            envid = sid;
            vchtypes = row[2];

            //取下一条记录（主件）
            row = res.Fetch();           
            if(row !=NULL)
            {
                //主件号
                mainid = atoi(row[0]);
                if(multiflag == 0 )
                {
                    FmIC->gdb.VQuery(512,"update multis set mainflag=1,amount='%s',vchtype='%s',envid=%d ,mainid=%d where sid=%d",amt.c_str(),vchtypes.c_str(),envid,mainid,mainid);        //更新记录为主件，保存金额，类型，信封号
                    FmIC->gdb.VQuery(512,"update multis set mainid=%d  where sid=%d",mainid,envid);                                                                                         //更新该信封下面所有记录的主件ID
                } 
                FmIC->gdb.VQuery(512,"update multis set envid=%d,mainid=%d where sid=%d",envid,mainid,mainid);        //单信封号 
            }
            row = res.Fetch();
            continue;
        }

        //其他附件
        
        if(multiflag == 0 )
            ret = FmIC->gdb.VQuery(512,"update multis set vchtype='%s',envid=%d ,mainid=%d,mainflag=0 where sid=%d",vchtypes.c_str(),envid,mainid,sid);  //更新后续凭证类型、信封号、
        else
            ret = FmIC->gdb.VQuery(512,"update multis set vchtype='%s',envid=%d ,mainid=%d where sid=%d",vchtypes.c_str(),envid,mainid,sid);  //更新后续凭证类型、信封号、
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            Screen->Cursor = crDefault;
            return  -1;
        }
        row = res.Fetch();
    }

    //自动匹配信封与清分机信封
    //更新清分数据重新进行匹配


    if(type == 0 )
    {
        if(FmIC->gmode !=2 )    {
            ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0,mergeid=0 where  envelop>0 and  exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
       }
        else   {
            ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=0 where mergestate=2 and exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
            }
        if( FmIC->gmode != 0 )
            ret = FmIC->gdb.VQuery(512,"update multis set mergestate=0,mergeid=0 where  (multis.model=200 or multis.model=2 or multis.model=6 or multis.model=7) and ( mainid>0 or multiflag=1) and  exchno='%s' and area='%s' and date='%s' and session='%s'", cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    }
    ret = FmIC->gdb.VQuery(512,"select sid,model,vchtype,amount from multis where  (multis.model=200 or multis.model=2 or multis.model=6 or multis.model=7) and ( mainid>0 or multiflag=1) and  exchno='%s' and area='%s' and date='%s' and session='%s'", cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
       MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
       debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
       Screen->Cursor = crDefault;
       return  -1;
    }
    res = FmIC->gdb.Store();
    row = res.Fetch();

    while(row!=NULL)
    {
        sid = atoi(row[0]);
        vchflag = atoi(row[1]);
        vchtypes = row[2];
        amt = row[3];

        if(FmIC->gmode !=2)    {
            ret = FmIC->gdb.VQuery(1024,"select sid from pbcdata where mergestate=0 and vchtype='%s' and (envelop>0)  and amount='%s' and exchno='%s'  and area='%s' and date='%s' and session='%s' limit 1",vchtypes.c_str(),amt.c_str(),cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    //        ret = FmIC->gdb.VQuery(1024,"select sid from pbcdata where mergestate=0 and vchtype='%s' and (envelop>0)  and exchno='%s'  and area='%s' and date='%s' and session='%s' limit 1",vchtypes.c_str(),cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
          }
        else  {
            ret = FmIC->gdb.VQuery(1024,"select sid from dists where mergestate=0 and vchtype='%s' and (isenv>0)  and amount='%s' and exchno='%s'  and date='%s' and session='%s' limit 1",vchtypes.c_str(),amt.c_str(),cmbNetno->Text.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
              }
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error[%s]\n",mysql_error(FmIC->gdb.Handle()));
            Screen->Cursor = crDefault;
            return  -1;
        }

        MyRes res2 = FmIC->gdb.Store();
        MyRow row2 = res2.Fetch();
        int  mergecd = res2.Count();
        if( mergecd >1)
        {
            String b = AnsiString("有相同种类[")+vchtypes+"]金额["+amt+"]票据,请注意匹配!\t";
            MessageBox(Handle,b.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        }

        if(row2!=NULL)
        {
            aissid = atoi(row2[0]);

            if(FmIC->gmode !=2)
            {
                FmIC->gdb.VQuery(512,"update pbcdata set mergestate=1,mergeid=%d where sid=%d",sid,aissid );
            }
            else
                 FmIC->gdb.VQuery(512,"update dists set mergestate=2 ,mergeid=%d where sid=%d",sid,aissid );
            FmIC->gdb.VQuery(512,"update multis set mergestate=1 ,mergeid=%d where sid=%d",aissid,sid);
        }             

        row = res.Fetch();
    }

    //更新信封内票据匹配状态及匹配流水
    ret = FmIC->gdb.VQuery(512,"select sid,mergestate,mergeid from multis where  (multis.model=200 or multis.model=2 or multis.model= 6 or multis.model=7) and substate!=1 and  exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        Screen->Cursor = crDefault;
        return  -1;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();
    while(row!=NULL)
    {
        long id = atoi(row[0]);
        int  st = atoi(row[1]);
        long aissid = atoi(row[2]);
        if(st == 1)
            FmIC->gdb.VQuery(512,"update multis set mergestate = 1 ,mergeid =%d where envid=%d",aissid,id);
        else
            FmIC->gdb.VQuery(512,"update multis set mergestate = 0 ,mergeid =%d where envid=%d",aissid,id);
        row = res.Fetch();
    }    

     //更新南京多主件信封
    ret = FmIC->gdb.VQuery(512,"select distinct envid  from multis where multiflag = 1 and   exchno='%s' and area='%s' and date='%s' and session='%s' ",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        Screen->Cursor = crDefault;
        return  -1;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();
    while(row!=NULL)
    {
        long ed = atoi(row[0]);

        FmIC->gdb.VQuery(512,"update multis set multiflag = 1  where envid=%d or sid=%d",ed,ed);
        row = res.Fetch();
    }

    //自动校验
    ret = FmIC->gdb.VQuery(512,"select envid  ,concat(area,vchtype),model from multis where multiflag = 0 and mainflag=1 and   exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        Screen->Cursor = crDefault;
        return  -1 ;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();
    while(row!=NULL)
    {
        long sed = atoi(row[0]);

        if( _vtMap[row[1]] == row[2])
        {
            FmIC->gdb.VQuery(512,"update multis set checkstate=1 where envid=%d",sed);    
        }

        row = res.Fetch();
    }


    //针对南京分行人行扫描第一张不是主件问题做的特殊处理
    if( strcmp(g_sys_area.c_str(),"320001") == 0 )
    {
        ret = FmIC->gdb.VQuery(512,"select sid from multis where exchno='%s' and substate!=1 and model in(2,6,7,200) and area='%s' and date='%s' and session='%s' order by sid",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            Screen->Cursor = crDefault;
            return  -1 ;
        }

        res = FmIC->gdb.Store();
       
        while( (row = res.Fetch())!=NULL)
        {
                long envid = atoi(row[0]);
                ret =  FmIC->gdb.VQuery(512,"select sid from multis where mainflag=1 and envid=%d",envid);
                if(ret)
                {
                        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        Screen->Cursor = crDefault;
                        return  -1 ;
                }

                MyRes res22 = FmIC->gdb.Store();

                if( res22.Count() == 1 )
                {
                        MyRow row22 = res22.Fetch();
                        long mainid =    atoi(row22[0]);
                        ret =  FmIC->gdb.VQuery(512,"update multis set mainid=%d where envid=%d",mainid,envid);
                        if(ret)
                        {
                                MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
                                debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                                Screen->Cursor = crDefault;
                                return  -1 ;
                        }
                }
        }        
    }
       
    MessageBox(Handle,"匹配成功!\t","提示",MB_OK|MB_ICONINFORMATION);
    pageCtrl->ActivePageIndex = 0 ;
    cmbMerge->ItemIndex = 2;
    Screen->Cursor = crDefault;
    R1Click(NULL);
    return 0;
}
void __fastcall TFmEnv::btnMergeClick(TObject *Sender)
{
     mergeEnvs(0);
}
//---------------------------------------------------------------------------      

void __fastcall TFmEnv::E1Click(TObject *Sender)
{
    Close();    
}
//--------------------------------------------------------------------------- 

int TFmEnv::getNext()
{
    //TODO: Add your source code here
    TListItem* li;
    if( lstSingle->Items->Count>0)
    {
        li= lstSingle->Items->Item[0];
        li->Delete();
    }
    if(lstSingle->Items->Count==0)
    {
         MessageBox(Handle,"记录处理完毕\t","提示",MB_OK|MB_ICONINFORMATION);
        return -1;
    }

    li= lstSingle->Items->Item[0];
    li->Selected = true;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,substate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,envid ,state,modeflag from multis where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return -1;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return -1;

    char ip[24];

    char amount[16];
    char fname[80];
    char bname[80];
    char pkgno[12];
    int  submitstate;
    int  rotate;
    int  vchflag;
    int  vchtype;
    int  envid;
    int  state;
    int  modeflag;

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
    envid = atoi(row[11]);
    state  = atoi(row[12]);
    modeflag  = atoi(row[13]);

    if(submitstate!=0 || state == 0 )  FormEnvImage->btnSave->Visible = false;
    else                FormEnvImage->btnSave->Visible = true;    

    if( strlen(amount)<=0) strcpy(amount,"0");
    
    FormEnvImage->txtVchType->Text  = vchtype;
    FormEnvImage->txtAmount->Text = amount;
    FormEnvImage->txtAmount->SetFocus();

    FormEnvImage->txtType->Visible = false;
    FormEnvImage->Label1->Visible = false;

    FormEnvImage->chkMode->Checked = (modeflag == 1);

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
        return -1;
    }

    unsigned char * bmp;
    int size;

    bmp =ti_RGB2BMP(image.data(),sizex,sizey,0,0,0,0,sizex,sizey,size);

    TMemoryStream* pms = new TMemoryStream();
    pms->Clear();
    pms->SetSize(size);
    ret = pms->Write(bmp,size);
    pms->Position = 0;
    FormEnvImage->curID =  sid;
    FormEnvImage->type  =  4;
    FormEnvImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();
    ti_RGB2BMPFREE(bmp);

    return 0;

}


int TFmEnv::getNextScan()
{
    //TODO: Add your source code here
    TListItem* li;
    if( lstMerge->Items->Count>0)
    {
        li= lstMerge->Items->Item[0];
        li->Delete();
    }
    if(lstMerge->Items->Count==0)
    {
         MessageBox(Handle,"记录处理完毕\t","提示",MB_OK|MB_ICONINFORMATION);
        return -1;
    }

    li= lstMerge->Items->Item[0];
    li->Selected = true;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,substate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,envid ,state,modeflag from multis where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return -1;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return -1;

    char ip[24];

    char amount[16];
    char fname[80];
    char bname[80];
    char pkgno[12];
    int  submitstate;
    int  rotate;
    int  vchflag;
    int  vchtype;
    int  envid;
    int  state;
    int  modeflag;

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
    envid = atoi(row[11]);
    state  = atoi(row[12]);
    modeflag = atoi(row[13]);

    if(submitstate!=0 || state == 0 )  FormEnvImage->btnSave->Visible = false;
    else                FormEnvImage->btnSave->Visible = true;    

    if( strlen(amount)<=0) strcpy(amount,"0");
    
    FormEnvImage->txtVchType->Text  = vchtype;
    FormEnvImage->txtAmount->Text = amount;
    FormEnvImage->curID  = sid;
    
    FormEnvImage->txtAmount->SetFocus();

    FormEnvImage->txtType->Visible = true;
    FormEnvImage->Label1->Visible = true;

    if(vchflag == 2 )           FormEnvImage->txtType->ItemIndex =  0 ;
    else if(vchflag == 6)       FormEnvImage->txtType->ItemIndex =  1 ;
    else if(vchflag == 7)       FormEnvImage->txtType->ItemIndex =  2 ;
    else   FormEnvImage->txtType->ItemIndex =  3 ;     

    FormEnvImage->chkMode->Checked = (modeflag == 1);
    
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
        return -1;
    }

    unsigned char * bmp;
    int size;

    bmp =ti_RGB2BMP(image.data(),sizex,sizey,0,0,0,0,sizex,sizey,size);

    TMemoryStream* pms = new TMemoryStream();
    pms->Clear();
    pms->SetSize(size);
    ret = pms->Write(bmp,size);
    pms->Position = 0;
    FormEnvImage->curID =  sid;
    FormEnvImage->type  =  1;
    FormEnvImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();
    ti_RGB2BMPFREE(bmp);

    return 0;

}

void __fastcall TFmEnv::txtDetailDblClick(TObject *Sender)
{
    if(FmIC->gmode !=2 ) return ;
    
    TListItem* li;
    li= txtDetail->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select sid, accno,vchno,vchtype,amount ,mergestate,fname,ip ,0,date,session,isenv from dists where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"获取图像错误!\t","提示",MB_OK|MB_ICONINFORMATION);
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
       R1Click(NULL);
    }    
}
//---------------------------------------------------------------------------




bool TFmEnv::FindResult(String StrToFind, TListItem* pItem, bool bExact)
{
    //TODO: Add your source code here
    String sub_str = pItem->SubItems->Strings[3];

    // 金额搜索
    bool found = (sub_str ==StrToFind);

    if( found ) return true;

    // 金额搜索
    sub_str = pItem->SubItems->Strings[4];
    found = (sub_str.Pos(StrToFind)>0);
    if( found ) return true;

    // 帐号搜索
    sub_str = pItem->Caption;
    return (sub_str.Pos(StrToFind)>0);
}

//---------------------------------------------------------------------------


void __fastcall TFmEnv::FindDlgFind(TObject *Sender)
{
     String find_str = FindDlg->FindText;
    FindDlg->CloseDialog();
    if( find_str.IsEmpty() ) return;

    int sub_no, start_index;

    TListView  *pFound;

    if( pageCtrl->TabIndex == 0 )
    {
        pFound = lstMerge ;
    }
    else if( pageCtrl->TabIndex == 1 )
    {
        pFound = lstEnvs ;    
    }
    else if( pageCtrl->TabIndex == 2 )
    {
        pFound = lstMain ;    
    }
    else if( pageCtrl->TabIndex == 3 )
    {
        pFound = lstSingle ;    
    }
    else if( pageCtrl->TabIndex == 4 )
    {
        pFound = lstMulti ;    
    }
    else 
    {
        pFound = txtDetail ;    
    }

    start_index = pFound->Selected? pFound->Selected->Index:0;

    pFound->SetFocus();

    if( start_index>0) pFound->Selected->Selected = false;
    //从指定记录开始搜索
    int count = pFound->Items->Count  ;


    Screen->Cursor = crHourGlass	;
    for( int i=  count-1; i>=0; i-- )
    {
        TListItem* pItem = pFound->Items->Item[i];
        if( FindResult(find_str,pItem,false) )
        {
            pItem->MakeVisible(false);
            pItem->Selected = true;
            pItem->Focused = true;
        }
        else
        {
            pItem->Delete();
        }
    }

    Screen->Cursor = crDefault	;        
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::F1Click(TObject *Sender)
{
     FindDlg->Execute();    
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
    if( locked == true)
    {
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,6),'0',right(state,9)) where exchno='%s' and right(left(state,7),1)!='1' and date='%s' and session=%s and area='%s'",curNetno.c_str(),txtDate->Text.c_str(),lastSess.c_str(),g_sys_area.c_str());
        locked = false;
    }
    if(cmbNetState->ItemIndex == 0 )
        flushExchno(0);
    else if(cmbNetState->ItemIndex == 1 )
        flushExchno(1);
    else
        flushExchno(2);
        
    R1Click(NULL);
    if( cmbNetState->ItemIndex == 1)
    {
        btnMerge->Enabled = false;
        btnSubmit->Enabled = false;
        btnReSubmit->Enabled = false;
    }
    else
    {
        btnMerge->Enabled  = true;
        btnSubmit->Enabled = true;
        btnReSubmit->Enabled = false;
    }      
}
//---------------------------------------------------------------------------

void __fastcall TFmEnv::UpDown1Changing(TObject *Sender, bool &AllowChange)
{
    lastSess  = txtSession->Text.c_str();             
}
//---------------------------------------------------------------------------


int TFmEnv::flushdate()
{
    int ret = FmIC->gdb.Query("update multis set model=2 where model=200");
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
         return ret;
    }
        return 0;
}

//---------------------------------------------------------------------------
int TFmEnv::flush_sendstate()
{
    int ret = FmIC->gdb.Query("select sendstate,exchno from multis where mainflag=1");
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
         return ret;
    }
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    while(row)
    {
        ret = FmIC->gdb.VQuery(512,"update multis set sendstate=%d where mainflag!=1 and exchno='%s'",atoi(row[0]),row[1]);
        if(ret)
        {
                MessageBox(Handle,"查询出错!0001\t","提示",MB_OK|MB_ICONINFORMATION);
                debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
             return ret;
        }
        row = res.Fetch();
    }
}

