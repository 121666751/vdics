//---------------------------------------------------------------------------

#ifndef loginH
#define loginH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFormLogin : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
    void __fastcall FormShow(TObject *Sender);
//    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
    __fastcall TFormLogin(TComponent* Owner);
    
    int Login();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLogin *FormLogin;
extern String  g_sys_opt;  //��Ա��
extern int     g_sys_priv; //��Ա����
extern String  g_sys_area;  //��������

extern String  g_sys_name;  //��Ա��
extern String  g_sys_org;   //������
//---------------------------------------------------------------------------
#endif
