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
class TFormMainImage : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TImage *image;
    TGroupBox *GroupBox1;
    TButton *btnSave;
    TButton *btnExit;
    TLabel *Label2;
    TEdit *txtVchtype;
    TCheckBox *chkMode;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall imageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TFormMainImage(TComponent* Owner);
    int type;
    void MoneyChange(TEdit * txtMoney);
    void MoneyKeyPress(TEdit* txtMoney, char & Key);
    int    curID;
    bool bFormatMoney;
    int nPopos;
    bool bPoint;
    
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMainImage *FormMainImage;
//---------------------------------------------------------------------------
#endif
