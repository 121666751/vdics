/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 �����촴������Ϣ�Ƽ����޹�˾    ��Ȩ����
 *
 * ���ļ�Ϊ�촴���������Ʒ��һ���֡�
 * �촴���������Ʒ�����в���,�����������������Ʒ���������κ�ͼ����Ƭ��
 * ���������ֺ͸��ӳ���applets�������ܿ����渽��ӡˢ���ϡ�Դ�����ļ���
 * �������Ʒ���κθ�����һ������Ȩ��֪ʶ��Ȩ�������Ȩ���촴����ӵ�С�
 *
 * �ļ���	$Id: HDPCtrlUI.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com
 * ���䣺   support@tchzt.com
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
