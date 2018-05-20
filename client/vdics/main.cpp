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
#include <zcommon.h>

#include "main.h"
#include "batscan.h"
#include "pbcdata.h"
#include "single.h"
#include "more.h"
#include "clerks.h"
#include "login.h"
#include "bocs.h"
#include "vchtype.h"
#include "Password.h"
#include "configs.h"
#include "upload.h"
#include "bjpbc.h"
#include "datesess.h"
#include "collection.h"
#include "replacescan.h"
#include "waitdlg.h"
#include "image.h"
#include <zcommon.h>
#include <transimg.h>
#include "state.h"
#include "warrant.h"
#include "undo.h"
#include "distData.h"
#include "statics.h"
#include "ScannerIni.h"
#include <string.h>
#include <io.h>
#include "pubfun.h"
#include "tinystr.h"
#include "tinyxml.h"

#include <zsocket.hpp>
#include <ztime.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
String HOSTS_FILE;//  "C:\\WINNT\\system32\\drivers\\etc\\Hosts"

String m_XmlMsg_In;


TFmIC *FmIC;

int ParsePriv(char *val)
{
        int priv = 0;
        int len = strlen(val);
        if(len < 11)
        {
                debugout("接收数据错误！");
                return -1;
        }
    /*    if(*(val+0)!='1' || *(val+1)!='9')
        {
                debugout("接收数据操作符不匹配！");
                return -1;
        }
      */
       /* int dataLen = (*(val+8)-'0')*10 + (*(val+9)-'0');
        if((dataLen + 10 - len) != 0)
        {
                debugout("接收数据长度错误！");
                return -1;
        }*/
        int i;
        char ch;

        for(i = 0; i < len; i++)
        {
                ch = *(val + i);
                priv = priv * 2 + ch - '0';
				debugout("priv = %d",priv);
        }

        return priv;
}
//-----------------------------------------------------------------------------
void utf8_to_ansi(char* src, char* &dest) 
{
        int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, src, strlen(src), NULL, 0);
        wchar_t* wszString = new wchar_t[wcsLen + 1];
        ::MultiByteToWideChar(CP_UTF8, NULL, src, strlen(src), wszString, wcsLen);  //utf8 转Unicode 。
        wszString[wcsLen] = '\0';

        int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
        dest = new char[ansiLen + 1];        //unicode版对应的strlen是wcslen 。
        ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), dest, ansiLen, NULL, NULL);
        dest[ansiLen] = '\0';
        delete [] wszString;
}


