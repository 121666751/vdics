//---------------------------------------------------------------------------

#ifndef MOREH
#define MOREH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <myplus.h>
#include <ztime.hpp>
#include "pubfun.h"
#include <Dialogs.hpp>

#define ZM_MODEL_RES       WM_USER+300
//---------------------------------------------------------------------------
class TFmEnv : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TStatusBar *StatusBar;
    TPanel *Panel4;
    TPanel *Panel3;
    TEdit *txtDate;
    TLabel *Label2;
    TLabel *Label4;
    TEdit *txtSession;
    TComboBox *cmbNetno;
    TLabel *Label5;
    TLabel *Label6;
    TComboBox *cmbIdentify;
    TComboBox *cmbMerge;
    TLabel *Label7;
    TLabel *Label8;
    TComboBox *cmbNetState;
    TButton *btnMerge;
    TButton *btnExit;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *E1;
    TMenuItem *N2;
    TMenuItem *M1;
    TMenuItem *R1;
    TMenuItem *H1;
    TMenuItem *A1;
    TButton *btnSubmit;
    TPageControl *pageCtrl;
    TTabSheet *TabSheet1;
    TListView *lstMerge;
    TTabSheet *TabSheet2;
    TListView *lstEnvs;
    TTabSheet *TabSheet3;
    TListView *lstSpecial;
    TListView *lstMain;
    TTabSheet *TabSheet4;
    TListView *lstSingle;
    TTabSheet *TabSheet5;
    TListView *lstMulti;
    TPopupMenu *ErrResPopMenu;
    TMenuItem *MeForceLook;
    TButton *btnRefresh;
    TTabSheet *TabSheet6;
    TListView *txtDetail;
    TMenuItem *B1;
    TMenuItem *btnReSubmit;
    TFindDialog *FindDlg;
    TMenuItem *F1;
        TUpDown *UpDown1;
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall R1Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall cmbNetStateCloseUp(TObject *Sender);
    void __fastcall cmbNetnoCloseUp(TObject *Sender);
    void __fastcall cmbNetnoDropDown(TObject *Sender);
    void __fastcall btnSubmitClick(TObject *Sender);
    void __fastcall lstMergeDblClick(TObject *Sender);
    void __fastcall lstEnvsDblClick(TObject *Sender);
    void __fastcall lstMainDblClick(TObject *Sender);
    void __fastcall lstMultiDblClick(TObject *Sender);
    void __fastcall lstSingleDblClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall MeForceLookClick(TObject *Sender);
    void __fastcall cmbIdentifyCloseUp(TObject *Sender);
    void __fastcall cmbMergeCloseUp(TObject *Sender);
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall pageCtrlChange(TObject *Sender);
    void __fastcall lstEnvsColumnClick(TObject *Sender,
          TListColumn *Column);
    void __fastcall B1Click(TObject *Sender);
    void __fastcall btnReSubmitClick(TObject *Sender);
    void __fastcall btnMergeClick(TObject *Sender);
    void __fastcall E1Click(TObject *Sender);
    void __fastcall txtDetailDblClick(TObject *Sender);
    void __fastcall FindDlgFind(TObject *Sender);
    void __fastcall F1Click(TObject *Sender);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown1Changing(TObject *Sender,
          bool &AllowChange);
private:	// User declarations
   
    char date[12];
    char session[4];
    String curNetno;
    String lastNetno;

    String curSess;
    String lastSess;
    
    bool locked;
    ZDate  _dt;
    long   _modifyno;
    XmlMap _vtMap;

    int flushdate();
    int flush_sendstate();

public:		// User declarations
    __fastcall TFmEnv(TComponent* Owner);
    int flushData1();
    int flushData2();
    int flushData3();
    int flushData4();
    int flushData5();
    int flushData6();
    int flushExchno(int type);
    String oldDir;
    int setitem(int m, int i, int j);
    int mergeEnvs(int type);
    int getNext();
    int getNextScan();
    bool FindResult(String StrToFind, TListItem* pItem, bool bExact);
protected:

};
//---------------------------------------------------------------------------
extern PACKAGE TFmEnv *FmEnv;
//---------------------------------------------------------------------------
#endif
