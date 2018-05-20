/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#include "IniRead.h"
#include <zstandard.h>
#include <fstream>

using namespace std;

void FillMap( int *map, char *str)
{
    int length = strlen(str);
    int pos = 0;
    for( int i = 0 ; i < length ; i++)
    {
        if( str[i] == '=' )
        {
            pos = i;
            break;
        }
    }
    if( pos <= 0 ) return;
	
    char item1[40];
    char item2[40];
    memset( item1, 0, sizeof(item1));
    memset( item2, 0, sizeof(item2));
    memcpy( item1, str, pos );
    memcpy( item2, str+pos+1, length-pos-1 );
    map[atoi(item1)] = atoi(item2);
}

int ReadCfgFile(int *map)
{
    for( int i = 0 ; i < MAPCOUNT ; i++ )
    {
        map[i] = i;
    }
	
    if( !ZDoesFileExist("pocmap.ini")) return -1;
	
    ifstream in("pocmap.ini");
	
    char msg[40];
	
    while(!in.eof())
    {
        memset( msg,0, sizeof(msg));
		in>>msg;
		if( strlen(msg) <= 0 && in.eof() ) break;
		else FillMap(map,msg);
    }
    return 0;
}
