//---------------------------------------------------------------------------

#ifndef collectionH
#define collectionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "pubfun.h"
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormWS : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TStatusBar *statusBar;
    TPanel *Panel4;
    TPageControl *pageCtrl;
    TTabSheet *TabSheet1;
    TListView *lstMerge;
    TTabSheet *TabSheet2;
    TListView *lstEnvs;
    TTabSheet *TabSheet3;
    TListView *lstSpecial;
    TListView *lstMain;
    TPanel *Panel3;
    TLabel *Label2;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TEdit *txtDate;
    TEdit *txtSession;
    TComboBox *cmbNetno;
    TComboBox *cmbIdentify;
    TComboBox *cbCheck;
    TButton *btnExit;
    TButton *btnRefresh;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *E1;
    TMenuItem *N2;
    TMenuItem *R1;
    TMenuItem *H1;
    TMenuItem *A1;
    TPopupMenu *ErrResPopMenu;
    TMenuItem *MeForceLook;
    TMenuItem *B1;
    TButton *btnMerge;
    TComboBox *cbMerge;
    TLabel *Label1;
    TTabSheet *TabSheet4;
    TListView *lstFs;
    TComboBox *cmbNetState;
    TLabel *Label8;
    TTabSheet *TabSheet5;
    TListView *txtDetail;
    TMenuItem *I1;
    TFindDialog *FindDlg;
    TMenuItem *F1;
        TUpDown *UpDown1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall B1Click(TObject *Sender);
    void __fastcall R1Click(TObject *Sender);
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall lstMergeDblClick(TObject *Sender);
    void __fastcall lstEnvsDblClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall cmbNetnoDropDown(TObject *Sender);
    void __fastcall cmbIdentifyCloseUp(TObject *Sender);
    void __fastcall cbMergeCloseUp(TObject *Sender);
    void __fastcall cbCheckCloseUp(TObject *Sender);
    void __fastcall MeForceLookClick(TObject *Sender);
    void __fastcall lstMainDblClick(TObject *Sender);
    void __fastcall pageCtrlChange(TObject *Sender);
    void __fastcall btnMergeClick(TObject *Sender);
    void __fastcall cmbNetStateCloseUp(TObject *Sender);
    void __fastcall lstFsDblClick(TObject *Sender);
    void __fastcall txtDetailDblClick(TObject *Sender);
    void __fastcall I1Click(TObject *Sender);
    void __fastcall FindDlgFind(TObject *Sender);
    void __fastcall F1Click(TObject *Sender);
    void __fastcall cmbNetnoSelect(TObject *Sender);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
        void __fastcall txtSessionChange(TObject *Sender);
        void __fastcall UpDown1Changing(TObject *Sender,
          bool &AllowChange);
private:	// User declarations
    char date[12];
    char session[4];
    XmlMap _vtMap;
    String curNetno;
    String lastNetno;
    String curSess;
    String lastSess;
    bool locked;
    
public:		// User declarations
    __fastcall TFormWS(TComponent* Owner);

    int flushExchno(int state);
    int flushData1();
    int flushData2();
    int flushData3();
    int flushData4();
    int subMit(const char* exchno);
    int flushData5();
    int getNext();
    bool boolExchno;
    int CheckComplex(const char* exchno);
    bool FindResult(String StrToFind, TListItem* pItem, bool bExact);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormWS *FormWS;
//---------------------------------------------------------------------------
#endif
