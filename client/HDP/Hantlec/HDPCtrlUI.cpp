/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//-------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <TC_hdp.h>
#include "main.h"
#include "HDPCtrlUI.h"
#include <string>
#include <sstream>
#include <zcommon.h>
#include "SplitDc.h"
#include "setup.h"

#define ACC_HEADER  ""
#define ACC_START  0

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
HDPCtrlUI::HDPCtrlUI()
{
    //TODO: Add your source code here
    theData = new HDPDATA;
}
//---------------------------------------------------------------------------
HDPCtrlUI::~HDPCtrlUI()
{
    //TODO: Add your source code here
    delete theData;
}
//---------------------------------------------------------------------------
/**
    清分机接口函数
    初始化清分机
*/
int HDPCtrlUI::Init()
{
    //TODO: Add your source code here
    hHDP = InitMDL(FmDistMan->Handle,OnHDP,OnErrorHDP);
    if( hHDP != NULL) return 0;
    return -1;
}
//---------------------------------------------------------------------------
/**
    清分机接口函数
    得到清分机句柄
*/
int HDPCtrlUI::Connect()
{
    //TODO: Add your source code here
    return HDPOpen(hHDP);
}
//---------------------------------------------------------------------------

/**
    释放清分机句柄
*/
void HDPCtrlUI::Free()
{
    //TODO: Add your source code here
    FreeMDL(hHDP);
    hHDP=NULL;

}
//---------------------------------------------------------------------------
/**
    清分机接口函数
    关闭清分机句柄
*/
int HDPCtrlUI::Disconnect()
{
    //TODO: Add your source code here
    return HDPClose(hHDP);
}
//---------------------------------------------------------------------------
/**
    清分机接口函数
    开始清分
*/
int HDPCtrlUI::Start(int mode, int speed, bool initflag)
{
    //TODO: Add your source code here
    _mode = mode;
    theData->speed = speed;
    int ret = HDPStart(hHDP,mode, speed, initflag);
    switch( ret )
    {
        case 0:
        case 1:
            FmDistMan->InsertLog("Hopper Empty");
            if( FmDistMan->_nDstState == ST_RUN )
            {
                FmDistMan->btnStartClick(0);
                FmDistMan->CheckMoneyEqual();
            }

            break;
        case -1:
            FmDistMan->Pause();
            MessageBox(FmDistMan->Handle,"请放好票据或检查清分机内是否有异物","清分控制",MB_OK|MB_ICONINFORMATION);
            break;
        default:
            FmDistMan->InsertLog("Start Failed");
            FmDistMan->Pause();
            MessageBox(FmDistMan->Handle,"请放好票据或检查清分机内是否有异物","清分控制",MB_OK|MB_ICONINFORMATION);;
            debugout("ret =[%d]\n",ret);
            return -1;
    }
    return 0;
}
//---------------------------------------------------------------------------
int HDPCtrlUI::Stop()
{
    //TODO: Add your source code here
    return HDPStop(hHDP);
}
//---------------------------------------------------------------------------

/**
    设置票据清分口袋
*/
int HDPCtrlUI::SetPocket(int poc)
{
    //TODO: Add your source code here
    int pocket = ( poc <= 0 ) ? ERROR_POCKET-1 : poc-1;
    int ret = HDPSetPocket(hHDP,pocket);

    Application->ProcessMessages();
    return ret;
}
//---------------------------------------------------------------------------
/**
    清分机接口函数
    将轨道票据清分到错误口袋里面
*/
int HDPCtrlUI::Purge()
{
    //TODO: Add your source code here
    return HDPPurge(hHDP);
}
//----------------------------------------------------------------------

