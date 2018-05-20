//---------------------------------------------------------------------------

#ifndef imageH
#define imageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormImage : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TImage *image;
    TGroupBox *GroupBox1;
    TButton *btnSave;
    TButton *btnExit;
    TEdit *txtVchno;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *txtAccno;
    TEdit *txtAmount;
    TLabel *Label3;
    TEdit *txtVchtype;
    TLabel *Label4;
    TCheckBox *chkMode;
        TLabel *Label5;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall imageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall txtAmountChange(TObject *Sender);
    void __fastcall txtAmountEnter(TObject *Sender);
    void __fastcall txtAmountKeyPress(TObject *Sender, char &Key);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TFormImage(TComponent* Owner);
    void MoneyChange(TEdit * txtMoney);
    void MoneyKeyPress(TEdit* txtMoney, char & Key);
    int    curID;
    bool bFormatMoney;
    int nPopos;
    bool bPoint;
    String exchno;    
};
//---------------------------------------------------------------------------
extern PACKAGE TFormImage *FormImage;
//---------------------------------------------------------------------------
#endif
