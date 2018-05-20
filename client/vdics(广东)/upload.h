//---------------------------------------------------------------------------

#ifndef UPLOADH
#define UPLOADH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <myplus.h>
#include <Dialogs.hpp>
#include <ztime.hpp>

//---------------------------------------------------------------------------
class TFormTrans : public TForm
{
__published:	// IDE-managed Components
    TStatusBar *StatusBar;
    TToolBar *ToolBar1;
    TPanel *Panel1;
    TPanel *Panel2;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label5;
    TButton *btnRefresh;
    TButton *btnExit;
    TEdit *txtDate;
    TEdit *txtSession;
    TComboBox *txtExchno;
    TComboBox *txtSend;
    TPanel *Panel3;
    TPanel *Panel4;
    TListView *txtDetail;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TFindDialog *FindDlg;
    TMenuItem *N7;
    TMenuItem *A1;
    TMenuItem *E1;
    TMenuItem *E2;
    TMenuItem *N3;
    TPopupMenu *ErrResPopMenu;
    TMenuItem *MeForceLook;
    TMenuItem *R1;
    TMenuItem *N4;
    TComboBox *cbTimes;
    TLabel *Label4;
        TUpDown *UpDown1;
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall txtSendCloseUp(TObject *Sender);
    void __fastcall txtDetailDblClick(TObject *Sender);
    void __fastcall FindDlgFind(TObject *Sender);
    void __fastcall R1Click(TObject *Sender);
    void __fastcall txtExchnoDropDown(TObject *Sender);
    void __fastcall txtExchnoChange(TObject *Sender);
    void __fastcall E2Click(TObject *Sender);
    void __fastcall N3Click(TObject *Sender);
    void __fastcall MeForceLookClick(TObject *Sender);
    void __fastcall N4Click(TObject *Sender);
    void __fastcall E1Click(TObject *Sender);
    void __fastcall txtDetailCustomDrawItem(TCustomListView *Sender,
          TListItem *Item, TCustomDrawState State, bool &DefaultDraw);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
private:	// User declarations
    
    bool   _needMerge;
    ZDate  _dt;
    long   _modifyno;

public:		// User declarations
    __fastcall TFormTrans(TComponent* Owner);
    distCFGT  _cfg;

    int flushData();
    int getCFG();
    char date[12];
    char session[4];

    bool FindResult(String StrToFind, TListItem* pItem, bool bExact);
    int flushExchno();
protected:

};
//---------------------------------------------------------------------------
extern PACKAGE TFormTrans *FormTrans;
//---------------------------------------------------------------------------
#endif
