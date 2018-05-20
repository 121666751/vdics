/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//---------------------------------------------------------------------------
#include <winsock2.h>
#include <vcl.h>
#pragma hdrstop

#include "selNet.h"
#include <zcommon.h>
#include "batscan.h"
#include "main.h"
#include "ConfigFile.h"
#include "login.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmSelNet *FmSelNet;
//---------------------------------------------------------------------------
__fastcall TFmSelNet::TFmSelNet(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TFmSelNet::btnOkClick(TObject *Sender)
{
     VCHTYPELIST::iterator  pos= FormScan->m_Ini.pList->myList.begin();
     while( pos != FormScan->m_Ini.pList->myList.end())
     {
        if(strcmp(pos->vchname,txtVchType->Text.c_str())==0) break;
        ++pos;
     }

     _type = pos->scantype;
     
    if( txtPkgNo->Text.IsEmpty()  )
    {
        txtPkgNo->SetFocus();
        ModalResult = mrNone;
        return;
    }

    if( txtVchType->Text.IsEmpty()  )
    {
        txtVchType->SetFocus();
        ModalResult = mrNone;
        return;
    }

    if(txtPkgNo->Text.Length()<5 || txtPkgNo->Text.Length()>12)
    {
        MessageBox(Handle,"交换号不对!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtPkgNo->SetFocus();
        ModalResult = mrNone;
        return;
    }

     if(txtArea->Text.Length()<5)
    {
        MessageBox(Handle,"交换区域不对!\t","提示",MB_OK|MB_ICONINFORMATION);
        txtArea->SetFocus();
        ModalResult = mrNone;
        return;
    }

//---------------------------------------------------------------------------//20150511

     String pkgno;
     String exchno;
    if(txtPkgNo->Text.Trim().Length()> 6 && txtPkgNo->Text.Trim().Length() <=12)
    {
        int ret_2 = FmIC->gdb.VQuery(512,"select pkgno,exchno from bocnets where pkgno='%s' and area='%s'",txtPkgNo->Text.c_str(),txtArea->Text.c_str());

        if(ret_2)
        {
                MessageBox(Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
                return ;
        }
        MyRes res_2=FmIC->gdb.Store();
        MyRow row_2=res_2.Fetch();

        if(row_2 != NULL)
        {
                pkgno = row_2[1];
        }
        else
        {
                MessageBox(Handle,"未配置对应的归属网点与交换区域!\t","提示",MB_OK|MB_ICONINFORMATION);
                txtPkgNo->SetFocus();
                ModalResult = mrNone;
                return ;
        }
    }
    else if(txtPkgNo->Text.Trim().Length()== 6)
    {
//--------------------------------------------------------------------------------------//20150511
	 int ret_1 = 0;                           //ret_1为临时使用
	 ret_1 = FmIC->gdb.Query("select exchno,pkgno,area,merge,compare from bocnets");
	 if(ret_1)
	 {
		 MessageBox(Handle,"查询错误!\t","提示",MB_OK|MB_ICONINFORMATION);
		 txtPkgNo->SetFocus();
		 ModalResult = mrNone;
		 return ;
	 }
	 MyRes res_1 = FmIC->gdb.Store();        //res_1为临时使用
	 MyRow row_1 = res_1.Fetch();            //row_1为临时使用
	 while(row_1 != NULL)
	 {
		 if(txtPkgNo->Text == row_1[0] && txtArea->Text == row_1[2])  //判断交换号与交换区域是否对应
			 break;
		 row_1 = res_1.Fetch();
	 }

	 if(row_1 == NULL)
	 {
		 MessageBox(Handle,"未配置对应的交换号与交换区域!\t","提示",MB_OK|MB_ICONINFORMATION);
		 txtPkgNo->SetFocus();
		 ModalResult = mrNone;
		 return ;
	 }
         pkgno = txtPkgNo->Text.c_str();
    }

    int ret = 0;
    if(_type == 0 )
    {
   //20150511     ret=FmIC->gdb.VQuery(512,"select right(left(state,3),1) from bocctrl where  exchno='%s' and area='%s' and date='%s' and session='%s'" ,txtPkgNo->Text.c_str(),g_sys_area.c_str(),FormScan->date,FormScan->session);
        ret=FmIC->gdb.VQuery(512,"select right(left(state,3),1) from bocctrl where  exchno='%s' and area='%s' and date='%s' and session='%s'" ,pkgno,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }
    else if(_type == 1 )
    {
  //20150511      ret=FmIC->gdb.VQuery(512,"select right(left(state,4),1) from bocctrl where  exchno='%s' and area='%s' and date='%s' and session='%s'" ,txtPkgNo->Text.c_str(),g_sys_area.c_str(),FormScan->date,FormScan->session);
         ret=FmIC->gdb.VQuery(512,"select right(left(state,4),1) from bocctrl where  exchno='%s' and area='%s' and date='%s' and session='%s'" ,pkgno,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }
    else if(_type == 2 )
    {
   //20150511     ret=FmIC->gdb.VQuery(512,"select right(left(state,5),1) from bocctrl where  exchno='%s' and area='%s' and date='%s' and session='%s'" ,txtPkgNo->Text.c_str(),g_sys_area.c_str(),FormScan->date,FormScan->session);
        ret=FmIC->gdb.VQuery(512,"select right(left(state,5),1) from bocctrl where  exchno='%s' and area='%s' and date='%s' and session='%s'" ,pkgno,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }
    else
    {
  //20150511      ret=FmIC->gdb.VQuery(512,"select right(left(state,6),1) from bocctrl where  exchno='%s' and area='%s' and date='%s' and session='%s'" ,txtPkgNo->Text.c_str(),g_sys_area.c_str(),FormScan->date,FormScan->session);
        ret=FmIC->gdb.VQuery(512,"select right(left(state,6),1) from bocctrl where  exchno='%s' and area='%s' and date='%s' and session='%s'" ,pkgno,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }
    if(ret)
    {
        MessageBox(Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    MyRes res=FmIC->gdb.Store();
    MyRow row=res.Fetch();
    if(row == NULL  )
    {
        if(FmIC->gmode == 2 )
        {
            if( MessageBox( Handle,"交换点总控数据不存在,是否继续?\t","询问",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1) == IDNO )
            {
                ModalResult = mrNone;
                return ;
            }
        }
    }
    else
    {
        int st = atoi(row[0]);
        if(st == 1 )
        {
            if( MessageBox( Handle,"交换点已扫描,是否继续?\t","询问",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1) == IDNO )
            {
                ModalResult = mrNone;
                return ;
            }
        }
        else if(st == 2 )
        {
            if( MessageBox( Handle,"交换点扫描中,是否继续?\t","询问",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1) == IDNO )
            {
                ModalResult = mrNone;
                return ;
            }
        }
    }

    if(_type == 0 ){
//20150511        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,2),'2',right(state,'13')) where exchno='%s' and area='%s' and date='%s' and session='%s'",txtPkgNo->Text.c_str(),g_sys_area.c_str(),FormScan->date,FormScan->session);
       FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,2),'2',right(state,'13')) where exchno='%s' and area='%s' and date='%s' and session='%s'",pkgno,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }else if(_type == 1 ){
//20150511        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,3),'2',right(state,'12')) where exchno='%s' and area='%s' and date='%s' and session='%s'",txtPkgNo->Text.c_str(),g_sys_area.c_str(),FormScan->date,FormScan->session);
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,3),'2',right(state,'12')) where exchno='%s' and area='%s' and date='%s' and session='%s'",pkgno,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }else if(_type == 2 ){
//20150511        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,4),'2',right(state,'11')) where exchno='%s' and area='%s' and date='%s' and session='%s'",txtPkgNo->Text.c_str(),g_sys_area.c_str(),FormScan->date,FormScan->session);
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,4),'2',right(state,'11')) where exchno='%s' and area='%s' and date='%s' and session='%s'",pkgno,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }else{
 //20150511       FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,5),'2',right(state,'10')) where exchno='%s' and area='%s' and date='%s' and session='%s'",txtPkgNo->Text.c_str(),g_sys_area.c_str(),FormScan->date,FormScan->session);
        FmIC->gdb.VQuery(512,"update bocctrl set state=concat(left(state,5),'2',right(state,'10')) where exchno='%s' and area='%s' and date='%s' and session='%s'",pkgno,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }
}
//---------------------------------------------------------------------------


void __fastcall TFmSelNet::FormShow(TObject *Sender)
{     
    int i = 0;
    int type ;

    txtVchType->Items->Clear();
    cbType->Items->Clear();
    VCHTYPELIST::iterator  pos= FormScan->m_Ini.pList->myList.begin();
    while( pos != FormScan->m_Ini.pList->myList.end())
	{
       txtVchType->Items->Add(pos->vchname);
       if(i == 0 )   type = pos->scantype;
       i++;
       ++pos;
	}
    txtVchType->ItemIndex=0;    
    txtPkgNo->SetFocus();
    initNetno(0);

    txtPkgNo->ItemIndex = 0;
    txtPkgNo->SetFocus();


    if(txtPkgNo->Text.Length()>5 )
    {
        int ret=FmIC->gdb.VQuery(512,"select distinct area from bocnets where exchno='%s'",txtPkgNo->Text.c_str());
        if(ret)
        {
            MessageBox(FormScan->Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            return ;
        }

        MyRes res=FmIC->gdb.Store();
        MyRow row=res.Fetch();
        while(row!=NULL)
        {

            txtArea->Items->Add( row[0] );
            row=res.Fetch();
        }
        txtArea->ItemIndex = 0;
    }

   txtArea->Text = g_sys_area.c_str();

    int ret=FmIC->gdb.VQuery(512,"select distinct expcn,vchtype from vchtypes where area='%s'",g_sys_area.c_str());
    if(ret)
    {
        MessageBox(FormScan->Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    MyRes res=FmIC->gdb.Store();
    MyRow row=res.Fetch();

    _vtMap.clear();

    while(row!=NULL)
    {                           
        _vtMap[row[0]] = row[1];
        cbType->Items->Add( row[0] );
        row=res.Fetch();
    }
    cbType->ItemIndex = 0;  
    txtArea->ItemIndex = 0;           

}
//---------------------------------------------------------------------------


void __fastcall TFmSelNet::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   /* switch( (int)Key )
    {
    case 13:
        PostMessage( Handle, WM_NEXTDLGCTL, 0, 0);
        break;
    case 27:
        ModalResult = mrCancel;
        break;
    case 33:
        PostMessage( Handle, WM_NEXTDLGCTL, 1, 0);
        break;
    }     */
}
//---------------------------------------------------------------------------



void __fastcall TFmSelNet::FormDestroy(TObject *Sender)
{
 //   delete[] NetList;
}
//---------------------------------------------------------------------------

int TFmSelNet::initNetno(int type)
{
    //TODO: Add your source code here
    txtPkgNo->Items->Clear();
    txtArea->Items->Clear();


    int ret;

    //增加bocctrl已有数据
    if(type == 0 )
    {
        ret=FmIC->gdb.VQuery(512,"select distinct exchno from bocctrl where area='%s' and  right(left(state,3),1)='0' and date='%s' and session='%s' order by exchno" ,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }
    else if(type == 1 )
    {
        ret=FmIC->gdb.VQuery(512,"select distinct exchno from bocctrl where area='%s' and  right(left(state,4),1)='0' and date='%s' and session='%s' order by exchno" ,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }
    else if(type == 2 )
    {
        ret=FmIC->gdb.VQuery(512,"select distinct exchno from bocctrl where area='%s' and  right(left(state,5),1)='0' and date='%s' and session='%s' order by exchno" ,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }
    else
    {
        ret=FmIC->gdb.VQuery(512,"select distinct exchno from bocctrl where area='%s' and  right(left(state,6),1)='0' and date='%s' and session='%s' order by exchno" ,g_sys_area.c_str(),FormScan->date,FormScan->session);
    }
    if(ret)
    {
        MessageBox(FormScan->Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return -2;
    }

    MyRes res=FmIC->gdb.Store();
    MyRow row=res.Fetch();
    txtPkgNo->Items->Add("");
    while(row!=NULL)
    {   
        txtPkgNo->Items->Add( row[0] );
        row=res.Fetch();
    }

    //增加bocnets数据
    ret=FmIC->gdb.VQuery(512,"select distinct exchno from bocnets where  merge=0 and area='%s' and  exchno not in (select distinct exchno from bocctrl) order by exchno" ,g_sys_area.c_str());
    if(ret)
    {
        MessageBox(FormScan->Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return -2;
    }

    res=FmIC->gdb.Store();
    row=res.Fetch();
  
    while(row!=NULL)
    {

        txtPkgNo->Items->Add( row[0] );
        row=res.Fetch();
    }

    txtPkgNo->ItemIndex = 0;
    return 0;
} 

void __fastcall TFmSelNet::txtPkgNoDropDown(TObject *Sender)
{
     VCHTYPELIST::iterator  pos= FormScan->m_Ini.pList->myList.begin();
     while( pos != FormScan->m_Ini.pList->myList.end())
     {
        if(strcmp(pos->vchname,txtVchType->Text.c_str())==0) break;
        ++pos;
     }

     _type = pos->scantype;

    initNetno(pos->scantype);
}
//---------------------------------------------------------------------------




void __fastcall TFmSelNet::txtPkgNoChange(TObject *Sender)
{
     txtArea->Items->Clear();
    int ret=FmIC->gdb.VQuery(512,"select distinct area from bocnets where exchno='%s'",txtPkgNo->Text.c_str());
    if(ret)
    {
        MessageBox(FormScan->Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    
    MyRes res=FmIC->gdb.Store();
    MyRow row=res.Fetch();
    while(row!=NULL)
    {

        txtArea->Items->Add( row[0] );
        row=res.Fetch();
    }
    txtArea->ItemIndex = 0;

    txtArea->Text = g_sys_area.c_str();
}
//---------------------------------------------------------------------------

void __fastcall TFmSelNet::txtPkgNoExit(TObject *Sender)
{
    txtArea->Items->Clear();
    int ret=FmIC->gdb.VQuery(512,"select distinct area from bocnets where exchno='%s'",txtPkgNo->Text.c_str());
    if(ret)
    {
        MessageBox(FormScan->Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    
    MyRes res=FmIC->gdb.Store();
    MyRow row=res.Fetch();
    while(row!=NULL)
    {

        txtArea->Items->Add( row[0] );
        row=res.Fetch();
    }
    txtArea->ItemIndex = 0;

    txtArea->SetFocus();

    txtArea->Text = g_sys_area.c_str();
}
//---------------------------------------------------------------------------



void __fastcall TFmSelNet::cbVchtypeClick(TObject *Sender)
{
    if(cbVchtype->Checked == true)
    {
        cbType->Visible = true;
        LabelType->Visible = true;
        cbNewMode->Visible = true;

    }
    else
    {
        cbType->Visible = false;
        LabelType->Visible = false;
        cbNewMode->Visible = false;
    }
}
//---------------------------------------------------------------------------


