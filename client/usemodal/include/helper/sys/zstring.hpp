#ifndef __zdev_string_hpp__
#define __zdev_string_hpp__

#include "zbase.hpp"

ZDEV_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// 前向声明
vector<string>	str_split(string str, string dim);
string			str_trim(string str);
string			str_toUpper(string str);
string			str_toLower(string str);
bool			str_equalsIC(string s1, string s2);
bool			str_startsWith(string str, string prefix);
bool			str_startsWithIC(string str, string prefix);
bool			str_endsWith(string str, string suffix);
bool			str_endsWithIC(string str, string suffix);
string          str_fromInt(int n);
string          str_fromFloat(float f);
int             str_toInt(string str);
float           str_toFloat(string str);

//////////////////////////////////////////////////////////////////////////
// 函数定义

// 按照指定的分割符，分割字符串
inline vector<string> str_split(string str, string dim) {
	vector<string> result;
	do {
		int pos = str.find(dim);
		if (pos <= 0) break;
		result.push_back(str.substr(0, pos));
		str = str.substr(pos + dim.length());
	} while (1);
	result.push_back(str);
	return result;
}
// 删除字符串前后的空格和回车换行字符
inline string str_trim(string str) {
	int i = 0;
	while (isspace(str[i])) i++;
	int n = str.length() - 1;
	while (isspace(str[n])) n--;
	if (i>n) return "";
	return str.substr(i, n - i + 1);
}
// 转换字符串为大写
inline string str_toUpper(string str) {
	string dst = str;
	//std::transform(str.begin(), str.end(),
	//	std::insert_iterator<string>(dst, dst.begin()), toupper);
	for (uint i=0; i<str.size(); i++) {
	    dst[i] = toupper(str[i]);
	}
	return dst;
}
// 转换字符串为小写
inline string str_toLower(string str) {
	string dst = str;
	//std::transform(str.begin(), str.end(),
	//	std::insert_iterator<string>(dst, dst.begin()), tolower);
	for (uint i=0; i<str.size(); i++) {
	    dst[i] = tolower(str[i]);
	}
	return dst;
}
// 忽略大小写比较
inline bool str_equalsIC(string s1, string s2) {
	return str_toLower(s1) == str_toLower(s2);
}
// 判断字符串开头
inline bool str_startsWith(string str, string prefix) {
	return (str.find(prefix)) == 0;
}
// 判断字符串开头, 忽略大小写
inline bool str_startsWithIC(string str, string prefix) {
	return (str_toLower(str).find(str_toLower(prefix))) == 0;
}
// 判断字符串结尾
inline bool str_endsWith(string str, string suffix) {
	return (str.length() - str.rfind(suffix)) == suffix.length();
}
// 判断字符串结尾, 忽略大小写
inline bool str_endsWithIC(string str, string suffix) {
	return (str.length() - str_toLower(str).rfind(str_toLower(suffix))) == suffix.length();
}

inline string str_fromInt(int n) {
    ostringstream oss;
    oss << n;
    return oss.str();
}

inline string str_fromFloat(float f) {
    ostringstream oss;
    oss << f;
    return oss.str();
}

inline int str_toInt(string str) {
    int n;
    istringstream iss;
    iss >> n;
    return n;
}

inline float str_toFloat(string str) {
    float f;
    istringstream iss;
    iss >> f;
    return f;    
}


ZDEV_NAMESPACE_END

#endif //__zdev_string_hpp__
