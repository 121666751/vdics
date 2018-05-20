/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 北京天创征腾信息科技有限公司    版权所有
 *
 * 此文件为天创征腾软件产品的一部分。
 * 天创征腾软件产品的所有部分,包括但不限于软件产品中所含的任何图象、照片、
 * 动画、文字和附加程序（applets）、加密卡、随附的印刷材料、源代码文件及
 * 本软件产品的任何副本的一切所有权和知识产权，均归版权人天创征腾拥有。
 *
 * 文件：	$Id: cbxctrl.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
-----------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "cbxctrl.h"
#include "serialport.h"
#include "main.h"
#include "wiseIPdlg.h"
//#include "ncrdef.h"

#define END_OF_COUNT		0x80000000
#define SYSTEMERROR 		0x40000000
#define MICR_FAILURE 		0x08000000
#define SORTER_FULL 		0x04000000
#define SCANNER_BADLINK 	0x01000000
#define OCR_SYS_FAILURE 	0x00800000
#define SCANNER_JAM 		0x00400000
#define LIFT_JAM 			0x00200000
#define JAMWHILESORTING 	0x00100000
#define NO_MICR_RIBBON 		0x00080000
#define JAM_AT_INIT 		0x00040000
#define WAITING_FOR_POCKET 	0x00020000
#define LOOSE_COVERS 		0x00010000
#define JAM_IN_BEND 		0x00008000
#define FONTWHEEL_FAIL 		0x00004000
#define JAM_AT_VIEWING 		0x00002000
#define JAM_IN_MARKER 		0x00001000
#define JAMWHILEREADING 	0x00000800
#define JAMBEFOREREADER 	0x00000400
#define FEEDERJAM 			0x00000200
#define SIDE_FEEDER_JAM 	0x00000100
#define MACHINE_FAILURE 	0x00000080
#define HOST_ERROR 			0x00000040
#define HOSTLINE_ERROR 		0x00000020
#define HOSTLINE_BREAK 		0x00000010
#define SYSTEMHALTED 		0x00000008
#define DOUBLEFEED 			0x00000004
#define SINGLE_EMPTY 		0x00000002
#define FEEDER_EMPTY 		0x00000001

enum CBXSTATE
{
	CBX_IDLE		= 0,

	CBX_INIT_PARA	= 1,
    CBX_ERROR_OCC,
    CBX_SORT_START,
    CBX_SORT_STOP,

    CBX_Q_FEEDER	= 10,
    CBX_Q_COVER	,
    CBX_Q_POCKET

};

enum COMSTATE
{
	COM_IDLE	= 0,
	COM_SEND,
    COM_RECV
};

struct CBXDATA
{
    CSerialPort *m_port;
    CBXSTATE cbx_state;
    COMSTATE com_state;
    bool mashinestart;
    bool hoper_first;
    bool feed_single;
    char recvline[512];
    char micr_code[80];
    char ocr_code[80];
    CBXDATA(){
     	m_port = new CSerialPort;
        cbx_state = CBX_IDLE;
        com_state = COM_IDLE;
        mashinestart = false;
        hoper_first = false;
        feed_single = false;
        memset(recvline,0,sizeof(recvline));
        memset(micr_code,0,sizeof(micr_code));
        memset(ocr_code,0,sizeof(ocr_code));
    }
    ~CBXDATA(){ delete m_port;}
};

//---------------------------------------------------------------------------

#pragma package(smart_init)


CbxCtrl::CbxCtrl()
{
//TODO: Add your source code here
    theData = new CBXDATA;
}


CbxCtrl::~CbxCtrl()
{
	//TODO: Add your source code here
    delete theData;
}

int CbxCtrl::Init(HWND handle, int port, int baud)
{
	//TODO: Add your source code here
    BOOL ret = theData->m_port->InitPort(handle,port,baud);
    if( ret != TRUE ) return -1;
    ret = theData->m_port->StartMonitoring((void *)(theData->m_port));
    if( ret != TRUE ) return -2;
    return 0;
}

void CbxCtrl::OnRecvCh(char ch)
{
	//TODO: Add your source code here
    if( ch == 10 ){
    	OnRecvLine();
        return;
    }
    char a[2];
	a[0] = ch;
    a[1] = 0;
    strcat(theData->recvline,a);
}


void CbxCtrl::OnRecvLine()
{
	//TODO: Add your source code here
    TreatLine();
    memset( theData->recvline, 0, sizeof( theData->recvline ));
}

void CbxCtrl::Bell(int type, int duration)
{
	//TODO: Add your source code here
    char command[40];
    memset( command, 0, sizeof(command));
    char t;
    switch(type)
    {
    case 1:
    	sprintf(command,"@X,T,%d\n",duration);
        break;
    case 2:
    	sprintf(command,"@X,C,%d\n",duration);
	    break;
    default:
    	sprintf(command,"@X\n");
        break;
    }
    runCmd(command);
}

