#ifndef __PUBFUN_H__
#define __PUBFUN_H__
//////////////////////////////////////////////////////////////////////////


#include <map>
#include <vector>
#include <mem.h>
#include "gsipcom.h"

#define ZM_SCAN_TASK        "451600"
#define ZM_DIST_TASK        "451601"
#define ZM_DIST_TASKEX      "451603"
#define ZM_DIST_DATA        "556114"
#define ZM_DIST_RTCS        "556503"
#define ZM_DIST_BRNO        "310200"
#define ZM_DIST_AREA        "310200"

using namespace std;


//字符编码转换函数
int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen);
// 字符串相关函数声明

// 删除字符串左右空格
char* Trim(char* s);

bool isNumeric(char *str);

// 删除字符串中的括号
char* DelBracket(char* s);

// 字符串转大写
char* to_upper(char* str);

// 字符串转小写
char* to_lower(char* str);

//查找进程
int find_pid_by_name(char* pidName, int &first_pid);

/*
*/
void init_dameon();

/*
 *组报文头
 */
string makeHead(char* mode);

/*
 *组流程任务报文体
 */
string makeTaskBody(distTaskT body);

/*
 *组流程任务报文体
 */
string makeTaskBodyEx(distTaskT body,string acc,string vchno,string amt);

string makeTaskBodyEx1(distTaskT body, string banktype, string elpId, string uplCnt,string uplAmt);

string makeTaskBodyEx2(distTaskT body, string elpId, string imgId);
/*
 *删除文件
 */
int deleteFiles(FileVector Files);

/*
 *流程交易
 */
int doTrade(const char* send,const char *ip,const char* port,int & result,char*out,char*outstr);

/*
 *创建数据报文
 */
string makeDataBody(distDataT body);
/*
 *解析输出报文
 */
KeyMap parseOutXml(string xmlstr);

/*
 *创建ECM报文
 */
string makeXml(KeyMap keys,FileVector Files);

/*
 *ECM通讯
 */
int myECM(MemT in,const char *ip,const char* port,char*pout);

int utf8togb2312(const char *src, int src_len, char *destbuf, int dest_len);
int gb2312toutf8(const char *src, int src_len, char *destbuf, int dest_len);

#endif //__PUBFUN_H__
