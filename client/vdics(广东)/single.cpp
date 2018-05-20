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
#include <transimg.h>
#include <bcbext.h>
#include <TC_ImageLib.h>

#include "single.h"
#include "image.h"
#include "login.h"
#include "ConfigFile.h"
#include "main.h"
#include "login.h"
#include "pubfun.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmVoucher *FmVoucher;

//---------------------------------------------------------------------------
__fastcall TFmVoucher::TFmVoucher(TComponent* Owner)
    : TForm(Owner)
{
    _needMerge = true;
}
//---------------------------------------------------------------------------
void __fastcall TFmVoucher::btnExitClick(TObject *Sender)
{
    Close(); 
}
//---------------------------------------------------------------------------
void __fastcall TFmVoucher::FormShow(TObject *Sender)
{
    FmIC->getDS("date",date);
    FmIC->getDS("session",session);
    txtDate->Text = date;
    txtSession->Text = session;


    flushExchno();
    Screen->Cursor = crHourGlass;
    flushData();
    Screen->Cursor = crDefault;

}
//---------------------------------------------------------------------------


int TFmVoucher::flushData()
{

    
    String cond;

    cond = AnsiString(" where date='")+txtDate->Text.c_str()+"' and session='" + txtSession->Text.c_str()  +"'";
    
    if(txtNetno->ItemIndex == 0 )
        cond = cond + " and 1=1";
    else
    //    cond = cond + AnsiString(" and exchno = '")+txtNetno->Text.c_str()+"'";
        cond = cond + AnsiString(" and pkgno = '")+txtNetno->Text.c_str()+"'";

    cond = cond + " and area='"+g_sys_area.c_str()+"'";       

    if(txtIdentify->ItemIndex == 0 )
        cond = cond + "  ";
    else if(txtIdentify->ItemIndex == 1 )
        cond = cond = cond + " and state=1  ";
    else if(txtIdentify->ItemIndex == 2 )
        cond = cond + " and ( state =0 or state = 2)  ";
    else
        cond = cond + " and (state=3 or state=4 or state=5)  ";
    if(txtMerge->ItemIndex == 0 )
        cond = cond + "  ";
    else if(txtMerge->ItemIndex == 1 )
        cond = cond = cond + " and mergestate=1  ";
    else
        cond = cond = cond + " and mergestate!=1  ";

    String str = "select fname,exchno,accno,vchno, vchtype,amount,state,mergestate,sid,pkgno from vouchers ";
    str = str +cond;

	debugout("str = %s",str);

    int ret = FmIC->gdb.Query(str.c_str());
    if(ret)
    {
         MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
         debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
         return ret;
    }

   txtDetail->Items->BeginUpdate();
   txtDetail->Clear();
   TListItem* li;
   MyRes res =  FmIC->gdb.Store();
   MyRow row = res.Fetch();

   while(row!=NULL)
   {
       li = txtDetail->Items->Add();
       li->Caption = row[8];

  ////20150511     li->SubItems->Insert(0,row[1]);
       li->SubItems->Insert(0,row[9]);
       
       li->SubItems->Insert(1,row[2]);
       li->SubItems->Insert(2,row[3]);
       li->SubItems->Insert(3,row[4]);
       li->SubItems->Insert(4,row[5]);

       if(atoi(row[6]) == 0 )
            li->SubItems->Insert(5,"未识别");
       else  if(atoi(row[6]) == 1 )
            li->SubItems->Insert(5,"已识别");
       else  if(atoi(row[6]) == 3 )
            li->SubItems->Insert(5,"处理失败");
       else  if(atoi(row[6]) == 4 )
            li->SubItems->Insert(5,"校验失败");
       else  if(atoi(row[6]) == 5 )
            li->SubItems->Insert(5,"识别失败");
       else
            li->SubItems->Insert(5,"识别中");

       if(atoi(row[7]) == 1 )
            li->SubItems->Insert(6,"已匹配");
       else if(atoi(row[7]) == 3 )
            li->SubItems->Insert(6,"匹配失败");
       else
            li->SubItems->Insert(6,"未匹配");

       li->Data = row[8];
       row = res.Fetch();
    }
    txtDetail->Items->EndUpdate();
    str = "select count(*),sum(amount) from vouchers ";

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

   StatusBar->Panels->Items[0]->Text = String("交换号[")+txtNetno->Text.c_str()+"]";
   StatusBar->Panels->Items[1]->Text = String("总笔数[")+row[0]+"]";
   StatusBar->Panels->Items[2]->Text = String("总金额[")+row[1]+"]";

   
   return 0;
}
void __fastcall TFmVoucher::btnRefreshClick(TObject *Sender)
{
    flushData();
}
//---------------------------------------------------------------------------
void __fastcall TFmVoucher::txtDetailDblClick(TObject *Sender)
{
    TListItem* li;
    li= txtDetail->Selected;
    if(li==NULL) return ;

    String sid = li->Caption.c_str();

    int curid = txtDetail->ItemIndex;

    debugout("select  ip,rotate,sendstate,accno,vchno,amount,fname,right(pkgno,5),vchtype,date,session,bname,mergestate,state,exchno,modeflag,area from vouchers where sid=%s",sid.c_str());

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,sendstate,accno,vchno,amount,fname,right(pkgno,5),vchtype,date,session,bname,mergestate,state,exchno,modeflag,area from vouchers where sid=%s",sid.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
        return;
    }
    MyRes res  =  FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return ;

    if (strcmp(g_sys_area.c_str(),row[16]) != 0)
    {
		MessageBox(Handle,"非当前交换区域影像!\t","提示",MB_OK|MB_ICONINFORMATION);
		return;
    }

    char ip[24];
    char accno[32];
    char vchno[30];
    char amount[16];
    char fname[80];
    char bname[80];
    char vchtype[8];
    char pkgno[12];
    int  mergestate;
    int  sendstate;
    int  rotate;
    int  state;
    int  modeflag;

    memset(ip,0,sizeof(ip));
    memset(accno,0,sizeof(accno));
    memset(vchno,0,sizeof(vchno));
    memset(amount,0,sizeof(amount));
    memset(pkgno,0,sizeof(pkgno));
    memset(fname,0,sizeof(fname));
    memset(bname,0,sizeof(bname));

    strcpy(ip,row[0]);
    rotate = atoi(row[1]);
    sendstate = atoi(row[2]);
    strncpy(accno,row[3],sizeof(accno)-1);
    strncpy(vchno,row[4],sizeof(vchno)-1);
    strncpy(amount,row[5],sizeof(amount)-1);
    strcpy(fname,row[6]);
    strcpy(pkgno,row[7]);
    strncpy(vchtype,row[8],sizeof(vchtype)-1);
    strcpy(bname,row[11]);
    mergestate  = atoi(row[12]);
    state  = atoi(row[13]);
    modeflag = atoi(row[15]);

    if( strlen(amount)<=0) strcpy(amount,"0");

    if(sendstate==1 || state == 0 || (state==1 && mergestate==1))   FormImage->btnSave->Visible = false;
    else               FormImage->btnSave->Visible = true;

    FormImage->txtAccno->Text  = accno;
    FormImage->txtVchno->Text  = vchno;
    FormImage->txtAmount->Text = amount;
    FormImage->txtVchtype->Text = vchtype;
    FormImage->chkMode->Checked = (modeflag==1);

    FormImage->exchno = row[14];


	FormImage->curID =  sid.ToInt();

	debugout("CurID = %d",FormImage->curID);

    char tname[128];
    memset(tname,0,sizeof(tname));
    sprintf(tname,"%s//%02d//%05d//%s",row[9],atoi(row[10]),atoi(row[7]),fname);

    MemT image;

    int sizex;
    int sizey;
    int panel;


	debugout("tname = %s",tname);
    ret =  ti_getimage( tname, ip, rotate,   image, sizex, sizey,panel );
    if(ret)
    {
        MessageBox(Handle,"获取图像错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
	debugout("ip = %s,sizex=%d,sizey=%d",ip,sizex,sizey);

	tilSaveImage(image.data(),sizex,sizey,panel,"vdics.jpg");

	 FormImage->image->Picture->LoadFromFile("vdics.jpg");

	 DeleteFile("vdics.jpg");

/*
    unsigned char * bmp;
    int size;

    bmp =ti_RGB2BMP(image.data(),sizex,sizey,0,0,0,0,sizex,sizey,size);

	debugout("size=%d",size);

    TMemoryStream* pms = new TMemoryStream();
    pms->Clear();
    pms->SetSize(size);
    ret = pms->Write(bmp,size);
    pms->Position = 0;
    FormImage->image->Picture->Bitmap->LoadFromStream(pms);
    pms->Clear();

    FormImage->curID =  sid.ToInt();

    ti_RGB2BMPFREE(bmp);

*/
    ret = FormImage->ShowModal();

    if((txtIdentify->ItemIndex == 3 || txtIdentify->ItemIndex == 1)  )
        flushData();
    else
    {
        li->SubItems->Strings[1] =  FormImage->txtAccno->Text.c_str();
        li->SubItems->Strings[2] =  FormImage->txtVchno->Text.c_str();
        li->SubItems->Strings[3] =  FormImage->txtVchtype->Text.c_str();
        li->SubItems->Strings[4] =  RemoveThousandChar(FormImage->txtAmount->Text.c_str());
        
    }
        

    if(curid > txtDetail->Items->Count)
        curid = 0;

    if( txtDetail->Items->Count>curid ) txtDetail->Items->Item[curid]->Selected = true;


}
//---------------------------------------------------------------------------
void __fastcall TFmVoucher::txtIdentifyCloseUp(TObject *Sender)
{
    flushData();
}
//---------------------------------------------------------------------------
void __fastcall TFmVoucher::txtMergeCloseUp(TObject *Sender)
{
     flushData();
}
//---------------------------------------------------------------------------


int TFmVoucher::mergeOne(int vid,const char* accno, const char* vchno, const char*vchtype,const char*amount,const char* netno,const char*fname,const char*bname,const char*ip,int rotate,const char*dt,const char*sess)
{
    //TODO: Add your source code here
    String saccno = accno;
    String svchno = vchno;

    MyRes res;
    MyRow row;
    int sid;
    if(saccno.Length()>10)
        saccno = saccno.c_str()+saccno.Length()-10;
    if(svchno.Length()>6)
        svchno = svchno.c_str()+svchno.Length()-6;
    //自动匹配
    int ret;

    if(FmIC->gmode !=2 )
        ret = FmIC->gdb.VQuery(512,"select sid,mergestate from pbcdata where exchno='%s' and  mergestate=0  and vchtype='%s'  and right(vchno,6)='%s' and right(accno,10)='%s' and amount='%s' and area='%s' and date='%s' and session='%s' limit 1",netno,vchtype,svchno.c_str(),saccno.c_str(),amount,g_sys_area.c_str(),dt,sess);
    else
        ret = FmIC->gdb.VQuery(512,"select sid,mergestate from dists where mergestate=0  and isenv=0 and exchno='%s' and vchtype='%s'  and right(vchno,6)='%s' and right(accno,10)='%s' and amount='%s' and area='%s' and date='%s' and session='%s' limit 1",netno,vchtype,svchno.c_str(),saccno.c_str(),amount,g_sys_area.c_str(),dt,sess);
    if(ret)
    {
        MessageBox(Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    res = FmIC->gdb.Store();
    if(res.Count()!=1 )    //结果没有
    {
        if( strcmp(g_sys_area.c_str(),BRNO_BJ)== 0  && atoi(vchtype)>=50)
        { 
            ret = FmIC->gdb.VQuery(512,"select sid,mergestate from dists where mergestate=0  and isenv=0 and exchno='%s' and vchtype='%s'  and amount='%s' and area='%s' and date='%s' and session='%s' limit 1",netno,vchtype,amount,g_sys_area.c_str(),dt,sess);
            if(ret)
            {
                MessageBox(Handle,"查询数据库错误!\t","提示",MB_OK|MB_ICONINFORMATION);
                return 0;
            }
            res = FmIC->gdb.Store();
            if(res.Count()!=1 )    return 0;
        }
        else return 0;
     }
     row = res.Fetch();
     if(row!=NULL)
     {
         sid = atoi(row[0]);

         FmIC->gdb.VQuery(512,"update vouchers  set mergestate=1,mergeid=%d where sid=%d",sid,vid);

         if(FmIC->gmode !=2 )
         {
             FmIC->gdb.VQuery(512,"update pbcdata  set mergestate=1,mergeid=%d where sid=%d",vid,sid);
         }
         else
         {
             FmIC->gdb.VQuery(512,"update dists  set mergestate=1,mergeid=%d where sid=%d",vid,sid);
         }
     }

     return 0;
}

int TFmVoucher::mergeAll()
{
    //TODO: Add your source code here
    int ret = FmIC->gdb.VQuery(512,"select accno,vchno,amount,exchno,fname,bname,ip,rotate,vchtype ,sid ,date,session from vouchers where mergestate=0 and state=1 and area='%s' and date='%s' and session='%s'",g_sys_area.c_str(),txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
     }
     Screen->Cursor = crHourGlass;
     MyRes res = FmIC->gdb.Store();
     MyRow row = res.Fetch();
     while(row!=NULL)
     {
        mergeOne(atoi(row[9]),row[0],row[1],row[8],row[2],row[3],row[4],row[5],row[6],atoi(row[7]),row[10],row[11]);
        row = res.Fetch();
     }
     flushData();
     Screen->Cursor = crDefault;
     return 0;
}
void __fastcall TFmVoucher::btnMergeClick(TObject *Sender)
{
    mergeAll();    
}
//---------------------------------------------------------------------------
void __fastcall TFmVoucher::E1Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------


void __fastcall TFmVoucher::M1Click(TObject *Sender)
{
    mergeAll();        
}
//---------------------------------------------------------------------------


void __fastcall TFmVoucher::R1Click(TObject *Sender)
{
    flushData();    
}
//---------------------------------------------------------------------------


void __fastcall TFmVoucher::MeForceLookClick(TObject *Sender)
{
    TListItem* li;
    li = txtDetail->Selected ;
    if(li == NULL ) return ;
    String sid = li->Caption;

    int curid = txtDetail->ItemIndex;

    String mergeid;

    int ret = FmIC->gdb.VQuery(512,"select sendstate,mergeid  from vouchers where sid=%d",sid.ToInt());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error [%s]\n",mysql_error( FmIC->gdb.Handle()));
        return;
    }
    MyRes res  =  FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return ;

    int st = atoi(row[0]) ;

    mergeid = row[1];
    if(st==1)
    {
        MessageBox(Handle,"数据已上送,不能删除!\t","提示",MB_OK|MB_ICONINFORMATION);
        return ;
    }
    if(MessageBox(Handle,"是否要删除数据?\t","提示",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO ) return ;
    ret =  FmIC->gdb.VQuery(512,"delete from vouchers where  sid=%d",sid.ToInt());
    ret =  FmIC->gdb.VQuery(512,"update dists set mergestate=0,mergeid=-1 where  sid=%d",mergeid.ToInt());
    li->Delete();

    WriteToLogFile(g_sys_opt.c_str(),"删除票据",sid.ToInt(),"");
    if(curid > txtDetail->Items->Count)
        curid = 0;

    if( txtDetail->Items->Count>curid ) txtDetail->Items->Item[curid]->Selected = true;
}
//---------------------------------------------------------------------------


void __fastcall TFmVoucher::txtDetailColumnClick(TObject *Sender,
      TListColumn *Column)
{
    int i,m,n,j;
    static bool od=true;

    Screen->Cursor = crHourGlass;
    txtDetail->Items->BeginUpdate();

    od=!od;
    n = Column->Index;
    m = txtDetail->Items->Count;
    txtDetail->Items->Add();
    if(n==3 || n==4 || n==5 || n==2)
    {
        for(i=0;i< m-1;i++)
            for(j=i+1;j< m;j++)

            if(od)
            {
                if(sg(txtDetail->Items->Item[i]->SubItems->Text,n-1) >
                   sg(txtDetail->Items->Item[j]->SubItems->Text,n-1))
                {
                    setitem(m,i,j);
                } 
            }
            else
            {
                if(sg(txtDetail->Items->Item[i]->SubItems->Text,n-1) <
                   sg(txtDetail->Items->Item[j]->SubItems->Text,n-1))
                {
                    setitem(m,i,j);
                }
            }
    }

    Screen->Cursor = crDefault;
    txtDetail->Items->Delete(m);
    txtDetail->Items->EndUpdate();
    return;
}
//---------------------------------------------------------------------------


int TFmVoucher::setitem(int m, int i, int j)
{
    //TODO: Add your source code here
    txtDetail->Items->Item[m]=
    txtDetail->Items->Item[i];

    txtDetail->Items->Item[i]=
    txtDetail->Items->Item[j];

    txtDetail->Items->Item[j]=
    txtDetail->Items->Item[m];

    return 0;
}

int TFmVoucher::setimg(int a, int b)
{
    //TODO: Add your source code here
    txtDetail->Items->Item[a]->SubItemImages[0] = txtDetail->Items->Item[b]->SubItemImages[0];
    txtDetail->Items->Item[a]->SubItemImages[1] = txtDetail->Items->Item[b]->SubItemImages[1];
    txtDetail->Items->Item[a]->SubItemImages[2] = txtDetail->Items->Item[b]->SubItemImages[2];
    txtDetail->Items->Item[a]->SubItemImages[5] = txtDetail->Items->Item[b]->SubItemImages[5];
    txtDetail->Items->Item[a]->SubItemImages[7] = txtDetail->Items->Item[b]->SubItemImages[7];
    return 0;
}

AnsiString TFmVoucher::sg(AnsiString str, int n)
{
    //TODO: Add your source code here
     int l,i;
    AnsiString qq(str);
    l=qq.Length();
    AnsiString p(str);
    char a[]="\r\n";
    int j=0,k=0;

    for(j=1;j< l;j++)
    {
        i=1;
        while(qq[j]!=a[0] && qq[j+1]!=a[1])
        {
            p[i++]=qq[j];
            j++;
        }
        k++;
        if((k-1)==n)
        break;
    }
    p[i]=NULL;

    return (p);
}
void __fastcall TFmVoucher::FindDlgFind(TObject *Sender)
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


int TFmVoucher::FindResult(String StrToFind, TListItem* pItem, bool bExact)
{
    //TODO: Add your source code here
     //按帐号进行查找
    String sub_str = pItem->SubItems->Strings[1];
    bool found = (sub_str.Pos(StrToFind)>0);

    if( found ) return true;

    //按金额进行查找
    sub_str = pItem->SubItems->Strings[4];
    found = (sub_str.Pos(StrToFind)>0);

    if( found ) return true;

    //按凭证号行查找
    sub_str = pItem->SubItems->Strings[2];
    found = (sub_str.Pos(StrToFind)>0);

    if( found ) return true;

    //按序号进行查找
    sub_str = pItem->Caption;
    return (sub_str.Pos(StrToFind)>0);
}
void __fastcall TFmVoucher::F1Click(TObject *Sender)
{
     FindDlg->Execute();    
}
//---------------------------------------------------------------------------


int TFmVoucher::flushExchno()
{
    //TODO: Add your source code here
     txtNetno->Clear();

    int ret = FmIC->gdb.VQuery(512,"select distinct pkgno from vouchers  where date='%s' and session='%s' order by pkgno ",txtDate->Text.c_str(),txtSession->Text.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        return 0;
    }

    MyRes res =  FmIC->gdb.Store();
    MyRow row = res.Fetch();

    txtNetno->Items->Add("全部");
    while(row!=NULL)
    {
        txtNetno->Items->Add(row[0]);
        row = res.Fetch();
    }

    txtNetno->ItemIndex = 0 ;
}
void __fastcall TFmVoucher::txtNetnoDropDown(TObject *Sender)
{
    flushExchno();    
}
//---------------------------------------------------------------------------

void __fastcall TFmVoucher::txtNetnoChange(TObject *Sender)
{
    flushData();      
}
//---------------------------------------------------------------------------

int TFmVoucher::getNext()
{
    //TODO: Add your source code here
    TListItem* li;
    if(txtDetail->Items->Count>0)
    {
        li= txtDetail->Items->Item[0];
        li->Delete();
    }
    if(txtDetail->Items->Count==0)
    {
         MessageBox(Handle,"记录处理完毕\t","提示",MB_OK|MB_ICONINFORMATION);
        return -1;
    }
    li= txtDetail->Items->Item[0];
    li->Selected = true;

    String sid = li->Caption.c_str();

	debugout("sid =%s",sid.c_str());

    int ret = FmIC->gdb.VQuery(512,"select  ip,rotate,sendstate,accno,vchno,amount,fname,right(pkgno,5),vchtype,date,session,bname,mergestate,state,exchno,modeflag  from vouchers where sid=%s",sid.c_str());
    if(ret)
    {
        MessageBox(Handle,"查询出错!\t","提示",MB_OK|MB_ICONINFORMATION);
        debugout("sql error[%s]\n",mysql_error( FmIC->gdb.Handle()));
        return ret;
    }
    MyRes res  =  FmIC->gdb.Store();
    MyRow row  = res.Fetch();

    if(row==NULL) return 0;

    char ip[24];
    char accno[32];
    char vchno[30];
    char amount[16];
    char fname[80];
    char bname[80];
    char vchtype[8];
    char pkgno[12];
    int  mergestate;
    int  sendstate;
    int  state;
    int  rotate;
    int  modeflag;

    memset(ip,0,sizeof(ip));
    memset(accno,0,sizeof(accno));
    memset(vchno,0,sizeof(vchno));
    memset(amount,0,sizeof(amount));
    memset(pkgno,0,sizeof(pkgno));
    memset(fname,0,sizeof(fname));
    memset(bname,0,sizeof(bname));

    strcpy(ip,row[0]);
    rotate = atoi(row[1]);
    sendstate = atoi(row[2]);
    strcpy(accno,row[3]);
    strcpy(vchno,row[4]);
    strcpy(amount,row[5]);
    strcpy(fname,row[6]);
    strcpy(pkgno,row[7]);
    strcpy(vchtype,row[8]);
    strcpy(bname,row[11]);

    mergestate  = atoi(row[12]);
    state  = atoi(row[13]);

    modeflag = atoi(row[15]);

    if( strlen(amount)<=0) strcpy(amount,"0");       
    if(sendstate==1 || state == 0 || (state==1 && mergestate==1))   FormImage->btnSave->Visible = false;

    
    else               FormImage->btnSave->Visible = true;
    FormImage->txtAccno->Text  = accno;
    FormImage->txtVchno->Text  = vchno;
    FormImage->txtAmount->Text = amount;
    FormImage->txtVchtype->Text = vchtype;

    FormImage->chkMode->Checked = (modeflag==1);

    
    FormImage->txtAccno->SetFocus();

    FormImage->exchno = row[14];
     
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
        return -1;
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

    FormImage->curID =  sid.ToInt();

	debugout("CurID = %d",FormImage->curID);
    pms->Clear();
    ti_RGB2BMPFREE(bmp);
    return 0;
}



void __fastcall TFmVoucher::txtDetailSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
    TListItem* li;
    li= txtDetail->Selected;
    if(li==NULL) return ;

    String sid = li->Caption.c_str();
    _curPos = sid.ToInt();    
}
//---------------------------------------------------------------------------

void __fastcall TFmVoucher::UpDown1Click(TObject *Sender,
      TUDBtnType Button)
{
        flushData();
}
//---------------------------------------------------------------------------

