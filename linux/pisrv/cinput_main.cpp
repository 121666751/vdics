#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <myplus.h>
#include "cinput.h"

#define MAXLINE 1024

static void usage(void)
{
	printf("Usage: ./pisrv or ./pisrv -v\n");
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
    
	CInput serv;
	
	LogPrint( AUTHINPUT_NODE, "pisrv start to run...");

	int ret = serv.StartServer(verbose);

	if(!ret)	
	{
		LogPrint( CINPUT_NODE, "pisrv 正常退出!\n");
	}
	else
	{
		LogPrint( CINPUT_NODE, "pisrv 终止!\n");
	}

	return 0;
}
