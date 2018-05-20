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
#include "distorCtrl.h"
#include "distControllor.h"
#include "dstData.h"
#include <dstcommon.h>
#include <zcommon.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

HANDLE __stdcall InitDistCtrl( HWND Handle )
{
	distorCtrl * pCtrl = new distorCtrl;
	pCtrl->hMainWnd = Handle;
	int ret = pCtrl->Init();
	if( ret )
		
	{
		debugout("init error [%s]\n");
		return NULL;
	}

	return (HANDLE)pCtrl;
}

void __stdcall ReleaseDistCtrl( HANDLE Handle )
{
	delete (distorCtrl *)Handle;
}
int __stdcall PauseDist(HANDLE hDistCtrl)
{
	distorCtrl *pCtrl = (distorCtrl *)hDistCtrl;
	
	return pCtrl->Pause();
}

int __stdcall StopDist(HANDLE hDistCtrl)
{
	distorCtrl *pCtrl = (distorCtrl *)hDistCtrl;
	
	return pCtrl->End();
}

DISTDATA* __stdcall GetOneData(HANDLE hDistCtrl)
{
	distorCtrl *pCtrl = (distorCtrl *)hDistCtrl;
	
	return pCtrl->GetDistData();
}

bool __stdcall SetOneData( HANDLE hDistCtrl, DISTDATA *pData )
{
	distorCtrl *pCtrl = (distorCtrl *)hDistCtrl;
	
	return pCtrl->SetDistData(pData);
}

int __stdcall StoreDistData(HANDLE hDistCtrl)
{
	distorCtrl *pCtrl = (distorCtrl *)hDistCtrl;
	
	return pCtrl->StoreData();
}


int __stdcall GetPocketNo(HANDLE hDistCtrl)
{
	distorCtrl *pCtrl = (distorCtrl *)hDistCtrl;
	
	return pCtrl->GetPocketNum();
}


int __stdcall CalTotalAmount(HANDLE hDistCtrl, double* dramount, int* drcount, 
							 double* cramount, int* crcount)
{
	distorCtrl *pCtrl = (distorCtrl *)hDistCtrl;
	
	return pCtrl->pDstData->CalTotalAmount(*dramount, *drcount, *cramount, *crcount);
}

int __stdcall DelSpecialItems(HANDLE hDistCtrl, int rightIndex)
{
	distorCtrl *pCtrl = (distorCtrl *)hDistCtrl;
	
	return pCtrl->pDstData->DelRightItems(rightIndex);
}

int __stdcall GetAlltems(HANDLE hDistCtrl, DISTDATA* pData, int* count)
{
	distorCtrl *pCtrl = (distorCtrl *)hDistCtrl;
	
	return pCtrl->pDstData->GetRightItems(pData, count);
}

int __stdcall StartDstEx(HANDLE hDistCtrl,DISTCTRLDATA* pDcd)
{
	distorCtrl *pCtrl = (distorCtrl *)hDistCtrl;
	int ret = 0;
	switch( pDcd->dstState )
	{
		case ST_STOP:
			 ret = pCtrl->Start(pDcd);
			 break;
		case ST_PAUSE:
			 ret = pCtrl->goOn();
		 	 break;
		default:
			 break;
	}
	return ret;
}
