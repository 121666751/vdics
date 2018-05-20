#ifndef __zdev_file_hpp__
#define __zdev_file_hpp__

#include <sys/stat.h>
#include "zbase.hpp"
#include "zstring.hpp"

ZDEV_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// ǰ������
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
// ��������

// �����ļ���С, -1 ��ʾ�ļ�������
inline int file_size(string filename) {
	struct stat info;
	int err = stat(filename.c_str(), &info);  
	return (err) ? -1: info.st_size;
}
// �����ļ��Ƿ����
inline bool file_exists(string filename) {
	return file_size(filename) != -1;
}
// ���������ļ�
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
// ���ı��ļ�
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
// д�������ļ�
inline void file_bianry_write(string filename, char *data, int size) {
	ofstream fout(filename.c_str(), ios::binary);
	fout.write(data, size);
	fout.close();
}
// д�������ļ�
inline void file_bianry_write(string filename, vector<byte> data) {
	ofstream fout(filename.c_str(), ios::binary);
	fout.write((char *)&data[0], data.size());
	fout.close();
}
// д�ı��ļ�
inline void file_text_write(string filename, string data) {
	ofstream fout(filename.c_str(), ios::binary);
	fout.write(data.c_str(), data.size());
	fout.close();
}
// �����ļ����ڵ�Ŀ¼
string file_parent_path(string filename) {
	int n = filename.find_last_of("\\");
	if (n > 0) {
		return filename.substr(0, n);
	} else {
		return "";
	}
}
// ���ݸ�·��������ļ��������ļ�����
string file_combo_path(string parent, string filename) {
	if (str_endsWith(parent, "\\")) {
		return parent + filename;
	} else {
		return parent + "\\" + filename;
	}
}
ZDEV_NAMESPACE_END

#endif //__zdev_file_hpp__
