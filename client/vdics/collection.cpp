//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <zcommon.h>
#include <transimg.h>
#include <libmem.h>
#include <bcbext.h>

#include "collection.h"
#include "replacescan.h"
#include "main.h"    

#include "wsimages.h"
#include "images.h"

#include "image.h"
#include "wscheck.h"
#include "login.h"
#include "distimage.h"
#include "warrant.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormWS *FormWS;
//---------------------------------------------------------------------------
__fastcall TFormWS::TFormWS(TComponent* Owner)
    : TForm(Owner)
{
    locked = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormWS::FormShow(TObject *Sender)
{
    memset(date,0,sizeof(date));
    memset(session,0,sizeof(session));
    FmIC->getDS("date",date);
    FmIC->getDS("session",session);
    txtDate->Text = date;
    txtSession->Text = session;

    pageCtrl->ActivePageIndex = 0;
    if(boolExchno == true)
        flushExchno(0);
    flushData1();


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
        row = res.Fetch();
    }
}
//---------------------------------------------------------------------------


int TFormWS::flushData1()
{
    //TODO: Add your source code here
    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";

    if(cmbNetno->ItemIndex == 0  || cmbNetno->Text=="全    部")
        cond = cond + " and 1= 1";
    else
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";

    cond = cond + " and area='"+g_sys_area.c_str()+"'";

    if(cmbIdentify->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbIdentify->ItemIndex == 1 )
        cond = cond + " and state=1  ";
    else if(cmbIdentify->ItemIndex == 2 )
        cond = cond + " and ( state =0 or state = 2 )  ";
    else
        cond = cond + " and ( state =3 or state = 5 or state = 6) ";

    if(cbMerge->ItemIndex == 0 )
        cond = cond + " ";
    else if(cbMerge->ItemIndex == 1 )
        cond = cond + " and mergestate=1 ";
    else
        cond = cond + " and mergestate=0 and ( (mainflag=1 and multiflag<3)  or model in (2,6,7) )";

    if(cbCheck->ItemIndex == 0 )
        cond = cond + " order by sid ";
    else if(cbCheck->ItemIndex == 1 )
        cond = cond + " and checkstate=1 order by sid ";
    else if(cbCheck->ItemIndex == 2 )
        cond = cond + " and checkstate=0 order by sid";
    else
        cond = cond + " and checkstate=2 order by sid";
                                                   
    String str = "select sid,exchno,fname,model,vchtype, amount,state,mergestate ,mainflag,mainid,checkstate,mergeid from authinput  where 1=1 ";
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
       li->SubItems->Insert(0,row[1]);
       li->SubItems->Insert(1,row[2]);
       if(atoi(row[3]) == 2 )
            li->SubItems->Insert(2,"信封票据");
       else if(atoi(row[3]) == 6)
            li->SubItems->Insert(2,"信封票据");
       else if(atoi(row[3]) == 7)
            li->SubItems->Insert(2,"信封票据");
       else
       {
            if(atoi(row[8]) == 1 )
                li->SubItems->Insert(2,"第一联");
            else if( atoi(row[8]) == 2 )
                li->SubItems->Insert(2,"第二联");
            else
                li->SubItems->Insert(2,"");
       }
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
       else  if(atoi(row[6]) == 6 )
            li->SubItems->Insert(5,"联次连续");
       else
            li->SubItems->Insert(5,"识别中");

       if(atoi(row[7]) == 1 )
            li->SubItems->Insert(6,"已匹配");
       else
            li->SubItems->Insert(6,"未匹配");

      if(atoi(row[10]) == 1 )
       {
            li->SubItems->Insert(7,"已核打");
       }
       else
            li->SubItems->Insert(7,"未核打");

       li->SubItems->Insert(8,row[11]);
       row = res.Fetch();
   }

   lstMerge->Items->EndUpdate();

    str = "select count(*),sum(amount) from authinput where  ( model=2 or model=6 or model=7 )  ";

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

   statusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";
   statusBar->Panels->Items[1]->Text = String("信封总笔数[")+row[0]+"]";
   statusBar->Panels->Items[2]->Text = String("信封总金额[")+row[1]+"]";
   
   return 0;
}


int TFormWS::flushData2()
{
    //TODO: Add your source code here
    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";

    if(cmbNetno->ItemIndex == 0 )
        cond = cond + " and 1= 1";
    else
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";

    cond = cond + " and area='"+g_sys_area.c_str()+"'";

    if(cmbIdentify->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbIdentify->ItemIndex == 1 )
        cond = cond + " and state=1  ";
    else if(cmbIdentify->ItemIndex == 2 )
        cond = cond + " and ( state =0 or state = 2)  ";
    else
        cond = cond + " and state=3  ";

    if(cbMerge->ItemIndex == 0 )
        cond = cond + " ";
    else if(cbMerge->ItemIndex == 1 )
        cond = cond + " and mergestate=1 ";
    else
        cond = cond + " and mergestate=0 and ( (mainflag=1 and multiflag<3)  or model in (2,6,7) )";

    if(cbCheck->ItemIndex == 0 )
        cond = cond + " order by sid ";
    else if(cbCheck->ItemIndex == 1 )
        cond = cond + " and checkstate=1 order by sid ";
    else if(cbCheck->ItemIndex == 2 )
        cond = cond + " and checkstate=0 order by sid";
    else
        cond = cond + " and checkstate=2 order by sid";



    String str = "select sid,exchno,fname,vchtype, amount,mergestate,mergeid,checkstate from authinput where ( model=2 or model=6 or model=7 )  and mergestate=1 ";
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
       li->SubItems->Insert(0,row[1]);
       li->SubItems->Insert(1,row[2]);
       li->SubItems->Insert(2,row[3]);
       li->SubItems->Insert(3,row[4]);
       
       if(atoi(row[5]) == 1 )
            li->SubItems->Insert(4,"已匹配");
       else
            li->SubItems->Insert(4,"未匹配");

       if(atoi(row[7]) == 1 )
            li->SubItems->Insert(5,"已核打");
       else
            li->SubItems->Insert(5,"未核打");
       li->SubItems->Insert(6,row[6]);
      
       row = res.Fetch();
   }

    lstEnvs->Items->EndUpdate();

    str = "select count(*),sum(amount) from authinput where  ( model=2 or model=6 or model=7 )  ";

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

   statusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";
   statusBar->Panels->Items[1]->Text = String("信封总笔数[")+row[0]+"]";
   statusBar->Panels->Items[2]->Text = String("信封总金额[")+row[1]+"]";
   
   return 0;
}


