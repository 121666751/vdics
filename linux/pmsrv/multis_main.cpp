#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <myplus.h>
#include "multis_main.h"
#include "multis.h"

#define MAXLINE 1024

static void usage(void)
{
	printf("Usage: ./pmsrv or ./pmsrv -v\n");
}

int main(int argc,char **argv)
{
	
	int ch=0, verbose=0;
    
    while (argc > 1 && ((ch = getopt(argc, argv, "vn:")) != -1))
    {   
        switch (ch)
        {
            case 'v':
                verbose = 1;
                break;  
            case 'n':
                usage();
                return -1; 
        }
    }   

	CMultis serv;
	
	LogPrint( MULTIS_NODE, "pmsrv start to run...");

	int ret = serv.StartServer(verbose);

	if(!ret)	
	{
		LogPrint( MULTIS_NODE, "pmsrv 正常退出!\n");
	}
	else
	{
		LogPrint( MULTIS_NODE, "pmsrv 终止!\n");
	}

	return 0;
}
