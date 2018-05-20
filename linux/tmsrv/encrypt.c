#include <stdio.h>
#include <time.h>

static void FillKey( char * des )
{
    time_t t;
    long l;
    unsigned char buf[9];
    unsigned char * p;
    int i;

   
    if ( time( &t ) < 0 )
        l = 0;
    else
        l = (long) t;

    memset( buf, 0, 9 );
    sprintf( ( char *)buf, "%08x", l );
    p = ( unsigned char *) des;
    for( i=0; i<8; i++ ) {
        p[i] = buf[i] ^ 0x55;
    }
    return;
}

static char EncryptBase( char k, char c )
{
    unsigned char k1, c1;
    int i,j, m;

    if ( !c ) return 0;

    k1 = (unsigned char) k;
    c1 = (unsigned char) c;

    i = c1;
    j = k1;
    m = i-1;
    m = m ^ j;
    m = m & 0xff;
    m = m - ( ( j ^ 0xff ) & 0xff );
    if ( m < 0 ) m = m + 0x100;
    return m;
}

int Encrypt( char * src, char * des )
{
    int srclen, i;

    if ( des == NULL )
        return -1;
    if ( src == NULL ) {
       des[0] = '\0';
       return 0;
    }
    srclen = strlen( src );
    if ( srclen <= 0 ) {
       des[0] = '\0';
       return 0;
    }

    memset( des, 0, srclen+8 );
    FillKey( des );
     
    for ( i=0; i<srclen; i++ )
        des[i+8] = EncryptBase( des[i%8], src[i] );
    des[srclen+8] = '\0';
    return 0;
}
