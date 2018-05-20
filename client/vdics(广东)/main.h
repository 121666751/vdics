//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
#include <Chart.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>
#include <ImgList.hpp>
#include <jpeg.hpp>
#include <Menus.hpp>
#include <Series.hpp>
#include <StdCtrls.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ToolWin.hpp>


//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <jpeg.hpp>
#include <ImgList.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <DB.hpp>
#include <DBClient.hpp>
#include <DBLocal.hpp>
#include <DBLocalS.hpp>
#include <DBXpress.hpp>
#include <Provider.hpp>
#include <SqlExpr.hpp>
#include <FMTBcd.hpp>

#include <myplus.h>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>

//---------------------------------------------------------------------------
class TFmIC : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N4;
    TMenuItem *muPbc;
    TMenuItem *muScan;
    TMenuItem *muVouchers;
    TMenuItem *muMultis;
    TPanel *Panel1;
    TCoolBar *CoolBar1;
    TToolBar *MainToolBar;
    TToolButton *tolPbc;
    TToolButton *sltPbc;
    TToolButton *tolScan;
    TToolButton *sltScan;
    TToolButton *tolVoucher;
    TToolButton *sltVoucher;
    TToolButton *tolMultis;
    TToolButton *sltMultis;
    TToolButton *tolUpload;
    TToolButton *sltUpload;
    TToolButton *tolHelp;
    TImageList *ImageList1;
    TPanel *Panel2;
    TPanel *Panel3;
    TTreeView *lstTree;
    TImage *Image1;
    TMenuItem *N9;
    TMenuItem *muClerk;
    TMenuItem *muUpload;
    TChart *MainChart;
    TBarSeries *Series1;
    TMenuItem *muVchtype;
    TMenuItem *muDepart;
    TMenuItem *muConfig;
    TMenuItem *N5;
    TToolButton *tolSpec;
    TToolButton *sltSpec;
    TOpenPictureDialog *openFile;
    TButton *Button1;
    TMenuItem *N6;
    TMenuItem *N7;
    TMenuItem *N8;
    TMenuItem *N10;
    TMenuItem *N11;
    TMenuItem *N12;
    TMenuItem *OldBi;
    TMenuItem *N13;
    TMenuItem *N14;
    void __fastcall muScanClick(TObject *Sender);
    void __fastcall muPbcClick(TObject *Sender);
    void __fastcall muVouchersClick(TObject *Sender);
    void __fastcall muMultisClick(TObject *Sender);
    void __fastcall muClerkClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall N4Click(TObject *Sender);
    void __fastcall muDepartClick(TObject *Sender);
    void __fastcall lstTreeClick(TObject *Sender);
    void __fastcall muVchtypeClick(TObject *Sender);
    void __fastcall muConfigClick(TObject *Sender);
    void __fastcall tolPbcClick(TObject *Sender);
    void __fastcall tolScanClick(TObject *Sender);
    void __fastcall tolVoucherClick(TObject *Sender);
    void __fastcall tolMultisClick(TObject *Sender);
    void __fastcall muUploadClick(TObject *Sender);
    void __fastcall tolUploadClick(TObject *Sender);
    void __fastcall N5Click(TObject *Sender);
    void __fastcall tolSpecClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall N7Click(TObject *Sender);
    void __fastcall N8Click(TObject *Sender);
    void __fastcall N11Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall N12Click(TObject *Sender);
    void __fastcall N13Click(TObject *Sender);
    void __fastcall N14Click(TObject *Sender);
private:	// User declarations
        int GetPriv();
    
   
public:		// User declarations
     __fastcall TFmIC(TComponent* Owner);
    MyDb gdb;
    int  gmode;
    int  gprest;

    int  gclear;
    String gPath;
    char gbrno[14];
    char gdcode[8];
    //char gsignip[24];
    //int  gsignport;

    char gflowip[24];
    int  gflowport;

    char gmsgtype[8];
    char gmsgtypeex[8];

    int showMenus();
    char gdate[12];
    char gsession[4];
    int getDS(char* name, char* value);
    int showChart();  
    
    bool m_bReplace;
    bool m_bReStart;
    bool m_bCommuteDate;
    bool m_bNetDelete;
    bool m_bSupply;
    bool m_bSend;

	String m_sOrgCode;
	String m_sOptCode;

  //---------------20150414----------------
        String m_area;

    
    int CleanOldImage(String filename,long CreateTime);
    int FindFile(String Path);
	int Login();



    TStringList *p;

    String GetProwIP;
    String GetProwPort;

    int LoadHosts();

	int PrintToLog(const char* msg,...);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmIC *FmIC;
//---------------------------------------------------------------------------
#endif
