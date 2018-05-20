/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#include <vcl.h>
#pragma hdrstop
#include <zcommon.h>
#include "ztwain.h"
#include "batscan.h"
#include <zim.h>

#include "SetupDlg.h"
#include "replacescan.h"
// *******************************************************************************
// inline functions start here.
// *******************************************************************************
ZTwain::ZTwain()
{
	m_hDsmDll = 0;                    //// Dll 句柄
	DsmEntry = 0;                     //// 函数句柄
	m_bDSMIsOpen = FALSE;			//是否打开DSMOpen

	m_twState = NO_TWAIN_STATE;       ////状态

	bShowUI = false;
	m_appID = DefAppId;

	bTrace = TRUE;

	SetState(PRE_SESSION);
  
}

ZTwain::~ZTwain()
{
	CloseScanner();
}

int ZTwain::OpenScanner(HWND hWnd )
{
  
    twClose = twStop = 0;
    m_hWnd = hWnd;
    if (m_twState >= SOURCE_MANAGER_OPEN || OpenSourceManager())      ////m_twState=SOURCE_MANGER_LOADED
    {
	   	if (m_twState >= SOURCE_OPEN || OpenDefaultSource())          ////m_twState=SOURCE_OPEN
	   	{
	    	return TRUE;
	    }
        else
	    {
		    TWMessageBox("打开扫描仪失败!\t");
            return FALSE;
	   	}
	}
    else
	{
	    TWMessageBox("Open DSM Err");
	}
	return FALSE;
}

ZTwain::StopScanner()
{
	twStop = 1;
	return 0;
}

ZTwain::CloseScanner()
{
	twClose = 1;
        m_bDSMIsOpen = FALSE;
	if( m_twState <= SOURCE_ENABLED )
	{
		CloseSourceManager();
		UnloadSourceManager();
	}
	return 0;
}

int ZTwain::ResetScanner()
{
	twStop = 0;
	twClose = 0;
	if (m_twState >= SOURCE_MANAGER_OPEN || OpenSourceManager())
	{
		if (m_twState >= SOURCE_OPEN || OpenDefaultSource())
		{
				return TRUE;
		} else
		{
			TWMessageBox("打开扫描仪失败!\t");
		}
	} else
	{
		TWMessageBox("打开扫描仪失败!\t");
	}
	return FALSE;
}

