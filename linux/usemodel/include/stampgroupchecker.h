/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
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
	
	�ж������Ƿ�ͱ���rht�غϡ�
	���ȣ���������������rht���򷵻�false�����򣬷����ж��Ƿ����������rht�Ƿ����غ�����
	������غ������򷵻�true�����򣬷���false��
	
	�����������rht���жϷ�����
	����±߽�ֵ���־��ȫ��ͬ�����������rhtû�е�ֵ���򷵻������������rht�����򷵻�������������rht��
	������
	RangeT a="[10-100]"
	a.isOverlapped(RangeT("[10-20]"))�ķ���ֵΪfalse��
	a.isOverlapped(RangeT("[10-100)"))�ķ���ֵΪfalse��
	a.isOverlapped(RangeT("[10-100]"))�ķ���ֵΪtrue��
	a.isOverlapped(RangeT("[10-200]"))�ķ���ֵΪtrue��
	a.isOverlapped(RangeT("(10-20]"))�ķ���ֵΪtrue��
	a.isOverlapped(RangeT("(10-100]"))�ķ���ֵΪtrue��
	a.isOverlapped(RangeT("(10-200]"))�ķ���ֵΪture��
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
ӡ���������
_rules �����з��Ϲ����ӡ���Ĵ���
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
