//---------------------------------------------------------------------------

#ifndef vchnameH
#define vchnameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TNewvch : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TButton *Button1;
    TLabel *Label1;
    TEdit *txtVchName;
private:	// User declarations
public:		// User declarations
    __fastcall TNewvch(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TNewvch *Newvch;
//---------------------------------------------------------------------------
#endif
