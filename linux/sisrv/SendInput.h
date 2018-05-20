
#ifndef _LINUX_SENDINPUT_H
#define _LINUX_SENDINPUT_H

#include <map>
#include <string>
#include <../dbolayer/Dboperate.h>
#include <../common/gsipcom.h>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// 常量定义
//////////////////////////////////////////////////////////////////////////
// 运算类
class CSENDC
{
public:
	CSENDV();
	~CSENDV();

	// 开始上传
	void Start(int verbose);

	//保存影像ID
	int SaveImageID(int sid,const char*imageid);

private:
	//获取影像ID
	int getTaskId();
	int get_config();
private:
	char TaskId[16];
	char TaskIp[24];
	int  TaskPort;	
	char DbHost[24];
	char DbUser[24];
	char DbPswd[24];
	char DbName[24];
	char ImagePath[40];		
	char ImageID[80];
	
	CDboperate m_db;

	KeyMap   _cfgMap;
	
	KeyMap   _vtMap;
	
	KeyMap   _areaMap;
	
	scanInfoT _scanNode;
	distTaskT  disttask;
    distDataT  distdata;   
	

};

#endif //_LINUX_SENDINPUT_H
