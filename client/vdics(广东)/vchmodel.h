//---------------------------------------------------------------------------

#ifndef vchmodelH
#define vchmodelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ScannerIni.h"
//---------------------------------------------------------------------------
class TVchMode : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBevel *Bevel2;
    TGroupBox *GroupBox2;
    TRadioButton *RadioBmp;
    TRadioButton *RadioJpg;
    TGroupBox *GroupBox3;
    TCheckBox *chkSideA;
    TCheckBox *chkSideB;
    TCheckBox *chkProcA;
    TCheckBox *chkProcB;
    TGroupBox *GroupBox4;
    TRadioButton *Radio0;
    TRadioButton *Radio90;
    TRadioButton *Radio180;
    TRadioButton *Radio270;
    TGroupBox *GroupBox5;
    TRadioButton *RadioRgb;
    TRadioButton *RadioGray;
    TRadioButton *RadioBw;
    TGroupBox *GroupBox1;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TEdit *txtDpi;
    TEdit *txtVchX;
    TEdit *txtVchY;
    TButton *Conform;
    TButton *Cancel;
    TLabel *Label1;
    TLabel *vchname;
    TLabel *Label2;
    TComboBox *cbScanType;
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall ConformClick(TObject *Sender);
private:	// User declarations
    vch_type_t  curVch;
    int         mode;
public:		// User declarations
    __fastcall TVchMode(TComponent* Owner);
    int SetData(vch_type_t oneVch,int flag);
};
//---------------------------------------------------------------------------
extern PACKAGE TVchMode *VchMode;
//---------------------------------------------------------------------------
#endif
