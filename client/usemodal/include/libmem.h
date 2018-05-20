#ifndef __LIBMEM_H__
#define __LIBMEM_H__

#include <zcommon.h>


/**
 * �ڴ�����࣬�����Զ�������ͷ��ڴ档��������ڶ�̬�����Ϊ�������ݡ�
   ͷ�ļ� include <libmem.h>
   ���ļ� libmem.lib(VC++), libmem_b.lib(BC++)

   ��������Ϊ��ĳ�����ڲ������ڴ棬Ȼ�󷵻ظ������ߡ�
 
    int get_some_image(MemT& mem)  // �ڴ���亯���������ڶ�̬���С�
	{
		unsigend char* p = new unsigned char[some_size];
		memcpy(p, some_data, some_size);

		mem.assign(p, some_size);	// �����ݷŵ�MemT���б���
		delete[] p;					// ��ԭ����ʱ�����ͷ�
		return 0;
	}

	int caller_function()		// �����������������κζ�̬��������
	{
		MemT mem;
		get_some_image(mem);

		unsigned char* p = mem;		//	�õ������ڴ��ַ
		int len = mem.size();	//  �õ����ݳ���

		do_some_function(p, len);

		return 0; // ��ʱmem�е��ڴ潫�Զ����ͷš�
	}
 */
class MemT
{
public:
	/**
     * Constructor
	 */

	/**
	 * ȱʡ���캯��, �������ڴ�.	
	 */
	MemT();

	/**
	 * ͨ��MemT������ɿ�������
	 */
	MemT(MemT& mem);

	/**
	 * ͨ���ڴ��ַ��ɿ�������
	 * @param ptr:	�������ڴ��ַ
	 * @param size:	�������ڴ��С
	 */
	MemT(unsigned char* ptr, size_t size);

	virtual ~MemT();

	/**
	 * Attribute
	 */

	/**
	 * �õ��ڴ����ݵ�ַ��ע�⣡�õ�ַֻ��������ʱʹ�ã��ڵ���MemT����������
	   �õ�ַָ��������п����ѱ��ͷš�
	 * @return �ڴ����ݵ�ַ
	 */
	unsigned char* data();

	/**
	 * �õ��ڴ����ݴ�С
	 * @return �ڴ����ݴ�С
	 */
	size_t size();

	/**
	 * Method
	 */

	/**
	 * �����ڴ�
	 * @size:	�ڴ��С
	 * @return ��ǰ���������
	 */
	MemT& assign(size_t size);

	/**
	 * ���䲢����MemT����
	 * @mem:	����������
	 * @return ��ǰ���������
	 */
	MemT& assign(MemT& mem);

	/**
	 * ���䲢�����ڴ�
	 * @ptr:	�������ڴ��ַ
	 * @size:	�ڴ��С
	 * @return ��ǰ���������
	 */
	MemT& assign(unsigned char* ptr, size_t size);

	/**
	 * �ͷ��ڲ��ѷ����ڴ�	
	 */
	void clear();
	
	/**
	 * ���ڴ��ʼ��Ϊָ��ֵ
	 */
	MemT& memset(int val); 

	/**
	 * Operator
	 */

	/**
	 * ֧��ͨ����=����ֵ����ʽͬ�� MemT::assign(MemT& mem)
	 */
	MemT& operator = (MemT& mem);

	/**
	 * ֧��(unsigned char*)�Զ�����ת������ʽͬ�� MemT::data()	
	 */
	unsigned char* operator ()(MemT& mem);
	
	
	/**
	 * ���ļ���ȡ���� 
	 * @fname: �ļ���
	 * @return 0-success, other-fail
	 */
	int loadFromFile(const char* fname);
	
	/**
	 * ���浽�ļ�
	 * @fname: �ļ���
	 * @return 0-success, other-fail
	 */
	int saveToFile(const char* fname);


protected:
	unsigned char* _data;	/**< �ڴ����� */
	size_t	_size;			/**< �ڴ����ݴ�С */
	char	_reserved[100];	/**< �����ռ� */
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