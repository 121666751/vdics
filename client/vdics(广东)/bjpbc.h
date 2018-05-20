//---------------------------------------------------------------------------

#ifndef BJPBC
#define BJPBC
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
#include "upLoadsum.h"

using namespace std;

typedef map<String ,String> vtMap;
//---------------------------------------------------------------------------
class TBjFormPBC : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TToolBar *ToolBar1;
    TToolButton *tolPBC;
    TToolButton *ToolButton2;
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
    TEdit *txtPath;
    TPanel *Panel6;
    TListView *lstImage;
    TSpeedButton *SpeedButton1;
    TOpenDialog *LoadDialog;
    TToolButton *tolFind;
    TToolButton *ToolButton5;
    TFindDialog *FindDlg;
    TToolButton *tolRefresh;
    TToolButton *ToolButton6;
    TToolButton *ToolButton4;
    TSaveDialog *fileSave;
    TToolButton *ToolButton1;
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall numExitClick(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall btnReadDataClick(TObject *Sender);
    void __fastcall FindDlgFind(TObject *Sender);
    void __fastcall tolFindClick(TObject *Sender);
    void __fastcall tolPBCClick(TObject *Sender);
    void __fastcall tolRefreshClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall txtDateChange(TObject *Sender);
    void __fastcall txtDownClick(TObject *Sender, TUDBtnType Button);
    void __fastcall ToolButton1Click(TObject *Sender);
private:	// User declarations
   
    vtMap      _vtMap;
public:		// User declarations
    __fastcall TBjFormPBC(TComponent* Owner);
    int ReadBJPBC();
    int checkHead(const char* fname);
    int InertOne(pbc_node_t p);
    bool FindResult(String StrToFind, TListItem* pItem, bool bExact);

    char g_ip[24];
 
    int queryAll();
    int  _total;
    char _date[12];
    char _session[4];
    char _bankno[8];
    int  GetLocalHost();
};
//---------------------------------------------------------------------------
extern PACKAGE TBjFormPBC *BjFormPBC;
//---------------------------------------------------------------------------
#endif
