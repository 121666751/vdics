#include <stdio.h>
#include <../common/PubFun.h>
#include "SendVouchers.h"


static void usage(void)
{
	printf("Usage: ./pmsrv or ./pmsrv -v\n");
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

	CSENDV cc;
	cc.Start(verbose);
	return 0;
	
	
}

