//---------------------------------------------------------------------------

#ifndef vchtypeH
#define vchtypeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFmVchtype : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *numExit;
    TMenuItem *N8;
    TMenuItem *numAbout;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TListView *lstVchtype;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TEdit *txtVchtype;
    TLabel *Label2;
    TEdit *txtType;
    TLabel *Label3;
    TEdit *txtModel;
    TLabel *Label4;
    TComboBox *cbVchtype;
    TButton *btnAdd;
    TButton *btnExit;
    TEdit *txtName;
    TLabel *Label5;
    TPopupMenu *ErrResPopMenu;
    TMenuItem *MeForceLook;
    TLabel *Label6;
    TEdit *txtArea;
        TButton *btnModif;
    void __fastcall numExitClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnAddClick(TObject *Sender);
    void __fastcall MeForceLookClick(TObject *Sender);
        void __fastcall btnModifClick(TObject *Sender);
        void __fastcall lstVchtypeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFmVchtype(TComponent* Owner);
    int showVchtype();
};
//---------------------------------------------------------------------------
extern PACKAGE TFmVchtype *FmVchtype;
//---------------------------------------------------------------------------
#endif
