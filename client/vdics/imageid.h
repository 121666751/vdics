//---------------------------------------------------------------------------

#ifndef imageidH
#define imageidH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormImageID : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TEdit *txtImageID;
    TButton *btnOK;
    TButton *btnExit;
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormImageID(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormImageID *FormImageID;
//---------------------------------------------------------------------------
#endif
