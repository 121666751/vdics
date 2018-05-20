//---------------------------------------------------------------------------

#ifndef imageEnvH
#define imageEnvH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormEnvImage : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TImage *image;
    TGroupBox *GroupBox1;
    TButton *btnSave;
    TButton *btnExit;
    TEdit *txtAmount;
    TLabel *Label3;
    TLabel *Label1;
    TComboBox *txtType;
    TLabel *Label2;
    TEdit *txtVchType;
    TCheckBox *chkMode;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall imageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall txtAmountKeyPress(TObject *Sender, char &Key);
    void __fastcall txtAmountEnter(TObject *Sender);
    void __fastcall txtAmountChange(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TFormEnvImage(TComponent* Owner);
    int type;
    void MoneyChange(TEdit * txtMoney);
    void MoneyKeyPress(TEdit* txtMoney, char & Key);
    int    curID;
    bool bFormatMoney;
    int nPopos;
    bool bPoint;
    
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEnvImage *FormEnvImage;
//---------------------------------------------------------------------------
#endif
