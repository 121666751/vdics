
#ifndef _LINUX_SENDVOUCHERS_H
#define _LINUX_SENDVOUCHERS_H

#include <map>
#include <string>
#include <../dbolayer/Dboperate.h>
#include <../common/gsipcom.h>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////
// ������
class CSENDV
{
public:
	CSENDV();
	~CSENDV();

	// ��ʼ�ϴ�
	void Start(int verbose);

	//����Ӱ��ID
	int SaveImageID(int sid,const char*imageid);

private:
	//��ȡӰ��ID
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

	KeyMap   _exnoMap;
		
	scanInfoT _scanNode;
	distTaskT  disttask;
    distDataT  distdata;   
	

};

#endif //__SENDVOUCHER_H__