//---------------------------------------------------------------------------
__fastcall TFmIC::TFmIC(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFmIC::muClerkClick(TObject *Sender)
{
    FmClerks->ShowModal();

    int ret = FmIC->gdb.VQuery(512,"select state,authority,area,name from clerks where clkno='%s'",g_sys_opt.c_str());
    if(ret)
    {
          MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
          ModalResult = mrNone;
          return ;
    }
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();


    if(row == NULL)
    {
        MessageBox(Handle,"柜员不存在!\t","提示",MB_OK|MB_ICONINFORMATION);
        Application->Terminate();
    }

    g_sys_priv  = atoi(row[1]);
    g_sys_area  = row[2];
    g_sys_name  = row[3];

    showMenus();

    showChart();

    FmIC->Caption  = AnsiString("流程银行-集中提回影像采集系统")+"    登陆柜员["+g_sys_opt+"]["+g_sys_name+"]";
}
//---------------------------------------------------------------------------

void __fastcall TFmIC::FormShow(TObject *Sender)
{

    debugout("FromShow");
    int ret = gdb.Open("ZTIC_DB_HOST","gsip","gsip0728","gsipdb");
    if(ret)
    {
        MessageBox(Handle,"数据库连接错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        Application->Terminate();
    }

    m_bReplace = false;
    m_bReStart = false;
    m_bCommuteDate = false;
    m_bNetDelete = false;
    m_bSupply = false;
    m_bSend = false;
            
  /*  ret = FormLogin->ShowModal();
    if(ret == mrCancel )
    {
        Application->Terminate();
    }*/
/*
	FmWait = new TFmWait(Application);

	FmWait->Label1->Caption = "正在登录系统，请稍后 ...";

	FmWait->Show();*/

	ret  = Login();
	if (ret != 0)
	{
		if (ret == -1)
		{
			MessageBox(Handle,"登陆信息报文格式不正确!\n","提示",MB_OK|MB_ICONINFORMATION);
			Application->Terminate();
			return;
		}

		MessageBox(Handle,"登录失败!\n","提示",MB_OK|MB_ICONINFORMATION);
		Application->Terminate();
		return;
	}
   /*
        ret = GetPriv();
        if(ret != 0)
        {
                MessageBox(Handle,"获取权限错误\n","提示",MB_OK|MB_ICONINFORMATION);
        }
   */      
    showMenus();

    char mod[24];
/*
 //	memset(mod,0,sizeof(mod));
 //	getDS("orgcode",mod);
	//gmode = atoi(mod);
 */

    memset(mod,0,sizeof(mod));       //模式
    getDS("mode",mod);
    gmode = atoi(mod);

    memset(mod,0,sizeof(mod));        //是否检查清算信息
    getDS("clear",mod);               //
    gclear = atoi(mod);               //
    
    memset(mod,0,sizeof(mod));        //
    getDS("pretreat",mod);            //
    gprest = atoi(mod);               //

   /* memset(gsignip,0,sizeof(gsignip));
    memset(mod,0,sizeof(mod));
    getDS("signip",mod);
    strcpy(gsignip,mod);

    memset(mod,0,sizeof(mod));
    getDS("signport",mod);
    gsignport = atoi(mod);*/

    memset(gflowip,0,sizeof(gflowip));        //流程IP
    memset(mod,0,sizeof(mod));                //清空
    getDS("flowip",mod);
    strcpy(gflowip,mod);

    memset(mod,0,sizeof(mod));
    getDS("flowport",mod);
    gflowport = atoi(mod);

    memset(gdcode,0,sizeof(gdcode));
    memset(mod,0,sizeof(mod));
    getDS("dcode",mod);
    strcpy(gdcode,mod);

    memset(gbrno,0,sizeof(gbrno));
    memset(mod,0,sizeof(mod));
    getDS("brno",mod);
    strcpy(gbrno,mod);

    memset(gmsgtype,0,sizeof(gmsgtype));
    memset(mod,0,sizeof(mod));
    getDS("msgtype",mod);
    strcpy(gmsgtype,mod);

    memset(gmsgtypeex,0,sizeof(gmsgtypeex));
    memset(mod,0,sizeof(mod));
    getDS("msgtypeex",mod);
    strcpy(gmsgtypeex,mod);

    gPath = GetCurrentDir();
    showChart();

    m_area = g_sys_area;

    FmIC->Caption  = AnsiString("流程银行-集中提回影像采集系统")+"    登陆柜员["+g_sys_opt+"]["+g_sys_name+"]";
}
//---------------------------------------------------------------------------

void __fastcall TFmIC::N4Click(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------

void __fastcall TFmIC::muDepartClick(TObject *Sender)
{
    FormNets->ShowModal();
}
//---------------------------------------------------------------------------


int TFmIC::showMenus()
{
    //TODO: Add your source code here    
   lstTree->Items->Clear();

   TTreeNode *m;
   TTreeNode *p;
   TTreeNode *w;


    m = lstTree->Items->Add(0,"管理");
    p = lstTree->Items->Add(0,"业务");
    w = lstTree->Items->Add(0,"查询");


    if ( !(g_sys_priv & PRIV_OPT_CLK))
    {
        muClerk->Visible = false;
    }
    else
    {
      //  lstTree->Items->AddChild(m,EXP_PRIV_CLK);
        muClerk->Visible = false;
    }

    
    if ( !(g_sys_priv & PRIV_OPT_DEP))
    {
        muDepart->Visible = false;
    }
    else
    {
        lstTree->Items->AddChild(m,EXP_PRIV_DEP);
        muDepart->Visible = true;
    }

    if ( !(g_sys_priv & PRIV_OPT_VCH))
    {
        muVchtype->Visible = false;
    }
    else
    {
       lstTree->Items->AddChild(m,EXP_PRIV_VCH);
       muVchtype->Visible = true;
    }

    if ( !(g_sys_priv & PRIV_OPT_CFG))
    {
        muConfig->Visible = false;
    }
    else
    {
        lstTree->Items->AddChild(m,EXP_PRIV_CFG);
        muConfig->Visible = true;
    }

    if ( !(g_sys_priv & PRIV_OPT_PBC))
    {
        muPbc->Visible = false;
        tolPbc->Visible = false;
        sltPbc->Visible = false;
    }
    else
    {
        lstTree->Items->AddChild(p,EXP_PRIV_PBC);
        muPbc->Visible = true;
        tolPbc->Visible = true;
        sltPbc->Visible = true;
    }


    if ( !(g_sys_priv & PRIV_OPT_SCN))
    {
        muScan->Visible = false;
        tolScan->Visible = false;
        sltScan->Visible = false;
    }
    else
    {
        lstTree->Items->AddChild(p,EXP_PRIV_SCN);
        muScan->Visible = true;
        tolScan->Visible = true;
        sltScan->Visible = true;
    }

    if ( !(g_sys_priv & PRIV_OPT_VOU))
    {
        muVouchers->Visible = false;
        tolVoucher->Visible = false;
        sltVoucher->Visible = false;
    }
    else
    {
        lstTree->Items->AddChild(p,EXP_PRIV_VOU);
        muVouchers->Visible = true;
        tolVoucher->Visible = true;
        sltVoucher->Visible = true;
    }

    if ( !(g_sys_priv & PRIV_OPT_ENV))
    {
        muMultis->Visible = false;
        tolMultis->Visible = false;
        sltMultis->Visible = false;
        tolSpec->Visible = false;
        sltSpec->Visible = false;
    }
    else
    {
        lstTree->Items->AddChild(p,EXP_PRIV_ENV);
        if( strcmp(g_sys_area.c_str(),BRNO_BJ)== 0 )
        {
            lstTree->Items->AddChild(p,EXP_PRIV_SPC);
            tolSpec->Visible = true;
            sltSpec->Visible = true;
        }
        else
        {
            tolSpec->Visible = false;
            sltSpec->Visible = false;
        }
    }

    if ( !(g_sys_priv & PRIV_OPT_UPD))
    {
        muUpload->Visible = false;
        tolUpload->Visible = false;
        sltUpload->Visible = false;
    }
    else
    {
        lstTree->Items->AddChild(w,EXP_PRIV_UPD);
        muUpload->Visible = true;
        tolUpload->Visible = true;
        sltUpload->Visible = true;
    }
    
    if ( !(g_sys_priv & PRIV_OPT_REP))
    {
        m_bReplace = false;
    }
    else
    {
      
        m_bReplace = true;
    }
    
    if ( !(g_sys_priv & PRIV_OPT_RST))
    {
        m_bReStart = false;
    }
    else
    {
      
        m_bReStart = true;
    }
    
    if ( !(g_sys_priv & PRIV_OPT_DATA))
    {
        m_bCommuteDate = false;
        N5->Visible = false;
    }
    else
    {
        m_bCommuteDate = true;
        N5->Visible = true;
        
    }
    
     if ( !(g_sys_priv & PRIV_OPT_DELNET))
    {
        m_bNetDelete = false;
    }
    else
    {
        m_bNetDelete = true;
        
    }
    
      if ( !(g_sys_priv & PRIV_OPT_SUPPLY))
    {
        m_bSupply = false;
    }
    else
    {
        m_bSupply = true;
        
    }
    
      if ( !(g_sys_priv & PRIV_OPT_SEND))
    {
        m_bSend = false;
    }
    else
    {
        m_bSend = true;
        
    }


    if( strcmp(g_sys_area.c_str(),BRNO_BJ)== 0 )
    {
        N6->Visible = true;
        OldBi->Visible = true;
        N12->Visible = true;
    }
    
    return 0;
}
void __fastcall TFmIC::lstTreeClick(TObject *Sender)
{
    if( lstTree->Selected == NULL) return ;
    String opt = lstTree->Selected->Text;

    if(opt=="管理" || opt=="业务" || opt=="查询")
    {
        opt = "";
    }
    else
    {
        if( opt == EXP_PRIV_CLK )
        {
            FmClerks->ShowModal();
        }
        else if( opt == EXP_PRIV_DEP )
        {
            FormNets->ShowModal();
        }
        else if( opt == EXP_PRIV_VCH )
        {
            FmVchtype->ShowModal();  
        }
        else if( opt == EXP_PRIV_CFG)
        {
            FormConfig->ShowModal();
        }
        else if( opt == EXP_PRIV_DST )
        {
        }
        else if( opt == EXP_PRIV_PBC )
        {
            if( strcmp(g_sys_area.c_str(),BRNO_NJ)== 0 || strcmp(g_sys_area.c_str(),BRNO_SZ)== 0 )
            {
                FormPBC->ShowModal();
                showChart();
            }
            else if( strcmp(g_sys_area.c_str(),BRNO_BJ)== 0 )
            {
                BjFormPBC->ShowModal();
                showChart();
            }     
            else
                MessageBox(Handle,"暂不支持!\t","提示",MB_OK|MB_ICONINFORMATION);
        }
        else if( opt == EXP_PRIV_SCN )
        {
            FormScan->ShowModal();
        }
        else if( opt == EXP_PRIV_VOU )
        {
            FmVoucher->ShowModal();
        }
        else if( opt == EXP_PRIV_ENV )
        {
            FmEnv->ShowModal();

        }
        else if(opt == EXP_PRIV_SPC )
        {
          //  FormWS->boolExchno = true;
           // FormWS->ShowModal();
        }
        else if( opt == EXP_PRIV_UPD )
        {
            FormTrans->ShowModal();
        }
       /* else if( opt == EXP_PRIV_REP )
        {

        }*/
    }
     showChart();
}
//---------------------------------------------------------------------------

void __fastcall TFmIC::muVchtypeClick(TObject *Sender)
{
    FmVchtype->ShowModal();    
}
//---------------------------------------------------------------------------

void __fastcall TFmIC::muConfigClick(TObject *Sender)
{
    FormConfig->ShowModal();
}
//---------------------------------------------------------------------------   

void __fastcall TFmIC::muPbcClick(TObject *Sender)
{
	if( strcmp(g_sys_area.c_str(),BRNO_NJ)== 0 || strcmp(g_sys_area.c_str(),BRNO_SZ)== 0 )
	{
		FormPBC->ShowModal();
		showChart();
	}
	else if(strcmp(g_sys_area.c_str(),BRNO_BJ)== 0 )
	{
		BjFormPBC->ShowModal();
		showChart();
	}
	else
		MessageBox(Handle,"暂不支持!\t","提示",MB_OK|MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TFmIC::tolPbcClick(TObject *Sender)
{
    if( strcmp(g_sys_area.c_str(),BRNO_NJ)== 0  || strcmp(g_sys_area.c_str(),BRNO_SZ)== 0)
    {
        FormPBC->ShowModal();
        showChart();
    }
    else if( strcmp(g_sys_area.c_str(),BRNO_BJ)== 0 )
    {
        BjFormPBC->ShowModal();
        showChart();
    }
    else
        MessageBox(Handle,"暂不支持!\t","提示",MB_OK|MB_ICONINFORMATION); 
}
//---------------------------------------------------------------------------   

void __fastcall TFmIC::muScanClick(TObject *Sender)
{
	FormScan->ShowModal();
	showChart();  
}
//---------------------------------------------------------------------------
void __fastcall TFmIC::tolScanClick(TObject *Sender)
{
     FormScan->ShowModal();
     showChart();    
}
//---------------------------------------------------------------------------
void __fastcall TFmIC::muVouchersClick(TObject *Sender)
{
	FmVoucher->ShowModal();
	showChart();  
}
//---------------------------------------------------------------------------
void __fastcall TFmIC::tolVoucherClick(TObject *Sender)
{
    FmVoucher->ShowModal();
    showChart();     
}
//---------------------------------------------------------------------------
void __fastcall TFmIC::muMultisClick(TObject *Sender)
{
	FmEnv->ShowModal();
	showChart();  
}
//---------------------------------------------------------------------------
void __fastcall TFmIC::tolMultisClick(TObject *Sender)
{
    FmEnv->ShowModal();
    showChart();     
}
//--------------------------------------------------------------------------
void __fastcall TFmIC::muUploadClick(TObject *Sender)
{
    FormTrans->ShowModal();
    showChart();
}
//---------------------------------------------------------------------------

void __fastcall TFmIC::tolUploadClick(TObject *Sender)
{
    FormTrans->ShowModal();
    showChart();  
}
//---------------------------------------------------------------------------
int TFmIC::getDS(char* name, char* value)
{
    //TODO: Add your source code here
    int ret = gdb.VQuery(512,"select value from config where name='%s'",name);
	if(ret)
	{
		return ret;
	}

	MyRes res = gdb.Store();
	MyRow row;
	int count = (int)res.Count();
	if(count>0)
	{
		row = res.Fetch();
		strcpy(value,row[0]);

		if (strcmp(name,"orgcode") == 0)
		{
			ret = gdb.VQuery(512,"replace into config(name,value) values('orgcode','%s')",m_sOrgCode.c_str());
			if(ret)
			{
				MessageBox(Handle,"保存机构号码错误\t","提示",MB_OK|MB_ICONINFORMATION);
				return 0;
			}
		}
                
	}
	else 
	{
		//如果查询日期,没有查询到,则采用系统日期
		if( strcmp(name,"date") == 0)
		{
			ret=gdb.Query("select concat(substring(now(),1,4),substring(now(),6,2),substring(now(),9,2)) from  config limit 1");
			if(ret)
			{
				return ret;
			}
			res = FmIC->gdb.Store();
			row = res.Fetch();
			if(row == NULL)   strcpy(value,"00000000");
			else              strcpy(value,row[0]);
			return 0;
		}


		//如果查询场次,没有查询到,则根据时间设置场次
		else if (strcmp(name,"session")==0)
		{
			int T;
			ret = gdb.Query("select value from config where name='time'");
			if(ret)
			{
				return ret;
			}
			res = FmIC->gdb.Store();
			row = res.Fetch();
			if(row == NULL)   T=13;
			else              T=atoi(row[0]);

			ret=gdb.Query("select substring(now(),12,2) from  config limit 1");
			res = FmIC->gdb.Store();
			row = res.Fetch();
			if(row ==NULL) strcpy(value,"1");
			else
			{
				if( atoi(row[0])>=T)	strcpy(value,"2");
				else					strcpy(value,"1");
			}
			return 0;
		}
		else if (strcmp(name,"orgcode") == 0)
		{
			ret = gdb.VQuery(512,"insert into config(name,value) values('orgcode','%s')",m_sOrgCode.c_str());
			if(ret)
			{
				MessageBox(Handle,"保存机构号错误\t","提示",MB_OK|MB_ICONINFORMATION);
				return 0;
			}
		}
	    else
		{
			strcpy(value,"0");
			return 0;
		}
	}
        
	return 0;
}         

int TFmIC::showChart()
{
    //TODO: Add your source code here
    memset(gdate,0,sizeof(gdate));
    memset(gsession,0,sizeof(gsession));

    getDS("date",gdate);
    getDS("session",gsession);     
    
    int ret = gdb.VQuery(512,"select count(*) from vouchers where date='%s' and session='%s' and area='%s'",gdate,gsession,g_sys_area.c_str());
    if(ret)
    {
        return ret;
    }

    MyRes res = gdb.Store();
    MyRow row = res.Fetch();           

    int vt = atoi(row[0]);
    ret = gdb.VQuery(512,"select count(*) from multis where (model= 2 or model =6 or model =7) and date='%s' and session='%s' and area='%s'",gdate,gsession,g_sys_area.c_str());
    if(ret)
    {
        return ret;
    }

    res = gdb.Store();
    row = res.Fetch();
    int mt = atoi(row[0]);

    Series1->Clear();
    Series1->Add(vt,"单票票据",clRed);
    Series1->Add(mt,"信封票据",clBlue);

    if( strcmp(g_sys_area.c_str(),BRNO_BJ)== 0 )    //北京分行
    {
        ret = gdb.VQuery(512,"select count(*) from authinput where (multiflag=1 or multiflag=3) and mainflag=1 and  date='%s' and session='%s' and area='%s'",gdate,gsession,g_sys_area.c_str());
        if(ret)
        {
            return ret;
        }
        res = gdb.Store();
        row = res.Fetch();

        int at = atoi(row[0]);  
        Series1->Add(at,"特色业务",clBlue);
    }

    return 0;
}
void __fastcall TFmIC::N5Click(TObject *Sender)
{

	if(!FmIC->m_bCommuteDate)
	{
		MessageBox(Handle,"该柜员没有设置会计日期的权限!\t","提示",MB_OK|MB_ICONINFORMATION);
		return;	
	}

    
    FormDate->ShowModal();
    showChart();
}
//---------------------------------------------------------------------------
void __fastcall TFmIC::tolSpecClick(TObject *Sender)
{
  //  FormWS->boolExchno = true;
   // FormWS->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TFmIC::Button1Click(TObject *Sender)
{
   /* for(int i = 0 ;i<1000;i++)
    {
        char tname[128];
        memset(tname,0,sizeof(tname));
        sprintf(tname,"%s//%02d//%05d//%s","20100809",2,61147,"061147-05310179-0017-AA.jpg");

        MemT image;

        int sizex;
        int sizey;
        int panel;


        int ret =  ti_getimage( tname, "182.64.34.89", 0,   image, sizex, sizey,panel );
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
        FormImage->image->Picture->Bitmap->LoadFromStream(pms);
        pms->Clear();

        debugout("次数[%d]",i);

        ti_RGB2BMPFREE(bmp);

    }

    MessageBox(Handle,"获取图像完成!\t","提示",MB_OK|MB_ICONINFORMATION);
    int ret = FormWS->subMit("060149");
    if(ret)
    {
        FormWS->cmbNetno->SetTextBuf("060149");
        FormWS->boolExchno = false;
        FormWS->ShowModal();
    }        */

    ZSocket so;
    int ret = so.Connect("127.0.0.1",1105);

    if(ret!=0)
    {
    	ShowMessage("获取图像完成");
    	return;
    }
    so.Send("scan",5);
    
}
//网点处理状态
void __fastcall TFmIC::N7Click(TObject *Sender)
{
    FormST->ShowModal();    
}
//---------------------------------------------------------------------------
//未处理业务统计
void __fastcall TFmIC::N8Click(TObject *Sender)
{
    FormUndo->ShowModal();
}
//---------------------------------------------------------------------------
//业务统计
void __fastcall TFmIC::N11Click(TObject *Sender)
{
    FormStatic->ShowModal();  
}
//---------------------------------------------------------------------------

void __fastcall TFmIC::FormClose(TObject *Sender, TCloseAction &Action)
{
	
	String cmd =  AnsiString("update clerks set loginstate=0 where clkno='")+g_sys_opt.c_str()+"'";
	int ret = FmIC->gdb.Query(cmd.c_str());
	if(ret)
	{
		MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		return ;
	}

	ScannerIni m_Ini;
		
	if( access(m_Ini.m_iniPath,0) )
    {
    	MessageBox(Handle,"读取配置文件失败!\t","错误",MB_OK+MB_ICONERROR);
        return ;
    }

   	ret = m_Ini.LoadIniData("scanner.cfg");   ////初始化m_Ini
    if( ret == -1 )
    {
		MessageBox(Handle,"请设置系统参数!\t","警告",MB_OK+MB_ICONINFORMATION);
        return ;
    }
    
    
    String tmp;
    tmp.printf("%d",m_Ini.m_Date);
    
   
    FindFile(m_Ini.m_scannerInfo.path);
    
	
}
//---------------------------------------------------------------------------

//清分数据
void __fastcall TFmIC::N12Click(TObject *Sender)
{
    distForm->ShowModal();    
}
//---------------------------------------------------------------------------


void __fastcall TFmIC::N13Click(TObject *Sender)
{
    FormReplace->setDSEx2();

    FormReplace->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFmIC::N14Click(TObject *Sender)
{
    FormReplace->setDSEx3();

    FormReplace->ShowModal();
}
//---------------------------------------------------------------------------
int TFmIC::CleanOldImage(String filename,long CreateTime)
{
	
		ZDateTime zdate;
       zdate.Now();


       String nowTime;
       nowTime.printf("%s",zdate.BankDateString());

       int nowy = atoi(nowTime.SubString(0,4).c_str());

       int nowm = atoi(nowTime.SubString(5,2).c_str());

       int nowd = atoi(nowTime.SubString(7,2).c_str());

       int year = CreateTime/10000;

       int month = CreateTime%10000/100;

       int day = CreateTime%100;


       if(year != nowy)
       {
            DeleteFile(filename.c_str());
            return 0;
       }

       if(month != nowm)
       {
                if(month < nowm)
                {
                   int a = nowm - month;

                   int b = a*30 + nowd - day;

                   if(b>= 30)
                   {
                   			 DeleteFile(filename.c_str());
        						}
                 
                }

                return 0;
       }
				
			return 0;		
}
//---------------------------------------------------------------------------
int TFmIC::FindFile(String Path)
{
		String strPath ;
    strPath.printf("%s\\*.*",Path.c_str());

		WIN32_FIND_DATA wd;

		HANDLE hFile = FindFirstFile(strPath.c_str(),&wd);

		 if (hFile)
		 {
			 do
			 {


				 String strFile;
				 strFile.printf("%s\\%s",Path.c_str(),wd.cFileName);
				 
				 if(strcmp(wd.cFileName,".") == 0)
				 {
				 		continue ;
				 }

				 if(strcmp(wd.cFileName,"..") == 0)
				 {
				 		continue;
				 }

				if(wd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				{
				     FindFile(strFile);
				     continue;
				}
				 
        //MessageBox(Handle,strFile.c_str(),"提示",MB_OK|MB_ICONINFORMATION);
        
                
				 String createTime;

         SYSTEMTIME tm;

         FileTimeToSystemTime(&wd.ftCreationTime,&tm);

         createTime.printf("%04d%02d%02d",tm.wYear,tm.wMonth,tm.wDay);
         
         CleanOldImage(strFile,atol(createTime.c_str()));

       //  MessageBox(Handle,createTime.c_str(),"提示",MB_OK|MB_ICONINFORMATION);


			 } while (FindNextFile(hFile,&wd));
		 }
		 else
		 {

        MessageBox(Handle,"HFile is null","提示",MB_OK|MB_ICONINFORMATION);

		 }
		 FindClose(hFile);
}
int TFmIC::Login()
{

	TiXmlElement* root, *ele;
	TiXmlDocument* doc;
	doc = new TiXmlDocument();

	if (doc == NULL)
	{
		return -1;
	}

	TCHAR szIni[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szIni);
	debugout("%s",szIni);
 
	//doc->LoadFile("vdicsConfig.xml");
	if (m_XmlMsg_In.Length() > 0)
	{
		debugout("m_XmlMsg_In.len = %d",m_XmlMsg_In.Length());
		
		if (m_XmlMsg_In.SubString(0,1) == "\"")
		{
			m_XmlMsg_In.Delete(1,1);
			m_XmlMsg_In.Delete(m_XmlMsg_In.Length(),1);

			debugout("xml = %s",m_XmlMsg_In.c_str());
		}
	}
	else 
	{
		MessageBox(Handle,"传入登录信息为空!\n","提示",MB_OK|MB_ICONINFORMATION);
		ModalResult = mrNone;
		return -5;
	}

	doc->Parse(m_XmlMsg_In.c_str());

	if(doc->Error())
	{
		delete doc;
		ModalResult = mrNone;
		return -1;
	}
	///////////////////////////////////////////////////////////////////

	if (!doc || !(root = ((TiXmlDocument*)doc)->RootElement()))
	{
		OutputDebugString("根节点错误  返回");
		delete doc;
		ModalResult = mrNone;
		return -1;
	}

	ele = root->FirstChildElement();

	if (!ele)
	{
		OutputDebugString("子节点不存在");
		delete doc;
		ModalResult = mrNone;
		return -1;
	}

	char* pname, *pvalue;

	while(ele)
	{
		pname = (char *)ele->Value();
		pvalue = (char *)ele->GetText();

		debugout("[%s] [%s]",pname,pvalue);

		if (lstrcmpi(pname,"tellerNo") == 0)
		{

			if(pvalue)
			{
				g_sys_opt.printf(pvalue);
				m_sOptCode = g_sys_opt;
			}
			else
			{
				MessageBox(Handle,"登录失败,没有登录信息!\n","提示",MB_OK|MB_ICONINFORMATION);
				ModalResult = mrNone;
				return -5;
			}
		}

		/*
		else if(lstrcmpi(pname,"tellerPower") == 0)
		{

			if(pvalue)
			{
				
				g_sys_priv = ParsePriv(pvalue);
				
			}
		}
		*/

		else if(lstrcmpi(pname,"tellerOrgCode") == 0)
		{

			if(pvalue)
			{
				
				g_sys_org.printf(pvalue);
				m_sOrgCode = g_sys_org;
				
			}
		}
		else if(lstrcmpi(pname,"tellerName") == 0)
		{

			if(pvalue)
			{
				char *tmp = NULL;
				try
				{
					utf8_to_ansi(pvalue, tmp);	
					g_sys_name.printf(tmp);
				}
				catch(...)
				{
					g_sys_name.printf(pvalue);
				}
				
				//utf8_to_ansi(pvalue, tmp);		
				
			}
		}
		else if(lstrcmpi(pname,"tellerArea") == 0)
		{

			if(pvalue)
			{
				g_sys_area.printf(pvalue);
			}
		}
		else if(lstrcmpi(pname,"path") == 0)
		{

			if(pvalue)
			{
				::SetCurrentDirectory(pvalue);

			}
		}
		
		ele = ele->NextSiblingElement();
	}

	gPath = GetCurrentDir();

	debugout("Current Directory = %s",gPath.c_str());


	PrintToLog(m_XmlMsg_In.c_str());

    LoadHosts();

    PrintToLog("GetProwIP =%s,GetProwPort=%s",GetProwIP.c_str(),GetProwPort.c_str());

	char send[1024];
	memset(send,0,sizeof(send));

	ZSocket so;
	sprintf(send,"%08d19%08d%s",g_sys_opt.Length()+10,g_sys_opt.Length(),g_sys_opt.c_str());

	PrintToLog("获取柜员权限Send->[%s]",send);

	int iRstLen =so.Connect(GetProwIP.c_str(),atoi(GetProwPort.c_str()));
	if(iRstLen!= 0 )
	{
        PrintToLog("获取柜员权限Socket连接失败\n,[%s][%s]",GetProwIP.c_str(),GetProwPort.c_str());
		MessageBox(Handle,"获取柜员权限Socket连接失败!\n","提示",MB_OK|MB_ICONINFORMATION);
		so.Close();
		return -2;
	}

	iRstLen = so.Send(send,strlen(send));

	if(iRstLen<0 )
	{
        PrintToLog("获取柜员权限,发送数据失败[%s][%d]\n",send, iRstLen);
		MessageBox(Handle,"获取柜员权限,发送数据失败!\n","提示",MB_OK|MB_ICONINFORMATION);
		so.Close();
		return -1;
	}

	char out[512];
	memset(out,0,sizeof(out));
	iRstLen = so.RecvEx(out,10);
	if(iRstLen<= 0 )
	{
        PrintToLog("获取柜员权限,接收前10位标志出错[%s][%d]\n",send, iRstLen);
		MessageBox(Handle,"获取柜员权限,接收数据失败!\n","提示",MB_OK|MB_ICONINFORMATION);
		so.Close();
		return -2;
	}
    
	
	debugout("out = %s",out);
	

    memset(out,0,sizeof(out));
	iRstLen = so.RecvEx(out,8);
	if(iRstLen<= 0 )
	{
        PrintToLog("获取柜员权限,接收8位长度数据失败[%s][%d]\n",send, iRstLen);
		MessageBox(Handle,"获取柜员权限,接收数据失败!\n","提示",MB_OK|MB_ICONINFORMATION);
		so.Close();
		return -2;
	}


	int XmlLen = atoi(out);
	memset(out,0,sizeof(out));

	//RecvEx, 50秒超时
	iRstLen = so.RecvEx(out,XmlLen);
	if(iRstLen<= 0 )
	{
        PrintToLog("获取柜员权限,接收数据失败[%s][%d]\n",send, iRstLen);
		MessageBox(Handle,"获取柜员权限,接收数据失败!\n","提示",MB_OK|MB_ICONINFORMATION);
		so.Close();
		return -2;
	}

	so.Close();
   	sprintf(out,"%s","1111111111111111111111");

	PrintToLog("out = %s",out);

 //   strcpy(out,"000111111111111100000");


    //debugout("out = %s",out);

	g_sys_priv = ParsePriv(out);

        if(g_sys_priv <= 0 )
        {
                g_sys_priv = 31232;       //数据导入，扫描，单票，信封 ，上传
        }

	PrintToLog("g_sys_opt = %s,g_sys_priv= %d,g_sys_area=%s,g_sys_name=%s\n",g_sys_opt.c_str(),g_sys_priv,g_sys_area.c_str(),g_sys_name.c_str());

	if(g_sys_opt.Length()<=0)
	{
		MessageBox(Handle,"登陆信息缺少柜员号!\n","提示",MB_OK|MB_ICONINFORMATION);
		ModalResult = mrNone;
		return -6;
	}
	

	if(g_sys_area.Length()<=0)
	{
		MessageBox(Handle,"登陆信息缺少交换区域!\n","提示",MB_OK|MB_ICONINFORMATION);
		ModalResult = mrNone;
		return -6;
	}

	if(g_sys_priv <=0)
	{
		MessageBox(Handle,"获取柜员权限出错!\n","提示",MB_OK|MB_ICONINFORMATION);
		ModalResult = mrNone;
		return -6;
	}

	if(g_sys_name.Length()<=0)
	{
		MessageBox(Handle,"登陆信息缺少柜员名!\n","提示",MB_OK|MB_ICONINFORMATION);
		ModalResult = mrNone;
		return -6;
	}

	String cmd;

	int  ret = FmIC->gdb.VQuery(512,"select state,loginstate from clerks  where clkno='%s' ",g_sys_opt.c_str());
	 
	PrintToLog("select state,loginstate from clerks  where clkno='%s' ",g_sys_opt.c_str());

	if(ret)
	{
		MessageBox(Handle,"查询柜员登陆状态，数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		ModalResult = mrNone;
		return -3;
	}

	MyRes res = FmIC->gdb.Store();
	MyRow row = res.Fetch();

	if(row==NULL)
	{
		 cmd.sprintf("insert into clerks( clkno,name,authority,state,area) values('%s','%s',%d,1,'%s')",g_sys_opt.c_str(),g_sys_name.c_str(),g_sys_priv,g_sys_area.c_str());
		 PrintToLog(cmd.c_str());
		 ret = FmIC->gdb.Query(cmd.c_str());  
		 if(ret)
		 {
			 MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
			 ModalResult = mrNone;
			 return -3;
		 }    
	}
	else
	{
		int logst = atoi(row[1]);
		if(logst == 1)
		{
			if(MessageBox(Handle,"柜员已登录,是否继续?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO)
			{
				ModalResult = mrNone;
				return -7;
			}
		}
	}

    cmd =  AnsiString("update clerks set loginstate=1,logintime=now() where clkno='")+ g_sys_opt.c_str()+"'";  

	PrintToLog(cmd.c_str());
    
	ret = FmIC->gdb.Query(cmd.c_str());
    if(ret)
    {
          MessageBox(Handle,"数据库查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
          ModalResult = mrNone;
          return -4;
    }

	return 0;
  
}
/**
    设置host文件
*/
int TFmIC::LoadHosts()
{
    //TODO: Add your source code here
    char sysdir[MAX_PATH+80];
    int len = GetSystemDirectory(sysdir,MAX_PATH+80);
    HOSTS_FILE = sysdir;
    HOSTS_FILE += "\\drivers\\etc\\Hosts";


    delete p;
    p = 0;
    p = new TStringList;

    //从文件中获取内容
    p->LoadFromFile(HOSTS_FILE);

    for( int i = 0 ; i < p->Count ; i++ )
    {
        String stemp = p->Strings[i];
        if( !stemp.IsEmpty() && stemp[1] != '#' )
        {
            int itr = stemp.Pos(" ");

            if( itr == 0)
            {
                itr = stemp.Pos("\t");
            }
            if( itr != 0)
            {
                String szName = (stemp.SubString( itr+1, stemp.Length()-itr )).Trim();

                if( szName == "ZTIC_PROW_HOST" )
                {
                    GetProwIP = (stemp.SubString( 1, itr-1 )).Trim();
                }
                if( szName == "ZTIC_PROW_PORT" )
                {
                    GetProwPort = (stemp.SubString( 1, itr-1 )).Trim();
                }


                if( szName == "ZTIC_PROW_HOST"  ||
                    szName == "ZTIC_PROW_PORT"   )
                {
                    p->Delete(i);
                    i--;
                }
            }
        }
    }

    return 0;
}

int TFmIC::PrintToLog(const char* msg,...)
{

	SYSTEMTIME tm;

	GetLocalTime(&tm);

	String sznow, logname;

	logname.sprintf("%s\\log\\vdics-%04d-%02d-%02d.log", gPath.c_str(),tm.wYear, tm.wMonth, tm.wDay);

	sznow.sprintf("[%04d-%02d-%02d %02d:%02d:%02d]", 
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);


	char buf[2014];

	try{

		va_list ap;
		va_start(ap,msg);
		_vsnprintf(buf,sizeof(buf)-1,msg,ap);
		va_end(ap);
		//////////////////////////////////////
		FILE* fp;
		fp = fopen(logname.c_str(),"a+t");
		if(fp)
		{
			
			fwrite(sznow.c_str(), sznow.Length(), 1, fp);

			fprintf(fp,"%s\n",buf);

			fwrite("\r\n", strlen("\r\n"), 1, fp);

		}
		fclose(fp);
		//////////////////////////////////////
	}
	catch(...)
	{
		OutputDebugString("exception in construct debug message\n");
	}


	return 0;
}

//------------------------------------------------------------------------------
int TFmIC::GetPriv()
{
        int priv = 0;
        int ret = gdb.VQuery(512,"select authority from clerks where clkno='%s' and area='%s'",g_sys_opt.c_str(),g_sys_area.c_str());
	if(ret)
	{
		return -1;
	}

	MyRes res = gdb.Store();
	MyRow row = res.Fetch();

        while(row)
        {
                priv = atoi(row[0]);
                String s1;
                s1.sprintf("priv = %d",priv);
                ShowMessage(s1);
                row = res.Fetch();
        }

        String s2;
        s2.sprintf("g_sys_priv = %d",g_sys_priv);
        ShowMessage(s2);
        if(priv)
                g_sys_priv = priv;
       
        return 0;
}