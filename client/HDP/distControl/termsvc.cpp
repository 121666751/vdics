#include "stdafx.h"
#include <zcommon.h>
#include <wtsapi32.h>
#include <ipexport.h>
#include <iphlpapi.h>
#include <string>

using std::string;

typedef BOOL (WINAPI *pWTSQuerySessionInformation)(HANDLE,DWORD,WTS_INFO_CLASS,LPTSTR *,DWORD *);

pWTSQuerySessionInformation getip;

BOOL IsTerminalServicesEnabled( VOID );

string getUniqeFilename(){
	UUID uuid;
	UuidCreate(&uuid);
	char* uuidstr;
	UuidToString(&uuid,(BYTE**)&uuidstr);
	string uid = uuidstr;
	RpcStringFree((BYTE**)uuidstr);
	return uid;
}

string PrintMACaddress(unsigned char MACData[])
{
	char s[13];
	sprintf(s,"%02hx%02hx%02hx%02hx%02hx%02hx",
		MACData[0],
		MACData[1],
		MACData[2],
		MACData[3],
		MACData[4],
		MACData[5]);

	return s;
}


string GetMACaddress(void)
{
	/*	unsigned char MACData[6];

	UUID uuid;
	UuidCreateSequential( &uuid );    // Ask OS to create UUID

	for (int i=2; i<8; i++)  // Bytes 2 through 7 inclusive
	// are MAC address
	MACData[i - 2] = uuid.Data4[i];

	char s[13];
	sprintf(s,"%02hx%02hx%02hx%02hx%02hx%02hx",
	MACData[0],
	MACData[1],
	MACData[2],
	MACData[3],
	MACData[4],
	MACData[5]
	);
	return s;
	*/
	IP_ADAPTER_INFO AdapterInfo[16]; // 定义网卡信息存贮区。
	DWORD dwBufLen = sizeof(AdapterInfo); 

	DWORD dwStatus = GetAdaptersInfo( 
		AdapterInfo, // [output] 指向接收数据缓冲指针
		&dwBufLen); // [input] 缓冲区大小
	if(dwStatus != ERROR_SUCCESS) // 此处是个trap，用来保证返回值有效
	{
		return "";
	}

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; 
	string mac;
	do 
	{
			mac = PrintMACaddress(pAdapterInfo->Address); // 打印MAC地址
			debugout("get mac = [%s]\n",mac.c_str());
			pAdapterInfo = pAdapterInfo->Next; 
	}while(pAdapterInfo); 
	
	return mac;
	
}


//extern "C"
bool is_terminal_mode()
{
	return GetSystemMetrics( SM_REMOTESESSION ) != 0;
}

void DetectIP(char* buf)
{
	if(is_terminal_mode())
	{
		DWORD BytesReturned;
		struct sockaddr_in SockAddr;
		struct in_addr ipadd;
		char *clientaddr;

		HMODULE dll = LoadLibrary("wtsapi32.dll");
		if(!dll) {
			OutputDebugString("cannot load wtsapi32.dll!");
			return;
		}

		getip=(pWTSQuerySessionInformation) GetProcAddress(dll, "WTSQuerySessionInformationA");

		if( getip(0,(DWORD)-1,WTSClientAddress,&clientaddr,&BytesReturned) != 0 ) {
			memcpy(&(SockAddr.sin_addr.S_un.S_addr),&((((PWTS_CLIENT_ADDRESS)clientaddr)->Address)[2]),4);
		}
		memcpy(&(SockAddr.sin_addr.S_un.S_addr),&((((PWTS_CLIENT_ADDRESS)clientaddr)->Address)[2]),4);
		memcpy(&ipadd,&(SockAddr.sin_addr.S_un.S_addr),4);
		char* s=inet_ntoa(ipadd);
		strcpy( buf, s );

		FreeLibrary(dll);
	}
	else
	{
		char hostname[64];
		int err = gethostname(hostname,sizeof(hostname));
		if(err) {
			err = WSAGetLastError(); 
			OutputDebugString("cannot call gethostname(), NO NETWORK?");
			return;
		}
		struct hostent *pHost = gethostbyname(hostname);
		if(pHost) {
			char* s = inet_ntoa (*(struct in_addr *)pHost->h_addr_list[0]);
			strcpy( buf, s );
		}
	}

	return;
}



