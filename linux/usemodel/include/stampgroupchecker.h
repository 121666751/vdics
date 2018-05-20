/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __STAMPGROUPCHECKER_H__
#define __STAMPGROUPCHECKER_H__

#include "sealproc.h"
#include <queue>

typedef set<int> RuleItem;
typedef set< set<int> > RuleSet;


class RangeT
{
public:
	typedef pair<double,double>	ValueT;
	typedef pair<bool,bool>		FlagT;


	/**
	 \code
	 RangeT a="[0-1000]"		// 0 <= a <= 1000
	 RangeT b="(0-1000)"		// 0 < b < 1000
	 RangeT c="[200-1000)"		// 200 <= c < 1000
	 RangeT d="(200-1000]"		// 200 < c <= 1000
	 RangeT e="[0-*]"			// e >= 0
	 */
	RangeT(string rangestr);
	RangeT(){};
	~RangeT(){};
	void Load(string s);
	/**
	 @param val		
	 @return true-Yes, false-No
	 */
	bool isInside(const double val) const;
	bool isInclude(const RangeT&) const;
	/**
	 @param rht: the destination value.
	 @return true-Yes, false-No
	
	判断自身是否和变量rht重合。
	首先，如果自身包含变量rht，则返回false，否则，否则判断是否自身与变量rht是否有重合区域。
	如果有重合区域，则返回true，否则，返回false。
	
	自身包含变量rht的判断方法：
	如果下边界值与标志完全相同，且自身包含rht没有的值，则返回自身包含变量rht，否则返回自身不包含变量rht。
	举例：
	RangeT a="[10-100]"
	a.isOverlapped(RangeT("[10-20]"))的返回值为false；
	a.isOverlapped(RangeT("[10-100)"))的返回值为false；
	a.isOverlapped(RangeT("[10-100]"))的返回值为true；
	a.isOverlapped(RangeT("[10-200]"))的返回值为true；
	a.isOverlapped(RangeT("(10-20]"))的返回值为true；
	a.isOverlapped(RangeT("(10-100]"))的返回值为true；
	a.isOverlapped(RangeT("(10-200]"))的返回值为ture。
	*/
	bool isOverlapped(RangeT& rht);

	bool operator == (const RangeT& Ri) const
	{
		if(_range == Ri.getRange() && _edgeFlag == Ri.getEdgeFlag())
			return true;
		else
			return false;
	}

	ValueT getRange() const	
	{	return _range;	}

	FlagT getEdgeFlag() const 
	{	return _edgeFlag;	}

protected:
	ValueT		_range;			
	FlagT		_edgeFlag;	
};

/**
印鉴规则的类
_rules 是所有符合规则的印鉴的搭配
*/
class MoneyRuleT
{
public:
	MoneyRuleT(){};
	MoneyRuleT(string rulestr);
	void Load(string rulestr);
	RuleSet getRules() const 
	{	return _rules; }

	bool IsSubRule(RuleItem ri);

	bool operator == (const MoneyRuleT& ri) const {
		if(_rules == ri.getRules())
			return true;
		else
			return false;
	}

protected:
	RuleSet _rules;
};

class CombCreatorT : public CombList
{
public:
	CombCreatorT(int totalSealCount) : _total(totalSealCount) {}
	void add( CombItemT& it ) {

	}

	void add( string range, string rule, string currency) {
		CombItemT it = {currency,range,rule};
	}

	CombList getAllRules() { 
		return static_cast<CombList>(*this);
	}

	int _total;
};

class CombCheckerT
{
public:
	CombCheckerT(CombList& combs) :_combs(combs) {}

	bool check(string currency, double money, RuleItem sealFound) {
		char buf[1000];
		sprintf(buf,"%4.2f",money);
		CombList used;
		return comb_check(_combs, buf, currency, sealFound, used);
	}

	CombList _combs;
};

#endif //__STAMPGROUPCHECKER_H__
