//---------------------------------------------------------------------------

#ifndef DIST_DATA_H
#define DIST_DATA_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <myplus.h>

#include <Dialogs.hpp>
#include <ztime.hpp>

#define ZM_PROCESS_RES       WM_USER+300
//---------------------------------------------------------------------------
class TdistForm : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TToolBar *ToolBar1;
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TListView *txtDetail;
    TButton *btnRefresh;
    TButton *btnExit;
    TLabel *Label1;
    TEdit *txtDate;
    TLabel *Label2;
    TEdit *txtSession;
    TLabel *Label3;
    TComboBox *txtNetno;
    TFindDialog *FindDlg;
    TMenuItem *N3;
    TMenuItem *E1;
    TMenuItem *N4;
    TMenuItem *A1;
    TMenuItem *R1;
    TLabel *Label7;
    TComboBox *txtMerge;
    TMenuItem *N6;
    TStatusBar *stBar;
    TLabel *Label4;
    TComboBox *cbType;
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall txtNetnoCloseUp(TObject *Sender);
    void __fastcall txtIdentifyCloseUp(TObject *Sender);
    void __fastcall txtSendCloseUp(TObject *Sender);
    void __fastcall txtDetailDblClick(TObject *Sender);
    void __fastcall txtDetailColumnClick(TObject *Sender,
          TListColumn *Column);
    void __fastcall FindDlgFind(TObject *Sender);
    void __fastcall N3Click(TObject *Sender);
    void __fastcall R1Click(TObject *Sender);
    void __fastcall txtMergeCloseUp(TObject *Sender);
    void __fastcall txtNetnoDropDown(TObject *Sender);
private:	// User declarations
    
    char  date[12];
    char  session[4];
    
public:		// User declarations
    __fastcall TdistForm(TComponent* Owner);
    int flushData();

    int setitem(int m, int i,int j);
    AnsiString sg(AnsiString str, int n);

    bool FindResult(String StrToFind, TListItem* pItem, bool bExact);

   
protected:

};
//---------------------------------------------------------------------------
extern PACKAGE TdistForm *distForm;
//---------------------------------------------------------------------------
#endif
