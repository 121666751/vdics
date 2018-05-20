/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCHdpMDL_H__016C1D14_C05B_4CDA_9BA8_3C8C36B31617__INCLUDED_)
#define AFX_TCHdpMDL_H__016C1D14_C05B_4CDA_9BA8_3C8C36B31617__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_COLOR_SCAN_SPEED			30			
#define DEFAULT_GRAY_SCAN_SPEED				180

struct MDLDATA;

class TCHdpMDL  
{
public:
	void setFlagStop();
	int  decBankPtr();
	int  Endorse(char *str,int on=1);
	int  SaveImage(int index,char *fname,char* rname);
	void SetMainWnd(HWND hwnd);
	TCHdpMDL();
	virtual ~TCHdpMDL();

	bool 
	InitDlls(void (*post_proc)(int num),void (*post_err)(int condition));					//Load the Libraries

	void
	FreeDlls();									//Free the libraries

	int
	Open();										//Connect with HDP sorter

	int
	Close();									//Disconnect with HDP sorter

	int
	Start(int mode, int speed, bool initflag);	//Start Sort with HDP sorter

	int
	Stop();										//Stop the HDP sorter

	int
	Purge();									//Clear the current voucher in the HDP sorter

	int
	BaseSet();									//Set the base color of the HDP sorter

	
	int 
	GetMicr(int num, char *micr);				//Get MICR code from the HDP sorter
	
	int 
	GetOcr(int num, char *ocr);					//Get OCR code from the HDP sorter

	int 
	SetPocket(int pocket);						//Set the sort pocket of the HDP sorter

	int AdSet(int mode, int speed);
protected:
	bool IsGoodMicr(const char* mir,const char*ocr);
	int GetBankHead();							//Get BankHead from the HDP sorter
	int OnHDPCall(int num);
	void OnHDPErr(int condition);
	int  _mode;									//control scan mode : 3--color or 1--gray
	int  _speed;								// scan speed
};

extern	MDLDATA *theData;

void OnHDPErr(int condition);

#endif // !defined(AFX_TCHdpMDL_H__016C1D14_C05B_4CDA_9BA8_3C8C36B31617__INCLUDED_)
