#ifndef __ZTWAIN_H__
#define __ZTWAIN_H__

#include <vcl.h>
#include <windows.h>
#include <twain/twain.h>
#include <direct.h>
#include <process.h>
#include <stdio.h>
#include "scannerini.h"

#ifndef TRACE(a)
#define TRACE
#define TRACE0
#endif

#ifndef ASSERT(a)
#define ASSERT(a)
#endif
///////////////////////////TWCP TWFF   _TIFF TWFF_PICT TWFF_XMP TWFF_JFIF  TWFF_PNG
//#define	Resolution		300
//#define	SizeX		        479	          //  3.2       150dpi
//#define	SizeY		        1004		  //  6.7       150dpi
//#define	SizeX		        624	  	      //  3.2       200dpi
//#define	SizeY		        1400  	      //  6.7       200dpi
//#define	left	       	    375	    	  //  2.5
//#define   SizeX   			936		      //  3.2       300dpi
//#define   SizeY	    		2100	      //  6.7       300dpi
//#define   left		    	375	          //  2.5
//#define	Pand		        TWPT_RGB      // TWPT_BW  TWPT_GRAY TWPT_RGB TWPT_PLATETTE TWPT_CMY  TWPT_CMYK


#define		units			TWUN_INCHES
#define     TransMode       TWSX_FILE//TWSX_NATIVE//TWSX_FILE//TWSX_MEMORY//TWSX_FILE
#define		Gamma			1
#define		Highlight		0
#define		Shadow			255

// ****************************************************************
// Define symbols
// ****************************************************************
//#define DSM_NAME	"TWAIN_32.DLL"

#ifdef WIN32
  #define DSM_FILENAME   "TWAIN_32.DLL"
  #define DSM_ENTRYPOINT "DSM_Entry"
#else
  #define DSM_FILENAME "TWAIN.DLL"
  #define DSM_ENTRYPOINT "DSM_ENTRY"
#endif

#define TWMessageBox(a)  MessageBox(m_hWnd,a,"Twain Information : ", MB_OK+MB_ICONWARNING);
#define IsValidHandle(h) ((h) != NULL)
#define PM_XFERDONE	(WM_USER+20)

const TW_IDENTITY	DefAppId = {
    0,									                    // Id, filled in by SM
	{ 1, 703, TWLG_USA, TWCY_USA, "Batch Scanner 1.02"}, 	// Version : 	Version.MajorNum, Version.MinorNum,	Version.Language,
                                                            //				Version.Country,  Version.Info
    TWON_PROTOCOLMAJOR,		                                // ProtocolMajor
    TWON_PROTOCOLMINOR,			                            // ProtocolMinor
    DG_IMAGE | DG_CONTROL,		                            // SupportedGroups
    "Nie",		                            // Manufacturer
    "TCHZT",		                            // ProductFamily
    "�������ϵͳ"	                            // ProductName
};

typedef enum {
	NO_TWAIN_STATE,		                                	// 0	internal use only
	PRE_SESSION,		                                	// 1	ground state, nothing loaded
	SOURCE_MANAGER_LOADED,                                  // 2	DSM loaded but not open
	SOURCE_MANAGER_OPEN,	                                // 3	DSM open
	SOURCE_OPEN,			                                // 4	some Source open - Negotiation state!
	SOURCE_ENABLED,		                                	// 5	acquisition started
	TRANSFER_READY,			                                // 6	data ready to transfer
	TRANSFERRING			                                // 7	transfer started
} TW_STATE;

typedef enum {
	TWERR_OPEN_DSM,			                                // unable to load or open Source Manager
	TWERR_OPEN_SOURCE,		                                // unable to open Datasource
	TWERR_ENABLE_SOURCE,                                    // unable to enable Datasource
} TW_ERR;

typedef struct ZTwErrMsg
{
	int err_code;
	char err_msg[255];
} ZTW_ERRMSG, *LPZTW_ERRMSG;


