/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//---------------------------------------------------------------------------

#ifndef pbcimgH
#define pbcimgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include "main.h"
//---------------------------------------------------------------------------
class TFormDet : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TToolBar *ToolBarView;
    TToolButton *TBExit;
    TImageList *ImageList1;
    TPanel *Panel2;
    TStatusBar *StatusBar1;
    TStringGrid *txtInfo;
    TPanel *Panel3;
    TImage *txtImage;
    TMainMenu *MainMenu1;
    TMenuItem *N3;
    TMenuItem *numExit;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall TBExitClick(TObject *Sender);
    void __fastcall numExitClick(TObject *Sender);
    void __fastcall txtImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
    pbc_node_t _node;
public:		// User declarations
    __fastcall TFormDet(TComponent* Owner);
    int SetData(pbc_node_t node);
    int ShowImage();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDet *FormDet;
//---------------------------------------------------------------------------
#endif
