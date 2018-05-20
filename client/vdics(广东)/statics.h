//---------------------------------------------------------------------------

#ifndef staticsH
#define staticsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Chart.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <DbChart.hpp>
//---------------------------------------------------------------------------
class TFormStatic : public TForm
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
    TPanel *Panel4;
    TPanel *Panel9;
    TPanel *Panel5;
    TPanel *Panel11;
    TDBChart *DBChart1;
    TDBChart *DBChart2;
private:	// User declarations
public:		// User declarations
    __fastcall TFormStatic(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormStatic *FormStatic;
//---------------------------------------------------------------------------
#endif
