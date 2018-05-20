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
 * �ļ���	$Id: HashSearch.h,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * �汾��	$Revision: 1.2 $
 * ��ַ��   www.tchzt.com.cn
 * ���䣺   support@tchzt.com.cn
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