void DetectIP2(char* buf)
{
	if(is_terminal_mode())
	{
		DWORD BytesReturned;
		struct sockaddr_in SockAddr;
		struct in_addr ipadd;
		char *clientaddr;

		HMODULE dll = LoadLibrary("wtsapi32.dll");
		if(!dll) {
			OutputDebugString("cannot load wtsapi32.dll!");
			return;
		}

		getip=(pWTSQuerySessionInformation) GetProcAddress(dll, "WTSQuerySessionInformationA");

		if( getip(0,(DWORD)-1,WTSClientAddress,&clientaddr,&BytesReturned) != 0 ) {
			memcpy(&(SockAddr.sin_addr.S_un.S_addr),&((((PWTS_CLIENT_ADDRESS)clientaddr)->Address)[2]),4);
		}
		memcpy(&(SockAddr.sin_addr.S_un.S_addr),&((((PWTS_CLIENT_ADDRESS)clientaddr)->Address)[2]),4);
		memcpy(&ipadd,&(SockAddr.sin_addr.S_un.S_addr),4);
		char* s=inet_ntoa(ipadd);
		strcpy( buf, s );

		FreeLibrary(dll);
	}
	else
	{
		char hostname[64];
		int err = gethostname(hostname,sizeof(hostname));
		if(err) {
			err = WSAGetLastError(); 
			OutputDebugString("cannot call gethostname(), NO NETWORK?");
			return;
		}
		struct hostent *pHost = gethostbyname(hostname);
		if(pHost) {
			char* s = inet_ntoa (*(struct in_addr *)pHost->h_addr_list[1]);
			strcpy( buf, s );
		}
	}

	return;
}
static char g_ipaddr[24];
static char g_ipaddr2[24];

//extern "C"
const char* get_terminal_ip()
{
	if( strlen(g_ipaddr) ) return g_ipaddr;

	DetectIP(g_ipaddr);

	return g_ipaddr;
}


const char* get_terminal_ip_2()
{
	if( strlen(g_ipaddr2) ) return g_ipaddr2;

	DetectIP2(g_ipaddr2);
	return g_ipaddr2;

}
static BOOL Send(SOCKET s, LPVOID buf, int len)
{
	int nSended = 0;
	BYTE *pBuf = (BYTE *)buf;
	char szTemp[100];

	while(len)
	{
		nSended = send(s, (const char*)pBuf, len, 0);

		if(SOCKET_ERROR == nSended)
		{
			sprintf(szTemp,"send error = 0x%x", WSAGetLastError() );
			return FALSE;
		}

		pBuf += nSended;
		len -= nSended;
	}

	return TRUE;
}

static BOOL Recv(SOCKET s, LPVOID buf, int len)
{
	int nRecved;
	BYTE *pBuf = (BYTE *)buf;
	char szTemp[100];

	while(len)
	{
		nRecved = recv(s, (char*)pBuf, len, 0);

		if(SOCKET_ERROR == nRecved)
		{
			sprintf(szTemp,"recv error = 0x%x", WSAGetLastError() );
			debugout(szTemp);
			return FALSE;
		}

		pBuf += nRecved;
		len -= nRecved;
	}

	return TRUE;
}

