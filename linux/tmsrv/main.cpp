#include "main.h"
#include <unistd.h>
#include <stdio.h>

static void usage(void)
{
	printf("Usage: ./tmsrv or tmsrv -v\n");
}

int main(int argc,char **argv)
{
	int verbose=0, ch=0;

	while(argc > 1 && ((ch=getopt(argc, argv, "vn:"))!=-1))
	{
		switch(ch)
		{
			case 'v':
				verbose=1;
				break;
			case 'n':
				usage();
				return -1;
		}

	}

	printf("verbose->[%d]\n", verbose);

	CTaskManagerServer serv;

	serv.StartServer(verbose);

	return 0;
}
