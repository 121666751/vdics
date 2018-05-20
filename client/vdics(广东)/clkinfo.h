//---------------------------------------------------------------------------

#ifndef clkinfoH
#define clkinfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFmCkInfo : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *txtClkno;
    TEdit *txtName;
    TButton *btnAud;
    TGroupBox *GroupBox2;
    TLabel *Label4;
    TLabel *Label5;
    TMaskEdit *txtPswdOne;
    TMaskEdit *txtPswdTwo;
    TBitBtn *btnSave;
    TBitBtn *btnExit;
    TGroupBox *GroupBox4;
    TCheckListBox *txtPrivList;
    TLabel *Label8;
    TEdit *txtID;
    TRadioButton *txtRadioA;
    TRadioButton *txtRadioC;
    TRadioButton *txtRadioB;
    TRadioButton *txtRadioD;
    TLabel *Label6;
    TComboBox *txtSex;
    TCheckBox *ckModify;
    TBevel *Bevel1;
    TLabel *Label7;
    TEdit *txtArea;
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall txtRadioAClick(TObject *Sender);
    void __fastcall txtRadioCClick(TObject *Sender);
    void __fastcall txtRadioBClick(TObject *Sender);
    void __fastcall txtRadioDClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall txtClknoExit(TObject *Sender);
private:	// User declarations
    int _type ;   // 0 - 查看  1 - 增加 2 - 修改
    int _priv;
    char _clkno[12];
public:		// User declarations
    __fastcall TFmCkInfo(TComponent* Owner);
    int clear();
    int setPriv(int priv);
    int setMethod(int type,char *clkno);
    int getPriv();
};
//---------------------------------------------------------------------------
extern PACKAGE TFmCkInfo *FmCkInfo;
//---------------------------------------------------------------------------
#endif
