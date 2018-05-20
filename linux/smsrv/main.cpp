#include <stdio.h>
#include <PubFun.h>
#include "SendMultis.h"


static void usage(void)
{
	printf("Usage: ./smsrv or ./smsrv -v\n");
}

int main(int argc, char* argv[])
{
//	init_dameon();
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
    
	CSENDM cc;

	cc.Start(verbose);
	return 0;
	
	
}

