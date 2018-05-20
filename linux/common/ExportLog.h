#ifndef __EXPORTLOG_H__
#define __EXPORTLOG_H__

#include <stdio.h>
#include <time.h>

#define __EXPORT_LOG

//#define VOUCHER_SEND_LOG		"/tmp/prets5.log"

#define TASK_NODE_LOG 			LOG_PREFIX"tmsrv.log"
#define VOUCHER_PARSE_LOG 		LOG_PREFIX"pvsrv.log"
#define MULTS_PARSE_LOG 		LOG_PREFIX"pmsrv.log"
#define VOUCHER_SEND_LOG		LOG_PREFIX"svsrv.log"
#define MULTS_SEND_LOG			LOG_PREFIX"smsrv.log"
#define AUTHINPUT_PARSE_LOG		LOG_PREFIX"pasrv.log"
#define AUTHINPUT_SEND_LOG		LOG_PREFIX"sasrv.log"

#define LOG_PREFIX  "/tmp/"

#define DEBUG_BUFFER_MAX 			4096
#define DEBUG_TIME_MAX				30
        
#define	TASK_NODE					1
#define VOUCHER_NODE				2
#define MULTIS_NODE					3
#define VOUCHER_SEND_NODE			4
#define MULTIS_SEND_NODE			5
#define AUTHINPUT_NODE				6
#define AUTHINPUT_SEND_NODE			7
        
#ifdef  __EXPORT_LOG
	#ifndef __OS_LINUX_VER
		#define LogPrint	_Track
		#define LogError()
	#else
//		#define LogPrint(fmt, args...)	debug_print(stdout, __FILE__, __LINE__, __FUNCTION__, fmt, ##args);
		#define LogError()	debug_print(stdout, __FILE__, __LINE__, __FUNCTION__, "error: %s", strerror(errno));
		#define LogPrint(node, format, args...) {\
            char buffer[DEBUG_BUFFER_MAX-1] = {0};\
            char strtime[DEBUG_TIME_MAX] = {0};\
            struct tm *ptm;\
  			long ts;\
  			int  y,m,d,h,n,s;\
    		ts = time(NULL);\
  			ptm = localtime(&ts);\
            y = ptm->tm_year+1900;\
            m = ptm->tm_mon+1;\
            d = ptm->tm_mday;\
            h = ptm->tm_hour;\
            n = ptm->tm_min;\
            s = ptm->tm_sec;\
            sprintf(strtime, "[%04d-%02d-%02d %02d:%02d:%02d]", y, m, d, h, n, s);\
            snprintf(buffer, DEBUG_BUFFER_MAX \
                    , ""format" \n", ##args);\
            FILE* fd;\
            if (node == TASK_NODE)\
            	fd = fopen(TASK_NODE_LOG, "a");\
            else if (node == VOUCHER_NODE)\
            	fd = fopen(VOUCHER_PARSE_LOG, "a");\
            else if (node == MULTIS_NODE)\
            	fd = fopen(MULTS_PARSE_LOG, "a");\
            else if (node == VOUCHER_SEND_NODE)\
            	fd = fopen(VOUCHER_SEND_LOG, "a");\
			else if (node == MULTIS_SEND_NODE)\
            	fd = fopen(MULTS_SEND_LOG, "a");\
			else if (node == AUTHINPUT_NODE)\
            	fd = fopen(AUTHINPUT_PARSE_LOG, "a");\
			else if (node == AUTHINPUT_SEND_NODE)\
            	fd = fopen(AUTHINPUT_SEND_LOG, "a");\
			if (fd != NULL) {\
            	fwrite(strtime, strlen(strtime), 1, fd);\
            	fwrite(buffer, strlen(buffer), 1, fd);\
                fflush(fd);\
                fclose(fd);\
            }\
        }

	#endif
#else 
	#define LogPrint
	#define LogError()
#endif	// __EXPORT_LOG

#ifndef __OS_LINUX_VER
#include <windows.h>

__inline VOID _Track(int node, LPCTSTR ptzFormat, ...)
{
	va_list vlArgs;
	TCHAR tzText[1024];
	TCHAR str[1024];
	va_start(vlArgs, ptzFormat);
	wvsprintf(tzText, ptzFormat, vlArgs);
	
	wsprintf(str, "tchzt: %s", tzText);
	OutputDebugString(str);
	va_end(vlArgs);
}

#else
// Linux
#include <sys/param.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>


static void debug_print(FILE * fp, const char * filename, const int line, 
				 const char * funcname, char *fmt, ...)
{
    char buf[1024];
    time_t t;
    struct tm * now;
    va_list ap;
	
    time(&t);
    now = localtime(&t);
    va_start(ap, fmt);
//    fprintf(fp, "%02d:%02d:%02d -- %s(%d):%s - \"", 
//			now -> tm_hour, now -> tm_min, now -> tm_sec, filename, line, funcname);
    vsprintf(buf, fmt, ap);
    fprintf(fp, "%s\"\n", buf);
    va_end(ap);
}

#endif	// WIN32


#endif //__EXPORTLOG_H__
