#ifndef MDLSRV_H
#define MDLSRV_H

#include <iostream>
#include <zsocket.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <db_common.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include <package.h>
#include <myplus.h>
#include <ConfigFile.h>
#include <gsip_util.h>
#include <AnsiMemfile.h>
#include <mem.h>
#include <ZTime.hpp>

//单票识别类
class CMdlsrv
{
	public:
		CMdlsrv();
		~CMdlsrv();
		int StartServer(char *fname);
		ssize_t chomp(char *buffer, size_t length);
		ssize_t replace_slash(char *buffer);
		char *getfname(char *buffer, char *fname);
		char * get_date_str(char *date_str);
	private:
};

#endif
