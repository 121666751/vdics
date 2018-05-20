#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <myplus.h>
#include "authinput.h"

#define MAXLINE 1024

static void usage(void)
{
	printf("Usage: ./pasrv or ./pasrv -v\n");
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
          case '?':
              usage();
              return -1; 
      }
  }   
    
	CAuthInput serv;
	
	LogPrint( AUTHINPUT_NODE, "pasrv start to run...");

	int ret = serv.StartServer(verbose);

	if(!ret)	
	{
		LogPrint( AUTHINPUT_NODE, "pasrv 正常退出!\n");
	}
	else
	{
		LogPrint( AUTHINPUT_NODE, "pasrv 终止!\n");
	}

	return 0;
}
