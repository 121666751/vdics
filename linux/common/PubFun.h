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


//�ַ�����ת������
int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen);
// �ַ�����غ�������

// ɾ���ַ������ҿո�
char* Trim(char* s);

bool isNumeric(char *str);

// ɾ���ַ����е�����
char* DelBracket(char* s);

// �ַ���ת��д
char* to_upper(char* str);

// �ַ���תСд
char* to_lower(char* str);

//���ҽ���
int find_pid_by_name(char* pidName, int &first_pid);

/*
*/
void init_dameon();

/*
 *�鱨��ͷ
 */
string makeHead(char* mode);

/*
 *��������������
 */
string makeTaskBody(distTaskT body);

/*
 *��������������
 */
string makeTaskBodyEx(distTaskT body,string acc,string vchno,string amt);

string makeTaskBodyEx1(distTaskT body, string banktype, string elpId, string uplCnt,string uplAmt);

string makeTaskBodyEx2(distTaskT body, string elpId, string imgId);
/*
 *ɾ���ļ�
 */
int deleteFiles(FileVector Files);

/*
 *���̽���
 */
int doTrade(const char* send,const char *ip,const char* port,int & result,char*out,char*outstr);

/*
 *�������ݱ���
 */
string makeDataBody(distDataT body);
/*
 *�����������
 */
KeyMap parseOutXml(string xmlstr);

/*
 *����ECM����
 */
string makeXml(KeyMap keys,FileVector Files);

/*
 *ECMͨѶ
 */
int myECM(MemT in,const char *ip,const char* port,char*pout);

int utf8togb2312(const char *src, int src_len, char *destbuf, int dest_len);
int gb2312toutf8(const char *src, int src_len, char *destbuf, int dest_len);

#endif //__PUBFUN_H__
