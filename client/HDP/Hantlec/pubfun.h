#ifndef PUBFUN_H
#define PUBFUN_H
#include <vcl.h>

#include <map>
#include <vector>
#include <zcommon.h>

#define ZM_DIST_TASK        "451601"
#define ZM_DIST_DATA        "556114"
#define ZM_DIST_SIGN        "556504"
#define ZM_DIST_RTCS        "556503"
#define ZM_DIST_BRNO        "310200"
#define ZM_DIST_AREA        "310200"
using namespace std;

typedef map<String ,String> XmlMap;

String makeHead(char* mode);

String makeTaskBody(distTaskT body);

String makeHead(char* mode);

String makeSignBody(char*brno,char*area,char*accdate,char*session);

String makeTaskBody(distTaskT body);

char* pvb( int vt,char*  bt);

int doTrade(char* send,char *ip,int port,int & result,char*out,char*outstr);

String makeDataBody(distDataT body);
XmlMap parseOutXml(String xmlstr);
String makeXml(XmlMap keys,FileVector Files);
int parsecd(char* vt);

#endif
