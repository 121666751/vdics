//---------------------------------------------------------------------------

#ifndef configsH
#define configsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormConfig : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *txtFlowip;
    TEdit *txtFlowport;
    TGroupBox *GroupBox2;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *txtEcmip;
    TEdit *txtEcmport;
    TPanel *Panel4;
    TPanel *Panel5;
    TButton *btnSave;
    TButton *btnExit;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *numExit;
    TMenuItem *N8;
    TMenuItem *numAbout;
    TGroupBox *GroupBox4;
    TCheckBox *cbPre;
    TCheckBox *cbSign;
    TCheckBox *cbAmount;
    TLabel *Label8;
    TEdit *txtBrno;
    TLabel *Label9;
    TEdit *txtMsgType;
    TLabel *Label10;
    TEdit *txtAccno;
    TLabel *Label11;
    TEdit *txtDcode;
    TLabel *Label12;
    TComboBox *cbMode;
    TLabel *Label7;
    TEdit *txtMsgTypeEx;
    TCheckBox *cbClear;
        TGroupBox *GroupBox3;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *txtSignip;
        TEdit *txtSignport;
    void __fastcall numExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormConfig(TComponent* Owner);
    int showConfig();
    int saveConfig();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormConfig *FormConfig;
//---------------------------------------------------------------------------
#endif
