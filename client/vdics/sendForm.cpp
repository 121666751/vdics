/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "images.h"
#include "sendForm.h"

#include "main.h"
#include <myplus.h>
#include <libmem.h>
#include <transimg.h>
#include <zcommon.h>
#include "pubfun.h"
#include "login.h"
#include "replacescan.h"
#include "mainimage.h"

#include "warrant.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSend *FormSend;



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
__fastcall TFormSend::TFormSend(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


int TFormSend::flushExchno()
{
    //TODO: Add your source code here
   
    cmbNetno->Clear();
    int ret =  FmIC->gdb.VQuery(512,"select distinct exchno from multis where date='%s' and session='%s' order  by exchno",txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    MyRes res =  FmIC->gdb.Store();
    MyRow row = res.Fetch();

    cmbNetno->Items->Add("全部");
    while(row!=NULL)
    {
        cmbNetno->Items->Add(row[0]);
        row = res.Fetch();
    }

    cmbNetno->ItemIndex = 0 ;
}
void __fastcall TFormSend::FormShow(TObject *Sender)
{
    char date[12];
    char session[4];
    memset(date,0,sizeof(date));
    memset(session,0,sizeof(session));
    FmIC->getDS("date",date);
    FmIC->getDS("session",session);
    txtDate->Text = date;
    txtSession->Text = session;

    flushExchno();
    flushData();
}
//---------------------------------------------------------------------------
void __fastcall TFormSend::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormSend::btnRefreshClick(TObject *Sender)
{
    //TODO: Add your source code here
    flushData();
}
//---------------------------------------------------------------------------
void __fastcall TFormSend::lstSendDblClick(TObject *Sender)
{
    TListItem* li;
    li= lstSend->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();

    int ret =  FmIC->gdb.VQuery(512,"select  ip,rotate,substate,amount,vchtype,fname,bname,right(pkgno,5),vchtype,date,session  from multis where sid=%d",sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
    MyRes res  =  FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return ;

    char ip[24];

    char amount[16];
    char fname[80];
    char bname[80];
    char pkgno[12];
    int  submitstate;
    int  rotate;
    int  vchflag;
    int  vchtype;
      
    memset(ip,0,sizeof(ip));

    memset(amount,0,sizeof(amount));
    memset(fname,0,sizeof(fname));
    memset(bname,0,sizeof(bname));
    memset(pkgno,0,sizeof(pkgno));
    strcpy(ip,row[0]);
    rotate = atoi(row[1]);
    submitstate = atoi(row[2]);     

    strcpy(amount,row[3]);
    vchflag = atoi(row[4]);

    strcpy(fname,row[5]);
    strcpy(bname,row[6]);
    strcpy(pkgno,row[7]);
    vchtype = atoi(row[8]);


    MemT image;
    int sizex;
    int sizey;
    int panel;

    char tname[128];
    memset(tname,0,sizeof(tname));
    sprintf(tname,"%s//%02d//%05d//%s",row[9],atoi(row[10]),atoi(row[7]),fname);

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
    ti_RGB2BMPFREE(bmp);
    pms->Position = 0;
    FormImages->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    FormImages->setMethod(1,sid);

     
    ret = FormImages->ShowModal();
    
}
//---------------------------------------------------------------------------


int TFormSend::flushData()
{
    //TODO: Add your source code here
    String cond;

    cond = AnsiString("  date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"' ";
    
    if(cmbNetno->ItemIndex == 0 )
        cond = cond + "  and 1=1";
    else
        cond = cond + AnsiString(" and exchno = '")+cmbNetno->Text.c_str()+"'";

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
        
        
    String str = "select sid ,exchno , fname,vchtype, amount,sendstate,imageid,sendcode,senddesc,date,session  from multis where substate=1 and mainflag=1 and ";
    str = str +cond;
    int ret =  FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         return 0;
    }


    Screen->Cursor = crHourGlass;
    lstSend->Items->BeginUpdate();
    lstSend->Clear();
    TListItem* li;
    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();
    
    while(row!=NULL)
    {
       li = lstSend->Items->Add();
       li->Caption = row[0];
       li->SubItems->Insert(0,row[1]);
       li->SubItems->Insert(1,row[2]);
       li->SubItems->Insert(2,row[3]);
       li->SubItems->Insert(3,row[4]);

       if(atoi(row[5])== 1)
            li->SubItems->Insert(4,"已送");
       else  if(atoi(row[5])== 2 )
           li->SubItems->Insert(4,"上送中");
       else  if(atoi(row[5])== 3 )
           li->SubItems->Insert(4,"上送失败");
       else  if(atoi(row[5])== 4 )
           li->SubItems->Insert(4,"ECM上送失败");
       else  if(atoi(row[5])== 5 )
           li->SubItems->Insert(4,"流程触发超时");
       else
           li->SubItems->Insert(4,"未送");

       li->SubItems->Insert(5,row[6]);

       li->SubItems->Insert(6,row[9]);

       li->SubItems->Insert(7,row[10]);

	   char *tmp = NULL;

	   debugout("tmp = %s",row[8]);
	   try
	   {
		   utf8_to_ansi(row[8], tmp);	

	   }
	   catch(...)
	   {
		   strcpy(tmp,row[8]);
	   }

	   debugout("tmp = %s",tmp);


	   li->SubItems->Insert(8,tmp);


       li->Data = (void*)atoi(row[5]);
       
       row = res.Fetch();
    }

    lstSend->Items->EndUpdate();

    Screen->Cursor = crDefault;

     str = "select count(*),sum(amount) from  multis where substate=1 and mainflag=1 and  ";

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

    StatusBar->Panels->Items[0]->Text = String("交换号[")+cmbNetno->Text.c_str()+"]";
    StatusBar->Panels->Items[1]->Text = String("总笔数[")+row[0]+"]";
    StatusBar->Panels->Items[2]->Text = String("总金额[")+row[1]+"]";

    return 0;
}
void __fastcall TFormSend::txtSendCloseUp(TObject *Sender)
{
    flushData();    
}
//---------------------------------------------------------------------------
void __fastcall TFormSend::cmbNetnoDropDown(TObject *Sender)
{
    flushExchno();    
}
//---------------------------------------------------------------------------
void __fastcall TFormSend::cmbNetnoChange(TObject *Sender)
{
     flushData();      
}
//---------------------------------------------------------------------------


void __fastcall TFormSend::MeForceLookClick(TObject *Sender)
{
    TListItem* li;
    li= lstSend->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();
    int ret = FmIC->gdb.VQuery(512,"select sendstate,imageid,date,session,pkgno from multis where sid=%d",sid);
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
  
    FormReplace->setDS(sid,row[2],row[3],row[4],row[1],"multis");

    FormReplace->ShowModal();
    
}
//---------------------------------------------------------------------------

void __fastcall TFormSend::N1Click(TObject *Sender)
{
    TListItem* li;
    li= lstSend->Selected;
    if(li==NULL) return ;

    int sid = li->Caption.ToInt();
    int ret = FmIC->gdb.VQuery(512,"select sendstate,imageid,date,session,right(pkgno,5),fname,ip,rotate ,vchtype from multis where sid=%d",sid);
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

    ti_RGB2BMPFREE(bmp);


    ret = FormMainImage->ShowModal();
    if(ret != mrOk) return ;

    int modeflag = FormMainImage->chkMode->Checked?1:0;
    ret = FmIC->gdb.VQuery(512,"update multis set sendstate=0 ,vchtype='%s',imageid='',modeflag=%d where sid=%d",FormMainImage->txtVchtype->Text.c_str(),modeflag,sid);
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }


    WriteToLogFile(g_sys_opt.c_str(),"流程重启",sid,"");
    MessageBox(Handle,"交易成功!\t","提示",MB_OK|MB_ICONINFORMATION);    
}
//---------------------------------------------------------------------------

void __fastcall TFormSend::E1Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TFormSend::N4Click(TObject *Sender)
{
    FindDlg->Execute();        
}
//---------------------------------------------------------------------------

void __fastcall TFormSend::FindDlgFind(TObject *Sender)
{
    String find_str = FindDlg->FindText;
    FindDlg->CloseDialog();
    if( find_str.IsEmpty() ) return;

    int sub_no, start_index;
    start_index = lstSend->Selected? lstSend->Selected->Index:0;
    lstSend->SetFocus();
    if(start_index == 0) lstSend->Items->Item[0]->Focused = true;
    bool found = false;

    //从当前位置开始查询
    for( int i=start_index+1; i<lstSend->Items->Count; i++ )
    {
        TListItem* pItem = lstSend->Items->Item[i];
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
            TListItem* pItem = lstSend->Items->Item[i];
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
bool TFormSend::FindResult(String StrToFind, TListItem* pItem, bool bExact)
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
    sub_str =  pItem->SubItems->Strings[5];
    return (sub_str.Pos(StrToFind)>0);
}

void __fastcall TFormSend::lstSendCustomDrawItem(TCustomListView *Sender,
      TListItem *Item, TCustomDrawState State, bool &DefaultDraw)
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

void __fastcall TFormSend::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
    flushExchno();
    flushData();        
}
//---------------------------------------------------------------------------

