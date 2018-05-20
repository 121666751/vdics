#ifndef __zdev_win32_hpp__
#define __zdev_win32_hpp__

#include <io.h>

#include "../sys/zbase.hpp"
#include "../sys/zfile.hpp"

ZDEV_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
vector<string>	win32_file_dirs(string dir, string filespec);
int				win32_file_dirs_all(string dir, string filespec, vector<string> &files);


//////////////////////////////////////////////////////////////////////////
// 查找目录下的所有符合条件的文件（不包括子目录）
vector<string> win32_file_dirs(string dir, string filespec) {
	vector<string> files;
	struct _finddata_t c_file;
    long hFile;
	if( (hFile = _findfirst( (char *) file_combo_path(dir, filespec.c_str()).c_str(), &c_file )) != -1L ) {
		if (!(c_file.attrib & _A_SUBDIR)) {
			files.push_back(file_combo_path(dir, c_file.name));
		}
		while( _findnext( hFile, &c_file ) == 0 ) {
			if (!(c_file.attrib & _A_SUBDIR)) {
				files.push_back(file_combo_path(dir, c_file.name));
			}
		}
		_findclose( hFile );
	}
	return files;
}

// 查找目录下的所有符合条件的文件（包括子目录）
int win32_file_dirs_all(string dir, string filespec, vector<string> &files) {
	struct _finddata_t c_file;
    long hFile;
	// find sub folders
	if( (hFile = _findfirst( (char *) file_combo_path(dir, "*").c_str(), &c_file )) != -1L ) {
		if (c_file.attrib & _A_SUBDIR) {
			if (string(c_file.name) != "." && string(c_file.name) != "..")
				win32_file_dirs_all(file_combo_path(dir, c_file.name), filespec, files);
		}
		while( _findnext( hFile, &c_file ) == 0 ) {
			if (c_file.attrib & _A_SUBDIR) {
				if (string(c_file.name) != "." && string(c_file.name) != "..")
					win32_file_dirs_all(file_combo_path(dir, c_file.name), filespec, files);
			}
		}
		_findclose( hFile );
	}
	// find files
	if( (hFile = _findfirst( (char *) file_combo_path(dir, filespec.c_str()).c_str(), &c_file )) != -1L ) {
		if (!(c_file.attrib & _A_SUBDIR)) {
			files.push_back(file_combo_path(dir, c_file.name));
		}
		while( _findnext( hFile, &c_file ) == 0 ) {
			if (!(c_file.attrib & _A_SUBDIR)) {
				files.push_back(file_combo_path(dir, c_file.name));
			}
		}
		_findclose( hFile );
	}
	return files.size();
}

ZDEV_NAMESPACE_END

#endif // __zdev_win32_hpp__