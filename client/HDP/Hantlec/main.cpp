/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//--------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "about.h"
#include "wiseIPdlg.h"
#include "totalmoney.h"
#include "selNet.h"
#include "wait.h"
#include "ConfigFile.h"
#include "SplitDc.h"
#include "rescan.h"
#include "distData.h"

#include <distControllor.h>
#include <TC_hdp.h>
#include <IO.H>
#include <windows.h>
#include <DIRECT.H>
#include <AnsiMemfile.h>
#include <zcommon.h>
#include <bcbext.h>
#include <Filectrl.hpp>
#include <dstcommon.h>
#include <zsocket.hpp>
#include <zcommon.h>
#include <dboperate.h>
#include <ztime.hpp>
#include <utility/singleinstance.h>
#include <iostream>
#include "state.h"

#include <commonresource.h>
#include <checkidrc.h>

#include "badclean.h"
#include "histquery.h"
#include "HDPCtrlUI.h"
#include "setup.h"
#include "ManVch.h"
#include "wait.h"
#include "login.h"
#include "pubfun.h"
#include "ManVch.h"
#include "pocket.h"
#include "vips.h"

#define TXT_START   "��ʼ &S"
#define TXT_GOON    "���� &S"
#define TXT_PAUSE   "��ͣ &S"

