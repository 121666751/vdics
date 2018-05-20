//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include <znetdef.h>

String HOSTS_FILE;//  "C:\\WINNT\\system32\\drivers\\etc\\Hosts"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmSethosts *FmSethosts;
//---------------------------------------------------------------------------
__fastcall TFmSethosts::TFmSethosts(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TFmSethosts::FormDestroy(TObject *Sender)
{
    delete p;
}
//---------------------------------------------------------------------------

/**
    设置host文件
*/
int TFmSethosts::LoadHosts()
{
    //TODO: Add your source code here
    char sysdir[MAX_PATH+80];
    int len = GetSystemDirectory(sysdir,MAX_PATH+80);
    HOSTS_FILE = sysdir;
    HOSTS_FILE += "\\drivers\\etc\\Hosts";

    lstHost->Lines->Clear();

    delete p;
    p = 0;
    p = new TStringList;

    //从文件中获取内容
    p->LoadFromFile(HOSTS_FILE);

    for( int i = 0 ; i < p->Count ; i++ )
    {
        String stemp = p->Strings[i];
        lstHost->Lines->Add( p->Strings[i] );
        if( !stemp.IsEmpty() && stemp[1] != '#' )
        {
            int itr = stemp.Pos(" ");
            if( itr == 0)
            {
                itr = stemp.Pos("\t");
            }
            if( itr != 0)
            {
                String szName = (stemp.SubString( itr+1, stemp.Length()-itr )).Trim();

                if( szName == "MESSAGE_SERVER" )
                {
                    message_server->Text = (stemp.SubString( 1, itr-1 )).Trim();
                }

                if( szName == "ZTIC_DB_HOST" )
                {
                    ztic_db_server->Text = (stemp.SubString( 1, itr-1 )).Trim();
                }   

                if( szName == "ZTIC_DB_HOST"  ||
                    szName == "MESSAGE_SERVER"   )
                {
                    p->Delete(i);
                    i--;
                }
            }
        }
    }

    btnSaveHosts->Enabled = false;
    btnRefresh->Enabled = false;
    ztic_db_server->SetFocus();
    return 0;
}

/**
    控件控制,如果数据有变化,则允许保存
*/
void __fastcall TFmSethosts::local_db_hostChange(TObject *Sender)
{
    btnSaveHosts->Enabled = true;
    btnRefresh->Enabled = true;
}
//---------------------------------------------------------------------------

/**
    控件控制,如果数据有变化,则允许保存
*/
void __fastcall TFmSethosts::btnRefreshClick(TObject *Sender)
{
    LoadHosts();
}
//---------------------------------------------------------------------------

/**
    保存host文件
*/
void __fastcall TFmSethosts::btnSaveHostsClick(TObject *Sender)
{
    String tmp;

    if( !message_server->Text.IsEmpty() )
    {
        tmp.sprintf( "%s\t%s", message_server->Text, "MESSAGE_SERVER" );
        p->Add( tmp );
    }

    if( !ztic_db_server->Text.IsEmpty() )
    {
        tmp.sprintf( "%s\t%s", ztic_db_server->Text, "ZTIC_DB_HOST" );
        p->Add( tmp );
    }
    p->SaveToFile( HOSTS_FILE );

    LoadHosts();

    btnExit->SetFocus();
}
//---------------------------------------------------------------------------

/**
    退出
*/
void __fastcall TFmSethosts::btnExitClick(TObject *Sender)
{
    if( GetFocus() == btnExit->Handle )
    {
        Close();
        return;
    }
    PostMessage( Handle, WM_NEXTDLGCTL, 0 , 0);
}
//---------------------------------------------------------------------------


/**
    主窗口显示
*/
void __fastcall TFmSethosts::FormShow(TObject *Sender)
{
    LoadHosts();
}
//---------------------------------------------------------------------------

/**
    快捷键控制
*/
void __fastcall TFmSethosts::FormKeyPress(TObject *Sender, char &Key)
{
    if( Key == 27 )
        Close();
}
//---------------------------------------------------------------------------