int ZTwain::Scan(int st)
{
    _st = st;                                               ////////////// Step 4 Source Open was done; m_twState=SOURCE_OPEN
	if( m_twState==SOURCE_OPEN ) EnableSource();            //////Actting step 4,4-5  init NegotiateScan ,enable source

	MSG msg;
	memset(&msg, 0, sizeof(MSG));

    CanLeaveMessageLoop = false;              
	while ( m_twState>=SOURCE_ENABLED && GetMessage(&msg, NULL, 0, 0) )
	{
		if (!ProcessTWMessage(&msg))                        ////////Actting step 5-6,recognize that the data transfer ready
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int ZTwain::LoadSourceManager(void)                         ////Load Dlls and its' entry of function
{
	char		szSM[256];
	OFSTRUCT	of;

	if (m_twState >= SOURCE_MANAGER_LOADED)	return TRUE;    ////  false

	GetWindowsDirectory(szSM, sizeof(szSM));                ///Check
	if (szSM[lstrlen(szSM)-1] != '\\')	lstrcat(szSM, "\\");

	lstrcat(szSM, DSM_FILENAME);	                        // could crash! DSM_FILENAME=TWAIN.DLL
	if (OpenFile(szSM, &of, OF_EXIST) != -1)
	{
		m_hDsmDll = LoadLibrary(szSM);                      //// load dll
	} else
	{
		if (bTrace)
		{
			TWMessageBox("LoadDS() failed!");
			return -1;
		}
		m_hDsmDll = NULL;
	}
	if (IsValidHandle(m_hDsmDll)) {
		DsmEntry = (DSMENTRYPROC) GetProcAddress(m_hDsmDll, DSM_ENTRYPOINT); ////load function
		if (DsmEntry) {
			SetState(SOURCE_MANAGER_LOADED);              //// m_twState=SOURCE_MANAGER_LOADED
		} else {
			if (bTrace)
			{
				TWMessageBox("LoadDSM from dll failed!");
				return -2;
			}
			FreeLibrary(m_hDsmDll);
			m_hDsmDll = NULL;
		}
	} else {
		DsmEntry = NULL;
	}

	if (m_twState != SOURCE_MANAGER_LOADED && bTrace) TRACE("TWAIN: LoadSourceManager() failed.\n");
	return (m_twState >= SOURCE_MANAGER_LOADED);
}

int ZTwain::UnloadSourceManager(void)                   ///unload Dlls and its entry
{
	twRC = TWRC_SUCCESS;
	if (m_twState == SOURCE_MANAGER_LOADED)
	{
		if (m_hDsmDll)
		{
			FreeLibrary(m_hDsmDll);
			m_hDsmDll = NULL;
		}
		DsmEntry = NULL;
		SetState(PRE_SESSION);
	}
	return (m_twState == PRE_SESSION);
}

int ZTwain::OpenSourceManager()
{
	if (LoadSourceManager())
	{
		SM(DG_CONTROL, DAT_PARENT, MSG_OPENDSM, &m_hWnd);           ////m_twState=SOURCE_MANAGER_OPEN
                m_bDSMIsOpen = TRUE;
		if (m_twState != SOURCE_MANAGER_OPEN && bTrace) TRACE("TWAIN: OPENDSM failed.\n");
	}
	return (m_twState >= SOURCE_MANAGER_OPEN);
}

int ZTwain::CloseSourceManager()
{
	CloseSource();		                               // close source if open
	UnloadSourceManager();
	if (m_twState >= SOURCE_MANAGER_OPEN) {
		SM(DG_CONTROL, DAT_PARENT, MSG_CLOSEDSM, &m_hWnd);
		if (m_twState >= SOURCE_MANAGER_OPEN && bTrace)
			TRACE("TWAIN: CloseDSM failed.\n");
	}
	return (m_twState < SOURCE_MANAGER_OPEN);
}

int ZTwain::OpenDefaultSource(void)
{
	if (m_twState != SOURCE_MANAGER_OPEN) return FALSE;
	m_dsID.ProductName[0] = '\0';
	m_dsID.Id = 0;
	SM(DG_CONTROL, DAT_IDENTITY, MSG_OPENDS, &m_dsID);     //// m_twState=SOURCE_OPEN
	if (m_twState != SOURCE_OPEN && bTrace) TRACE("TWAIN: OPENDS failed.\n");

	return (m_twState == SOURCE_OPEN);
}

int ZTwain::CloseSource(void)
{
	twRC = TWRC_SUCCESS;

	if (m_twState == SOURCE_ENABLED)
		DisableSource();
	if (m_twState == SOURCE_OPEN)
	{
		DS(DG_CONTROL, DAT_IDENTITY, MSG_CLOSEDS, &m_dsID);
		if (m_twState == SOURCE_OPEN && bTrace) TRACE("TWAIN: CloseDS_1 failed.\n");
	}
	return (m_twState < SOURCE_OPEN);
}

int ZTwain::EnableSource(void)
{
	if (m_twState != SOURCE_OPEN) {
		if (bTrace) TRACE("TWAIN Warning: wrong state for EnableSource()\n");
		return FALSE;
	}

  	NegotiateSource();      ////////////Accting step 4

	memset(&m_twUI, 0, sizeof(TW_USERINTERFACE));
	m_twUI.ShowUI = bShowUI;
	m_twUI.hParent = m_hWnd;

	if( m_twState >= SOURCE_OPEN )
	{
		if( twRC == TWRC_FAILURE )
			DS(DG_CONTROL, DAT_USERINTERFACE, MSG_DISABLEDS, &m_twUI);
	}
	else return false;

	DS(DG_CONTROL, DAT_USERINTERFACE, MSG_ENABLEDS, &m_twUI);  /////step 4--5   request the acquisition of data form the source

	if (m_twState != SOURCE_ENABLED && bTrace) TRACE("TWAIN: ENABLEDS failed.\n");
	return (m_twState == SOURCE_ENABLED);
}

int ZTwain::DisableSource(void)
{
	if (m_twState == SOURCE_ENABLED) {
		DS(DG_CONTROL, DAT_USERINTERFACE, MSG_DISABLEDS, &m_twUI);
		if (m_twState == SOURCE_ENABLED && bTrace) TRACE("TWAIN: DISABLEDS failed.\n");
	}
	return (m_twState < SOURCE_ENABLED);
}

int ZTwain::EndXfer(void)
{
	if (m_twState == TRANSFERRING)
	{
		DS(DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER, &twPendingXfer);
	}
	return m_twState < TRANSFERRING;
}

int ZTwain::DS(unsigned long dg, unsigned dat, unsigned msg, void FAR *pd)
{
	int bSuccess = FALSE;
	ASSERT(m_twState >= SOURCE_OPEN);
	if (DsmEntry) {
		twRC = (*DsmEntry)(&m_appID, &m_dsID, dg, dat, msg, (TW_MEMREF)pd);
		bSuccess = (TWRC_SUCCESS == twRC);

       /* if(bSuccess)
        {
            (*DsmEntry)(&m_appID, &m_dsID, dg, dat, MSG_GET,&m_Status);
			debugout("Call Twain failed %d, condition=%d\n",(int)twRC, (int)m_Status.ConditionCode);
        }   */
		if (DG_CONTROL == dg)
                   {
			if (DAT_EVENT == dat)
                         {
				if (MSG_PROCESSEVENT == msg)
                                  {
				    if (MSG_XFERREADY == ((TW_EVENT FAR *)pd)->TWMessage)
                                          {
						if (bTrace) TRACE0("TWAIN: received MSG_XFERREADY\n");
						SetState(TRANSFER_READY);
					  }
                                     else if (MSG_CLOSEDSREQ == ((TW_EVENT FAR *)pd)->TWMessage)
                                          {
						if (bTrace) TRACE0("TWAIN: received MSG_CLOSEDSREQ\n");
						SetState(SOURCE_ENABLED);
				         }
			         }
		          }
                        else if (DAT_PENDINGXFERS == dat)
                         {
				if (MSG_RESET == msg)
                                {
				   if (bSuccess) SetState(SOURCE_ENABLED);
			     	}
                               else if (MSG_ENDXFER == msg)
                              {
				  if (bSuccess)
                                    {
				   	SetState(((TW_PENDINGXFERS FAR *)pd)->Count ? TRANSFER_READY : SOURCE_ENABLED);
				    }
			     }
			  }
                         else if (DAT_USERINTERFACE == dat)
                            {
				if (MSG_DISABLEDS == msg)
                                {
					if (bSuccess) SetState(SOURCE_OPEN);
				}
                                else if (MSG_ENABLEDS == msg)
                                {
					if (bSuccess) SetState(SOURCE_ENABLED);
				}
			  }
                        else if (DAT_IDENTITY == dat)
                         {
				if (MSG_CLOSEDS == msg)
                                {
					if (bSuccess) SetState(SOURCE_MANAGER_OPEN);
				}
			  }
	          	}
            else if (DG_IMAGE == dg)
              {
			if (DAT_IMAGENATIVEXFER == dat)
                          {
				if (MSG_GET == msg)
                                 {
				    if (TWRC_XFERDONE == twRC)
                                     {
				  	SetState(TRANSFERRING);
				     }
                                  else if (TWRC_CANCEL == twRC)
                                    {
						SetState(TRANSFERRING);
				    }
                                 else
                                   {
						SetState(TRANSFER_READY);
				   }
				}
			}
			else if (DAT_IMAGEMEMXFER == dat)
                           {
				if (MSG_GET == msg)
                                 {
				    if (TWRC_SUCCESS == twRC)
                                     {
				    	SetState(TRANSFERRING);
				     }
                                    else if (TWRC_XFERDONE == twRC)
                                     {
				    	SetState(TRANSFERRING);
				     }
                                   else if (TWRC_CANCEL == twRC)
                                     {
				    	SetState(TRANSFERRING);
				      }
                                   else
                                   {
			  	      SetState(TRANSFER_READY);
				   }
				}
			}
		}
	}
	return bSuccess;
}

int ZTwain::SM(unsigned long dg, unsigned dat, unsigned msg, void FAR *pd)
{
	int bSuccess = FALSE;
	twRC = TWRC_SUCCESS;
	ASSERT(m_twState >= SOURCE_MANAGER_LOADED);
	ASSERT(m_hDsmDll);
	if (m_hDsmDll)
	{
		twRC = (*DsmEntry)(&m_appID, NULL, dg, dat, msg, (TW_MEMREF)pd);
		bSuccess = (TWRC_SUCCESS == twRC);
		if (DG_CONTROL == dg)
                 {
			if (DAT_PARENT == dat)
                        {
				if (MSG_OPENDSM == msg)
                                {
					if (bSuccess) SetState(SOURCE_MANAGER_OPEN);
				}
                                else if (MSG_CLOSEDSM == msg)
                                {
					if (bSuccess) SetState(SOURCE_MANAGER_LOADED);
				}
			  }
                        else if (DAT_IDENTITY == dat)
                        {
				if (MSG_OPENDS == msg)
                                {
					if (bSuccess) SetState(SOURCE_OPEN);
				}
			}
		}
	}
	return bSuccess;
}

/*TW_STATE ZTwain::State(void)
{
	return m_twState;
}
*/
void ZTwain::SetState(TW_STATE nS)
{
#ifdef _DEBUG
	if (bTrace) {
		const char *pzState[] = {
			"",
			"1:PRE_SESSION",
			"2:SOURCE_MANAGER_LOADED",
			"3:SOURCE_MANAGER_OPEN",
			"4:SOURCE_OPEN",
			"5:SOURCE_ENABLED",
			"6:TRANSFER_READY",
			"7:TRANSFERRING"
		};
		TRACE("TWAIN: State %s -> %s\n", pzState[m_twState], pzState[nS]);
	}
#endif
	m_twState = nS;			        	                                // update the 'global'
}

BOOL ZTwain::ProcessTWMessage(LPMSG lpMsg)
{
	TW_UINT16  twRC = TWRC_NOTDSEVENT;
	TW_EVENT   twEvent;

	memset(&twEvent, 0, sizeof(TW_EVENT));
	if (m_twState>=SOURCE_ENABLED                                       /*&& twStop == 0 && twClose == 0*/ )
	{
		twEvent.pEvent = (TW_MEMREF)lpMsg;
		DS(DG_CONTROL, DAT_EVENT, MSG_PROCESSEVENT, &twEvent );         /////step 5-6  recognize transfer ready?

        char twm[100];
        sprintf(twm,"TWMessage=%d [%X]",(int)twEvent.TWMessage,(int)twEvent.TWMessage);
        switch (twEvent.TWMessage)
		{
			case MSG_XFERREADY:
				TWTransferImage();                                       //////step 5-6 choose transfer mode
				break;
			case MSG_CLOSEDSREQ:
			case MSG_CLOSEDSOK:
				break;
			case MSG_NULL:
                return 0;
			default:
				CloseSourceManager();
				break;
		}
	}
	DisableSource();
	return (twRC==TWRC_DSEVENT);                                        // returns TRUE or FALSE
}

void ZTwain::TWTransferImage()
{
	if( m_TransMode == TWSX_NATIVE )	    	DoNativeTransfer();
	else if( m_TransMode == TWSX_FILE )		    DoFileTransfer();
        else if (m_TransMode == TWSX_MEMORY)    DoMemoryTransfer();
	else TWMessageBox("Invalid TransMode Value!");

	return;
}

void ZTwain::DoNativeTransfer()
{
    TW_UINT32           hBitMap = NULL;
    HANDLE              hbm_acq = NULL;
    TW_IMAGEINFO      info;

    memset(&info, 0, sizeof(TW_IMAGEINFO));
    DS(DG_IMAGE, DAT_IMAGEINFO, MSG_SET, &info);

	memset(&twPendingXfer, 0, sizeof(TW_PENDINGXFERS));
	twPendingXfer.Count = 0;
	do
	{
		DS(DG_IMAGE, DAT_IMAGENATIVEXFER, MSG_GET, &hBitMap);
		switch (twRC)
		{
			case TWRC_XFERDONE:             // Session is in State 7

 				hbm_acq = (HBITMAP)hBitMap;
                EndXfer();

				m_twState = TRANSFERRING;
				m_ErrMsg.err_code = TWRC_XFERDONE;
				strcpy(m_ErrMsg.err_msg, "Xfer: TWRC_XFERDONE Success!");
                                OnSendMsgToParentWnd(0,&m_ErrMsg);
                SaveBitmap(m_hWnd,hbm_acq);
                break;

			case TWRC_CANCEL:               // Session is in State 7
				EndXfer();
				m_ErrMsg.err_code = TWRC_CANCEL;
				strcpy(m_ErrMsg.err_msg, "XFer: TWRC_CANCEL");
			   	OnSendMsgToParentWnd(0,&m_ErrMsg);
                                twStop = 1;
				break;
			case TWRC_FAILURE:              //Session is in State 6
			default:                        //Sources should never return any other RC
				EndXfer();
				m_ErrMsg.err_code = TWRC_FAILURE;
				strcpy(m_ErrMsg.err_msg, "XFer: TWRC_FAILUE");
			     	OnSendMsgToParentWnd(0,&m_ErrMsg);
				break;
		}
	} while (twPendingXfer.Count != 0 && twRC != TWRC_FAILURE&& twStop == 1&& twClose != 1 );

        if( twStop == 1 )
	{
		m_twState = SOURCE_ENABLED;
		twStop = 0;
		EndXfer();
	}
	if( twClose == 1 )
	{
		CloseSourceManager();
		UnloadSourceManager();
	}

    CanLeaveMessageLoop = true;

	return;
}

void ZTwain::DoFileTransfer()
{
    //TW_UINT16           twRC2 = TWRC_FAILURE;
	TW_SETUPFILEXFER    setup;
    TW_UINT16       	rcc;
    String              tmp;
    int                 count=1;
	OFSTRUCT            of;
    imgfile_node_t      m_one_record;
    bool                save=true;
    int                 tmprotate;
    int                 ret;
    long                imgcount=0;

	memset(&twPendingXfer, 0, sizeof(TW_PENDINGXFERS));
	memset(&setup, 0, sizeof(TW_SETUPFILEXFER));
	memset(&of, 0, sizeof(OFSTRUCT));
    memset(&m_one_record,0,sizeof(m_one_record));

    strcpy(m_one_record.pkgno,m_pkgno.c_str());
    m_one_record.prefix=m_scannerParam.prefix[0];

   if(m_scannerParam.needRotate==1&&rotateSuc==0)
    {
        tmprotate=0;
    }
    else
    {
        tmprotate=m_scannerParam.onevch.rotate*90;
    }

    m_one_record.pkgflag=m_scannerParam.pkgflag;
    m_one_record.dpi=m_scannerParam.onevch.dpi;
    m_one_record.calx=m_scannerParam.calx;
    m_one_record.caly=m_scannerParam.caly;

    m_one_record.state=0;


    if(m_scannerParam.onevch.colortype==0)
        strcpy(m_one_record.colortype,"RGB24BIT");
    else if(m_scannerParam.onevch.colortype==1)
        strcpy(m_one_record.colortype,"GRAY256");
    else
        strcpy(m_one_record.colortype,"BlackWhite");


	twPendingXfer.Count = 0;
    String fname;
    String bname;
    int    frotate;
    String fmt;

    debugout("tw_Format [%d]\n",tw_Format);


    if( tw_Format == TWFF_JFIF )
		fmt = ".jpg" ;
	else if( tw_Format == TWFF_BMP )
		fmt = ".bmp" ;
    else if( tw_Format == TWFF_TIFF )
        fmt = ".tif" ;
    else
        fmt = ".NoDefineFormat" ;

	do
	{
        //如果只扫正面 则所有图像都是正面
        //如果只扫反面 则扫描图像为正面、反面，需要将正面图像屏蔽（偶数面）

		debugout("m_scannerParam.onevch.sideB = %d,m_scannerParam.onevch.sideA=%d",m_scannerParam.onevch.sideB,m_scannerParam.onevch.sideA);

        if(!m_scannerParam.onevch.sideB&&m_scannerParam.onevch.sideA)           //扫正面
        {
            count=count+1;
           // curMaxCount = curMaxCount+1;

            tmp.sprintf("%s-%08d-%04d-%cA",m_pkgno.c_str(),baseCount,count,m_one_record.prefix);
            m_one_record.fcount=imgcount+1;

            m_one_record.side='A';

            if(m_scannerParam.onevch.procA)   m_one_record.proctype =1;
            else                              m_one_record.proctype =0;

            m_one_record.rotate=tmprotate;
            fname = tmp;
            bname = "None";
            frotate = tmprotate;
            m_one_record.fcount=baseCount+count;
        }

        else if(m_scannerParam.onevch.sideA&&m_scannerParam.onevch.sideB)       //双面
        {
            if((count+1)%2==0)
            {
                count=count+1;
               // curMaxCount = curMaxCount+1;
                tmp.sprintf("%s-%08d-%04d-%cA",m_pkgno.c_str(),baseCount,count/2,m_one_record.prefix);

                m_one_record.side='A';
                if(m_scannerParam.onevch.procA)  m_one_record.proctype =1;
                else                             m_one_record.proctype =0;

                m_one_record.rotate=tmprotate;
                fname = tmp;
                frotate = tmprotate;
            }
            else
            {

                tmp.sprintf("%s-%08d-%04d-%cB",m_pkgno.c_str(),baseCount,count/2,m_one_record.prefix);
                count=count+1;
                m_one_record.side='B';
                if(m_scannerParam.onevch.procB)  m_one_record.proctype =1;
                else                             m_one_record.proctype =0;

                if(m_scannerParam.needRotate==1&&rotateSuc==0)
                {
                        m_one_record.rotate=180-tmprotate;
                }
                else
                {
                        m_one_record.rotate=360-tmprotate;
                }
                bname = tmp;

            }
           m_one_record.fcount=baseCount+count;
        }

        else                                                                            //反面
        {
           if((count+1)%2==0)
            {
                count=count+1;
                // curMaxCount = curMaxCount+1;
                tmp.sprintf("%s-%08d-%04d-%cA",m_pkgno.c_str(),baseCount,count,m_one_record.prefix);

                m_one_record.side='A';
                if(m_scannerParam.onevch.procA)  m_one_record.proctype =1;
                else                             m_one_record.proctype =0;

                m_one_record.rotate=tmprotate;
                fname = tmp;
                frotate = tmprotate;

            }
            else
            {

                tmp.sprintf("%s-%08d-%04d-%cB",m_pkgno.c_str(),baseCount,count,m_one_record.prefix);
                count=count+1;

                m_one_record.side='B';

                if(m_scannerParam.onevch.procB)  m_one_record.proctype =1;
                else                             m_one_record.proctype =0;

                if(m_scannerParam.needRotate==1&&rotateSuc==0)
                {
                        m_one_record.rotate=180-tmprotate;
                }
                else
                {
                        m_one_record.rotate=360-tmprotate;
                }
                bname = tmp;
            }
           m_one_record.fcount=baseCount+count;

        }
        strcpy( setup.FileName, tmp.c_str());

        strcat(setup.FileName,fmt.c_str());

        if((m_scannerParam.onevch.sideB==1&&m_scannerParam.onevch.sideA==0)&&((count)%2==0))    //只扫反面
        {
           save=false;
           strcpy(setup.FileName,"None");
           fname = "None";
        }

        strcpy(m_one_record.fname,setup.FileName);
  //**********************************************************

   		setup.Format = tw_Format;//tw_Format;
		setup.VRefNum = 1;

        DS(DG_CONTROL, DAT_SETUPFILEXFER, MSG_SET, &setup);

 //*************************************************************************

        if (twRC == TWRC_SUCCESS)
        {
            DS(DG_IMAGE, DAT_IMAGEFILEXFER, MSG_GET, NULL);      /////////////File Transfer
         }

        char twm[100];

        debugout("\n\n\n\n In Scan twRC=[%d]\n",twRC);
		switch (twRC)
  		{

			case TWRC_XFERDONE:
				m_twState = TRANSFERRING;
				m_ErrMsg.err_code = TWRC_XFERDONE;
				strcpy(m_ErrMsg.err_msg, setup.FileName);
				EndXfer();
                
				debugout(" twRC=== fname = %s",fname);

                if(!m_scannerParam.onevch.sideB&&m_scannerParam.onevch.sideA)           //只扫正面
                {
					debugout("正面");
                    fname = fname+fmt;
					debugout("fname ==== %s",fname);
                    if(_st == 0)
                    {
                        FormScan->insertFname(fname.c_str(),frotate,m_one_record.fcount);
                    }
                    else
                        FormReplace->insertFname(fname.c_str(),frotate,m_one_record.fcount);
                }
                if(m_scannerParam.onevch.sideB&&m_scannerParam.onevch.sideA &&  (count+1)%2 == 0 )           //扫双面,扫反面时显示
                {
                    fname = fname+fmt;
                    bname = bname+fmt;
                    if(_st == 0)
                    {
                        FormScan->insertFname(fname.c_str(),frotate,m_one_record.fcount);
                    }
                    else
                        FormReplace->insertFname(fname.c_str(),frotate,m_one_record.fcount);
                }
                if(m_scannerParam.onevch.sideB&&!m_scannerParam.onevch.sideA &&  (count)%2 == 0 )           //扫反面
                {
                    bname = bname + fmt;
                    if( _st == 0)
                    {
                        FormScan->insertFname(fname.c_str(),frotate,m_one_record.fcount);
                    }
                    else
                        FormReplace->insertFname(fname.c_str(),frotate,m_one_record.fcount);
                }

                OnSendMsgToParentWnd( 0, &m_ErrMsg);
				break;
			case TWRC_CANCEL:
				m_ErrMsg.err_code = TWRC_CANCEL;
				strcpy(m_ErrMsg.err_msg, "XFer: TWRC_CANCEL");
				OnSendMsgToParentWnd(0,&m_ErrMsg);
                twStop = 1;
                debugout("\n\n\n\n scan cancel\n");
				break;
			case TWRC_FAILURE:
                m_ErrMsg.err_code = TWRC_FAILURE;
				strcpy(m_ErrMsg.err_msg, "XFer: TWRC_FAILURE");
				OnSendMsgToParentWnd(0,&m_ErrMsg);
                twStop = 1;
                debugout("\n\n\n\n scan failure\n");
                break;
			default:
				m_ErrMsg.err_code = TWRC_FAILURE;
				strcpy(m_ErrMsg.err_msg, "XFer: TWRC_FAILURE");
				OnSendMsgToParentWnd(0,&m_ErrMsg);
                twStop = 1;
                debugout("\n\n\n\n scan default\n");
				break;
		}   

	} while ( twPendingXfer.Count != 0 && twRC != TWRC_FAILURE && twStop != 1 && twClose != 1 );

	m_twState = SOURCE_ENABLED;
  	twStop = 0;
   	EndXfer();
	CloseSourceManager();
    UnloadSourceManager();

    CanLeaveMessageLoop = true;

    int total;
    if(!m_scannerParam.onevch.sideA)
       scancount=count-1;
    else
       scancount=count/2;
        
   	return;
}

void ZTwain::DoMemoryTransfer()
{
     TW_SETUPMEMXFER   setup;
     TW_IMAGEMEMXFER   dat;
     TW_CAPABILITY     twCap;
     TW_IMAGEINFO      info;
     TW_PALETTE8       pal;
     HANDLE            h_image;
     LPBITMAPINFO      pdib = NULL;
     unsigned char     TW_HUGE  *ptr = NULL;
     TW_UINT16         PixelFlavor = 0;
     pTW_ONEVALUE      pOneV = NULL;
     TW_UINT16         Units = 0;
     TW_UINT16         index = 0;
     TW_UINT32         size = 0;
     DWORD sizex;
     int               blocks=0;
     float XRes=0;
     float YRes=0;

     memset(&info, 0, sizeof(TW_IMAGEINFO));

     twPendingXfer.Count = 0;
     do
     {

       DS(DG_IMAGE, DAT_IMAGEINFO, MSG_GET, &info);

       ///////////////get size of  image
       size = (((((TW_INT32)info.ImageWidth*info.BitsPerPixel+31)/32)*4)
							* info.ImageLength);
       ////////////// ensure  buffer size and  blocks of buffer
       memset(&setup, 0, sizeof(TW_SETUPMEMXFER));
       DS(DG_CONTROL, DAT_SETUPMEMXFER, MSG_GET, &setup);

       blocks = (int)(size /(setup.Preferred));
       size = (blocks+1) * setup.Preferred;


       h_image = GlobalAlloc(GHND,
			size + sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD));


       pdib = (LPBITMAPINFO)GlobalLock(h_image);


       // fill in the image information *
       pdib->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	   pdib->bmiHeader.biWidth = info.ImageWidth;
	   pdib->bmiHeader.biHeight = info.ImageLength;
   	   //Only 1 is supported
	   pdib->bmiHeader.biPlanes = 1;
	   pdib->bmiHeader.biBitCount = info.BitsPerPixel;
	   //This application does not support compression
	   pdib->bmiHeader.biCompression = BI_RGB;
	   pdib->bmiHeader.biSizeImage = size;

       twCap.Cap = ICAP_UNITS;
       twCap.ConType = TWON_DONTCARE16;
       twCap.hContainer = NULL;
       DS(DG_CONTROL,DAT_CAPABILITY,MSG_GETCURRENT,&twCap);

       pOneV = (pTW_ONEVALUE)GlobalLock(twCap.hContainer);
					Units = (TW_UINT16)(pOneV->Item);
					GlobalUnlock(twCap.hContainer);
					GlobalFree((HANDLE)twCap.hContainer);

       XRes = FIX32ToFloat(info.XResolution);
       YRes = FIX32ToFloat(info.YResolution);


       switch(Units)
       {
	    	case TWUN_INCHES:
		 		 pdib->bmiHeader.biXPelsPerMeter = (LONG)((XRes*2.54)*100);
				 pdib->bmiHeader.biYPelsPerMeter = (LONG)((YRes*2.54)*100);
				 break;
		   	case TWUN_CENTIMETERS:
		  		 pdib->bmiHeader.biXPelsPerMeter = (LONG)(XRes*100);
				 pdib->bmiHeader.biYPelsPerMeter = (LONG)(YRes*100);
				 break;
		   	case TWUN_PICAS:
			case TWUN_POINTS:
			case TWUN_TWIPS:
			case TWUN_PIXELS:
			default:
				 pdib->bmiHeader.biXPelsPerMeter = 0;
				 pdib->bmiHeader.biYPelsPerMeter = 0;
				 break;
            }
       switch (info.PixelType)
				{
					case TWPT_BW:
						pdib->bmiHeader.biClrUsed = 2;
						pdib->bmiHeader.biClrImportant = 0;

						/*
						* Get CAP_PIXELFLAVOR to determine colors
						* fill in the palette information
						*/
						twCap.Cap     = ICAP_PIXELFLAVOR;
						twCap.ConType = TWON_DONTCARE16;
						twCap.hContainer = NULL;
                        DS(DG_CONTROL,DAT_CAPABILITY,MSG_GETCURRENT,&twCap);
                        if (twCap.ConType != TWON_ONEVALUE)
							{
								PixelFlavor = TWPF_CHOCOLATE;
							}
							else
							{
								pOneV = (pTW_ONEVALUE)GlobalLock(twCap.hContainer);
								PixelFlavor = (TW_UINT16)(pOneV->Item);
								GlobalUnlock(twCap.hContainer);
							}
                        GlobalFree((HANDLE)twCap.hContainer);
                        if (PixelFlavor == 0)
						{
							/*
							* 0=Black
							*/
							pdib->bmiColors[0].rgbGreen = 0x0000;
							pdib->bmiColors[0].rgbBlue = 0x0000;
							pdib->bmiColors[0].rgbReserved = 0;

							pdib->bmiColors[1].rgbRed = 0x00FF;
							pdib->bmiColors[1].rgbGreen = 0x00FF;
							pdib->bmiColors[1].rgbBlue = 0x00FF;
							pdib->bmiColors[1].rgbReserved = 0;
						}
						else
						{

							pdib->bmiColors[0].rgbRed = 0x0000;
							pdib->bmiColors[0].rgbRed = 0x00FF;
							pdib->bmiColors[0].rgbGreen = 0x00FF;
							pdib->bmiColors[0].rgbBlue = 0x00FF;
							pdib->bmiColors[0].rgbReserved = 0;

							pdib->bmiColors[1].rgbRed = 0x0000;
							pdib->bmiColors[1].rgbGreen = 0x0000;
							pdib->bmiColors[1].rgbBlue = 0x0000;
							pdib->bmiColors[1].rgbReserved = 0;
						}
						break;
                  case TWPT_GRAY:
						pdib->bmiHeader.biClrUsed = 256;
						for (index=0; index<256; index++)
						{
							pdib->bmiColors[index].rgbRed = (BYTE) index;
							pdib->bmiColors[index].rgbGreen = (BYTE) index;
							pdib->bmiColors[index].rgbBlue = (BYTE) index;
							pdib->bmiColors[index].rgbReserved = 0;
						}
						break;
                  case TWPT_RGB:
						pdib->bmiHeader.biClrUsed = 0;
						break;
                    
					case TWPT_PALETTE:
					case TWPT_CMY:
					case TWPT_CMYK:
					case TWPT_YUV:
					case TWPT_YUVK:
					case TWPT_CIEXYZ:
					default:
                      DS(DG_IMAGE,DAT_PALETTE8,MSG_GET,&pal);
                      pdib->bmiHeader.biClrUsed = pal.NumColors;
							pdib->bmiHeader.biClrImportant = 0;
							for (index=0; index<pal.NumColors; index++)
							{
								pdib->bmiColors[index].rgbRed = pal.Colors[index].Channel1;
								pdib->bmiColors[index].rgbGreen = pal.Colors[index].Channel2;
								pdib->bmiColors[index].rgbBlue = pal.Colors[index].Channel3;
								pdib->bmiColors[index].rgbReserved = 0;
							}
                            break;
                     }

       ptr = (unsigned char TW_HUGE *) pdib;
       ptr+= sizeof(BITMAPINFOHEADER);
	   ptr+= pdib->bmiHeader.biClrUsed * sizeof(RGBQUAD);

       memset(&dat, 0, sizeof(TW_IMAGEMEMXFER));
       dat.Compression =TWON_DONTCARE16;
       dat.BytesPerRow =TWON_DONTCARE32;
       dat.Columns     =TWON_DONTCARE32;
       dat.Rows        =TWON_DONTCARE32;
       dat.XOffset     =TWON_DONTCARE32;
       dat.YOffset     =TWON_DONTCARE32;
       dat.BytesWritten=TWON_DONTCARE32;

       dat.Memory.Flags=TWMF_APPOWNS |TWMF_POINTER;
       dat.Memory.Length=setup.Preferred;
       dat.Memory.TheMem=ptr;

       do
      {
          DS(DG_IMAGE, DAT_IMAGEMEMXFER, MSG_GET, &dat);
  		switch (twRC)
		{
		   case TWRC_SUCCESS:
                     ptr += dat.BytesWritten;
                     dat.Memory.TheMem=ptr;
                  	 break;
                    case TWRC_XFERDONE:
                      m_ErrMsg.err_code = TWRC_XFERDONE;
                      strcpy(m_ErrMsg.err_msg, "Xfer: TWRC_XFERDONE!");
                      OnSendMsgToParentWnd(0,&m_ErrMsg);
                      EndXfer();
                      GlobalUnlock(h_image);
                      FlipBitMap(m_hWnd,h_image, info.PixelType);
                      SaveBitmap(m_hWnd ,h_image);

                      GlobalFree(h_image);
                          break;
          	   case TWRC_CANCEL:
          	      m_ErrMsg.err_code = TWRC_CANCEL;
		      strcpy(m_ErrMsg.err_msg, "XFer: TWRC_CANCEL");
		      OnSendMsgToParentWnd(0,&m_ErrMsg);
                      EndXfer();
                      GlobalUnlock(h_image);
                      GlobalFree(h_image);
                      twStop = 1;
		          break;
                   case TWRC_FAILURE:
                      EndXfer();
                      GlobalUnlock(h_image);
                      GlobalFree(h_image);
		  default:
         	       EndXfer();
		       m_ErrMsg.err_code = TWRC_FAILURE;
		       strcpy(m_ErrMsg.err_msg, "XFer: TWRC_FAILURE");
		       OnSendMsgToParentWnd(0,&m_ErrMsg);
                       GlobalUnlock(h_image);
                       GlobalFree(h_image);
		 break;
		}

      }while (twRC==TWRC_SUCCESS);

    }while( twPendingXfer.Count != 0 );

	return;

}


void ZTwain::OnSendMsgToParentWnd(HGLOBAL hDib, ZTW_ERRMSG* pMsg)
{
	SendMessage(m_hWnd,PM_XFERDONE, (WPARAM)hDib, (LPARAM)pMsg);
}

int ZTwain::NegotiateSource()
{
	TW_UINT16	rc;
	TW_FIX32	v32;
	TW_CAPABILITY	twCap;
	pTW_ONEVALUE	pval;
	TW_IMAGELAYOUT	twLayout;

    TW_IDENTITY NewDSIdentity;
	memset(&NewDSIdentity, 0, sizeof(TW_IDENTITY));
		rc = (*DsmEntry)(&m_appID, NULL,DG_CONTROL,DAT_IDENTITY,MSG_GETDEFAULT,
                        (TW_MEMREF)&NewDSIdentity);
	debugout("scanner [%s]",NewDSIdentity.ProductName);

    twCap.hContainer = GlobalAlloc(GHND, sizeof(TW_ONEVALUE));
    pval = (pTW_ONEVALUE)GlobalLock(twCap.hContainer);

// ************************Set paper autofeeding*******************************
		twCap.Cap        = CAP_AUTOFEED;
		twCap.ConType    = TWON_ONEVALUE;
		pval->ItemType   = TWTY_BOOL;
		pval->Item       = TRUE;
		rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
						  (TW_MEMREF)&twCap);
        debugout("\nAutoFeeding  rc=[%d]\n ",rc);
// ************************Set paper autoscan**********************************
		twCap.Cap        = CAP_AUTOSCAN;
		twCap.ConType    = TWON_ONEVALUE;
		pval->ItemType   = TWTY_BOOL;
		pval->Item       = TRUE;
		rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
						  (TW_MEMREF)&twCap);
        debugout("\nAutoScan  rc=[%d]\n ",rc);
