/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//------------------------------------------------------------

#ifndef mainH
#define mainH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>

#include <dstCommon.h>
#include <ScktComp.hpp>
#include <Graphics.hpp>
#define ERROR_POCKET        1       ////   error check
#include <myplus.h>
#include <cslogin.h>
#include <map>
//---------------------------------------------------------------------------
struct MSG_STATE;
struct MSG_INFO;
class HDPCtrlUI;
#define MAX_POCKETS 9

#define DEFAULT_COLOR_SCAN_SPEED    30
#define DEFAULT_GRAY_SCAN_SPEED     180

using namespace std;

typedef  map<String,String> cfgMap;

enum
{
    POC1_COL_DIS  = 0,
    POC1_COL_CNT  = 1,
    POC1_COL_DOC  = 2
};
enum
{
    VCH1_COL_DIS  = 0,
    VCH1_COL_CNT  = 1,
    VCH1_COL_DOC  = 2,
    VCH1_COL_POS  = 3
};
enum
{
    POC2_COL_DIS  = 0,
    POC2_COL_CNT1 = 1,
    POC2_COL_CNT2 = 2
};
enum
{
    VCH2_COL_DIS  = 0,
    VCH2_COL_CNT1 = 1,
    VCH2_COL_CNT2 = 2,
    VCH2_COL_POS  = 3
};
//---------------------------------------------------------------------------
typedef struct _ACCINFO
{
    char exno[16];
    char sbno[8];
    char brno[8];
    char cdcode[4];
    char vhtype[8];
    char sorno[20];
    char accno[32];
}ACCINFO;

typedef struct _DISTINFO
{
    char   prefix[4];
    char   path[128];
    double calx;
    double caly;
    int    dpi;
    char   ip[20];
    int    haveOCR;
    int    inPutAmount;
    int    inputCheck;
    int    hdpType;
    int    vchLen;
    int    exnoLen;
    int    accLen;
    int    vchtpLen;
    int    amountLen;

    int    colorScanSpeed;
    int    grayScanSpeed;

    char   vch1_interval[4];
    char   exno1_interval[4];
    char   exno2_interval[4];
    char   accno1_interval[4];
    char   accno2_interval[4];
    char   vchtp1_interval[4];
    char   vchtp2_interval[4];
    char   amount1_interval[4];
    char   amount2_interval[4];
}DISTINFO;

typedef struct _vtMap
{
    char pbcvt[12];
    char bankvt[12];
    int  cdcode;
    int  proctype;
    char vchname[40];
}vt_map_t;

typedef list<vt_map_t> VTMAP;