USEFORM("ManVch.cpp", ManAdd);

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFmDistMan *FmDistMan;
static char txtDeCr[2][8] = {
    "�跽",
    "����"
};
static char txtEgg[2][16] = {
    "-��ͨģʽ-",
    "-����ģʽ-"
};
static int Dist2Color[12] = {
    0x02FFFFFF,
    0x02C8FFC8,
    0x02AAFFAA,
    0x02C8C8FF,
    0x02AAAAFF,
    0x02C8C8FF,
    0x02AAAAFF,
    0x02C8E3FF,
    0x02AAD5FF,
    0x02FFAAFF,
    0x02FFFFFF,
    0x02FFFFFF
};
enum
{
    STB_STATE = 1,          //STATE
    STB_MCR   = 3,          //MAN CHECK RESULT
    STB_MODE  = 6,          //FAST MODE,COMMON MODE
    STB_RVC   = 9,          //VOUCHER REDIST CHECK
    STB_ENDORSE = 12        //ENDORSING IN SECOND SORT
};
enum
{
    RDS_NONE  = 0,          //NO REDIST VOUCHER
    RDS_FIRST = 1,          //FIRST TIME REDIST
    RDS_SECOND = 2          //SECOND TIME REDIST
};
//---------------------------------------------------------------------------
/**
    ���캯��
    ��ʼ����Ա����
*/
__fastcall TFmDistMan::TFmDistMan(TComponent* Owner)
    : TForm(Owner)
{
    _stop = false;

    Distval.colorScanSpeed = DEFAULT_COLOR_SCAN_SPEED;
    Distval.grayScanSpeed = DEFAULT_GRAY_SCAN_SPEED;
}
//---------------------------------------------------------------------------
void __fastcall TFmDistMan::muAboutClick(TObject *Sender)
{
    show_about("��ֿ���");
}
//---------------------------------------------------------------------------
/**
    ��ʼ����¼�����
    �ȼ��Ӳ�̿ռ�
    Ȼ����ݵ�ǰ״̬������ֻ�����
*/
void __fastcall TFmDistMan::btnStartClick(TObject *Sender)
{
    //���Ӳ�̿ռ�
    Screen->Cursor = crHourGlass;
    btnTallyCheck->Enabled = false;
    N7->Enabled = false;

    int ret = GetDS("session",session);
    if(ret)
    {
        MessageBox(Handle,"��ȡ���γ���","��ʾ",MB_OK|MB_ICONERROR);
        FmWait->Hide();
        delete FmWait; FmWait = NULL;
        return ;
    }
    txtSession->Items->Clear();
    txtSession->Items->Add(session);
    txtSession->ItemIndex = 0;        

    ZDateTime zdate;
    //���ݵ�ǰ״̬������ֻ�����
    switch( _nDstState )
    {
        case ST_STOP:                     
                zdate.Now();
                curid = zdate.GetAnsiVal()%10000000;
                scanid = 0 ;
                ToFastMode();
                m_bCtrlAmountLoaded = false;
 //            break;
        case ST_PAUSE:
                btnTallyCheck->Enabled = false;
                ret = Start();
                break;
        case ST_RUN:
                ret = Pause();
                break;
        default:
                break;
    }
    if(ret)
    {
        errMsg(ret);
    }

    Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
/**
    ���崴���¼�����
    �Կؼ����г�ʼ��
*/
void __fastcall TFmDistMan::FormCreate(TObject *Sender)
{
    //��ʼ����½���� , add by fanpf -------------------------------------------
    //�����ݿ�
    int ret = gdb.Open("ZTIC_DB_HOST",DB_USER,DB_PASSWD,DB_DBAlias);

    if(ret)
    {
        MessageBox(Handle,"���ݿ����Ӵ���!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        Application->Terminate();
    }

    //��½���ڳ�ʼ������ ------------------------------------------------------
    
    Screen->Cursor = crHourGlass;

    bmp1        = new Graphics::TBitmap;
    bmp2        = new Graphics::TBitmap;
    bmp3        = new Graphics::TBitmap;
    bmpStart    = new Graphics::TBitmap;
    bmpPause    = new Graphics::TBitmap;

    tsFirst->TabVisible  = false;
    tsSecond->TabVisible = false;

    pgDists->ActivePageIndex = 0;
    cbMoneyKind->ItemIndex   = 0;
    pHDP = new HDPCtrlUI;

    imgs12->GetBitmap( 0, bmp1 );
    imgs12->GetBitmap( 1, bmp2 );
    imgs12->GetBitmap( 2, bmp3 );
    imgsSS->GetBitmap( 0, bmpStart );
    imgsSS->GetBitmap( 1, bmpPause );

    signJF->Caption = "=";
    signDF->Caption = "=";

    btnDstChange->Glyph->Assign(bmp1);
    btnStart->Glyph->Assign( bmpStart );

    btnStart->Caption = TXT_START;
    btnFinish->Enabled  = false;

    _nDstState   = ST_STOP;
    _nDstMode    = MD_FIRST;
    _reDistState = RDS_NONE;

    muFlush->Visible = true;

    N7->Enabled = false;
}
//---------------------------------------------------------------------------
/**
    ������̿��ƺ���
*/
int TFmDistMan::Start()
{
    //TODO: Add your source code here
    int ret = 0;
    int netno = 0;
    switch( _nDstMode )
    {
        case MD_FIRST:  //һ�����
                ret = StartFirst();
                break;
        case MD_SECOND: //�������
                ret = StartSecond();
                break;
        case MD_THIRD:  //�������
                muFastModeClick(NULL);
                ret = StartThird();
                break;
        default:
                break;
    }
    if( ret )
    {
        return ret;
    }


    txtSession->Enabled = false;

    //��ʾ�����������������Ϣ
    ShowSetting();
    if(btnStart->Caption==TXT_START)
    {
        _stop = false;
        N2->Checked = true;
    }

   int currency = getCurrency( cbMoneyKind->ItemIndex );

    //���Ʊ������
    int vch_tc = ( _nDstMode == MD_FIRST) ? 0 : FmSelNet->m_vchTc;

    DISTCTRLDATA dcd;
    memset(&dcd,0,sizeof(dcd));
    dcd.dstMode  = _nDstMode;
    dcd.dstState = _nDstState;
    dcd.currency = currency;
    dcd.netno = atoi(curPkgNo);

    dcd.vch_tc = vch_tc;
    dcd.isTallyMode = (_nDstMode==MD_FIRST)? true : FmSelNet->cbTallyMode->Checked;

    //�������
    ret = StartDstEx(hDistUI,&dcd);    //Act decided by dcd.dststate;

    btnFinish->Enabled = false;
    btnExit->Enabled = false;
    muExit->Enabled  = false;

    btnStart->Glyph->Assign( bmpPause );

    btnFinish->Enabled = true;

    btnNetCancel->Enabled = true;

    cbMoneyKind->Enabled = false;

    _nDstState = ST_RUN;

    btnFinish->Enabled = false;
    btnNetCancel->Enabled = false;
    btnStart->Enabled = false;
    //�������
    try
    {
        if(_nDstMode == MD_FIRST)
        {
            if(SetupDlg->ckColor->Checked == true)
            {
                if(btnStart->Caption==TXT_START ) pHDP->Start(3, Distval.colorScanSpeed, true);  //��ɫ
                else                              pHDP->Start(3, Distval.colorScanSpeed, false); //��ɫ
            }
            else
            {
                if(btnStart->Caption==TXT_START ) pHDP->Start(2, Distval.colorScanSpeed, true);  //��ɫ
                else                              pHDP->Start(2, Distval.colorScanSpeed, false); //��ɫ
            }
        }
        else if(_nDstMode == MD_SECOND)
        {
            if(btnStart->Caption==TXT_START ) pHDP->Start(2, Distval.grayScanSpeed, true); //�Ҷ�
            else                              pHDP->Start(2, Distval.grayScanSpeed, false);//�Ҷ�
        }

        else if(_nDstMode == MD_THIRD)
        {
            if(btnStart->Caption==TXT_START ) pHDP->Start(2, Distval.colorScanSpeed, true); //��ɫ
            else                              pHDP->Start(2, Distval.colorScanSpeed, false);//��ɫ
        }
    }
    catch(...)
    {
        Screen->Cursor = crDefault;
        debugout("start catch ... \n");
        Pause();
    }
    btnStart->Caption = TXT_GOON;
    UpdateStateBar();

    return 0;
}
//---------------------------------------------------------------------------
/**
    ֹͣ��ֺ���
*/
int TFmDistMan::Stop(bool netcancel)
{
    int ret = StopDist( hDistUI );
    if( ret ) return ret;
    pHDP->Stop();

    btnStart->Glyph->Assign( bmpStart );
    btnStart->Caption = TXT_START;
    _nDstState = ST_STOP;
    UpdateStateBar();

    btnFinish->Enabled = false;
    btnNetCancel->Enabled = false;
    btnExit->Enabled = true;
    muExit->Enabled = true;
    cbMoneyKind->Enabled = true;
    _reDistState = RDS_NONE;

    if( netcancel )
    {
        return 0;
    }

    int count1 = 0;
    int count2 = 0;

    //һ�������Ҫ��������
    if(_nDstMode==MD_FIRST)
    {
        count1 = txt1TotalCountJF->Caption.ToInt();
        count2 = txt1TotalCountDF->Caption.ToInt();

        if( SetupDlg->ckManCtrlAmount->Checked )
        {
            String dr = RemoveThousandChar(txt1TotalSumJF->Caption.c_str());
            String cr = RemoveThousandChar(txt1TotalSumDF->Caption.c_str());
        }

        FmWait = new TFmWait(Application);
        FmWait->txtMessage->Caption = "  ���ڱ������ݣ����Ժ�......";
        FmWait->Show();
        Application->ProcessMessages();
    }
    //����ֽ������
    ret = StoreDistData(hDistUI);
    if(_nDstMode == MD_FIRST)
    {
        FmWait->Hide();
        delete FmWait;
        FmWait = NULL;
    }
    if(ret)
    {
        return ret;
    }

    return 0;
}
//---------------------------------------------------------------------------
/**
    ��ɰ�ť�����Ӧ����
*/
void __fastcall TFmDistMan::btnFinishClick(TObject *Sender)
{
    Screen->Cursor = crHourGlass;

    //��ֻ�������תʱ������ֹͣ
    if( !btnFinish->Enabled )
    {
        MessageBox(Handle,"�����δֹͣ,�޷����!\t","���",MB_OK|MB_ICONWARNING);
        Screen->Cursor = crDefault;
        return;
    }
    int ret = 0;

    //һ�������Ҫ��������
    if(_nDstMode==MD_FIRST)
    {
        if(txt1SortCount->Caption.Trim().ToInt()==0)
        {
             MessageBox(Handle,"���������!\t","���",MB_OK|MB_ICONWARNING);
             Screen->Cursor = crDefault;
             return ;
        }
        //����������
        if( CheckTotalAmount())
        {
            MessageBox(Handle,"��ƽ,�޷��ύ!\t","���",MB_OK|MB_ICONWARNING);
            Screen->Cursor = crDefault;
            return;
        }
    }


     if( _nDstMode == MD_FIRST &&
        IsNetStored(txt1PkgNo->Caption) )
    {
        MessageBox(Handle,"�������ѽ��й�һ�����,���������!\t","��ʾ",
            MB_OK|MB_ICONINFORMATION);
        Screen->Cursor = crDefault;
        return;
    }

    ret = Stop();
    if(ret&& _nDstMode==MD_FIRST )
    {
        MessageBox(Handle,"�洢ʧ��!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        Screen->Cursor = crDefault;
        return;
    }

    ret = gdb.VQuery(512,"update bocctrl set state='1100000000000000'  where exchno='%s' and date='%s' and session='%s'",txt1PkgNo->Caption.c_str(),date,session);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    ret = gdb.VQuery(512,"update dists set area='%s' where exchno='%s' and date='%s' and session='%s'",g_sys_area.c_str(),txt1PkgNo->Caption.c_str(),date,session);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    
    _nDstState = ST_STOP;


    m_bCtrlAmountLoaded = false;
    txtSession->Enabled = true;

    int retry_count = 0;
    if(_nDstMode==MD_FIRST)
    {
        do
        {
            ret = StartTally(txt1PkgNo->Caption.c_str());
        } while( ret && retry_count++ < 3 );

        if( ret)
        {
            String msg = "�����Ʊ�ǼǴ���!\t" ;
            MessageBox(Handle,msg.c_str(),"����",MB_OK+MB_ICONERROR);
        }

    }     

    
    Application->ProcessMessages();
    Screen->Cursor = crDefault;
}


int TFmDistMan::StartTally(char *exchno)
{
    int ret =  gdb.VQuery(512,"select notice from bocnets where exchno='%s'",exchno);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����","��ʾ",MB_OK|MB_ICONINFORMATION);
        return ret;
    }

    MyRes res = gdb.Store();
    MyRow row = res.Fetch();
    int notice;
    if(row != NULL)
    {
        notice = atoi(row[0]);
    }
    else
         notice = 1;
         
    if(notice == 0 ) return 0;
    
    ret = gdb.VQuery(512,"select count(*) from  dists where vchtype = '%s' and exchno='%s' and area='%s' and date='%s' and session='%s'",_cfgMap["cdcode"].c_str(),exchno,g_sys_area.c_str(),date,session);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    res = gdb.Store();
    row = res.Fetch();
    int count = atoi(row[0]);


    if(count == 0 ) return 0;

    String headstr = makeHead(ZM_DIST_RTCS);


    CAnsiMemFile memFile;
    memFile.Write(date,8,strlen(date),false);
    memFile.Write(g_sys_area.c_str(),6,g_sys_area.Length(),false);
    memFile.Write(exchno,12,strlen(exchno),true,'0');


    memFile.Write(session,3,strlen(session),true,'0');
    memFile.Write("CNY",3,3,false);

    char ct[8];
    memset(ct,0,sizeof(ct));
    sprintf(ct,"%d",count);
    memFile.Write(ct,4,strlen(ct),true);
    memFile.Write("\0", 1, 1, false);

    String body = (char*)memFile.GetPtr();
    String send = headstr + body;

    char cmd[8];
    char cmdstr[512];
    char sendstr[1024];
    int  result=0;

    memset(sendstr,0,sizeof(sendstr));
    memset(cmd,0,sizeof(cmd));
    memset(cmdstr,0,sizeof(cmdstr));

    strcpy(sendstr,send.c_str());
    ret = doTrade(sendstr,_cfgMap["signip"].c_str(),_cfgMap["signport"].ToInt(),result,cmd,cmdstr);
    if(ret)
    {
        MessageBox(Handle,"���׳���!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return ret;
    }
    if(result!=0)
    {
        String tmp;
        tmp.sprintf("���׳���[%s][%s]!\t",cmd,cmdstr);
        MessageBox(Handle,tmp.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
        return result;
    }
    ret = gdb.VQuery(512,"update bocctrl set checked=1 where exchno='%s' and area='%s' and date='%s' and session='%s'",exchno,g_sys_area.c_str(),date,session);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return ret;
    }

    return 0;
}
//---------------------------------------------------------------------------
/**
    ��ͣ����
    ������ֻ���ʱֹͣ���У����ݲ���ʧ
*/
int TFmDistMan::Pause()
{
    //TODO: Add your source code here
    _nDstState = ST_PAUSE;
    int ret = PauseDist( hDistUI );
    if( ret )
    {
        return ret;
    }

    pHDP->Stop();

 
    btnStart->Enabled=true;
    btnFinish->Enabled=true;
    btnNetCancel->Enabled=true;

    btnStart->Glyph->Assign( bmpStart );
    btnStart->Caption = TXT_GOON;

    UpdateStateBar();
    Application->ProcessMessages();

    return 0;
}
//---------------------------------------------------------------------------
void TFmDistMan::errMsg(int err)
{
    //TODO: Add your source code here
    if( err >= 0 ) return;
}
//---------------------------------------------------------------------------
/**
    �������ģʽ����
    ���Ը���Ϊһ����֣��������
*/
void __fastcall TFmDistMan::btnDstChangeClick(TObject *Sender)
{
    if(btnFinish->Enabled == true )
    {
        return;
    }
    if( _nDstState != ST_STOP )
    {
        return;
    }

    int ret = 0;
    switch( _nDstMode )
    {
        case MD_FIRST:
            ret = Init2Dst();
            break;
        case MD_SECOND:
            ret = Init1Dst();
            break;

        default:
            break;
    }
    if( ret )
    {
        errMsg( ret );
    }

    UpdateStateBar();
}
//---------------------------------------------------------------------------
/**
��������ֳ�ʼ������
*/
int TFmDistMan::Init2Dst()
{
    //TODO: Add your source code here
    btnDstChange->Glyph->Assign( bmp2 );
    pgDists->ActivePageIndex = 1;
    mu2Dst->Checked = true;
    _nDstMode = MD_SECOND;
    muEndorse->Checked=false;
    return 0;
}
//---------------------------------------------------------------------------
/**
��һ����ֳ�ʼ������
*/
int TFmDistMan::Init1Dst()
{
    //TODO: Add your source code here
    btnDstChange->Glyph->Assign( bmp1 );

    pgDists->ActivePageIndex = 0;
    mu1Dst->Checked = true;

    _nDstMode = MD_FIRST;

    return 0;
}
//---------------------------------------------------------------------------
/**
    ����ע���¼�����
    �ͷ���Դ
*/
void __fastcall TFmDistMan::FormDestroy(TObject *Sender)
{
    bmp1->FreeImage();
    bmp2->FreeImage();
    bmp3->FreeImage();
    bmpStart->FreeImage();
    bmpPause->FreeImage();

    ReleaseDistCtrl( hDistUI );

  //  pHDP->Disconnect();
   
    delete pHDP;
    pHDP = NULL;
    delete bmp1;
    delete bmp2;
    delete bmp3;
    delete bmpStart;
    delete bmpPause;   
}
//---------------------------------------------------------------------------
void __fastcall TFmDistMan::btnExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

/**
    ���ģʽ�л� ,�л���һ�����ģʽ
*/
void __fastcall TFmDistMan::mu1DstClick(TObject *Sender)
{
    if( _nDstMode == MD_FIRST ) return;
    btnDstChangeClick(0);
}
//---------------------------------------------------------------------------

/**
    ���ģʽ�л�,�л����������ģʽ
*/
void __fastcall TFmDistMan::mu2DstClick(TObject *Sender)
{
    if( _nDstMode == MD_SECOND ) return;
    btnDstChangeClick(0);
}
//---------------------------------------------------------------------------
/**
    ��������¼�����
*/
void __fastcall TFmDistMan::btnNetCancelClick(TObject *Sender)
{
    Screen->Cursor =crHourGlass;
    if( !btnNetCancel->Enabled )
    {
        Screen->Cursor = crDefault;
        return;
    }

    //ѯ���Ƿ����Ҫ����
    int ret = MessageBox(Handle,"ȷʵҪ������?\t","��ʾ",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
    if( ret != IDYES )
    {
        Screen->Cursor = crDefault;
        return;
    }

    Stop(true);

    ClearShow();
    txtSession->Enabled = true;
    m_bCtrlAmountLoaded = false;
    Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

/**
    һ�������ʾ���ܶԻ���
*/
int TFmDistMan::StartFirst()
{
    //TODO: Add your source code here
    if( _nDstState != ST_STOP || _reDistState != RDS_NONE ) return 0;
    ClearShow();

    //���������Ϣ
    if( SetupDlg->ckManCtrlAmount->Checked )
    {
        FmTotalMoney->txtMoneyKind->Caption = cbMoneyKind->Text;
        FmTotalMoney->mode = 0;
        int ret = FmTotalMoney->ShowModal();
        if( ret != mrOk ) return 1;
    }

    return 0;
}
//---------------------------------------------------------------------------

/**
    ���������ʾ���������Ϣ
*/
int TFmDistMan::StartSecond()
{
    //TODO: Add your source code here
    if( _nDstState != ST_STOP ) return 0;
    ClearShow();

    //ѡ��Ҫ��ֵ�����
    int ret = FmSelNet->ShowModal();
    if( ret != mrOk ) return 1;

    return 0;
}
//---------------------------------------------------------------------------

/**��
    ��ʾ�����������������Ϣ
*/
void TFmDistMan::ShowSetting()
{
    //TODO: Add your source code here
    if( _nDstState != ST_STOP ) return;
    memset(curPkgNo,0,sizeof(curPkgNo));
    switch( _nDstMode )
    {
        //һ�������ʾ������Ϣ,��������Ϣ
        case MD_FIRST:
            if(FmTotalMoney->txtTotalMoneyJF->Text.Length() > 0 ) txt1TotalMoneyJF->Caption = FmTotalMoney->txtTotalMoneyJF->Text;
            else txt1TotalMoneyJF->Caption = "0.00";
            if(FmTotalMoney->txtTotalMoneyDF->Text.Length() > 0 ) txt1TotalMoneyDF->Caption = FmTotalMoney->txtTotalMoneyDF->Text;
            else txt1TotalMoneyDF->Caption = "0.00";
            txt1PkgNo->Caption=FmTotalMoney->txtpkgno->Text;
            strcpy(curPkgNo,FmTotalMoney->txtpkgno->Text.c_str());
            CheckMoneyEqual();
            break;
        //���������ʾ��������Ϣ
        case MD_SECOND:
            txt2PkgNo->Caption = FmSelNet->cbNetNo->Text;
            strcpy(curPkgNo,FmSelNet->cbNetNo->Text.c_str());
            break;
        //���������ʾ��������Ϣ
        case MD_THIRD:
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------
/**
    ����
    �Խ�����г�ʼ��
*/
void TFmDistMan::ClearShow()
{
    //TODO: Add your source code here
    switch( _nDstMode )
    {
        case MD_FIRST:
            lst1PocketDetail->Items->Clear();
            lst1CheckDetail->Items->Clear();
            txt1PkgNo->Caption = "000000";
            txt1TotalCountJF->Caption = "0";
            txt1TotalCountDF->Caption = "0";

            txtPbcCountJF->Caption = "0";
            txtPbcCountDF->Caption = "0";
            txt1SortCount->Caption = "0";
            txt1TotalMoneyJF->Caption = "0.00";
            txt1TotalMoneyDF->Caption = "0.00";
            txt1TotalSumJF->Caption = "0.00";
            txt1TotalSumDF->Caption = "0.00";
            txtJFC->Caption = "0.00";
            txtDFC->Caption = "0.00";
            break;
        case MD_SECOND:
            lst2PocketDetail->Items->Clear();
            lst2CheckDetail->Items->Clear();
            txt2dTotalCount->Caption = "0";
            txt2pTotalCount->Caption = "0";
            txt2SortCount->Caption = "0";
            txt2PkgNo->Caption = "000000";
            break;
        case MD_THIRD:
            lst3PocketDetail->Items->Clear();
            lst3CheckDetail->Items->Clear();
            txt3dTotalCount->Caption = "0";
            txt3pTotalCount->Caption = "0";
            txt3SortCount->Caption = "0";
            txt3PkgNo->Caption = "000000";
            break;
        default:
            break;
    }
    signJF->Caption = "=";
    signDF->Caption = "=";
}
//---------------------------------------------------------------------------
void __fastcall TFmDistMan::OnDist(TMessage &Message)
{
  
}
//---------------------------------------------------------------------------
/**
    �����������,���һ��Ʊ�ݺ�,�����ݽ��д���
*/
void TFmDistMan::GetChkData()
{
    //TODO: Add your source code here
    //��ȡ��ǰ���Ʊ������
    pData = GetOneData( hDistUI ); /////return one Line;
    switch( _nDstMode )
    {
        //�����ۼ�
        case MD_FIRST:
            Treat1Data();
            break;
        //�����ۼ�
        case MD_SECOND:
            Treat2Data();
            break;
        case MD_THIRD:
            Treat3Data();
            break;
        default:
            break;
    }
}

void TFmDistMan::LoadCtrlAmount()
{
    if( m_bCtrlAmountLoaded ) return;


    if( !SetupDlg->ckManCtrlAmount->Checked )
    {
        txt1PkgNo->Caption=pData->exchno;
        strcpy(curPkgNo ,pData->exchno);
        int ret =  gdb.VQuery(512,"select dramount,cramount,drcount,crcount from bocctrl where exchno='%s' and date='%s' and session='%s'",txt1PkgNo->Caption.c_str(),date,session );
        if(ret)
        {
            MessageBox(Handle,"��ѯ���ݿ����!\t","����",MB_OK|MB_ICONERROR);
            return ;
        }
        MyRes res = gdb.Store();
        MyRow row = res.Fetch();
        if(row!=NULL)
        {
            txt1TotalMoneyJF->Caption = FloatToThousandStr(atof(row[0]));
            txt1TotalMoneyDF->Caption = FloatToThousandStr(atof(row[1]));
            txtPbcCountJF->Caption = row[2];
            txtPbcCountDF->Caption = row[3];
        }
        else
        {
            return ;
        }

        cmpMoney( txt1TotalMoneyJF->Caption,txt1TotalSumJF->Caption,signJF,txtJFC);
        cmpMoney( txt1TotalMoneyDF->Caption,txt1TotalSumDF->Caption,signDF,txtDFC);
    }
    m_bCtrlAmountLoaded = true;
}
//---------------------------------------------------------------------------
/**
    һ�����һ��Ʊ�ݺ�,������ݴ���
*/
void TFmDistMan::Treat1Data()
{
    //����ܶ�仯

    LoadCtrlAmount();
    if( IsJFVoucher() )
    {
        addTotalSum( txt1TotalSumJF );
        cmpMoney( txt1TotalMoneyJF->Caption,txt1TotalSumJF->Caption,signJF,txtJFC);
        txt1TotalCountJF->Caption = IntToStr(txt1TotalCountJF->Caption.ToInt()+1);
    }
    else
    {
        addTotalSum( txt1TotalSumDF );
        cmpMoney( txt1TotalMoneyDF->Caption,txt1TotalSumDF->Caption,signDF,txtDFC);
        txt1TotalCountDF->Caption = IntToStr(txt1TotalCountDF->Caption.ToInt()+1);
    }

    //�ܱ���
    txt1SortCount->Caption = IntToStr(txt1SortCount->Caption.ToInt()+1);

    //�ڴ���ϸ˵��,�����ۼ�
    TListItem* pItem = lst1PocketDetail->FindCaption(0, pData->PockCode, 0, 0, 1 );
    //����ҵ��˿ڴ���,��,ֱ���ۼ�
    if( pItem )
    {
        pItem->SubItems->Strings[POC1_COL_CNT] = IntToStr(pItem->SubItems->Strings[POC1_COL_CNT].ToInt() + 1);
    }
    //���û���ҵ�,�����һ��
    else
    {
        pItem = getRightItem(lst1PocketDetail,pData->PockNum);                  // sort the items use pocketnum
        pItem->Data = (void *)pData->PockNum;
        pItem->Caption = pData->PockCode;
        pItem->SubItems->Strings[POC1_COL_DIS] = pData->PockText;
        pItem->SubItems->Strings[POC1_COL_CNT] = 1;
        pItem->SubItems->Strings[POC1_COL_DOC] = txtDeCr[pData->cdcode];
    }

    //���˵��
    pItem = lst1CheckDetail->FindCaption(0, pData->VchKindCod, 0, 0, 1 );
    //����ҵ���ƾ֤����,������+1,����ۼ�
    if( pItem )
    {
        pItem->SubItems->Strings[VCH1_COL_CNT] = IntToStr(pItem->SubItems->Strings[VCH1_COL_CNT].ToInt() + 1);
        String sTotal = RemoveThousandChar(pItem->SubItems->Strings[VCH1_COL_POS]);
        String sMoney = pData->amount;
        sMoney.Insert(".", sMoney.Length() - 1 );
        pItem->SubItems->Strings[VCH1_COL_POS] = FloatToThousandStr( sTotal.ToDouble() + sMoney.ToDouble() );
    }
    //���û���ҵ���ƾ֤����,�����һ��
    else
    {
        pItem = getRightItem(lst1CheckDetail,pData->PockNum);                   // sort the items use pocketnum
        pItem->Data = (void *)pData->PockNum;
        pItem->Caption = pData->VchKindCod;
        pItem->SubItems->Strings[VCH1_COL_DIS] = pData->VchKindTxt;
        pItem->SubItems->Strings[VCH1_COL_CNT] = 1;
        pItem->SubItems->Strings[VCH1_COL_DOC] = txtDeCr[pData->cdcode];
        String sMoney = pData->amount;
        sMoney.Insert(".", sMoney.Length() - 1 );
        pItem->SubItems->Strings[VCH1_COL_POS] = FloatToThousandStr(sMoney.ToDouble());
    }
}
//---------------------------------------------------------------------------
/**
    �������һ��Ʊ�ݺ�,������ݴ���
*/
void TFmDistMan::Treat2Data()
{
    //TODO: Add your source code here
    //�����ۼ�
    txt2dTotalCount->Caption = IntToStr(txt2dTotalCount->Caption.ToInt()+1);
    txt2SortCount->Caption = IntToStr(txt2SortCount->Caption.ToInt()+1);

    //���ڴ�����
    TListItem* pItem = lst2PocketDetail->FindCaption(0, pData->PockCode, 0, 0, 1 );
    //�ҵ��˸ÿڴ�,ֱ���ۼӽ��ͱ���
    if( pItem )
    {
       pItem->SubItems->Strings[POC2_COL_CNT2] = IntToStr(pItem->SubItems->Strings[POC2_COL_CNT2].ToInt() + 1);
        String sTotal = RemoveThousandChar(pItem->SubItems->Strings[POC2_COL_CNT2+1]);
        String sMoney = pData->amount;
        sMoney.Insert(".", sMoney.Length() - 1 );
        pItem->SubItems->Strings[POC2_COL_CNT2+1] = FloatToThousandStr( sTotal.ToDouble() + sMoney.ToDouble() );
    }
    //û���ҵ�,ֱ�Ӳ���һ��
    else
    {
        pItem = getRightItem(lst2PocketDetail,pData->PockNum);                  // sort the items use pocketnum
        pItem->Data = (void *)pData->PockNum;
        pItem->Caption = pData->PockCode;
        pItem->SubItems->Strings[POC2_COL_DIS] = PocketLabel[pData->PockNum-1]; // pData->PockText;
        pItem->SubItems->Strings[POC2_COL_CNT1] = PocketCount[pData->PockNum-1];
        pItem->SubItems->Strings[POC2_COL_CNT2] = 1;

        String sMoney = pData->amount;
        sMoney.Insert(".", sMoney.Length() - 1 );
        pItem->SubItems->Strings[POC2_COL_CNT2+1] = FloatToThousandStr(sMoney.ToDouble());
    }

    //��ƾ֤�������
    pItem = lst2CheckDetail->FindCaption(0, pData->VchKindCod, 0, 0, 1 );
    //����ҵ�,�ۼӽ��ͱ���
    if( pItem )
    {
        pItem->SubItems->Strings[VCH2_COL_CNT2] = IntToStr(pItem->SubItems->Strings[VCH2_COL_CNT2].ToInt() + 1);

        String sTotal = RemoveThousandChar(pItem->SubItems->Strings[VCH2_COL_CNT2+1]);
        String sMoney = pData->amount;
        sMoney.Insert(".", sMoney.Length() - 1 );
        pItem->SubItems->Strings[VCH2_COL_CNT2+1] = FloatToThousandStr( sTotal.ToDouble() + sMoney.ToDouble() );
    }
    //û���ҵ�,ֱ�Ӳ���һ��
    else
    {
        pItem = getRightItem(lst2CheckDetail,pData->PockNum);                   // sort the items use pocketnum
        pItem->Data = (void *)pData->PockNum;
        pItem->Caption = pData->VchKindCod;
        pItem->SubItems->Strings[VCH2_COL_DIS] = pData->VchKindTxt;

        pItem->SubItems->Strings[POC2_COL_CNT2] = 1;
        if(strcmp(pData->pbcvt,"00") == 0)
        {
            pItem->SubItems->Strings[VCH2_COL_CNT1] = PocketTpCount[0];
            String sMoney = pData->amount;
            sMoney.Insert(".", sMoney.Length() - 1 );
            pItem->SubItems->Strings[POC2_COL_CNT2+1] = FloatToThousandStr(sMoney.ToDouble());
        }
        else
        {
            pItem->SubItems->Strings[VCH2_COL_CNT1] = PocketTpCount[1];
            String sMoney = pData->amount;
            sMoney.Insert(".", sMoney.Length() - 1 );
            pItem->SubItems->Strings[POC2_COL_CNT2+1] = FloatToThousandStr(sMoney.ToDouble());
        }
    }
    lst2CheckDetail->Invalidate();
}
//---------------------------------------------------------------------------

/**
    �����水ť������Ӧ����,ʵ�ֿ�ݼ�
*/
void __fastcall TFmDistMan::FormKeyPress(TObject *Sender, char &Key)
{
    switch( Key )
    {
        //��ʼ
        case 'S':
        case 's':
            if(btnStart->Enabled==false)  return;
            btnStartClick(0);
            break;
        //���
        case 'F':
        case 'f':
            if(btnFinish->Enabled==false) return;
            btnFinishClick(0);
            break;
        //�˳�
        case 'X':
        case 'x':
            if(btnExit->Enabled==false)   return;
            btnExitClick(0);
            break;
        //ȡ��
        case 'P':
        case 'p':
            if(btnNetCancel->Enabled==false) return;
            btnNetCancelClick(0);
            break;
        //��Ʊ����
        case 'R':
        case 'r':
            if(btnStart->Enabled==false)     return;
            btnBadCleanClick(0);
            break;
        //����
        case 'Q':
        case 'q':
            if(btnStart->Enabled==false)     return;
            btnHistSearchClick(0);
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------
/**
    �����˳���Ӧ����
*/
void __fastcall TFmDistMan::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    //�����ֻ�δֹͣ,�����˳�
    if( _nDstState != ST_STOP )
    {
        MessageBox(Handle,"���δֹͣ,�������˳�����!","��ʾ",MB_OK|MB_ICONINFORMATION);

        Action = caNone;
        return;
    }

    int ret = MessageBox( Handle, "ȷʵҪ�˳���ֳ�����?","��ʾ",MB_OKCANCEL|MB_ICONQUESTION);
    if( ret != IDOK )
    {
        Action = caNone;
        return;
    }
  
  
}
//---------------------------------------------------------------------------
/**
    ������ʾ�¼�����
*/
void __fastcall TFmDistMan::FormShow(TObject *Sender)
{
    //��½���ڵ���

    int ret = FormLogin->ShowModal();
    if(ret != mrOk )
    {
        Application->Terminate();
    }

    else
    {
        FmWait = new TFmWait(Application);
        FmWait->txtMessage->Caption = "  ���ڳ�ʼ��,���Ժ�......";
        FmWait->Show();
        Application->ProcessMessages();
        ZSingleInstance g_single;
        if(g_single.init("Hantlec")!=0||g_single.init("ScanA4")!=0)      //DistManager��һ̨������ֻ������һ��ʵ��
        {
	        MessageBox(Handle,"ÿ̨��ֻ�����������һ����ֿ��Ƴ���","����",MB_OK|MB_ICONWARNING);
            Application->Terminate();
        }

        TDateTime tdt;
        tdt = tdt.CurrentDateTime();
        InsertLog(tdt.DateTimeString().c_str());

        InsertLog("-------");

        //��ֻ���ʼ��
        Application->ProcessMessages();

        hDistUI = InitDistCtrl( Handle );

        if( !hDistUI )
        {
            MessageBox(Handle,"��ʼ����ֿ��ƿ����","��ʾ",MB_OK|MB_ICONINFORMATION);
            Application->Terminate();
        }

        ret = pHDP->Init();
        if( ret != 0 )
        {
            MessageBox(Handle,"װ����ֻ���̬�����","��ʾ",MB_OK|MB_ICONINFORMATION);
            Application->Terminate();
        }

        ret = pHDP->Connect();
        if(ret!=0)
        {
            switch(ret)
            {
                case  -1: InsertLog("Ocr Open Failed!");      break;
                case  -2: InsertLog("COM Open Failed!");      break;
                case  -3: InsertLog("COM Set  Failed!");      break;
                default : InsertLog("Sort Initilize Failed"); break;
            }
            MessageBox(Handle,"��ֻ���ʼ��ʧ��","����",MB_OK|MB_ICONERROR);
            Application->Terminate();
        }
    
        InsertLog("Sort Initilize Succeed");
        Cursor = crDefault;
        UpdateStateBar();

        memset(date,0,sizeof(date));
        memset(session,0,sizeof(session));

        //��ȡ����,����
        ret = GetDS("date",date);
        if(ret)
        {
            MessageBox(Handle,"��ȡ���ڳ���","��ʾ",MB_OK|MB_ICONERROR);
            return ;
        }
        ret = GetDS("session",session);
        if(ret)
        {
            MessageBox(Handle,"��ȡ���γ���","��ʾ",MB_OK|MB_ICONERROR);
            FmWait->Hide();
            delete FmWait; FmWait = NULL;
            return ;
        }
        txtSession->Items->Clear();
        txtSession->Items->Add(session);
        txtSession->ItemIndex = 0;

        LoadCFG();
        ZDateTime zdate;
        zdate.SetBankDateStr(date);
        txtDate->Date = StrToDate(zdate.DateString().c_str());
        DisplayDateSession();

        FmWait->Hide();
        delete FmWait;
        FmWait = NULL;

 

        char dir[128];
        memset(dir,0,sizeof(dir));
        sprintf(dir,"%s",Distval.path);
	    if( _access(dir,0) != 0 )
        {
		    _mkdir(dir);
	    }

        memset(dir,0,sizeof(dir));
        sprintf(dir,"%s\\%s",Distval.path,date);
	    if( _access(dir,0) != 0 )
        {
		    _mkdir(dir);
	    }

        memset(dir,0,sizeof(dir));
        sprintf(dir,"%s\\%s\\%02d",Distval.path,date,atoi(session));
	    if( _access(dir,0) != 0 )
        {
	    	_mkdir(dir);
	    }

        memset(dir,0,sizeof(dir));
        sprintf(dir,"%s\\%s\\%02d\\dists",Distval.path,date,atoi(session));
	    if( _access(dir,0) != 0 )
        {
		    _mkdir(dir);
	    }
        //ƾ֤�����Ӧ��ʼ��
        initVTList();

        pData = new DISTDATA;
        Screen->Cursor = crDefault;
        Application->ProcessMessages();

        FmDistMan->Caption  = AnsiString("����������ֿ���")+"    ��½��Ա["+g_sys_opt+"]["+g_sys_name+"]";
   
        String originPath = GetCurrentDir();
    }
}
//---------------------------------------------------------------------------

/**
    ���ؿڴ���
*/
int TFmDistMan::GetPocketNum()
{
    //TODO: Add your source code here
    return GetPocketNo(hDistUI);
}
//---------------------------------------------------------------------------

/**
    �����ܶ�
*/
void TFmDistMan::addTotalSum(TLabel* totalSum)
{
    //TODO: Add your source code here
    String sTotal = RemoveThousandChar(totalSum->Caption);
    String sMoney = pData->amount;
    sMoney.Insert(".", sMoney.Length() - 1 );
    totalSum->Caption = FloatToThousandStr( sTotal.ToDouble() + sMoney.ToDouble() );
}
//---------------------------------------------------------------------------

/**
    �жϽ����ϵ
*/
bool TFmDistMan::IsJFVoucher()
{
    //TODO: Add your source code here
    return (pData->cdcode == DC_DEBIT );
}
//---------------------------------------------------------------------------

/**
    ���ر���
*/
int TFmDistMan::getCurrency(int index)
{
    //TODO: Add your source code here
    memset(_Currency,0,sizeof(_Currency));
    switch( index )
    {
        case 0:                 // RMB
            strcpy(_Currency,"RMB");
            return CS_RMB;
        case 1:                 // US$
            strcpy(_Currency,"USD");
            return CS_USD;
        case 2:                 // HK$
            strcpy(_Currency,"HKD");
            return CS_HKD;
        case 3:                 // EUR
            strcpy(_Currency,"EUR");
            return CS_EUR;
        case 4:                 // SGD
            strcpy(_Currency,"SGD");
            return CS_SGD;
        case 5:                 // JPY
            strcpy(_Currency,"JPY");
            return CS_JPY;
        default:
            return CS_UNKNOWN;
    }
}
//---------------------------------------------------------------------------

/**
    �����������
*/
void TFmDistMan::AddSortCount()
{
    //TODO: Add your source code here
    if( _nDstMode == MD_FIRST )
        txt1SortCount->Caption = IntToStr(txt1SortCount->Caption.ToInt()+1);
    else
        txt2SortCount->Caption = IntToStr(txt2SortCount->Caption.ToInt()+1);
}
//---------------------------------------------------------------------------

/**
    �����ض��ļ�¼
*/
TListItem * TFmDistMan::getRightItem(TListView * list, int data)
{
    //TODO: Add your source code here
    TListItem *pItem;
    for( int i = 0; i < list->Items->Count ; i++ )
    {
        if( data < (int)list->Items->Item[i]->Data )
        {
            pItem = list->Items->Insert(i);
            for( int j = 0; j < list->Columns->Count -1; j++ )
            {
                pItem->SubItems->Add("");
            }
            return pItem;
        }
    }
    pItem = list->Items->Add();
    for( int j = 0; j < list->Columns->Count -1; j++ )
    {
        pItem->SubItems->Add("");
    }
    return pItem;
}
//---------------------------------------------------------------------------

/**
    ����ģʽ������ģʽ�л�
*/

/**
    ����һ�������ֻ��ڴ���ϸ��ɫ
*/
void __fastcall TFmDistMan::lst1PocketDetailCustomDrawItem(
      TCustomListView *Sender, TListItem *Item, TCustomDrawState State,
      bool &DefaultDraw)
{
    if(Item->SubItems->Strings[POC1_COL_DOC] == txtDeCr[DC_DEBIT])
        Sender->Canvas->Brush->Color = panDebit->Color;
    else
        Sender->Canvas->Brush->Color = panCredit->Color;

    Sender->Canvas->FillRect( Item->DisplayRect(drBounds));
}
//---------------------------------------------------------------------------

/**
    ����һ��������֧Ʊ������ϸ��ɫ
*/
void __fastcall TFmDistMan::lst1CheckDetailCustomDrawItem(
      TCustomListView *Sender, TListItem *Item, TCustomDrawState State,
      bool &DefaultDraw)
{
    if(Item->SubItems->Strings[VCH1_COL_DOC] == txtDeCr[DC_DEBIT])
        Sender->Canvas->Brush->Color = panDebit->Color;
    else
        Sender->Canvas->Brush->Color = panCredit->Color;

    Sender->Canvas->FillRect( Item->DisplayRect(drBounds));
}
//---------------------------------------------------------------------------
/**
    ����ģʽ�л��˵������Ӧ����
*/
void __fastcall TFmDistMan::muFastModeClick(TObject *Sender)
{
    //��������������,�������˳�
    if( _nDstState == ST_RUN ) return;
    if( btnStart->Enabled==false) return;

    muFastMode->Checked = !muFastMode->Checked;

    bool endorse = ((_nDstMode == MD_SECOND) && muEndorse->Checked);
    txtEngageMode->Caption = txtEgg[(int)muFastMode->Checked];

    //����ǿ���,�����������
    if(muFastMode->Checked == true)
    {
        chManCheckResult->Checked = false;
        chManCheckResult->Enabled = false;
    }
    //���������,������ɿ���
    else
    {
        chManCheckResult->Checked = true;
        chManCheckResult->Enabled = true;
    }

    UpdateStateBar();
}
//---------------------------------------------------------------------------
/**
    �ֹ���¼�����¼�����
*/

/**
    ����״̬��
*/
void TFmDistMan::UpdateStateBar()
{
    //TODO: Add your source code here
    stbDistState->Panels->Items[STB_MODE]->Text = txtEngageMode->Caption;
    switch(_nDstState)
    {
        case ST_STOP:
            stbDistState->Panels->Items[STB_STATE]->Text = "�ȴ�";
            break;
        case ST_RUN:
            stbDistState->Panels->Items[STB_STATE]->Text = "������";
            break;
        case ST_PAUSE:
            stbDistState->Panels->Items[STB_STATE]->Text = "��ͣ";
            break;
    }
    stbDistState->Panels->Items[3]->Text = login.clkno;
}
//---------------------------------------------------------------------------
/**
    ���������ȽϽ��Ĵ�С
*/
int TFmDistMan::cmpMoney(String sum1, String sum2, TLabel * signimg,TLabel * diff)
{
    //TODO: Add your source code here
    String sTotal1, sTotal2,s1,s2;
    s1 = sum1;
    s2 = sum2;
    sTotal1 = RemoveThousandChar(s1);
    sTotal2 = RemoveThousandChar(s2);


    double dsum1,dsum2,dsum;

    dsum1 = sTotal1.ToDouble();
    dsum2 = sTotal2.ToDouble();
    dsum  = dsum1 - dsum2;
    
    diff->Caption =  FloatToThousandStr(dsum);

    if(sTotal1==sTotal2)
    {
        signimg->Caption = "=";
        return 0;
    }
    else if( sTotal1.Length() > sTotal2.Length() )
    {
        signimg->Caption = ">";
        return 1;
    }
    else
    {
        if(sTotal1>sTotal2)
        {
            signimg->Caption = ">";
            return 1;
        }
        else
        {
            signimg->Caption = "<";
            return -1;
        }
    }
}
//---------------------------------------------------------------------------

/**
    �������һ���������ܽ��ͻ��ܽ���Ƿ�һ��
*/
void TFmDistMan::CheckMoneyEqual()
{
    //TODO: Add your source code here
    if( _nDstMode != MD_FIRST ) return;

    int ret1 = cmpMoney( txt1TotalMoneyJF->Caption, txt1TotalSumJF->Caption, signJF,txtJFC );
    int ret2 = cmpMoney( txt1TotalMoneyDF->Caption, txt1TotalSumDF->Caption, signDF,txtDFC );

    if( ret1 != 0 || ret2 != 0)
    {
        return;
    }
}
//---------------------------------------------------------------------------

/**
    ��ʼ�����ؼ�ֵ
*/
void TFmDistMan::ClearFirst4RD()
{
    lst1PocketDetail->Items->Clear();
    lst1CheckDetail->Items->Clear();
    txt1TotalCountJF->Caption = "0";
    txt1TotalCountDF->Caption = "0";
    txt1SortCount->Caption = "0";
    txt1TotalSumJF->Caption = "0.00";
    txt1TotalSumDF->Caption = "0.00";
    _nDstState = ST_STOP;
}
//---------------------------------------------------------------------------
/**
    �����ܽ���¼�����
*/
void __fastcall TFmDistMan::lbSetTotalMoneyClick(TObject *Sender)
{
//    if( SetupDlg->ckManCtrlAmount->Checked == false) return ;    
    if(btnStart->Caption == TXT_START ) return ;
    if( _nDstState == ST_RUN || _reDistState == RDS_SECOND ) return;

    FmTotalMoney->txtMoneyKind->Caption = cbMoneyKind->Text;

 
    FmTotalMoney->mode = 1;
    int ret = FmTotalMoney->ShowModal();
    if( ret != mrOk )
    {
        return;
    }

    if(FmTotalMoney->txtTotalMoneyJF->Text.Length() > 0 ) txt1TotalMoneyJF->Caption = FmTotalMoney->txtTotalMoneyJF->Text;
    else txt1TotalMoneyJF->Caption = "0.00";
    if(FmTotalMoney->txtTotalMoneyDF->Text.Length() > 0 ) txt1TotalMoneyDF->Caption = FmTotalMoney->txtTotalMoneyDF->Text;
    else txt1TotalMoneyDF->Caption = "0.00";
    CheckMoneyEqual();

    return;
}
//---------------------------------------------------------------------------

/**
    ��ʾ������Ϣ
*/
void TFmDistMan::InsertLog(char *txtLog)
{
    //TODO: Add your source code here
    if( txtLog == NULL ) return;
    txtSorterState->Lines->Add(txtLog);
}
//---------------------------------------------------------------------------

/**
    ������ģʽ�л�
*/
void TFmDistMan::ToFastMode()
{
    //TODO: Add your source code here
    if( _nDstState != ST_STOP ) return;
    if( muFastMode->Checked ) return;

    if( !muDefaultFastMode->Checked )
    {
        int ret = MessageBox(Handle,"�Ƿ��л�������ģʽ", "", MB_YESNO|MB_ICONQUESTION);
        if( ret != IDYES ) return;
    }

    muFastModeClick(0);
}
//---------------------------------------------------------------------------
/**
    ���µ�¼�¼�����
*/
void __fastcall TFmDistMan::muReLoginClick(TObject *Sender)
{
    if( _nDstState != ST_STOP ) return;
}
//---------------------------------------------------------------------------
void TFmDistMan::DisplayDateSession()
{
    //TODO: Add your source code here
  /*  PocketLabel[0] = "���˳ɹ�";
    PocketLabel[1] = "����ʧ��";
    PocketLabel[2] = "��ӡ����";
    PocketLabel[3] = "�˹�����";
    PocketLabel[4] = "Ʊ�治��";
    PocketLabel[5] = "���֧Ʊ";
    PocketLabel[6] = "����";
    PocketLabel[7] = "�ͻ�������";
    PocketLabel[8] = "����ͻ�";  */
}
//---------------------------------------------------------------------------

/**
    ����
*/
void __fastcall TFmDistMan::btnTallyCheckClick(TObject *Sender)
{
  
    btnTallyCheck->Enabled = false;
    N7->Enabled = false;


}
//---------------------------------------------------------------------------
/**
    ��Ʊ�����¼�����
*/
void __fastcall TFmDistMan::btnBadCleanClick(TObject *Sender)
{
    if(btnStart->Caption == TXT_START ) return ;
    TBadCleanDlg *dlg = new TBadCleanDlg(this);
    if(dlg==NULL)
    {
        MessageBox(Handle,"���ɴ�Ʊ������ʧ��!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }

    //��ʾ�����������
    dlg->m_distUI = hDistUI;
    dlg->count = txt1SortCount->Caption.ToInt();
    dlg->countjf = txt1TotalCountJF->Caption.ToInt();
    dlg->countdf = txt1TotalCountDF->Caption.ToInt();
    dlg->ShowModal();

    //���¼���ܶ�
    CheckTotalAmount();
    delete dlg;
}
//---------------------------------------------------------------------------

/**
    ��ʷ���ݲ�ѯ��ť�����Ӧ����
*/
void __fastcall TFmDistMan::btnHistSearchClick(TObject *Sender)
{
    THistQueryDlg* dlg = new THistQueryDlg(this);
    if(dlg==NULL)
    {
        MessageBox(Handle,"������ʷ���ݲ�ѯ����ʧ��!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    dlg->ShowModal();
    delete dlg;
}
//---------------------------------------------------------------------------

/**
    ��ʼ��
*/
void __fastcall TFmDistMan::txtDateSesClick(TObject *Sender)
{
    DisplayDateSession();
}
//---------------------------------------------------------------------------

/**
    ���������ֿڴ���ϸ,���Ƽ�¼��ɫ
*/
void __fastcall TFmDistMan::lst2PocketDetailCustomDrawItem(
      TCustomListView *Sender, TListItem *Item, TCustomDrawState State,
      bool &DefaultDraw)
{
    int poc = (int)Item->Data-1;
    if( poc < 0 ) return;
    Sender->Canvas->Brush->Color = (TColor)Dist2Color[poc];
    Sender->Canvas->FillRect( Item->DisplayRect(drBounds));
}
//---------------------------------------------------------------------------

/**
    �ж������Ƿ��Ѿ���ֹ�
*/
int TFmDistMan::IsNetStored(String netno)
{
    int ret = gdb.VQuery(512,"select count(*) from dists where exchno='%s' and date='%s' and session='%s'", txt1PkgNo->Caption.c_str(),date,session);
    if(ret)
    {
        MessageBox(Handle,"��ѯ����\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return ret;
    }
    MyRes res = gdb.Store();
    MyRow row = res.Fetch();
    if(atoi(row[0])>0)
    {
        return 1;
    }
    return 0;
}
//---------------------------------------------------------------------------

/**
    ����ܽ����ܱ���
*/
int TFmDistMan::CheckTotalAmount()
{
    double dramount,cramount;
    int drc,crc;
    dramount = cramount = 0.0;  //���
    drc = crc = 0;              //����

    CalTotalAmount(hDistUI,&dramount,&drc,&cramount,&crc);

    txt1TotalSumDF->Caption = FloatToThousandStr(dramount);
    txt1TotalCountDF->Caption = drc;

    txt1TotalSumJF->Caption = FloatToThousandStr(cramount);

    txt1TotalCountJF->Caption = crc;
    txt1SortCount->Caption = drc+crc;

    //�жϽ跽�ܽ��,�ܱ���
    cmpMoney( txt1TotalMoneyJF->Caption,txt1TotalSumJF->Caption,signJF,txtJFC);

    //�жϴ����ܽ��,�ܱ���
    cmpMoney( txt1TotalMoneyDF->Caption,txt1TotalSumDF->Caption,signDF,txtDFC);
    if( signJF->Caption != "=" || signDF->Caption != "=" )
    {
        return -1;
    }

    return 0;
}
//---------------------------------------------------------------------------

/**
    ���ò˵������Ӧ����
*/
void __fastcall TFmDistMan::muSetupClick(TObject *Sender)
{
    //��ȡ��������
    LoadCFG();
    if( SetupDlg->ShowModal() != mrOk ) return;

    //����������
    strcpy(Distval.prefix,SetupDlg->txtPrefix->Text.Trim().c_str());
    strcpy(Distval.path ,SetupDlg->txtPath->Text.Trim().c_str());
    Distval.calx = SetupDlg->txtCalX->Text.ToDouble();
    Distval.caly = SetupDlg->txtCalY->Text.ToDouble();
    Distval.dpi  = SetupDlg->txtDPI->Text.ToInt();
    if(SetupDlg->ckOcrRecognize->Checked == true)
        Distval.haveOCR = 0;
    else
        Distval.haveOCR = 1;

    if(SetupDlg->ckManCtrlAmount->Checked == true)
        Distval.inPutAmount = 0;
    else
        Distval.inPutAmount = 1;

    if(SetupDlg->chkInput->Checked == true)
        Distval.inputCheck = 0;
    else
        Distval.inputCheck = 1;

    if(SetupDlg->chkNewHDP->Checked == true)
        Distval.hdpType = 0;
    else
        Distval.hdpType = 1;
        
    Distval.vchLen  = SetupDlg->txtVchLen->Text.ToInt();
    Distval.exnoLen = SetupDlg->txtExnoLen->Text.ToInt();
    Distval.accLen  = SetupDlg->txtAccLen->Text.ToInt();
    Distval.vchtpLen = SetupDlg->txtVchtpLen->Text.ToInt();
    Distval.amountLen = SetupDlg->txtAmountLen->Text.ToInt();

    strcpy(Distval.vch1_interval,SetupDlg->txtVchInterval->Text.Trim().c_str());

    strcpy(Distval.exno1_interval,SetupDlg->txtExnoInterval1->Text.Trim().c_str());
    strcpy(Distval.exno2_interval,SetupDlg->txtExnoInterval2->Text.Trim().c_str());

    strcpy(Distval.accno1_interval,SetupDlg->txtAccInterval1->Text.Trim().c_str());
    strcpy(Distval.accno2_interval,SetupDlg->txtAccInterval2->Text.Trim().c_str());

    strcpy(Distval.vchtp1_interval,SetupDlg->txtVchtpInterval1->Text.Trim().c_str());
    strcpy(Distval.vchtp2_interval,SetupDlg->txtVchtpInterval2->Text.Trim().c_str());

    strcpy(Distval.amount1_interval,SetupDlg->txtAmountInterval1->Text.Trim().c_str());
    strcpy(Distval.amount2_interval,SetupDlg->txtAmountInterval2->Text.Trim().c_str());

    SaveCFG();

    char dir[128];
    memset(dir,0,sizeof(dir));
    sprintf(dir,"%s",Distval.path);

    //���ָ��·��������,�򴴽�
	if( _access(dir,0) != 0  )
    {
		_mkdir(dir);
	}
}
//---------------------------------------------------------------------------

/**
    ��Ʊ�˵������Ӧ����
*/
void __fastcall TFmDistMan::muFlushClick(TObject *Sender)
{
    //�����Ʊ
    pHDP->Purge();

    //���ð�ť״̬
    if(btnStart->Caption !=TXT_START ) btnStart->Caption = TXT_GOON;
    btnStart->Enabled=true;

}
//---------------------------------------------------------------------------

/**
    �������Ӳ˵������Ӧ����
*/
void __fastcall TFmDistMan::muRelinkClick(TObject *Sender)
{
    //�Ͽ���ֻ�

    pHDP->Disconnect();
    //����������ֻ�
    pHDP->Connect();
    if( muFastMode->Checked ) return;
    //���ÿ������ģʽ
    muFastModeClick(0);
}
//---------------------------------------------------------------------------

/**
    ��ֻ�������ݴ�����,�ú�������ֻ���ʼ��ʱ������ֻ�����ģ��
*/
int TFmDistMan::OnHDPCall(int num)
{
    //TODO: Add your source code here
    char pfname[128];
    char prname[128];
    char sfname[128];
    char srname[128];
    memset(pData->codeline,0,sizeof(pData->codeline));
    memset(pData->codeacc,0,sizeof(pData->codeacc));
    memset(pData->ocr,0,sizeof(pData->ocr));
    memset(pData->vchno,0,sizeof(pData->vchno));
    memset(pData->amount,0,sizeof(pData->amount));
    memset(pData->exchno,0,sizeof(pData->exchno));
    memset(pData->vchtype,0,sizeof(pData->vchtype));
    memset(pData->fname,0,sizeof(pData->fname));
    memset(pData->rname,0,sizeof(pData->rname));
    memset(sfname,0,sizeof(sfname));
    memset(srname,0,sizeof(srname));
    memset(pData->ip,0,sizeof(pData->ip));
    pData->modified  =0;

 
    scanid = scanid+1;

    pData->fcount = scanid;


    //һ����ֻ������������Ҫ����ͼ��
    if(_nDstMode==MD_FIRST || _nDstMode == MD_THIRD)
    {
        strcpy(pData->ip,Distval.ip);
        pData->calx = Distval.calx;
        pData->caly = Distval.caly;
        pData->dpi =  Distval.dpi;
        strcpy(pData->prefix,Distval.prefix);

        sprintf(sfname,"%cA-%08d-%04d.bmp",Distval.prefix[0],curid,pData->fcount);
        sprintf(srname,"%cB-%08d-%04d.bmp",Distval.prefix[0],curid,pData->fcount);

        sprintf(pfname,"%s\\%s\\%02d\\dists\\%s",Distval.path,date,atoi(session),sfname);
        sprintf(prname,"%s\\%s\\%02d\\dists\\%s",Distval.path,date,atoi(session),srname);
    }
    else
    {
        sprintf(pfname,"%s\\%s\\%02d\\dists\\front.bmp",Distval.path,date,atoi(session));
        sprintf(prname,"%s\\%s\\%02d\\dists\\rear.bmp",Distval.path,date,atoi(session));
    }
    scancount = num;

    //��ȡMICR ,OCR
    int ret = pHDP->GetMicr(num,pfname,prname);
    if(ret==999)  return 999;
    if( ret != 0  )
    {
        if(muFastMode->Checked== true)
        {
            _unlink(pfname);
            _unlink(prname);
            return pHDP->SetPocket(ERROR_POCKET);
        }
    }

    //�ָ����
    ret = pHDP->DecodeMicr(pData);
    //�������ָ����,������ģʽ,���߷�����ģʽ����������ʾ�Ի���
    if( ret != 0 || _stop == true || _nDstMode == MD_THIRD  )
    {
        if(muFastMode->Checked==false || _stop == true)
        {
            FmWiseIP->SetResult(pData,pfname);
            ret = FmWiseIP->ShowModal();
            if(ret!=mrOk )
            {
                _unlink(pfname);
                _unlink(prname);
                return pHDP->SetPocket(ERROR_POCKET);
            }

            pData->modified = 1;
        }
        //����Ի���ȡ��,��Ҫɾ��ԭͼ��
        else
        {
            _unlink(pfname);
            _unlink(prname);
            //��Ʊ����ֵ���Ʊ�ڴ�
            return pHDP->SetPocket(ERROR_POCKET);
        }
    }

    memset(pData->fname,0,sizeof(pData->fname));
    memset(pData->rname,0,sizeof(pData->rname));
    memset(pData->ip,0,sizeof(pData->ip));

    //һ�λ������������Ҫ��������
    if(_nDstMode==MD_FIRST || _nDstMode == MD_THIRD)
    {
        strcpy(pData->ip,Distval.ip);
        pData->calx = Distval.calx;
        pData->caly = Distval.caly;
        pData->dpi  = Distval.dpi;
        strcpy(pData->prefix,Distval.prefix);

        strcpy(pData->fname,sfname);
        strcpy(pData->rname,srname);
    }
    strcpy(pData->date,date);
    strcpy(pData->session,session);
    strcpy(pData->clerk,login.clkno);

    strcpy(pData->pkgno,pData->exchno);

   
    strcpy(pData->vchtype,pData->pbcvt);

    String cd = _cdcMap[pData->vchtype];

    if(cd.Length()>0)
    {
        pData->cdcode = cd.ToInt();
    }
    else
        return pHDP->SetPocket(ERROR_POCKET);

    int size;

    //�����ļ���С���ж��ǲ����ŷ�
    size = ZGetFileSize(pfname) ;
    if(size<1300000)
    {
        if(atoi(pData->vchtype)<4)
        {
            if(strncmp(pData->codeacc,"0000000000",8)==0 && strncmp(pData->vchno,"0000000000",8)==0 )
                pData->isEnvelop = 1;      //Envelop
            else
                pData->isEnvelop = 0;      //check
        }
        else
        {
            if(size >1000000 )
                pData->isEnvelop = 0;           //���˵�
            else
                pData->isEnvelop = 1;           //����
        }
    }
    else
    {
        pData->isEnvelop = 1;            //Envelop
    }

    //������
    int pocket;
    if( SetOneData( hDistUI, pData ) == true)
    {
        pocket = GetPocketNum();
        if(pocket==0)
        {
            _unlink(pfname);
            _unlink(prname);
            DelSpecialItems(hDistUI,0);
        }
    }
    else
    {
        pocket = 0;
        _unlink(pfname);
        _unlink(prname);
    }          

    //����ڴ��ŷ�0,���Ǵ�Ʊ�ڴ�,ˢ�½���
    if(pocket>0)
    {
        GetChkData();
    }
    //���ÿڴ���
    return pHDP->SetPocket(pocket);
}
//---------------------------------------------------------------------------

/**
    ������ģʽ�л�
*/
void __fastcall TFmDistMan::muManCheckbClick(TObject *Sender)
{
    FormST->ShowModal();
}
//---------------------------------------------------------------------------
/**
    ȡ��������Ϣ
*/
int TFmDistMan::LoadCFG()
{
    //TODO: Add your source code here
    memset(&Distval,0,sizeof(Distval));
    String tmp;
    if(access(".\\config\\dists.cfg",0 ))
    {
        MessageBox(Handle,"�����ļ� dist.cfg ������,ʹ��ȱʡ����","��ʾ",MB_OK);
        return 0;
    }
    try
    {
        Config dist(".\\config\\dists.cfg");

        //��ֻ�ǰ׺,���������ļ���
        tmp = dist.value("prefix",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.prefix,tmp.Trim().c_str());
            debugout("perfix:%c\n",Distval.prefix[0]);
        }
        else
        {
            debugout("Prefix is null \n");
            strcpy(Distval.prefix,"A");
        }
        SetupDlg->txtPrefix->Text = Distval.prefix;

        //X����У׼ֵ
        tmp = dist.value("calx",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.calx = tmp.Trim().ToDouble();
            debugout("calx:%s\n",tmp.c_str());
        }
        else
        {
            debugout("calx is null \n");
            Distval.calx = 1.00;
        }
        SetupDlg->txtCalX->Text = Distval.calx;

        //Y����У׼ֵ
        tmp = dist.value("caly",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.caly = tmp.Trim().ToDouble();
            debugout("caly:%s\n",tmp.c_str());
        }
        else
        {
            debugout("caly is null \n");
            Distval.caly = 1.00;
        }
        SetupDlg->txtCalY->Text = Distval.caly;

        //�洢·��
        tmp = dist.value("path",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.path,tmp.Trim().c_str());
            debugout("path:%s\n",tmp.c_str());
        }
        else
        {
            strcpy(Distval.path,".\\");
            debugout("path is null \n");
        }
        SetupDlg->txtPath->Text = Distval.path;

        //���ɨ��ͼ�񾫶�
        tmp = dist.value("dpi",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.dpi = tmp.Trim().ToInt();
            debugout("dpi:%s\n",tmp.c_str());
        }
        else
        {
            debugout("dpi is null \n");
            Distval.dpi = 200;
        }

        SetupDlg->txtDPI->Text = Distval.dpi;

        //ƾ֤�ų���
        tmp = dist.value("vchLen",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.vchLen = tmp.Trim().ToInt();
            debugout("vchLen:%s\n",tmp.c_str());
        }
        else
        {
            debugout("vchLen is null \n");
            Distval.vchLen =6;
        }

        SetupDlg->txtVchLen->Text = Distval.vchLen;

        //�����ų���
        tmp = dist.value("exnoLen",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.exnoLen = tmp.Trim().ToInt();
            debugout("exnoLen:%s\n",tmp.c_str());
        }
        else
        {
            debugout("exnoLen is null\n");
            Distval.exnoLen = 8;
        }
        SetupDlg->txtExnoLen->Text = Distval.exnoLen;

        //�ʺų���
        tmp = dist.value("accnoLen",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.accLen = tmp.Trim().ToInt();
            debugout("accLen:%s\n",tmp.c_str());
        }
        else
        {
            debugout("accLen is null\n");
            Distval.accLen = 14;
        }
        SetupDlg->txtAccLen->Text = Distval.accLen;

        //ƾ֤���೤��
        tmp = dist.value("vchtpLen",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.vchtpLen = tmp.Trim().ToInt();
            debugout("vchtpLen:%s\n",tmp.c_str());
        }
        else
        {
            debugout("vchtpLen is null\n");
            Distval.vchtpLen = 2;
        }
        SetupDlg->txtVchtpLen->Text = Distval.vchtpLen;

        //����
        tmp = dist.value("amountLen",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.amountLen = tmp.Trim().ToInt();
            debugout("amountLen:%s\n",tmp.c_str());
        }
        else
        {
            debugout("amountLen is null\n");
            Distval.amountLen = 10;
        }
        SetupDlg->txtAmountLen->Text = Distval.amountLen;

        //ƾ֤�ŷָ��
        tmp = dist.value("vchno_interval",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.vch1_interval,tmp.Trim().c_str());
            debugout("vchno_interval:%s\n",tmp.c_str());
        }
        else
        {
            debugout("vchno_interval is null\n" );
            strcpy(Distval.vch1_interval,"<<");
        }
        SetupDlg->txtVchInterval->Text = Distval.vch1_interval;

        //�����ŷָ��
        tmp = dist.value("exno1_interval",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.exno1_interval,tmp.Trim().c_str());
            debugout("exno1_interval:%s\n",tmp.c_str());
        }
        else
        {
            debugout("exno1_interval is null\n" );
            strcpy(Distval.exno1_interval,"<<");
        }
        SetupDlg->txtExnoInterval1->Text = Distval.exno1_interval;

        //�����ŷָ��
        tmp = dist.value("exno2_interval",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.exno2_interval,tmp.Trim().c_str());
            debugout("exno2_interval:%s\n",tmp.c_str());
        }
        else
        {
            debugout("exno2_interval is null\n" );
            strcpy(Distval.exno2_interval,"<<");
        }
        SetupDlg->txtExnoInterval2->Text = Distval.exno2_interval;

        //�ʺŷָ��
        tmp = dist.value("accno1_interval",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.accno1_interval,tmp.Trim().c_str());
            debugout("accno1_interval:%s\n",tmp.c_str());
        }
        else
        {
            debugout("accno1_interval is null\n" );
            strcpy(Distval.accno1_interval,"<<");
        }
        SetupDlg->txtAccInterval1->Text = Distval.accno1_interval;

        //�ʺŷָ��
        tmp = dist.value("accno2_interval",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.accno2_interval,tmp.Trim().c_str());
            debugout("accno2_interval:%s\n",tmp.c_str());
        }
        else
        {
            debugout("accno2_interval is null\n" );
            strcpy(Distval.accno2_interval,"<<");
        }
        SetupDlg->txtAccInterval2->Text = Distval.accno2_interval;

        //ƾ֤����ָ��
        tmp = dist.value("vchtp1_interval",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.vchtp1_interval,tmp.Trim().c_str());
            debugout("vchtp1_interval:%s\n",tmp.c_str());
        }
        else
        {
            debugout("vchtp1_interval is null\n" );
            strcpy(Distval.vchtp1_interval,"<<");
        }
        SetupDlg->txtVchtpInterval1->Text = Distval.vchtp1_interval;

        //ƾ֤����ָ��
        tmp = dist.value("vchtp2_interval",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.vchtp2_interval,tmp.Trim().c_str());
            debugout("vchtp2_interval:%s\n",tmp.c_str());
        }
        else
        {
            debugout("vchtp2_interval is null\n" );
            strcpy(Distval.vchtp2_interval,"<<");
        }
        SetupDlg->txtVchtpInterval2->Text = Distval.vchtp2_interval;

        //���ָ��
        tmp = dist.value("amount1_interval",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.amount1_interval,tmp.Trim().c_str());
            debugout("amount1_interval:%s\n",tmp.c_str());
        }
        else
        {
            debugout("amount1_interval is null\n" );
            strcpy(Distval.amount1_interval,"<<");
        }
        SetupDlg->txtAmountInterval1->Text = Distval.amount1_interval;

        //���ָ��
        tmp = dist.value("amount2_interval",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            strcpy(Distval.amount2_interval,tmp.Trim().c_str());
            debugout("amount2_interval:%s\n",tmp.c_str());
        }
        else
        {
            debugout("amount2_interval is null\n" );
            strcpy(Distval.amount2_interval,"<<");
        }

        SetupDlg->txtAmountInterval2->Text = Distval.amount2_interval;

        //�Ƿ������ܽ��
        tmp = dist.value("inPutAmount",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.inPutAmount = tmp.Trim().ToInt();
            debugout("inPutAmount:%s\n",tmp.c_str());
        }
        else
        {
            debugout("inPutAmount is null\n" );
            Distval.inPutAmount = 0;
        }

        if(Distval.inPutAmount == 0)
            SetupDlg->ckManCtrlAmount->Checked = true;
        else
            SetupDlg->ckManCtrlAmount->Checked = false;

        //�Ƿ����OCR
        tmp = dist.value("haveOCR",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.haveOCR = tmp.Trim().ToInt();
            debugout("haveOCR:%s\n",tmp.c_str());
        }
        else
        {
            debugout("haveOCR is null\n" );
            Distval.haveOCR = 0;
        }

        if(Distval.haveOCR == 0)
            SetupDlg->ckOcrRecognize->Checked = true;
        else
            SetupDlg->ckOcrRecognize->Checked = false;

        //�Ƿ��������м��
        tmp = dist.value("inputCheck",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.inputCheck=tmp.Trim().ToInt();
            debugout("inputCheck:%s\n",tmp.c_str());
        }
        else
        {
            debugout("inputCheck is null\n" );
            Distval.inputCheck = 0;
        }

        if(Distval.inputCheck == 0)
            SetupDlg->chkInput->Checked = true;
        else
            SetupDlg->chkInput->Checked = false;

        //�Ƿ�����ֻ�
        tmp = dist.value("hdpType",1);
        if(tmp.Trim().c_str()!=NULL)
        {
            Distval.hdpType=tmp.Trim().ToInt();
            debugout("hdpType:%s\n",tmp.c_str());
        }
        else
        {
            debugout("hdpType is null\n" );
            Distval.hdpType = 0;
        }

        if(Distval.hdpType == 0)
            SetupDlg->chkNewHDP->Checked = true;
        else
            SetupDlg->chkNewHDP->Checked = false;

        // Color scan speed
        tmp = dist.value("colorScanSpeed", 1);
        if (tmp != NULL && tmp.Trim().c_str() != NULL)
        {
            Distval.colorScanSpeed = tmp.Trim().ToInt();
            debugout("colorScanSpeed:%s\n", tmp.c_str());
        }
        else
        {
            debugout("colorScanSpeed is null\n");
            Distval.colorScanSpeed = DEFAULT_COLOR_SCAN_SPEED;
        }

        // Gray scan speed
        tmp = dist.value("grayScanSpeed", 1);
        if (tmp != NULL && tmp.Trim().c_str() != NULL)
        {
            Distval.grayScanSpeed = tmp.Trim().ToInt();
            debugout("grayScanSpeed:%s\n", tmp.c_str());
        }
        else
        {
            debugout("grayScanSpeed is null\n");
            Distval.grayScanSpeed = DEFAULT_GRAY_SCAN_SPEED;
        }

    }
    catch(...)
    {
        MessageBox(Handle,"��ȡ��ֻ������ļ�dists.cfg����","��ʾ",MB_OK|MB_ICONINFORMATION);
        return -1;
    }

    //��ȡ������ַ
    char host_str[100];
	memset( host_str,0,100 );

	int ret;
	ret = gethostname(host_str,100);
    if( ret ) return -1;

	struct hostent* hp;
	hp = (hostent *)gethostbyname(host_str);
	if( hp == NULL ) return -1;

   	unsigned long ipaddr = *(unsigned long *)(hp->h_addr);
    strcpy(Distval.ip,inet_ntoa(*(in_addr*)&ipaddr));


    if( Distval.hdpType == 0 )
        skipHead = 7;
    else
        skipHead = 24;
 	return 0;
}
//---------------------------------------------------------------------------
/**
    ����������Ϣ
*/
int TFmDistMan::SaveCFG()
{
    //TODO: Add your source code here
    FILE *fp;
    if((fp = fopen(".\\config\\dists.cfg","w+"))==NULL)
    {
        MessageBox(Handle,"�������ļ�����","����",MB_OK);
        return -1;
    }
    fprintf(fp,"#              ��ֻ����������ļ�\n");
    fprintf(fp,"#******************************************************\n");
    fprintf(fp,"# prefix:��ֻ���ţ�ֻ��һ����ĸ����������ͬ����ֻ�,\n");
    fprintf(fp,"#        ����������������ɨ���ǵı�־��ǧ���ܺ�������\n");
    fprintf(fp,"#        ��ֻ��ظ�\n");
    fprintf(fp,"prefix=%c\n\n",Distval.prefix[0]);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# path:ͼ�񱣴�·��\n");
    fprintf(fp,"path=%s\n\n",Distval.path);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# calx  :X����У׼ֵ\n");
    fprintf(fp,"# caly  :Y����У׼ֵ\n");
    fprintf(fp,"calx=%8.6f\n",Distval.calx);
    fprintf(fp,"caly=%8.6f\n\n",Distval.caly);

    fprintf(fp,"#******************************************************\n");
    fprintf(fp,"# dpi   :ɨ��ͼƬ����,���ڿ���ɨ�費ͬDPI��ͼ��\n");
    fprintf(fp,"#	 һ��Ϊ200��300\n");
    fprintf(fp,"dpi=%3d\n",Distval.dpi);

    fprintf(fp,"# vchLen:�����ƾ֤�ų���\n");
    fprintf(fp,"vchLen =%d\n",Distval.vchLen);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# exnoLen:����뽻���ų���\n");
    fprintf(fp,"exnoLen =%d\n",Distval.exnoLen);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# accnoLen:������ʺų���\n");
    fprintf(fp,"accnoLen =%d\n",Distval.accLen);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# vchtpLen:�����ƾ֤���೤��\n");
    fprintf(fp,"vchtpLen =%d\n",Distval.vchtpLen);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# amountLen:��������\n");
    fprintf(fp,"amountLen =%d\n",Distval.amountLen);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# vchno_interval:�����ƾ֤�ŷָ����\n");
    fprintf(fp,"vchno_interval =%s\n",Distval.vch1_interval);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# exno1_interval:����뽻���ŷָ����\n");
    fprintf(fp,"exno1_interval =%s\n",Distval.exno1_interval);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# exno2_interval:����뽻���ŷָ����\n");
    fprintf(fp,"exno2_interval =%s\n",Distval.exno2_interval);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# accno1_interval:������ʺŷָ����\n");
    fprintf(fp,"accno1_interval =%s\n",Distval.accno1_interval);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# accno2_interval:������ʺŷָ����\n");
    fprintf(fp,"accno2_interval =%s\n",Distval.accno2_interval);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# vchtp1_interval:�����ƾ֤����ָ����\n");
    fprintf(fp,"vchtp1_interval =%s\n",Distval.vchtp1_interval);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# vchtp2_interval:�����ƾ֤����ָ����\n");
    fprintf(fp,"vchtp2_interval =%s\n",Distval.vchtp2_interval);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# amount1_interval:�����ƾ֤����ָ����\n");
    fprintf(fp,"amount1_interval =%s\n",Distval.amount1_interval);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# amount2_interval:�����ƾ֤����ָ����\n");
    fprintf(fp,"amount2_interval =%s\n",Distval.amount2_interval);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# inPutAmount:�ֹ������ܶ�\n");
    fprintf(fp,"inPutAmount =%d\n",Distval.inPutAmount);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# inputCheck:����������\n");
    fprintf(fp,"inputCheck =%d\n",Distval.inputCheck);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# haveOCR:����ͼ��ʶ��\n");
    fprintf(fp,"haveOCR =%d\n",Distval.haveOCR);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# hdpType :��ֻ�����\n");
    fprintf(fp,"hdpType =%d\n",Distval.hdpType);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# colorScanSpeed :��ɫ����ٶ�\n");
    fprintf(fp,"colorScanSpeed =%d\n",Distval.colorScanSpeed);
    fprintf(fp,"#********************************************************\n");

    fprintf(fp,"# grayScanSpeed :�Ҷ�����ٶ�\n");
    fprintf(fp,"grayScanSpeed =%d\n",Distval.grayScanSpeed);
    fprintf(fp,"#********************************************************\n");

    fclose(fp);

    if( Distval.hdpType == 0 )
        skipHead = 7;
    else
        skipHead = 24;
        
}
//---------------------------------------------------------------------------

/**
    ������ֽ����ʼ��
*/
int TFmDistMan::Init3Dst()
{
    //TODO: Add your source code here
    btnDstChange->Glyph->Assign( bmp3 );
    pgDists->ActivePageIndex = 2;
    mu2Dst->Checked = true;
    _nDstMode = MD_THIRD;
    muEndorse->Checked=false;
    return 0;
}
//---------------------------------------------------------------------------

/**
    ��ʼ�������
*/
int TFmDistMan::StartThird()
{
    //TODO: Add your source code here
    if( _nDstState != ST_STOP || _reDistState != RDS_NONE )
    {
        debugout("...return ...\n");
        return 0;
    }
    ClearShow();

    return 0;
}
//---------------------------------------------------------------------------

/**
    ����������ݴ���
*/
int TFmDistMan::Treat3Data()
{
    //TODO: Add your source code here
    txt3SortCount->Caption = IntToStr(txt3SortCount->Caption.ToInt()+1);

    //������ֻ��ڴ���ϸ
    TListItem* pItem = lst3PocketDetail->FindCaption(0, pData->PockCode, 0, 0, 1 );
    //����ҵ���,����ֱ��+1
    if( pItem )
    {
        pItem->SubItems->Strings[POC1_COL_CNT] = IntToStr(pItem->SubItems->Strings[POC1_COL_CNT].ToInt() + 1);
    }
    //���û���ҵ�,�����һ��
    else
    {
        pItem = getRightItem(lst3PocketDetail,pData->PockNum);                  // sort the items use pocketnum
        pItem->Data = (void *)pData->PockNum;
        pItem->Caption = pData->PockCode;
        pItem->SubItems->Strings[POC1_COL_DIS] = pData->PockText;
        pItem->SubItems->Strings[POC1_COL_CNT] = 1;
        pItem->SubItems->Strings[POC1_COL_DOC] = txtDeCr[pData->cdcode];
    }

    //�������֧Ʊ������ϸ
    pItem = lst3CheckDetail->FindCaption(0, pData->VchKindCod, 0, 0, 1 );
    //����ҵ�,����ֱ��+1
    if( pItem )
    {
        pItem->SubItems->Strings[VCH1_COL_CNT] = IntToStr(pItem->SubItems->Strings[VCH1_COL_CNT].ToInt() + 1);
    }
    //���û���ҵ�,�����һ��
    else
    {
        pItem = getRightItem(lst3CheckDetail,pData->PockNum);                   // sort the items use pocketnum
        pItem->Data = (void *)pData->PockNum;
        pItem->Caption = pData->VchKindCod;
        pItem->SubItems->Strings[VCH1_COL_DIS] = pData->VchKindTxt;
        pItem->SubItems->Strings[VCH1_COL_CNT] = 1;
        pItem->SubItems->Strings[VCH1_COL_DOC] = txtDeCr[pData->cdcode];
        pItem->SubItems->Strings[VCH1_COL_POS] = pData->PockCode;
    }
}
//---------------------------------------------------------------------------

/**
    ���������
*/
bool TFmDistMan::CheckMoney(const char * exno,const char* pbcvt, char* amount,char*accno,char*vchno)
{
    //TODO: Add your source code here

    int ret;
    MyRes res;
    MyRow row;

    //һ����ֲ�ѯpbcdata��
    if(_nDstMode == MD_FIRST )
    {
        ret = gdb.VQuery(512,"select count(*) from pbcdata where exchno='%s' and date='%s' and amount=%s and session=%s and vchtype='%s' and right(accno,%d)='%s' and vchno='%s'",
                    exno,amount,date,session,pbcvt,Distval.accLen,accno,vchno);
        if(ret)
        {
            debugout("sql error [%s]\n",mysql_error(gdb.Handle()));
            return false;
        }

        res = gdb.Store();
        row = res.Fetch();
        if( atoi(row[0])>=1) return true;

        //���û�иý���Ӧ����,��Ӧ�ò���pbcdata���Ƿ�������
        else
        {
            ret = gdb.VQuery(512,"select count(*) from pbcdata \
                        where exchno='%s' and date='%s' and session='%s'",exno,date,session);
            if(ret)
            {
                debugout("sql error [%s]\n",mysql_error(gdb.Handle()));
                return false;
            }
            res = gdb.Store();
            row = res.Fetch();
            if(atoi(row[0])>0 ) return false;
            else return true;
        }
    }       

    return true;
}
//---------------------------------------------------------------------------

/**
    �������ʱ��ʼ��������
*/
int TFmDistMan::InitDistNode(const char * pkgno)
{
    //TODO: Add your source code here

   return 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/**
    �õ����ñ������Ϣ
*/
int TFmDistMan::GetDS(const char* name, char * value)
{
    //TODO: Add your source code here
    txtSession->Items->Clear();

    int ret = gdb.VQuery(512,"select value from config where name='%s'",name);
	if(ret)
    {
		debugout("sql error [%s]\n",mysql_error(gdb.Handle()));
		zreturn(ret);
	}

    MyRes	res = gdb.Store();
	int count = (int)res.Count();
    MyRow row;
    
	if(count>0)
    {
		row = res.Fetch();
		strcpy(value,row[0]);
	}
	else
    {
		if( strcmp(name,"date") == 0)
		{
			ret = gdb.Query("select concat(substring(now(),1,4),substring(now(),6,2),substring(now(),9,2)) from  bocnets limit 1");
			if(ret)
			{
				debugout("sql error [%s]\n",mysql_error(gdb.Handle()));
				zreturn(ret);
			}

			res = gdb.Store();
			row = res.Fetch();
			if(row == NULL)   strcpy(value,"00000000");
			else              strcpy(value,row[0]);
			return 0;
		}
		else if (strcmp(name,"session")==0)
		{
    		int T;
			ret = gdb.Query("select value from config where name='time'");
			if(ret)
			{
				debugout("sql error [%s]\n",mysql_error(gdb.Handle()));
				zreturn(ret);
			}

			res = gdb.Store();
			row = res.Fetch();
			if(row == NULL)   T = 13;
			else              T = atoi(row[0]);

    		ret = gdb.Query("select substring(now(),12,2) from  bocnets limit 1");
		    res = gdb.Store();
			row = res.Fetch();
			if(row==NULL)
            {
                strcpy(value,"1");
            }
			else
			{
				if( atoi(row[0])>=T) strcpy(value,"2");
				else				 strcpy(value,"1");
			}
			return 0;
	  	}
	  	else if (strcmp(name,"moneylimit")==0)
	  	{
	  		strcpy(value,"0");
	  		return 0;
	  	}
	  	else if(strcmp(name,"money_level")==0)
	  	{
	  		zreturn(1);
	  	}
	  	else
	   	{
	   		strcpy(value,"0");
	   		return 0;
	   	}
	}
	return 0;
}
//---------------------------------------------------------------------------

/**
    ���������ɺ�,֧Ʊ���ŷ��Զ�ƥ��
*/
int TFmDistMan::CTE(const char* pkgno,const char* session)
{
    //TODO: Add your source code here
    return 0;  
}
//---------------------------------------------------------------------------

/**
    ������ֲ˵������Ӧ����
*/
void __fastcall TFmDistMan::N2Click(TObject *Sender)
{
    N2->Checked = !N2->Checked;
    _stop = !N2->Checked;
}
//---------------------------------------------------------------------------

/**
    ��ʼ��������ָ��ڴ���
*/
int TFmDistMan::InitPocketCount(const char* pkgno, const char* session)
{
    //TODO: Add your source code here
    memset(PocketCount,0,sizeof(PocketCount));
    PocketCount[0] = "0";
    PocketCount[1] = "0";
    PocketCount[2] = "0";
    PocketCount[3] = "0";
    PocketCount[4] = "0";
    PocketCount[5] = "0";
    PocketCount[6] = "0";
    PocketCount[7] = "0";
    PocketCount[8] = "0";

    PocketTpCount[0] = "0";
    PocketTpCount[1] = "0";
    PocketTpCount[2] = "0";
    PocketTpCount[3] = "0";
    PocketTpCount[4] = "0";
    PocketTpCount[5] = "0";
    PocketTpCount[6] = "0";
    PocketTpCount[7] = "0";
    PocketTpCount[8] = "0";

    return 0;
}
//---------------------------------------------------------------------------
/**
    ��Ʊ����ɾ��һ����¼��,�����Ӧˢ��
*/
int TFmDistMan::SubOne(const char* vchtype, int isenvelop,double amount)
{
    //TODO: Add your source code here
    char PockCode[4];
    char KindCode[4];

    int pocket = 0;

    String ps = _pocMap[vchtype];

    if(ps.Length()<=0) pocket = 0 ;
    else
    {
        pocket = ps.ToInt();
    }

    pocket = ps.ToInt();

    if( atoi(vchtype) == 0 && isenvelop == 1)
    {
        pocket = pocket +1;
    }

    if( atoi(vchtype) >= 50 && isenvelop == 0 )
    {
        String ps = _pocMap["JZD"];

        if(ps.Length()<=0) pocket = 0 ;
        else
        {
            pocket = ps.ToInt();
        }
    }
    
    sprintf(KindCode,"A%02s",vchtype);
    sprintf(PockCode,"DA%d",pocket);
    debugout("KindCode [%s] PocketCode[%s]\n",KindCode,PockCode);

    //���ݿڴ���������ͳ����
    TListItem* pItem = lst1PocketDetail->FindCaption(0, PockCode, 0, 0, 1 );
    if( pItem )
    {
        pItem->SubItems->Strings[POC1_COL_CNT] = IntToStr(pItem->SubItems->Strings[POC1_COL_CNT].ToInt() - 1);
    }

    pItem = lst1CheckDetail->FindCaption(0, KindCode, 0, 0, 1 );
    if( pItem )
    {
        pItem->SubItems->Strings[VCH1_COL_CNT] = IntToStr(pItem->SubItems->Strings[VCH1_COL_CNT].ToInt() - 1);
        String sTotal = RemoveThousandChar(pItem->SubItems->Strings[VCH1_COL_POS]);
        String sMoney = amount;
        pItem->SubItems->Strings[VCH1_COL_POS] = FloatToThousandStr( sTotal.ToDouble() - sMoney.ToDouble() );
    }

    return 0;
}
//---------------------------------------------------------------------------

/**
    ��ʼ��ƾ֤���������Ϣ
*/
int TFmDistMan::initVTList()
{
    //TODO: Add your source code here
    myVT.clear();
    int ret = gdb.Query("select vchtype,cdcode from vchtypes");
    if(ret)
    {
        MessageBox(Handle,"��ѯ����","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(gdb.Handle()));
        return  0;
    }

    MyRes res = gdb.Store();
    int count = res.Count();
    MyRow row = res.Fetch();
    if(row == NULL)
    {
        MessageBox(Handle,"ƾ֤��Ӧ��ϵ��Ϊ��,��ά��","��ʾ",MB_OK|MB_ICONINFORMATION);
        return 0;
    }
    while(row!=NULL)
    {
        _cdcMap[row[0]] = row[1];
        row = res.Fetch();
    }


    ret = gdb.Query("select name ,value from config");
    if(ret)
    {
        MessageBox(Handle,"��ѯ����","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(gdb.Handle()));
        return  0;
    }

    res = gdb.Store();
    row = res.Fetch();
    while(row!=NULL)
    {
        _cfgMap[row[0]] = row[1];
        row = res.Fetch();
    }

    ret = gdb.Query("select vchtype,pockno,pocdesc from pocket where distno=1");
    if(ret)
    {
        MessageBox(Handle,"��ѯ����","��ʾ",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error(gdb.Handle()));
        return  0;
    }

    res = gdb.Store();
    row = res.Fetch();
    while(row!=NULL)
    {
        _pocMap[row[0]] = row[1];
        _desMap[row[0]] = row[2];
        row = res.Fetch();
    }    
    
    return 0;
}
//---------------------------------------------------------------------------

/**
    ���ݶ��չ�ϵ,��������ƾ֤�����Ӧ������ƾ֤����
*/
int TFmDistMan::getVT(vt_map_t & k)
{
    //TODO: Add your source code here

    //��ʼ��Ĭ��ֵ
    if( atoi(k.pbcvt)<50&& atoi(k.pbcvt)!=27)
        k.cdcode = 0;
    else
        k.cdcode  = 1;

    if(atoi(k.pbcvt) == 0 || atoi(k.pbcvt) == 1 || atoi(k.pbcvt ) ==50 || atoi(k.pbcvt)==54)
        k.proctype = 1;
    else
        k.proctype = 0;

    strcpy(k.bankvt , k.pbcvt);
    VTMAP::iterator pos= myVT.begin();
    while( pos != myVT.end())
	{
        if(atoi(pos->pbcvt) == atoi(k.pbcvt))
        {
            k = *pos ;
            break;
        }
        ++pos;
	}
}
//---------------------------------------------------------------------------

/**
    ����������仯��Ӧ����
*/
void __fastcall TFmDistMan::txtSessionSelect(TObject *Sender)
{
    strncpy(session,txtSession->Text.Trim().c_str(),sizeof(session));
}
//--------------------------------------------------------------------------- 


void __fastcall TFmDistMan::N10Click(TObject *Sender)
{
     if( FmPocket->ShowModal() != mrOk ) return;
}
//---------------------------------------------------------------------------

void __fastcall TFmDistMan::O1Click(TObject *Sender)
{
     if( FmVips->ShowModal() != mrOk ) return;        
}
//---------------------------------------------------------------------------


void __fastcall TFmDistMan::muManCheckClick(TObject *Sender)
{
    distForm->ShowModal();
}
//---------------------------------------------------------------------------

