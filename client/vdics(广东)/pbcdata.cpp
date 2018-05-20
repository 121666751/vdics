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
#include "pbcdata.h"
#include <list>
#include "pbcimg.h"
#include "ConfigFile.h"
#include "upLoadsum.h"
#include "waitdlg.h"
#include "pubfun.h"
#include "login.h"
using namespace std;
using namespace xml;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPBC *FormPBC;
//---------------------------------------------------------------------------
__fastcall TFormPBC::TFormPBC(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormPBC::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormPBC::numExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormPBC::btnDirClick(TObject *Sender)
{

    Screen->Cursor = crHourGlass	;

    //显示选择文件对话框,只显示以.xml结尾的文件

    String _originPath = GetCurrentDir();

    
    LoadDialog->Filter  =AnsiString(".xml");
    LoadDialog->Execute();

    char  fname[100];
    memset(fname,0,sizeof(fname));
    strcpy(fname,LoadDialog->FileName.c_str());
    txtFname->Text = fname;
    Screen->Cursor = crDefault	;

    if(txtFname->Text.Length()<=0)
    {
        return;
    }

    String newPath = GetCurrentDir();

   
   memset(qsfile,0,sizeof(qsfile));

    String dd = txtDate->Date.FormatString("yyyymmdd");


  //  sprintf(qsfile,"%s//QS%08s%s%03d.txt",newPath.c_str(),dd.c_str(),g_sys_area.c_str(),txtSess->Text.ToInt());
    sprintf(qsfile,"%s\\QS%08s%s%03d.txt",newPath.c_str(),dd.c_str(),g_sys_area.c_str(),txtSess->Text.ToInt());

	debugout("qsfile = %s",qsfile);

    if(!ZDoesFileExist(qsfile))
    {
        String b = AnsiString("汇总文件[")+qsfile+"]不存在\t";
        MessageBox(Handle,b.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    SetCurrentDirectory(_originPath.c_str());
    checkHead(fname);  
    
}
//---------------------------------------------------------------------------
int TFormPBC::checkHead(const char* fname)
{
    //TODO: Add your source code here
    DocT doc; //定义解析对象
    if(doc.parseFile(fname))
    {
        MessageBox(Handle,"非法提回文件\n","提示",MB_OK|MB_ICONINFORMATION);
        txtFname->Text = "";
        return -1;
    }

    NodeT node = doc.getRoot();	// 获取root节点
	if(node.name()!="MXQingDan")
    {
        MessageBox(Handle,"提回文件节点内容不对\n","提示",MB_OK|MB_ICONINFORMATION);
        txtFname->Text = "";
        return  -2;
    }

    node.child();	// 移动到子节点       
    
	while( node.fetch() )
    {
        String aaa = node.name().c_str();
        if(node.name()=="check")
        {
            continue;
        }

        if(node.name()=="head")
        {
             NodeT node2 = node;

             node2.child();
             while(node2.fetch())
             {

                String aa = node2.name().c_str();
                if(node2.name() == "text")
                {
                    continue;
                }
                if( node2.name() == "head_row")
                {
                     NodeT node3 = node2;
                     node3.child();
                     while(node3.fetch())
                     {
                        if(node3.name()=="text")
                        {
                            continue;
                        }
                        else if (node3.name()=="rq")
                            strcpy(_date,node3.value().c_str());
                        else if (node3.name() == "cc")
                            strcpy(_session,node3.value().c_str());
                        else if (node3.name() == "total")
                            _total = atoi(node3.value().c_str());
                        else if (node3.name() == "bankno")
                            strcpy(_bankno,node3.value().c_str());
						else if (node3.name() == "qy")
							_area = node3.value().c_str();
                     }
                }
             }
        }  
    }


    String dd = txtDate->Date.FormatString("yyyymmdd");
    if(strcmp(dd.c_str(),_date)!=0)
    {
        String msg = AnsiString("提入文件日期[")+_date+"]与提入日期["+ dd.c_str()+"]不一致,请检查!\t";
        MessageBox(Handle,msg.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        txtFname->Text = "";
        return -3;
    }

    if(atoi(txtSess->Text.c_str())!=atoi(_session))
    {
        String msg = AnsiString("提入文件场次[")+_session+"]与提入场次["+ txtSess->Text.c_str()+"]不一致,请检查!\t";
        MessageBox(Handle,msg.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        txtFname->Text = "";
        return -4;
    }

    LabelCount->Caption = _total;
    return 0;
}


int TFormPBC::ReadNJPBC(const char* fname)
{
    //TODO: Add your source code here       

    int ret = checkHead(fname);
    if(ret) return ret;

    String dd = txtDate->Date.FormatString("yyyymmdd");

   
    DocT doc; //定义解析对象
    if(doc.parseFile(fname))
    {
        MessageBox(Handle,"非法提回文件\n","提示",MB_OK|MB_ICONINFORMATION);
        txtFname->Text = "";
        return  -1;
    }

    NodeT node = doc.getRoot();	// 获取root节点
	if(node.name()!="MXQingDan")
    {
        MessageBox(Handle,"提回文件节点内容不对\n","提示",MB_OK|MB_ICONINFORMATION);
        txtFname->Text = "";
        return -2 ;
    }

    node.child();	// 移动到子节点

    barTotal->Max = LabelCount->Caption.ToInt();
    barTotal->Position = 0;
    barTotal->Step = 1;

   PBCLIST  myPbc;

   ATTACHLIST  fnameVector;
   ATTACHLIST  multiVector;
   attachments_node fnameNode;

   pbc_node_t  *pbcData ;
   pbcData = new pbc_node_t[ _total+1];

   bool eraseFlag = false;
	while( node.fetch() )
    {
        String aaa = node.name().c_str();
        if(node.name()=="head")
        {
            continue;
        }

        if(node.name()=="mx")
        {
             NodeT node2 = node;
             node2.child();
             while(node2.fetch())
             {
                String aa = node2.name().c_str();
                if(node2.name() == "text")
                {
                    continue;
                }

                eraseFlag = false;
                if( node2.name() == "mx_row")
                {
                     NodeT node3 = node2;
                     node3.child();

                     pbc_node_t myNode;

                     memset(&myNode,0,sizeof(myNode));

                     myNode.envelop = 0;
                     String amt;

                     while(node3.fetch())
                     {
						debugout("node.name =%s",node3.name());

                        if(node3.name()=="text")
                        {
                            continue;
                        }
                        else if (node3.name() == "jhh")
                        {
                            sprintf(myNode.exchno,"%06d",atoi(node3.value().c_str()));
                        }
                        else if (node3.name() == "zh")
                            strcpy(myNode.accno,myTrim((char*)node3.value().c_str()));
                        else if (node3.name() == "pzh")
                            strcpy(myNode.vchno,myTrim((char*)node3.value().c_str()));
                        else if (node3.name() == "jdbz")
                            strcpy(myNode.cdcode,node3.value().c_str());
                        else if (node3.name() == "pzzl")
                            sprintf(myNode.vchtype,"%02d",atoi(node3.value().c_str()));
                        else if (node3.name() == "je")
                        {
                            amt = node3.value().c_str();
                            amt = amt.Trim().c_str();

                            if(amt.Length() == 1 )
                                amt = "00"+amt;
                            if(amt.Length() == 2)
                                amt = "0"+amt;

                            amt.Insert(".",amt.Length()-1);

                            sprintf(myNode.amount,"%.2f", atof(amt.c_str()));

                        }
                        else if (node3.name() == "voucher_row")
                        {
                            NodeT node4 = node3;
                            node4.child();
                            while(node4.fetch())
                            {
                                if(node4.name()=="text")
                                {
                                    continue;
                                }
                                else if (node4.name() == "zmtp")
                                {
                                    strcpy(myNode.fname,node4.value().c_str());
                                    fnameNode.fname = myNode.fname;
                                    fnameNode.attsid = myNode.envelop;
                                    fnameNode.vchtype = myNode.vchtype;
                                    fnameNode.netno   = myNode.exchno;
                                    fnameNode.leader  = myNode.fname;
                                    fnameVector.push_back(fnameNode);                                                                          
                                }
                            }
                        }
                        else if (node3.name() == "attachments")
                        {
                            NodeT node4 = node3;
                            node4.child();
                           

                            if(eraseFlag == false )
                            {
                                fnameVector.erase(fnameVector.end());

                                multiVector.push_back(fnameNode);

                                eraseFlag = true;
                            }   
                            while(node4.fetch())
                            {    
                                if(node4.name()=="text")
                                {
                                    continue;
                                }
                                else if(node4.name() == "zmtp")
                                {
                                    myNode.envelop++ ;
                                    fnameNode.attsid = myNode.envelop;
                                    fnameNode.fname = node4.value().c_str();
                                    fnameNode.vchtype = myNode.vchtype;
                                    fnameNode.leader  = myNode.fname;
                                    fnameNode.netno   = myNode.exchno;
                                    multiVector.push_back(fnameNode);
                                }
                            }
                        }

                    }
                    strcpy(myNode.date,_date);
                    strcpy(myNode.session,_session);
                    strcpy(myNode.ip,g_ip);

					debugout("amount = %s",myNode.amount);

                    InertOne(myNode);
                    barTotal->Position++;
                    myPbc.push_back(myNode);
                    Application->ProcessMessages();
                }
             }
         }
    }      

    ret = savePBC(myPbc,fnameVector,multiVector);
    if(ret)
    {
        MessageBox(Handle,"保存数据出错\t","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }

    MessageBox(Handle,"人行数据导入成功\t","提示",MB_OK|MB_ICONINFORMATION);
    return 0;

}
void __fastcall TFormPBC::btnReadDataClick(TObject *Sender)
{
    String dd = txtDate->Date.FormatString("yyyymmdd");
    
    int ret =  FmIC->gdb.VQuery(512,"select count(*) from pbcdata where date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),g_sys_area.c_str());
    if(ret)
     {
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
    FmWait = new TFmWait(Application);

    FmWait->Label1->Caption = "正在导入，请等待...";
    FmWait->Show();
    Application->ProcessMessages();
    
    ReadNJPBC(txtFname->Text.c_str());

    FmWait->Hide();
    delete FmWait; FmWait = NULL;
    Screen->Cursor = crDefault	;
}
//---------------------------------------------------------------------------


int TFormPBC::InertOne(pbc_node_t node)
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
    li->SubItems->Add(node.fname);
    return 0;
}
void __fastcall TFormPBC::FindDlgFind(TObject *Sender)
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


bool TFormPBC::FindResult(String StrToFind, TListItem* pItem, bool bExact)
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
    // 交易序号
    sub_str = pItem->SubItems->Strings[4];
    found = (sub_str==StrToFind);
    if( found ) return true;

     // 帐号搜索
    sub_str = pItem->Caption;
    return (sub_str.Pos(StrToFind)>0);
}
void __fastcall TFormPBC::tolFindClick(TObject *Sender)
{
     FindDlg->Execute();

     stBar->Panels->Items[1]->Text = "";
     stBar->Panels->Items[2]->Text = "";

     stBar->Panels->Items[3]->Text = "";
     stBar->Panels->Items[4]->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TFormPBC::tolPBCClick(TObject *Sender)
{
    btnReadDataClick(NULL);    
}
//---------------------------------------------------------------------------
                                

void __fastcall TFormPBC::lstImageDblClick(TObject *Sender)
{
    TListItem* li = lstImage->Selected;
    if(li == NULL) return ;
    pbc_node_t node;
    memset(&node,0,sizeof(node));

    strcpy(node.exchno,li->SubItems->Strings[3].c_str());
    strcpy(node.fname,li->SubItems->Strings[4].c_str());  

    int ret =  FmIC->gdb.VQuery(512,"select ip,date,session,accno,vchno,amount,sid,right(exchno,5) from pbcdata where fname='%s' and exchno='%s'",node.fname,node.exchno);
    if(ret)
    {
        MessageBox(Handle,"查询错误","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    if(row!=NULL)
    {
        strcpy(node.ip,row[0]);
        strcpy(node.date,row[1]);
        strcpy(node.session,row[2]);
        strcpy(node.accno,row[3]);
        strcpy(node.vchno,row[4]);
        strcpy(node.amount,row[5]);
        node.sid = atoi(row[6]);
        strcpy(node.exchno,row[7]);
    }

    FormDet->SetData(node);
    FormDet->ShowModal();
}
//---------------------------------------------------------------------------


int TFormPBC::queryAll()
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

     int ret =  FmIC->gdb.VQuery(512,"select  date,session,accno,vchno,exchno,amount,fname ,vchtype from pbcdata where date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),g_sys_area.c_str());
     if(ret)
     {
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return  0;
     }

	 debugout("select  date,session,accno,vchno,exchno,amount,fname ,vchtype from pbcdata where date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),g_sys_area.c_str());
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
        strcpy(t.fname,row[6]);
        strcpy(t.vchtype,row[7]);
        InertOne(t);
        row = res.Fetch();
     }
     lstImage->Items->EndUpdate();
     Screen->Cursor = crDefault	;

     ret = FmIC->gdb.VQuery(512,"select count(*),sum(amount) from pbcdata where cdcode='D' and date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),g_sys_area.c_str());
     if(ret)
     {
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
        return  0;
     }

     res = FmIC->gdb.Store();
     row = res.Fetch();
     stBar->Panels->Items[3]->Text = AnsiString("贷方总金额[")+ row[1]+"]";
     stBar->Panels->Items[4]->Text = AnsiString("贷方总笔数[")+ row[0]+"]";
     return 0;
}
void __fastcall TFormPBC::tolRefreshClick(TObject *Sender)
{
    queryAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormPBC::FormShow(TObject *Sender)
{
    GetLocalHost();

    memset(_date,0,sizeof(_date));
    memset(_session,0,sizeof(_session));

   
    FmIC->getDS("date",_date);
    FmIC->getDS("session",_session);

    ZDateTime zdate;
    zdate.SetBankDateStr(_date);
    txtDate->Date = StrToDate(zdate.DateString().c_str());

    txtSess->Text = _session;

    queryAll(); 

}
//---------------------------------------------------------------------------


void __fastcall TFormPBC::ToolButton1Click(TObject *Sender)
{

    String dd = txtDate->Date.FormatString("yyyymmdd");


    int ret= FmIC->gdb.VQuery(512,"select count(*) from vouchers where  state!=1 and date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),g_sys_area.c_str());
    if(ret)
	{
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ;
	}

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    
    if(atoi(row[0])>0)
    {
        MessageBox(Handle,"支票分析没有做完,不能生成预扣文件!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    ZSocket so;

    ret = so.Connect("MESSAGE_SERVER",8855);
    if(ret)
    {
        MessageBox(Handle,"后台通讯错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    char cmd[64];
    char str[8];
    memset(cmd,0,sizeof(cmd));
    memset(str,0,sizeof(str));

    sprintf(cmd,"00000023031010%08s%06s%03d",dd.c_str(),g_sys_area.c_str(),txtSess->Text.ToInt());

    debugout("send [%s]\n",cmd);
    ret = so.Send(cmd,strlen(cmd));
    if(ret<=0)
    {
        MessageBox(Handle,"后台通讯错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    memset(cmd,0,sizeof(cmd));

   	ret =  so.Recv(cmd,8);
  	if(ret!=8)
  	{
        MessageBox(Handle,"后台通讯错误!\t","提示",MB_OK|MB_ICONINFORMATION);
  		return ;
  	}
  	
  	int len = atoi(cmd);
  	memset(cmd,0,sizeof(cmd));
  	ret =  so.Recv(cmd,len);
  	if(ret!=len)
  	{
  		MessageBox(Handle,"后台通讯错误!\t","提示",MB_OK|MB_ICONINFORMATION);
  		return ;
  	}
  	
	strncpy(str,cmd+6,4);
	
	
	if(strncmp(str,"0000",4) == 0)
	{
        MessageBox(Handle,"生成预扣文件成功\t","提示",MB_OK|MB_ICONINFORMATION);
	}
    else
        MessageBox(Handle,"生成预扣文件失败\t","提示",MB_OK|MB_ICONINFORMATION);

}
//---------------------------------------------------------------------------


int TFormPBC::createPreFile(const char* fname)
{
    //TODO: Add your source code here
    pbc_node_t  *pbcData ;
    int count=0;

    String dd = txtDate->Date.FormatString("yyyymmdd");

    String oneLine;

    _unlink(fname);
    ofstream ofs(fname,ios_base::app/*|ios_base::binary*/);

    char head[100];
    sprintf(head,"%s%s%03d%11s",dd.c_str(),_cfg.area,txtSess->Text.ToInt(),_cfg.brno);
    ofs<<head<<endl;


    int ret =  FmIC->gdb.VQuery(512,"select exchno,amount*100,accno,vchno,vchtype,cdcode  from pbcdata where cdcode='D' and  date='%s' and session=%s",dd.c_str(),txtSess->Text.c_str());
    if(ret)
	{
		return ret ;
	}

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    
    while(row!=NULL)
    {
        oneLine = formatOneData(row[0],row[1],row[2],row[3],row[4],row[5]);
        ofs<<oneLine.c_str()<<endl;
        row = res.Fetch();
    }
    return 0;
}

String TFormPBC::formatAmout(double amt)
{
    //TODO: Add your source code here
    char amount[16];
	sprintf(amount, "%.2f", amt);
	string tmp = amount;

	int i = 0;
	int pos = 0;	
	while((pos = tmp.length() - (3*(i+1)+i+3)) > 0 ) {
			tmp.insert(pos, ",");	
			i++;
	}
	return tmp.c_str();
}

String TFormPBC::formatOneData(String netno,String amount,String accno,String vchno,String vchtype,String cdcode)
{
    //TODO: Add your source code here

    String vt = getBankType(vchtype);

    CAnsiMemFile memFile;
   
    
    //12位交换号，左补0
    memFile.Write(netno.c_str(),12,netno.Length(),true,'0');

    //币种
    memFile.Write("CNY",3,3,false);

    //15位金额，左补0
    memFile.Write(amount.c_str(),15,amount.Length(),true,'0');

    //21位账号，右补空格
    memFile.Write(accno.c_str(),21,accno.Length(),false);

    //8位凭证号，左补0
    memFile.Write(vchno.c_str(),30,vchno.Length(),true,'0');

    //3位凭证类型，左补0
    memFile.Write(vt.c_str(),3,vt.Length(),true,'0');

    //8位凭证日期，
    memFile.Write(" ",8,1,true,' ');

    //32位付款人账号，右补空格
    memFile.Write(accno.c_str(),32,accno.Length(),false);

    //60位付款人账号，右补空格
    memFile.Write(" ",60,1,false);

    //32位收款人账号，右补空格
    memFile.Write(" ",32,1,false);

    //60位收款人名称，右补空格
    memFile.Write(" ",60,1,false);

    //60位摘要，右补空格
    memFile.Write(" ",60,1,false);
    
    memFile.Write("D",1,1,false);

    memFile.Write("\0", 1, 1, false);

    return (char*)memFile.GetPtr();
}

int TFormPBC::makePreProcess()
{
    //TODO: Add your source code here
    return 0;
}



int TFormPBC::savePBC(PBCLIST  pbclist,ATTACHLIST fnamelist,ATTACHLIST multilist)
{  
    char cmd[1024];

    String dd = txtDate->Date.FormatString("yyyymmdd");
    
    int ret = 0;
    
	MemT  fmem;
    fmem.loadFromFile(qsfile);

    string s((char*)fmem.data(),fmem.size());
	istringstream file(s);

    string dramount="0.00";
    string cramount="0.00";
    string dcamount="0.00";
    string ccamount="0.00";
    
    string netno="";
    string str;

    int i = 0;

	while(!file.eof())
    {
		getline(file,str);
        i++;
        String b = str.c_str();
        if(i == 1 )
        {
            _area =  b.SubString(9,6).c_str();
            debugout("交换区域[%s]",_area.c_str());
        }
        else if(str.size()>30)
        {
             
		  //  netno =  b.SubString(1,12).c_str();
		   	netno =  b.SubString(7,6).c_str();       /*
//----------------------------------------------------20150511
            int n_netno = atoi(b.c_str());
            char ch[12] = {0};
            itoa(n_netno,ch,10);
            netno = ch;       */
//----------------------------------------------------20150511
            ccamount = b.SubString(16,15).c_str();
            dcamount = b.SubString(31,15).c_str();
            cramount = b.SubString(46,15).c_str();
            dramount = b.SubString(61,15).c_str();


            char cmd[512];
            memset(cmd,0,sizeof(cmd));

            sprintf(cmd,"replace into bocctrl set state='1000000000000000',exchno='%s',dramount=%s/100.00,cramount=%s/100.00,dcamount=%s/100.00,ccamount=%s/100.00,date='%s',session='%d',area='%s'",netno.c_str(),dramount.c_str(),cramount.c_str(),dcamount.c_str(),ccamount.c_str(),dd.c_str(), txtSess->Text.ToInt(),g_sys_area.c_str());
			debugout("cmd = %s",cmd);

            ret = FmIC->gdb.Query(cmd);
            if(ret)
            {
                clearData();
                MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
		        return ret;
            }
        }
    }
  
    PBCLIST::iterator  it ;
    for(it = pbclist.begin(); it!=pbclist.end(); ++it)

	{

		memset(cmd,0,sizeof(cmd));

		sprintf(cmd,"insert into pbcdata(date,session,exchno,accno,vchno,cdcode,fname,amount,vchtype,envelop,ip,timestamp,area,clkno) values('%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,'%s',now(),'%s','%s')",
			         it->date,it->session,it->exchno,it->accno,it->vchno,it->cdcode,it->fname,it->amount,it->vchtype,it->envelop,g_ip,_area.c_str(),g_sys_opt.c_str());
		ret = FmIC->gdb.Query(cmd);
		if(ret)
		{
            clearData();
			MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
			return ret;
		}
	}  

    ATTACHLIST::iterator  itf ;

    String bname;
   
    for(itf = fnamelist.begin(); itf!=fnamelist.end(); ++itf)
	{     
		memset(cmd,0,sizeof(cmd));

		if (strcmp(g_sys_area.c_str(),BRNO_SZ)== 0)
		{

			sprintf(cmd,"insert into vouchers(fname,vchtype,exchno,pkgno,ip,date,session,timestamp,scantime,dpi,area,clkno,orgcode) values('%s','%s','%s','%s','%s','%s','%s',now(),now(),200,'%s','%s','%s')",
				itf->fname.c_str(),itf->vchtype.c_str(),itf->netno.c_str(),itf->netno.c_str(),g_ip,_date,_session,_area.c_str(),g_sys_opt.c_str(),g_sys_org.c_str());
		}
		else
		{
			sprintf(cmd,"insert into vouchers(fname,vchtype,exchno,pkgno,ip,date,session,timestamp,scantime,dpi,area,clkno,orgcode) values('%s','%s','%s','%s','%s','%s','%s',now(),now(),300,'%s','%s','%s')",
				itf->fname.c_str(),itf->vchtype.c_str(),itf->netno.c_str(),itf->netno.c_str(),g_ip,_date,_session,_area.c_str(),g_sys_opt.c_str(),g_sys_org.c_str());
		}


		ret =  FmIC->gdb.Query(cmd);
		if(ret)
		{
            clearData();
			MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
			return ret;
		}
    }

    int model;
    int multiflag = 0 ;
    for(itf = multilist.begin(); itf!=multilist.end(); ++itf)
    {
        if(itf->attsid == 0 )   model = 2;
        else                    model = 0;
    
        if(itf->attsid >1)  multiflag = 1;
        else                multiflag = 0;
      
        memset(cmd,0,sizeof(cmd));

		if (strcmp(g_sys_area.c_str(),BRNO_SZ)== 0)
		{
			sprintf(cmd,"insert into multis(fname,vchtype,exchno,pkgno,ip,date,session,timestamp,scantime,dpi,model,area,multiflag,clkno,orgcode ) values('%s','%s','%s','%s','%s','%s','%s',now(),now(),200,%d,'%s',%d,'%s','%s')",
				itf->fname.c_str(),itf->vchtype.c_str(),itf->netno.c_str(),itf->netno.c_str(),g_ip,_date,_session,model,_area.c_str(),multiflag,g_sys_opt.c_str(),g_sys_org.c_str());
		}
		else
		{
			sprintf(cmd,"insert into multis(fname,vchtype,exchno,pkgno,ip,date,session,timestamp,scantime,dpi,model,area,multiflag,clkno,orgcode ) values('%s','%s','%s','%s','%s','%s','%s',now(),now(),300,%d,'%s',%d,'%s','%s')",
				itf->fname.c_str(),itf->vchtype.c_str(),itf->netno.c_str(),itf->netno.c_str(),g_ip,_date,_session,model,_area.c_str(),multiflag,g_sys_opt.c_str(),g_sys_org.c_str());
		}
	
		ret =  FmIC->gdb.Query(cmd);
		if(ret)
		{
            clearData();
			MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
			return ret;
		}
      
	}

    //设置影像 反面图像文件名
    ret = FmIC->gdb.VQuery(512,"update vouchers set bname=concat(left(fname,length(fname)-4),'b',right(fname,4)) where date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),_area.c_str());
    if(ret)
	{
        clearData();
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    ret = FmIC->gdb.VQuery(512,"update multis set bname=concat(left(fname,length(fname)-4),'b',right(fname,4)) where date='%s' and session=%s and area='%s'",dd.c_str(),txtSess->Text.c_str(),_area.c_str());
    if(ret)
	{
        clearData();
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    //设置影像 与 数据对应的 流水
    ret = FmIC->gdb.VQuery(512,"update vouchers,pbcdata set vouchers.accno=pbcdata.accno,vouchers.vchtype=pbcdata.vchtype,vouchers.amount=pbcdata.amount,vouchers.vchno=pbcdata.vchno,vouchers.mergeid=pbcdata.sid,vouchers.mergestate=1 where  vouchers.fname=pbcdata.fname and vouchers.exchno=pbcdata.exchno  and vouchers.date='%s' and vouchers.session=%s and vouchers.area='%s'",dd.c_str(),txtSess->Text.c_str(),_area.c_str());
    if(ret)
	{
        clearData();
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    //设置数据 与 影像 对应流水
    ret = FmIC->gdb.VQuery(512,"update pbcdata,vouchers set pbcdata.mergeid=vouchers.sid ,pbcdata.mergestate=1 where    vouchers.fname=pbcdata.fname and vouchers.exchno=pbcdata.exchno  and pbcdata.date='%s' and pbcdata.session=%s",dd.c_str(),txtSess->Text.c_str());
    if(ret)
	{
        clearData();
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    //设置影像 与 数据对应的 流水
    ret = FmIC->gdb.VQuery(512,"update multis,pbcdata set multis.mergeid=pbcdata.sid,multis.mergestate=1 ,multis.amount = pbcdata.amount where  multis.fname=pbcdata.fname and multis.exchno=pbcdata.exchno  and multis.model=2 and  multis.date='%s' and multis.session=%s and multis.area='%s'",dd.c_str(),txtSess->Text.c_str(),_area.c_str());
    if(ret)
	{
        clearData();
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    //设置数据 与 影像 对应流水
    ret = FmIC->gdb.VQuery(512,"update pbcdata,multis set pbcdata.mergeid=multis.sid ,pbcdata.mergestate=1 where    multis.fname=pbcdata.fname and multis.exchno=pbcdata.exchno  and multis.model=2 and pbcdata.date='%s' and pbcdata.session=%s and pbcdata.area='%s'",dd.c_str(),txtSess->Text.c_str(),_area.c_str());
    if(ret)
	{
        clearData();
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('session',%s)",txtSess->Text.c_str());
    if(ret)
	{
        clearData();
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('date','%s')",dd.c_str());
    if(ret)
	{
        clearData();
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}


    return 0;
}
void __fastcall TFormPBC::txtDateChange(TObject *Sender)
{
    queryAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormPBC::txtDownClick(TObject *Sender, TUDBtnType Button)
{
    queryAll();
}
//---------------------------------------------------------------------------
int  TFormPBC::GetLocalHost()
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

	//PrintToLog("本地IP[%s]",g_ip);

	return 0;
}

int TFormPBC::getCFG()
{
    //TODO: Add your source code here         
    return 0;
}



String TFormPBC::getBankType(String vchtype)
{
    //TODO: Add your source code here
    String vt  = _vtMap[vchtype];
    if(vt.Length()<2 )
        vt = "000";

    return vt;
}
void __fastcall TFormPBC::ToolButton7Click(TObject *Sender)
{
    String dd = txtDate->Date.FormatString("yyyymmdd");


    int ret= FmIC->gdb.VQuery(512,"select count(*) from vouchers where prestate=1 and date='%s' and session=%s",dd.c_str(),txtSess->Text.c_str());
    if(ret)
	{
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ;
	}

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    if(atoi(row[0])>1)
    {
        MessageBox(Handle,"预扣已完成\t","提示",MB_OK|MB_ICONINFORMATION );
         return ;    
    }


    ret = FmIC->gdb.VQuery(512,"select count(*) from bocctrl where substring(state,2,1)=1 and date='%s' and session=%s", dd.c_str(),txtSess->Text.c_str());
    if(ret)
	{
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ;
	}
    
    res = FmIC->gdb.Store();
    row = res.Fetch();
    if(atoi(row[0])==0)
    {
         MessageBox(Handle,"预扣文件没有生成,不能继续!\t","提示",MB_ICONINFORMATION|MB_ICONINFORMATION);
         return ;
    }


    if(MessageBox(Handle,"预扣文件是否已导入流程控制系统?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;  


    ret = FmIC->gdb.VQuery(512,"update pbcdata set prestate=1 where date='%s' and session=%s",dd.c_str(),txtSess->Text.c_str());
    if(ret)
	{
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ;
	}

    ret = FmIC->gdb.VQuery(512,"update vouchers set prestate=1 where date='%s' and session=%s",dd.c_str(),txtSess->Text.c_str());
    if(ret)
	{
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ;
	}

    MessageBox(Handle,"处理成功!\t","提示",MB_ICONINFORMATION|MB_ICONINFORMATION);
  
}
//---------------------------------------------------------------------------


void __fastcall TFormPBC::ToolButton3Click(TObject *Sender)
{
    FormSign->ShowModal();    
}
//---------------------------------------------------------------------------


int TFormPBC::clearData()
{
    //TODO: Add your source code here
    String dd = txtDate->Date.FormatString("yyyymmdd");
     
    int ret= FmIC->gdb.VQuery(512,"delete from pbcdata where area='%s'  and date='%s' and session=%s",g_sys_area.c_str(),dd.c_str(),txtSess->Text.c_str());
    if(ret)
	{
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    ret= FmIC->gdb.VQuery(512,"delete from vouchers where area='%s'  and date='%s' and session=%s",g_sys_area.c_str(),dd.c_str(),txtSess->Text.c_str());
    if(ret)
	{
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    ret= FmIC->gdb.VQuery(512,"delete from multis where area='%s'  and date='%s' and session=%s",g_sys_area.c_str(),dd.c_str(),txtSess->Text.c_str());
    if(ret)
	{
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    ret= FmIC->gdb.VQuery(512,"delete from bocctrl where area='%s'  and date='%s' and session=%s",g_sys_area.c_str(),dd.c_str(),txtSess->Text.c_str());
    if(ret)
	{
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ret;
	}

    return 0;
}

