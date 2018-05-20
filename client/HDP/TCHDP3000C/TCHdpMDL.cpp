/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */

#include "stdafx.h"
#include "TCHdpMDL.h"
#include <zcommon.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#define HDP_STDCALL __stdcall
//---------------------------------------------------------------------------
typedef int (HDP_STDCALL CALLBACK* INITSCANNER)(int);
typedef int (HDP_STDCALL CALLBACK* SCANNEROPEN)(void);
typedef int (HDP_STDCALL CALLBACK* SCANNERCLOSE)(void);
typedef int (HDP_STDCALL CALLBACK* SETSCANPARAMETER)(int);
typedef int (HDP_STDCALL CALLBACK* ENDSCANNER)(void);

typedef int	(HDP_STDCALL CALLBACK* CLEARSCAN)(int);

typedef int	(HDP_STDCALL CALLBACK* GETMICRBUFFER)(int, LPSTR);	
typedef int	(HDP_STDCALL CALLBACK* GETOCRBUFFER)(int, LPSTR);	
				
typedef int	(HDP_STDCALL CALLBACK* MOVECIS)(void);
typedef int	(HDP_STDCALL CALLBACK* BASESET)(void);

typedef int	(HDP_STDCALL CALLBACK* SCANPAGEBATCH)();

typedef int (HDP_STDCALL CALLBACK* INIT_RECOGNIZER)(void);
typedef int (HDP_STDCALL CALLBACK* END_RECOGNIZER)(void);

typedef int (HDP_STDCALL CALLBACK* SETSCANMODE)(int);
typedef int (HDP_STDCALL CALLBACK* DECBANKPTR)(void);


typedef void (HDP_STDCALL CALLBACK* PROCESSSETFLAG)(LPSTR,LPSTR);

typedef BYTE *(HDP_STDCALL CALLBACK* GETBUFFERF)(void);
typedef BYTE *(HDP_STDCALL CALLBACK* GETBUFFERR)(void);
typedef int  (HDP_STDCALL CALLBACK* GETINFOF)(RECT * r);
typedef int  (HDP_STDCALL CALLBACK* GETINFOR)(RECT *r);
typedef void (HDP_STDCALL CALLBACK* SETSAVEFILENAME)(LPSTR);
typedef void (HDP_STDCALL CALLBACK* PROCESSETCFROMMEMORY)(LPSTR,int,int,int,int,LPSTR,LPSTR,LPSTR);


typedef int (HDP_STDCALL CALLBACK* SETUPSCANNER)(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int);
typedef int (HDP_STDCALL CALLBACK* ISREADYPAPER)(int,long);
typedef int (HDP_STDCALL CALLBACK* CMDSTACKER)(int);
typedef int (HDP_STDCALL CALLBACK* CMDENDOSER)(LPSTR);


typedef int (HDP_STDCALL CALLBACK* SAVEIMAGEDATA)(int,LPSTR,LPSTR);


typedef void	(*SETPOSTPROC)(void *,void *);
typedef int 	(*POSTPROC)(int num);
typedef void	(*POSTERRPROC)(int condition);

int ShtCnt = 999;
//------------------------------------------------------
struct MDLDATA
{
	HWND hMainWnd;
	int speed;
	int wide;
	//---------------------------------
	HINSTANCE			 hInstDll;

	HINSTANCE		  	 hInstDllb;
	INITSCANNER			 InitScanner;
	ENDSCANNER		     EndScanner;
	SCANNEROPEN          ScannerOpen;
	SCANNERCLOSE         ScannerClose;
	SETSCANPARAMETER     SetScanParameter;
    CLEARSCAN			 clearScan;
	MOVECIS				 moveCIS;
	BASESET				 baseSet;
	SCANPAGEBATCH		 scanPageBatch;

	GETMICRBUFFER		 getMicrBuffer;
	GETOCRBUFFER		 getOcrBuffer;
	SETUPSCANNER		 SetupScanner;

	ISREADYPAPER		 isReadyPaper;
	CMDSTACKER			 cmdStacker;
	SAVEIMAGEDATA		 saveImageData;

	SETPOSTPROC			 setPostProc;
	POSTPROC	 		 postProc;
	POSTERRPROC	 		 postErrProc;

	INIT_RECOGNIZER      Init_Recognizer;
	END_RECOGNIZER       End_Recognizer;
	PROCESSSETFLAG       ProcessSetFlag;
	GETBUFFERF           getBufferF;
	GETBUFFERR			 getBufferR;
	GETINFOF			 getInfoF;
	GETINFOR			 getInfoR;
	CMDENDOSER            cmdEndoser;

	SETSAVEFILENAME      SetSaveFileName; 
	PROCESSETCFROMMEMORY ProcessEtcFromMemory;
};

MDLDATA *theData;

//------------------------------------------------------

