//---------------------------------------------------------------------------

#ifndef PBCH
#define PBCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <zcommon.h>
#include <myplus.h>
#include <map.h>

using namespace std;
struct attachments_node
{
    string fname;
    string leader;
    string vchtype;
    string netno;
    int    attsid;
};

typedef vector<attachments_node> ATTACHLIST;

typedef map<String ,String> vtMap;
//---------------------------------------------------------------------------
class TFormPBC : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TToolBar *ToolBar1;
    TToolButton *tolPBC;
    TToolButton *ToolButton2;
    TToolButton *ToolButton3;
    TImageList *ImageList1;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *numExit;
    TMenuItem *N8;
    TMenuItem *numAbout;
    TPanel *Panel3;
    TStatusBar *stBar;
    TPanel *Panel4;
    TPanel *Panel5;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TSpeedButton *btnMsgPath;
    TLabel *Label1;
    TLabel *Label6;
    TLabel *LabelCount;
    TLabel *Label7;
    TProgressBar *barTotal;
    TDateTimePicker *txtDate;
    TGroupBox *GroupBox1;
    TSpeedButton *btnReadData;
    TSpeedButton *btnExit;
    TEdit *txtSess;
    TUpDown *txtDown;
    TEdit *txtFname;
    TPanel *Panel6;
    TListView *lstImage;
    TSpeedButton *btnDir;
    TOpenDialog *LoadDialog;
    TToolButton *tolFind;
    TToolButton *ToolButton5;
    TFindDialog *FindDlg;
    TToolButton *tolRefresh;
    TToolButton *ToolButton6;
    TToolButton *ToolButton1;
    TToolButton *ToolButton4;
    TSaveDialog *fileSave;
    TCheckBox *chkFileType;
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall numExitClick(TObject *Sender);
    void __fastcall btnDirClick(TObject *Sender);
    void __fastcall btnReadDataClick(TObject *Sender);
    void __fastcall FindDlgFind(TObject *Sender);
    void __fastcall tolFindClick(TObject *Sender);
    void __fastcall tolPBCClick(TObject *Sender);
    void __fastcall lstImageDblClick(TObject *Sender);
    void __fastcall tolRefreshClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ToolButton1Click(TObject *Sender);
    void __fastcall txtDateChange(TObject *Sender);
    void __fastcall txtDownClick(TObject *Sender, TUDBtnType Button);
    void __fastcall ToolButton7Click(TObject *Sender);
    void __fastcall ToolButton3Click(TObject *Sender);
private:	// User declarations
     char  qsfile[128];
    vtMap      _vtMap;
    String     _area;
public:		// User declarations
    __fastcall TFormPBC(TComponent* Owner);
    int ReadNJPBC(const char* fname);
    int checkHead(const char* fname);
    int InertOne(pbc_node_t p);
    bool FindResult(String StrToFind, TListItem* pItem, bool bExact);

    char g_ip[24];
    DIST_CFG_T _cfg;
    int queryAll();

    int  _total;
    char _date[12];
    char _session[4];
    char _bankno[8];
    int createPreFile(const char* fname);

    String formatAmout(double amt);
    String formatOneData(String netno,String amount,String accno,String vchno,String vchtype,String cdcode);
    int makePreProcess();
    int savePBC(PBCLIST  pbclist,ATTACHLIST fnamelist,ATTACHLIST multilist);
    int GetLocalHost();

    int getCFG();
    String getBankType(String vchtype);
    int clearData();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPBC *FormPBC;
//---------------------------------------------------------------------------
#endif
