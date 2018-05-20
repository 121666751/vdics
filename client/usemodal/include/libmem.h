#ifndef __LIBMEM_H__
#define __LIBMEM_H__

#include <zcommon.h>


/**
 * 内存管理类，用于自动分配和释放内存。该类可以在动态库间作为参数传递。
   头文件 include <libmem.h>
   库文件 libmem.lib(VC++), libmem_b.lib(BC++)

   下面例子为在某函数内部分配内存，然后返回给调用者。
 
    int get_some_image(MemT& mem)  // 内存分配函数，可以在动态库中。
	{
		unsigend char* p = new unsigned char[some_size];
		memcpy(p, some_data, some_size);

		mem.assign(p, some_size);	// 将数据放到MemT类中保管
		delete[] p;					// 将原有临时数据释放
		return 0;
	}

	int caller_function()		// 主调函数，可以在任何动态库或程序中
	{
		MemT mem;
		get_some_image(mem);

		unsigned char* p = mem;		//	得到数据内存地址
		int len = mem.size();	//  得到数据长度

		do_some_function(p, len);

		return 0; // 此时mem中的内存将自动被释放。
	}
 */
class MemT
{
public:
	/**
     * Constructor
	 */

	/**
	 * 缺省构造函数, 不分配内存.	
	 */
	MemT();

	/**
	 * 通过MemT类型完成拷贝构造
	 */
	MemT(MemT& mem);

	/**
	 * 通过内存地址完成拷贝构造
	 * @param ptr:	待拷贝内存地址
	 * @param size:	待拷贝内存大小
	 */
	MemT(unsigned char* ptr, size_t size);

	virtual ~MemT();

	/**
	 * Attribute
	 */

	/**
	 * 得到内存数据地址。注意！该地址只能用于临时使用，在调用MemT其他函数后，
	   该地址指向的数据有可能已被释放。
	 * @return 内存数据地址
	 */
	unsigned char* data();

	/**
	 * 得到内存数据大小
	 * @return 内存数据大小
	 */
	size_t size();

	/**
	 * Method
	 */

	/**
	 * 分配内存
	 * @size:	内存大小
	 * @return 当前对象的引用
	 */
	MemT& assign(size_t size);

	/**
	 * 分配并拷贝MemT对象
	 * @mem:	待拷贝对象
	 * @return 当前对象的引用
	 */
	MemT& assign(MemT& mem);

	/**
	 * 分配并拷贝内存
	 * @ptr:	待拷贝内存地址
	 * @size:	内存大小
	 * @return 当前对象的引用
	 */
	MemT& assign(unsigned char* ptr, size_t size);

	/**
	 * 释放内部已分配内存	
	 */
	void clear();
	
	/**
	 * 将内存初始化为指定值
	 */
	MemT& memset(int val); 

	/**
	 * Operator
	 */

	/**
	 * 支持通过‘=’赋值，方式同于 MemT::assign(MemT& mem)
	 */
	MemT& operator = (MemT& mem);

	/**
	 * 支持(unsigned char*)自动类型转换，方式同于 MemT::data()	
	 */
	unsigned char* operator ()(MemT& mem);
	
	
	/**
	 * 从文件读取数据 
	 * @fname: 文件名
	 * @return 0-success, other-fail
	 */
	int loadFromFile(const char* fname);
	
	/**
	 * 保存到文件
	 * @fname: 文件名
	 * @return 0-success, other-fail
	 */
	int saveToFile(const char* fname);


protected:
	unsigned char* _data;	/**< 内存数据 */
	size_t	_size;			/**< 内存数据大小 */
	char	_reserved[100];	/**< 保留空间 */
};

///////////////////////////////////////////////////////////////////////////////////
// depend functions 
extern "C" {

unsigned char* mem_alloc(size_t size);
unsigned char* mem_realloc(unsigned char* orgPtr, size_t size);
void mem_free(unsigned char* ptr);

}

///////////////////////////////////////////////////////////////////////////////////
// inline functions 
inline MemT::MemT()
:_data(0),_size(0)
{
}

inline MemT::MemT(MemT& mem)
:_data(0),_size(0)
{
	assign(mem);
}

inline MemT::MemT(unsigned char* ptr, size_t size)
:_data(0),_size(0)
{
	assign(ptr,size);
}

inline MemT::~MemT()
{
	mem_free(_data);
}

inline unsigned char* MemT::data()
{
	return _data;
}

inline size_t MemT::size()
{
	return _size;
}

inline MemT& MemT::assign(size_t size)
{
	return assign(NULL,size);
}

inline MemT& MemT::assign(unsigned char* ptr, size_t size)
{
	_data = mem_realloc(_data, size);
	_size = size;

	if( ptr ) memcpy(_data,ptr,size);

	return *this;
}

inline MemT& MemT::assign(MemT& mem)
{
	return assign(mem.data(),mem.size());
}

inline void MemT::clear()
{
	if( _data ) mem_free(_data);
	_data = 0;
	_size = 0;
}

inline MemT& MemT::memset(int val)
{
	if( _size ) {
		::memset(_data,val,_size);
	}
	
	return *this;	
}



inline MemT& MemT::operator = (MemT& mem)
{
	return assign(mem);
}

inline unsigned char* MemT::operator()(MemT&)
{
	return _data;
}

inline int MemT::loadFromFile(const char* fname)
{
	struct stat buf;
	if( stat(fname, &buf) ) return -1;
	assign( buf.st_size );
	
	FILE* fp = fopen(fname,"r+b");
	if(!fp)  return -2;
	
	fread(_data,1,buf.st_size,fp);
	
	fclose(fp);
	return 0;
}

inline int MemT::saveToFile(const char* fname)
{
	if(_size==0) return 0;
	
	FILE* fp = fopen(fname,"w+b");
	if(!fp)  return -2;
	
	fwrite(_data,1,_size,fp);
	
	fclose(fp);
	return 0;
}
inline void ConvertFromChannelToDib(unsigned char *srcData, unsigned char *dstData, int SizeX, int SizeY, int nChannels)
{
	debugout("convert channel to dib the channel [%d]",nChannels);
	if (nChannels == 3)
	{
		int i, j, k;
		unsigned char *pcolor[3];

		int widthBytes = (SizeX * nChannels * 8 + 31) / 32 * 4;

		for (k=0; k<nChannels; k++)
			pcolor[k] = srcData + SizeX * SizeY * k;

		for (i=0; i<SizeY; i++)
		{
			for (j=0; j<SizeX; j++)
			{
				for (k=0; k<nChannels; k++)
				{
					dstData[widthBytes * (SizeY - i -1) + nChannels * j + k] = *pcolor[nChannels - 1 - k];
					pcolor[nChannels - 1 - k]++;
				}
			}
			for (k = 0; k < widthBytes - SizeX * 3; k++)
				dstData[widthBytes * (SizeY - i -1) + SizeX * 3 + k] = 0; 
		}

	}
	else
	{
	}
	
 }
#endif // __LIBMEM_H__