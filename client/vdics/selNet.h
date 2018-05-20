//---------------------------------------------------------------------------

#ifndef selNetH
#define selNetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <myplus.h>
#include <zcommon.h>
//---------------------------------------------------------------------------
class TFmSelNet : public TForm
{
__published:	// IDE-managed Components
    TBevel *Bevel1;
    TBitBtn *btnOk;
    TBitBtn *BitBtn2;
    TLabel *Label2;
    TComboBox *txtPkgNo;
    TComboBox *txtVchType;
    TLabel *Label1;
    TCheckBox *chbSingle;
    TLabel *Label3;
    TComboBox *txtArea;
    TCheckBox *cbVchtype;
    TLabel *LabelType;
    TComboBox *cbType;
    TCheckBox *cbNewMode;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall txtPkgNoDropDown(TObject *Sender);
    void __fastcall txtPkgNoChange(TObject *Sender);
    void __fastcall txtPkgNoExit(TObject *Sender);
    void __fastcall cbVchtypeClick(TObject *Sender);
private:	// User declarations
   
    int initNetno(int type);
   

    int _type;    
public:		// User declarations
    PocketMap  _vtMap;
    __fastcall TFmSelNet(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmSelNet *FmSelNet;
//---------------------------------------------------------------------------
#endif