// ***********************Set X-resolution ************************************
    twCap.Cap        = ICAP_XRESOLUTION;
    twCap.ConType    = TWON_ONEVALUE;
    pval->ItemType   = TWTY_FIX32;
    v32.Whole = tw_Resolution;
    v32.Frac  = 0;
    pval->Item       = *((TW_UINT32 *)&v32);
    rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                       (TW_MEMREF)&twCap);
#ifdef WATCH_NEGOTIATE_INFO
    UNTwainError("Negotiate XRESOLUTION", rc);
#endif
    debugout("\n X-resolution  rc=[%d]\n ",rc);
// ************************Set Y-resolution ************************************
    twCap.Cap        = ICAP_YRESOLUTION;
    twCap.ConType    = TWON_ONEVALUE;
    pval->ItemType   = TWTY_FIX32;
    v32.Whole = tw_Resolution;
    v32.Frac  = 0;
    pval->Item       = *((TW_UINT32 *)&v32);
    rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                         (TW_MEMREF)&twCap);
#ifdef WATCH_NEGOTIATE_INFO
    UNTwainError("Negotiate YRESOLUTION", rc);
#endif
    debugout("\n Y-solution  rc=[%d]\n ",rc);
//*************************Set Pixel type**************************************
    twCap.Cap        = ICAP_PIXELTYPE;
    twCap.ConType    = TWON_ONEVALUE;
    pval->ItemType   = TWTY_UINT16;
    pval->Item       = (TW_UINT32)tw_Color;
    rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                       (TW_MEMREF)&twCap);
 #ifdef WATCH_NEGOTIATE_INFO
    UNTwainError("Negotiate PIXELTYPE", rc);