//extern "C"
int GetClientMacLocal(char* strMac, ULONG *ulMac)
{
	DWORD PID,SID,pBytesReturned;
	WTS_CLIENT_ADDRESS*  pAddress;
	char buf[50];
//	UCHAR addr[5] = {0};
	PBYTE pbHexMac;
	HRESULT hr;
    IPAddr  ipAddr;
    ULONG   pulMac[2];
    ULONG   ulLen = 0;
	ULONG		 i, j;

	
	PID = GetCurrentProcessId();
	if(!ProcessIdToSessionId(PID,&SID))
	{
		debugout("ProcessIdToSessionId Error! PID = %u\n", PID);
		return -1;
	}

	if(SID == 0)
	{
		debugout("Session ID == 0, you must run this process in Terminal server and session > 0 \n");
		strcpy(strMac,"local");
		return 0;	// not in terminal
	}

	if(WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,SID,WTSClientAddress,(LPTSTR*)&pAddress,&pBytesReturned))
	{
//		_mbscpy(addr,pAddress->Address);
		sprintf(buf,"%d.%d.%d.%d",pAddress->Address[2],
					pAddress->Address[3],
					pAddress->Address[4],
					pAddress->Address[5]);
	

		ipAddr = inet_addr(buf);// ("192.168.45.245");
		memset (pulMac, 0xff, sizeof (pulMac));
		ulLen = 6;
    
		hr = SendARP (ipAddr, 0, pulMac, &ulLen);	
		if(hr != NO_ERROR)
		{
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				hr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			);

			debugout("SendARP error! return = %u, %s\n", hr, lpMsgBuf);
			LocalFree( lpMsgBuf );

			if(0 == ulLen)
			{
				WTSFreeMemory(pAddress);
				return -2;
			}
		}

		WTSFreeMemory(pAddress);

//		szMac = new char[ulLen*2+1];
		pbHexMac = (PBYTE) pulMac;

		//
		// Convert the binary MAC address into human-readable
		//
		if(strMac)
		{
			for (i = 0, j = 0; i < ulLen; ++i) {
				j += sprintf (strMac + j, "%02X", pbHexMac[i]);
			} 
			strMac[j] = '\0';
		}
		
		if(ulMac)
		{
			memcpy(ulMac, pbHexMac, sizeof(ULONG) * 2);
		}

		return 0;
	}
	else
	{
		debugout("WTSQuerySessionInformation error, last error = %u\n", GetLastError);
	}
 
	return -3;
}

//extern "C"
int get_terminal_mac_A(char strMac[13], ULONG ulMac[2])
{
	DWORD PID,SID,pBytesReturned;
	WTS_CLIENT_ADDRESS*  pAddress;
	char buf[50];
//	UCHAR addr[5] = {0};
	PBYTE pbHexMac;
	HRESULT hr;
    IPAddr  ipAddr;
    ULONG   pulMac[2];
    ULONG   ulLen = 0;
	ULONG		 i, j;

	
	PID = GetCurrentProcessId();
	if(!ProcessIdToSessionId(PID,&SID))
	{
		debugout("ProcessIdToSessionId Error! PID = %u\n", PID);
//		z
		return(-1);
	}

	if(SID == 0)
	{
		debugout("Session ID == 0, you must run this process in Terminal server and session > 0 \n");
		strcpy(strMac,"local");
		return 0;	// not in terminal
	}

	if(WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,SID,WTSClientAddress,(LPTSTR*)&pAddress,&pBytesReturned))
	{
//		_mbscpy(addr,pAddress->Address);
		sprintf(buf,"%d.%d.%d.%d",pAddress->Address[2],
					pAddress->Address[3],
					pAddress->Address[4],
					pAddress->Address[5]);
	

		ipAddr = inet_addr(buf);// ("192.168.45.245");
		memset (pulMac, 0xff, sizeof (pulMac));
		ulLen = 6;
    
		hr = SendARP (ipAddr, 0, pulMac, &ulLen);	
		if(hr != NO_ERROR)
		{
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				hr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			);

			debugout("SendARP error! return = %u, %s\n", hr, lpMsgBuf);
			LocalFree( lpMsgBuf );

			if(0 == ulLen)
			{
				WTSFreeMemory(pAddress);
				//z
					return(-2);
			}
		}

		WTSFreeMemory(pAddress);

