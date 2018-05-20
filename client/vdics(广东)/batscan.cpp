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
#include <io.h>
#include <myplus.h>
#include <ztime.hpp>
#include <zcommon.h>
#include <transimg.h>
#include <TC_ImageLib.h>
#include <jpeg.hpp>

#include "main.h"
#include "batscan.h"
#include "waitdlg.h"

#include "setupdlg.h"
#include "selNet.h"
#include "pubfun.h"
#include "login.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormScan *FormScan;


void Bitmap2Jpeg(AnsiString bmpFile, AnsiString jpegFile)
{
	Graphics::TBitmap *bitmap = new Graphics::TBitmap();
	bitmap->LoadFromFile(bmpFile);
	TJPEGImage *jpeg = new TJPEGImage();
	try {
		jpeg->Assign(bitmap);
		jpeg->SaveToFile(jpegFile);
	}
	__finally {
		delete jpeg;
	}
	delete bitmap;
}

void Jpeg2Bitmap(AnsiString jpegFile, AnsiString bmpFile)
{
	Graphics::TBitmap *bmp = new Graphics::TBitmap();
	TJPEGImage *jpeg = new TJPEGImage();
	jpeg->LoadFromFile(jpegFile);
	bmp->Assign(jpeg);
	bmp->SaveToFile(bmpFile);
	delete jpeg;
	delete bmp;
}
//---------------------------------------------------------------------------
__fastcall TFormScan::TFormScan(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormScan::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------


int TFormScan::GetLocalHost()
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
void __fastcall TFormScan::muSysSetupClick(TObject *Sender)
{
    String stmp;

    FmSetup = new TFmSetup(this);

    FmSetup->txtScannerName->Text = m_Ini.m_scannerInfo.scanner_name;

    stmp.printf("%d",m_Ini.m_scannerInfo.scanner_sn);
    FmSetup->txtSerial->Text=stmp;

    stmp.printf("%8.6f",m_Ini.m_scannerInfo.calx) ;
    FmSetup->txtCalX->Text = stmp;
    stmp.printf("%8.6f",m_Ini.m_scannerInfo.caly);
    FmSetup->txtCalY->Text = stmp;

    FmSetup->txtPath->Text = m_Ini.m_scannerInfo.path;

    FmSetup->txtPrefix->Text=m_Ini.m_scannerInfo.prefix;
    
    FmSetup->txtOffSet->Text=m_Ini.m_scannerInfo.offset;

    if(m_Ini.m_scannerInfo.pkgflag==0)     FmSetup->pkgflag->Checked=false;
    else                                   FmSetup->pkgflag->Checked=true;

    if(m_Ini.m_scannerInfo.needRotate==0)  FmSetup->ishardrotate->Checked=false;
    else                                   FmSetup->ishardrotate->Checked=true;

    vch_type_t   onevch;
    VCHTYPELIST::iterator  pos= m_Ini.pList->myList.begin();
    FmSetup->VchDetailList->Items->Clear();
    TListItem* li;
    while( pos != m_Ini.pList->myList.end())
	{
        onevch= *pos;
        li =  FmSetup->VchDetailList->Items->Add();
		li->Caption=pos->vchname;
        stmp.printf("%5.2f x %5.2f",pos->vch_sizex,pos->vch_sizey);
        li->SubItems->Insert(0,stmp.c_str()) ;
        li->Data =(void*)pos->vchname;

        ++pos;
	}

    try
    {
        if( FmSetup->ShowModal() != mrOk )
        {
            delete FmSetup; FmSetup=0;
            return;
        }

        m_Ini.m_scannerInfo.scanner_sn=FmSetup->txtSerial->Text.ToInt();
        //扫描仪名称
        strcpy(m_Ini.m_scannerInfo.scanner_name,FmSetup->txtScannerName->Text.c_str());
        //扫描仪标号
        strcpy(m_Ini.m_scannerInfo.prefix,FmSetup->txtPrefix->Text.c_str());
        //图像保存路径
        strcpy(m_Ini.m_scannerInfo.path,FmSetup->txtPath->Text.c_str());
        //是否按交换号扫描
        if(FmSetup->pkgflag->Checked==true)
                m_Ini.m_scannerInfo.pkgflag=1;
        else
                m_Ini.m_scannerInfo.pkgflag=0;
        if(FmSetup->ishardrotate->Checked==true)
                m_Ini.m_scannerInfo.needRotate=1;
        else
                m_Ini.m_scannerInfo.needRotate=0;
        //基准偏移
        m_Ini.m_scannerInfo.offset=FmSetup->txtOffSet->Text.ToInt();
        //X方向校准值
        m_Ini.m_scannerInfo.calx=FmSetup->txtCalX->Text.ToDouble();
        //Y方向校准值
        m_Ini.m_scannerInfo.caly=FmSetup->txtCalY->Text.ToDouble();

        int ret=m_Ini.SaveIniData();//保存
        if(ret)
        {
            MessageBox(Handle,"图像参数设置失败!\t","提示",MB_OK|MB_ICONINFORMATION);
            return;
        }
        if( access(m_Ini.m_scannerInfo.path,0) )
        {
    	    MessageBox(Handle,"图像文件保存路径不存在!\t","错误",MB_OK+MB_ICONERROR);
            return;  
        }
    }
    catch(...)
    {
        MessageBox(Handle,"图像参数设置失败设置无效!\t","错误",MB_OK+MB_ICONERROR);
        return ;
    }

    MessageBox(Handle,"扫描仪参数已更改!\t","警告",MB_OK+MB_ICONWARNING);
    SetCurrentDirectory(m_Ini.m_scannerInfo.path);
}
//---------------------------------------------------------------------------
void __fastcall TFormScan::btnStartClick(TObject *Sender)
{

     __int64 FreeDisk = DiskFree(0);
     float FreeGB=FreeDisk/(1024.00*1024.00*1024.00);

     if(FreeGB<0.3)
     {
        MessageBox(Handle,"硬盘空间严重不足，请清理后再扫描!\t","警告", MB_OK);
        return ;
     }

//--------------------- Begin -----------------------------
//       if(/*m_bIsOpen == TRUE ||*/ m_bIsOpen == FALSE)
/*        {
        int bScan = scanner.SelectScanner();
        if(0 == bScan)
        {
        }else if(1 == bScan)
        {
                return ;
        }else
        {
       //         MessageBox(Handle,"扫描仪选择错误!\t","警告", MB_OK);
    //            return ;
        }

        if(FALSE == scanner.GetDSCap())
        {
                MessageBox(Handle,"扫描仪打开错误!\t","警告", MB_OK);
                return ;
        }
        m_bIsOpen = TRUE;    //new

        }
//--------------------- End --------------------------------
 */
     if(FmIC->gmode!=0)
     {
        showDS();
     }
     else
     {
        memset(date,0,sizeof(date));
        memset(session,0,sizeof(session));
        strcpy(date,txtDate->Date.FormatString("yyyymmdd").c_str());
        strcpy(session,txtSession->Text.c_str());
     }
     scanner.bShowUI = (chbShow->Checked==true)?true:false;
     
     statusBar->Panels->Items[2]->Text="";
     statusBar->Panels->Items[3]->Text="";

     int ret;
     String vchname;
     memset(&curVchType,0,sizeof(curVchType));

     if(m_Ini.m_scannerInfo.pkgflag==0)
     {
        FmSelNet->Caption="扫描交换号选择";
        FmSelNet->Label2->Caption="待扫描交换号";
     }
     else
     {
        FmSelNet->Caption="扫描交换号选择";
        FmSelNet->Label2->Caption="待扫描交换号";
     }

     if( btnStart->Caption == "开  始 &A")
     {
        _vchFlag = false;

        _newModeFlag = false;

        if( FmSelNet->ShowModal() != mrOk ) return;

        _vchFlag        = FmSelNet->cbVchtype->Checked;
        _newModeFlag    = FmSelNet->cbNewMode->Checked;

        if(_vchFlag == true)
            _vchType =  FmSelNet->_vtMap[FmSelNet->cbType->Text.c_str()].c_str();
        else
            _vchType = "";
        imgList.clear();
        txtFnameList->Clear();
        CurPkgNo=FmSelNet->txtPkgNo->Text.Trim().c_str();
		
        vchname=FmSelNet->txtVchType->Text;
        single = FmSelNet->chbSingle->Checked?0:1;

        Area =  FmSelNet->txtArea->Text.Trim().c_str();

        if( FmIC->gclear == 1)
        {
            String ckstr ;

            char    cmd[8];
            char    cmdstr[512];
            int     result;

            memset(cmd,0,sizeof(cmd));
            memset(cmdstr,0,sizeof(cmdstr));

            ckstr = makeQueryDs(FmIC->m_sOrgCode.c_str(),FmIC->m_sOptCode.c_str(),FmIC->gbrno,txtDate->Date.FormatString("yyyymmdd").c_str(),txtSession->Text.c_str(),Area.c_str(),FmSelNet->txtPkgNo->Text.c_str());

			debugout("\n\nCkStr = %s\n\n",ckstr.c_str());

            ret = doTrade(ckstr.c_str(), FmIC->gflowip,FmIC->gflowport,result,cmd,cmdstr);
            if(ret)
            {
                ckstr.sprintf("清算信息检查失败[%d]!\t",ret);
                MessageBox(Handle,ckstr.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
                Screen->Cursor = crDefault;
                return ;
            }

            if(result!=0)
            {
                ckstr.sprintf("清算信息检查失败[%s][%s]!\t",cmd,cmdstr);
                MessageBox(Handle,ckstr.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
                Screen->Cursor = crDefault;
                return ;
            }
            else
            {
                if(strncmp(cmdstr,"46Z010",6)!=0 )
                {
                    ckstr.sprintf("清算信息检查失败[%s][%s]!\t",cmd,cmdstr);
                    MessageBox(Handle,ckstr.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
                    Screen->Cursor = crDefault;
                    return ;
       	        }
            }
        }

        VCHTYPELIST::iterator  pos= m_Ini.pList->myList.begin();
        while( pos != m_Ini.pList->myList.end())
        {
            if(strcmp(pos->vchname,vchname.c_str())==0) break;
            ++pos;
        }
        m_Ini.m_scannerInfo.onevch=*pos;
        _scanType = pos->scantype;

        _hardRotate = pos->rotate;
        _imageFormat= pos->format;
        scanner.SetData(&m_Ini.m_scannerInfo);

        scancount=0;

        memset(curdir,0,sizeof(curdir));
        SetCurrentDirectory(m_Ini.m_scannerInfo.path);

        sprintf(curdir,"%s\\%s",m_Ini.m_scannerInfo.path,date);
        if(_access(curdir,0)!=0)
        {
            _mkdir(curdir);
        }

        sprintf(curdir,"%s\\%s\\%02d",m_Ini.m_scannerInfo.path,date,atoi(session));
        if(_access(curdir,0)!=0)
        {
            _mkdir(curdir);
        }

        sprintf(curdir,"%s\\%s\\%02d",m_Ini.m_scannerInfo.path,date,atoi(session));
        if(_access(curdir,0)!=0)
        {
            _mkdir(curdir);
        }

        String kn;

 //-----------------------------------------------------------------------------//20150511
        if(CurPkgNo.Length() > 6 && CurPkgNo.Length() <=12)
        {
   //             int index = CurPkgNo.Length();
   //             index -= 6;
   //             CurPkgNo = CurPkgNo.SubString(index,6);
                ret = FmIC->gdb.VQuery(512,"select exchno from bocnets where pkgno='%s' and area='%s'",CurPkgNo.c_str(),Area.c_str());
                MyRes  res = FmIC->gdb.Store();
                MyRow  row = res.Fetch();

                CurPkgNo = row[0];

       //         ShowMessage(CurPkgNo);
        }
 //-----------------------------------------------------------------------------//20150511

        ret = FmIC->gdb.VQuery(512,"select pkgno  from bocnets where exchno='%s' and area='%s'",CurPkgNo.c_str(),Area.c_str());
        if(ret)
        {
            MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            return ;
        }

        MyRes  res = FmIC->gdb.Store();
        MyRow  row = res.Fetch();

        String pkgNo = row[0];
        if(pkgNo.Length()<5) pkgNo = CurPkgNo;


        if(pkgNo.Length()>5)
            kn = pkgNo.c_str()+ pkgNo.Length() - 5;
            
        sprintf(curdir,"%s\\%s\\%02d\\%05d",m_Ini.m_scannerInfo.path,date,atoi(session),kn.ToInt());
        if(_access(curdir,0)!=0)
        {
            _mkdir(curdir);
        }

       // SetCurrentDirectory(m_Ini.m_scannerInfo.path);
        
     }

	SetCurrentDirectory(curdir);

    scanner.ResetScanner();

    ZDateTime zdate;
    zdate.Now();
    long ct = zdate.GetAnsiVal()%10000000;

    statusBar->Panels->Items[1]->Text = String("正在扫描[")+CurPkgNo+"]";

    statusBar->Panels->Items[2]->Text = String("扫描类型[")+FmSelNet->txtVchType->Text.c_str()+"]";
    btnStop->Enabled = false;
    btnStart->Enabled = false;
    btnCancel->Enabled = true;

    ret=scanner.SetPkgno(CurPkgNo.c_str(),ct);
    if( btnStart->Caption == "开  始 &A")      Application->ProcessMessages();

     try
     {
        Screen->Cursor = crHourGlass;
        scanner.Scan(0);
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
     msg = "[" + CurPkgNo + "] 共["+scancount+"]张是否完成？";
     if( MessageBox( Handle, msg.c_str(),"询问",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1) == IDNO )
     {
            btnStartClick(0);
     }

     statusBar->Panels->Items[2]->Text = String("[")+CurPkgNo+"]"+"扫描结束";
     statusBar->Panels->Items[3]->Text = String("本批次总共扫描张数[")+scancount+"]";
}
//---------------------------------------------------------------------------   

int TFormScan::insertFname(const char* fname,int rotate,int fcount)
{
    //TODO: Add your source code here
    TListItem* li;;
    li = txtFnameList->Items->Add();
    li->Caption = fname;

    char bname[80];
    memset(bname,0,sizeof(bname));
    strcpy(bname,fname);
    String t = fname;

	debugout("fname = %s",fname);
	debugout("bname =%s",bname);

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
    strcpy(Node.pkgno,CurPkgNo.c_str());

    strcpy(Node.date,date);
    strcpy(Node.session,session);
    strcpy(Node.ip,m_ipaddress);
    Node.procA  = m_Ini.m_scannerInfo.onevch.procA;

    Node.dpi  = m_Ini.m_scannerInfo.onevch.dpi;

    if(_hardRotate == false )
        Node.rotate = m_Ini.m_scannerInfo.onevch.rotate*90;
    else
        Node.rotate = 0;
    Node.fcount = fcount;

    imgList.push_back(Node);


    return 0;
}
void __fastcall TFormScan::FormShow(TObject *Sender)
{
    int ret ;
    FmWait = new TFmWait(Application);

    FmWait->Label1->Caption = "正在初始化扫描设备，请稍后 ...";
    
    FmWait->Show();
    Application->ProcessMessages();

    StartDir = GetCurrentDir();

	strcpy(m_Ini.m_iniPath,StartDir.c_str());
	strcat(m_Ini.m_iniPath,"\\config");

	debugout("Path = %s,StarDit =%s",m_Ini.m_iniPath,StartDir.c_str());

    if( access(m_Ini.m_iniPath,0) )
    {
    	MessageBox(Handle,"读取配置文件失败!\t","错误",MB_OK+MB_ICONERROR);
        btnStart->Enabled = false;
        IniLoaded = false;
        return ;
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

    m_bIsOpen = FALSE;

    m_bIsOpen = scanner.OpenScanner(Handle);

    if( m_bIsOpen == FALSE )
    {
      //  btnStart->Enabled = false;
        FmWait->Hide();
        delete FmWait;
        FmWait = NULL;
        return ;
    }   

    if( access(m_Ini.m_scannerInfo.path,0) )
    {
    	MessageBox(Handle,"图像文件保存路径不存在!\t","错误",MB_OK+MB_ICONERROR);
        btnStart->Enabled = false;
        return ;
    }             
    
    btnStart->Enabled = true;
	Rotate->Enabled = false;
    SetCurrentDirectory(m_Ini.m_scannerInfo.path);    

    ret=GetLocalHost();
    if(ret)
    {
        MessageBox(Handle,"读取本机网络地址出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        btnStart->Enabled = false;
        return ;
    }

    debugout("m_ipaddress = %s",m_ipaddress);

    if(strlen(m_ipaddress) < 7)
    {
        MessageBox(Handle,"读取本机网络地址出错01!\t","提示",MB_OK|MB_ICONINFORMATION);
        btnStart->Enabled = false;
        return ;
    }
       
    FmWait->Hide();
    delete FmWait; FmWait = NULL;

    showDS();
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
void __fastcall TFormScan::imgFMouseUp(TObject *Sender,
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

void __fastcall TFormScan::imgBMouseUp(TObject *Sender,
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
void __fastcall TFormScan::txtFnameListSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
    Rotate->Enabled = true;
    imgF->AutoSize = false;
    imgF->Align = alClient;
    imgF->Proportional = true;
    imgF->Stretch = true;
    imgF->Tag = 0;

    imgB->AutoSize = false;
    imgB->Align = alClient;
    imgB->Proportional = true;
    imgB->Stretch = true;
    imgB->Tag = 0;

    String fname;
    TListItem* li;
    li= txtFnameList->Selected;
    if(li==NULL) return ;
    fname = li->Caption ;

    char bname[80];
    memset(bname,0,sizeof(bname));
    strcpy(bname,fname.c_str());

    bname[fname.Length() -5] = 'B';


	IMAGELIST::iterator  pos= imgList.begin();

	while( pos != imgList.end())
	{
		if(strcmp(pos->fname,fname.c_str()) == 0)
		{
			//bname.sprintf("%s",pos->bname);
			break;
		}
		++pos;
	}


	char tfname[128];
	memset(tfname,0,sizeof(tfname));
	sprintf(tfname,"%s\\%s\\%02d\\%05d\\%s",m_Ini.m_scannerInfo.path,FormScan->date,atoi(FormScan->session),atoi(pos->pkgno)%100000,fname);

	debugout("tfname = %s",tfname);

	char tBname[128];
	memset(tBname,0,sizeof(tBname));
	sprintf(tBname,"%s\\%s\\%02d\\%05d\\%s",m_Ini.m_scannerInfo.path,FormScan->date,atoi(FormScan->session),atoi(pos->pkgno)%100000,bname);

	debugout("tname = %s",tBname);



    if(_imageFormat == 2 )
    {
        imgF->Picture->LoadFromFile(tfname);
        imgB->Picture->LoadFromFile(tBname);
    }
    else
    {
        TJPEGImage *ps = new TJPEGImage();
        ps->LoadFromFile(tfname);
        imgF->Picture->Bitmap->Assign(ps);
        delete ps;

        TJPEGImage *psb = new TJPEGImage();
        psb->LoadFromFile(tBname);
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


void __fastcall TFormScan::FormDestroy(TObject *Sender)
{
    scanner.CloseScanner();
}
//---------------------------------------------------------------------------

void __fastcall TFormScan::btnCancelClick(TObject *Sender)
{
    txtFnameList->Clear();
    imgList.clear();
    btnStart->Caption = "开  始 &A";
    btnStop->Enabled = false;
    btnCancel->Enabled = false;
    statusBar->Panels->Items[1]->Text = "等待扫描...";

    int ret ;
    if(_scanType == 0 )
    {
        ret = FmIC->gdb.VQuery(512,"select count(*) from vouchers  where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
        if(ret)
        {
            MessageBox(Handle,"查询数据出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            return ;
        }
        MyRes res = FmIC->gdb.Store();
        MyRow row = res.Fetch();
        if( atoi(row[0]) == 0 )
            FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,2),'0',right(state,13)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
        else
            FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,2),'1',right(state,13)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
    }
    else if(_scanType == 1 )
    {
        ret = FmIC->gdb.VQuery(512,"select count(*) from multis  where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
        if(ret)
        {
            MessageBox(Handle,"查询数据出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            return ;
        }
        MyRes res = FmIC->gdb.Store();
        MyRow row = res.Fetch();
        if( atoi(row[0]) == 0 )
            FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,3),'0',right(state,12)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
        else
            FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,3),'1',right(state,12)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
     }
    else if(_scanType == 2 )
    {
        ret = FmIC->gdb.VQuery(512,"select count(*) from authinput  where multiflag=1 and exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
        if(ret)
        {
            MessageBox(Handle,"查询数据出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            return ;
        }
        MyRes res = FmIC->gdb.Store();
        MyRow row = res.Fetch();
        if( atoi(row[0]) == 0 )
            FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,4),'0',right(state,11)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
        else
            FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,4),'1',right(state,11)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
    }
    else
    {
        ret = FmIC->gdb.VQuery(512,"select count(*) from authinput  where multiflag=3  and exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
        if(ret)
        {
            MessageBox(Handle,"查询数据出错!\t","提示",MB_OK|MB_ICONINFORMATION);
            return ;
        }
        MyRes res = FmIC->gdb.Store();
        MyRow row = res.Fetch();
        if( atoi(row[0]) == 0 )
            FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,5),'0',right(state,10)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
        else
            FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,5),'0',right(state,10)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str(),date,session);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormScan::btnStopClick(TObject *Sender)
{
    int ret = saveToDb();
    if(ret)
    {
        MessageBox(Handle,"保存数据出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    txtFnameList->Clear();
    imgList.clear();
    btnStart->Caption = "开  始 &A";
    btnStop->Enabled = false;
    btnCancel->Enabled = false;
    statusBar->Panels->Items[1]->Text = "等待扫描...";
    btnStart->Default = true;


}
//---------------------------------------------------------------------------

void __fastcall TFormScan::F1Click(TObject *Sender)
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


int TFormScan::saveToDb()
{
    //TODO: Add your source code here
    image_info_t   oneimage;
    IMAGELIST::iterator  pos= imgList.begin();

    String sql;

    String szMoney;

    int ret;

    ret = FmIC->gdb.VQuery(512,"select pkgno ,merge,compare from bocnets where exchno='%s' and area='%s'",CurPkgNo.c_str(),Area.c_str());
    if(ret)
    {          
        MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }

    MyRes  res = FmIC->gdb.Store();
    MyRow  row = res.Fetch();

    char   pk[12];
    memset(pk,0,sizeof(pk));

    if(row!=NULL) strcpy(pk,row[0]);
    else          strcpy(pk,CurPkgNo.c_str());

    int merge,compare,mt,state,newmode;


    newmode = 0 ;
                                       
    merge = atoi(row[1]);
    compare = atoi(row[2]);

    if(RadioButton1->Checked == true)
    {
        szMoney = "CNY";
    }else if(RadioButton2->Checked == true)
    {
        szMoney = "USD";
    }else
    {
        szMoney = "CNY";
    }

    if( merge == 0 )
        mt = 1;
    else
    {
        mt = 0;
    }

    if(_newModeFlag == true )
        newmode = 1;
        
    if(_vchFlag == true)
        state = 1;
    else
        state = 0;

    while( pos != imgList.end())
	{
        if(_scanType == 0 )
        {
            sql.sprintf("insert into vouchers(fname,bname,ip,rotate,exchno,pkgno,date,session,dpi,scantime,timestamp,mergestate,area,state,vchtype,modeflag,clkno,orgcode,money) values ('%s','%s','%s',%d,'%s','%s','%s','%s',%d,now(),now(),%d,'%s','%d','%s',%d,'%s','%s','%s')", pos->fname,pos->bname,pos->ip,pos->rotate,pos->pkgno ,pk,pos->date,pos->session,pos->dpi,mt,Area.c_str(),state,_vchType.c_str(),newmode,g_sys_opt.c_str(),g_sys_org.c_str(),szMoney.c_str());
        }
        else if(_scanType == 1 )
        {
            sql.sprintf("insert into  multis(fname,bname,ip,rotate,exchno,pkgno,date,session,dpi,scantime,timestamp,multiflag,mergestate,area,clkno,orgcode,money) values ('%s','%s','%s',%d,'%s','%s','%s','%s',%d,now(),now(),%d,%d,'%s','%s','%s','%s')", pos->fname,pos->bname,pos->ip,pos->rotate,pos->pkgno ,pk,pos->date,pos->session,pos->dpi,single,mt,Area.c_str(),g_sys_opt.c_str(),g_sys_org.c_str(),szMoney.c_str());
        }

        else if(_scanType == 2 )
        {
            sql.sprintf("insert into  authinput(fname,bname,ip,rotate,exchno,pkgno,date,session,dpi,scantime,timestamp,multiflag,mergestate,area,clkno,orgcode) values ('%s','%s','%s',%d,'%s','%s','%s','%s',%d,now(),now(),%d,%d,'%s','%s','%s')", pos->fname,pos->bname,pos->ip,pos->rotate,pos->pkgno ,pk,pos->date,pos->session,pos->dpi,1,mt,Area.c_str(),g_sys_opt.c_str(),g_sys_org.c_str());
            if( FmSetup->pkgflag->Checked == true)
            {
                ret = FmIC->gdb.Query(sql.c_str());
                sql.sprintf("insert into  authinput(fname,bname,ip,rotate,exchno,pkgno,date,session,dpi,scantime,timestamp,multiflag,mergestate,area,clkno,orgcode) values ('%s','%s','%s',%d,'%s','%s','%s','%s',%d,now(),now(),%d,%d,'%s','%s','%s')", pos->fname,pos->bname,pos->ip,pos->rotate,pos->pkgno ,pk,pos->date,pos->session,pos->dpi,2,mt,Area.c_str(),g_sys_opt.c_str(),g_sys_org.c_str());
            }
        }

        else if(_scanType == 3 )
        {
            sql.sprintf("insert into  authinput(fname,bname,ip,rotate,exchno,pkgno,date,session,dpi,scantime,timestamp,multiflag,mergestate,area,clkno,orgcode) values ('%s','%s','%s',%d,'%s','%s','%s','%s',%d,now(),now(),%d,%d,'%s','%s','%s')", pos->fname,pos->bname,pos->ip,pos->rotate,pos->pkgno ,pk,pos->date,pos->session,pos->dpi,3,mt,Area.c_str(),g_sys_opt.c_str(),g_sys_org.c_str());
        }
        ret = FmIC->gdb.Query(sql.c_str());  
        if(ret)
        {
            return ret;
        }    
        ++pos;
	}

    if(_scanType == 0 )
        ret = FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,2),'1',right(state,13)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str());
    else  if(_scanType == 1 )
        ret = FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,3),'1',right(state,12)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str());
    else  if(_scanType == 2 )
        ret = FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,4),'1',right(state,11)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str());
    else  if(_scanType == 3 )
        ret = FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,5),'1',right(state,10)) where exchno='%s' and area='%s' and date='%s' and session='%s'",CurPkgNo.c_str(),Area.c_str());

    if(ret)
    {
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }  
  
    if(FmIC->gmode==0)
    {
        String dd = txtDate->Date.FormatString("yyyymmdd");
        ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('session',%s)",txtSession->Text.c_str());
        if(ret)
	    {
            MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
		    return ret;
	    }

        ret = FmIC->gdb.VQuery(512,"replace into config(name,value) values('date','%s')",dd.c_str());
        if(ret)
	    {
            MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
	    	return ret;
	    }
    }

    ret = FmIC->gdb.VQuery(512,"select count(*) from bocctrl where exchno='%s' and date='%s' and session='%s' and area='%s'",CurPkgNo.c_str(),date,session,Area.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询数据错误\t","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }
    res = FmIC->gdb.Store();
    row = res.Fetch();      
    
    if(atoi(row[0]) == 0 )
    {
        if(_scanType == 0 )
            ret = FmIC->gdb.VQuery(512,"insert into  bocctrl (exchno,date,session,state,area) values('%s','%s','%s','1110000000000000','%s')",CurPkgNo.c_str(),FormScan->date,FormScan->session,Area.c_str());
        else if(_scanType == 1 )
            ret = FmIC->gdb.VQuery(512,"insert into  bocctrl (exchno,date,session,state,area) values('%s','%s','%s','1001000000000000','%s')",CurPkgNo.c_str(),FormScan->date,FormScan->session,Area.c_str());
        else if(_scanType == 2 )
            ret = FmIC->gdb.VQuery(512,"insert into  bocctrl (exchno,date,session,state,area) values('%s','%s','%s','1000100000000000','%s')",CurPkgNo.c_str(),FormScan->date,FormScan->session,Area.c_str());
        else
            ret = FmIC->gdb.VQuery(512,"insert into  bocctrl (exchno,date,session,state,area) values('%s','%s','%s','1000010000000000','%s')",CurPkgNo.c_str(),FormScan->date,FormScan->session,Area.c_str());

        if(ret)
        {
            MessageBox(Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            return ret;
        }
    }
    return 0;
}                     

int TFormScan::showDS()
{
    //TODO: Add your source code here
    memset(date,0,sizeof(date));
    memset(session,0,sizeof(session));

    FmIC->getDS("date",date);
    FmIC->getDS("session",session);

    txtSession->Text=session;
    ZDateTime zdate;
    zdate.SetBankDateStr(date);
    txtDate->Date = StrToDate(zdate.DateString().c_str());
    return 0;
}

void __fastcall TFormScan::muExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------


void __fastcall TFormScan::FormClose(TObject *Sender, TCloseAction &Action)
{
    imgF->AutoSize = false;
    imgF->Align = alClient;
    imgF->Proportional = true;
    imgF->Stretch = true;
    imgF->Tag = 0;

    imgB->AutoSize = false;
    imgB->Align = alClient;
    imgB->Proportional = true;
    imgB->Stretch = true;
    imgB->Tag = 0;

 //   m_bIsOpen = TRUE;    //new

    SetCurrentDirectory(FmIC->gPath.c_str());   
}
//---------------------------------------------------------------------------
void __fastcall TFormScan::RotateClick(TObject *Sender)
{

	String fname;
	TListItem* li;
	li= txtFnameList->Selected;
	if(li==NULL) 
	{
		Rotate->Enabled = false;
		return ;
	}
	fname = li->Caption ;

	String bname;

	debugout("fname = %s",fname);

	image_info_t   oneimage;
	IMAGELIST::iterator  pos= imgList.begin();

	while( pos != imgList.end())
	{
		if(strcmp(pos->fname,fname.c_str()) == 0)
		{
			bname.sprintf("%s",pos->bname);
			break;
		}
		++pos;
	}

	if (bname.Length() <= 0)
	{
		return;
	}

	debugout("bnmae = %s",bname.c_str());

	char tname[128];
	memset(tname,0,sizeof(tname));
	sprintf(tname,"%s\\%s\\%02d\\%05d\\%s",m_Ini.m_scannerInfo.path,FormScan->date,atoi(FormScan->session),atoi(pos->pkgno)%100000,fname);

	debugout("tname = %s",tname);


	MemT image;

	int sizex;
	int sizey;
	int panel;

/*
    int  ret =  ti_getimage(tname,"127.0.0.1" , 0, image, sizex, sizey,panel );
	if(ret)
	{
		MessageBox(Handle,"获取图像错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ;
	}

	debugout("获取正面成功，【%d】【%d】【%d】",sizex,sizey,panel);
	ret = ti_rotateimg( image, sizex, sizey, panel, 90);
	if (ret !=0 )
	{
		MessageBox(Handle,"旋转图像错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ;
	}

	debugout("旋转成功");

	unsigned char * bmp;
	int size;
*/

	int ret = tilRotateImageF2F(tname,tname,90);

	imgF->Picture->LoadFromFile(tname);


	memset(tname,0,sizeof(tname));
	sprintf(tname,"%s\\%s\\%02d\\%05d\\%s",m_Ini.m_scannerInfo.path,FormScan->date,atoi(FormScan->session),atoi(pos->pkgno)%100000,bname);

	debugout("tname = %s",tname);

	ret = tilRotateImageF2F(tname,tname,90);


	//imgF->Picture->LoadFromFile(fname);
	imgB->Picture->LoadFromFile(tname);
	/*

	bmp =ti_RGB2BMP(image.data(),sizex,sizey,0,0,0,0,sizex,sizey,size);

	TMemoryStream* pms = new TMemoryStream();
	pms->Clear();
	pms->SetSize(size);
	ret = pms->Write(bmp,size);
	pms->Position = 0;
	imgF->Picture->Bitmap->LoadFromStream(pms);
	

	memset(tname,0,sizeof(tname));
	sprintf(tname,"C://image//%s//%02d//%05d//%s",FormScan->date,atoi(FormScan->session),atoi(pos->pkgno)%100000,fname);

	debugout("tname = %s",tname);

	pms->SaveToFile(tname);

	Bitmap2Jpeg(tname,tname);

	pms->Clear();

	ti_RGB2BMPFREE(bmp);

	image.clear();

	memset(tname,0,sizeof(tname));
	sprintf(tname,"%s//%02d//%05d//%s",FormScan->date,atoi(FormScan->session),atoi(pos->pkgno)%100000,bname);

	debugout("tname = %s",tname);


	ret =  ti_getimage(tname,"127.0.0.1" , 0, image, sizex, sizey,panel );
	if(ret)
	{
		MessageBox(Handle,"获取图像错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ;
	}

	debugout("获取反面成功，【%d】【%d】【%d】",sizex,sizey,panel);

	ret = ti_rotateimg( image, sizex, sizey, panel, 270);
	if (ret !=0 )
	{
		MessageBox(Handle,"旋转图像错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ;
	}

	debugout("旋转成功");

	memset(tname,0,sizeof(tname));
	sprintf(tname,"C://image//%s//%02d//%05d//%s",FormScan->date,atoi(FormScan->session),atoi(pos->pkgno)%100000,bname);

	debugout("tname = %s",tname);


	bmp =ti_RGB2BMP(image.data(),sizex,sizey,0,0,0,0,sizex,sizey,size);

	pms->Clear();
	pms->SetSize(size);
	ret = pms->Write(bmp,size);
	pms->Position = 0;
	imgB->Picture->Bitmap->LoadFromStream(pms);

	pms->SaveToFile(tname);

	pms->Clear();

	ti_RGB2BMPFREE(bmp);

	Bitmap2Jpeg(tname,tname);
*/

	//imgB->Picture->LoadFromFile(tname);

}
//---------------------------------------------------------------------------

void __fastcall TFormScan::N1Click(TObject *Sender)
{

        int bScan = scanner.SelectScanner();
        if(0 == bScan)
        {
        }else if(1 == bScan)
        {
                FmWait->Hide();
                return ;
        }else
        {
                return ;
        }

        MessageBox(Handle,"选择扫描仪成功，请重启程序!\t","提示",MB_OK|MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