#endif
    debugout("\n Pixeltype  rc=[%d]\n ",rc);
// ************************Set BitDepth  **************************************
    twCap.Cap        = ICAP_BITDEPTH;
    twCap.ConType    = TWON_ONEVALUE;
    pval->ItemType   = TWTY_UINT16;
    pval->Item       = 24;
    rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                       (TW_MEMREF)&twCap);
    debugout("\n BitDepth  rc=[%d]\n ",rc);
 //***********************Set Memory  or File Transfer  Mode******************
    twCap.Cap       =ICAP_XFERMECH;
    twCap.ConType   =TWON_ONEVALUE;
    pval->ItemType  =TWTY_UINT16;
    pval->Item      =TransMode;           ///////TWSX_FILE  TWSX_FILE2
    rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                         (TW_MEMREF)&twCap);
    debugout("\n Transfer Mode  rc=[%d]\n ",rc);
// **************************Set ImageFileFormat ******************************
  //  if(m_scannerParam.onevch.format)
   {
    twCap.Cap        = ICAP_IMAGEFILEFORMAT;
    twCap.ConType    = TWON_ONEVALUE;
    pval->ItemType   = TWTY_UINT16;
    pval->Item       = tw_Format ;      //TWCP_JPEG;
    rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                       (TW_MEMREF)&twCap);
    debugout("\n Format  rc=[%d]\n ",rc);
    }
    // **************************Compressed Mode **********************************
    if(rc==0&&tw_Format==TWFF_JFIF)
    {
        twCap.Cap        =  ICAP_COMPRESSION;
        twCap.ConType    = TWON_ONEVALUE;
        pval->ItemType   = TWTY_UINT16;
        pval->Item       = TWFF_JFIF;      //TWCP_JPEG;   //TWFF_JFIF
        rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                       (TW_MEMREF)&twCap);
        debugout("\n Compressed Mode  rc=[%d]\n ",rc);
    }

