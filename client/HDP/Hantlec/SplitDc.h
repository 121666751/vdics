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
 * 文件：	$Id: SplitDc.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
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