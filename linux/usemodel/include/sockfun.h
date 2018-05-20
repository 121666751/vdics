/*
This is a header file contain socket extend functions.

To use this functions, you must include proper socket header file before
include this file.

*/ 


#ifndef __SOCKFUN_H__
#define __SOCKFUN_H__

#include <stdio.h>
#include "ZSocket.h"

typedef unsigned int u_int32_t;

/* readn - read exactly n bytes */
inline int readn( SOCKET fd, char *bp, size_t len)
{
	int cnt;
	int rc;

	cnt = len;
	while ( cnt > 0 )
	{
		rc = recv( fd, bp, cnt, 0 );
/*@.bp*/
		if ( rc < 0 )				/* read error? */
		{
//			if ( errno == EINTR )	/* interrupted? */
//				continue;			/* restart the read */
			return -1;				/* return error */
		}
		if ( rc == 0 )				/* EOF? */
			return len - cnt;		/* return short count */
		bp += rc;
		cnt -= rc;
	}
	return len;
}

/* readvrec - read a variable record */
/*
int readvrec( SOCKET fd, char *bp, size_t len )
{
	u_int32_t reclen;
	int rc;

// Retrieve the length of the record 
	rc = readn( fd, ( char * )&reclen, sizeof( u_int32_t ) );
	if ( rc != sizeof( u_int32_t ) )
		return rc < 0 ? -1 : 0;
	reclen = ntohl( reclen );
	if ( reclen > len )
	{
//
//		 *  Not enough room for the record--
//		 *  discard it and return an error.
//		 
		while ( reclen > 0 )
		{
			rc = readn( fd, bp, len );
			if ( rc != len )
				return rc < 0 ? -1 : 0;
			reclen -= len;
			if ( reclen < len )
				len = reclen;
		}
		set_errno( EMSGSIZE );
		return -1;
	}

	//  Retrieve the record itself 

	rc = readn( fd, bp, reclen );
	if ( rc != reclen )
		return rc < 0 ? -1 : 0;
	return rc;
}
*/


// write n byte
inline int writen(SOCKET fd, char *bp, size_t len )
{
	int r;
	int have_send = 0;
	int expect_len = len;
	while( expect_len>0 ) {
		r = send( fd, bp+have_send, expect_len, 0 );
		if( r <= 0 ) {
//			if ( errno == EINTR )	/* interrupted? */
//				continue;			/* restart the read */
			return -1;				/* return error */
		}
		have_send += r;
		expect_len -= r;
	}
	
	return have_send;
}

#endif // __SOCKFUN_H__
