/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 北京天创征腾信息科技有限公司    版权所有
 *
 * 此文件为天创征腾软件产品的一部分。
 * 天创征腾软件产品的所有部分,包括但不限于软件产品中所含的任何图象、照片、
 * 动画、文字和附加程序（applets）、加密卡、随附的印刷材料、源代码文件及
 * 本软件产品的任何副本的一切所有权和知识产权，均归版权人天创征腾拥有。
 *
 * 文件：	$Id: cbxctrl.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
---------------------------------------------------------------

#ifndef cbxctrlH
#define cbxctrlH

struct CBXDATA;

class CbxCtrl
{
private:
	CBXDATA *theData;
public:
	CbxCtrl();
	~CbxCtrl();
    
	/************************************************************
    Function Init()
    Comment: Initialize the class and the comport.
     	handle 	- HWND,main window handle to recieve message.
        port    - int,comport no connect to cbx.
        baud	- int,baud rate of the comport.
    *************************************************************/
	int Init(HWND handle, int port = 1, int baud=9600);

	/************************************************************
     Function Bell()
     Comment: Let the Cbx 6000 to bell a sound.
     	type	- int,sound type.0:default type,duration omitted.
        						 1:tune mode.
                                 2:continues mode.
        duration- int,sound duration in miliseconds.
     ************************************************************/
	void Bell(int type = 0, int duration = 100);

	/************************************************************
     Function OnRecvCh()
     Comment: When main window get the recieve char message,call
    		  this function to treate the char.
     	ch		- char,recieved char from main window message.
     ************************************************************/
	void OnRecvCh(char ch);

	/************************************************************
     Function StartFeed()
     Comment: Start the process of sort.
     	hoper_first	- bool,determine if firstly run from hoper or
        			  hand drop.
        single		- bool,determine if just run from the hoper or
         			  hand drop.
     ************************************************************/
	void StartFeed(bool hfirst = false, bool single=false);

	/************************************************************
     Function StopFeed()
     Comment: Stop the process of sort.
     ************************************************************/
	void StopFeed();

	/************************************************************
     Function RecoverCBX()
     Comment: When the cbx6000 in error,call the function.
     ************************************************************/
	void RecoverCBX();

	/************************************************************
     Function GetCBXState()
     Comment: To get the state of the cbx6000.
     ************************************************************/
	void GetCBXState();

	/************************************************************
     Function CheckCover()
     Comment: To check the covers of the cbx6000.
     ************************************************************/
	void CheckCover();

	/************************************************************
     Function CheckPocket()
     Comment: To check the pockets full of the cbx6000.
     ************************************************************/
	void CheckPocket();
	void runCmd(char * cmd);
	void InitPara();
	void Sort(int poc);
	void Flush();
	void Endorse(char * text);
    void ChangeMode(bool needstop);
    bool Initiallized();

protected:
	/************************************************************
     Function OnRecvCh()
     Comment: When main window get a whole line from the Cbx6000,
     		  call this function to treate the line.
     ************************************************************/
	void OnRecvLine();
	void TreatLine();
	void treatState();
	void treatCode();
	void treatErr();
	void treatChk();
	void startFeedEx();
	void stopFeedEx();
	void getCode(char * &p, char * buf);
	void initParaEx();
    void Outline(char * msg);
    void AnalyzeState(char * state);
	void FormatCode(char * buf);
	bool IsSymbol(char a);
};
//---------------------------------------------------------------------------
#endif
