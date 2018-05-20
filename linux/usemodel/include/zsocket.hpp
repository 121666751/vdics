
#ifndef __Z_SOCKET_HPP__
#define __Z_SOCKET_HPP__

#include <zsocket.h>
#include <ZStream.hpp>

#define 	Z_INVSOCK 				(~0)

#define		ZS_SUCCESS 				0
#define		ZS_INVALID_SOCKET		-1
#define		ZS_INVALID_HOSTNAME		-2
#define		ZS_CONNECT_ERR			-3
#define		ZS_TIMEOUT				-4
#define		ZS_OVERFLOW				-5
#define		ZS_UNMATCHEDLENGTH		-6
#define		ZS_SENDERR				-8

#define		ZS_MAXBUF				10000000
#define		ZS_PKG_SIZE				5000000
#ifdef GNU_PTH

#include <zpthsocket.hpp>

#else

class ZSocket
{
// public member functions
public:

	ZSocket()
			{	m_hSocket = Z_INVSOCK;
			}

	virtual ~ZSocket()
			{
				Close();
			}

            // SOCK_DGRAM
	int 	Create( int Type=SOCK_STREAM )
			{
				m_hSocket = socket(AF_INET, Type, 0);
				if( m_hSocket == Z_INVSOCK ) return ZS_INVALID_SOCKET;

				return ZS_SUCCESS;
			}

	int 	Bind( int port )
			{
				sockaddr_in server;
				server.sin_family = AF_INET;
				server.sin_port = htons(port);
				server.sin_addr.s_addr = htonl( INADDR_ANY );

				return bind(m_hSocket, (sockaddr*)&server, sizeof(server));
			}

	int 	Listen( int backlog = 50 )	{	return listen(m_hSocket,50); }

	SOCKET 	Accept() 				{	return accept(m_hSocket,NULL,NULL);	}

	SOCKET 	Accept( struct sockaddr *addr, int &addrlen )	{	return accept(m_hSocket,addr,(z_socklen *)&addrlen);	}
	
	unsigned long GetHostByStr(const char* host) 
			{
				unsigned long addr_val = inet_addr( host );
				
				if( addr_val != INADDR_NONE ) return addr_val;
				
				struct hostent* hp = (hostent *)gethostbyname(host);
				
				if( hp == 0 )	return INADDR_NONE;
				
				return ((struct in_addr *)(hp->h_addr))->s_addr;
			}

	int 	Connect( const char* host, int port )
			{
				if(m_hSocket==Z_INVSOCK)
				{
					if( Create() != ZS_SUCCESS )	return ZS_INVALID_SOCKET;
				}

				struct sockaddr_in server;
				
				server.sin_port = htons(port);
				server.sin_family = AF_INET;	
				
				unsigned long addr_val = GetHostByStr( host );
				
				if( addr_val == INADDR_NONE ) 	return ZS_INVALID_HOSTNAME;
				
				server.sin_addr.s_addr = addr_val;
				
				
				memset( server.sin_zero,0, 8);


				if( connect(m_hSocket, (struct sockaddr*)&server, sizeof(server)) < 0 )
				{
					z_closesocket(m_hSocket);
					m_hSocket = Z_INVSOCK;
					return ZS_CONNECT_ERR;
				}

				return ZS_SUCCESS;
			}

	int 	Send( void* buf, int buflen )	{	return send( m_hSocket, (char*)buf, buflen, 0 );	}

	int 	Recv( void* buf, int buflen )
			{
				return recv( m_hSocket, (char*)buf, buflen, 0 );
			}

	int 	RecvEx( char* buf, int buflen, int timeout = 5000 )
			{
				timeval tv;
				tv.tv_sec = timeout/1000;
				tv.tv_usec = (timeout%1000)*1000;

				fd_set readfds;
				FD_ZERO( &readfds );
				FD_SET( m_hSocket, &readfds );

				select(m_hSocket+1, &readfds,0,0, &tv );
				if( !FD_ISSET(m_hSocket, &readfds) ) return ZS_TIMEOUT;

				return recv( m_hSocket, buf, buflen, 0 );
			}

