//---------------------------------------------------------------------------

#ifndef replacescanH
#define replacescanH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>

#include "ScannerIni.h"
#include "ztwain.h"
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TFormReplace : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TStatusBar *statusBar;
    TToolBar *ToolBar1;
    TPanel *Panel2;
    TPanel *Panel3;
    TListView *txtFnameList;
    TPanel *Panel4;
    TGroupBox *GroupBox1;
    TLabel *Label2;
    TLabel *Label3;
    TDateTimePicker *txtDate;
    TEdit *txtSession;
    TUpDown *UpDown;
    TGroupBox *GroupBox2;
    TButton *btnStart;
    TButton *btnStop;
    TButton *btnCancel;
    TButton *btnExit;
    TCheckBox *chkScan;
    TCheckBox *chbShow;
    TPanel *Panel5;
    TPanel *Panel6;
    TImage *imgF;
    TPanel *Panel7;
    TImage *imgB;
    TMainMenu *MainMenu1;
    TMenuItem *muFile;
    TMenuItem *muExit;
    TMenuItem *S1;
    TMenuItem *muSysSetup;
    TMenuItem *H1;
    TMenuItem *A1;
    TPopupMenu *popMenu;
    TMenuItem *F1;
    void __fastcall muExitClick(TObject *Sender);
    void __fastcall btnStartClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall imgFMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall imgBMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall txtFnameListSelectItem(TObject *Sender,
          TListItem *Item, bool Selected);
    void __fastcall F1Click(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall btnStopClick(TObject *Sender);
    void __fastcall muSysSetupClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    char    m_ipaddress[24];
    char _date[12];
    char _session[4];
    char _exchno[16];
    char _imageid[80];
    char _tbname[12];

    bool bShow;
    bool _hardRotate;
    int  _imageFormat;
    image_info_t    Node;
    IMAGELIST       imgList;
    int  scancount;
    bool IniLoaded;
    char curdir[255];
    int  _type;
    int  _multiflag;
    int  _envid;
    int  _ckst;

public:		// User declarations
    __fastcall TFormReplace(TComponent* Owner);
    int GetLocalHost();
    int setDS(int sid,char*date,char*session,char*exchno,char*imageid,char*tab);
    int insertFname(const char* fname, int rotate, int count);
    int setDSEx(int envid,char*date,char*session,char*exchno,int multiflag,int ck);

    int setDSEx2();

    int setDSEx3();

    HANDLE m_DbHandle;
    ZTwain scanner;
    ScannerIni m_Ini;
    String StartDir;
    int setReplace();
    int setInsert();

    int setReplaceOld();
    int setReStartOld();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReplace *FormReplace;
//---------------------------------------------------------------------------
#endif
