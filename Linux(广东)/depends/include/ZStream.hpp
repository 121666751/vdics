#ifndef __Z_STREAM_H__
#define __Z_STREAM_H__

#include <string.h>

class ZStream
{

// Constructions
public:

	ZStream() {		m_iSize = 0;	m_pBuffer = 0;		}

	ZStream( unsigned int Size )
		{
			if( Size )
			{
				m_pBuffer = new char[Size];
				m_iSize = Size;
			}
		}

	~ZStream() {	Free();		}

// Operations
public:

	/* Empty the stream and free the memory */
	void Free()	{
			if( m_pBuffer )
			{
				delete[] m_pBuffer;
				m_pBuffer = 0;
				m_iSize = 0;
			}
		}

	/* Add data into stream buffer */
	char* Append(void* p, int size)
		{
			if( size<=0 ) return 0;

			char* tmp = new char[m_iSize+size];

			if( m_iSize>0 ) memcpy(tmp,m_pBuffer,m_iSize);
			memcpy(tmp+m_iSize,p,size);
			m_iSize += size;
			delete[] m_pBuffer;
			m_pBuffer = tmp;

			return m_pBuffer;
		}

	char* AppendInteger(int val)
		{	return Append( &val, sizeof(int) );	}

	char* AppendString(char* str)
		{	return Append( str, strlen(str) );	}

	unsigned int GetSize() const
		{	return m_iSize;		}

	char* GetBuffer() const
		{	return m_pBuffer;		}

	char* GetBufferEx()
		{ 	return m_pBuffer; }

	char* Copy( ZStream stream )
		{	Free(); Append( stream.GetBuffer(), stream.GetSize() ); return m_pBuffer;	}

	int StealMemory( char*& retMem )
		{
			retMem = m_pBuffer;
			int size = m_iSize;
			m_iSize = 0;
			m_pBuffer = 0;
			return size;
		}

	void bzero()
		{
			if( m_iSize == 0 ) return;
			memset(m_pBuffer, 0, m_iSize);
		}
	const char* strcpy(const char* src)
		{
			if( m_iSize == 0 ) return 0;
			strncpy( m_pBuffer, src, m_iSize );
			return m_pBuffer;
		}                                 
	const char* cat_printf( const char* fmt, ... )
		{
			int used_len = strlen(m_pBuffer);
			char *cmd_ptr = m_pBuffer + used_len;
            try{
                va_list ap;
                va_start(ap,fmt);
                vsnprintf(cmd_ptr,m_iSize-used_len,fmt,ap);
                va_end(ap);
            }
            catch(...)
            {
            }
            return m_pBuffer;
		}

	// operator overwrite
	char* operator = ( ZStream& stream )
		{ 		return Copy(stream);		}

	operator char*(void)
		{		return m_pBuffer;	}

	operator const char*(void)
		{		return m_pBuffer;	}

//=======================================================================================
// protected section
protected:
	unsigned int m_iSize;
	char* m_pBuffer;

};

#endif // !__Z_STREAM_H__
