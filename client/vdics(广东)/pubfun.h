#ifndef PUBFUN_H
#define PUBFUN_H
#include <vcl.h>


#include <map>
#include <vector>
#include <libmem.h>

#define ZM_SCAN_TASK        "451600"
#define ZM_DIST_TASK        "451601"
#define ZM_DIST_TASKEX      "451603"
#define ZM_DIST_DATA        "556114"
#define ZM_DIST_RTCS        "556503"
#define ZM_DIST_BRNO        "310200"
#define ZM_DIST_AREA        "310200"
#define ZM_DIST_CHECK       "556601"

using namespace std;

typedef map<String ,String> XmlMap;

String makeHead(char* mode);

String makeTaskBody(distTaskT body,String modeType);
String makeTaskBodyEx(distTaskT body,String acc,String vchno,String amt,String modeType);

String makeHead(char* mode);

String makeQueryDs(const char* orgCode,const char* optCode,const char* brno,const char*date,const char*session,const char*area,const char*exchno);
String makeTaskBody(distTaskT body);

int deleteFiles(FileVector Files);

char* pvb( int vt,char*  bt);

int doTrade(char* send,char *ip,int port,int & result,char*out,char*outstr);

String makeDataBody(distDataT body);
XmlMap parseOutXml(String xmlstr);
String makeXml(XmlMap keys,FileVector Files);

String makeUpdateXml(XmlMap keys,FileVector Files);
int parsecd(char* vt);

int myECM(MemT in,char *ip,int port,char*pout);

int WriteToLogFile(char*clkno,char *msgtype,int id,const char* msg);
char* myTrim(char* s);

#endif
