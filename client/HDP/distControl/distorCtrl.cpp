/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#include "stdafx.h"
#include "distorCtrl.h"
#include <dstCommon.h>
#include "dstData.h"
#include <zcommon.h>

distorCtrl::distorCtrl()
{
	pDstData = new dstData;
	lastNetNo = 0;
}

distorCtrl::~distorCtrl()
{
	delete pDstData;
}

/**
	��������,�����������,����,������Ϣ
*/
int distorCtrl::Start(int nDstMode, int nDstState,int currency, int netno, int vch_tc, bool isTallyMode)
{
	
	DstMode = nDstMode;
	DstState = ST_RUN;
	if( nDstState == ST_STOP )									////////nDstState==ST_STOP
	{              
		lastNetNo = 0;
		if( nDstMode == MD_SECOND || nDstMode== MD_FIRST|| nDstMode == MD_THIRD) lastNetNo = netno;
		return pDstData->Initialize( nDstMode,currency,netno,vch_tc,isTallyMode );
	}
	
	return 0;
}

/**
	ֹͣ���
*/
int distorCtrl::End()
{
	DstState = ST_STOP;
	return 0;
}

/**
	�������
*/
int distorCtrl::goOn()
{
	DstState = ST_RUN;
	return 0;
}


/**
	���
*/
void distorCtrl::OnDist(int pocket)
{
	if( pocket <= 0 )
	{
		return;		// ERROR READING VOUCHER
	}
	
	DISTDATA *pdt = pDstData->pData;	
	char code = ( DstMode == MD_SECOND ) ? 'C' : 'A';
	{
		pdt->PockNum = pocket;
		sprintf( pdt->PockCode, "D%c%d",code, pocket );		
	}
}

/**
	�����ͣ
*/
int distorCtrl::Pause()
{
	DstState = ST_PAUSE;
	::SendMessage(hMainWnd, ZWM_DIST_CHECK, DstMode, ST_PAUSE );
	return 0;
}

/**
	��ʼ��
*/
int distorCtrl::Init()
{
	DstMode = MD_FIRST;
	DstState = ST_STOP;
	int ret = pDstData->OpenDb();
	if( ret ) return ret;
	
	return 0;
}

/**
	���ص�ǰ����
*/
DISTDATA* distorCtrl::GetDistData()
{
	return pDstData->pData;
}

/**
	�洢�������
*/
int distorCtrl::StoreData()
{
	return pDstData->StoreState(DstMode);
}

/**
	���ؿڴ���
*/
int distorCtrl::GetPocketNum(bool isEndorse)
{
	int pocket = 0;
	
	if(  DstMode == MD_FIRST ||DstMode ==MD_SECOND||DstMode ==MD_THIRD)
	{
		int netno = 0;
		try
		{
			netno = atoi( pDstData->pData->exchno);
		}
		catch(...)
		{
			netno = -1;
		}

		//�Ǳ�������,ֱ�ӷ���
		if( lastNetNo != 0 && lastNetNo != netno)
		{
			return 0;
		}
		else lastNetNo = netno;
		
		pocket = pDstData->GetPocketNum(DstMode);
	}
	else
	{
		pocket = pDstData->pData->PockNum;
	}
	
	if( !isEndorse) OnDist(pocket);                        
	return pocket;
}

/**
	�����������,��ӵ�����
*/
bool distorCtrl::SetDistData(DISTDATA *pData)
{
	memcpy( pDstData->pData, pData, sizeof(DISTDATA));

	if(atoi(pDstData->pData->pkgno)==lastNetNo || lastNetNo==0)   
	{
		pDstData->StoreList(hMainWnd);
		pDstData->MicrOK = true;
		return true;
	}
	else
		return false;
}


int distorCtrl::Start(DISTCTRLDATA *pDcd)
{
	return Start(pDcd->dstMode,pDcd->dstState,pDcd->currency,pDcd->netno,pDcd->vch_tc,pDcd->isTallyMode);
}
