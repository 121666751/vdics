//---------------------------------------------------------------------------

#ifndef PasswordH
#define PasswordH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFormPWD : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *txtPwdOld;
    TEdit *txtPwdOne;
    TEdit *txtPwdTwo;
    TImage *Image1;
    TButton *btnOK;
    TButton *btnCancel;
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
    __fastcall TFormPWD(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPWD *FormPWD;
//---------------------------------------------------------------------------
#endif
