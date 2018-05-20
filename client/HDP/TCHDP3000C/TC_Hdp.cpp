/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//

#include "stdafx.h"
#include "TCHdpMDL.h"
#include "TC_hdp.h"
#include <libmem.h>
#include <string.h>
#include <transimg.h>
#include <TC_E13bOCR.h>
#include <TC_ImageLib.h>
#include <LibZilOcr.h>
#define  HEIGHT 90

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	return TRUE;
}


HANDLE 
__stdcall InitMDL(HWND hwnd,void (*post_proc)(int num),void (*post_err)(int condition))		//Initialize TCHdpMDL and return its instance handle
{
	TCHdpMDL *theMDL = new TCHdpMDL;
	theMDL->SetMainWnd(hwnd);
	if( !theMDL->InitDlls(post_proc,post_err) ) return NULL;
	return (HANDLE)theMDL;
}

void
__stdcall FreeMDL(HANDLE handle)										//Release the TCHdpMDL instance
{
	if(handle!=NULL)
	delete (TCHdpMDL *)handle;
}

int
__stdcall HDPOpen(HANDLE handle)										//Connect with HDP sorter
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle; 
	return theMDL->Open();
}

int
__stdcall HDPClose(HANDLE handle)										//Disconnect with HDP sorter
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle; 
	return theMDL->Close();
}

int
__stdcall HDPStart(HANDLE handle,int mode, int speed, bool initflag)								//Start Sort with HDP sorter
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle;
	return theMDL->Start(mode, speed, initflag);
}

int
__stdcall HDPStop(HANDLE handle)										//Stop the HDP sorter
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle; 
	return theMDL->Stop();
}


int
__stdcall HDPGetMicr(HANDLE handle, int num, char *micr)				//Get MICR code from the HDP sorter
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle;
	return theMDL->GetMicr(num, micr);
}

int
__stdcall HDPGetOcr(HANDLE handle, int num, char *ocr)					//Get MICR code from the HDP sorter
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle;
	return theMDL->GetOcr(num, ocr);
}

int
__stdcall HDPSetPocket(HANDLE handle, int pocket)						//Set the sort pocket of the HDP sorter
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle;
	return theMDL->SetPocket(pocket);
}

int
__stdcall HDPPurge(HANDLE handle)										//Clear the current voucher in the HDP sorter
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle;
	return theMDL->Purge();
}

int
__stdcall HDPBaseSet(HANDLE handle)										//Set the base color of the HDP sorter
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle;
	return theMDL->BaseSet();
}

INT
__stdcall HDPAdSet(HANDLE handle, int mode, int speed)					//Set the speed and wide of the HDP sorter
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle;
	return theMDL->AdSet(mode, speed);
}

int
__stdcall HDPEndorse(HANDLE handle, char *str, int on)
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle;
	return theMDL->Endorse(str, on);
}

void
__stdcall HDPErrMsg(int errcode, char *err_msg)							//Return the error explaination of the HDP error code
{
}

int 
__stdcall HDPdecBankPtr(HANDLE handle)
{

	TCHdpMDL *theMDL = (TCHdpMDL *)handle;
	return theMDL->decBankPtr();
}

int 
__stdcall HDPSaveImage(HANDLE handle,int index,char *fname,char *rname)
{
	TCHdpMDL *theMDL = (TCHdpMDL *)handle;
	return theMDL->SaveImage(index,fname,rname);
}

int
 NewHDPGetOcr(char *fname,char*ocr)
{

	MemT pRGB;
	MemT pSRGB;

	unsigned char*source;

	int x,y;


	int bands;
	
	int ret = tilLoadImageFile(fname,source,x,y,bands);
	if(ret)
	{
		debugout("ti_filetoRGB error [%d]\n",ret);
		zreturn(  -2);
	}


	pSRGB.assign(source,x*y*bands);	
	
	tilImageRelease(source);

	pRGB.assign( (x-10)*90*bands);

	ret =zilocr_get_subimage(pSRGB.data(),x,y,bands,5,y-95,x-10,90,pRGB.data());
	if(ret)
	{
		debugout("zilocr_get_subimage error [%d]\n",ret);
		zreturn( -3);
	}	

	x = x-10;
	y = 90;	

	ret = E13b_OCR(pRGB.data(), x, y, 1, 200, ocr);
	if(ret)
	{
		debugout("ocr error [%d]\n",ret);
	}
	
	debugout("ocr [%s]\n",ocr);

	return ret;
}