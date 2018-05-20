//---------------------------------------------------------------------------

#ifndef clerksH
#define clerksH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFmClerks : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TListView *txtClerkList;
    TGroupBox *GroupBox1;
    TButton *btnAdd;
    TButton *btnMod;
    TButton *btnDel;
    TButton *Button4;
    void __fastcall btnAddClick(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
    void __fastcall btnModClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnDelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFmClerks(TComponent* Owner);
    int showClerks();
};
//---------------------------------------------------------------------------
extern PACKAGE TFmClerks *FmClerks;
//---------------------------------------------------------------------------
#endif
