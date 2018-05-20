/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef DSTDATA_H_INCLUDED_C40DF80C
#define DSTDATA_H_INCLUDED_C40DF80C

#include "IniRead.h"
#include <windows.h>
#include <zcommon.h>
struct DISTDATA;

class MyList;


using namespace std ;


class MyDb;
//##ModelId=3BF0D00F00C7
class dstData
{
  public:
	  	  
	  int StoreData(DISTDATA *data);
	  int CalTotalAmount(double& dramount,int& drcount, double& cramount, int& crcount);
	  int DelRightItems(int rightIndex);
	  int DeleteCurrent();
	  int GetRightItems(DISTDATA* pData, int* count);
	  int Currency;
	  void StoreList(HWND hWnd);
	  bool MicrOK;
	  int GetPocketNum(int mode);
	  bool SetCodeData(char* codeline,bool micr=true);
	  int OpenDb();
	  int StoreState(int mode);
    
	  dstData();
	virtual ~dstData();

    //##ModelId=3BF0D4E701CE
    int Initialize(int nDstMode,int currency, int netno=0, int vch_tc=12, bool isTallyMode=true);


	DISTDATA *pData;                        ///////////
	DISTDATA *pOcrData;						//////////
    

	MyList *pList;                         //////////DistDataList
	bool bReDist;

protected:
	int SaveSecondResult();
	int SaveFirstResult();
	int StoreData_Third(DISTDATA *data);
	int SaveThirdResult();
	int GetIDResult();
	int SaveResult(int mode);
	void GetVchKindNum(DISTDATA *data);
	bool chkMicrCode();
	bool setCode( DISTDATA* mdata, DISTDATA* odata);
	void clearData();
	int  getPocEx(int mode);
private:
	bool m_isTallyMode;
	int DstMode;
	int m_vchTc;
	MyDb* TheMyDb;

	PocketMap   pocMap;
	PocketMap   desMap;
	PocketMap   cdMap;

	PocketMap   vpMap;
};



#endif /* DSTDATA_H_INCLUDED_C40DF80C */
