#include <stdio.h>

static char UncryptBase( char k, char d )
{
    unsigned char k1, d1;
    int i,j,m;

    if ( !d ) return 0;
    k1 = (unsigned char) k;
    d1 = (unsigned char) d;
    i = d1;
    j = k1;
 
    m = i - 0x100;
    m = m + ( ( j ^ 0xff ) & 0xff );
    m = m ^ j;
    m = m & 0xff;
    m = m+1;
    return m;
}

int UnEncrypt( char * src, char * des )
{
    int deslen, i;

    if ( src == NULL )
        return -1;
    if ( des == NULL ) {
       return -1;
    }
    deslen = strlen( des );
    if ( deslen <= 8 ) {
       return -1;
    }

    memset( src, 0, deslen-8 );
    for ( i=8; i<deslen; i++ )
        src[i-8] = UncryptBase( des[i%8], des[i] );
    src[deslen-8] = '\0';
    return 0;
}

