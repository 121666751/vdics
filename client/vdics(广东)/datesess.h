//---------------------------------------------------------------------------

#ifndef datesessH
#define datesessH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormDate : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TGroupBox *GroupBox1;
    TLabel *Label3;
    TLabel *Label2;
    TDateTimePicker *txtDate;
    TEdit *txtSession;
    TBitBtn *BtnOk;
    TBitBtn *BtnCancel;
    TUpDown *UpDown1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall BtnOkClick(TObject *Sender);
private:	// User declarations
    char date[12];
    char session[4];
public:		// User declarations
    __fastcall TFormDate(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDate *FormDate;
//---------------------------------------------------------------------------
#endif
