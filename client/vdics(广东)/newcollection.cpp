//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <zcommon.h>
#include <transimg.h>
#include <libmem.h>
#include <bcbext.h>

#include "newcollection.h"
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
TFormNWS *FormNWS;
//---------------------------------------------------------------------------
__fastcall TFormNWS::TFormNWS(TComponent* Owner)
    : TForm(Owner)
{
    locked = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormNWS::FormShow(TObject *Sender)
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
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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


int TFormNWS::flushData1()
{
    //TODO: Add your source code here
    String cond;

    cond = AnsiString(" and date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";

    if(cmbNetno->ItemIndex == 0  || cmbNetno->Text=="ȫ    ��")
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


    String str = "select sid,exchno,fname,model,vchtype, amount,state,mergestate ,mainflag,mainid,checkstate,mergeid from authinput  where 1=1 ";
    str = str +cond;
    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
            li->SubItems->Insert(2,"�ŷ�Ʊ��");
       else if(atoi(row[3]) == 6)
            li->SubItems->Insert(2,"�ŷ�Ʊ��");
       else if(atoi(row[3]) == 7)
            li->SubItems->Insert(2,"�ŷ�Ʊ��");
       else
       {
            if(atoi(row[8]) == 1 )
                li->SubItems->Insert(2,"��һ��");
            else if( atoi(row[8]) == 2 )
                li->SubItems->Insert(2,"�ڶ���");
            else
                li->SubItems->Insert(2,"");
       }
       li->SubItems->Insert(3,row[4]);
       li->SubItems->Insert(4,row[5]);
       if(atoi(row[6]) == 0 )
            li->SubItems->Insert(5,"δʶ��");
       else  if(atoi(row[6]) == 1 )
            li->SubItems->Insert(5,"��ʶ��");
       else  if(atoi(row[6]) == 3 )
            li->SubItems->Insert(5,"����ʧ��");
       else  if(atoi(row[6]) == 5 )
            li->SubItems->Insert(5,"ʶ��ʧ��");
       else  if(atoi(row[6]) == 6 )
            li->SubItems->Insert(5,"��������");
       else
            li->SubItems->Insert(5,"ʶ����");

       if(atoi(row[7]) == 1 )
            li->SubItems->Insert(6,"��ƥ��");
       else
            li->SubItems->Insert(6,"δƥ��");


       li->SubItems->Insert(7,row[11]);
       row = res.Fetch();
   }

   lstMerge->Items->EndUpdate();

    str = "select count(*),sum(amount) from authinput where  ( model=2 or model=6 or model=7 )  ";

    str = str +cond;
    ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }



   res =  FmIC->gdb.Store();
   row = res.Fetch();

   statusBar->Panels->Items[0]->Text = String("������[")+cmbNetno->Text.c_str()+"]";
   statusBar->Panels->Items[1]->Text = String("�ŷ��ܱ���[")+row[0]+"]";
   statusBar->Panels->Items[2]->Text = String("�ŷ��ܽ��[")+row[1]+"]";
   
   return 0;
}


int TFormNWS::flushData2()
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
        cond = cond + " and mergestate=0 ";


    String str = "select sid,exchno,fname,vchtype, amount,mergestate,mergeid,checkstate from authinput where ( model=2 or model=6 or model=7 )  ";
    str = str +cond;
    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
            li->SubItems->Insert(4,"��ƥ��");
       else
            li->SubItems->Insert(4,"δƥ��");


       li->SubItems->Insert(5,row[6]);
       row = res.Fetch();
   }

    lstEnvs->Items->EndUpdate();

    str = "select count(*),sum(amount) from authinput where  ( model=2 or model=6 or model=7 )  ";

    str = str +cond;
    ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }
        
   res =  FmIC->gdb.Store();
   row = res.Fetch();

   statusBar->Panels->Items[0]->Text = String("������[")+cmbNetno->Text.c_str()+"]";
   statusBar->Panels->Items[1]->Text = String("�ŷ��ܱ���[")+row[0]+"]";
   statusBar->Panels->Items[2]->Text = String("�ŷ��ܽ��[")+row[1]+"]";
   
   return 0;
}


