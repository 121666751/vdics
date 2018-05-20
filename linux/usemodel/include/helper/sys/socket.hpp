#ifndef __SOCKET_INCLUDE__
#define __SOCKET_INCLUDE__

#include <stdio.h>
#ifdef _WIN32
	#include <winsock2.h>
	typedef int socklen_t;
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <errno.h>

	typedef unsigned int        SOCKET;
	#define INVALID_SOCKET      (SOCKET)(~0)
	#define SOCKET_ERROR                (-1)
#endif


ZDEV_NAMESPACE_BEGIN

class Socket {
private:
	SOCKET handle;
	sockaddr_in addr;
public:
	static void init() {
		#ifdef _WIN32
		   WSADATA wsaData;
		   ::WSAStartup(MAKEWORD(2,2), &wsaData);
		#endif
	}
		
	static int getLastError() {
		#ifdef _WIN32
			return ::WSAGetLastError();
		#else
			return errno;
		#endif
	}

	Socket() {}
	Socket(SOCKET handle) { this->handle = handle; }

	SOCKET getHandle() { return this->handle; }
	
	bool connect(const char *ip, ushort port) {
	   handle = ::socket(AF_INET, SOCK_STREAM, 0);
	   if (handle == INVALID_SOCKET) return false;
	   
	   addr.sin_family = AF_INET;
	   addr.sin_port = ::htons(port);
	   addr.sin_addr.s_addr = ::inet_addr(ip);
	   socklen_t len = sizeof(addr);
	   int err = ::connect(handle, (const sockaddr*)&addr, len);
	   if (err) {
		   this->close();
		   return false;
	   }
	   return true;
	}

	bool listen(const char *ip, ushort port) {
		handle = ::socket(AF_INET, SOCK_STREAM, 0);
        if (handle == INVALID_SOCKET) return false;
		
		addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = ::inet_addr(ip);
        addr.sin_port = ::htons(port);

        socklen_t len = sizeof(addr);
        int err = ::bind(handle, (const sockaddr*)&addr, len);
        if (err == SOCKET_ERROR) {
            printf("bind port [%d] error.\n", port);
            this->close();
            return false;
        }
		
        err = ::listen(handle, 5);
        if (err == SOCKET_ERROR) {
            printf("listen in port [%d] error.\n", port);
            this->close();
            return false;
        }
		return true;
	}

	bool accept(Socket *s) {
		socklen_t len = sizeof(s->addr);
		s->handle = ::accept(handle, (sockaddr*)&s->addr, &len);
		return s->handle != INVALID_SOCKET;
	}

	int recv(void *buffer, int size) {
		return ::recv(handle, (char *)buffer, size, 0);
	}

	int send(const void *buffer, int size) {
		return ::send(handle, (const char *)buffer, size, 0);
	}

	void close() {
		#ifdef _WIN32
			::closesocket(handle);
		#else
			::close(handle);;
		#endif
	}

	char *getRemoteIP() {
		return ::inet_ntoa(addr.sin_addr);
	}

	ushort getRemotePort() {
		return ::ntohs(addr.sin_port);
	}
};

ZDEV_NAMESPACE_END

#endif // __SOCKET_INCLUDE__
