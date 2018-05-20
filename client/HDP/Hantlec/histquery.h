/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//------------------------------------------------------------------
#ifndef histqueryH
#define histqueryH

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
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <myplus.h>
//----------------------------------------------------------------------------
class THistQueryDlg : public TForm
{
__published:
    TLabel *Label2;
    TImageList *ImageList1;
    TPanel *Panel1;
    TListView *VchList;
    TLabel *Label1;
    TLabel *Label3;
    TLabel *Label4;
    TButton *btnQuery;
    TButton *btnExit;
    TEdit *txtNetNo;
    TEdit *txtAccNo;
    TEdit *txtAmount;
    TBevel *Bevel1;
    void __fastcall btnQueryClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:
    void AddResult(String vchno, String netno, String accno, String tc, String amount);
    void Query();
    void LocateList();
public:
	virtual __fastcall THistQueryDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
//extern PACKAGE THistQueryDlg *HistQueryDlg;
//----------------------------------------------------------------------------
#endif    
