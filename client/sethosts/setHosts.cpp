/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 北京天创征腾信息科技有限公司    版权所有
 *
 * 此文件为天创征腾软件产品的一部分。
 * 天创征腾软件产品的所有部分,包括但不限于软件产品中所含的任何图象、照片、
 * 动画、文字和附加程序（applets）、加密卡、随附的印刷材料、源代码文件及
 * 本软件产品的任何副本的一切所有权和知识产权，均归版权人天创征腾拥有。
 *
 * 文件：	$Id: setHosts.cpp,v 1.2 2008/03/30 10:40:36 Nie Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("setHosts.res");
USEFORM("main.cpp", FmSethosts);
BOOL am_i_administrator( void );
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
 /*   char usr_name[40];
    unsigned long sz = (unsigned long)sizeof( usr_name );
    GetUserName( usr_name, &sz);
    String szUsr_name = usr_name;
    if( szUsr_name.AnsiCompareIC("Administrator") != 0 )
    {
        ShowMessage( "运行本程序请使用管理员(Administrator)身份登录操作系统！" );
        Application->Terminate();
        return 0;
    }
     */
    if( !am_i_administrator())
    {
        ShowMessage( "运行本程序请使用管理员(Administrator)身份登录操作系统！" );
        Application->Terminate();
        return 0;
    }
    try
    {
         Application->Initialize();
         Application->Title = "-飓风-网络参数设置";
         Application->CreateForm(__classid(TFmSethosts), &FmSethosts);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
BOOL am_i_administrator( void )
{
   // Plagerized from Microsoft Knowledge Base article Q118626

   HANDLE access_token_handle = NULL;

   BYTE buffer[ 1024 ];

   PTOKEN_GROUPS token_groups_p = (PTOKEN_GROUPS) buffer;

   DWORD buffer_size = 0;

   BOOL success = FALSE;

   if ( OpenThreadToken( GetCurrentThread(),
                         TOKEN_QUERY,
                         TRUE,
                        &access_token_handle ) == FALSE )
   {
      if ( GetLastError() != ERROR_NO_TOKEN )
      {
         return( FALSE );
      }

      // retry against process token if no thread token exists

      if ( OpenProcessToken( GetCurrentProcess(),
                             TOKEN_QUERY,
                            &access_token_handle ) == FALSE )
      {
         return( FALSE );
      }
   }

   success = GetTokenInformation( access_token_handle,
                                  TokenGroups,
                                  buffer,
                                  1024,
                                 &buffer_size );

   CloseHandle( access_token_handle );
   access_token_handle = NULL;

   if ( success == FALSE )
   {
      return( FALSE );
   }

   SID_IDENTIFIER_AUTHORITY nt_authority_sid = SECURITY_NT_AUTHORITY;

   PSID administrators_sid_p = NULL;

   if ( AllocateAndInitializeSid( &nt_authority_sid,
                                   2,
                                   SECURITY_BUILTIN_DOMAIN_RID,
                                   DOMAIN_ALIAS_RID_ADMINS,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                  &administrators_sid_p ) == FALSE )
   {
      return( FALSE );
   }

   // assume that we don't find the admin SID.
   success = FALSE;

   UINT x = 0;

   for( x = 0; x < token_groups_p->GroupCount; x++ )
   {
      if ( EqualSid( administrators_sid_p, token_groups_p->Groups[ x ].Sid ) != FALSE )
      {
         success = TRUE;
         break;
      }
   }

   FreeSid( administrators_sid_p );

   return( success );
}
