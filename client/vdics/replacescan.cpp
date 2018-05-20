//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "replacescan.h"
#include <io.h>
#include <myplus.h>
#include <ztime.hpp>
#include <zcommon.h>
#include <bcbext.h>
#include "waitdlg.h"
#include "main.h"
#include "pubfun.h"
#include "login.h"
#include "imageid.h"
#include "billinfo.h"
#include "warrant.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormReplace *FormReplace;
//---------------------------------------------------------------------------
__fastcall TFormReplace::TFormReplace(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::muExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------



int TFormReplace::GetLocalHost()
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
    memset(m_ipaddress,0,sizeof(m_ipaddress));
    sprintf(m_ipaddress,"%s",inet_ntoa(*(in_addr*)&ipaddr));

	return 0;
}

int TFormReplace::setDS(int sid,char*date,char*session,char*exchno,char*imageid,char*tab)
{
    //TODO: Add your source code here

    _type = 0;
    
    memset(_date,0,sizeof(_date));
    memset(_session,0,sizeof(_session));
    memset(_exchno,0,sizeof(_exchno));
    memset(_imageid,0,sizeof(_imageid));
    memset(_tbname,0,sizeof(_tbname));   

    strcpy(_date,date);
    strcpy(_session,session);
    strcpy(_exchno,exchno);
    strcpy(_imageid,imageid);
    strcpy(_tbname,tab);
    _envid = sid;

    txtSession->Text=session;
    ZDateTime zdate;
    zdate.SetBankDateStr(date);
    txtDate->Date = StrToDate(zdate.DateString().c_str());

    FormReplace->Caption = "影像替换";

    return 0;
}
void __fastcall TFormReplace::btnStartClick(TObject *Sender)
{
    __int64 FreeDisk = DiskFree(0);
    float FreeGB=FreeDisk/(1024.00*1024.00*1024.00);

    if(FreeGB<0.3)
    {
        MessageBox(Handle,"硬盘空间严重不足，请清理后再扫描!\t","警告", MB_OK);
        return ;
    }

    scanner.bShowUI = (chbShow->Checked==true)?true:false;

   
    memset(curdir,0,sizeof(curdir));
    SetCurrentDirectory(m_Ini.m_scannerInfo.path);

    sprintf(curdir,"%s\\%s",m_Ini.m_scannerInfo.path,_date);
    if(_access(curdir,0)!=0)
    {
        _mkdir(curdir);
    }

    sprintf(curdir,"%s\\%s\\%02d",m_Ini.m_scannerInfo.path,_date,atoi(_session));
    if(_access(curdir,0)!=0)
    {
        _mkdir(curdir);
    }

    sprintf(curdir,"%s\\%s\\%02d",m_Ini.m_scannerInfo.path,_date,atoi(_session));
    if(_access(curdir,0)!=0)
    {
        _mkdir(curdir);
    }

    String kn;

    String CurPkgNo=_exchno;

    if(CurPkgNo.Length()>5)
        kn = CurPkgNo.c_str()+ CurPkgNo.Length() - 5;
    sprintf(curdir,"%s\\%s\\%02d\\%05d",m_Ini.m_scannerInfo.path,_date,atoi(_session),kn.ToInt());
    if(_access(curdir,0)!=0)
    {
        _mkdir(curdir);
    }

    SetCurrentDirectory(curdir);
    scanner.ResetScanner();

    ZDateTime zdate;
    zdate.Now();
    long ct = zdate.GetAnsiVal()%10000000;

    statusBar->Panels->Items[1]->Text = String("正在扫描[")+CurPkgNo+"]";

    btnStop->Enabled = false;
    btnStart->Enabled = false;
    btnCancel->Enabled = true;

    int ret=scanner.SetPkgno(CurPkgNo.c_str(),ct);
    if( btnStart->Caption == "开  始 &A")      Application->ProcessMessages();

    scancount=0;

    try
    {
        Screen->Cursor = crHourGlass;
        scanner.Scan(1);
    }
    catch(...)
    {
        Screen->Cursor = crDefault;
        Application->ProcessMessages();
        btnStart->Enabled = true;
        btnCancel->Enabled = true;
        btnStop->Enabled = true;
        btnStart->Caption = "继  续 &A";
    }

    Screen->Cursor = crDefault;
    btnStart->Enabled = true;
    btnCancel->Enabled = true;
    btnStop->Enabled = true;
    btnStart->Caption = "继  续 &A";
    scancount=scancount+scanner.scancount;


    String msg;
    msg = "[" + CurPkgNo + "] 共["+scancount+"]张是否完成?\t";
    if( MessageBox( Handle, msg.c_str(),"询问",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1) == IDNO )
    {
        btnStartClick(0);
    }

    statusBar->Panels->Items[1]->Text = String("[")+CurPkgNo+"]"+"扫描结束";
    statusBar->Panels->Items[2]->Text = String("本批次总共扫描张数[")+scancount+"]";
}
//---------------------------------------------------------------------------