int TFormNWS::flushData3()
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

    String str = "select sid,exchno,fname,vchtype, amount,mergestate,mergeid from authinput where mainflag=1 and multiflag=1 ";
    str = str +cond;
    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
            li->SubItems->Insert(4,"��ƥ��");
       else 
            li->SubItems->Insert(4,"δƥ��");

       li->SubItems->Insert(5,row[6]);

       row = res.Fetch();
   }

   lstMain->Items->EndUpdate();

    str = "select count(*),sum(amount) from authinput where   mainflag=1 and multiflag=1 ";

    str = str +cond;
    ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }    


   res =  FmIC->gdb.Store();
   row = res.Fetch();

   statusBar->Panels->Items[0]->Text = String("������[")+cmbNetno->Text.c_str()+"]";
   statusBar->Panels->Items[1]->Text = String("ί���ܱ���[")+row[0]+"]";
   statusBar->Panels->Items[2]->Text = String("ί���ܽ��[")+row[1]+"]";
   
   return 0;
}

int TFormNWS::flushData4()
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

    String str = "select sid,exchno,fname,vchtype, amount,mergestate,mergeid from authinput where mainflag=1 and multiflag=3 ";
    str = str +cond;

    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
            li->SubItems->Insert(4,"��ƥ��");
       else 
            li->SubItems->Insert(4,"δƥ��");

       li->SubItems->Insert(5,row[6]);

       row = res.Fetch();
   }

   lstFs->Items->EndUpdate();

   
   str = "select count(*),sum(amount) from authinput where  mainflag=1 and multiflag=3";

   str = str +cond;
   ret = FmIC->gdb.Query(str.c_str());
   if(ret)
   {
         MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
   }

   res =  FmIC->gdb.Store();
   row = res.Fetch();

   statusBar->Panels->Items[0]->Text = String("������[")+cmbNetno->Text.c_str()+"]";
   statusBar->Panels->Items[1]->Text = String("��˰�ܱ���[")+row[0]+"]";
   statusBar->Panels->Items[2]->Text = String("��˰�ܽ��[")+row[1]+"]";
   return 0;
}



