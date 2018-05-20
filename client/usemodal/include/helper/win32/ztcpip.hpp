#ifndef __zdev_win32_tcpip_hpp__
#define __zdev_win32_tcpip_hpp__

#include "../sys/zbase.hpp"
#include <windows.h>


ZDEV_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
struct tcp_keepalive {
    u_long  onoff;
    u_long  keepalivetime;
    u_long  keepaliveinterval;
};

// New WSAIoctl Options

#define SIO_RCVALL            _WSAIOW(IOC_VENDOR,1)
#define SIO_RCVALL_MCAST      _WSAIOW(IOC_VENDOR,2)
#define SIO_RCVALL_IGMPMCAST  _WSAIOW(IOC_VENDOR,3)
#define SIO_KEEPALIVE_VALS    _WSAIOW(IOC_VENDOR,4)
#define SIO_ABSORB_RTRALERT   _WSAIOW(IOC_VENDOR,5)
#define SIO_UCAST_IF          _WSAIOW(IOC_VENDOR,6)
#define SIO_LIMIT_BROADCASTS  _WSAIOW(IOC_VENDOR,7)
#define SIO_INDEX_BIND        _WSAIOW(IOC_VENDOR,8)
#define SIO_INDEX_MCASTIF     _WSAIOW(IOC_VENDOR,9)
#define SIO_INDEX_ADD_MCAST   _WSAIOW(IOC_VENDOR,10)
#define SIO_INDEX_DEL_MCAST   _WSAIOW(IOC_VENDOR,11) 


/* Option to use with [gs]etsockopt at the IPPROTO_IP level */

#define IP_OPTIONS      1 /* set/get IP options */
#define IP_HDRINCL      2 /* header is included with data */
#define IP_TOS          3 /* IP type of service and preced*/
#define IP_TTL          4 /* IP time to live */
#define IP_MULTICAST_IF     9 /* set/get IP multicast i/f  */
#define IP_MULTICAST_TTL       10 /* set/get IP multicast ttl */
#define IP_MULTICAST_LOOP      11 /*set/get IP multicast loopback */
#define IP_ADD_MEMBERSHIP      12 /* add an IP group membership */
#define IP_DROP_MEMBERSHIP     13/* drop an IP group membership */
#define IP_DONTFRAGMENT     14 /* don't fragment IP datagrams */
#define IP_ADD_SOURCE_MEMBERSHIP  15 /* join IP group/source */
#define IP_DROP_SOURCE_MEMBERSHIP 16 /* leave IP group/source */
#define IP_BLOCK_SOURCE           17 /* block IP group/source */
#define IP_UNBLOCK_SOURCE         18 /* unblock IP group/source */
#define IP_PKTINFO                19 /* receive packet information for ipv4*/

/* Option to use with [gs]etsockopt at the IPPROTO_IPV6 level */

#define IPV6_HDRINCL            2  /* Header is included with data */
#define IPV6_UNICAST_HOPS       4  /* Set/get IP unicast hop limit */
#define IPV6_MULTICAST_IF       9  /* Set/get IP multicast interface */
#define IPV6_MULTICAST_HOPS     10 /* Set/get IP multicast ttl */
#define IPV6_MULTICAST_LOOP     11 /* Set/get IP multicast loopback */
#define IPV6_ADD_MEMBERSHIP     12 /* Add an IP group membership */
#define IPV6_DROP_MEMBERSHIP    13 /* Drop an IP group membership */
#define IPV6_JOIN_GROUP         IPV6_ADD_MEMBERSHIP
#define IPV6_LEAVE_GROUP        IPV6_DROP_MEMBERSHIP
#define IPV6_PKTINFO            19 /* Receive packet information for ipv6 */


/* Option to use with [gs]etsockopt at the IPPROTO_UDP level */

#define UDP_NOCHECKSUM  1
#define UDP_CHECKSUM_COVERAGE   20  /* Set/get UDP-Lite checksum coverage */

/* Option to use with [gs]etsockopt at the IPPROTO_TCP level */

#define  TCP_EXPEDITED_1122 0x0002


//
#define ICMP_ECHO		8			// ICMP回显请求报文的类型值为8 
#define ICMP_ECHOREPLY	0			// ICMP回显应答报文的类型值为0 

// 定义IP 首部 
typedef struct _iphdr 
{ 
	unsigned char h_verlen; // 4位首部长度,4位IP版本号 1 
	unsigned char tos; // 8位服务类型TOS 1 
	unsigned short total_len; // 16位总长度(字节) 2 
	unsigned short ident; // 16位标识 2 
	unsigned short frag_and_flags; // 3位标志位 2 
	unsigned char ttl; // 8位生存时间 TTL 1 
	unsigned char proto; // 8位协议(TCP, UDP 或其他) 1 
	unsigned short checksum; // 16位IP首部校验和 2 
	unsigned int sourceIP; // 32位源IP地址 4 
	unsigned int destIP; // 32位目的IP地址 4 
} IPHeader; // IP首部长度为: 20 

// 定义ICMP首部 
typedef struct _ihdr 
{ 
	unsigned char i_type; // 8位类型 1 
	unsigned char i_code; // 8位代码 1 
	unsigned short i_cksum; // 16位校验和 2 
	unsigned short i_id; // 识别号(用进程号作为识别) 2 
	unsigned short i_seq; // 报文序列号 2 
} ICMPHeader; // ICMP首部长度为: 8 

//////////////////////////////////////////////////////////////////////////

ZDEV_NAMESPACE_END

#endif // __zdev_win32_tcpip_hpp__