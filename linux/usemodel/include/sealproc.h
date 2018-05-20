/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
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
 印鉴组合节点
 */
struct CombItemT {
	string currency;	///< 币种信息
	string range;		///< 金额范围
	string moneyRule;	///< 印鉴组合规则
};

typedef vector<CombItemT> CombList;


/**
 印鉴组合检查，用于印鉴识别时进行金额组合权限的验证。
 @param list 印鉴库中印鉴组合链表
 @param money 票据金额
 @param currency 币种信息
 @param sealsFound 在票据上经验证合格的印鉴集合
 @param usedList [out]系统在进行印鉴组合判别时真正使用的有效规则。如果
 	印鉴组合判别通过，则此处返回实际用来判别的规则，如果金额范围有效但
 	组合不通过，则返回最小有效金额范围的组合或空。

 @return true-通过，false-不通过。
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
