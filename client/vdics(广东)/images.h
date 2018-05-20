//---------------------------------------------------------------------------

#ifndef imagesH
#define imagesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormImages : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TImage *image;
    TListView *lstFile;
    TButton *btnExit;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall imageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall lstFileSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
private:	// User declarations
    int _type;
    int _sid;
public:		// User declarations
    __fastcall TFormImages(TComponent* Owner);
    int setMethod(int type, int id);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormImages *FormImages;
//---------------------------------------------------------------------------
#endif
