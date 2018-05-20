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
 * 文件：	$Id: HashSearch.cpp,v 1.2 2008/03/30 09:51:03 Wu Exp $
 * 版本：	$Revision: 1.2 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */
//////////////////////////////////////////////////////////////
//	HashSearch.cpp
//	construct the hash talbe and search one record
//								2001-12-18
///////////////////////////////////////////////////////////////////////

#include "HashSearch.h"
#include <memory.h>
#include <stdlib.h>
#include <math.h>

int Process_Collision(vtMap k);
int Hash_Value(LPHashTable pHTable, vtMap k);


LPHashTable InitHashTable(unsigned long ItemAmount)
{
	LPHashTable p;
	p = new HashTable;
	p->Table = new HashNode[ItemAmount];
	p->ItemAmount = ItemAmount;
	memset(p->Table, 0, ItemAmount*sizeof(HashNode));

	return p;
}

int Process_Collision(vtMap k)
{
	return 0;
}

int Hash_Value(LPHashTable pHTable, vtMap *k)
{
    int value = atoi(k->bankvt);

	return value;
}

int CreateHashTable(LPHashTable pHTable, vtMap *k)
{
	if( pHTable->ItemAmount == 0 ) return 2;

	int idx = Hash_Value(pHTable, k);
	HashNode *p = &(pHTable->Table[idx]);
	if( p->key == 0 ){
		p->key = 1;
		memcpy(&p->Item, k, sizeof(vtMap));
	}
	else{
		HashNode *node = new HashNode;
		node->pNext = p->pNext;
		node->key = 1;
		memcpy(&node->Item, k, sizeof(vtMap));
		p->pNext = node;
	}

	return 0;
}

int HashSearch(LPHashTable pHTable, vtMap *k)
{
	if( pHTable->ItemAmount == 0 ) return 2;

	int idx = Hash_Value(pHTable, k);
	HashNode *p = &(pHTable->Table[idx]);
	while( p != NULL ){
		if( p->Item.VchKind == k->VchKind ){
			if( memcmp(p->Item.VchNo, k->VchNo, VOUCHER_LEN) == 0 ){
				if( memcmp(p->Item.Account, k->Account, ACCOUNT_LEN) == 0 ){
					if( memcmp(p->Item.Money, k->Money, MONEY_LEN) == 0 ){
						memcpy(k, &p->Item, sizeof(IDResDistItem));
						//k->DistSign = p->Item.DistSign;
						p->Item.DistSign = 1;
						return 0;
					}
					else p = p->pNext;
				}
				else p = p->pNext;
			}
			else p = p->pNext;
		}
		else p = p->pNext;
	}
	return 1;
}

int FreeHashTable(LPHashTable pHTable)
{
	unsigned long ItemAmount = pHTable->ItemAmount;
	if( ItemAmount == 0 ) return 1;
	for(int i = 0; i < (int)ItemAmount; i++){
		HashNode *p = pHTable->Table[i].pNext;
		while(p!=NULL){
			HashNode *node = p->pNext;
			delete p;
			p = node;
		}
	}
	delete[] pHTable->Table;

	return 0;
}
