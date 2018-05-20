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
 * �ļ���	$Id: SplitDc.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
 *
 ***************************************************************************
 */
#ifndef __SPLITDC_H__
#define __SPLITDC_H__

#include <string>
#include <map>
#include "main.h"

using namespace std;

struct DcInfo
{
	string vchno;
	string accno;
	string exchno;
	string money;
	string vchtp;
};

enum CodeType
{
	CT_VCHNO,
	CT_EXCHNO,
	CT_ACCNO,
	CT_VCHTP,
	CT_MONEY,
};

typedef map< string, CodeType, less<string> > CodeMap;
typedef CodeMap::value_type value_t;

class SplitDc
{
public:
	SplitDc();
    int init(DISTINFO &distinf);
	int split(string code, DcInfo& result);

private:
	void setInfo(string& val, CodeType ct, DcInfo& info);
	bool isSplitChar(char ch);
	CodeMap _typeMap;
};
//---------------------------------------------------------------------------
#endif //__SPLITDC_H__