/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */

#ifndef __SEALGROUPUTIL_H__
#define __SEALGROUPUTIL_H__

#include <map>
#include <string>
#include "sealproc.h"
#include "stampgroupchecker.h"
#include "combination.h"

using namespace std;

enum NumberType{
	NORMAL_TYPE = 0,		// 普通数字类型，如 23
	SET_TYPE	= 1,		// 集合类型,如 {1,2,3}
};


bool range_compare(double val, double base, bool isClosedEdge, bool isBiggerType);
bool range_included_check(pair<double,double> range, pair<bool,bool> edgeFlag, const RangeT& base);
bool range_overlapped_check(pair<double,double> range, pair<bool,bool> edgeFlag, RangeT& base);
bool rule_check_ruleset(RuleItem& stampfound, string rulestr);
bool rule_check_totalcount(int totalCount, string rule);
bool ruleitem_include(const RuleItem& bigrule, const RuleItem& smallrule );


void addopt_container(MoneyRuleT);
void range_decode_fromstr(string s, pair<double,double>& range, pair<bool,bool>& edgeFlag);

RuleItem add_ri_opt(RuleItem ri1,RuleItem ri2);
RuleItem make_ruleitem(int totalCount);
RuleItem make_ruleitem(string s);
RuleItem make_total_ruleitem(string str);

RuleSet add_opt(RuleSet va, RuleSet vb);
RuleSet comb_opt(RuleSet rs, RuleSet fetchcounter);
RuleSet make_ruleset(string s);
RuleSet or_opt(RuleSet rs1, RuleSet rs2);
RuleSet parse_queue(queue<string>& qs);
bool rule_check_explain(queue<string> qs,string& outstr);
string ruleset_to_str(const RuleSet& rules);
string ruleitem_to_str(const RuleSet& rules);


queue<string> check_queue(vector<string> opt_array);


/** 
* 将规则串切割成操作数和操作符队列。
* 例如 "{1-3}:2+4;5" => {1-3} : 2 + 4 ; 5
*/
queue<string> split_rules(string rulestr);


#define OP_GLEFT	"{"
#define OP_GRIGHT	"}"
#define OP_FETCH	":"
#define OP_AND		"+"
#define OP_OR		";"
#define OP_DELIM	","

#define DEFAULT_CURRENCY	"NUL"
#endif // __SEALGROUPUTIL_H__
