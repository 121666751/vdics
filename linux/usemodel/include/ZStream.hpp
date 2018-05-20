#ifndef __Z_STREAM_H__
#define __Z_STREAM_H__

#include <string.h>
#include <memory.h>

// predefined const values

// new capacity size is  m_capacity*pre_grow_rate+append_size+pre_add_size
#define         pre_grow_rate           1.5
#define         pre_add_size            256000


class ZStream
{
public:
	ZStream();
	ZStream( unsigned int Capacity );
	ZStream( ZStream& stream );
	virtual ~ZStream();

// Operations
public:
	// Adjust internal capacity to new size. if new Capacity to set is smaller than
	// origincal, it do nothing.
	void SetCapacity( unsigned int Capacity );
	unsigned int GetCapacity();
	
	// Empty the stream and free the memory
	void Free();
	
	// Add data into stream buffer
	char* Append(void* p, unsigned int size);
		
	char* AppendInteger(int val);
	char* AppendString(char* str);
	
	// return the internal buffer.
	char* GetBuffer() const;
	char* GetBufferEx();

	char* Copy( ZStream stream );
	
	
	// return the data size of stored.
	unsigned int GetSize() const;
	
	// return buffer to caller. this memory must be free my caller use "delete[]"
	unsigned int StealMemory( char*& retMem );

	// operator overwrite
	char* operator = ( ZStream& stream );
	operator char*(void);
	operator const char*(void);


// protected section
protected:

	// the data size of user stored.
	unsigned int m_iSize;
	
	// the data buffer ptr.
	char* m_pBuffer;

	// current buffer size pre alloced
	unsigned int m_capacity;


};


/////////////////////////////////////////////////////////////////////////////////////////
// Implimention session
inline ZStream::ZStream() : m_pBuffer(NULL)
{		
	m_pBuffer = 0;
	m_iSize = m_capacity = 0;
}

inline ZStream::ZStream( unsigned int Capacity )
{
	m_pBuffer = 0;
	m_iSize = m_capacity = 0;
	SetCapacity( Capacity );			
}

inline ZStream::ZStream( ZStream& stream )
{
	Copy(stream);
}

inline ZStream::~ZStream() 
{	
	Free();		
}

/* Empty the stream and free the memory */
inline void ZStream::Free()	
{
	m_iSize = 0;
	m_capacity = 0;
	
	if( m_pBuffer )
	{
		delete[] m_pBuffer;
		m_pBuffer = 0;
	}
}

/* Add data into stream buffer */
inline char* ZStream::Append(void* p, unsigned int size)
{
	if( size==0 ) return 0;

	if( m_iSize+size > m_capacity ) {
		int new_capacity = (int)((float)m_capacity*pre_grow_rate + pre_add_size + size);
		SetCapacity( new_capacity );
	}
	
	memcpy(m_pBuffer+m_iSize,p,size);
	m_iSize += size;
	
	return m_pBuffer;
}

inline void ZStream::SetCapacity( unsigned int Capacity ) 
{
	if( Capacity > m_capacity ) {
		m_capacity = Capacity;
		char* p = new char[m_capacity];
		if( m_iSize ) memcpy(p,m_pBuffer,m_iSize);
		if( m_pBuffer ) delete[] m_pBuffer;
		m_pBuffer = p;
	}
}		

inline unsigned int ZStream::GetCapacity()
{
	return m_capacity;
}

inline char* ZStream::AppendInteger(int val)
{	return Append( &val, sizeof(int) );	}

inline char* ZStream::AppendString(char* str)
{	return Append( str, strlen(str) );	}

inline unsigned int ZStream::GetSize() const
{	return m_iSize;		}

inline char* ZStream::GetBuffer() const
{	return m_pBuffer;		}

inline char* ZStream::GetBufferEx()
{ 	return m_pBuffer; }

inline char* ZStream::Copy( ZStream stream )
{	Free(); Append( stream.GetBuffer(), stream.GetSize() ); return m_pBuffer;	}

inline unsigned int ZStream::StealMemory( char*& retMem )
{
	retMem = m_pBuffer;
	unsigned int size = m_iSize;
	m_iSize = 0;
	m_pBuffer = 0;
	return size;
}

// operator overwrite
inline char* ZStream::operator = ( ZStream& stream )
{ 		return Copy(stream);		}

inline ZStream::operator char*(void)
{		return m_pBuffer;	}

inline ZStream::operator const char*(void)
{		return m_pBuffer;	}


#endif // !__Z_STREAM_H__
