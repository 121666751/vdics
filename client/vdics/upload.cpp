//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <zsocket.hpp>
#include <AnsiMemfile.h>
#include <libmem.h>
#include <transimg.h>
#include <jpeg.hpp>
#include "main.h"
#include "image.h"
#include "upload.h"
#include "main.h"
#include "sendForm.h"
#include "sendSpecForm.h"
#include "pubfun.h"
#include "login.h"
#include "replacescan.h"
#include "mainimage.h"

#include "warrant.h"

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTrans *FormTrans;

void utf8_to_ansi(char* src, char* &dest) 
{
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, src, strlen(src), NULL, 0);//测试转换
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, src, strlen(src), wszString, wcsLen);  //utf8 转Unicode
	wszString[wcsLen] = '\0';

	int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	dest = new char[ansiLen + 1];        //unicode版对应的strlen是wcslen
	::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), dest, ansiLen, NULL, NULL);
	dest[ansiLen] = '\0';
	delete [] wszString;
}

//--------------------------------------------------------------------------- 
__fastcall TFormTrans::TFormTrans(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TFormTrans::btnExitClick(TObject *Sender)
{
    if(MessageBox(Handle,"确实要退出?","提示",MB_YESNO|MB_ICONINFORMATION) == IDNO) return;
    Close();
}
//---------------------------------------------------------------------------
int TFormTrans::flushData()
{
  
    
    String cond;

    cond = AnsiString("  date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";
     
    if(txtExchno->ItemIndex == 0 )
        cond = cond + " and 1=1 ";
    else
        cond = cond + AnsiString(" and exchno = '")+txtExchno->Text.c_str()+"'";
 
    if(txtSend->ItemIndex == 0 )
        cond = cond + " and 1=1  ";
    else if(txtSend->ItemIndex == 1 )
        cond =  cond + " and sendstate=1  ";
    else
        cond =  cond + " and ( sendstate=0 or sendstate=2 or sendstate=3 or sendstate=4 or sendstate=5)  ";

    if(cbTimes->ItemIndex == 0 )
        cond = cond + " and 1=1  order by sendstate desc,times";
    else if(cbTimes->ItemIndex == 1 )
        cond =  cond + " and times<1  order by sendstate desc,times";
    else
        cond =  cond + " and  times>=1   order by sendstate desc,times";
 //-----------------------------------20150414----------------------------------
        
    String str = "select sid,exchno,accno,vchno, vchtype,amount,sendstate,imageid ,fname,sendcode,senddesc,date,session,area from vouchers where  mergestate=1 and state=1 and ";
 //--------------------------------------end------------------------------------

//    String str = "select sid,exchno,accno,vchno, vchtype,amount,sendstate,imageid ,fname,sendcode,senddesc,date,session from vouchers where  mergestate=1 and state=1 and ";
    str = str +cond;

	debugout(str.c_str());

    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         return ret;
    }

    Screen->Cursor = crHourGlass;
    txtDetail->Items->BeginUpdate();
    txtDetail->Clear();
    TListItem* li;
    MyRes res = FmIC->gdb.Store();                 
    MyRow row = res.Fetch();
    while(row!=NULL)
    {
//------------------------------------------------- 20150414 -------------------
        if(!strcmp(FmIC->m_area.c_str(),row[13]))
        {
                 li = txtDetail->Items->Add();
                li->Caption = row[0];
                 li->SubItems->Insert(0,row[1]);
                 li->SubItems->Insert(1,row[2]);
                 li->SubItems->Insert(2,row[3]);
                li->SubItems->Insert(3,row[4]);
                li->SubItems->Insert(4,row[5]);

                 if(atoi(row[6])== 1 )
                li->SubItems->Insert(5,"已送");
                else  if(atoi(row[6])== 2 )
                li->SubItems->Insert(5,"上送中");
                 else  if(atoi(row[6])== 3 )
                li->SubItems->Insert(5,"上送失败");
                else  if(atoi(row[6])== 4 )
                li->SubItems->Insert(5,"ECM上送失败");
                else  if(atoi(row[6])== 5 )
                li->SubItems->Insert(5,"流程触发超时");
                else
                        li->SubItems->Insert(5,"未送");

                li->SubItems->Insert(6,row[7]);


                li->SubItems->Insert(7,row[11]);
                li->SubItems->Insert(8,row[12]);

	        char *tmp = NULL;

	        debugout("tmp = %s",row[10]);
	        try
	        {
		   utf8_to_ansi(row[10], tmp);

	        }
	        catch(...)
	        {
		   strcpy(tmp,row[10]);
	        }

	        debugout("tmp = %s",tmp);

                 li->SubItems->Insert(9,tmp);

                li->Data = (void*)atoi(row[6]);

                }
                row = res.Fetch();

//------------------------------------------------- end ------------------------

#if 0
       li = txtDetail->Items->Add();
       li->Caption = row[0];
       li->SubItems->Insert(0,row[1]);
       li->SubItems->Insert(1,row[2]);
       li->SubItems->Insert(2,row[3]);
       li->SubItems->Insert(3,row[4]);
       li->SubItems->Insert(4,row[5]);

       if(atoi(row[6])== 1 )
           li->SubItems->Insert(5,"已送");
       else  if(atoi(row[6])== 2 )
           li->SubItems->Insert(5,"上送中");
       else  if(atoi(row[6])== 3 )
           li->SubItems->Insert(5,"上送失败");
       else  if(atoi(row[6])== 4 )
           li->SubItems->Insert(5,"ECM上送失败");
       else  if(atoi(row[6])== 5 )
           li->SubItems->Insert(5,"流程触发超时");
       else
           li->SubItems->Insert(5,"未送");

       li->SubItems->Insert(6,row[7]);


       li->SubItems->Insert(7,row[11]);
       li->SubItems->Insert(8,row[12]);

	   char *tmp = NULL;

	   debugout("tmp = %s",row[10]);
	   try
	   {
		   utf8_to_ansi(row[10], tmp);

	   }
	   catch(...)
	   {
		   strcpy(tmp,row[10]);
	   }

	   debugout("tmp = %s",tmp);

       li->SubItems->Insert(9,tmp);

       li->Data = (void*)atoi(row[6]);
       row = res.Fetch();
#endif
    }

    txtDetail->Items->EndUpdate();
    Screen->Cursor = crDefault;

    str = "select count(*),sum(amount) from vouchers  where  mergestate=1 and state=1 and";

    str = str +cond;
    ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }

    res =  FmIC->gdb.Store();
    row = res.Fetch();

    StatusBar->Panels->Items[0]->Text = String("交换号[")+txtExchno->Text.c_str()+"]";
    StatusBar->Panels->Items[1]->Text = String("总笔数[")+row[0]+"]";
    StatusBar->Panels->Items[2]->Text = String("总金额[")+row[1]+"]";
   
    return 0;
}
void __fastcall TFormTrans::FormShow(TObject *Sender)
{
  
    memset(date,0,sizeof(date));
    memset(session,0,sizeof(session));
    FmIC->getDS("date",date);
    FmIC->getDS("session",session);
    txtDate->Text = date;
    txtSession->Text = session;

    memset(&_cfg,0,sizeof(_cfg));
    getCFG();
    flushExchno();
    flushData();


}
//---------------------------------------------------------------------------
void __fastcall TFormTrans::btnRefreshClick(TObject *Sender)
{
    flushData();
}
//---------------------------------------------------------------------------
void __fastcall TFormTrans::txtSendCloseUp(TObject *Sender)
{
    flushData();    
}
//---------------------------------------------------------------------------

