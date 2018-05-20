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
 * 文件：	$Id: HDPCtrlUI.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com
 * 邮箱：   support@tchzt.com
 *
 ***************************************************************************
 */
//-----------------------------------------------------------------

#ifndef HDPCtrlUIH
#define HDPCtrlUIH
//---------------------------------------------------------------------------
struct VchStuff
{
    char netNo[10];
    char vchNo[9];
    char vchType[3];
    char accNo[28];
    char amount[16];
};

struct HDPDATA
{
    HINSTANCE       hInstDll;
    int speed;
    int wide;
    char micr_buf[256];
    char ocr_buf[256];
    VchStuff theVch;
    HDPDATA(){speed=200;wide=832;}
};
struct DISTDATA;

class HDPCtrlUI
{
private:
    
    HANDLE hHDP;
    int IsGoodMicr(const char* mir, const char* ocr);
    int _mode;

public:
    HDPDATA *theData;
    int Init();
    HDPCtrlUI();
    ~HDPCtrlUI();
    int Connect();
    void Free();
    int Disconnect();
    int Start(int mode, int speed, bool initflag);
    int Stop();
    int SetPocket(int poc);
    int Purge();
    int GetMicr(int num, char *fname, char *rname);
    int DecodeMicr(DISTDATA* pData);
    char CurrentPkgno[10];
    int OnEndorse( char * str, int pos);
    int SaveImage(int count,char * fname,char *rname);
};
//---------------------------------------------------------------------------
extern void OnHDP(int num);
extern void OnErrorHDP(int err);
//---------------------------------------------------------------------------
#endif
