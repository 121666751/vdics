/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __ZBASE_MEM_H__
#define __ZBASE_MEM_H__

#include <string>
#include <fstream>
#include "ZTSExcept.h"

///////////////////////////////////////////////////////////////////////////////////
// depend functions

class MemT
{
public:
	/**
	 * 缺省构造函数, 不分配内存.
	 */
	MemT():_size(0),_data(0) {}

	/**
	 * 通过MemT类型完成拷贝构造
	 */
	MemT(const MemT& mem):_size(0),_data(0) { assign(mem); }

	/**
	 * 通过内存地址完成拷贝构造
	 * @param ptr:	待拷贝内存地址
	 * @param size:	待拷贝内存大小
	 */
	MemT(const char* ptr, size_t size):_size(0),_data(0)
		{ assign(ptr, size); }

	/**
	 * 分配指定大小的内存
	 * @param size:	待拷贝内存大小
	 */
	MemT(size_t size):_size(0),_data(0)
		{ assign(NULL,size); }
		
		
	MemT(const std::string& str):_size(0),_data(0)
		{ assign(str.c_str(),str.length()); }


	virtual ~MemT() ;


	unsigned char* data() const;

	std::string str() const;


	/**
	 * 得到内存数据大小
	 * @return 内存数据大小
	 */
	size_t size() const { return _size; }


	/**
	 * 分配内存
	 * @param size:	内存大小
	 * @return 当前对象的引用
	 */
	void assign(size_t size) {	assign(NULL,size); }


	/**
	 * 分配并拷贝MemT对象
	 * @param mem:	待拷贝对象
	 * @return 当前对象的引用
	 */
	void assign(const MemT& mem)
		{	assign((const char*)mem.data(),mem.size()); }


 	void assign( const void* ptr, size_t size);
	
	void resize(size_t size);
	
	/**
	 * 释放内部已分配内存
	 */
	void clear();

	/**
	 * 将内存初始化为指定值
	 */
	void memset(int val)
		{	if( _size ) ::memset(_data,val,_size);	}

	/**
	 * 将内存初始化为0
	 */
	void bzero()
		{	if( _size ) ::memset(_data,0,_size);	}

	/**
	 * Operator
	 */

	/**
	 * 支持通过‘=’赋值，方式同于 MemT::assign(MemT& mem)
	 */
	void operator = (const MemT& mem)
		{	assign(mem); }

	/**
	 * 比较两段内存是否相同
	 */
	bool operator == (const MemT& mem)
		{
			if( mem.size() != _size ) return false;
			return ( memcmp(mem.data(), _data, _size) == 0 );
		}

	/**
	 * 从文件读取数据
	 * @param fname: 文件名
	 * \warning 如果文件无法打开，将抛出exception异常。
	 */
	int loadFromFile(const char* fname);

	/**
	 * 保存到文件
	 * @param fname: 文件名
	 * \warning 如果文件无法打开，将抛出exception异常。如果对象中没有数据，将不产生文件。
	 */
	int saveToFile(const char* fname);


protected:
	unsigned char* _data;	/**< 内存数据 */
	size_t	_size;			/**< 内存数据大小 */
	char	_reserved[100];	/**< 保留空间 */
};


/**
 压缩数据
 @param in 待压缩的内存数据
 @param out 压缩后的内存数据
 */
void compress(const MemT& in, MemT& out);

/**
 解压缩数据
 @param in 压缩后的内存数据
 @param out 解压缩后的内存数据
 */
void uncompress(const MemT& in, MemT& out);


/**
 压缩数据
 @param in 待压缩的内存数据
 @return 压缩后的内存数据
 */
 
 
std::string compress(const std::string& in);

/**
 解压缩数据
 @param in 压缩后的内存数据
 @return 解压缩后的内存数据
 */
std::string uncompress(const std::string& in);
/**
 压缩数据
 @param data 待压缩的内存数据
 @param len 待压缩的内存数据大小
 */
MemT zbase_compress(unsigned char* data, size_t len);

/**
压缩数据
@param data 待解压缩的内存数据
@param len 待解压缩的内存数据大小
*/
MemT zbase_uncompress(unsigned char* data, size_t len);
 
/**
 * convert string into int, more safe than atoi
 */
int stoi(string s);

/** convert int to string */
string itos(int i);

#endif // __ZBASE_MEM_H__
