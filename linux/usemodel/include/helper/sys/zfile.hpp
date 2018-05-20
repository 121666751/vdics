#ifndef __zdev_file_hpp__
#define __zdev_file_hpp__

#include <sys/stat.h>
#include "zbase.hpp"
#include "zstring.hpp"

ZDEV_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// 前向声明
/*
class FileHelper {
public:
	int					size(string filename);
	bool				exists(string filename);
	string				textRead(string filename);
	void				textWrite(string filename, string data);
	vector<byte>		binaryRead(string filename);
	void 				bianryWrite(string filename, char *data, int size);
	void				bianryWrite(string filename, vector<byte> data);
	string				getParentPath(string filename);
	string				getComboPath(string parent, string filename);
};
*/
int				file_size(string filename);
bool			file_exists(string filename);
vector<byte>	file_binary_read(string filename);
string			file_text_read(string filename);
void 			file_bianry_write(string filename, char *data, int size);
void			file_bianry_write(string filename, vector<byte> data);
void			file_text_write(string filename, string data);
string			file_parent_path(string filename);
string			file_combo_path(string parent, string filename);

//////////////////////////////////////////////////////////////////////////
// 函数定义

// 返回文件大小, -1 表示文件不存在
inline int file_size(string filename) {
	struct stat info;
	int err = stat(filename.c_str(), &info);  
	return (err) ? -1: info.st_size;
}
// 返回文件是否存在
inline bool file_exists(string filename) {
	return file_size(filename) != -1;
}
// 读两进制文件
inline vector<byte> file_binary_read(string filename) {
	int size = file_size(filename);
	vector<byte> buffer;
	if (size < 0) return buffer;
	buffer.resize(size);
	ifstream fin(filename.c_str(), ios::binary);
	fin.read((char *)&buffer[0], size);
	fin.close();
	return buffer;
}
// 读文本文件
inline string file_text_read(string filename) {
	int size = file_size(filename);
	string buffer;
	if (size < 0) return buffer;
	buffer.resize(size);
	ifstream fin(filename.c_str(), ios::binary);
	fin.read(&buffer[0], size);
	fin.close();
	return buffer;
}
// 写两进制文件
inline void file_bianry_write(string filename, char *data, int size) {
	ofstream fout(filename.c_str(), ios::binary);
	fout.write(data, size);
	fout.close();
}
// 写两进制文件
inline void file_bianry_write(string filename, vector<byte> data) {
	ofstream fout(filename.c_str(), ios::binary);
	fout.write((char *)&data[0], data.size());
	fout.close();
}
// 写文本文件
inline void file_text_write(string filename, string data) {
	ofstream fout(filename.c_str(), ios::binary);
	fout.write(data.c_str(), data.size());
	fout.close();
}
// 返回文件所在的目录
string file_parent_path(string filename) {
	int n = filename.find_last_of("\\");
	if (n > 0) {
		return filename.substr(0, n);
	} else {
		return "";
	}
}
// 根据父路径和相对文件名返回文件名称
string file_combo_path(string parent, string filename) {
	if (str_endsWith(parent, "\\")) {
		return parent + filename;
	} else {
		return parent + "\\" + filename;
	}
}
ZDEV_NAMESPACE_END

#endif //__zdev_file_hpp__
