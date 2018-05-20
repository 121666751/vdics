/********************************************************************

  FUNCTION USED FOR TRANSFORM A NUMBER TO A STRING HAVE A IDENTICAL 
  LENGTH

 ********************************************************************/

#ifndef __ZERONETNO_H__
#define __ZERONETNO_H__

#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH	20
//////////////////////////////////////////////////////////////////////
int GetRightNumber( int length, int number, char* pnumber);


//////////////////////////////////////////////////////////////////////
inline int GetRightNumber( int length, int number, char* pnumber)
{
	if( length <= 0){
		return -1;			//0 number to change
	}
	if( length > MAX_LENGTH){
		return -2;			//number too long to change
	}
	if( number < 0 ){
		return -3;			//can not change number less than 0
	}

	memset( pnumber, 0, MAX_LENGTH+1);
	char fmt[10];
	sprintf( fmt, "%%0%dd", length);
	int err = _snprintf( pnumber, length, fmt, number);
	
	if( err > 0) return 0;
	return -4;				//number biger than wanted length

}

#endif //__ZERONETNO_H__
