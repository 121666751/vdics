//---------------------------------------------------------------------------

#ifndef waitdlgH
#define waitdlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFmWait : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TAnimate *Animate1;
private:	// User declarations
public:		// User declarations
    __fastcall TFmWait(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmWait *FmWait;
//---------------------------------------------------------------------------
#endif