/**
    清分数据处理函数
*/
void OnHDP(int num)
{
    FmDistMan->OnHDPCall(num);
}
//---------------------------------------------------------------------------
void OnErrorHDP(int err)
{
    switch( err )
    {
        case 0:
             Application->ProcessMessages();
        break;
    }
}
//---------------------------------------------------------------------------
/**
    清分机接口函数
    得到清分机的ocr结果
*/
int HDPCtrlUI::GetMicr(int num, char*fname, char *rname)
{
    //TODO: Add your source code here
    memset( theData->micr_buf,0,sizeof(theData->micr_buf));
    memset( theData->ocr_buf,0,sizeof(theData->ocr_buf));
    int ret = HDPGetMicr(hHDP,num,theData->micr_buf);
    char errcode[3];
    char ocr[255];
    memset(ocr,0,sizeof(ocr));
    String tmp = theData->micr_buf;
    if(tmp.Trim().Length()<=0)
    {
        return 999;
    }

    FmDistMan->skipHead = 106;
    if(theData->micr_buf[2]=='F')
    {
        return 999;
    }

    SaveImage(num,fname,rname);

    strncpy(errcode,theData->micr_buf+3,2);
    if( SetupDlg->ckOcrRecognize->Checked == false )
    {
        strcpy(theData->ocr_buf,theData->micr_buf+FmDistMan->skipHead);
    }
    else
    {
        if( FmDistMan->_nDstMode == MD_FIRST )
        {
            ret = NewHDPGetOcr(  fname,ocr);
            strcpy(theData->ocr_buf,ocr);
        }
        else
        {
            strcpy(theData->ocr_buf,theData->micr_buf+FmDistMan->skipHead);
        }
    }
    if(strncmp(errcode,"00",2)==0)
    {
      
    }
    else  if (strncmp(errcode, "33", 2) == 0)
    {
        return 33;
    }
    else
    {
        debugout("read micr error [%s]\n",errcode);
        if(strncmp(errcode,"32",2)==0 )
        {
            return 32;
        }
        return 888;
    }

    Application->ProcessMessages();
    return ret;
}
//---------------------------------------------------------------------------