	int		SendTo( char* buf, int buflen, char* addr, int port )
			{
				struct sockaddr_in server;
				struct hostent* hp;
				server.sin_family = AF_INET;

				hp = (hostent *)gethostbyname(addr);
				if( hp == 0 )
				{
					return ZS_INVALID_HOSTNAME;
				}

				server.sin_port = htons(port);

				server.sin_addr = *( (struct in_addr *)(hp->h_addr) );
				memset( server.sin_zero,0, 8);

				return sendto(m_hSocket,buf,buflen,0,(sockaddr*)&server, sizeof(server));
			}

	int 	RecvFrom( char* buf, int buflen, char* addr, int port )
			{
				struct sockaddr_in server;
				struct hostent* hp;
				server.sin_family = AF_INET;

				hp = (hostent *)gethostbyname(addr);
				if( hp == 0 )
				{
					return ZS_INVALID_HOSTNAME;
				}

				server.sin_port = htons(port);

				server.sin_addr = *( (struct in_addr *)(hp->h_addr) );
				memset( server.sin_zero,0, 8);

				z_socklen fromlen = sizeof(server);
				return recvfrom(m_hSocket,buf,buflen,0,(sockaddr*)&server,&fromlen);
			}

	int 	Close()
			{

				if( m_hSocket!=Z_INVSOCK )
				{
					shutdown( m_hSocket, 2 );
					z_closesocket(m_hSocket);
					m_hSocket = Z_INVSOCK;

				}

				return ZS_SUCCESS;
			}

	SOCKET& Handle()		{	return (SOCKET&)m_hSocket;	}

	int 	Attach(SOCKET hSocket)
			{
				Close();
				m_hSocket = hSocket;

				return ZS_SUCCESS;
			}

	SOCKET 	Detach()
			{
				SOCKET hSocket = m_hSocket;
				m_hSocket = Z_INVSOCK;
				return hSocket;
			}

	int		SetBroadCast(bool Enable = true)
			{
				int on=(Enable)?1:0;
				return setsockopt(m_hSocket,SOL_SOCKET,SO_BROADCAST,(const char*)&on,sizeof(on));
			}

	/*	This function sends a four-bytes frame which are the size of the data to be sent,
		then the data are sent.
		This function should be used while the peer process is waiting on a TurboRecv call.*/
	int		TurboSend( void* buf, int buflen )
			{
				int restlen = buflen;
				int pkglen, ret, havesend=0;

				// send total size
				ret = Send( (char*)&buflen, 4 );
				if( ret != 4 ) return ZS_SENDERR;
				int tmp;
				RecvEx( (char*)&tmp, 4,1000 );

				// send package
				do {
					pkglen = (restlen>ZS_PKG_SIZE)? ZS_PKG_SIZE:restlen;
					restlen -= pkglen;
					ret = pkgSend( (char*)buf+havesend, pkglen );
					havesend += pkglen;
				} while( restlen > 0 && ret == 0 );

				return ret;
			}
	/*	This function receives a frame with a timeout. The first 4 BYTE of this frame
		contains its length. They are NOT passed into the user's buffer.
		This is a safe way to exchange binary structures in a client-server application.
		The server will neither hang nor crash if a bad structure is sent: it will just
		return errors such as ZS_TIMEOUT, ZS_OVERFLOW or ZS_UNMATCHEDLENGTH.*/
	int		TurboRecv( char* buf, int buflen )
			{
				int ret, nToBeRecv;
				ret = RecvEx((char*)&nToBeRecv,4, 3000);
				if( ret < 0 ) return ZS_TIMEOUT;
				if( nToBeRecv > buflen ) return ZS_OVERFLOW;

				Send( (char*)&nToBeRecv, 4 );

				int nHaveRecv = 0;
				do {
					ret = pkgRecv( buf+nHaveRecv, buflen-nHaveRecv );
					nHaveRecv += ret;
				} while( ret > 0 && nHaveRecv < nToBeRecv );

				if(ret<0 || nHaveRecv != nToBeRecv) return ZS_UNMATCHEDLENGTH;
				else	return nHaveRecv;
			}

