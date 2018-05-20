#include "vouchers_main.h"
#include "vouchers.h"

static void usage(void)
{
	    printf("Usage: ./pvsrv or ./pvsrv -v\n");
}

int main(int argc,char **argv)
{
	CVouchers serv;

	//verbose=1为调试模式： 打印的信息长
	int ch=0, verbose=0;

	while(argc >1 && ((ch=getopt(argc, argv, "vn:"))!=-1))
	{
		switch(ch)
		{
			case 'v':
				verbose = 1;
				break;
			case 'n':
				usage();
				return -1;
		}
	}

	LogPrint( VOUCHER_NODE,"pvsrv start to run!\n"); 

	int ret = serv.StartServer(verbose);

	if(!ret)	
	{
		LogPrint( VOUCHER_NODE,"pvsrv 正常退出!\n"); 
	}
	else
	{
		LogPrint( VOUCHER_NODE,"pvsrv 终止!\n"); 
	}

	return 0;
}
