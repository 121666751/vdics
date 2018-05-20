//---------------------------------------------------------------------------

#ifndef pocketH
#define pocketH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFmPocket : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
        TListView *lstPocket;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
        TEdit *txtPockdesc;
    TLabel *Label4;
    TButton *btnAdd;
    TButton *btnExit;
    TPopupMenu *ErrResPopMenu;
    TMenuItem *MeForceLook;
    TLabel *Label6;
        TEdit *txtName;
        TComboBox *txtVchtype;
        TEdit *txtPockno;
        TUpDown *ud_pockno;
        TEdit *txtDistno;
        TUpDown *UpDown1;
    void __fastcall numExitClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnAddClick(TObject *Sender);
    void __fastcall MeForceLookClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFmPocket(TComponent* Owner);
    int showVchtype();
    int BindPocket();
    int IsPockExist(char *distno, char *vchtype);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmPocket *FmPocket;
//---------------------------------------------------------------------------
#endif
