/*
 ***************************************************************************
 * 作者：   Nie
 * 网址：   www.tchzt.com
 * 邮箱：   support@tchzt.com
 *
 ***************************************************************************
 */
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <libmem.h>
#include <utility/xmlpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <zsocket.hpp>
#include <AnsiMemfile.h>
#include "main.h"
#include <list>
#include "pbcimg.h"
#include "ConfigFile.h"
#include "upLoadsumbj.h"
#include "bjpbc.h"
#include "login.h"

#include <io.h>
#include <zcommon.h>
#include <filectrl.hpp>

using namespace std;
using namespace xml;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBjFormPBC *BjFormPBC;
//---------------------------------------------------------------------------
__fastcall TBjFormPBC::TBjFormPBC(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBjFormPBC::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TBjFormPBC::numExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TBjFormPBC::SpeedButton1Click(TObject *Sender)
{
    AnsiString S;
    S = "";
    if (SelectDirectory("浏览文件夹", "", S))  txtPath->Text = S;

    SetCurrentDirectory(S.c_str());
}
//---------------------------------------------------------------------------


int TBjFormPBC::ReadBJPBC()
{

    char det[100];
    char tot[100];
    memset(det,0,sizeof(det));
    memset(tot,0,sizeof(tot));
    String dd = txtDate->Date.FormatString("yyyymmdd");

    sprintf(tot,"%s//QS%08s110001%03d.txt",txtPath->Text.c_str(),dd.c_str(), txtSess->Text.ToInt());
    sprintf(det,"%s//%08s_%03d_rcv_rmb.txt",txtPath->Text.c_str(),dd.c_str(), txtSess->Text.ToInt());

    if(!ZDoesFileExist(tot))
    {
        String b = AnsiString("汇总文件[")+tot+"]不存在\t";
        MessageBox(Handle,b.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    if(!ZDoesFileExist(det))
    {
        String b = AnsiString("明细文件[")+det+"]不存在\t";
        MessageBox(Handle,b.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    MemT  fmem;
    fmem.loadFromFile(tot);

    string s((char*)fmem.data(),fmem.size());
	istringstream file(s);

    string dramount="0.00";
    string cramount="0.00";
    string netno="";
    string str;

	int ret;
	while(!file.eof())
    {
		getline(file,str);
     
        if(str.length()<30) continue;

        String b = str.c_str();

        netno =  b.SubString(1,12).c_str();

        cramount = b.SubString(46,15).c_str();
        dramount = b.SubString(61,15).c_str();

        char cmd[512];
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"replace into bocctrl set exchno='%s',dramount=%s/100.00,cramount=%s/100,date='%s',session='%d',area='%s'",netno.c_str()+6,dramount.c_str(),cramount.c_str(),dd.c_str(), txtSess->Text.ToInt(),g_sys_area.c_str());

        ret = FmIC->gdb.Query(cmd);
        if(ret)
        {
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
            FmIC->gdb.VQuery(512,"delete from bocctrl where date='%s' and session='%s' and area='%s'",dd.c_str(), txtSess->Text.ToInt(),g_sys_area.c_str());
            return 0 ;
        }
    }


    fmem.loadFromFile(det);     
    string st((char*)fmem.data(),fmem.size());
	istringstream filet(st);


    string vsbank;
    string exchno;
    string accno;
    string vchno;
    string vchtype;
    string amount;
    string cdcode;
    
	while(!filet.eof())
    {
		getline(filet,str);
     
        if(str.length()<30) continue;

        String b = str.c_str();

        vsbank =  b.SubString(1,9).c_str();

        exchno =  b.SubString(11,8).c_str();

        accno = b.SubString(21,14).c_str();

        vchno = b.SubString(36,6).c_str();

        vchtype = b.SubString(53,2).c_str();

        amount = b.SubString(56,14).c_str();

        cdcode = b.SubString(71,1).c_str();

        char cmd[512];
        memset(cmd,0,sizeof(cmd));
    

        sprintf(cmd,"insert  into pbcdata (vsexno,exchno,accno,vchno,vchtype,amount,cdcode,date,session,area,clkno) values('%s','%s','%s','%s','%s',%s/100,'%s','%s',%d,'%s','%s')",vsbank.c_str(),exchno.c_str()+2,accno.c_str(),vchno.c_str(),vchtype.c_str(),amount.c_str(),cdcode.c_str(),dd.c_str(), txtSess->Text.ToInt(),g_sys_area.c_str(),g_sys_opt.c_str());
        ret = FmIC->gdb.Query(cmd);
        if(ret)
        {
            debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
            MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
            FmIC->gdb.VQuery(512,"delete from bocctrl where date='%s' and session='%s' and area='%s'",dd.c_str(), txtSess->Text.ToInt(),g_sys_area.c_str());
            FmIC->gdb.VQuery(512,"delete from pbcdata where date='%s' and session='%s' and area='%s'",dd.c_str(), txtSess->Text.ToInt(),g_sys_area.c_str());
            return  0;
        }
    }
          

    MessageBox(Handle,"人行数据导入成功\t","提示",MB_OK|MB_ICONINFORMATION);


    FmIC->gdb.VQuery(512,"update config set value='%s' where name='date'",dd.c_str());
    FmIC->gdb.VQuery(512,"update config set value='%d' where name='session'",txtSess->Text.ToInt());

    return 0;

}
void __fastcall TBjFormPBC::btnReadDataClick(TObject *Sender)
{
    String dd = txtDate->Date.FormatString("yyyymmdd");

    int ret = FmIC->gdb.VQuery(512,"select count(*) from pbcdata where date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),g_sys_area.c_str());
    if(ret)
     {
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return  ;
     }
    MyRes  res = FmIC->gdb.Store();
    MyRow  row = res.Fetch();
    if(atoi(row[0])>0)
    {
        MessageBox(Handle,"数据已导入,不能重复导入\t","提示",MB_OK|MB_ICONINFORMATION);
        return  ;
    }

    Screen->Cursor = crHourGlass	;
    ReadBJPBC();
    queryAll();
    Screen->Cursor = crDefault	;
}
//---------------------------------------------------------------------------


int TBjFormPBC::InertOne(pbc_node_t node)
{
    //TODO: Add your source code here
    TListItem * li = NULL;
    String str;
    li = lstImage->Items->Add();

    li->Caption = node.accno;

    li->SubItems->Add(node.vchno);
    li->SubItems->Add(node.amount);
    li->SubItems->Add(node.vchtype);
   
    li->SubItems->Add(node.exchno);

    li->SubItems->Add(node.vsexno);
    return 0;
}
void __fastcall TBjFormPBC::FindDlgFind(TObject *Sender)
{

    String find_str = FindDlg->FindText;
    FindDlg->CloseDialog();
    if( find_str.IsEmpty() ) return;

    int sub_no, start_index;
    start_index = lstImage->Selected? lstImage->Selected->Index:0;

    lstImage->SetFocus();

    if( start_index>0) lstImage->Selected->Selected = false;
    //从指定记录开始搜索
    int count = lstImage->Items->Count  ;


    Screen->Cursor = crHourGlass	;
    for( int i=  count-1; i>=0; i-- )
    {
        TListItem* pItem = lstImage->Items->Item[i];
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


bool TBjFormPBC::FindResult(String StrToFind, TListItem* pItem, bool bExact)
{
    //TODO: Add your source code here
     String sub_str = pItem->SubItems->Strings[0];

    // 凭证号搜索
    bool found = (sub_str ==StrToFind);

    if( found ) return true;

    // 金额搜索
    sub_str = pItem->SubItems->Strings[1];
    found = (sub_str.Pos(StrToFind)>0);
    if( found ) return true;


    sub_str = pItem->SubItems->Strings[3];
    found = (sub_str.Pos(StrToFind)>0);
    if( found ) return true;
    //提入行
    sub_str = pItem->SubItems->Strings[4];
    found = (sub_str==StrToFind);
    if( found ) return true;

     // 帐号搜索
    sub_str = pItem->Caption;
    return (sub_str.Pos(StrToFind)>0);
}
void __fastcall TBjFormPBC::tolFindClick(TObject *Sender)
{
     FindDlg->Execute();

     stBar->Panels->Items[1]->Text = "";
     stBar->Panels->Items[2]->Text = "";

     stBar->Panels->Items[3]->Text = "";
     stBar->Panels->Items[4]->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TBjFormPBC::tolPBCClick(TObject *Sender)
{
    btnReadDataClick(NULL);    
}
//---------------------------------------------------------------------------
                                



int TBjFormPBC::queryAll()
{
    //TODO: Add your source code here
     pbc_node_t  *pbcData ;
     int count=0;
     char suma[20];
     char totala[10];
     char sumd[20];
     char totald[10];
     char sumc[20];
     char totalc[10];

     memset(suma,0,sizeof(suma));
     memset(sumd,0,sizeof(sumd));
     memset(sumd,0,sizeof(sumd));

     memset(totala,0,sizeof(totala));
     memset(totald,0,sizeof(totald));
     memset(totalc,0,sizeof(totalc));


     String dd = txtDate->Date.FormatString("yyyymmdd");

     int ret = FmIC->gdb.VQuery(512,"select  date,session,accno,vchno,exchno,amount,vchtype, vsexno from pbcdata where date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),g_sys_area.c_str());
     if(ret)
     {
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);

        return  0;
     }
     MyRes  res = FmIC->gdb.Store();
     MyRow  row = res.Fetch();
     
     Screen->Cursor = crHourGlass	;
     lstImage->Items->BeginUpdate();
     lstImage->Clear();

     pbc_node_t t ;

     while(row !=NULL)
     {
        memset(&t,0,sizeof(t));
        strcpy(t.date,row[0]);
        strcpy(t.session,row[1]);
        strcpy(t.accno,row[2]);
        strcpy(t.vchno,row[3]);
        strcpy(t.exchno,row[4]);
        strcpy(t.amount,row[5]);
        strcpy(t.vchtype,row[6]);
        strcpy(t.vsexno,row[7]);
        InertOne(t);
        row = res.Fetch();
     }
     lstImage->Items->EndUpdate();
     Screen->Cursor = crDefault	;

     ret = FmIC->gdb.VQuery(512,"select count(*),sum(amount) from pbcdata where cdcode='D' and date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),g_sys_area.c_str());
     if(ret)
     {
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);

        return  0;
     }

     res = FmIC->gdb.Store();
     row = res.Fetch();
     stBar->Panels->Items[1]->Text = AnsiString("借方总金额[")+ row[1]+"]";
     stBar->Panels->Items[2]->Text = AnsiString("借方总笔数[")+ row[0]+"]";

     ret = FmIC->gdb.VQuery(512,"select count(*),sum(amount) from pbcdata where cdcode='C' and date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),g_sys_area.c_str());
     if(ret)
     {
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(FmIC->gdb.Handle()));
        return  0;
     }

     res = FmIC->gdb.Store();
     row = res.Fetch();
     stBar->Panels->Items[3]->Text = AnsiString("贷方总金额[")+ row[1]+"]";
     stBar->Panels->Items[4]->Text = AnsiString("贷方总笔数[")+ row[0]+"]";
     return 0;
}
void __fastcall TBjFormPBC::tolRefreshClick(TObject *Sender)
{
    queryAll();
}
//---------------------------------------------------------------------------

void __fastcall TBjFormPBC::FormShow(TObject *Sender)
{                             
    FmIC->getDS("date",_date);
    FmIC->getDS("session",_session);


    ZDateTime zdate;
    zdate.SetBankDateStr(_date);
    txtDate->Date = StrToDate(zdate.DateString().c_str());

    txtSess->Text = _session;

    queryAll();

   
}
//---------------------------------------------------------------------------





void __fastcall TBjFormPBC::txtDateChange(TObject *Sender)
{
    queryAll();
}
//---------------------------------------------------------------------------

void __fastcall TBjFormPBC::txtDownClick(TObject *Sender, TUDBtnType Button)
{
    queryAll();
}
//---------------------------------------------------------------------------
int  TBjFormPBC::GetLocalHost()
{
    //TODO: Add your source code here
    char host_str[100];
	memset( host_str,0,100 );

	int ret;
	ret = gethostname(host_str,100);

	if( ret ) return -1;

	struct hostent* hp;
	hp = (hostent *)gethostbyname(host_str);
	if( hp == NULL ) return -1;

   	unsigned long ipaddr = *(unsigned long *)(hp->h_addr);
    memset(g_ip,0,sizeof(g_ip));
    sprintf(g_ip,"%s",inet_ntoa(*(in_addr*)&ipaddr));

	return 0;
}





void __fastcall TBjFormPBC::ToolButton1Click(TObject *Sender)
{
    FormSignBJ->ShowModal();        
}
//---------------------------------------------------------------------------