int TFormTrans::getCFG()
{
      return 0;
}
void __fastcall TFormTrans::txtDetailDblClick(TObject *Sender)
{
    TListItem* li;
    li= txtDetail->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();
    int ret = FmIC->gdb.VQuery(512,"select right(accno,12),vchno,fname,bname,ip ,rotate,amount,right(pkgno,5),date,session,vchtype from vouchers where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return ;
    char ip[24];
    char fname[80];
    char bname[80];
    char accno[32];
    char vchno[30];
    char amount[16];
    int rotate;

    memset(ip,0,sizeof(ip));
    memset(fname,0,sizeof(fname));
    memset(bname,0,sizeof(bname));
    memset(vchno,0,sizeof(vchno));
    memset(amount,0,sizeof(amount));

    strcpy(accno,row[0]);
    strcpy(vchno,row[1]);
    strcpy(fname,row[2]);
    strcpy(bname,row[3]);
    strcpy(ip,row[4]);
    rotate  = atoi(row[5]);
    strcpy(amount,row[6]);

    FormImage->txtAccno->Text = accno;
    FormImage->txtVchno->Text = vchno;
    FormImage->txtAmount->Text = amount;
    FormImage->txtVchtype->Text = row[10];
    FormImage->btnSave->Visible = false;

    char tname[128];
    memset(tname,0,sizeof(tname));
    sprintf(tname,"%s//%02d//%05d//%s",row[8],atoi(row[9]),atoi(row[7]),fname);
    

   MemT image;
    int sizex;
    int sizey;
    int panel;
    
    ret =  ti_getimage( tname, ip, rotate,   image, sizex, sizey,panel );
    if(ret)
    {
        MessageBox(Handle,"获取图像错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    unsigned char * bmp;
    int size;

    bmp =ti_RGB2BMP(image.data(),sizex,sizey,0,0,0,0,sizex,sizey,size);

    TMemoryStream* pms = new TMemoryStream();
    pms->Clear();
    pms->SetSize(size);
    ret = pms->Write(bmp,size);
    pms->Position = 0;
    FormImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();
    ti_RGB2BMPFREE(bmp);
    ret = FormImage->ShowModal();

    
}
//---------------------------------------------------------------------------
void __fastcall TFormTrans::FindDlgFind(TObject *Sender)
{
    String find_str = FindDlg->FindText;
    FindDlg->CloseDialog();
    if( find_str.IsEmpty() ) return;

    int sub_no, start_index;
    start_index = txtDetail->Selected? txtDetail->Selected->Index:0;
    txtDetail->SetFocus();
    if(start_index == 0) txtDetail->Items->Item[0]->Focused = true;
    bool found = false;

    //从当前位置开始查询
    for( int i=start_index+1; i<txtDetail->Items->Count; i++ )
    {
        TListItem* pItem = txtDetail->Items->Item[i];
        if( FindResult(find_str,pItem,false) )
        {
            pItem->MakeVisible(false);
            pItem->Selected = true;
            pItem->Focused= true;
            found = true;
            break;
        }
    }

    //如果没有找到,从头开始查询
    if( !found )
    {
        for( int i=0; i<start_index; i++ )
        {
            TListItem* pItem = txtDetail->Items->Item[i];
            if( FindResult(find_str,pItem,false) )
            {
                pItem->MakeVisible(false);
                pItem->Selected = true;
                pItem->Focused = true;
                break;
            }
        }
    }    
}

//---------------------------------------------------------------------------
bool TFormTrans::FindResult(String StrToFind, TListItem* pItem, bool bExact)
{
    //TODO: Add your source code here
     //按帐号进行查找
    String sub_str = pItem->SubItems->Strings[1];
    bool found = (sub_str.Pos(StrToFind)>0);

    if( found ) return true;

    //按凭证号进行查找
    sub_str = pItem->SubItems->Strings[2];
    found = (sub_str.Pos(StrToFind)>0);

    if( found ) return true;

    //按金额进行查询
    sub_str = pItem->SubItems->Strings[4];
    found = (sub_str.Pos(StrToFind)>0);

    if( found ) return true;

    //按影像ID进行查找
    sub_str =  pItem->SubItems->Strings[6];
    return (sub_str.Pos(StrToFind)>0);
}
void __fastcall TFormTrans::R1Click(TObject *Sender)
{
    TListItem* li;
    li= txtDetail->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();
    int ret = FmIC->gdb.VQuery(512,"select sendstate,imageid,date,session,right(pkgno,5),fname,ip,rotate ,vchtype,modeflag from vouchers where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL || row[0]==NULL || strcmp(row[0],"")==0)  return ;

    ret = atoi(row[0]);
    if( !(ret==1 || ret==5))
    {
        MessageBox(Handle,"流程未触发,不能执行交易!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }

	if(!FmIC->m_bReStart)
    {
    		MessageBox(Handle,"该柜员没有流程重启权限,不能重新启动流程!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;	
    }

    if(MessageBox(Handle,"是否要重新启动流程?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

    if(MessageBox(Handle,"流程系统已经删除先前流程?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;

/*
    MessageBox(Handle,"重新启动流程需要授权!\t","提示",MB_OK|MB_ICONINFORMATION);

    if( FormWarrant->ShowModal() == mrCancel)
    {
        MessageBox(Handle,"授权失败,不能重新启动流程!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
 */   
    FormMainImage->txtVchtype->Text = row[8];

    char tname[128];
    memset(tname,0,sizeof(tname));
    sprintf(tname,"%s//%02d//%05d//%s",row[2],atoi(row[3]),atoi(row[4]),row[5]);

    MemT image;

    int sizex;
    int sizey;
    int panel;


    ret =  ti_getimage( tname, row[6], atoi(row[7]),   image, sizex, sizey,panel );
    if(ret)
    {
        MessageBox(Handle,"获取图像错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }

    unsigned char * bmp;
    int size;

    bmp =ti_RGB2BMP(image.data(),sizex,sizey,0,0,0,0,sizex,sizey,size);

    TMemoryStream* pms = new TMemoryStream();
    pms->Clear();
    pms->SetSize(size);
    ret = pms->Write(bmp,size);
    pms->Position = 0;
    FormMainImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    ti_RGB2BMPFREE(bmp);//
    
    ret = FormMainImage->ShowModal();         //
    if(ret != mrOk) return ;

    int modeflag = FormMainImage->chkMode->Checked?1:0;

    ret = FmIC->gdb.VQuery(512,"update vouchers set sendstate=0 ,vchtype='%s',imageid='',modeflag=%d where sid=%d",FormMainImage->txtVchtype->Text.c_str(),modeflag,sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }

    WriteToLogFile(g_sys_opt.c_str(),"流程重启",sid,"");
    MessageBox(Handle,"交易成功!\t","提示",MB_OK|MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

int TFormTrans::flushExchno()
{
    //TODO: Add your source code here
    txtExchno->Clear();

	debugout("select distinct exchno from vouchers where date='%s' and session='%s' order by exchno",txtDate->Text.c_str(),txtSession->Text.c_str());

    int ret = FmIC->gdb.VQuery(512,"select distinct exchno from vouchers where date='%s' and session='%s' and area='%s' order by exchno",txtDate->Text.c_str(),txtSession->Text.c_str(),FmIC->m_area.c_str());
    //int ret = FmIC->gdb.VQuery(512,"select distinct exchno from vouchers where date='%s' and session='%s' order by exchno",txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ret;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    txtExchno->Items->Add("全部");
    while(row!=NULL)
    {
        txtExchno->Items->Add(row[0]);
        row = res.Fetch();
    }

    txtExchno->ItemIndex = 0 ;
    return 0;
}
void __fastcall TFormTrans::txtExchnoDropDown(TObject *Sender)
{
    flushExchno();    
}
//---------------------------------------------------------------------------

void __fastcall TFormTrans::txtExchnoChange(TObject *Sender)
{
     flushData();
}
//---------------------------------------------------------------------------    
void __fastcall TFormTrans::E2Click(TObject *Sender)
{
    FormSend->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TFormTrans::N3Click(TObject *Sender)
{
    FormSendSpec->ShowModal();   
}
//---------------------------------------------------------------------------


void __fastcall TFormTrans::MeForceLookClick(TObject *Sender)
{

   
    TListItem* li;
    li= txtDetail->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();
    int ret = FmIC->gdb.VQuery(512,"select sendstate,imageid,date,session,pkgno from vouchers where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
    MyRes res  = FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL || row[0]==NULL || strcmp(row[0],"")==0)  return ;

    ret = atoi(row[0]);
    if(ret!=1)
    {
        MessageBox(Handle,"影像未上传,不能进行影像替换!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }

		if(!FmIC->m_bReplace)
    {
    		MessageBox(Handle,"该柜员没有影像替换权限,不能进行影像替换!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;	
    }
    
/*
    MessageBox(Handle,"影像替换需要授权!\t","提示",MB_OK|MB_ICONINFORMATION);
    if( FormWarrant->ShowModal() == mrCancel)
    {
        MessageBox(Handle,"授权失败,不能进行影像替换!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
*/
    FormReplace->setDS(sid,row[2],row[3],row[4],row[1],"vouchers");

    FormReplace->ShowModal();

}
//---------------------------------------------------------------------------

void __fastcall TFormTrans::N4Click(TObject *Sender)
{
    FindDlg->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TFormTrans::E1Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TFormTrans::txtDetailCustomDrawItem(
      TCustomListView *Sender, TListItem *Item, TCustomDrawState State,
      bool &DefaultDraw)
{
    TColor color;
    switch( (int)Item->Data)
    {
       
        case 0:
        case 2:
        case 3:
        case 4:
            color = clLime;                  //待上送
            break;
        case 1:
            color = clWhite;
            break;
        case 5:
            color = clRed;
            break;
        default :
            color = clRed;
            break;
    }
    Sender->Canvas->Brush->Color =  color;
    Sender->Canvas->FillRect(Item->DisplayRect(drBounds));
}
//---------------------------------------------------------------------------


void __fastcall TFormTrans::UpDown1Click(TObject *Sender,
      TUDBtnType Button)
{
    flushExchno();
    flushData();        
}
//---------------------------------------------------------------------------