int TFormNWS::flushExchno(int state)
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
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return 0;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    cmbNetno->Items->Add(" ȫ    ��");
    while(row!=NULL)
    {
        if( state == 2)
        {
            ret =  FmIC->gdb.VQuery(512,"select count(*) from authinput where exchno='%s' and checkstate!=1 and area='%s' and date='%s' and session='%s'", row[0],g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
            if(ret)
            {
                MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

void __fastcall TFormNWS::B1Click(TObject *Sender)
{
    TListItem* li;
    int ret = 0;


    li= lstMerge->Selected;
    if(li==NULL) return ;
    
    
    if(!FmIC->m_bNetDelete)
   	{
   		MessageBox(Handle,"�ù�Աû������ɾ��Ȩ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
   		return;
   	}

    int sid = li->Caption.ToInt();

    ret = FmIC->gdb.VQuery(512,"select pkgno  from authinput where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return ;

    String netno =  row[0];

    String sw = AnsiString("�Ƿ�Ҫɾ������[")+netno +"]ɨ����Ϣ?\t";
    if(MessageBox(Handle,sw.c_str(),"��ʾ",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;


    ret = FmIC->gdb.VQuery(512,"select count(*) from authinput where checkstate=1 and pkgno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    
    res  = FmIC->gdb.Store();
    row  = res.Fetch();

    if(row==NULL) return ;

    int ct = atoi(row[0]) ;
    if(ct !=0)
    {
        MessageBox(Handle,"���м�¼�ύ,����ɾ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
/*
    MessageBox(Handle,"����ɾ����Ҫ��Ȩ!\t","��ʾ",MB_OK|MB_ICONINFORMATION);

    if( FormWarrant->ShowModal() == mrCancel)
    {
        MessageBox(Handle,"��Ȩʧ��,����ɾ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
*/
    ret = FmIC->gdb.VQuery(512,"delete from authinput where  pkgno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }


     ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0,mergeid=-1 where mergestate=3 and  exchno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
     if(ret)
     {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
     }

     ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=-1 where mergestate=3 and  exchno='%s' and area='%s' and date='%s' and session='%s'",netno.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
     if(ret)
     {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
     }
    MessageBox(Handle,"ɾ���ɹ�!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
    R1Click(0);
}
//---------------------------------------------------------------------------



void __fastcall TFormNWS::R1Click(TObject *Sender)
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
            MessageBox(Handle,"None","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
    catch(...)
    {
        Screen->Cursor = crDefault;
    }

    Screen->Cursor = crDefault;

}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::btnRefreshClick(TObject *Sender)
{
    R1Click(NULL);    
}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::lstMergeDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstMerge->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int curid = lstMerge->ItemIndex;

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,mergestate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,state,mainflag,substate,multiflag from authinput where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

    //δʶ���ñ���
    if( state == 0 ) FormWsImage->btnSave->Visible = false;
    else                FormWsImage->btnSave->Visible = true;


    //��ƥ�� �ŷⲻ�ñ���
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
        MessageBox(Handle,"��ȡͼ�����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

int TFormNWS::getNext()
{
    TListItem* li;
    if(lstMerge->Items->Count>0)
    {
        li= lstMerge->Items->Item[0];
        li->Delete();
    }
    if(lstMerge->Items->Count==0)
    {
        MessageBox(Handle,"��¼�������\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return -1;
    }

    li= lstMerge->Items->Item[0];
    li->Selected = true;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,mergestate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,state,mainflag,substate,multiflag from authinput where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

   //δʶ���ñ���
    if( state == 0 ) FormWsImage->btnSave->Visible = false;
    else                FormWsImage->btnSave->Visible = true;


    //��ƥ�� �ŷⲻ�ñ���
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
        MessageBox(Handle,"��ȡͼ�����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

void __fastcall TFormNWS::lstEnvsDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstEnvs->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,checkstate,amount,mainflag,fname,bname,right(pkgno,5),vchtype,date,session  from authinput where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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


    FormWsCheck->btnSave->Visible = false;
    FormWsCheck->btnCheck->Visible = false;

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
        MessageBox(Handle,"��ȡͼ�����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

    R1Click(NULL);
}
//---------------------------------------------------------------------------


void __fastcall TFormNWS::btnExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------



void __fastcall TFormNWS::cmbNetnoDropDown(TObject *Sender)
{
    lastNetno =  curNetno;
    flushExchno(cmbNetState->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::cmbIdentifyCloseUp(TObject *Sender)
{
     R1Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::cbMergeCloseUp(TObject *Sender)
{
    R1Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::cbCheckCloseUp(TObject *Sender)
{
     R1Click(NULL);    
}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::MeForceLookClick(TObject *Sender)
{
    TListItem* li;
    int ret = 0;    

    if( pageCtrl->TabIndex == 0 )
    {
        li= lstMerge->Selected;
        if(li==NULL) return ;
        int curid = lstMerge->ItemIndex;

        if(MessageBox(Handle,"�Ƿ�Ҫɾ��ѡ�м�¼?\t","��ʾ",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

        for(int i =lstMerge->Items->Count - 1;i>=0 ;i--)
        {
            li = lstMerge->Items->Item[i];
            if(li->Selected  == true)
            {
                int sid = li->Caption.ToInt();

                ret = FmIC->gdb.VQuery(512,"select checkstate,mergeid  from authinput where sid=%d",sid);
                if(ret)
                {
                    MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }
                MyRes res  = FmIC->gdb.Store();
                MyRow row  = res.Fetch();

                if(row==NULL) return ;

                int substate = atoi(row[0]) ;
                if(substate !=0)
                {
                    MessageBox(Handle,"��¼�Ѻ˴�,����ɾ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                long aissid = atoi(row[1]);

                ret = FmIC->gdb.VQuery(512,"delete from authinput where sid=%d",sid);
                if(ret)
                {
                    MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                if( aissid>0)
                {
                    ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }

                    ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

        if(MessageBox(Handle,"�Ƿ�Ҫɾ��ѡ�м�¼?\t","��ʾ",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

        for(int i =lstEnvs->Items->Count -1;i>=0 ;i--)
        {
            li = lstEnvs->Items->Item[i];
            if(li->Selected  == true)
            {
                int sid = li->Caption.ToInt();

                ret = FmIC->gdb.VQuery(512,"select substate,mergeid,envid  from authinput where sid=%d",sid);
                if(ret)
                {
                    MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
                    MessageBox(Handle,"��¼���ύ,����ɾ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                long aissid = atoi(row[1]);

                ret = FmIC->gdb.VQuery(512,"delete from authinput where envid=%d or sid=%d",sid,sid);
                if(ret)
                {
                    MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                    debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                    return;
                }

                if(aissid!=0)
                {
                    ret = FmIC->gdb.VQuery(512,"update pbcdata set mergestate=0,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
                        return;
                    }

                    ret = FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=-1 where sid=%d",aissid);
                    if(ret)
                    {
                        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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

    MessageBox(Handle,"ɾ���ɹ�!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------


void __fastcall TFormNWS::lstMainDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstMain->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,mergestate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,state,mainflag from authinput where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"��ȡͼ�����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        else
            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,mainflag=1,model=0,multiflag=1,clkno='%s' where sid=%d",RemoveThousandChar(amount),g_sys_opt,sid);

        if(ret)
        {
            MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }
        li->SubItems->Strings[3] = RemoveThousandChar( amount );
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::pageCtrlChange(TObject *Sender)
{
    R1Click(NULL);
}
//---------------------------------------------------------------------------      

int TFormNWS::Merge(const char* exchno)
{
    //TODO: Add your source code here
    //�ŷ�ĳɷ�����
    FmIC->gdb.VQuery(512,"update authinput set mainflag=-1,envid=sid where  ( model=2 or model=6 or model=7) and  exchno='%s' and substate!=1 and area='%s' and date='%s' and session='%s'",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());

    //��˰ȫ���ĳ�����
    FmIC->gdb.VQuery(512,"update authinput set mainflag=1   multiflag=3  and model int in (2,6,7) and  exchno='%s' and substate!=1 and area='%s' and date='%s' and session='%s'",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());


    int ret = FmIC->gdb.VQuery(512,"select count(*) from authinput where state!=1 and exchno='%s' and area='%s' and date='%s' and session='%s'",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return  ret;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    if(atoi(row[0])>0)
    {
        MessageBox(Handle,"ɨ������ûʶ�������ʶ������,���ܽ���ƥ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        pageCtrl->ActivePageIndex = 0 ;
        cmbIdentify->ItemIndex = 3;
        flushData1();
        return -1;
    }

    //����һ��ͼ��������ŷ�
    ret = FmIC->gdb.VQuery(512,"select sid,model from authinput where  checkstate!=1 and exchno='%s' and area='%s' and date='%s' and session='%s' order by sid limit 1",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"δƥ���¼��һ��ͼ�����ŷ�,����ɨ�����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        pageCtrl->ActivePageIndex = 0 ;
        cmbIdentify->ItemIndex = 1;
        flushData1();
        return -1;
    }

    //����Ƿ������������ָ�ҳ
    ret = FmIC->gdb.VQuery(512,"select sid,model from authinput where exchno='%s' and checkstate!=1  and prestate=0 and area='%s' and date='%s' and session='%s' order by sid",exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
                 MessageBox(Handle,"ί��ƥ����󣬴������������ŷ�!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        if(vchflag == 2 || vchflag ==6 || vchflag == 7)                                      //����Ƿָ�ҳ
        {
            envid = sid;
            vchtypes = row[2];
            row = res.Fetch();
            continue;
        }

        ret = FmIC->gdb.VQuery(512,"update authinput set vchtype='%s',envid=%d ,mainid=%d where sid=%d",vchtypes.c_str(),envid,mainid,sid);  //���º���ƾ֤���͡��ŷ�š�
        if(ret)
        {
            MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return ret;
        }
        row = res.Fetch();
    }                 

    //�Զ�ƥ���ŷ�����ֻ��ŷ�
    //��������������½���ƥ��

    ret = FmIC->gdb.VQuery(512,"select sid,model,vchtype,amount from authinput where   (authinput.model=2 or authinput.model=6 or authinput.model=7) and mergestate!=1 and exchno='%s' and area='%s' and date='%s' and session='%s' ", exchno,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
       MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
            MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
            debugout("sql error[%s]\n",mysql_error(FmIC->gdb.Handle()));
            return  ret;
        }

        MyRes res2 = FmIC->gdb.Store();
        MyRow row2 = res2.Fetch();
        int  mergecd = res2.Count();
        if( mergecd >1)
        {
            String b = AnsiString("����ͬ����[")+vchtypes+"]���["+amt+"]Ʊ��,��ע��ƥ��!\t";
            MessageBox(Handle,b.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
        }
        if(row2!=NULL)
        {
            aissid = atoi(row2[0]);

            FmIC->gdb.VQuery(512,"update dists set mergestate=3 ,mergeid=%d where sid=%d",sid,aissid );
            FmIC->gdb.VQuery(512,"update authinput set mergestate=1 ,mergeid=%d where sid=%d",aissid,sid);
        }

        row = res.Fetch();
    }

    //�����ŷ���Ʊ��ƥ��״̬��ƥ����ˮ
    ret = FmIC->gdb.VQuery(512,"select sid,mergestate,mergeid from authinput where  (authinput.model=2 or authinput.model= 6 or authinput.model=7) and substate!=1 and  exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
            FmIC->gdb.VQuery(512,"update authinput set mergestate = 1 ,mergeid =%d where envid=%d",aissid,id);
        row = res.Fetch();
    }


    return 0;
}
void __fastcall TFormNWS::btnMergeClick(TObject *Sender)
{

    if(cmbNetno->ItemIndex == 0 ) return  ;


    Screen->Cursor = crHourGlass;

    int ret = Merge(cmbNetno->Text.c_str());

    Screen->Cursor = crDefault;
    if(ret == 0 )
    {
        MessageBox(Handle,"ƥ��ɹ�!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
    }

    R1Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::cmbNetStateCloseUp(TObject *Sender)
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
void __fastcall TFormNWS::lstFsDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstFs->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,mergestate,amount,model,fname,bname,right(pkgno,5),vchtype,date,session,state,mainflag from authinput where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"��ȡͼ�����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        else
            ret = FmIC->gdb.VQuery(512,"update authinput set amount='%s',state=1,mainflag=1,multiflag=3,model=0,clkno='%s' where sid=%d",RemoveThousandChar(amount),g_sys_opt,sid);

        if(ret)
        {
            MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            return;
        }

        li->SubItems->Strings[3] = RemoveThousandChar( amount );
    }
}
//---------------------------------------------------------------------------


int TFormNWS::flushData5()
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
         MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
           li->SubItems->Insert(5,"δƥ��");
       else
           li->SubItems->Insert(5,"��ƥ��");

       li->SubItems->Insert(6,row[7]);

       if(FmIC->gmode !=0 )
       {
        if(atoi(row[8])== 1 )
           li->SubItems->Insert(7,"�ŷ�");
        else
           li->SubItems->Insert(7,"Ʊ��");
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
         MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }



   res =  FmIC->gdb.Store();
   row = res.Fetch();

   statusBar->Panels->Items[0]->Text = String("������[")+cmbNetno->Text.c_str()+"]";

   if(FmIC->gmode !=2 )
   {
        statusBar->Panels->Items[1]->Text = String("�����ܱ���[")+row[0]+"]";
        statusBar->Panels->Items[2]->Text = String("�����ܽ��[")+row[1]+"]";
   }
   else
   {
        statusBar->Panels->Items[1]->Text = String("����ܱ���[")+row[0]+"]";
        statusBar->Panels->Items[2]->Text = String("����ܽ��[")+row[1]+"]";
   }
   txtDetail->Items->EndUpdate();
   return 0;
}

void __fastcall TFormNWS::txtDetailDblClick(TObject *Sender)
{
    if(FmIC->gmode !=2 ) return ;
    
    TListItem* li;
    li= txtDetail->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret = FmIC->gdb.VQuery(512,"select sid, accno,vchno,vchtype,amount ,mergestate,fname,ip ,0,date,session,isenv from dists where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"��ȡͼ�����","��ʾ",MB_OK|MB_ICONINFORMATION);
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


void __fastcall TFormNWS::I1Click(TObject *Sender)
{
    if( pageCtrl->TabIndex != 1 )
    {
        MessageBox(Handle,"�����ŷ���Ϣҳ����в�ɨ!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
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
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return ;

    FormReplace->setDSEx(sid,row[1],row[2],row[3],atoi(row[4]),atoi(row[0]));  
    FormReplace->ShowModal();
  
}
//---------------------------------------------------------------------------     

int TFormNWS::CheckComplex(const char* exchno)
{
    return 0;       
}
void __fastcall TFormNWS::FindDlgFind(TObject *Sender)
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
    //��ָ����¼��ʼ����
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


bool TFormNWS::FindResult(String StrToFind, TListItem* pItem, bool bExact)
{
    //TODO: Add your source code here
    String sub_str = pItem->SubItems->Strings[3];

    // �������
    bool found = (sub_str ==StrToFind);

    if( found ) return true;

    // �������
    sub_str = pItem->SubItems->Strings[4];
    found = (sub_str.Pos(StrToFind)>0);
    if( found ) return true;

    // �ʺ�����
    sub_str = pItem->Caption;
    return (sub_str.Pos(StrToFind)>0);
}

void __fastcall TFormNWS::F1Click(TObject *Sender)
{
    FindDlg->Execute();
}
//---------------------------------------------------------------------------


void __fastcall TFormNWS::cmbNetnoSelect(TObject *Sender)
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


void __fastcall TFormNWS::UpDown1Click(TObject *Sender, TUDBtnType Button)
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

void __fastcall TFormNWS::txtSessionChange(TObject *Sender)
{
    curSess  = txtSession->Text.c_str();  
}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::UpDown1Changing(TObject *Sender,
      bool &AllowChange)
{
    lastSess  = txtSession->Text.c_str();     
}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::btnSubmitClick(TObject *Sender)
{
    Screen->Cursor = crHourGlass;

    int ret = Merge(cmbNetno->Text.c_str());

    Screen->Cursor = crDefault;

    if(ret) return ;

    ret = FmIC->gdb.VQuery(512,"select count(*) from authinput where mergestate=0 and (model=2 or model=6 or model=7) and exchno='%s' and area='%s' and  date='%s' and session='%s'",cmbNetno->Text,g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    int ct = atoi(row[0]);
    if(ct > 0 )
    {
        String sw = AnsiString("��[")+ct +"]���ŷ�û��ƥ����,�Ƿ��ύ?\t";
        if(MessageBox(Handle,sw.c_str(),"��ʾ",MB_YESNO|MB_ICONINFORMATION) == IDNO)
        {
            pageCtrl->ActivePageIndex  = 1 ;
            cbMerge->ItemIndex = 0;
            flushData2();
            return ;
        }
        else
        {
        	
        	if(!FmIC->m_bSend)
    			{
    				MessageBox(Handle,"�ù�Աû��ǿ���ύ!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
    				return;
    			}
        	/*
            MessageBox(Handle,"ǿ���ύ��Ҫ��Ȩ!\t","��ʾ",MB_OK|MB_ICONINFORMATION);

            if( FormWarrant->ShowModal() == mrCancel)
            {
                MessageBox(Handle,"��Ȩʧ��,����ǿ���ύ!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
                return;
            }
           */
        }
    }


    FmIC->gdb.VQuery(512,"update authinput set substate=1,checkstate=1 where exchno='%s' and area='%s' and date='%s' and session='%s'",cmbNetno->Text.c_str(),g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());

    MessageBox(Handle,"�ύ�ɹ�!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
    R1Click(NULL);

}
//---------------------------------------------------------------------------

void __fastcall TFormNWS::cmbNetnoCloseUp(TObject *Sender)
{
    if(cmbNetState->ItemIndex == 0 || cmbNetState->ItemIndex == 2)
    {
        btnMerge->Enabled = true;
        btnSubmit->Enabled = true;
    }
    else
    {
        btnMerge->Enabled = false;
        btnSubmit->Enabled = false;
    }    
}
//---------------------------------------------------------------------------

