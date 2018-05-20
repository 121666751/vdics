//---------------------------------------------------------------------------

#ifndef undoH
#define undoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormUndo : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *E1;
    TMenuItem *N2;
    TMenuItem *R1;
    TMenuItem *H1;
    TMenuItem *A1;
    TPanel *Panel1;
    TStatusBar *StatusBar;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
    TPanel *Panel8;
    TPanel *Panel9;
    TPanel *Panel10;
    TPanel *Panel11;
    TPanel *Panel12;
    TPanel *Panel13;
    TListView *lstVouchers;
    TListView *lstWs;
    TListView *lstEnv;
    TButton *btnRefresh;
    TButton *Button2;
    void __fastcall E1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall R1Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    int flushUndo();
public:		// User declarations
    __fastcall TFormUndo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormUndo *FormUndo;
//---------------------------------------------------------------------------
#endif
