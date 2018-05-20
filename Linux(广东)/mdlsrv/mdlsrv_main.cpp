#include "mdlsrv_main.h"
#include "mdlsrv.h"

int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("Usage: ./mdlsrv filename\n");
		return -1;
	}	
	CMdlsrv serv;

	printf("start mdlsrv...\n");

	int ret = serv.StartServer(argv[1]);

	return 0;
}