int TFormReplace::insertFname(const char* fname, int rotate, int fcount)
{
    //TODO: Add your source code here
    TListItem* li;;
    li = txtFnameList->Items->Add();
    li->Caption = fname;

    char bname[80];
    memset(bname,0,sizeof(bname));
    strcpy(bname,fname);
    String t = fname;

    bname[t.Length()-5] = 'B';

    if(chkScan->Checked == true )
    {
        if(_imageFormat == 2 )
        {
            imgF->Picture->LoadFromFile(fname);
            imgB->Picture->LoadFromFile(bname);
        }
        else
        {
            TJPEGImage *ps = new TJPEGImage();
            ps->LoadFromFile(fname);
            imgF->Picture->Bitmap->Assign(ps);
            delete ps;

            TJPEGImage *psb = new TJPEGImage();
            psb->LoadFromFile(bname);
            imgB->Picture->Bitmap->Assign(psb);
            delete psb;               
        }
        imgF->Align = alClient;
        imgF->Left = 1;
        imgF->Top = 1;
        imgF->AutoSize = false;
        imgF->Tag = 1;

        imgB->Align = alClient;
        imgB->Left = 1;
        imgB->Top = 1;
        imgB->AutoSize = alClient;
        imgB->Tag = 1;
        Application->ProcessMessages();
    }
    memset(&Node,0,sizeof(Node));

    strcpy(Node.fname,fname);
    strcpy(Node.bname,bname);

    strcpy(Node.date,_date);
    strcpy(Node.session,_session);
    strcpy(Node.ip,m_ipaddress);

    strcpy(Node.pkgno,_exchno);


    Node.dpi  = m_Ini.m_scannerInfo.onevch.dpi;

    if(_hardRotate == false )
        Node.rotate = m_Ini.m_scannerInfo.onevch.rotate*90;
    else
        Node.rotate = 0;
    Node.fcount = fcount;

    imgList.push_back(Node);

    return 0;
}
void __fastcall TFormReplace::FormDestroy(TObject *Sender)
{
    scanner.CloseScanner();    
}
//---------------------------------------------------------------------------
void __fastcall TFormReplace::FormShow(TObject *Sender)
{
    int ret ;
    FmWait = new TFmWait(Application);

    FmWait->Label1->Caption = "正在初始化扫描设备,请稍后 ...";
    
    FmWait->Show();
    Application->ProcessMessages();

    StartDir = GetCurrentDir();

	strcpy(m_Ini.m_iniPath,StartDir.c_str());
	strcat(m_Ini.m_iniPath,"\\config");


    if( access(m_Ini.m_iniPath,0) )
    {
    	MessageBox(Handle,"读取配置文件失败!\t","错误",MB_OK+MB_ICONERROR);
    }

    ret = m_Ini.LoadIniData("scanner.cfg");   ////初始化m_Ini
    if( ret == -1 )
    {
    	MessageBox(Handle,"请设置系统参数!\t","警告",MB_OK+MB_ICONINFORMATION);
        btnStart->Enabled = false;
        IniLoaded = false;
        return ;
    }
    else IniLoaded = true;

    scanner.SetData(&m_Ini.m_scannerInfo);

    if( scanner.OpenScanner(Handle)==FALSE )
    {
        btnStart->Enabled = false;
        return ;
    }

    btnStart->Enabled = true;

    if( access(m_Ini.m_scannerInfo.path,0) )
    {
    	MessageBox(Handle,"图像文件保存路径不存在!\t","错误",MB_OK+MB_ICONERROR);
        btnStart->Enabled = false;
        IniLoaded = false;
        return ;   
    }
    SetCurrentDirectory(m_Ini.m_scannerInfo.path);


    ret=GetLocalHost();
    if(ret)
        MessageBox(Handle,"读取本机网络地址出错!\t","提示",MB_OK|MB_ICONINFORMATION);

    FmWait->Hide();
    delete FmWait; FmWait = NULL;

    imgF->Align = alNone;
    imgF->Left = 1;
    imgF->Top = 1;
    imgF->AutoSize = true;
    imgF->Tag = 1;
    imgB->Align = alNone;
    imgB->Left = 1;
    imgB->Top = 1;
    imgB->AutoSize = true;
    imgB->Tag = 1;
    Panel6->Height = Panel5->Height / 2;
}
//---------------------------------------------------------------------------
void __fastcall TFormReplace::imgFMouseUp(TObject *Sender,
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
void __fastcall TFormReplace::imgBMouseUp(TObject *Sender,
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
void __fastcall TFormReplace::txtFnameListSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
    String fname;
    TListItem* li;
    li= txtFnameList->Selected;
    if(li==NULL) return ;
    fname = li->Caption ;

    char bname[80];
    memset(bname,0,sizeof(bname));
    strcpy(bname,fname.c_str());

    bname[fname.Length() -5] = 'B';

    if(_imageFormat == 2 )
    {
        imgF->Picture->LoadFromFile(fname);
        imgB->Picture->LoadFromFile(bname);          
    }
    else
    {
        TJPEGImage *ps = new TJPEGImage();
        ps->LoadFromFile(fname);
        imgF->Picture->Bitmap->Assign(ps);
        delete ps;

        TJPEGImage *psb = new TJPEGImage();
        psb->LoadFromFile(bname);
        imgB->Picture->Bitmap->Assign(psb);
        delete psb;
    }
    imgF->Align = alClient;
    imgF->Left = 1;
    imgF->Top = 1;
    imgF->AutoSize = false;
    imgF->Tag = 1;

    imgB->Align = alClient;
    imgB->Left = 1;
    imgB->Top = 1;
    imgB->AutoSize = alClient;
    imgB->Tag = 1;
    return ;    
}
//---------------------------------------------------------------------------
void __fastcall TFormReplace::F1Click(TObject *Sender)
{
    String fname;
    TListItem* li;
    li= txtFnameList->Selected;
    if(li==NULL) return ;
    fname = li->Caption ;

    image_info_t   oneimage;
    IMAGELIST::iterator  pos= imgList.begin();

    while( pos != imgList.end())
	{
        if(strcmp(pos->fname,fname.c_str()) == 0)
        {
            imgList.erase(pos);
            li->Delete();
            break;
        }
        ++pos;
	}    
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::btnCancelClick(TObject *Sender)
{
    txtFnameList->Clear();
    imgList.clear();
    btnStart->Caption = "开  始 &A";
    btnStop->Enabled = false;
    btnCancel->Enabled = false;
    statusBar->Panels->Items[1]->Text = "等待扫描...";
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::btnExitClick(TObject *Sender)
{
     Close();
}


int TFormReplace::setDSEx2()
{
    //TODO: Add your source code here

    _type = 2;

    memset(_date,0,sizeof(_date));
    memset(_session,0,sizeof(_session));
    memset(_exchno,0,sizeof(_exchno));
    memset(_imageid,0,sizeof(_imageid));
    memset(_tbname,0,sizeof(_tbname));   

    strcpy(_exchno,"000000");
    txtSession->Text= 1;
    ZDateTime zdate;
    zdate.SetBankDateStr( FmIC->gdate);
    txtDate->Date = StrToDate(zdate.DateString().c_str());

    FormReplace->Caption = "隔场影像替换";


    return 0;
}

int TFormReplace::setDSEx3()
{
    //TODO: Add your source code here   
    _type = 3;
        
    memset(_date,0,sizeof(_date));
    memset(_session,0,sizeof(_session));
    memset(_exchno,0,sizeof(_exchno));
    memset(_imageid,0,sizeof(_imageid));
    memset(_tbname,0,sizeof(_tbname));

    strcpy(_exchno,"000000");
  
    txtSession->Text= 1;
    ZDateTime zdate;
    zdate.SetBankDateStr( FmIC->gdate);
    txtDate->Date = StrToDate(zdate.DateString().c_str());

    FormReplace->Caption = "隔场流程重启"; 

    return 0;
}
//---------------------------------------------------------------------------

int TFormReplace::setDSEx(int envid,char*date,char*session,char*exchno,int multiflag,int ckst)
{
    //TODO: Add your source code here
    _type = 1;
    memset(_date,0,sizeof(_date));
    memset(_session,0,sizeof(_session));
    memset(_exchno,0,sizeof(_exchno));
    memset(_imageid,0,sizeof(_imageid));
    memset(_tbname,0,sizeof(_tbname));

    strcpy(_date,date);
    strcpy(_session,session);
    strcpy(_exchno,exchno);

    _envid = envid;
    _ckst  = ckst;

    _multiflag = multiflag;

    txtSession->Text=session;
    ZDateTime zdate;
    zdate.SetBankDateStr(date);
    txtDate->Date = StrToDate(zdate.DateString().c_str());

    FormReplace->Caption = "票据补扫";   

    return 0;
}


int TFormReplace::setReplace()
{
    //TODO: Add your source code here
    int size = 0;

    XmlMap gloabXml;
    String show;

    image_info_t   oneimage;
    FileVector t;
    distFileList node;
    char fname[80];
    char bname[80];

    IMAGELIST::iterator  pos= imgList.begin();
    while( pos != imgList.end())
    {
        memset(fname,0,sizeof(fname));
        memset(bname,0,sizeof(bname));
        strcpy(fname,pos->fname);

        memset(&node,0,sizeof(node));
        strcpy(node.fileName,fname);
        sprintf(node.fileFullName,"%s/%s",curdir,fname);
        t.push_back(node);  

        memset(bname,0,sizeof(bname));
        strcpy(bname,fname);

        bname[ strlen(fname) -5] = 'B';

        memset(&node,0,sizeof(node));
        strcpy(node.fileName,bname);
        sprintf(node.fileFullName,"%s/%s",curdir,bname);
        t.push_back(node);
        size = size + ZGetFileSize(pos->fname)+ZGetFileSize(bname);
        ++pos;
	}

    gloabXml["TotalSize"] = size;
    ZDate dt;
    dt.Now();
    String dd = dt.BankDateString().c_str();
    String hh = dt.TimeString().c_str();
    hh.Delete(6,1);
    hh.Delete(3,1);
    gloabXml["TradeDate"] = dd.c_str();
    gloabXml["TradeTime"] = hh.c_str();

    gloabXml["ESBReqTimestamp"] = (dd+hh).c_str();

    gloabXml["EventID"] = (dd+hh+g_sys_opt+"UPD001").c_str();
    gloabXml["CaseID"]  = (dd+hh+g_sys_opt+"UPD001").c_str();
    gloabXml["SubBankNo"] = FmIC->gbrno;
    gloabXml["TellerNo"] = g_sys_opt;
    gloabXml["DocumentID"] = _imageid;     

    String xmlstr = makeUpdateXml(gloabXml,t);

    char Results[8192];
    memset(Results,0,sizeof(Results));
    MemT xmlm;
    xmlm.assign((unsigned char*)xmlstr.c_str(),xmlstr.Length());   

    WriteToLogFile(g_sys_opt.c_str(),"影像替换",0,xmlstr.c_str());
    
    Screen->Cursor = crHourGlass;
    int ret = myECM(xmlm, "127.0.0.1",30003,Results);

    WriteToLogFile(g_sys_opt.c_str(),"影像替换",0,Results);
    Screen->Cursor = crDefault;

    if(ret)
    {
        show.sprintf("ECM上传图像失败[%d]!\t",ret);
        MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }

    XmlMap  outmap;

    xmlm.assign((unsigned char*)Results,strlen(Results));

    outmap = parseOutXml(Results);
    if( (outmap["MsgCode"]!="000000" &&  outmap["MsgCode"]!="0") )
    {
        show.sprintf("ECM上传图像失败,原因[%s]!\t",outmap["MsgInfo"].c_str());
        MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return -1;
    }

    txtFnameList->Clear();
    imgList.clear();
    btnStart->Caption = "开  始 &A";
    btnStop->Enabled = false;
    btnCancel->Enabled = false;
    statusBar->Panels->Items[1]->Text = "等待扫描...";
    btnStart->Default = true;
    return 0; 
}

int TFormReplace::setReplaceOld()
{
    if(FormImageID->ShowModal()!=mrOk) return 0;

    int size = 0;

    XmlMap gloabXml;
    String show;

    image_info_t   oneimage;
    FileVector t;
    distFileList node;
    char fname[80];
    char bname[80];

    IMAGELIST::iterator  pos= imgList.begin();
    while( pos != imgList.end())
    {
        memset(fname,0,sizeof(fname));
        memset(bname,0,sizeof(bname));
        strcpy(fname,pos->fname);

        memset(&node,0,sizeof(node));
        strcpy(node.fileName,fname);
        sprintf(node.fileFullName,"%s/%s",curdir,fname);
        t.push_back(node);  

        memset(bname,0,sizeof(bname));
        strcpy(bname,fname);

        bname[ strlen(fname) -5] = 'B';

        memset(&node,0,sizeof(node));
        strcpy(node.fileName,bname);
        sprintf(node.fileFullName,"%s/%s",curdir,bname);
        t.push_back(node);
        size = size + ZGetFileSize(pos->fname)+ZGetFileSize(bname);
        ++pos;
	}

    gloabXml["TotalSize"] = size;
    ZDate dt;
    dt.Now();
    String dd = dt.BankDateString().c_str();
    String hh = dt.TimeString().c_str();
    hh.Delete(6,1);
    hh.Delete(3,1);
    gloabXml["TradeDate"] = dd.c_str();
    gloabXml["TradeTime"] = hh.c_str();

    gloabXml["ESBReqTimestamp"] = (dd+hh).c_str();

    gloabXml["EventID"] = (dd+hh+g_sys_opt+"UPD001").c_str();
    gloabXml["CaseID"]  = (dd+hh+g_sys_opt+"UPD001").c_str();
    gloabXml["SubBankNo"] = FmIC->gbrno;
    gloabXml["TellerNo"] = g_sys_opt;
    gloabXml["DocumentID"] = FormImageID->txtImageID->Text;

    String xmlstr = makeUpdateXml(gloabXml,t);

    char Results[8192];
    memset(Results,0,sizeof(Results));
    MemT xmlm;
    xmlm.assign((unsigned char*)xmlstr.c_str(),xmlstr.Length());   

    Screen->Cursor = crHourGlass;

    WriteToLogFile(g_sys_opt.c_str(),"隔场影像替换",0,xmlstr.c_str());
    int ret = myECM(xmlm, "127.0.0.1",30003,Results);
    Screen->Cursor = crDefault;

    if(ret)
    {
        show.sprintf("ECM上传图像失败[%d]!\t",ret);
        MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }

    XmlMap  outmap;

    xmlm.assign((unsigned char*)Results,strlen(Results));     

    WriteToLogFile(g_sys_opt.c_str(),"隔场影像替换",0,Results);
    outmap = parseOutXml(Results);
    if( (outmap["MsgCode"]!="000000" &&  outmap["MsgCode"]!="0") )
    {
        show.sprintf("ECM上传图像失败,原因[%s]!\t",outmap["MsgInfo"].c_str());
        MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        return -1;
    }

    txtFnameList->Clear();
    imgList.clear();
    btnStart->Caption = "开  始 &A";
    btnStop->Enabled = false;
    btnCancel->Enabled = false;
    statusBar->Panels->Items[1]->Text = "等待扫描...";
    btnStart->Default = true;

    String msg = AnsiString("影像替换成功,影像ID[") + FormImageID->txtImageID->Text+"]";
    MessageBox(Handle,msg.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
    
    return 0;
}

int TFormReplace::setReStartOld()
{
    if(FormBill->ShowModal()!=mrOk) return 0;

    int size = 0;

    XmlMap gloabXml;
    String show;

    image_info_t   oneimage;
    FileVector t;
    distFileList node;
    char fname[80];
    char bname[80];
    char session[16];

    memset(session,0,sizeof(session));

    IMAGELIST::iterator  pos= imgList.begin();
    while( pos != imgList.end())
    {
        memset(fname,0,sizeof(fname));
        memset(bname,0,sizeof(bname));
        strcpy(fname,pos->fname);

        memset(&node,0,sizeof(node));
        strcpy(node.fileName,fname);
        sprintf(node.fileFullName,"%s/%s",curdir,fname);
        t.push_back(node);  

        memset(bname,0,sizeof(bname));
        strcpy(bname,fname);

        bname[ strlen(fname) -5] = 'B';

        memset(&node,0,sizeof(node));
        strcpy(node.fileName,bname);
        sprintf(node.fileFullName,"%s/%s",curdir,bname);
        t.push_back(node);
        size = size + ZGetFileSize(pos->fname)+ZGetFileSize(bname);
        ++pos;
	}

    gloabXml["TotalSize"] = size;
    ZDate dt;
    dt.Now();
    String dd = dt.BankDateString().c_str();
    String hh = dt.TimeString().c_str();
    hh.Delete(6,1);
    hh.Delete(3,1);
    gloabXml["TradeDate"] = dd.c_str();
    gloabXml["TradeTime"] = hh.c_str();

    gloabXml["ESBReqTimestamp"] = (dd+hh).c_str();

    gloabXml["EventID"] = (dd+hh+g_sys_opt+"UPD001").c_str();
    gloabXml["CaseID"]  = (dd+hh+g_sys_opt+"UPD001").c_str();
    gloabXml["SubBankNo"] = FmIC->gbrno;
    gloabXml["TellerNo"] = g_sys_opt;

    gloabXml["AccountDate"]     =  FormBill->txtDate->Date.FormatString("yyyymmdd");

    sprintf(session,"%03d",FormBill->txtSession->Text.ToInt());
    gloabXml["ExchangeScene"]   =  session;

    memset(session,0,sizeof(session));
    strcpy(session,"000000000000");
    strncpy(session+ 12 - FormBill->txtExchno->Text.Trim().Length(),FormBill->txtExchno->Text.c_str(),FormBill->txtExchno->Text.Trim().Length());

    gloabXml["ExchangeNo"] = session;    

    gloabXml["AttachType"] = "1";
    gloabXml["ExchangeArea"] = g_sys_area;

    int ret = FmIC->gdb.VQuery(512,"select banktype from vchtypes where vchtype='%s' and area='%s'",FormBill->txtVchtype->Text.c_str(),g_sys_area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    if(row == NULL  )
        gloabXml["BillType"] = "000";
    else
        gloabXml["BillType"] = row[0];

    String xmlstr = makeXml(gloabXml,t);

    char Results[8192];
    memset(Results,0,sizeof(Results));
    MemT xmlm;
    xmlm.assign((unsigned char*)xmlstr.c_str(),xmlstr.Length());   


    WriteToLogFile(g_sys_opt.c_str(),"流程重启",0,xmlstr.c_str());

    Screen->Cursor = crHourGlass;
    ret = myECM(xmlm, "127.0.0.1",30003,Results);
    WriteToLogFile(g_sys_opt.c_str(),"流程重启",0,Results);

    if(ret)
    {
        show.sprintf("ECM上传图像失败[%d]!\t",ret);
        MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        Screen->Cursor = crDefault;
        return ret;
    }

    XmlMap  outmap;

    xmlm.assign((unsigned char*)Results,strlen(Results));

    outmap = parseOutXml(Results);
    if( (outmap["MsgCode"]!="000000" &&  outmap["MsgCode"]!="0") )
    {
        show.sprintf("ECM上传图像失败,原因[%s]!\t",outmap["MsgInfo"].c_str());
        MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        Screen->Cursor = crDefault;
        return -1;
    }  

    char ImageID[64];

    char    cmd[8];
    char    cmdstr[512];
    int     result;

    memset(ImageID,0,sizeof(ImageID));
    memset(cmd,0,sizeof(cmd));
    memset(cmdstr,0,sizeof(cmdstr));
                                      
    strcpy(ImageID,outmap["DocumentID"].c_str());     

    distTaskT  disttask;

    memset(&disttask,0,sizeof(disttask));

    strcpy(disttask.imageid,ImageID);
    strcpy(disttask.brno,FmIC->gbrno);
    strcpy(disttask.accdate,FormBill->txtDate->Date.FormatString("yyyymmdd").c_str());
    strcpy(disttask.areano,g_sys_area.c_str());
    strcpy(disttask.exchno,FormBill->txtExchno->Text.c_str());
    strcpy(disttask.session, FormBill->txtSession->Text.c_str());

    strcpy(disttask.imgtype, gloabXml["BillType"].c_str());

    strcpy(disttask.ccycode,"CNY");

    String msgcode;
    if( FormBill->cbMsgType->ItemIndex == 0 )         //451601
        msgcode = FmIC->gmsgtype;
    else
        msgcode =  FmIC->gmsgtypeex;
	String headstr = makeHead((char*)msgcode.c_str());

    String taskstr ;


    String modeType =  FormBill->chkMode->Checked?"N":"O";

    String amt =  RemoveThousandChar(FormBill->txtAmount->Text.c_str());

    int posd = amt.Pos(".");
    if(posd>0)
    {
        amt = amt.Delete(posd,1);
    }
    
    if( msgcode == ZM_SCAN_TASK)
        taskstr = makeTaskBody(disttask,modeType);
    else
        taskstr = makeTaskBodyEx(disttask,FormBill->txtAccno->Text.c_str(),FormBill->txtVchno->Text.c_str(), amt,modeType);

	String send = headstr+taskstr;
    memset(cmd,0,sizeof(cmd));
    memset(cmdstr,0,sizeof(cmdstr));

    WriteToLogFile(g_sys_opt.c_str(),"流程重启",0,send.c_str());

    //流程交易
    ret = doTrade(send.c_str(), FmIC->gflowip,FmIC->gflowport,result,cmd,cmdstr);
    if(ret)
    {
        show.sprintf("启动流程任务失败[%d]!\t",ret);
        MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        Screen->Cursor = crDefault;
        return ret;
    }

    if(result!=0)
    {
        if(strncmp(cmdstr,"SCD044516030001该影像处理流程已启动",35)!=0 )
        {
            show.sprintf("启动流程任务失败[%s][%s]!\t",cmd,cmdstr);
            MessageBox(Handle,show.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
            Screen->Cursor = crDefault;
            return -1;
       	}
    }

    Screen->Cursor = crDefault;
                                             
    txtFnameList->Clear();
    imgList.clear();
    btnStart->Caption = "开  始 &A";
    btnStop->Enabled = false;
    btnCancel->Enabled = false;
    statusBar->Panels->Items[1]->Text = "等待扫描...";
    btnStart->Default = true;
    String msg = AnsiString("流程重启成功,影像ID[") + ImageID+"]";
    MessageBox(Handle,msg.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
    return 0;
}


int TFormReplace::setInsert()
{
    //TODO: Add your source code here
    int ret = 0;

    IMAGELIST::iterator  pos= imgList.begin();
    int mergestate = 0;
    int checkstate = 0;

    if(_ckst ==1 )
    {
        mergestate = 1 ;
        checkstate = 1;
    }
    while( pos != imgList.end())
    {
        ret = FmIC->gdb.VQuery(512,"insert into  authinput(fname,bname,ip,rotate,exchno,pkgno,date,session,dpi,scantime,timestamp,multiflag,mergestate,area,envid,prestate,mergestate,checkstate) values ('%s','%s','%s',%d,'%s','%s','%s','%s',%d,now(),now(),%d,%d,'%s',%d,1,%d,%d)", pos->fname,pos->bname,pos->ip,pos->rotate,pos->pkgno ,_exchno,pos->date,pos->session,pos->dpi,_multiflag,0,g_sys_area.c_str(),_envid,mergestate,checkstate);
        if(ret)
        {
            MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
            return ret;
        }
        ++pos;
	} 
   
    txtFnameList->Clear();
    imgList.clear();
    btnStart->Caption = "开  始 &A";
    btnStop->Enabled = false;
    btnCancel->Enabled = false;
    statusBar->Panels->Items[1]->Text = "等待扫描...";
    btnStart->Default = true;
    return 0;
    
}
void __fastcall TFormReplace::btnStopClick(TObject *Sender)
{
   SetCurrentDirectory(FmIC->gPath.c_str());
    if(_type == 0 )
    {
        setReplace();
    }
    else  if(_type == 1 )
    {
        setInsert();
    }
    else  if(_type == 2 )
    {
    	
    		if(!FmIC->m_bReplace)
        {
    			MessageBox(Handle,"该柜员没有影像替换权限,不能进行影像替换!\t","提示",MB_OK|MB_ICONINFORMATION);
        	return;	
    		}
    	/*
        MessageBox(Handle,"隔场影像替换需要授权!\t","提示",MB_OK|MB_ICONINFORMATION);
        if( FormWarrant->ShowModal() == mrCancel)
        {
            MessageBox(Handle,"授权失败,不能进行影像替换!\t","提示",MB_OK|MB_ICONINFORMATION);
            return;
        }
       */
        setReplaceOld();
    }

    else  if(_type == 3 )
    {
    	 if(!FmIC->m_bReStart)
    		{
    			MessageBox(Handle,"该柜员没有流程重启权限,不能重新启动流程!\t","提示",MB_OK|MB_ICONINFORMATION);
        	return;	
    		}
    		/*
        MessageBox(Handle,"隔场流程重启需要授权!\t","提示",MB_OK|MB_ICONINFORMATION);
        if( FormWarrant->ShowModal() == mrCancel)
        {
            MessageBox(Handle,"授权失败,不能进行流程重启!\t","提示",MB_OK|MB_ICONINFORMATION);
            return;
        }
        */
        setReStartOld();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::muSysSetupClick(TObject *Sender)
{
    btnStartClick(NULL);    
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    SetCurrentDirectory(FmIC->gPath.c_str());
    debugout("set dir");
}
//---------------------------------------------------------------------------

