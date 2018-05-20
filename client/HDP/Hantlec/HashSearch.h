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
 * 文件：	$Id: HashSearch.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
ASHSEARCH_H__
#define __HASHSEARCH_H__

#include "main.h"


struct HashNode{
	unsigned char key;
	vtMap Item;
	HashNode *pNext;
};

struct HashTab{
	HashNode *Table;
	unsigned long ItemAmount;
};

typedef HashTab HashTable, *LPHashTable;


LPHashTable InitHashTable(unsigned long ItemAmount);
int CreateHashTable(LPHashTable pHTable, vtMap *k);
int HashSearch(LPHashTable pHTable, vtMap *k);
int FreeHashTable(LPHashTable pHTable);


#endif //__HASHSEARCH_H__