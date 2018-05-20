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
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall cmbPhaseChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormST(TComponent* Owner);
    int flushState();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormST *FormST;
//---------------------------------------------------------------------------
#endif