int TFormWS::flushData3()
{
    //TODO: Add your source code here
     //TODO: Add your source code here
    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";

    if(cmbNetno->ItemIndex == 0 )
        cond = cond + " and 1= 1";
    else
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";

    cond = cond + " and area='"+g_sys_area.c_str()+"'";
     
    if(cmbIdentify->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbIdentify->ItemIndex == 1 )
        cond = cond + " and state=1  ";
    else if(cmbIdentify->ItemIndex == 2 )
        cond = cond + " and ( state =0 or state = 2)  ";
    else
        cond = cond = cond + " and state=3  ";

    if(cbMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cbMerge->ItemIndex == 1 )
        cond = cond + " and mergestate=1 ";
    else
        cond = cond + " and mergestate=0 and ( (mainflag=1 and multiflag<3)  or model in (2,6,7) )";

    if(cbCheck->ItemIndex == 0 )
        cond = cond + " order by sid ";
    else if(cbCheck->ItemIndex == 1 )
        cond = cond + " and checkstate=1 order by sid ";
    else if(cbCheck->ItemIndex == 2 )
        cond = cond + " and checkstate=0 order by sid";
    else
        cond = cond + " and checkstate=2 order by sid";


    String str = "select sid,exchno,fname,vchtype, amount,mergestate,mergeid from authinput where mainflag=1 and multiflag=1 ";
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

   lstMain->Items->EndUpdate();

    str = "select count(*),sum(amount) from authinput where   mainflag=1 and multiflag=1 ";

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

   statusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";
   statusBar->Panels->Items[1]->Text = String("委收总笔数[")+row[0]+"]";
   statusBar->Panels->Items[2]->Text = String("委收总金额[")+row[1]+"]";
   
   return 0;
}

int TFormWS::flushData4()
{
    //TODO: Add your source code here
    //TODO: Add your source code here
    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";

    if(cmbNetno->ItemIndex == 0 )
        cond = cond + " and 1= 1 ";
    else
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";

    cond = cond + " and area='"+g_sys_area.c_str()+"'";
     
    if(cmbIdentify->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cmbIdentify->ItemIndex == 1 )
        cond = cond + " and state=1  ";
    else if(cmbIdentify->ItemIndex == 2 )
        cond = cond + " and ( state =0 or state = 2)  ";
    else
        cond = cond = cond + " and state=3  ";

    if(cbMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cbMerge->ItemIndex == 1 )
        cond = cond + " and mergestate=1 ";
    else
       cond = cond + " and mergestate=0 and ( (mainflag=1 and multiflag<3)  or model in (2,6,7) )";

    if(cbCheck->ItemIndex == 0 )
        cond = cond + " order by sid ";
    else if(cbCheck->ItemIndex == 1 )
        cond = cond + " and checkstate=1 order by sid ";
    else if(cbCheck->ItemIndex == 2 )
        cond = cond + " and checkstate=0 order by sid";
    else
        cond = cond + " and checkstate=2 order by sid";


    String str = "select sid,exchno,fname,vchtype, amount,mergestate,mergeid from authinput where mainflag=1 and multiflag=3 ";
    str = str +cond;

    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
         return ret;
    }

   lstFs->Items->BeginUpdate();
   lstFs->Clear();
   TListItem* li;
   MyRes res = FmIC->gdb.Store();
   MyRow row = res.Fetch();
   while(row!=NULL)
   {
       li = lstFs->Items->Add();
       li->Caption = row[0];
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

   lstFs->Items->EndUpdate();

   
   str = "select count(*),sum(amount) from authinput where  mainflag=1 and multiflag=3";

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

   statusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";
   statusBar->Panels->Items[1]->Text = String("非税总笔数[")+row[0]+"]";
   statusBar->Panels->Items[2]->Text = String("非税总金额[")+row[1]+"]";
   return 0;
}