// ************************Set Hightlight type ********************************
	twCap.Cap		= ICAP_HIGHLIGHT;
	twCap.ConType	= TWON_ONEVALUE;
	pval->ItemType	= TWTY_FIX32;
	pval->Item		= tw_Highlight;
    rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                       (TW_MEMREF)&twCap);
    debugout("\n Hightlight  rc=[%d]\n ",rc);
//************************ Set  Sense of pixel *********************************
   twCap.Cap       =ICAP_PIXELFLAVOR;
   twCap.ConType   =TWON_ONEVALUE;
   pval->ItemType  =TWTY_UINT16;
   pval->Item      =TWPF_CHOCOLATE;
   rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                       (TW_MEMREF)&twCap);
   debugout("\n Sense of Pixel  rc=[%d]\n ",rc);
// **********************Set Shadow ********************************************

	twCap.Cap		= ICAP_SHADOW;
	twCap.ConType	= TWON_ONEVALUE;
	pval->ItemType	= TWTY_FIX32;
	pval->Item		= tw_Shadow;
	rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
						(TW_MEMREF)&twCap);
    debugout("\n Shadow  rc=[%d]\n ",rc);
//  *********************Set Gamma **********************************************
  	twCap.Cap		= ICAP_GAMMA;
	twCap.ConType	= TWON_ONEVALUE;
	pval->ItemType	= TWTY_FIX32;
	pval->Item		= tw_Gamma;
	rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
						(TW_MEMREF)&twCap);
    debugout("\n Gamma  rc=[%d]\n ",rc);