struct imgfile_node_t{
		char fname[40];				/**<�ļ���*/	
		int dpi;					/**<dpiֵ*/
		double calx;				/**<ɨ����x��У׼ֵ*/
		double caly;				/**<ɨ����y��У׼ֵ*/
		int rotate;					/**<��ת�Ƕ�*/
		char pkgno[20];				/**<���κ�*/
		int pkgflag;				/**<�������ͱ�־λ��0-���� 1-������*/
		char pkgdate[10];			/**<��������*/
		int pkgsess;				/**<����*/
		char prefix;				/**<ͼ���ļ����е�ǰ׺���������ֲ�̨ͬɨ����*/
		char side;					/**<���������棺A-���� B-����*/
		int fcount;					/**<ͼ���ļ����е����ֲ���*/
		char ip[20];				/**<�ļ����ڼ����ip��ַ*/
		int state;					/**<����״̬*/
		char colortype[12];			/**<��ɫ����*/
		char clerk[20];				/**<������Ա��*/
		int proctype;				/**<��������:0-������ 1-����*/
		char reserve[512];			/**<������Ϣ*/
	};

class ZTwain
{
public:
	ZTwain();
	virtual ~ZTwain();

	int OpenScanner(HWND hWnd);
	int CloseScanner();
	int ResetScanner();
	int StopScanner();
	int Scan(int st);
    float FIX32ToFloat(TW_FIX32 );
    void SaveBitmap(HWND hWnd, HANDLE _hDIB) ;
    void FlipBitMap(HWND hWnd, HANDLE hBM, TW_INT16 PixType);
	
	int SelectScanner();
	BOOL GetDSCap(void);

public:

    int     Resolution;
    double  SizeX;
    double  SizeY;
    int SetData(scanner_info_t * param);
    scanner_info_t  m_scannerParam;
    int SetPkgno(char * pkgno,long ct);
    int scancount;
    BOOL				bShowUI;

private:
	int LoadSourceManager(void);
	int UnloadSourceManager(void);

	int OpenSourceManager(void);
	int CloseSourceManager();

	int OpenDefaultSource(void);
	int CloseSource(void);

	int EnableSource(void);
	int DisableSource(void);

	int EndXfer(void);
	int NegotiateSource();

	int SM(unsigned long dg, unsigned dat, unsigned msg, void FAR *pd);
	int DS(unsigned long dg, unsigned dat, unsigned msg, void FAR *pd);

	void SetState(TW_STATE nS);				// assume Twain in State nS

//protected:
	void TWTransferImage();
	void DoNativeTransfer();
	void DoFileTransfer();
    void DoMemoryTransfer();

	virtual void	OnSendMsgToParentWnd(HGLOBAL hDib, ZTW_ERRMSG* pMsg);
	BOOL			ProcessTWMessage(LPMSG lpMsg);
	HANDLE			FileBitMap_To_RGB( char* FileName );
	WORD			DibNumColors(VOID FAR *pv);


	TW_INT16			twRC;			  	// last result code

	TW_IDENTITY 	    m_appID;
	TW_IDENTITY	       	m_dsID;             // storage for App and DS (Source) states
	TW_USERINTERFACE    m_twUI;
	HINSTANCE			m_hDsmDll;
	HWND				m_hWnd;				// storage for Parent Window's handle
	TW_STATE			m_twState;
	TW_PENDINGXFERS	    twPendingXfer;
    int                 _st;
//new 2015-3-19
	BOOL 				m_bTwainIsInit;//�Ƿ��ʼ��
	BOOL 				m_bDSMIsOpen; //�Ƿ��DSMOpen
	BOOL 				m_bDSIsOpen;//�Ƿ���豸

//private:
	BOOL				bTrace;				// enable TRACE output
	DSMENTRYPROC 	    DsmEntry;		    // main entry of Data Source Manager
	ZTW_ERRMSG			m_ErrMsg;

	int			     	m_TransMode;
	TW_UINT16			tw_Format;
	TW_INT16			tw_Resolution;
	TW_UINT32			tw_Color, tw_Highlight, tw_Shadow, tw_Gamma, tw_Units;
	double				tw_Size_X, tw_Size_Y, tw_Left;
	int			     	twStop, twClose;
    bool                CanLeaveMessageLoop;
    String              m_pkgno;
  
    int                 rotateSuc;
    long                baseCount;
    TW_STATUS m_Status;

};


#endif //__ZTWAIN_H__