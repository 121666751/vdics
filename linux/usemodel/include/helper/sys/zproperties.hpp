#ifndef __zdev_properties_hpp__
#define __zdev_properties_hpp__

#include "zbase.hpp"
#include "zfile.hpp"
#include "zstring.hpp"

ZDEV_NAMESPACE_BEGIN


// 读取配置文件
class Properties {
private:
	std::map<std::string, std::string> properties;
public:
	Properties() {}
	Properties(std::string filename) { this->load(filename); }
	void load(std::string filename);
	std::string getProperty(std::string key);
	std::string setProperty(std::string key, std::string value);
	bool exists(std::string key);
	std::string getString(std::string key, std::string defaultValue="");
	int getInt(std::string key, int defaultValue=0);
	float getFloat(std::string key, float defaultValue=0.0);
	bool getBoolean(std::string key, bool defaultValue=false);
	std::vector<std::string> propertyNames();
};

//////////////////////////////////////////////////////////////////////////
inline std::string Properties::setProperty(std::string key, std::string value)
{
	std::string oldValue = properties[key];
	properties[key] = value;
	//tcout << ("setting property key=") << key << (", value=") << value << std::endl;
	return oldValue;
}

inline std::string Properties::getProperty(std::string key)
{
	std::map<std::string, std::string>::const_iterator it = properties.find(key);
	return (it != properties.end()) ? it->second : std::string();
}

/**
 * 返回 key 是否存在
 */
inline bool Properties::exists(std::string key) {
	std::map<std::string, std::string>::const_iterator it = properties.find(key);
	return it != properties.end();
}

inline std::string Properties::getString(std::string key, std::string defaultValue) {
	std::string v = this->getProperty(key);
	return (v == "") ? defaultValue : v;
}

inline int Properties::getInt(std::string key, int defaultValue) {
	std::string v = this->getProperty(key);
	return (v == "") ? defaultValue : atoi(v.c_str());
}

inline float Properties::getFloat(std::string key, float defaultValue) {
	std::string v = this->getProperty(key);
	return (v == "") ? defaultValue : (float)atof(v.c_str());
}

inline bool Properties::getBoolean(std::string key, bool defaultValue) {
	std::string v = this->getProperty(key);
	if (v == "") return defaultValue;
	if (strcmp(str_toLower(v).c_str(), "true") == 0) return true;
	if (strcmp(str_toLower(v).c_str(), "yes") == 0) return true;
	if (strcmp(str_toLower(v).c_str(), "on") == 0) return true;
	if (strcmp(v.c_str(), "1") == 0) return true;
	return false;
}

/**
 * 从文件中读入属性.
 * 以 # 开头的为注释， 以 \ 结尾的为续行
 * 不是 key=value 格式的行，会被忽略
 */
inline void Properties::load(std::string filename)
{
	properties.clear();
	std::string content = file_text_read(filename);
	std::vector<std::string> lines = str_split(content, "\n");
	for (int i=0; i<(int)lines.size(); i++) {
		std::string line = str_trim(lines[i]);
		if (line.size() > 0) {
			if (line[0] == '#') continue;	// 注释
			int n = line.find('=');
			if (n < 1) continue; // 不是 key=value 格式
			std::string key = str_trim(line.substr(0, n));
			std::string value = str_trim(line.substr(n+1));
			while (str_endsWith(value, "\\")) { // 续行
				if (++i < (int)lines.size()) {
					value = value.substr(0, value.size()-1) + str_trim(lines[i]);
				} else {
					break;
				}
			}
			properties[key] = value;
		}
	}
}

inline std::vector<std::string> Properties::propertyNames()
{
	std::vector<std::string> names;
	names.reserve(properties.size());
	std::map<std::string, std::string>::const_iterator it;
	for (it = properties.begin(); it != properties.end(); it++)	{
		const std::string& key = it->first;
		names.push_back(key);
	}
	return names;
};


ZDEV_NAMESPACE_END

#endif //__zdev_properties_hpp__