//  **********************Set Units *********************************************
    twCap.Cap        = ICAP_UNITS;
    twCap.ConType    = TWON_ONEVALUE;
    pval->ItemType   = TWTY_UINT16;
    pval->Item       = (TW_UINT32)tw_Units;
    rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                      (TW_MEMREF)&twCap);
    debugout("\n Unit  rc=[%d]\n ",rc);
//  **********************Set Rotation ******************************************
    if(m_scannerParam.needRotate==1)
    {
        twCap.Cap        = ICAP_ROTATION;
        twCap.ConType    = TWON_ONEVALUE;
        pval->ItemType   = TWTY_FIX32;
        pval->Item       = m_scannerParam.onevch.rotate*90;
        rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                      (TW_MEMREF)&twCap);
        rotateSuc=rc;
        debugout("\n Rotate rc=[%d] rotate=[%d]\n",rc,m_scannerParam.onevch.rotate*90);
    }
//  **********************Set Scan one or double faces**************************
    twCap.Cap        = CAP_DUPLEXENABLED;
    twCap.ConType    = TWON_ONEVALUE;
    pval->ItemType   = TWTY_BOOL;
    if(m_scannerParam.onevch.sideB)
        pval->Item       = true;
    else
        pval->Item       = false;
   
    rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                      (TW_MEMREF)&twCap);

    debugout("\n Double Scan  rc=[%d]\n ",rc);

    //对于富士通型号扫描仪需要关闭该功能
    if( strncmp(NewDSIdentity.ProductName,"fi-***",3) !=0  && strncmp(NewDSIdentity.ProductName,"FUJ-***",3) !=0 )
    {
	    twCap.Cap        = ICAP_AUTOMATICDESKEW;
        twCap.ConType    = TWON_ONEVALUE;
        pval->ItemType   = TWTY_BOOL;
        pval->Item       = true;
        rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
		    (TW_MEMREF)&twCap);
    }
    
    if( strncmp(NewDSIdentity.ProductName,"Kodak Scanner: i600",5) !=0 )
    {
        twCap.Cap        =     ICAP_AUTOMATICBORDERDETECTION;//ICAP_FLIPROTATION;//ICAP_AUTOMATICBORDERDETECTION;//;ICAP_PATCHCODEDETECTIONENABLED;

        twCap.ConType    = TWON_ONEVALUE;
        pval->ItemType   = TWTY_BOOL;
        pval->Item       = true;
        rc = (*DsmEntry)(&m_appID, &m_dsID, DG_CONTROL, DAT_CAPABILITY, MSG_SET,
                      (TW_MEMREF)&twCap);
    }

    {
        // ***********************Set  Layer Out  ***************************************
        twLayout.Frame.Left.Whole   = (int)tw_Left;
        twLayout.Frame.Left.Frac    = (TW_UINT16)(65536*(tw_Left-(int)tw_Left));
        double X = tw_Left + tw_Size_X;
        twLayout.Frame.Right.Whole  = (int)X;
        twLayout.Frame.Right.Frac   = (TW_UINT16)(65536*(X-(int)X));

        twLayout.Frame.Top.Whole    = 0;
        twLayout.Frame.Top.Frac     = 0;
        twLayout.Frame.Bottom.Whole = (int)tw_Size_Y;
        twLayout.Frame.Bottom.Frac  = (TW_UINT16)(65536*(tw_Size_Y-(int)tw_Size_Y));

        rc = (*DsmEntry)(&m_appID, &m_dsID, DG_IMAGE, DAT_IMAGELAYOUT, MSG_SET,
                       (TW_MEMREF)&twLayout);
        debugout("\n Layout  rc=[%d]\n ",rc);
    }
    GlobalUnlock(twCap.hContainer);
    GlobalFree(twCap.hContainer);
    if(rc)  debugout("rc =[%d]\n",rc);
    return (0);
}

