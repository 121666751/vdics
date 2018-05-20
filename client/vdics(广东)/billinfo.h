//---------------------------------------------------------------------------

#ifndef billinfoH
#define billinfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormBill : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *txtSession;
    TUpDown *UpDown;
    TLabel *Label3;
    TEdit *txtAccno;
    TDateTimePicker *txtDate;
    TLabel *Label4;
    TEdit *txtVchno;
    TLabel *Label5;
    TEdit *txtVchtype;
    TLabel *Label6;
    TEdit *txtExchno;
    TLabel *Label7;
    TEdit *txtAmount;
    TLabel *Label8;
    TComboBox *cbMsgType;
    TButton *btnExit;
    TButton *btnOk;
    TCheckBox *chkMode;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall txtAmountChange(TObject *Sender);
    void __fastcall txtAmountEnter(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall txtAmountKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    void MoneyChange(TEdit * txtMoney);
    void MoneyKeyPress(TEdit* txtMoney, char & Key);
    bool bFormatMoney;
    int nPopos;
    bool bPoint;
public:		// User declarations
    __fastcall TFormBill(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBill *FormBill;
//---------------------------------------------------------------------------
#endif