void CbxCtrl::StartFeed(bool h_first,bool single)
{
	//TODO: Add your source code here
    theData->hoper_first = h_first;
    theData->feed_single = single;
    theData->cbx_state = CBX_SORT_START;
//	initParaEx();
    Application->ProcessMessages();
    GetCBXState();
}


void CbxCtrl::StopFeed()
{
	//TODO: Add your source code here
#ifndef HANTLE
    theData->cbx_state = CBX_SORT_STOP;
    stopFeedEx();                              
#endif    //GetCBXState();
}


void CbxCtrl::RecoverCBX()
{
	//TODO: Add your source code here
    runCmd("@R\n");
}


void CbxCtrl::GetCBXState()
{
	//TODO: Add your source code here
    runCmd("@Q\n");
}


void CbxCtrl::CheckCover()
{
	//TODO: Add your source code here
    runCmd("@/C\n");
}


void CbxCtrl::CheckPocket()
{
	//TODO: Add your source code here
    runCmd("@/S\n");
}


void CbxCtrl::runCmd(char * cmd)
{
	//TODO: Add your source code here
    if( !theData->m_port->Initiallized() )  return;
    theData->m_port->WriteToPort(cmd);
    Application->ProcessMessages();
}


void CbxCtrl::TreatLine()
{
	//TODO: Add your source code here
    switch( theData->recvline[0] )
    {
    case '#':
    	treatState();
        break;
    case '|':
    	treatCode();
        break;
    case '?':
    	treatErr();
        break;
    default:
    	if( isdigit(theData->recvline[0])) treatChk();
        break;
    }
}


void CbxCtrl::treatState()
{
	//TODO: Add your source code here
    Outline(theData->recvline);
    AnalyzeState(theData->recvline);
    switch(theData->cbx_state)
    {
    case CBX_SORT_START:
     	startFeedEx();
        break;
    case CBX_SORT_STOP:
    	stopFeedEx();
        break;
    case CBX_INIT_PARA:
     	initParaEx();
        break;
    default:
    	break;
    }
    theData->cbx_state = CBX_IDLE;
}


void CbxCtrl::treatCode()
{
	//TODO: Add your source code here
    char *p = theData->recvline;
    getCode(p,theData->micr_code);
    getCode(p,theData->ocr_code);
    int  result=memcmp(theData->micr_code,theData->ocr_code,sizeof(theData->micr_code));
    if(!result)   ShowMessage("micr_code!=ocr_code");
//    ShowMessage(theData->micr_code);
//    ShowMessage(theData->ocr_code);

    FmWiseIP->txtMicrCode->Text = theData->micr_code;
    //FmWiseIP->txtMicrCode->Text = theData->ocr_code;        //use when micr can't use
    FmWiseIP->txtOcrCode->Text = theData->ocr_code;
    Application->ProcessMessages();
    FmWiseIP->ctlCodelineDataComplete(1);//MICR_CODELINE);
    FmWiseIP->ctlCodelineDataComplete(2);//OCR_CODELINE);
    Application->ProcessMessages();
    //Form1->mmRes->Lines->Add(theData->micr_code);
    //Form1->mmRes->Lines->Add(theData->ocr_code);
    FmWiseIP->ctlOutputDecisionPoint();
    FmWiseIP->ctlPocketDecisionPoint();
    Application->ProcessMessages();
    FmWiseIP->ctlUpstreamInputDataComplete();
}
void CbxCtrl::treatErr()
{
    Outline(theData->recvline);
    RecoverCBX();
	//TODO: Add your source code here
}


void CbxCtrl::treatChk()
{
	//TODO: Add your source code here
    Outline(theData->recvline);
}


void CbxCtrl::startFeedEx()
{
	//TODO: Add your source code here
//    runCmd("@H\n");
//    runCmd("@R\n");
    if( theData->hoper_first )
    	runCmd("@G,1000\n");
    else
    	runCmd("@G,1,M\n");
    theData->mashinestart = true;
}


void CbxCtrl::stopFeedEx()
{
	//TODO: Add your source code here
    runCmd("@H\n");
    theData->mashinestart = false;
}


void CbxCtrl::getCode(char * &p, char * buf)
{
	//TODO: Add your source code here
    int length = 0;
    int pos = p - theData->recvline;      ///pos==0
    for( int i = pos+1 ; i <int(strlen(theData->recvline)) ; i++ )
    {
    	p++;
    	if( *p == '|' ) break;
        length++;
    }
    memset( buf, 0, sizeof(buf));
    memcpy( buf, theData->recvline+pos+1, length );
    buf[length] = 0;
    FormatCode(buf);
}


