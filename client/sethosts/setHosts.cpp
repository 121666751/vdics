/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 �����촴������Ϣ�Ƽ����޹�˾    ��Ȩ����
 *
 * ���ļ�Ϊ�촴���������Ʒ��һ���֡�
 * �촴���������Ʒ�����в���,�����������������Ʒ���������κ�ͼ����Ƭ��
 * ���������ֺ͸��ӳ���applets�������ܿ����渽��ӡˢ���ϡ�Դ�����ļ���
 * �������Ʒ���κθ�����һ������Ȩ��֪ʶ��Ȩ�������Ȩ���촴����ӵ�С�
 *
 * �ļ���	$Id: setHosts.cpp,v 1.2 2008/03/30 10:40:36 Nie Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
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
        ShowMessage( "���б�������ʹ�ù���Ա(Administrator)��ݵ�¼����ϵͳ��" );
        Application->Terminate();
        return 0;
    }
     */
    if( !am_i_administrator())
    {
        ShowMessage( "���б�������ʹ�ù���Ա(Administrator)��ݵ�¼����ϵͳ��" );
        Application->Terminate();
        return 0;
    }
    try
    {
         Application->Initialize();
         Application->Title = "-쫷�-�����������";
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