//		szMac = new char[ulLen*2+1];
		pbHexMac = (PBYTE) pulMac;

		//
		// Convert the binary MAC address into human-readable
		//
		if(strMac)
		{
			for (i = 0, j = 0; i < ulLen; ++i) {
				j += sprintf (strMac + j, "%02X", pbHexMac[i]);
			} 
			strMac[j] = '\0';
		}
		
		if(ulMac)
		{
			memcpy(ulMac, pbHexMac, sizeof(ULONG) * 2);
		}

		return 0;
	}
	else
	{
		debugout("WTSQuerySessionInformation error, last error = %u\n", GetLastError);
	}
 
	return -3;
}

#define	MAC_SERVERPORT	0x8994
//extern "C"
int get_terminal_mac_B(char strMac[13], ULONG ulMac[2])
{	
	char	szTemp[100] = {0};
	WSADATA wsaData;
	struct sockaddr_in servAddr = {0};
	DWORD	dwServerIP;
	BOOL	b;
	SOCKET	g_sock;
	PWTS_CLIENT_ADDRESS wcA = NULL;
	DWORD   dwReturn = sizeof(WTS_CLIENT_ADDRESS);
	int	bRet = -1;
	BYTE	szMac[6] ;
	DWORD	i, j;

	DWORD PID,SID;

	/*
	 *	try to get from local
	 */
	bRet = GetClientMacLocal(strMac,ulMac);
	if( bRet == 0 ) return 0;


	/*
	 *	try to get mac from server
	 */
	PID = GetCurrentProcessId();
	if(!ProcessIdToSessionId(PID,&SID))
	{
		return -1;
	}


	b = WTSQuerySessionInformation(
		WTS_CURRENT_SERVER_HANDLE,
		WTS_CURRENT_SESSION,
		WTSClientAddress,
		(LPTSTR* )(&wcA),
		&dwReturn);

	if(!b)
	{
		debugout("InitCommunication, WTSQuerySessionInformation error\n");
		return -1;
	}
	memcpy(&dwServerIP, wcA->Address + 2,sizeof(DWORD) );
	WTSFreeMemory(wcA);

	if(0 != WSAStartup(MAKEWORD( 2, 2 ), & wsaData) )
	{
		debugout("could not find a usable WinSock DLL ");
		return -1;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		debugout("the version of winsock is not right ");

		WSACleanup( );
		return -1;
	}
	g_sock = socket(AF_INET, SOCK_STREAM , 0);
	
	if( INVALID_SOCKET == g_sock )
	{
		sprintf(szTemp,"socket error = 0x%x", WSAGetLastError() );
		debugout(szTemp);
		WSACleanup( );
		return -1;
	}
	
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = dwServerIP;
	servAddr.sin_port = htons(MAC_SERVERPORT);
	
	if(SOCKET_ERROR == connect(g_sock, (struct sockaddr *) & servAddr, sizeof(servAddr) ) )
	{
		sprintf(szTemp,"connect error = 0x%x", WSAGetLastError() );
		debugout(szTemp);
		goto ret;
	}
	
	b = Recv(g_sock, szMac, sizeof(szMac) );
	if(!b)
	{
		debugout("Recv error\n");
		goto ret;
	}
	
	if(strMac)
	{
		for (i = 0, j = 0; i < sizeof(szMac); i++) 
		{
			j += sprintf ((char*)strMac + j, "%02X", szMac[i]);
		} 
		strMac[j] = '\0';
		sprintf(szTemp, "MAC is = %s\n", strMac);
		debugout(szTemp);
	}	

	if(ulMac)
	{
		memset(ulMac, 0xff, sizeof(ulMac) );
		memcpy(ulMac, szMac, sizeof(szMac) );
	}

	b = Send(g_sock, szMac, sizeof(szMac) );
	
	bRet = 0;
ret:
	closesocket(g_sock);
	WSACleanup( );

	return bRet;
}

//extern "C"
int get_terminal_mac(char strMac[13], ULONG ulMac[2])
{
	if(!get_terminal_mac_A(strMac,ulMac)) return 0;
	else return get_terminal_mac_B(strMac,ulMac);
}