void CbxCtrl::InitPara()
{
	//TODO: Add your source code here
    theData->cbx_state = CBX_INIT_PARA;
    initParaEx();
    GetCBXState();
}


void CbxCtrl::initParaEx()
{
	//TODO: Add your source code here
    Sleep(200);
    runCmd("@J,2\n");
    Sleep(200);
    Application->ProcessMessages();
    runCmd("@F,1,6,0,1,1\n");
}


void CbxCtrl::Sort(int poc)
{
	//TODO: Add your source code here
    char command[40];
    memset( command, 0, sizeof(command));
	sprintf( command, "@B,%d\n",poc);
    runCmd(command);
}


void CbxCtrl::Flush()
{
	//TODO: Add your source code here
    runCmd("@/F\n");
    runCmd("@B,0");
}


void CbxCtrl::Endorse(char * text)
{
	//TODO: Add your source code here
    if( strlen(text) > 64 ) return;
    char command[80];
    memset( command, 0, sizeof(command));
	sprintf( command, "@T,\"%s\"\n",text);
    runCmd(command);
}


void CbxCtrl::Outline(char * msg)
{
    //TODO: Add your source code here
    FmDistMan->InsertLog(msg);
}

void CbxCtrl::AnalyzeState(char * state)
{
    //TODO: Add your source code here
    String s = state;
    s = Trim(s);
    s = s.Delete(1,1);
    unsigned int istate = 0;
    sscanf( s.c_str(), "%X", &istate );

 /*   if( istate & END_OF_COUNT );
    if( istate & SYSTEMERROR );
    if( istate & MICR_FAILURE );
    if( istate & SORTER_FULL );
    if( istate & SCANNER_BADLINK );
    if( istate & OCR_SYS_FAILURE );
    if( istate & SCANNER_JAM );
    if( istate & LIFT_JAM );
    if( istate & JAMWHILESORTING );
    if( istate & NO_MICR_RIBBON );
    if( istate & JAM_AT_INIT );
    if( istate & WAITING_FOR_POCKET );
    if( istate & LOOSE_COVERS );
    if( istate & JAM_IN_BEND );
    if( istate & FONTWHEEL_FAIL );
    if( istate & JAM_AT_VIEWING );
    if( istate & JAM_IN_MARKER );
    if( istate & JAMWHILEREADING );
    if( istate & JAMBEFOREREADER );
    if( istate & FEEDERJAM );
    if( istate & SIDE_FEEDER_JAM );
    if( istate & MACHINE_FAILURE );
    if( istate & HOST_ERROR );
    if( istate & HOSTLINE_ERROR );
    if( istate & HOSTLINE_BREAK );
    if( istate & SYSTEMHALTED );
    if( istate & DOUBLEFEED );
  */  theData->hoper_first = bool(istate & SINGLE_EMPTY);
    if( (istate & FEEDER_EMPTY) &&
    	(istate & SINGLE_EMPTY) &&
        theData->mashinestart)
    {
        FmWiseIP->ctlFeedFinished();
        StopFeed();
    }
}

void CbxCtrl::FormatCode(char * buf)
{
	//TODO: Add your source code here
    char *tmpbuf = NULL;
    tmpbuf = new char[strlen(buf)+4];
    memset(tmpbuf,0,sizeof(tmpbuf));
    int len = strlen(buf);
    int j = 0;
    for(int i = 0 ; i < len ; i++)
    {
    	if( buf[i] != ' '){
        	if( buf[i] == '@' )
            	tmpbuf[j] = '!';
            else
            	tmpbuf[j] = ( IsSymbol(buf[i]) ) ? 'a' : buf[i];
            j++;
        }
    }
    tmpbuf[j] = 0;
    memset(buf,0,sizeof(buf));
    strcpy(buf,tmpbuf);
    delete[] tmpbuf;
}


bool CbxCtrl::IsSymbol(char a)
{
	//TODO: Add your source code here
    if( (int)a == 0 ) return false;
    if( isdigit(a) ) return false;
    switch( a )
    {
    case 'o':
    case 't':
    case 'a':
    case 'd':
    case '<':
    case ':':
    case ';':
    case '=':
    	return true;
    default:
    	return false;
    }
}


void CbxCtrl::ChangeMode(bool needstop)
{
    //TODO: Add your source code here
    char ch = (needstop) ? '1' : '2';
    char cmd[64];
    sprintf( cmd, "@J,%c\n ",ch);
    runCmd(cmd);
}

bool CbxCtrl::Initiallized()
{
    //TODO: Add your source code here
    return theData->m_port->Initiallized();
}
