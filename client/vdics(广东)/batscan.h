//---------------------------------------------------------------------------

#ifndef BatScanH
#define BatScanH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>

#include <twain/twain.h>
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include "ScannerIni.h"
#include "ztwain.h"
#include <jpeg.hpp>
#include <string.h>
//---------------------------------------------------------------------------
class TFormScan : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TStatusBar *statusBar;
    TToolBar *ToolBar1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TListView *txtFnameList;
    TPanel *Panel6;
    TPanel *Panel7;
    TImage *imgF;
    TImage *imgB;
    TMainMenu *MainMenu1;
    TMenuItem *muFile;
    TMenuItem *muExit;
    TMenuItem *S1;
    TMenuItem *muSysSetup;
    TMenuItem *H1;
    TMenuItem *A1;
    TGroupBox *GroupBox1;
    TLabel *Label2;
    TDateTimePicker *txtDate;
    TLabel *Label3;
    TEdit *txtSession;
    TUpDown *UpDown;
    TGroupBox *GroupBox2;
    TButton *btnStart;
    TButton *btnStop;
    TButton *btnCancel;
    TButton *btnExit;
    TPopupMenu *popMenu;
    TMenuItem *F1;
    TCheckBox *chkScan;
    TCheckBox *chbShow;
        TButton *Rotate;
        TMenuItem *N1;
        TRadioGroup *RadioGroup1;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall muSysSetupClick(TObject *Sender);
    void __fastcall btnStartClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall imgFMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall imgBMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall txtFnameListSelectItem(TObject *Sender,
          TListItem *Item, bool Selected);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnStopClick(TObject *Sender);
    void __fastcall F1Click(TObject *Sender);
    void __fastcall muExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall RotateClick(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
private:	// User declarations
    image_info_t    Node;
    IMAGELIST       imgList;
    int             _scanType;
    int             single;       //单主件
    TJPEGImage*     img;
    void setVchImgSize(bool bStrech) ;
public:		// User declarations
    __fastcall TFormScan(TComponent* Owner);
public:		// User declarations
    HANDLE m_DbHandle;
    ZTwain scanner;
    ScannerIni m_Ini;
    String StartDir;
    String Area;     //交换区域
    bool IniLoaded;
    char    m_ipaddress[24];
    int     GetLocalHost();
    String CurPkgNo;
    vch_type_t  curVchType;
    int scancount;
    int insertFname(const char* fname,int rotate,int fcount);
    char date[12];
    char session[4];
    int  saveToDb();
    bool   bShow;
    bool   _hardRotate;
    int    _imageFormat;
    int showDS();

    string  _vchType;  //扫描凭证类型
    bool    _vchFlag; //是否指定凭证
    bool    _newModeFlag;  //是否是新版面

	char curdir[255];//扫描路径

private:
        BOOL m_bIsOpen;

};
//---------------------------------------------------------------------------
extern PACKAGE TFormScan *FormScan;
//---------------------------------------------------------------------------
#endif
