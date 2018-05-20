//---------------------------------------------------------------------------

#ifndef SINGLEH
#define SINGLEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ztime.hpp>
#include <myplus.h>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TFmVoucher : public TForm
{
__published:	// IDE-managed Components
    TStatusBar *StatusBar;
    TToolBar *ToolBar1;
    TPanel *Panel1;
    TPanel *Panel2;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TButton *btnRefresh;
    TButton *btnExit;
    TEdit *txtDate;
    TEdit *txtSession;
    TComboBox *txtNetno;
    TComboBox *txtIdentify;
    TComboBox *txtMerge;
    TPanel *Panel3;
    TPanel *Panel4;
    TListView *txtDetail;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TButton *btnMerge;
    TMenuItem *E1;
    TMenuItem *M1;
    TMenuItem *R1;
    TMenuItem *H1;
    TMenuItem *A1;
    TPopupMenu *ErrResPopMenu;
    TMenuItem *MeForceLook;
    TFindDialog *FindDlg;
    TMenuItem *F1;
        TUpDown *UpDown1;
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall txtDetailDblClick(TObject *Sender);
    void __fastcall txtIdentifyCloseUp(TObject *Sender);
    void __fastcall txtMergeCloseUp(TObject *Sender);
    void __fastcall btnMergeClick(TObject *Sender);
    void __fastcall E1Click(TObject *Sender);
    void __fastcall M1Click(TObject *Sender);
    void __fastcall R1Click(TObject *Sender);
    void __fastcall MeForceLookClick(TObject *Sender);
    void __fastcall txtDetailColumnClick(TObject *Sender,
          TListColumn *Column);
    void __fastcall FindDlgFind(TObject *Sender);
    void __fastcall F1Click(TObject *Sender);
    void __fastcall txtNetnoDropDown(TObject *Sender);
    void __fastcall txtNetnoChange(TObject *Sender);
    void __fastcall txtDetailSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
private:	// User declarations
    bool  _needMerge;
    char  date[12];
    char  session[4];
    ZDate  _dt;
  

public:		// User declarations
    __fastcall TFmVoucher(TComponent* Owner);
    int flushData();
    int mergeOne(int vid,const char* accno, const char* vchno,const char*vchtype,const char*amount, const char* netno,const char*fname,const char*bname,const char*ip,int rotate,const char*dt,const char*sess);
    int mergeAll();
    int setitem(int m, int i, int j);
    int setimg(int a, int b);
    AnsiString sg(AnsiString str, int n);
  
    int FindResult(String StrToFind, TListItem* pItem, bool bExact);
    int flushExchno();
    int getNext();

    int _curPos;

protected:
};
//---------------------------------------------------------------------------
extern PACKAGE TFmVoucher *FmVoucher;
//---------------------------------------------------------------------------
#endif
