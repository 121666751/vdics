//---------------------------------------------------------------------------

#ifndef stateH
#define stateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormST : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TPanel *Panel1;
    TPanel *Panel3;
    TLabel *Label1;
    TComboBox *cmbPhase;
    TListView *txtDetail;
    TButton *btnRefresh;
    TButton *Button2;
    TStatusBar *StatusBar;
    TLabel *Label2;
    TComboBox *cmbState;
    TMainMenu *MainMenuST;
    TMenuItem *N1;
    TMenuItem *E1;
    TMenuItem *N2;
    TMenuItem *R1;
    TMenuItem *H1;
    TMenuItem *A1;
    TEdit *txtDate;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *txtSession;
    TUpDown *UpDown1;
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall cmbPhaseChange(TObject *Sender);
    void __fastcall E1Click(TObject *Sender);
    void __fastcall R1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormST(TComponent* Owner);
    int flushState();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormST *FormST;
//---------------------------------------------------------------------------
#endif
