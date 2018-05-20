/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
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
	 * ȱʡ���캯��, �������ڴ�.
	 */
	MemT():_size(0),_data(0) {}

	/**
	 * ͨ��MemT������ɿ�������
	 */
	MemT(const MemT& mem):_size(0),_data(0) { assign(mem); }

	/**
	 * ͨ���ڴ��ַ��ɿ�������
	 * @param ptr:	�������ڴ��ַ
	 * @param size:	�������ڴ��С
	 */
	MemT(const char* ptr, size_t size):_size(0),_data(0)
		{ assign(ptr, size); }

	/**
	 * ����ָ����С���ڴ�
	 * @param size:	�������ڴ��С
	 */
	MemT(size_t size):_size(0),_data(0)
		{ assign(NULL,size); }
		
		
	MemT(const std::string& str):_size(0),_data(0)
		{ assign(str.c_str(),str.length()); }


	virtual ~MemT() ;


	unsigned char* data() const;

	std::string str() const;


	/**
	 * �õ��ڴ����ݴ�С
	 * @return �ڴ����ݴ�С
	 */
	size_t size() const { return _size; }


	/**
	 * �����ڴ�
	 * @param size:	�ڴ��С
	 * @return ��ǰ���������
	 */
	void assign(size_t size) {	assign(NULL,size); }


	/**
	 * ���䲢����MemT����
	 * @param mem:	����������
	 * @return ��ǰ���������
	 */
	void assign(const MemT& mem)
		{	assign((const char*)mem.data(),mem.size()); }


 	void assign( const void* ptr, size_t size);
	
	void resize(size_t size);
	
	/**
	 * �ͷ��ڲ��ѷ����ڴ�
	 */
	void clear();

	/**
	 * ���ڴ��ʼ��Ϊָ��ֵ
	 */
	void memset(int val)
		{	if( _size ) ::memset(_data,val,_size);	}

	/**
	 * ���ڴ��ʼ��Ϊ0
	 */
	void bzero()
		{	if( _size ) ::memset(_data,0,_size);	}

	/**
	 * Operator
	 */

	/**
	 * ֧��ͨ����=����ֵ����ʽͬ�� MemT::assign(MemT& mem)
	 */
	void operator = (const MemT& mem)
		{	assign(mem); }

	/**
	 * �Ƚ������ڴ��Ƿ���ͬ
	 */
	bool operator == (const MemT& mem)
		{
			if( mem.size() != _size ) return false;
			return ( memcmp(mem.data(), _data, _size) == 0 );
		}

	/**
	 * ���ļ���ȡ����
	 * @param fname: �ļ���
	 * \warning ����ļ��޷��򿪣����׳�exception�쳣��
	 */
	int loadFromFile(const char* fname);

	/**
	 * ���浽�ļ�
	 * @param fname: �ļ���
	 * \warning ����ļ��޷��򿪣����׳�exception�쳣�����������û�����ݣ����������ļ���
	 */
	int saveToFile(const char* fname);


protected:
	unsigned char* _data;	/**< �ڴ����� */
	size_t	_size;			/**< �ڴ����ݴ�С */
	char	_reserved[100];	/**< �����ռ� */
};


/**
 ѹ������
 @param in ��ѹ�����ڴ�����
 @param out ѹ������ڴ�����
 */
void compress(const MemT& in, MemT& out);

/**
 ��ѹ������
 @param in ѹ������ڴ�����
 @param out ��ѹ������ڴ�����
 */
void uncompress(const MemT& in, MemT& out);


/**
 ѹ������
 @param in ��ѹ�����ڴ�����
 @return ѹ������ڴ�����
 */
 
 
std::string compress(const std::string& in);

/**
 ��ѹ������
 @param in ѹ������ڴ�����
 @return ��ѹ������ڴ�����
 */
std::string uncompress(const std::string& in);
/**
 ѹ������
 @param data ��ѹ�����ڴ�����
 @param len ��ѹ�����ڴ����ݴ�С
 */
MemT zbase_compress(unsigned char* data, size_t len);

/**
ѹ������
@param data ����ѹ�����ڴ�����
@param len ����ѹ�����ڴ����ݴ�С
*/
MemT zbase_uncompress(unsigned char* data, size_t len);
 
/**
 * convert string into int, more safe than atoi
 */
int stoi(string s);

/** convert int to string */
string itos(int i);

#endif // __ZBASE_MEM_H__
