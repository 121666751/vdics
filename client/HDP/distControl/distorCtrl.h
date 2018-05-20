/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef TORCTRL_H_INCLUDED_C40DAD6B
#define DISTORCTRL_H_INCLUDED_C40DAD6B

class dstData;
struct DISTDATA;
struct DISTCTRLDATA;

//##ModelId=3BF0CEC40125
class distorCtrl
{
public:
	int Start(DISTCTRLDATA* pDcd);
	int StoreTotal( int currency, double sum1, double sum2, int jd);
	bool SetDistData(DISTDATA *pData);
	int  GetPocketNum(bool isEndorse = false);
	bool SetCodeData(char* codeline,bool micr=true);
	int  StoreData();              
	distorCtrl();
	virtual ~distorCtrl();
	
	int Init();
	HWND hMainWnd;
    //##ModelId=3BF0D4D30220
    int Start(int nDstMode, int nDstState,int currency=1, int netno=0, int vch_tc=12, bool isTallyMode=true);
	

    int End();
	

    int goOn();

    void OnDist(int pocket);

    int Pause();
	
	DISTDATA* GetDistData();
	dstData *pDstData;
	
private:
	int DstState;
	int DstMode;
	int lastNetNo;
	bool bEndorse;
protected:
	//int InitSorter();
};



#endif /* DISTORCTRL_H_INCLUDED_C40DAD6B */