WORD ZTwain::DibNumColors (VOID FAR *pv)
{
	int Bits = 0;
	LPBITMAPINFOHEADER lpbi = NULL;
	LPBITMAPCOREHEADER lpbc = NULL;

	ASSERT(pv);
	lpbi = ((LPBITMAPINFOHEADER)pv);
	lpbc = ((LPBITMAPCOREHEADER)pv);

	if (lpbi->biSize != sizeof(BITMAPCOREHEADER))
	{
		if (lpbi->biClrUsed != 0)
			return (WORD)lpbi->biClrUsed;
		Bits = lpbi->biBitCount;
	}
	else
		Bits = lpbc->bcBitCount;

	switch (Bits)
	{
		case 1:	return 2;
		case 4:	return 16;
		case 8:	return 256;
		default: return 0;
	}
}

HANDLE ZTwain::FileBitMap_To_RGB( char* FileName )
{
	OFSTRUCT of;
	short hfile = NULL;

	memset(&of, 0, sizeof(OFSTRUCT));
	if ((hfile = OpenFile(FileName, &of, OF_READ)) != -1)
	{
		DWORD dwSize = GetFileSize((HANDLE)hfile, NULL);
		dwSize -= sizeof(BITMAPFILEHEADER);

		BITMAPFILEHEADER header;
		memset(&header, 0, sizeof(BITMAPFILEHEADER));
		_lread(hfile, (LPSTR)&header, sizeof(BITMAPFILEHEADER));

		HANDLE	hbm_acq = GlobalAlloc(GHND, dwSize);
		if (hbm_acq)
		{
			BYTE *ptr = (BYTE*)GlobalLock(hbm_acq);
			_lread(hfile, ptr, dwSize);
			GlobalUnlock(hbm_acq);
		}
		_lclose(hfile);
		return hbm_acq;
	}
	else return 0;
}

float ZTwain::FIX32ToFloat (TW_FIX32 fix32)
{
	float   floater = 0;

	floater = (float) fix32.Whole + (float) (fix32.Frac/65536.0);
	return(floater);
}