int TFormWS::flushExchno(int state)
{
    //TODO: Add your source code here
    cmbNetno->Clear();
    int ret;
    if(state == 0 )
        ret = FmIC->gdb.VQuery(512,"select distinct authinput.exchno from authinput,bocctrl where right(left(bocctrl.state,8),1)='0' and authinput.exchno=bocctrl.exchno and bocctrl.area='%s' and bocctrl.date='%s' and bocctrl.session='%s' and  checkstate=0 order by authinput.exchno",g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    else  if(state == 1)
        ret = FmIC->gdb.VQuery(512,"select distinct authinput.exchno from bocctrl,authinput where right(left(bocctrl.state,8),1)!='0' and authinput.exchno=bocctrl.exchno and bocctrl.area='%s' and bocctrl.date='%s' and bocctrl.session='%s' and  checkstate=0 order by authinput.exchno",g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    else
        ret = FmIC->gdb.VQuery(512,"select distinct authinput.exchno from bocctrl,authinput where authinput.exchno=bocctrl.exchno and bocctrl.area='%s' and bocctrl.date='%s' and bocctrl.session='%s' and  checkstate=1 order by authinput.exchno",g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());

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
        if( state == 2)
        {
            ret =  FmIC->gdb.VQuery(512,"select count(*) from authinput where exchno='%s' and checkstate!=1 and area='%s' and date='%s' and session='%s'", row[0],g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
            if(ret)
            {
                MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                return 0;
            }
            MyRes res2 = FmIC->gdb.Store();
            MyRow row2 = res2.Fetch();
            if( atoi(row2[0])>0)
            {
                 row = res.Fetch();
                continue;
            }
        }
        cmbNetno->Items->Add(row[0]);
        row = res.Fetch();
    }

    cmbNetno->ItemIndex = 0 ;

}

void __fastcall TFormWS::B1Click(TObject *Sender)
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

    ret = FmIC->gdb.VQuery(512,"select pkgno  from authinput where sid=%d",sid);
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


    ret = FmIC->gdb.VQuery(512,"select count(*) from authinput where checkstate=1 and pkgno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    
    res  = FmIC->gdb.Store();
    row  = res.Fetch();

    if(row==NULL) return ;

    int ct = atoi(row[0]) ;
    if(ct !=0)
    {
        MessageBox(Handle,"已有记录提交,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
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
    ret = FmIC->gdb.VQuery(512,"delete from authinput where  pkgno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }


     ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0,mergeid=-1 where mergestate=3 and  exchno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
     if(ret)
     {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
     }

     ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=-1 where mergestate=3 and  exchno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
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



void __fastcall TFormWS::R1Click(TObject *Sender)
{     
    Screen->Cursor = crHourGlass;
    try
    {
        if( pageCtrl->TabIndex == 0 )
        {
            flushData1();
        }
        else if( pageCtrl->TabIndex == 1 )
        {
            flushData2();
        }
        else if( pageCtrl->TabIndex == 2 )
        {
            flushData3();
        }
         else if( pageCtrl->TabIndex == 3 )
        {
            flushData4();
        }
        else if( pageCtrl->TabIndex == 4 )
        {
            flushData5();
        }
        else
            MessageBox(Handle,"None","提示",MB_OK|MB_ICONINFORMATION);
    }
    catch(...)
    {
        Screen->Cursor = crDefault;
    }

    Screen->Cursor = crDefault;

}
//---------------------------------------------------------------------------

void __fastcall TFormWS::btnRefreshClick(TObject *Sender)
{
    R1Click(NULL);    
}
//---------------------------------------------------------------------------

void __fastcall TFormWS::lstMergeDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstMerge->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int curid = lstMerge->ItemIndex;

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,mergestate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,state,mainflag,substate,multiflag from authinput where sid=%d",sid);
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
    int  mergestate;
    int  rotate;
    int  vchflag;
    int  mainflag;
    String  vchtype;
    int  state;
    int  substate;

    memset(ip,0,sizeof(ip));

    memset(amount,0,sizeof(amount));
    memset(fname,0,sizeof(fname));
    memset(bname,0,sizeof(bname));
    memset(pkgno,0,sizeof(pkgno));
    strcpy(ip,row[0]);
    rotate = atoi(row[1]);
    mergestate = atoi(row[2]);

    strncpy(amount,row[3],sizeof(amount)-1);
    vchflag = atoi(row[4]);

    strcpy(fname,row[5]);
    strcpy(bname,row[6]);
    strcpy(pkgno,row[7]);
    vchtype = row[8];
    state = atoi(row[11]);

    mainflag = atoi(row[12]);

    substate = atoi(row[13]);

    FormWsImage->txtType->Visible = true;
    FormWsImage->Label1->Visible = true;

    //未识别不让保存
    if( state == 0 ) FormWsImage->btnSave->Visible = false;
    else                FormWsImage->btnSave->Visible = true;


    //已匹配 信封不让保存
    if(mergestate!=0)
    {
     if(vchflag==2 || vchflag==6 || vchflag==7)
     {
        FormWsImage->btnSave->Visible = false;
     }
     else
        FormWsImage->btnSave->Visible = true;
    }

    if(substate!=0)
        FormWsImage->btnSave->Visible = false;

    FormWsImage->txtAmount->Text = "";

    FormWsImage->state = state;
    FormWsImage->multiflag = atoi(row[14]);

    FormWsImage->txtVchtype->Text = vchtype;


    if(vchflag == 2 || vchflag == 6 || vchflag == 7 )     FormWsImage->txtType->ItemIndex =  0 ;
    else if(mainflag == 1 )       FormWsImage->txtType->ItemIndex =  1 ;
    else       FormWsImage->txtType->ItemIndex =  2 ;


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
    FormWsImage->type  =  1;
    FormWsImage->curID = sid;
    FormWsImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    ti_RGB2BMPFREE(bmp);
    ret = FormWsImage->ShowModal();

    if((cmbIdentify->ItemIndex == 3 || cbMerge->ItemIndex == 2)  )
        R1Click(NULL);
    else if(ret == mrOk)
    {
        li->SubItems->Strings[2] =  FormWsImage->txtType->Text.c_str();
        li->SubItems->Strings[3] =  FormWsImage->txtVchtype->Text.c_str();
        li->SubItems->Strings[4] =  RemoveThousandChar(FormWsImage->txtAmount->Text.c_str());
    }

    if(curid > lstMerge->Items->Count)
        curid = 0;

    if( lstMerge->Items->Count>curid ) lstMerge->Items->Item[curid]->Selected = true;
}

int TFormWS::getNext()
{
    TListItem* li;
    if(lstMerge->Items->Count>0)
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

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,mergestate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,state,mainflag,substate,multiflag from authinput where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return ret;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return -1;

    char ip[24];

    char amount[16];
    char fname[80];
    char bname[80];
    char pkgno[12];
    int  mergestate;
    int  substate;
    int  rotate;
    int  vchflag;
    int  mainflag;
    String  vchtype;
    int  state;

    memset(ip,0,sizeof(ip));

    memset(amount,0,sizeof(amount));
    memset(fname,0,sizeof(fname));
    memset(bname,0,sizeof(bname));
    memset(pkgno,0,sizeof(pkgno));
    strcpy(ip,row[0]);
    rotate = atoi(row[1]);
    mergestate = atoi(row[2]);

    strcpy(amount,row[3]);
    vchflag = atoi(row[4]);

    strcpy(fname,row[5]);
    strcpy(bname,row[6]);
    strcpy(pkgno,row[7]);
    vchtype = row[8];
    state = atoi(row[11]);

    mainflag = atoi(row[12]);

    substate = atoi(row[13]);

    FormWsImage->txtType->Visible = true;
    FormWsImage->Label1->Visible = true;

    FormWsImage->state = state;
    FormWsImage->multiflag = atoi(row[14]);

   //未识别不让保存
    if( state == 0 ) FormWsImage->btnSave->Visible = false;
    else                FormWsImage->btnSave->Visible = true;


    //已匹配 信封不让保存
    if(mergestate!=0)
    {
     if(vchflag==2 || vchflag==6 || vchflag==7)
     {
        FormWsImage->btnSave->Visible = false;
     }
     else
        FormWsImage->btnSave->Visible = true;
    }

    if(substate!=0)
       FormWsImage->btnSave->Visible = false;

    if( strlen(amount)<=0) strcpy(amount,"0");

    FormWsImage->txtAmount->Text = "";
    FormWsImage->txtAmount->SetFocus();

    FormWsImage->txtVchtype->Text = vchtype;

    if(vchflag == 2 || vchflag == 6 || vchflag == 7 )     FormWsImage->txtType->ItemIndex =  0 ;
    else if(mainflag == 1 )       FormWsImage->txtType->ItemIndex =  1 ;
    else       FormWsImage->txtType->ItemIndex =  2 ;

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
        return ret;
    }

    unsigned char * bmp;
    int size;

    bmp =ti_RGB2BMP(image.data(),sizex,sizey,0,0,0,0,sizex,sizey,size);

    TMemoryStream* pms = new TMemoryStream();
    pms->Clear();
    pms->SetSize(size);
    ret = pms->Write(bmp,size);
    pms->Position = 0;
    FormWsImage->type  =  1;
    FormWsImage->curID = sid;
    FormWsImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    ti_RGB2BMPFREE(bmp);

    return 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormWS::lstEnvsDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstEnvs->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,checkstate,amount,mainflag,fname,bname,right(pkgno,5),vchtype,date,session  from authinput where sid=%d",sid);
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
    int  checkstate;
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
    checkstate = atoi(row[2]);

    strcpy(amount,row[3]);
    mainflag = atoi(row[4]);

    strcpy(fname,row[5]);
    strcpy(bname,row[6]);


    if(checkstate==2)
    {
         if(MessageBox(Handle,"业务核打中,是否继续?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;
    }

    if(checkstate==1)
    {
        FormWsCheck->btnSave->Visible = false;
        FormWsCheck->btnCheck->Visible = false;
    }
    else
    {
        FormWsCheck->btnSave->Visible = true;
        FormWsCheck->btnCheck->Visible = true;
        ret = FmIC->gdb.VQuery(512," update authinput set checkstate=2 where envid=%d or sid =%d",sid,sid);
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }
    }

    FormWsCheck->txtAmount->Text  = amount;
    FormWsCheck->txtAmount->Visible = true;
    FormWsCheck->chkMain->Visible = false;
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
        if(checkstate!=1)
        {
            ret = FmIC->gdb.VQuery(512,"update authinput set checkstate=0 where sid=%d or envid=%d",sid,sid);
            if(ret)
            {
                MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                return;
            }
        }
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
    FormWsCheck->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();
   
  
    FormWsCheck->setMethod(1,sid);
    ret = FormWsCheck->ShowModal();
    if(ret == mrOk )
    {
        flushData2();
        if(lstEnvs->Items->Count>0 && cbCheck->ItemIndex == 2)
        {
            
            TListItem* nli;
            nli = lstEnvs->Items->Item[0];
            nli->Selected = true;
            lstEnvsDblClick(NULL);
        }         
    }
    else
    {
        if(checkstate!=1)
        {
            ret = FmIC->gdb.VQuery(512,"update authinput set checkstate=0 where sid=%d or envid=%d",sid,sid);
            if(ret)
            {
                MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                R1Click(NULL);   
                return;
            }
        }
    }  
    R1Click(NULL);
}
//---------------------------------------------------------------------------


void __fastcall TFormWS::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------



void __fastcall TFormWS::cmbNetnoDropDown(TObject *Sender)
{
    lastNetno =  curNetno;
    flushExchno(cmbNetState->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormWS::cmbIdentifyCloseUp(TObject *Sender)
{
     R1Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormWS::cbMergeCloseUp(TObject *Sender)
{
    R1Click(NULL);    
}
//---------------------------------------------------------------------------

void __fastcall TFormWS::cbCheckCloseUp(TObject *Sender)
{
     R1Click(NULL);    
}
//---------------------------------------------------------------------------

void __fastcall TFormWS::MeForceLookClick(TObject *Sender)
{
    TListItem* li;
    int ret = 0;    

    if( pageCtrl->TabIndex == 0 )
    {
        li= lstMerge->Selected;
        if(li==NULL) return ;
        int curid = lstMerge->ItemIndex;

        if(MessageBox(Handle,"是否要删除选中记录?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

        for(int i =lstMerge->Items->Count - 1;i>=0 ;i--)
        {
            li = lstMerge->Items->Item[i];
            if(li->Selected  == true)
            {
                int sid = li->Caption.ToInt();

                ret = FmIC->gdb.VQuery(512,"select checkstate,mergeid  from authinput where sid=%d",sid);
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
                    MessageBox(Handle,"记录已核打,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                long aissid = atoi(row[1]);

                ret = FmIC->gdb.VQuery(512,"delete from authinput where sid=%d",sid);
                if(ret)
                {
                    MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                if( aissid>0)
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

        if(curid > lstMerge->Items->Count)
            curid = 0;      
        if( lstMerge->Items->Count>curid ) lstMerge->Items->Item[curid]->Selected = true;
    }
    else  if(pageCtrl->TabIndex == 1)
    {
        li= lstEnvs->Selected;
        if(li==NULL) return ;

        int curid = lstEnvs->ItemIndex;

        if(MessageBox(Handle,"是否要删除选中记录?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

        for(int i =lstEnvs->Items->Count -1;i>=0 ;i--)
        {
            li = lstEnvs->Items->Item[i];
            if(li->Selected  == true)
            {
                int sid = li->Caption.ToInt();

                ret = FmIC->gdb.VQuery(512,"select substate,mergeid,envid  from authinput where sid=%d",sid);
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
                    MessageBox(Handle,"记录已提交,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                long aissid = atoi(row[1]);

                ret = FmIC->gdb.VQuery(512,"delete from authinput where envid=%d or sid=%d",sid,sid);
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

        if(curid > lstEnvs->Items->Count)
            curid = 0;  
        if( lstEnvs->Items->Count>curid ) lstEnvs->Items->Item[curid]->Selected = true;
    }
    else
        return ;

    MessageBox(Handle,"删除成功!\t","提示",MB_OK|MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------


void __fastcall TFormWS::lstMainDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstMain->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,mergestate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,state,mainflag from authinput where sid=%d",sid);
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
    int  state;
    int  mainflag;

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
    state = atoi(row[11]);
    mainflag = atoi(row[12]);

    FormWsImage->txtType->Visible = true;
    FormWsImage->Label1->Visible = true;

    if(submitstate!=0 || state == 0 ) FormWsImage->btnSave->Visible = false;
    else                FormWsImage->btnSave->Visible = true;


    if( strlen(amount)<=0) strcpy(amount,"0");

    FormWsImage->txtAmount->Text = amount;      


    if(vchflag == 2 || vchflag == 6 || vchflag == 7 )     FormWsImage->txtType->ItemIndex =  0 ;
    else if(mainflag == 1 )       FormWsImage->txtType->ItemIndex =  1 ;
    else       FormWsImage->txtType->ItemIndex =  2 ;


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
    FormWsImage->type  =  3;
    FormWsImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();
    ti_RGB2BMPFREE(bmp);

    ret = FormWsImage->ShowModal();     

    if(ret == mrOk )
    {
        memset(amount,0,sizeof(amount));   
        strcpy(amount,FormWsImage->txtAmount->Text.c_str());
        if(FormWsImage->txtType->ItemIndex  == 0 )
        {
            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,mainflag=0,multiflag=1,model=2,clkno='%s' where sid=%d",RemoveThousandChar(amount),g_sys_opt,sid);
         }
        else if (FormWsImage->txtType->ItemIndex  == 1 )
        {
            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,mainflag=1,model=0,multiflag=1 ,clkno='%s' where sid=%d",RemoveThousandChar(amount),g_sys_opt,sid);
        }
        else
            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,mainflag=2,model=0,multiflag=1,clkno='%s' where sid=%d",RemoveThousandChar(amount),g_sys_opt,sid);

        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }
        li->SubItems->Strings[3] = RemoveThousandChar( amount );
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormWS::pageCtrlChange(TObject *Sender)
{
    R1Click(NULL);
}
//---------------------------------------------------------------------------      

int TFormWS::subMit(const char* exchno)
{
    //TODO: Add your source code here
    //信封改成非主件
    FmIC->gdb.VQuery(512,"update authinput set mainflag=-1,envid=sid where  ( model=2 or model=6 or model=7) and  exchno='%s' and substate!=1 and area='%s' and date='%s' and session='%s'",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());

    //非税全部改成主件
    FmIC->gdb.VQuery(512,"update authinput set mainflag=1   multiflag=3  and model int in (2,6,7) and  exchno='%s' and substate!=1 and area='%s' and date='%s' and session='%s'",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());


    int ret = FmIC->gdb.VQuery(512,"select count(*) from authinput where state!=1 and exchno='%s' and area='%s' and date='%s' and session='%s'",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return  ret;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    if(atoi(row[0])>0)
    {
        MessageBox(Handle,"扫描数据没识别完或者识别有误,不能进行匹配!\t","提示",MB_OK|MB_ICONINFORMATION);
        pageCtrl->ActivePageIndex = 0 ;
        cmbIdentify->ItemIndex = 3;
        flushData1();
        return -1;
    }

    CheckComplex(exchno);

    ret = FmIC->gdb.VQuery(512,"select count(*) from authinput where state=6 and exchno='%s' and area='%s' and date='%s' and session='%s'",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return  ret;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();
    if(atoi(row[0])>0)
    {
        MessageBox(Handle,"联次连续,不能进行匹配!\t","提示",MB_OK|MB_ICONINFORMATION);
        pageCtrl->ActivePageIndex = 0 ;
        cmbIdentify->ItemIndex = 3;
        flushData1();
        return -1;
    }

    //检查第一个图像必须是信封
    ret = FmIC->gdb.VQuery(512,"select sid,model from authinput where  checkstate!=1 and exchno='%s' and area='%s' and date='%s' and session='%s' order by sid limit 1",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return  ret ;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();
    if(row == NULL)
    {
        return 0;
    }
    int ktc = atoi(row[1]);
    if(ktc!=2 && ktc!=6 && ktc!=7)
    {
        MessageBox(Handle,"未匹配记录第一个图像不是信封,可能扫描错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        pageCtrl->ActivePageIndex = 0 ;
        cmbIdentify->ItemIndex = 1;
        flushData1();
        return -1;
    }

    //检查是否有两个连续分隔页
    ret = FmIC->gdb.VQuery(512,"select sid,model from authinput where exchno='%s' and checkstate!=1  and prestate=0 and area='%s' and date='%s' and session='%s' order by sid",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return   ret;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();
    bool last=false ;
    while(row!=NULL)
    {
        int vf = atoi(row[1]);
        if(vf == 2 || vf ==6 || vf ==7)
        {
            if(last == true )
            {
                 MessageBox(Handle,"委收匹配错误，存在连续两个信封!\t","提示",MB_OK|MB_ICONINFORMATION);
                 Screen->Cursor = crDefault;
                 return -1;
            }
            last = true;
        }
        else    last = false;
        row = res.Fetch();
    }
    ret = FmIC->gdb.VQuery(512,"select sid,model,vchtype,mainflag,amount,multiflag from authinput where exchno='%s' and checkstate!=1 and prestate=0 and area='%s' and date='%s' and session='%s' order by sid",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return ret ;
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
        if(vchflag == 2 || vchflag ==6 || vchflag == 7)                                      //如果是分隔页
        {
            envid = sid;
            vchtypes = row[2];
            row = res.Fetch();
            continue;
        }

        ret = FmIC->gdb.VQuery(512,"update authinput set vchtype='%s',envid=%d ,mainid=%d where sid=%d",vchtypes.c_str(),envid,mainid,sid);  //更新后续凭证类型、信封号、
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return ret;
        }
        row = res.Fetch();
    }                 

    //判断委收第一联、第二联金额是否相符   
    ret = FmIC->gdb.VQuery(512,"select sid,trim(amount),mainflag  from authinput where (multiflag=1 or multiflag=2) and exchno='%s' and checkstate!=1 and (model!=2  and model!=6 and model!=7) and area='%s' and date='%s' and session='%s' order by sid",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return ret;
    }

    res = FmIC->gdb.Store();
    row = res.Fetch();

    long _firstsid;
    char _fisrtamt[18];
    char _secondamt[18];

    while(row!=NULL)
    {
        if( atoi(row[2]) == 1)
        {
            memset(_fisrtamt,0,sizeof(_fisrtamt));
            _firstsid = atoi(row[0]);
            strcpy(_fisrtamt,row[1]);
        }
        else if( atoi(row[2]) == 2)
        {
            memset(_secondamt,0,sizeof(_secondamt));
            strcpy(_secondamt,row[1]);
            if(strcmp(_fisrtamt,_secondamt) == 0)
            {
                ret = FmIC->gdb.VQuery(512,"update authinput set mergestate=1 where sid in (%d,%d) ",_firstsid,atoi(row[0]));
                if(ret)
                {
                    MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return ret;
                }
            }
        }
        else
        {
             memset(_fisrtamt,0,sizeof(_fisrtamt));
            _firstsid = -1;
        }
        row = res.Fetch();
    }


    //自动匹配信封与清分机信封
    //更新清分数据重新进行匹配

    ret = FmIC->gdb.VQuery(512,"select sid,model,vchtype,amount from authinput where   (authinput.model=2 or authinput.model=6 or authinput.model=7) and mergestate!=1 and exchno='%s' and area='%s' and date='%s' and session='%s' ", exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
       MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
       debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
       Screen->Cursor = crDefault;
       return ret;
    }
    res = FmIC->gdb.Store();
    row = res.Fetch();

    while(row!=NULL)
    {
        sid = atoi(row[0]);
        vchflag = atoi(row[1]);
        vchtypes = row[2];
        amt = row[3];

        ret = FmIC->gdb.VQuery(1024,"select sid from dists where mergestate=0 and vchtype='%s' and (isenv>0)  and amount='%s' and exchno='%s'  and date='%s' and session='%s' limit 1",vchtypes.c_str(),amt.c_str(),exchno,txtDate->Text.c_str(),txtSession->Text.c_str());
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error[%s]\n",mysql_error(FmIC->gdb.Handle()));
            return  ret;
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

            FmIC->gdb.VQuery(512,"update dists set mergestate=3 ,mergeid=%d where sid=%d",sid,aissid );
            FmIC->gdb.VQuery(512,"update authinput set mergestate=1 ,mergeid=%d where sid=%d",aissid,sid);
        }

        row = res.Fetch();
    }

     //对全部匹配通过的委收信封进行提交
    ret = FmIC->gdb.VQuery(512,"select sid,amount   from authinput where exchno='%s' and ( multiflag=1 or multiflag=2 ) and substate!=1 and area='%s' and mergestate=1 and ( model=2 or model = 6 or model=7) and date='%s' and session='%s' order by sid",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return ret;
    }

    res = FmIC->gdb.Store();

    while( (row = res.Fetch())!=NULL )
    {
        sid = atoi(row[0]);

        //所有第一联、第二联都已匹配
        ret = FmIC->gdb.VQuery(512,"select count(*) from authinput where envid='%d' and mergestate!=1  and (mainflag=1 or mainflag=2) and date='%s' and session='%s'",sid,txtDate->Text.c_str(),txtSession->Text.c_str());
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return ret;
        }
                     
        MyRes res2 = FmIC->gdb.Store();
        MyRow row2 = res2.Fetch();
        if( atoi(row2[0])>0)
        {
            continue;
        }

        //所有第一联总金额与信封金额相等
        ret = FmIC->gdb.VQuery(512,"select count(*),sum(amount) from authinput where envid='%d'  and mainflag=1 ",sid);
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return ret;
        }

        res2 = FmIC->gdb.Store();
        row2 = res2.Fetch();  
        if(row[1] == NULL || row2[1]== NULL) continue;
        if( strcmp(row2[1],row[1])!=0)
        {
            if( atof( row2[1])!=atof(row[1]))
                continue;
        }

        ret = FmIC->gdb.VQuery(512,"update authinput set checkstate=1,substate=1,mergestate=1  where envid='%d'  or sid =%d ",sid,sid);
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return ret;
        }
    }

    //对全部匹配通过的非税信封进行提交
    ret = FmIC->gdb.VQuery(512,"select sid,amount   from authinput where exchno='%s' and  substate!=1 and area='%s' and multiflag=3 and mergestate=1 and ( model=2 or model = 6 or model=7) and date='%s' and session='%s' order by sid",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return ret;
    }

    res = FmIC->gdb.Store();

    while( (row = res.Fetch())!=NULL )
    {
        sid = atoi(row[0]);  
      
        //所有第一联总金额与信封金额相等
        ret = FmIC->gdb.VQuery(512,"select count(*),sum(amount) from authinput where envid=%d  and mainflag=1 ",sid);
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return ret;
        }

        MyRes res2 = FmIC->gdb.Store();
        MyRow row2 = res2.Fetch();

        if(row[1] == NULL || row2[1]== NULL) continue;

        if( strcmp(row2[1],row[1])!=0)
        {
            if( atof( row2[1])!=atof(row[1]))
                continue;
        }

        ret = FmIC->gdb.VQuery(512,"update authinput set checkstate=1,substate=1,mergestate=1  where envid='%d'  or sid =%d ",sid,sid);
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return ret;
        }
    }

    return 0;
}
void __fastcall TFormWS::btnMergeClick(TObject *Sender)
{

    if(cmbNetno->ItemIndex == 0 ) return  ;


    Screen->Cursor = crHourGlass;
   
    int ret = subMit(cmbNetno->Text.c_str());

    Screen->Cursor = crDefault;
    if(ret == 0 )
    {
        MessageBox(Handle,"匹配成功!\t","提示",MB_OK|MB_ICONINFORMATION);
    }

    R1Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormWS::cmbNetStateCloseUp(TObject *Sender)
{
    if( locked == true)
    {
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,7),'0',right(state,8)) where exchno='%s' and right(left(state,8),1)!='1' and date='%s' and session=%s and area='%s'",curNetno.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str(),g_sys_area.c_str());
        locked = false;
    }
    if(cmbNetState->ItemIndex == 0 )
        flushExchno(0);
    else if(cmbNetState->ItemIndex == 1 )
        flushExchno(1);
    else
        flushExchno(2);
}
//---------------------------------------------------------------------------
void __fastcall TFormWS::lstFsDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstFs->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,mergestate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,state,mainflag from authinput where sid=%d",sid);
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
    int  state;
    int  mainflag;

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
    state = atoi(row[11]);
    mainflag = atoi(row[12]);

    FormWsImage->txtType->Visible = true;
    FormWsImage->Label1->Visible = true;

    if(submitstate!=0 || state == 0 ) FormWsImage->btnSave->Visible = false;
    else                FormWsImage->btnSave->Visible = true;


    if( strlen(amount)<=0) strcpy(amount,"0");

    FormWsImage->txtAmount->Text = amount;


    if(vchflag == 2 || vchflag == 6 || vchflag == 7 )     FormWsImage->txtType->ItemIndex =  0 ;
    else if(mainflag == 1 )       FormWsImage->txtType->ItemIndex =  1 ;
    else       FormWsImage->txtType->ItemIndex =  2 ;


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
    FormWsImage->type  =  4;
    FormWsImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    ti_RGB2BMPFREE(bmp);

    ret = FormWsImage->ShowModal();        

    if(ret == mrOk )
    {
        memset(amount,0,sizeof(amount));   
        strcpy(amount,FormWsImage->txtAmount->Text.c_str());
        if(FormWsImage->txtType->ItemIndex  == 0 )
        {
            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,mainflag=0,multiflag=3,model=2,clkno='%s' where sid=%d",RemoveThousandChar(amount),g_sys_opt,sid);
         }
        else if (FormWsImage->txtType->ItemIndex  == 1 )
        {
            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,mainflag=1,multiflag=3,model=0,clkno='%s' where sid=%d",RemoveThousandChar(amount),g_sys_opt,sid);
        }
        else
            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,mainflag=2,multiflag=3,model=0,clkno='%s' where sid=%d",RemoveThousandChar(amount),g_sys_opt,sid);

        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }

        li->SubItems->Strings[3] = RemoveThousandChar( amount );
    }
}
//---------------------------------------------------------------------------


int TFormWS::flushData5()
{
    //TODO: Add your source code here

    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";
     
    if(cmbNetno->ItemIndex == 0 )
        cond = cond + " and 1= 1";
    else
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";

    cond = cond + " and area='"+g_sys_area.c_str()+"'";
    
    if(cbMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(cbMerge->ItemIndex == 1 )
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
        if(atoi(row[8])== 1 )
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

   statusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";

   if(FmIC->gmode !=2 )
   {
        statusBar->Panels->Items[1]->Text = String("人行总笔数[")+row[0]+"]";
        statusBar->Panels->Items[2]->Text = String("人行总金额[")+row[1]+"]";
   }
   else
   {
        statusBar->Panels->Items[1]->Text = String("清分总笔数[")+row[0]+"]";
        statusBar->Panels->Items[2]->Text = String("清分总金额[")+row[1]+"]";
   }
   txtDetail->Items->EndUpdate();
   return 0;
}

void __fastcall TFormWS::txtDetailDblClick(TObject *Sender)
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
       R1Click(NULL);
    }        
}
//---------------------------------------------------------------------------


void __fastcall TFormWS::I1Click(TObject *Sender)
{
    if( pageCtrl->TabIndex != 1 )
    {
        MessageBox(Handle,"请在信封信息页面进行补扫!\t","提示",MB_OK|MB_ICONINFORMATION);
        pageCtrl->ActivePageIndex = 1;
        flushData1();
        return ;
    }

    TListItem* li= lstEnvs->Selected;
        if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select checkstate,date,session,exchno,multiflag,area  from authinput where sid=%d",sid);
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
        MessageBox(Handle,"记录已提交,不能补扫!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }

    FormReplace->setDSEx(sid,row[1],row[2],row[3],atoi(row[4]),atoi(row[0]));  
    FormReplace->ShowModal();
  
}
//---------------------------------------------------------------------------     

int TFormWS::CheckComplex(const char* exchno)
{
    //TODO: Add your source code here

    FmIC->gdb.VQuery(512,"update authinput set mainflag=-1,envid=sid where  ( model=2 or model=6 or model=7) and  exchno='%s' and substate!=1 and area='%s' and date='%s' and session='%s'",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());

    FmIC->gdb.VQuery(512,"update authinput set mainflag=1 where mainflag= 2 and multiflag=3 and  exchno='%s' and substate!=1 and area='%s' and date='%s' and session='%s'",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());

    //查询委收信息
    int ret = FmIC->gdb.VQuery(512,"select sid,mainflag from authinput where   substate!=1 and mergestate!=1 and multiflag<3 and model not in(2,6,7) and exchno='%s' and area='%s' and date='%s' and session='%s' order by sid",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return  ret;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    int  lastpg;
    long lastid;

    if(row == NULL) return 0;

    if(atoi(row[1]) != 1 )
    {
        lastid = atoi(row[0]);
        FmIC->gdb.VQuery(512,"update authinput set state=6 where sid=%d",lastid);
    }
    else
    {
        lastid  = atoi(row[0]);
        lastpg  = 1;
        row = res.Fetch();
    }
    while(row!=NULL)
    {
        if(atoi(row[1]) == lastpg )
        {
           if(lastpg == 1)     //都是第一联  前面那个有问题
           {
               FmIC->gdb.VQuery(512,"update authinput set state=6 where sid=%d",lastid);
               lastid = atoi(row[0]);
           }
           else                //都是第二联  后面那个有问题
           {
               FmIC->gdb.VQuery(512,"update authinput set state=6 where sid=%d", atoi(row[0]));
           }
           row = res.Fetch();
           continue;
        }

        lastpg = atoi(row[1]);
        lastid = atoi(row[0]);
        row = res.Fetch();
    }
    
    return 0;       
}
void __fastcall TFormWS::FindDlgFind(TObject *Sender)
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
        pFound = lstFs ;    
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


bool TFormWS::FindResult(String StrToFind, TListItem* pItem, bool bExact)
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

void __fastcall TFormWS::F1Click(TObject *Sender)
{
    FindDlg->Execute();
}
//---------------------------------------------------------------------------


void __fastcall TFormWS::cmbNetnoSelect(TObject *Sender)
{
    if(cmbNetno->ItemIndex!=0 )
        R1Click(NULL);

     if( locked == true)
    {
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,7),'0',right(state,8)) where exchno='%s' and right(left(state,8),1)!='1' and area='%s' and date='%s' and session='%s'",lastNetno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
        locked = false;
    }


   // R1Click(NULL); 
    if(cmbNetno->ItemIndex!=0 )
    {
        debugout("update [%s]",cmbNetno->Text);
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,7),'2',right(state,8)) where exchno='%s' and right(left(state,8),1)!='1' and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
        locked = true;
    }
    curNetno = cmbNetno->Text;
}
//---------------------------------------------------------------------------


void __fastcall TFormWS::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
    curSess  = txtSession->Text.c_str();

    if( locked == true)
    {
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,7),'0',right(state,8)) where exchno='%s' and right(left(state,8),1)!='1' and date='%s' and session=%s and area='%s'",curNetno.c_str(),txtDate->Text.c_str(),lastSess.c_str(),g_sys_area.c_str());
        locked = false;
    }
    if(cmbNetState->ItemIndex == 0 )
        flushExchno(0);
    else if(cmbNetState->ItemIndex == 1 )
        flushExchno(1);
    else
        flushExchno(2);
    R1Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormWS::txtSessionChange(TObject *Sender)
{
    curSess  = txtSession->Text.c_str();  
}
//---------------------------------------------------------------------------

void __fastcall TFormWS::UpDown1Changing(TObject *Sender,
      bool &AllowChange)
{
    lastSess  = txtSession->Text.c_str();     
}
//---------------------------------------------------------------------------

