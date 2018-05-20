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
#include <AnsiMemfile.h>

#include "pubfun.h"
#include "upLoadsum.h"

#include "login.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSign *FormSign;
//---------------------------------------------------------------------------
__fastcall TFormSign::TFormSign(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
                                                                                  

int TFormSign::getDC()
{
    //TODO: Add your source code here

    txtDCount->Text = "0";

    int ret = FmIC->gdb.VQuery(512,"select count(*) from  multis where mainflag=1 and  vchtype='%s' and exchno='%s'  and date='%s' and session=%s and area='%s'",FmIC->gdcode, txtExchno->Text.c_str(),date,session,g_sys_area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    txtDCount->Text = row[0];    

    return 0;
}
void __fastcall TFormSign::txtExchnoCloseUp(TObject *Sender)
{
    getDC();
}
//---------------------------------------------------------------------------
void __fastcall TFormSign::btnSubmitClick(TObject *Sender)
{
    if(txtExchno->Text.Length()<1) return ;
    int ret =  submitDC(txtExchno->Text.c_str(),txtDCount->Text.ToInt());
    if(ret == 0 )
    {
        String show = AnsiString("网点[")+ txtExchno->Text.c_str() + "]提回退票登记成功";
        MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        ret = FmIC->gdb.VQuery(512,"update bocctrl set state=concat('110',right(state,13)) where exchno='%s' and date='%s' and session=%s and area='%s'",txtExchno->Text.c_str(),date,session,g_sys_area.c_str());
        if(ret)
        {
            MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            return;
        }
        flushExchno();
        getDC();  
    }

}
//---------------------------------------------------------------------------
int TFormSign::submitDC(char* exchno,int count)
{
    //TODO: Add your source code here
    int ret =  FmIC->gdb.VQuery(512,"select notice,area from bocnets where exchno='%s' and area='%s'",exchno,g_sys_area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    String show;

    show = AnsiString("请维护交换号[")+exchno+"]!\t";
    if(row == NULL)
    {
        MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return -1;
    }

    int notice = atoi(row[0]);
    if(notice == 0 ) return 0;

    if(strlen(row[1])<4)
    {
        show = AnsiString("维护交换号[")+row[0]+"交换区域错误!\t";
        MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return -1;
    }

    String headstr = makeHead(ZM_DIST_RTCS);

    CAnsiMemFile memFile;
	memFile.Write(FmIC->m_sOrgCode.c_str(), 6, FmIC->m_sOrgCode.Length(), false);
	memFile.Write(FmIC->m_sOptCode.c_str(), 7, FmIC->m_sOptCode.Length(), true,' ');
    memFile.Write(date,8,strlen(date),false);
    memFile.Write(row[1],6,strlen(row[1]),false);
    memFile.Write(exchno,12,strlen(exchno),true,' ');    

    memFile.Write(session,3,strlen(session),true,'0');
    memFile.Write("CNY",3,3,false);

    char ct[8];
    memset(ct,0,sizeof(ct));
    sprintf(ct,"%d",count);
    memFile.Write(ct,4,strlen(ct),true);
    memFile.Write("\0", 1, 1, false);

    String body = (char*)memFile.GetPtr();
    //String send = headstr + body;

	String send = body;

    char cmd[8];
    char cmdstr[512];
    char sendstr[1024];
    int  result=0;

    memset(sendstr,0,sizeof(sendstr));
    memset(cmd,0,sizeof(cmd));
    memset(cmdstr,0,sizeof(cmdstr));

    strcpy(sendstr,send.c_str());
    ret = doTrade(sendstr,FmIC->gflowip,FmIC->gflowport,result,cmd,cmdstr);
    if(ret)
    {
        MessageBox(Handle,"交易出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }
    if(result!=0)
    {
        String tmp;
        tmp.sprintf("交易出错[%s][%s]!\t",cmd,cmdstr);
        MessageBox(Handle,tmp.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return result;
    }
    return 0;
}

int TFormSign::flushExchno()
{
    //TODO: Add your source code here
    int ret;
    if(chkSign->Checked == true)
        ret = FmIC->gdb.VQuery(512,"select distinct exchno from bocctrl where  left(state,2)='10'  and session=%s and area='%s' order by exchno" ,session,g_sys_area.c_str());
    else
        ret = FmIC->gdb.VQuery(512,"select distinct exchno from bocctrl where  left(state,2)='11'  and session=%s and area='%s' order by exchno",session,g_sys_area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    txtExchno->Items->Clear();
    while(row!=NULL)
    {
        txtExchno->Items->Add(row[0]);
        row = res.Fetch();
    }

    txtExchno->ItemIndex = 0 ;

    return 0;
}
void __fastcall TFormSign::FormShow(TObject *Sender)
{
    memset(date,0,sizeof(date));
    memset(session,0,sizeof(session));
    FmIC->getDS("date",date);
    FmIC->getDS("session",session);
    flushExchno();
    getDC();
}
//---------------------------------------------------------------------------
void __fastcall TFormSign::btnAllClick(TObject *Sender)
{
    int ret = MessageBox(Handle, "确实批量提交吗?\t", "", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
    if( ret != IDYES ) return;

    ret = FmIC->gdb.VQuery(512,"select distinct exchno from bocctrl where  left(state,2)='10' and area='%s' and date='%s' and session='%s'",g_sys_area.c_str(),date,session);
    if(ret)
    {
        MessageBox(Handle,"查询出错!001\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

//-----------------------------20150416-----------------------------------------
        if(row == NULL)
        {
                MessageBox(Handle,"没有数据!\t","提示",MB_OK|MB_ICONINFORMATION);
        }
//------------------------------------------------------------------------------
    char exchno[20];
    int ct;
    while(row!=NULL)
    {
       memset(exchno,0,sizeof(exchno));

       strcpy(exchno,row[0]);
       row = res.Fetch();
       ret =   getCount(exchno,ct);
       if(ret)
       {
            row = res.Fetch();
            return;
       }

       ret =  submitDC(exchno,ct);
       if(ret == 0 )
       {
          ret = FmIC->gdb.VQuery(512,"update bocctrl set state=concat('11',right(state,14)) where exchno='%s'  and date='%s' and session=%s and area='%s'",exchno,date,session,g_sys_area.c_str());
          if(ret)
          {
            MessageBox(Handle,"查询出错!002\t","提示",MB_OK|MB_ICONINFORMATION);
            return;
          }

       }
            return;
    }

}
//---------------------------------------------------------------------------

int TFormSign::getCount(char*exchno,int& ct)
{
    //TODO: Add your source code here
 //   int ret = FmIC->gdb.VQuery(512,"select count(*) from  multis where mainflag=1 and vchtype='%s' and cdcode='D'  and exchno='%s'   and date='%s' and session=%s and area='%s'",FmIC->gdcode, exchno,date,session,g_sys_area.c_str());
    int ret = FmIC->gdb.VQuery(512,"select count(*) from  multis,vchtypes where multis.mainflag=1 and multis.vchtype='%s' and multis.exchno='%s' and multis.date='%s' and multis.session=%s and multis.area='%s' and vchtypes.area='%s' and vchtypes.cdcode=1",FmIC->gdcode, exchno,date,session,g_sys_area.c_str(),g_sys_area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!003\t","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    ct = 0;
    if(row!=NULL)
    {
        ct = atoi(row[0]);
    }

    return 0;
}

void __fastcall TFormSign::chkSignClick(TObject *Sender)
{
    flushExchno();
}
//---------------------------------------------------------------------------

