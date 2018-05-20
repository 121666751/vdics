#ifndef __ZFILE_HPP__
#define __ZFILE_HPP__

#include <zstandard.h>
#include <zcommon.h>

class ZFile
{
public:
	ZFile() { fp = NULL; }
	~ZFile() { if(fp) fclose(fp); }

	bool Open( const char* sFileName, const char* sMode )
	{
		fp = fopen(sFileName,sMode);

		return (fp)? true:false;
	}
	
	operator FILE* (void)
	{
		return fp;
	}
	
public:
	FILE* fp;
};

#endif //__ZFILE_HPP__
