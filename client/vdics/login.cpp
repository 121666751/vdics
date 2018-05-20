/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <bcbext.h>
#include <zcommon.h>

#include "main.h"
#include "login.h"
#include "Password.h"
#include "tinystr.h"
#include "tinyxml.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

String  g_sys_opt;

String  g_sys_area;

int     g_sys_priv;

String  g_sys_name;

String  g_sys_org;

TFormLogin *FormLogin;
//---------------------------------------------------------------------------
__fastcall TFormLogin::TFormLogin(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLogin::FormShow(TObject *Sender)
{
   // SetCtrlStyle(txtClkno,ES_NUMBER); 
    int ret = Login();   
}
//---------------------------------------------------------------------------

/*
void __fastcall TFormLogin::btnOKClick(TObject *Sender)
{     
		MessageBox(Handle,txtClkno->Text.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
		MessageBox(Handle,txtPassword->Text.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
    String cmd =  AnsiString("select state,authority,area,name,loginstate from clerks where clkno='")+txtClkno->Text.c_str()+"' and password = PASSWORD('"+txtPassword->Text.c_str()+"')";
		//String cmd =  AnsiString("select state,authority,area,name,loginstate from clerks where clkno='")+name.c_str()+"' and password = PASSWORD('"+Password.c_str()+"')";
    int ret = FmIC->gdb.Query(cmd.c_str());
    if(ret)
    {
          MessageBox(Handle,"���ݿ��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
          ModalResult = mrNone;
          return ;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    if(row==NULL)
    {
        MessageBox(Handle,"��Ա�������!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        txtClkno->Text = "";
        txtPassword->Text = "";
        txtClkno->SetFocus();
        ModalResult = mrNone;
        return;
    }
    else
    {
        int state = atoi(row[0]);
        if(state!=1)
        {
            MessageBox(Handle,"��Ա״̬����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
            ModalResult = mrNone;
            return;
        }
    }

    int logst = atoi(row[4]);
    if(logst == 1)
    {
        if(MessageBox(Handle,"��Ա�ѵ�¼,�Ƿ����?\t","��ʾ",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO)
        {
             ModalResult = mrNone;
            return;
        }
    }

    g_sys_opt   = txtClkno->Text.c_str();
    g_sys_priv  = atoi(row[1]);
    g_sys_area  = row[2];
    g_sys_name  = row[3];

    if( g_sys_priv&PRIV_MOD_PWD)
    {
        if( FormPWD->ShowModal()!=IDOK)
        {
            Application->Terminate();
        }
    }

    cmd =  AnsiString("update clerks set loginstate=1,logintime=now() where clkno='")+txtClkno->Text.c_str()+"'";  
    ret = FmIC->gdb.Query(cmd.c_str());
    if(ret)
    {
          MessageBox(Handle,"���ݿ��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
          ModalResult = mrNone;
          return ;
    }
	
}
*/
//---------------------------------------------------------------------------

void __fastcall TFormLogin::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{

    if(Key == 13)
        PostMessage(Handle,WM_NEXTDLGCTL,0,0);
}
//---------------------------------------------------------------------------
int TFormLogin::Login()
{

	String name;
	String Password;

	TiXmlElement* root, *ele;
	TiXmlDocument* doc;
	doc = new TiXmlDocument();

	if (doc == NULL)
	{
		return -1;
	}

	doc->LoadFile("login.xml");

	if(doc->Error())
	{
		delete doc;
		return -1;
	}
	///////////////////////////////////////////////////////////////////

	if (!doc || !(root = ((TiXmlDocument*)doc)->RootElement()))
	{
		OutputDebugString("���ڵ����  ����");
		delete doc;
		return -1;
	}

	ele = root->FirstChildElement();

	if (!ele)
	{
		OutputDebugString("�ӽڵ㲻����");
		delete doc;
		return -1;
	}

	char* pname, *pvalue;

	while(ele)
	{
		pname = (char *)ele->Value();
		pvalue = (char *)ele->GetText();

		debugout("[%s] [%s]",pname,pvalue);

		if (lstrcmpi(pname,"clkno") == 0)
		{

			if(pvalue)
			{
				name.printf(pvalue);
			}
			else
			{
				MessageBox(Handle,"��¼ʧ��,û�е�¼��Ϣ!\n","��ʾ",MB_OK|MB_ICONINFORMATION);
				return -1;
			}
		}
		else if(lstrcmpi(pname,"password") == 0)
		{

			if(pvalue)
			{
				Password.printf(pvalue);
			}
		}

		ele = ele->NextSiblingElement();
	}


	//
	// char str[1024];

 //  FILE	*fp = fopen("login.cfg", "r");
 //  


 //  if(fp)
	//{
	//	
	//		memset(str, 0, strlen(str));
	//		fgets(str, sizeof(str), fp);
	//		
	//			if(strlen(str) == 0)
	//			{
	//				MessageBox(Handle,"��¼ʧ��,û�е�¼��Ϣ!\n","��ʾ",MB_OK|MB_ICONINFORMATION);
	//				return	-1;
	//			}
	//	
	//			name.printf("55555");
 //     
 //     memset(str, 0, strlen(str));
	//		fgets(str, sizeof(str), fp);
	//		
	//		
	//		if(strlen(str) != 0)
	//		{
	//			Password.printf("%s",str);
	//		}

	//	fclose(fp);
	//}
	//else
	//{

 //   MessageBox(Handle,"��¼ʧ��,û�е�¼��Ϣ!\n","��ʾ",MB_OK|MB_ICONINFORMATION);
	//	return	0;
	//}
	
	String cmd =  AnsiString("select state,authority,area,name,loginstate from clerks where clkno='")+name.c_str()+"' and password = PASSWORD('"+Password.c_str()+"')";
		
    int ret = FmIC->gdb.Query(cmd.c_str());
    if(ret)
    {
          MessageBox(Handle,"���ݿ��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
          ModalResult = mrNone;
          return 0;
    }

    MyRes res = FmIC->gdb.Store();
    MyRow row = res.Fetch();

    if(row==NULL)
    {
        MessageBox(Handle,"��Ա�������!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
       /* txtClkno->Text = "";
        txtPassword->Text = "";
        txtClkno->SetFocus();*/
        ModalResult = mrNone;
        return 0;
    }
    else
    {
        int state = atoi(row[0]);
        if(state!=1)
        {
            MessageBox(Handle,"��Ա״̬����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
            ModalResult = mrNone;
            return 0;
        }
    }

    int logst = atoi(row[4]);
    if(logst == 1)
    {
        if(MessageBox(Handle,"��Ա�ѵ�¼,�Ƿ����?\t","��ʾ",MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDNO)
        {
             ModalResult = mrNone;
            return 0;
        }
    }

    g_sys_opt   = name;
    g_sys_priv  = atoi(row[1]);
    g_sys_area  = row[2];
    g_sys_name  = row[3];

    /*if( g_sys_priv&PRIV_MOD_PWD)
    {
        if( FormPWD->ShowModal()!=IDOK)
        {
            Application->Terminate();
        }
    }*/

    cmd =  AnsiString("update clerks set loginstate=1,logintime=now() where clkno='")+name.c_str()+"'";  
    ret = FmIC->gdb.Query(cmd.c_str());
    if(ret)
    {
          MessageBox(Handle,"���ݿ��ѯ����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
          ModalResult = mrNone;
          return 0;
    }
  
}

