#include "mdlsrv.h"
#include <errno.h>
#include <transimg.h>
#include <sstream>
#include <PubFun.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAXLINE 1024

#define TASK_COUNT 1000

using namespace std;

CMdlsrv::CMdlsrv(){}

CMdlsrv::~CMdlsrv(){}

//启动服务

int CMdlsrv::StartServer(char *file)
{
	FILE * fp;
	char savedir[512];
	bzero(savedir, sizeof(savedir));

	int sizeX, sizeY, pannel, ret;

	MemT pRGB;
	
	char buf[MAXLINE];
	char ip[24], fname[512], line[256];

	if((fp = fopen( file, "r+"))==NULL)
		perror("fopen");

	char tmp[MAXLINE];
	
	bzero(buf, MAXLINE);

	char date_str[20];
	bzero(date_str, sizeof(date_str));

	get_date_str(date_str);

	printf("date_str->[%s]\n", date_str);

	sprintf(savedir, "/tmp/%s/", date_str);

	sprintf(buf, "mkdir %s", savedir);

	system(buf);

	bzero(buf, sizeof(buf));

	while(fgets(buf, MAXLINE, fp)!=NULL)
	{
		chomp(buf, strlen(buf));

		bzero( ip, sizeof(ip) );
		bzero( line, sizeof(line));
		
		sscanf( buf, "%s %s", line, ip); 

		bzero(buf, MAXLINE);
		printf("line->[%s] ip->[%s]\n", line, ip);

		ret = ti_getfileformat_ex2(line,"jpg",ip,3210,99,0,1,pRGB);

		//ret=ti_getimage( line, ip, 3210, 0,  pRGB, sizeX, sizeY, pannel );

		if(ret!=0)
		{
			printf("ti_getimage error.![%d]\n", ret);
			continue;
		}

		printf("size->[%d], sizeX->[%d], sizeY->[%d], pannel->[%d]\n", pRGB.size(), sizeX, sizeY, pannel);

		bzero(fname, sizeof(fname));

		getfname( line, fname);

		bzero(tmp, MAXLINE);
		sprintf(tmp, "%s%s", savedir, fname);
	
		printf("tmp->[%s]\n", tmp);

		replace_slash(tmp);

		printf("size->[%d]\n", pRGB.size());

		pRGB.saveToFile(tmp);

		printf("save %s ok!\n", tmp);
	}

	printf("Get images ok!\n\nImages were saved in %s\n", savedir);
}

//get fname from dir-file name
char *CMdlsrv::getfname(char *buffer, char *fname)
{
	char * p = strrchr(buffer, '\\');	
	strcpy(fname, p+1);
	return fname;
}

//replace '\' with '//'
ssize_t CMdlsrv::replace_slash(char *buffer)
{
	char *p;
	while( (p=strchr(buffer, '\\'))!=0)
	{
		*p = '/';
	}
}

ssize_t CMdlsrv::chomp(char *buffer, size_t length)
{
	size_t chars;

	assert(buffer != NULL);
	assert(length > 0);

	/* Make sure the arguments are valid */
	if (buffer == NULL) return -EFAULT;
	if (length < 1) return -ERANGE;

	chars = 0;

	--length;
	while (buffer[length] == '\r' || buffer[length] == '\n') {
		buffer[length] = '\0';
		chars++;

		/* Stop once we get to zero to prevent wrap-around */
		if (length-- == 0) break;
	}

	return chars;
}

char * CMdlsrv::get_date_str(char *date_str)
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);
	
	sprintf(date_str, "%d%d%d", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday);

	return date_str;
}