TCHdpMDL::TCHdpMDL()
{
	_mode = 3;
	_speed = DEFAULT_COLOR_SCAN_SPEED;
	theData = new MDLDATA;
	theData->speed = 150;
	theData->wide = 832;
}

TCHdpMDL::~TCHdpMDL()
{

	FreeDlls();                  ////
	delete theData;              ////  
}

bool
TCHdpMDL::InitDlls(void (*post_proc)(int num), void (*post_err)(int condition))
{

	theData->hInstDll = LoadLibrary("USRSAPI.dll");
	if( theData->hInstDll == NULL ) 
	{
		debugout("Load SURSAPI.dll Error\n");
		return false;
	}
	
	theData->postProc = (POSTPROC)post_proc;
	theData->postErrProc = (POSTERRPROC)post_err;

	return true;
}

void
TCHdpMDL::FreeDlls()
{
	FreeLibrary( theData->hInstDll );   
}

int
TCHdpMDL::Open()				//Connect with HDP sorter
{
	theData->InitScanner = (INITSCANNER)GetProcAddress( theData->hInstDll, "InitScanner" );

	int res;
	

	debugout("InitScanner()...\n");
    res = theData->InitScanner(1);
	if( res != 0 ) 
	{
		debugout("InitScanner() Error\n");
		zreturn(-2);
	}	
	
	res = AdSet(_mode, _speed);
	if(res )
	{
		debugout("adset() Error\n");
		zreturn(res);
	}
	
	Purge();	
	BaseSet();
	
	debugout("at %d in %s\n",__LINE__,__FILE__);
	
	return 0;
}

int
TCHdpMDL::Close()				//Disconnect with HDP sorter
{
	theData->EndScanner=(ENDSCANNER)GetProcAddress( theData->hInstDll,"EndScanner");

	theData->EndScanner();

	return 0;
}

int
TCHdpMDL::Start(int mode, int speed, bool initflag)				//Start Sort with HDP sorter
{	
 
	_mode=mode;
	_speed = speed;
	if(initflag == true)
	{
		AdSet(_mode, _speed);
	}
	theData->scanPageBatch = (SCANPAGEBATCH)GetProcAddress(theData->hInstDll,"scanPageBatch");
	theData->isReadyPaper = (ISREADYPAPER)GetProcAddress(theData->hInstDll,"isReadyPaper");



	int res = theData->scanPageBatch();		//    1: using micr
                                      
	if( res != 0 )
	{
		zreturn(-2);
	}

	int count = 0;

	while(true)
	{
		debugout("begin next ...\n");    	
		if( count >= ShtCnt ) 
		{
			break;
		}

		debugout("begin isReadyPaper...\n");
		
		int ret = theData->isReadyPaper(count,ShtCnt);
		if(ret)
		{
			debugout("isReadyPaper error [%d]\n",ret);
			break;
		}
	
		debugout("begin OnHDPCall...\n");
		int result=OnHDPCall(count);
		
		if(result==999) break;
		else if(result==888)                      //读清分码出错了
		{
			debugout("begin set Pocket ... \n");
			ret = SetPocket(10);
			if(ret) break;
		}
		else if(result!=0)
		{
			debugout("OnHDPCall error [%d]\n",result);
			break;
		}
		count=count+1;
		debugout("get out ... \n");
	}
	return 0;
}

int
TCHdpMDL::Stop()				//Stop the HDP sorter
{
	return 0;
}

int
TCHdpMDL::Purge()				//Clear the current voucher in the HDP sorter
{
	theData->clearScan = (CLEARSCAN)GetProcAddress( theData->hInstDll, "clearScan" );
	int res = theData->clearScan(630);
	if( res != 0 ) return -1;
	return 0;
}

int
TCHdpMDL::BaseSet()				//Set the base color of the HDP sorter
{
	theData->baseSet = (BASESET)GetProcAddress( theData->hInstDll, "BaseSet" );
	int res = theData->baseSet();

	if( res != 0 ) return -1;

	Sleep(2000);
	return 0;
}


int
TCHdpMDL::GetMicr(int num, char *micr)				//Get MICR code from the HDP sorter
{
	theData->getMicrBuffer = (GETMICRBUFFER)GetProcAddress(theData->hInstDll,"getMicrBuffer");

	int res = theData->getMicrBuffer( num, micr );
	if( res != 0)
	{
		zreturn(0);
	}
	debugout("mir =[%d][%s]\n",num,micr);
	return 0;
}

bool
TCHdpMDL::IsGoodMicr(const char* mic,const char* ocr)
{
	return 0;
}

int
TCHdpMDL::GetOcr(int num, char *code_buffer)				//Get MICR code from the HDP sorter
{
 	
	theData->getOcrBuffer =(GETOCRBUFFER)GetProcAddress(theData->hInstDll,"getOcrBuffer");

	int res = theData->getOcrBuffer( num, code_buffer );
	if( res != 0)
	{
		zreturn(0);
	}
	debugout("ocr =[%s]\n",code_buffer);
	return 0;
}

