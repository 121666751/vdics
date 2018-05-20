//---------------------------------------------------------------------------

#ifndef warrantH
#define warrantH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFormWarrant : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TButton *btnOK;
    TButton *btnCancel;
    TLabel *Label1;
    TEdit *txtClkno;
    TLabel *Label2;
    TEdit *txtPassword;
    TPanel *Panel1;
    TImage *Image1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
    __fastcall TFormWarrant(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormWarrant *FormWarrant;
//---------------------------------------------------------------------------
#endif