	int		TurboRecv( ZStream& stream )
			{
				int ret, nToBeRecv;
				ret = RecvEx((char*)&nToBeRecv,4, 3000);
				if( ret < 0 ) return ZS_TIMEOUT;
				if( nToBeRecv > ZS_MAXBUF ) ZS_OVERFLOW;

				Send( (char*)&nToBeRecv, 4 );

				char *buf = new char[nToBeRecv];
				int nHaveRecv = 0;
				do {
					ret = pkgRecv( buf+nHaveRecv, nToBeRecv-nHaveRecv );

                    if( ret > 0 ) {
					    nHaveRecv += ret;
                    }
				} while( ret > 0 && nHaveRecv < nToBeRecv );

				if( nHaveRecv == nToBeRecv ) {
					stream.Free();
					stream.Append( buf, nHaveRecv );
				}
                delete[] buf;

				if(ret<0 || nHaveRecv != nToBeRecv) return ZS_UNMATCHEDLENGTH;
				else {
					return nHaveRecv;
				}
			}
	//////////////////////////////////////////////////////////////////////////////
	// Windows socket init functions
	static int 	init_winsock()
			{
				int t = 0;
			#ifdef WIN32
					WORD wVersionRequested;
					WSADATA wsaData;
					wVersionRequested = MAKEWORD(1,1);
					t = WSAStartup(wVersionRequested,&wsaData);
			#endif //WIN32
				return t;
			}

	static void 	clear_winsock()
			{
			#ifdef WIN32
					WSACleanup();
			#endif //WIN32
			}

// private member variant.
protected:
	int		pkgSend( char* buf, int buflen )
			{
				int ret;
				ret = Send( (char*)&buflen, 4 );
				if( ret != 4 ) return ZS_SENDERR;

				int tmp;
				RecvEx( (char*)&tmp, 4,1000 );
				int nHaveSend;
				nHaveSend = 0;
				do {
					ret = Send( buf+nHaveSend, buflen-nHaveSend );
					nHaveSend += ret;
				} while( ret > 0 && nHaveSend < buflen );

				RecvEx( (char*)&tmp, 4,1000 );
				if(ret<0 || nHaveSend != buflen) return ZS_SENDERR;
				else	return ZS_SUCCESS;
			}

	int		pkgRecv( char* buf, int buflen  )
			{
				int ret, nToBeRecv;
				ret = RecvEx((char*)&nToBeRecv,4, 5000);
				if( ret < 0 ) return ZS_TIMEOUT;
				if( nToBeRecv > buflen ) return ZS_OVERFLOW;

				int nHaveRecv = 0;
				Send( (char*)&nToBeRecv, 4 );
				do {
					ret = RecvEx( buf+nHaveRecv, buflen-nHaveRecv, 3000 );
					nHaveRecv += ret;
				} while( ret > 0 && nHaveRecv < nToBeRecv );

				Send( (char*)&nToBeRecv, 4 );

				if(ret<0 || nHaveRecv != nToBeRecv) return ZS_UNMATCHEDLENGTH;
				else	return nHaveRecv;
			}

	SOCKET m_hSocket;
};

class ZmyWSA
{
public:
	ZmyWSA() { ZSocket::init_winsock();}
	~ZmyWSA() { ZSocket::clear_winsock(); }
};

//////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
	#pragma comment(lib,"ws2_32.lib")
#endif
//////////////////////////////////////////////////////////////////////

#endif 	// GNU_PTH

#endif	//__Z_SOCKET_HPP__
