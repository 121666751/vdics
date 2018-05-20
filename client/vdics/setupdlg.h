//----------------------------------------------------------------------------
#ifndef SetupDlgH
#define SetupDlgH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <NMHttp.hpp>
#include <Psock.hpp>
#include <ComCtrls.hpp>
//----------------------------------------------------------------------------
class TFmSetup : public TForm
{
__published:
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TBevel *Bevel1;
        TLabel *Label4;
        TLabel *Label1;
        TLabel *Label2;
    TSpeedButton *btnModify1;
        TLabel *Label5;
        TBevel *Bevel3;
        TLabel *Label6;
        TEdit *txtPrefix;
        TEdit *txtCalX;
        TEdit *txtCalY;
        TEdit *txtSerial;
    TEdit *txtScannerName;
        TEdit *txtOffSet;
        TTabSheet *TabSheet2;
        TBevel *Bevel2;
    TButton *btnConFirm;
    TButton *btnCancle;
    TEdit *txtPath;
    TLabel *Label13;
    TSpeedButton *SpeedButton1;
    TLabel *Label3;
    TCheckBox *pkgflag;
    TButton *btnModify;
    TButton *btnAdd;
    TButton *btnClear;
    TListView *VchDetailList;
        TCheckBox *ishardrotate;
	void __fastcall OKBtnClick(TObject *Sender);
	void __fastcall btnBrowseClick(TObject *Sender);
	void __fastcall btnModify1Click(TObject *Sender);
	void __fastcall txtSerialKeyPress(TObject *Sender, char &Key);
        void __fastcall btnchangemode(TObject *Sender);
    void __fastcall btnConFirmClick(TObject *Sender);
    void __fastcall VchDetailListDblClick(TObject *Sender);
    void __fastcall btnModifyClick(TObject *Sender);
    void __fastcall btnClearClick(TObject *Sender);
    void __fastcall btnAddClick(TObject *Sender);
private:
public:
	virtual __fastcall TFmSetup(TComponent* AOwner);

};
//----------------------------------------------------------------------------
extern PACKAGE TFmSetup *FmSetup;
//----------------------------------------------------------------------------
#endif    