int
TCHdpMDL::GetBankHead()				//Get BankHead from the HDP sorter
{
	return 0;
}

int
TCHdpMDL::SetPocket(int pocket)			//Set the sort pocket of the HDP sorter
{
	theData->cmdStacker = (CMDSTACKER)GetProcAddress( theData->hInstDll, "cmdStacker" );
	int num = pocket + 17;

	int res = theData->cmdStacker(num);
	return 0;
}


/************************************************************************/
/*
int WINAPI SetupScanner( 
int m_imageWidth=80,
int m_imageHeight=225,
int m_widthRatio=97,
int m_heightRatio=97,
int m_FCont=128,
int m_FBright=0,
int m_RCont=128,
int m_RBright=0,
int m_FThres=8,
int m_RThres=8,
int m_dpi=4,
int m_imageType=2,
int m_imageMode=3,
int m_scanside=0,
int m_doubleThick=3,
int m_jpgLoss=25,
int m_rotationMode=1,
int m_fitSize=1,
int m_speed=0,
int m_SoftRecog=1
)
*****************************************************************/	

int TCHdpMDL::AdSet(int mode, int speed)
{		
	_mode = mode;
	_speed = speed;
    theData->SetupScanner = (SETUPSCANNER)GetProcAddress( theData->hInstDll, "SetupScanner" );
	
 /*	int res =theData->SetupScanner( 112   ,   //wide    mm
									225   ,   //length  mm
									1    ,   //Image Width Ratio
									0    ,   //Image Height Ratio
									128   ,   //Front Contrast
									0     ,   //Front Brightness
									128   ,   //Rear  Contrast
									0     ,   //Rear  Brightness
									7     ,   //B/W   Front Image Thres
									7     ,   //B/w   Rear  Image Thres
									4     ,   //DPI   1--600  4==200DPI  2==300DPI
									2     ,   //Image Type 1==jpg  2==bmp 3==TIFF
									_mode ,   //Image Mode 0==BW 1==16Gray 2==256Gray 3==True Color 5==256Color  
									0     ,   //Scan Mode 0==double 1==Front only 2==Rear only
									6     ,   //Thickness 0(thin)---7(thick) 
									25    ,   //Jpeg Loss rate
									1     ,   //Rotate mode 1--rotate 90
									1     ,   //Cut Black area 
									0     ,   //speed 
									0         //soft Recognize 1--Recognize 
									     );
*/
	int res = theData->SetupScanner(110    ,		//wide    mm
									225   ,		//length  mm
									1    ,		//Image Width Ratio
									0    ,		//Image Height Ratio
									128   ,		//Front Contrast
									0	  ,		//Front Brightness
									128   ,		//Rear  Contrast
									0     ,		//Rear  Brightness
									7     ,		//B/W   Front Image Thres
									7     ,		//B/w   Rear  Image Thres
									4     ,		//DPI   1--600  4==200DPI  2==300DPI
									2     ,		//Image Type 1==jpg  2==bmp 3==TIFF
									_mode ,		//Image Mode 0==BW 1==16Gray 2==256Gray 3==True Color 5==256Color
									0     ,		//Scan Mode 0==double 1==Front only 2==Rear only
									4     ,		//Thickness 0(thin)---7(thick) 
									25    ,		//Jpeg Loss rate
									1     ,		//Rotate mode 1--rotate 90
									1     ,		//Cut Black area 
									speed ,		//speed 
									1,			//soft Recognize 1--Recognize 
									1,
									ShtCnt);			

	if( res != 0)   
	{
		debugout("SetupScanner() Error [%d]\n",res);
		zreturn(-1);
	}
	return 0;
}


int TCHdpMDL::SaveImage(int index,char *fname,char *rname)
{
	theData->saveImageData = (SAVEIMAGEDATA)GetProcAddress( theData->hInstDll, "saveImageData" );
   
	 int res=theData->saveImageData(index,
								    fname,
								    rname
								    );          //edge detection 0 no 1 top/bottom/height
	 return res;

	return 0;
}

int TCHdpMDL::Endorse(char *str, int on)
{
	return 0;
}

void TCHdpMDL::SetMainWnd(HWND hwnd)
{
	theData->hMainWnd = hwnd;
}

//------------------------------------------------------
int TCHdpMDL::OnHDPCall(int num)
{
	
	return (theData->postProc(num));
}

void TCHdpMDL::OnHDPErr(int condition)
{
	theData->postErrProc(condition);
}


int TCHdpMDL::decBankPtr()
{
	return 0;
}

void TCHdpMDL::setFlagStop()
{
	return ;
}
