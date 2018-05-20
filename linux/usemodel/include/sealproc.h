/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __SEALPROC_H__
#define __SEALPROC_H__

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <set>
#include "ZTSExcept.h"
#include <til_seal_struct.h>
#include <tilseal_202.h>
#include "mem.h"
#include "imaget.h"

using namespace std;
using namespace boost;

/**
 ӡ����Ͻڵ�
 */
struct CombItemT {
	string currency;	///< ������Ϣ
	string range;		///< ��Χ
	string moneyRule;	///< ӡ����Ϲ���
};

typedef vector<CombItemT> CombList;


/**
 ӡ����ϼ�飬����ӡ��ʶ��ʱ���н�����Ȩ�޵���֤��
 @param list ӡ������ӡ���������
 @param money Ʊ�ݽ��
 @param currency ������Ϣ
 @param sealsFound ��Ʊ���Ͼ���֤�ϸ��ӡ������
 @param usedList [out]ϵͳ�ڽ���ӡ������б�ʱ����ʹ�õ���Ч�������
 	ӡ������б�ͨ������˴�����ʵ�������б�Ĺ��������Χ��Ч��
 	��ϲ�ͨ�����򷵻���С��Ч��Χ����ϻ�ա�

 @return true-ͨ����false-��ͨ����
 */
bool comb_check(const CombList& list,
				const string& money,
				const string& currency,
				const set<int>& sealsFound,
				CombList& usedList );

CombList convert_customgroup(LPZIL_CUSTOMSTAMPGROUP pCustomGroup, int rows, LPZIL_GROUPALPHABET pAlphabet);

MemT Groups2Mem(CombList v);
CombList	Mem2Groups(MemT lib);

void TilMem2Stamp(MemT image,MemT &stamp,MemT &groups);

ImageT tilstamp2ImageT(TIL_ID id,int selPos);


#endif // __SEALPROC_H__
