//---------------------------------------------------------------------------

#ifndef sendSpecFormH
#define sendSpecFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ztime.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TFormSendSpec : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel5;
    TListView *lstSend;
    TLabel *Label3;
    TComboBox *txtExchno;
    TButton *btnRefresh;
    TButton *btnExit;
    TStatusBar *StatusBar;
    TPopupMenu *ErrResPopMenu;
    TMenuItem *MeForceLook;
    TMenuItem *F1;
    TFindDialog *FindDlg;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *E1;
    TMenuItem *N2;
    TMenuItem *N4;
    TMenuItem *N7;
    TMenuItem *A1;
    TComboBox *txtSend;
    TLabel *Label5;
    TComboBox *cbTimes;
    TLabel *Label4;
        TUpDown *UpDown1;
        TEdit *txtDate;
        TEdit *txtSession;
        TLabel *Label2;
        TLabel *Label1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall lstSendDblClick(TObject *Sender);
    void __fastcall txtSendCloseUp(TObject *Sender);
    void __fastcall txtExchnoDropDown(TObject *Sender);
    void __fastcall txtExchnoChange(TObject *Sender);
    void __fastcall MeForceLookClick(TObject *Sender);
    void __fastcall F1Click(TObject *Sender);
    void __fastcall E1Click(TObject *Sender);
    void __fastcall N4Click(TObject *Sender);
    void __fastcall FindDlgFind(TObject *Sender);
    void __fastcall lstSendCustomDrawItem(TCustomListView *Sender,
          TListItem *Item, TCustomDrawState State, bool &DefaultDraw);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
private:	// User declarations
     int        _count;
     ZDate  _dt;
     long   _modifyno;
public:		// User declarations
    __fastcall TFormSendSpec(TComponent* Owner);
    int flushExchno();
    int flushData();
    bool FindResult(String StrToFind, TListItem* pItem, bool bExact);
protected:

};
//---------------------------------------------------------------------------
extern PACKAGE TFormSendSpec *FormSendSpec;
//---------------------------------------------------------------------------
#endif
