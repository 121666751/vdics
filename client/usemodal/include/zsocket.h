#ifndef _ZSOCKET_H_
#define _ZSOCKET_H_

//#include <zstandard.h>

#ifdef WIN32		/* used in windows */
  #include <winsock.h>
  
  /* */
  #define z_closesocket closesocket
  

#else				/* used in Unix/Linux */
  #include <sys/socket.h>
  #include <sys/time.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <netinet/in.h>
  
  
  /* */
  #define z_closesocket close
  typedef u_int SOCKET;
  
  #ifndef INADDR_ANY
 	#define INADDR_ANY	      (u_long)0
  #endif
 
#endif 				/* WIN32 */

#ifdef TARGET_LINUX
  typedef unsigned int z_socklen;
#else
  typedef int z_socklen;
#endif

#endif /* _ZSOCKET_H_ */
