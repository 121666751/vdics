//---------------------------------------------------------------------------

#ifndef vipsH
#define vipsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFmVips : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
        TEdit *txtExchno;
    TButton *btnAdd;
    TButton *btnExit;
    TPopupMenu *ErrResPopMenu;
    TMenuItem *MeForceLook;
    TLabel *Label6;
        TEdit *txtName;
        TListView *lstVips;
        TEdit *txtAccno;
        TEdit *txtAccname;
    void __fastcall numExitClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnAddClick(TObject *Sender);
    void __fastcall MeForceLookClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFmVips(TComponent* Owner);
    int showVchtype();
    int BindVips();
    int IsVipsExist(char *accno);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmVips *FmVips;
//---------------------------------------------------------------------------
#endif
