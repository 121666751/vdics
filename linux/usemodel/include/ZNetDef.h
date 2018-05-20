#ifndef __ZNETDEF_H
#define __ZNETDEF_H

//////////////////////////////////////////////////////////////////
//	Center database server's Host address
#define CENTER_SERVER	"CENTER_SERVER"
#define CENTER_PORT		8088

///////////////////////////////////////////////////////////////////
// this micro use for the IP address of local database server
#define LOCAL_SERVER	"LOCAL_SERVER"
#define LOCAL_PORT		3503

///////////////////////////////////////////////////////////////////
//	The following micro set for Mysql Database Server
#define LOCAL_DB_HOST		"LOCAL_DB_HOST"
//#define LOCAL_DB_NAME		"LocalBuffer"
#define LOCAL_DB_NAME		"ztic"

#define AIS_DB_HOST		"AIS_DB_HOST"
#define AIS_DB_NAME		"ztic"

#define LDB_USERNAME		"ztic"
#define LDB_PASSWORD		"123123"

//----------------------------------------------------------------
#define CENTER_DB_HOST		"CENTER_DB_HOST"
#define CENTER_DB_NAME		"CheckId"

///////////////////////////////////////////////////////////////////
//	The following micro set for Sybase Database Server
#if defined(SJZ)
	#define ZDB_SERVERNAME	"CLIENT"
#elif defined(WUHAN)
	#define ZDB_SERVERNAME	"SYBASE"
#elif defined(FUZHOU)
	#define ZDB_SERVERNAME	"CLIENT"
#else
	#define ZDB_SERVERNAME	"redhat_sybase"
#endif

#define ZDB_USERNAME	"cidsa"
#define ZDB_PASSWORD	"z412t620"
#define ZDB_DBNAME		"CheckId"

///////////////////////////////////////////////////////////////////
//  The following micro set for Voucher Process Center
#define VPC_IMG_HOST	"VPC_IMG_HOST"
#define VPC_IMG_PORT	3210

#define VPC_UDP_HOST	"VPC_UDP_HOST"
#define VPC_UDP_PORT	3310

#define MESSAGE_SERVER  "MESSAGE_SERVER"
#define MESSAGE_PORT    1122

#define DUMP_PORT       7304        // Net mysql backup server in center


///////////////////////////////////////////////////////////////////
#endif //__ZNETDEF_H