/**
    切割清分码
*/
int HDPCtrlUI::DecodeMicr(DISTDATA *pData)
{
    //TODO: Add your source code here
    char  buf[56];
    int length;
    String  Mic;

    memset(&(theData->theVch),0,sizeof(VchStuff));
    memset(buf,0,sizeof(buf));

    if(strlen(theData->micr_buf)<= FmDistMan->skipHead) return 34;
    Mic = theData->micr_buf + FmDistMan->skipHead;

    debugout("mic dec [%s]\n", theData->micr_buf);
    debugout("mic dec [%s]\n",Mic.c_str());

    DcInfo info;
	SplitDc sd;
    sd.init(FmDistMan->Distval);

    //磁码各段分割区域
    sd.split(Mic.c_str(),info);
    String z;
    z = info.exchno.c_str();

    //删除=号
    while( (length=z.Pos("="))>0  || (length=z.Pos("-"))>0)
    {
        z.Delete(length,1);
    }
            
    info.exchno = z.c_str();

    //自动补0
    strncpy(theData->theVch.accNo,"000000000000000000",FmDistMan->Distval.accLen);                               //帐号长度,具体需要调整
    if(info.accno.length()>0 && info.accno.length()<FmDistMan->Distval.accLen )
        strcpy(theData->theVch.accNo+FmDistMan->Distval.accLen-info.accno.length(),info.accno.c_str());
    else if(info.accno.length() ==FmDistMan->Distval.accLen)
        strcpy(theData->theVch.accNo,info.accno.c_str());

    if(info.vchno.length()>0)
        strcpy(pData->vchno,info.vchno.c_str());
    else
        strncpy(pData->vchno,"0000000000",FmDistMan->Distval.vchLen);
   // if(info.exchno.length()>0)
   //     strcpy(pData->exchno,info.exchno.c_str());  //交换号长度,具体需要调整
   // else
       // strncpy(pData->exchno,"000000000000",FmDistMan->Distval.exnoLen);
    //   strncpy(pData->exchno,"000000000000",FmDistMan->Distval.exnoLen);
    if(info.vchtp.length()>0)
    {
        strcpy(pData->pbcvt,info.vchtp.c_str());
    }
    else
    {
        strcpy(pData->pbcvt,"00");
    }
    if(info.money.length()>0)
        strcpy( pData->amount, info.money.c_str());  //金额长度,具体需要配置

    strcpy(pData->codeacc,theData->theVch.accNo);

    strcpy(pData->codeline,Mic.c_str());
    strncpy(pData->ocr,theData->ocr_buf,sizeof(pData->ocr));

    if(strlen(pData->vchno)!= FmDistMan->Distval.vchLen)
    {
        FmDistMan->InsertLog("读凭证号错");
        debugout("Vchno Error [%s] \n",pData->vchno);
        return -1;
    }

    if(info.exchno.length()!=FmDistMan->Distval.exnoLen)
    {
        FmDistMan->InsertLog("读交换号错");
        debugout("交换号错 [%s]\n",pData->exchno);
        return -2;
    }
    else
    {
        strncpy(pData->exchno,info.exchno.c_str()+2,6);    
    }
    if(strlen(pData->amount)!= FmDistMan->Distval.amountLen )                                              //金额长度,具体需要调整
    {
        FmDistMan->InsertLog("读金额错!");
        debugout("金额错 [%s\n",pData->amount);
        strcpy(pData->amount,"");
        return -5;
    }

    debugout("[%s][%s][%s][%s]\n",pData->vchno,pData->exchno,theData->theVch.accNo,pData->amount);

    //判断是否读错
    char errcode[3];
    memset(errcode,0,sizeof(errcode));
    strncpy(errcode,theData->micr_buf+3,2);
    if(strcmp(errcode,"00") != 0) return 33;

    //MICR码和OCR码进行相互校验
    int ret = IsGoodMicr(theData->micr_buf+FmDistMan->skipHead,theData->ocr_buf);
    if(ret && FmDistMan->_nDstMode == MD_FIRST)
    {
        return ret;
    }

 
    return 0;
}
//---------------------------------------------------------------------------
/**
    函数
    检查磁码是否合格
*/
int HDPCtrlUI::IsGoodMicr(const char* mir, const char* ocr)
{
    //TODO: Add your source code here
   
    char mir_check[100];
	char ocr_check[100];
	memset(mir_check,0,sizeof(mir_check));
	memset(ocr_check,0,sizeof(ocr_check));
	strcpy(mir_check,mir);
	strcpy(ocr_check,ocr);

	int length = strlen(mir_check);

    for(int i=0;i<length;i++)                                                                                                        //
	{
        if(!isdigit(mir_check[i]))
        {
            mir_check[i] = 'F';
        }
    }

    length = strlen(ocr_check);
    for(int i=0;i<length;i++)                                                                                                                              //
    {
        if(!isdigit(ocr_check[i]))
        {
            ocr_check[i] = 'F';
        }
	}

    String mir_str = mir_check;
    String ocr_str = ocr_check;

    //去除F
    while( (length = mir_str.Pos("F"))>0)
    {
        mir_str.Delete(length,1);
    }

    while( (length=ocr_str.Pos("F"))>0)
    {
        ocr_str.Delete(length,1);
    }

    //进行比较
    if(strcmp(mir_str.Trim().c_str(),ocr_str.Trim().c_str())==0)
    {
        return 0;
    }
    else
    {
        int pos = ocr_str.Pos(mir_str.Trim().c_str());
        if(pos > 0 ) return 0;
        
        debugout("mir != ocr \n");
        debugout("trim mir =[%s] \n",mir_str.Trim().c_str());
        debugout("trim ocr =[%s] \n",ocr_str.Trim().c_str());
        return 888;
    }
}
//---------------------------------------------------------------------------
int HDPCtrlUI::OnEndorse( char * str, int pos)
{
    //TODO: Add your source code here
    int ret = HDPEndorse(hHDP,str,pos);
    return ret;
}
//---------------------------------------------------------------------------

/**
    保存清分图像
*/
int HDPCtrlUI::SaveImage(int count,char * fname,char *rname)
{
    //TODO: Add your source code here
    int ret = HDPSaveImage(hHDP,count,fname,rname);
    return ret;
}
//---------------------------------------------------------------------------