//---------------------------------------------------------------------------
class TFmDistMan : public TForm
{
__published:	// IDE-managed Components
    TImageList *imgsSS;
    TImageList *imgs12;
    TPanel *Panel1;
    TLabel *Label1;
    TBevel *Bevel1;
    TBevel *Bevel2;
    TComboBox *cbMoneyKind;
    TSpeedButton *btnDstChange;
    TBitBtn *btnStart;
    TPanel *Panel4;
    TBitBtn *btnExit;
    TMemo *txtSorterState;
    TCheckBox *chManCheckResult;
    TLabel *txtEngageMode;
    TLabel *Label17;
    TStatusBar *stbDistState;
    TBitBtn *btnNetCancel;
    TImageList *imgsSign;
    TBitBtn *btnTallyCheck;
    TBitBtn *btnFinish;
    TLabel *Label5;
    TPageControl *pgDists;
    TTabSheet *tsFirst;
    TBevel *Bevel3;
    TLabel *Label3;
    TBevel *Bevel4;
    TSplitter *Splitter2;
    TLabel *Label16;
    TLabel *Label18;
    TLabel *Label19;
    TListView *lst1PocketDetail;
    TListView *lst1CheckDetail;
    TPanel *Panel2;
    TBevel *Bevel7;
    TLabel *Label2;
    TLabel *txt1PkgNo;
    TLabel *lbSetTotalMoney;
    TLabel *Label6;
    TLabel *Label9;
    TLabel *Label8;
    TLabel *txt1SortCount;
    TSpeedButton *btnBadClean;
    TPanel *Panel3;
    TPanel *panDebit;
    TLabel *Label4;
    TLabel *txt1TotalMoneyJF;
    TLabel *txt1TotalSumJF;
    TLabel *txt1TotalCountJF;
    TLabel *signJF;
    TPanel *panCredit;
    TLabel *Label7;
    TLabel *txt1TotalMoneyDF;
    TLabel *txt1TotalSumDF;
    TLabel *txt1TotalCountDF;
    TLabel *signDF;
    TDateTimePicker *txtDate;
    TTabSheet *tsSecond;
    TLabel *Label11;
    TBevel *w;
    TBevel *Bevel6;
    TSplitter *Splitter1;
    TListView *lst2CheckDetail;
    TPanel *Panel5;
    TLabel *Label12;
    TLabel *txt2PkgNo;
    TBevel *Bevel8;
    TLabel *Label13;
    TLabel *txt2SortCount;
    TLabel *Label10;
    TSpeedButton *btnHistSearch;
    TPanel *Panel6;
    TPanel *Panel8;
    TLabel *txt2pTotalCount;
    TLabel *Label14;
    TPanel *Panel9;
    TLabel *txt2dTotalCount;
    TLabel *Label15;
    TListView *lst2PocketDetail;
    TTabSheet *tsThird;
    TBevel *Bevel5;
    TLabel *Label29;
    TPanel *Panel10;
    TLabel *Label20;
    TLabel *txt3PkgNo;
    TBevel *Bevel9;
    TLabel *Label22;
    TLabel *txt3dTotalCount;
    TLabel *Label24;
    TSpeedButton *SpeedButton1;
    TPanel *Panel11;
    TPanel *Panel12;
    TLabel *txt3pTotalCount;
    TLabel *Label26;
    TPanel *Panel13;
    TLabel *txt3SortCount;
    TLabel *Label28;
    TBevel *Bevel10;
    TListView *lst3CheckDetail;
    TListView *lst3PocketDetail;
    TMainMenu *MainMenu1;
    TMenuItem *menuTally;
    TMenuItem *muReLogin;
    TMenuItem *N4;
    TMenuItem *muChgMod;
    TMenuItem *N8;
    TMenuItem *N7;
    TMenuItem *N5;
    TMenuItem *muFlush;
    TMenuItem *muRelink;
    TMenuItem *N6;
    TMenuItem *muExit;
    TMenuItem *M1;
    TMenuItem *mu1Dst;
    TMenuItem *mu2Dst;
    TMenuItem *N1;
    TMenuItem *muManCheckb;
    TMenuItem *muFastMode;
    TMenuItem *N2;
    TMenuItem *muEndorse;
    TMenuItem *N3;
    TMenuItem *muDefaultFastMode;
    TMenuItem *N9;
    TMenuItem *muSetup;
    TMenuItem *H1;
    TMenuItem *muAbout;
    TComboBox *txtSession;
        TLabel *Label21;
        TLabel *txtJFC;
        TLabel *txtDFC;
        TMenuItem *N10;
        TMenuItem *O1;
    TLabel *Label23;
    TLabel *Label25;
    TLabel *txtPbcCountJF;
    TLabel *txtPbcCountDF;
    void __fastcall muAboutClick(TObject *Sender);
    void __fastcall btnStartClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnFinishClick(TObject *Sender);
    void __fastcall btnDstChangeClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall mu1DstClick(TObject *Sender);
    void __fastcall mu2DstClick(TObject *Sender);
    void __fastcall btnNetCancelClick(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall lst1PocketDetailCustomDrawItem(TCustomListView *Sender,
          TListItem *Item, TCustomDrawState State, bool &DefaultDraw);
    void __fastcall lst1CheckDetailCustomDrawItem(TCustomListView *Sender,
          TListItem *Item, TCustomDrawState State, bool &DefaultDraw);
    void __fastcall muFastModeClick(TObject *Sender);
    void __fastcall lbSetTotalMoneyClick(TObject *Sender);
    void __fastcall muReLoginClick(TObject *Sender);
    void __fastcall btnTallyCheckClick(TObject *Sender);
    void __fastcall btnBadCleanClick(TObject *Sender);
    void __fastcall btnHistSearchClick(TObject *Sender);
    void __fastcall txtDateSesClick(TObject *Sender);
    void __fastcall lst2PocketDetailCustomDrawItem(TCustomListView *Sender,
          TListItem *Item, TCustomDrawState State, bool &DefaultDraw);
    void __fastcall muSetupClick(TObject *Sender);
    void __fastcall muFlushClick(TObject *Sender);
    void __fastcall muRelinkClick(TObject *Sender);
    void __fastcall muManCheckbClick(TObject *Sender);
    void __fastcall N2Click(TObject *Sender);
    void __fastcall txtSessionSelect(TObject *Sender);
        void __fastcall N10Click(TObject *Sender);
        void __fastcall O1Click(TObject *Sender);
    void __fastcall muManCheckClick(TObject *Sender);
private:
    HANDLE hDistUI;
    bool _stop;
    HDPCtrlUI* pHDP;
    Graphics::TBitmap * bmp1;
    Graphics::TBitmap * bmp2;
    Graphics::TBitmap * bmp3;
    Graphics::TBitmap * bmpStart;
    Graphics::TBitmap * bmpPause;

    bool m_bCtrlAmountLoaded;
    void LoadCtrlAmount();
    void DisplayDateSession();
    int StartTally(char* exchno);

    String PocketLabel[MAX_POCKETS];
    String PocketCount[MAX_POCKETS];
    String PocketTpCount[MAX_POCKETS];
    int IsNetStored(String netno);
    int CheckTotalAmount();

    char _Currency[4];
    int  _reDistState;

    cfgMap   _cfgMap;


public:		// User declarations
    int  _nDstState;
    int  _nDstMode;
    __fastcall TFmDistMan(TComponent* Owner);
    void __fastcall OnDist(TMessage &Message);
    int  GetPocketNum();

    int  getCurrency(int index);
    int  OnHDPCall(int num);
    bool MicrOk;
    void UpdateStateBar();
    int  cmpMoney(String sum1, String sum2, TLabel *signimg,TLabel * diff);
    void CheckMoneyEqual();
    void ClearFirst4RD();
    int  Stop(bool netcancel=false);
    void ClearShow();
    int CheckTotalCount();
    void InsertLog(char *txtLog);
    MyDb gdb;
    csLogin login;
    char date[12];
    char session[4];
    
    int Pause();
    DISTDATA* pData;
   
    long maxCount;
    int scancount;
    DISTINFO  Distval;
    int LoadCFG();
    int SaveCFG();
    int Init3Dst();
    int StartThird();
    int Treat3Data();
    char curPkgNo[20];
    bool CheckMoney(const char * exno,const char* vchtype, char* amount,char*accno,char*vchno);
    int InitDistNode(const char * pkgno);


    int curid;

    int scanid;

    int GetDS(const char* name, char * value);
    bool haveRCV;
    int CTE(const char* pkgno,const char*session);
    int InitPocketCount(const char* pkgno, const char* session);
    int SubOne(const char* vchtype, int isenvelop,double amount);
    VTMAP myVT;
    int initVTList();
    int getVT(vt_map_t & k);


    int skipHead;
    String originPath;

    cfgMap  _pocMap;
	cfgMap  _desMap;
	cfgMap  _cdcMap;

protected:
    int Start();
    void errMsg(int err);
    int Init2Dst();
    int Init1Dst();
    int StartFirst();
    int StartSecond();
    void ShowSetting();
    void GetChkData();
    void Treat1Data();
    void Treat2Data();
    void addTotalSum(TLabel* totalSum);
    bool IsJFVoucher();
    void AddSortCount();
    TListItem * getRightItem(TListView * list, int data);
    void PrintList();
    void ToFastMode();
    BEGIN_MESSAGE_MAP
        MESSAGE_HANDLER(ZWM_DIST_CHECK, TMessage, OnDist)
    END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TFmDistMan *FmDistMan;
//---------------------------------------------------------------------------
#endif
