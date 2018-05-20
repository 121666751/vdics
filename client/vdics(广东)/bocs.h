//---------------------------------------------------------------------------

#ifndef bocsH
#define bocsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <myplus.h>
//---------------------------------------------------------------------------
class TFormNets : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TListView *ListView;
    TLabel *Label1;
    TEdit *txtNetno;
    TButton *BtnAdd;
    TButton *Button3;
    TCheckBox *chbCancel;
    TEdit *txtNet2;
    TLabel *Label2;
    TButton *btnDelete;
    TPopupMenu *ErrResPopMenu;
    TMenuItem *MeForceLook;
    TMenuItem *N3;
    TEdit *txtArea;
    TLabel *Label3;
    TCheckBox *cbMerge;
    TCheckBox *cbCheck;
        TButton *btnModif;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall BtnAddClick(TObject *Sender);
    void __fastcall MeForceLookClick(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall btnDeleteClick(TObject *Sender);
    void __fastcall ListViewClick(TObject *Sender);
    void __fastcall N3Click(TObject *Sender);
    void __fastcall chbCancelClick(TObject *Sender);
        void __fastcall btnModifClick(TObject *Sender);
private:	// User declarations
     MyDb  _db;
public:		// User declarations
    __fastcall TFormNets(TComponent* Owner);
    int flushData();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormNets *FormNets;
//---------------------------------------------------------------------------
#endif
