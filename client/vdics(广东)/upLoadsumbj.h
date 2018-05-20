//---------------------------------------------------------------------------

#ifndef upLoadsumbjH
#define upLoadsumbjH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <myplus.h>
#include "main.h"
//---------------------------------------------------------------------------
class TFormSignBJ : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TLabel *Label1;
    TComboBox *txtExchno;
    TLabel *Label2;
    TEdit *txtDCount;
    TButton *btnSubmit;
    TButton *btnExit;
        TCheckBox *chkSign;
    TButton *btnAll;
    void __fastcall txtExchnoCloseUp(TObject *Sender);
    void __fastcall btnSubmitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnAllClick(TObject *Sender);
    void __fastcall chkSignClick(TObject *Sender);
private:	// User declarations
    MyDb *db;
    char date[12];
    char session[4];
public:		// User declarations
    __fastcall TFormSignBJ(TComponent* Owner);
    int getDC();
    int submitDC(char* exchno, int count);
    int flushExchno();
    int getCount(char*exchno,int &ct);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSignBJ *FormSignBJ;
//---------------------------------------------------------------------------
#endif