void ZTwain::SaveBitmap(HWND hWnd, HGLOBAL _hDIB)
{
	/*
	*	Declared locals
	*/
		static OPENFILENAME ofn;
	char szDirName[256];
	char szFile[256];
	char szFileTitle[256];
	char szFilter[256];
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER *pBIH = NULL;

	HANDLE  hf = NULL;
	/*
	*	Initialize all structures
	*/
	memset(&ofn, 0, sizeof(OPENFILENAME));
	memset(szDirName, 0, sizeof(char[256]));
	memset(szFile, 0, sizeof(char[256]));
	memset(szFileTitle, 0, sizeof(char[256]));
	memset(szFilter, 0, sizeof(char[256]));
	memset(&bfh, 0, sizeof(BITMAPFILEHEADER));

//		We are expecting a valid HWND
	ASSERT(hWnd);

	if(_hDIB)
	{
  		hf = CreateFile("Test.bmp",
 					 	GENERIC_WRITE, 0,
 					 	(LPSECURITY_ATTRIBUTES)NULL,
  					 	CREATE_ALWAYS,
   					 	FILE_ATTRIBUTE_NORMAL,
					 	(HANDLE)NULL);

        if(hf)
		{
			//	Get a pointer to the beginning of the DIB
            pBIH = (BITMAPINFOHEADER*)GlobalLock(_hDIB);
			if(pBIH)
			{
				DWORD dwBytesWritten = 0l;
				// Fill in the Bitmap File Header
				bfh.bfType = ( (WORD) ('M' << 8) | 'B');
   				bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
					((((pBIH->biWidth * pBIH->biBitCount + 31)>>5)<<2) * pBIH->biHeight);
                bfh.bfReserved1 = 0;
				bfh.bfReserved2 = 0;
   		        bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
				ASSERT(bfh.bfSize > sizeof(BITMAPFILEHEADER));
                WriteFile(hf,&bfh.bfType,sizeof( bfh.bfType), &dwBytesWritten, NULL);
                WriteFile(hf,&bfh.bfSize,sizeof( bfh.bfSize), &dwBytesWritten, NULL);
                WriteFile(hf,&bfh.bfReserved1,sizeof(bfh.bfReserved1),&dwBytesWritten, NULL);
                WriteFile(hf,&bfh.bfReserved2,sizeof(bfh.bfReserved2),&dwBytesWritten, NULL);
                WriteFile(hf,&bfh.bfOffBits,sizeof(bfh.bfOffBits),&dwBytesWritten, NULL);
		   		WriteFile(hf, pBIH, bfh.bfSize-sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
				// Done now
                GlobalUnlock(_hDIB);
           	}
			CloseHandle(hf);
		}
	}


   	return;
}

void ZTwain::FlipBitMap(HWND hWnd, HANDLE hBM, TW_INT16 PixType)

{
	HANDLE                  temp;
	LPBITMAPINFO            pdib;
	BYTE*                   pDib;
	unsigned char TW_HUGE   *pbuffer;
	unsigned char TW_HUGE   *tempptr;
	unsigned char TW_HUGE   *tempptrsave;
	LONG                    Width;
	LONG                    Height;
	LONG                    Linelength;
	LONG                    indexH;
	DWORD                   SizeImage;
	WORD                    BitCount;
	DWORD                   offset;
	TW_UINT16               pixels;
	TW_INT32                items;
	TW_UINT16                i;
	BYTE                    SaveRed;
	BYTE                    SaveBlue;

	pDib = (BYTE*)GlobalLock(hBM);
	pdib = (LPBITMAPINFO)pDib;

	Width = pdib->bmiHeader.biWidth;
	Height = pdib->bmiHeader.biHeight;
	SizeImage = pdib->bmiHeader.biSizeImage;
	BitCount = pdib->bmiHeader.biBitCount;


	if ((temp = GlobalAlloc(GHND, SizeImage))!=NULL)
	{
		tempptr =(unsigned char TW_HUGE*) GlobalLock(temp);
		tempptrsave = tempptr;

		// calculate offset to start of the bitmap data
		offset = sizeof(BITMAPINFOHEADER);
		offset += pdib->bmiHeader.biClrUsed * sizeof(RGBQUAD);

		Linelength = (((Width*BitCount+31)/32)*4);

		//Goto Last line in bitmap
		offset += (Linelength * (Height-1));

		#ifdef WIN32
			pDib = pDib + offset - Linelength;
		#endif

		//For each line
		for (indexH = 1; indexH < Height; indexH++)
		{
			#ifdef WIN32
				memcpy(tempptr, pDib, Linelength);
				pDib -= (Linelength);
			#else
				MemoryRead(hBM, offset, tempptr, Linelength);
				offset -= (Linelength);
			#endif
				tempptr += Linelength;
		}

		// Copy temp over hBM
		pbuffer = (unsigned char TW_HUGE *) pdib;
		pbuffer += sizeof(BITMAPINFOHEADER);
		pbuffer += pdib->bmiHeader.biClrUsed * sizeof(RGBQUAD);

		#ifdef WIN32
			memcpy(pbuffer, tempptrsave, SizeImage);
		#else
			MemoryRead(temp, 0, pbuffer, SizeImage);
		#endif

		//Flip RGB color table
		if (PixType == TWPT_RGB)
		{
			pbuffer = (unsigned char TW_HUGE *)pdib;
			pbuffer += sizeof(BITMAPINFOHEADER);
			pbuffer += pdib->bmiHeader.biClrUsed * sizeof(RGBQUAD);

			pixels = (TW_UINT16)pdib->bmiHeader.biWidth;
			for (items = 0; items < Height; items++)
			{
				tempptr = pbuffer;
				for (i=0; i<pixels; i++)
				{
					//Switch Red byte and Blue byte
					SaveRed = (BYTE)*tempptr;
					SaveBlue = (BYTE)*(tempptr+2);
					(BYTE)*tempptr = SaveBlue;
					(BYTE)*(tempptr+2) = SaveRed;
					//increment to next triplet
					tempptr += 3;
				}
				pbuffer += Linelength;
			}
		}

		//Unlock
		GlobalUnlock(hBM);
		GlobalUnlock(temp);

		//Free
		GlobalFree(temp);
	}
	else
	{
	    GlobalUnlock(hBM);
	}
	return;
}

int ZTwain::SetData(scanner_info_t * param)
{
    //TODO: Add your source code here
    memset(&m_scannerParam,0,sizeof(m_scannerParam));
    //X方向校准值
    m_scannerParam.calx=param->calx;
    //Y方向校准值
    m_scannerParam.caly=param->caly;
    //基准偏移
    m_scannerParam.offset=0;
    //扫描仪标号
    strcpy(m_scannerParam.prefix,param->prefix);

    // 启用硬件旋转
    m_scannerParam.needRotate=param->needRotate;
    //扫描面
    m_scannerParam.onevch.sideA=param->onevch.sideA;
    m_scannerParam.onevch.procA=param->onevch.procA;
    m_scannerParam.onevch.sideB=param->onevch.sideB;
    m_scannerParam.onevch.procB=param->onevch.procB;
    //旋转角度
    m_scannerParam.onevch.rotate=param->onevch.rotate;
    //扫描精度
    Resolution=param->onevch.dpi;

    //图象格式
    tw_Format= param->onevch.format; //TWFF_JFIF;//TWFF_BMP;

    if( tw_Format == 0 )    tw_Format = TWFF_BMP;
    else if( tw_Format == 1 ) tw_Format = TWFF_JFIF;
    
  

     debugout("tw_Format [%d]\n",tw_Format);

    SizeX=Resolution*param->onevch.vch_sizex -1;
    SizeY=Resolution*param->onevch.vch_sizey-1;

    m_TransMode = TransMode;
    tw_Resolution = Resolution;
    //图像色彩
    param->onevch.colortype = param->onevch.colortype;
    switch(param->onevch.colortype)
    {
        case 0:
            tw_Color = TWPT_RGB;
            break;
        case 1:
            tw_Color=TWPT_GRAY;
            break;
        case 2:
            tw_Color=TWPT_BW;
            break;
        default:
            ShowMessage("Invalid Color type");
            return -1;
        }
    tw_Highlight = Highlight;

    tw_Shadow = Shadow;
    tw_Gamma = Gamma;
    tw_Units = units;
    tw_Size_X = (double)SizeX/(double)Resolution;
    tw_Size_Y = (double)SizeY/(double)Resolution;
    tw_Left=(double)param->offset/(double)Resolution;
    debugout("offset [%d][%f]\n",param->offset,tw_Left);
    return 0;
}

int ZTwain::SetPkgno(char * pkgno,long ct)
{
    //TODO: Add your source code here
    m_pkgno=pkgno;
    baseCount = ct;
    return 0;
}


int ZTwain::SelectScanner()
{
        //关闭DS
        CloseSource();

	//打开TWain Manager
	if(!m_bDSMIsOpen)
	{
                LoadSourceManager();     //重新加载扫描仪
 
		twRC = (*DsmEntry)(&m_appID, NULL, DG_CONTROL, DAT_PARENT, MSG_OPENDSM, &m_hWnd);
		if(TWRC_SUCCESS == twRC)
		{
			m_bDSMIsOpen = TRUE;
                        SetState(SOURCE_MANAGER_OPEN);
		}else
		{
			debugout("Manager error\n");
		}
	}

	//选择设备
	TW_IDENTITY dsID;
	if (DsmEntry) {
		twRC = (*DsmEntry)(&m_appID, NULL, DG_CONTROL, DAT_IDENTITY, MSG_USERSELECT, &dsID);

		if(TWRC_SUCCESS == twRC)
		{
			m_dsID = dsID;
			return 0;
		}else
		{
                //        ShowMessage("AAAAAAAAAAAAAAAA");
			return 1;
		}
	}
	else
	{
		debugout("kong");
      //          ShowMessage("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
		return -1;
	}

 //       ShowMessage("cccccccccccccccccccccccccccccccc");

	return 0;
}

BOOL ZTwain::GetDSCap(void)
{
}


