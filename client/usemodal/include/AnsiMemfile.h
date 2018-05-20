/*++

Copyright (c) 2001 - 2008, nnnnnn Corporation
  
Module Name:
	
	AnsiMemFile.h
	  
Abstract:
		
	�ڴ��ļ���
		  
Author:
			
	Kimi Hu 2007/11/13
			  
Revision History:
				
	Created -- 2007-11-13   22:11
				  
--*/
#ifndef __ANSIMEMFILE_H__
#define __ANSIMEMFILE_H__

#include <malloc.h>
#include <assert.h>

class CAnsiMemFile
{
protected:
	unsigned int  m_nGrowBytes;
	unsigned long m_nPosition;
	unsigned long m_nBufferSize;
	unsigned long m_nFileSize;
	unsigned char* m_lpBuffer;
	bool m_bAutoDelete;
	
	// -------------------------------------------------------------------
	unsigned char* Alloc(unsigned long nBytes)
	{
		return (unsigned char*)malloc((unsigned int)nBytes);
	}
	
	// -------------------------------------------------------------------
	unsigned char* Realloc(unsigned char* lpMem, unsigned long nBytes)
	{
		return (unsigned char*)realloc(lpMem, (unsigned int)nBytes);
	}
	
	// -------------------------------------------------------------------
#pragma intrinsic(memcpy)
	unsigned char* Memcpy(unsigned char* lpMemTarget, const unsigned char* lpMemSource,
		unsigned int nBytes)
	{
		assert(NULL != lpMemTarget);
		assert(lpMemSource != NULL);
		
		return (unsigned char*)memcpy(lpMemTarget, lpMemSource, nBytes);
	}
#pragma function(memcpy)
	
	// -------------------------------------------------------------------
	void Free(unsigned char* lpMem)
	{
		assert(lpMem != NULL);
		
		free(lpMem);
	}
	
	// -------------------------------------------------------------------
	void GrowFile(unsigned long dwNewLen)
	{
		assert(this);
		
		if (dwNewLen > m_nBufferSize)
		{
			// grow the buffer
			unsigned long dwNewBufferSize = (unsigned long)m_nBufferSize;
			
			// watch out for buffers which cannot be grown!
			assert(m_nGrowBytes != 0);
			
			// determine new buffer size
			while (dwNewBufferSize < dwNewLen)
				dwNewBufferSize += m_nGrowBytes;
			
			// allocate new buffer
			unsigned char* lpNew;
			if (m_lpBuffer == NULL)
				lpNew = Alloc(dwNewBufferSize);
			else
				lpNew = Realloc(m_lpBuffer, dwNewBufferSize);
					
			m_lpBuffer = lpNew;
			m_nBufferSize = dwNewBufferSize;
		}
		assert(this);
	}
public:
	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };
	
	// -------------------------------------------------------------------
	//nGrowBytes ���ļ���Ҫ����ʱ���������ȣ�ÿ������nGrowBytes���ֽڣ�
	CAnsiMemFile(unsigned int nGrowBytes = 1024)
	{	
		m_nGrowBytes = nGrowBytes;
		m_nPosition = 0;
		m_nBufferSize = 0;
		m_nFileSize = 0;
		m_lpBuffer = NULL;
		m_bAutoDelete = true;
	}

	// -------------------------------------------------------------------
	//����ڹ�������Attach
	CAnsiMemFile(unsigned char* lpBuffer, unsigned int nBufferSize, unsigned int nGrowBytes = 0)
	{
		m_nGrowBytes = nGrowBytes;
		m_nPosition = 0;
		m_nBufferSize = nBufferSize;
		m_nFileSize = nGrowBytes == 0 ? nBufferSize : 0;
		m_lpBuffer = lpBuffer;
		m_bAutoDelete = false;
	}
	
	// -------------------------------------------------------------------
	~CAnsiMemFile()
	{
		// Close should have already been called, but we check anyway
		if (m_lpBuffer)
			Close();
		assert(m_lpBuffer == NULL);
		
		m_nGrowBytes = 0;
		m_nPosition = 0;
		m_nBufferSize = 0;
		m_nFileSize = 0;
	}
	
	//////////////////////ȡ����
	
	// -------------------------------------------------------------------
	//ȡ�õ�ǰ��дλ��
	unsigned long GetPosition() const
	{
		assert(this);
		return m_nPosition;
	}

	// -------------------------------------------------------------------
	//ȡ���ļ��ĵ�ǰ��С
	unsigned long GetLength() const
	{
		unsigned long dwLen, dwCur;
		// Seek is a non const operation
		CAnsiMemFile* pFile = (CAnsiMemFile*)this;
		dwCur = pFile->Seek(0L, current);
		dwLen = pFile->SeekToEnd();
		pFile->Seek(dwCur, begin);
		
		return dwLen;
	}

	// -------------------------------------------------------------------
	//ȡ���ڴ�����ָ��
	//ע�⣺ֻ������Ҫ�޸ĸ�ָ��ָ������ݡ������Ҫ��д���ļ����ݣ�����Read()��Write()
	const unsigned char  * const GetPtr() const
	{
		return m_lpBuffer;
	}
	
	///////////////////////����
	
	// -------------------------------------------------------------------
	// �����Զ������ߴ�
	void SetGrowBytes(unsigned int nGrowBytes = 1024)
	{	
//		assert(nGrowBytes <= unsigned int_MAX);
		
		m_nGrowBytes = nGrowBytes;
	}
	
	// -------------------------------------------------------------------
	//ָ�����ļ����õ��ڴ�
	//�ö�������ʱ�����ͷŸ��ڴ��
	void Attach(unsigned char* lpBuffer, unsigned int nBufferSize, unsigned int nGrowBytes = 0)
	{
		assert(m_lpBuffer == NULL);
		
		m_nGrowBytes = nGrowBytes;
		m_nPosition = 0;
		m_nBufferSize = nBufferSize;
		m_nFileSize = nGrowBytes == 0 ? nBufferSize : 0;
		m_lpBuffer = lpBuffer;
		m_bAutoDelete = false;
	}

	// -------------------------------------------------------------------
	//������ļ�������ǰռ�õ��ڴ��Ĺ���
	//���ظ��ڴ���ָ��
	unsigned char* Detach()
	{
		unsigned char* lpBuffer = m_lpBuffer;
		m_lpBuffer = NULL;
		m_nFileSize = 0;
		m_nBufferSize = 0;
		m_nPosition = 0;
		
		return lpBuffer;
	}

	// -------------------------------------------------------------------
	//�ƶ���дλ�õ��ļ����
	unsigned long SeekToEnd()
	{ 
		return Seek(0, CAnsiMemFile::end); 
	}

	// -------------------------------------------------------------------
	//�ƶ���дλ�õ��ļ���ͷ
	void SeekToBegin()
	{ 
		Seek(0, CAnsiMemFile::begin); 
	}

	// -------------------------------------------------------------------
	//�ı��ļ�����
	//�����Ҫ�����·�����ռ�ڴ�
	void SetLength(unsigned long dwNewLen)
	{
		assert(this);
		
		if (dwNewLen > m_nBufferSize)
			GrowFile(dwNewLen);
		
		if (dwNewLen < m_nPosition)
			m_nPosition = dwNewLen;
		
		m_nFileSize = dwNewLen;
		assert(this);
	}

	// -------------------------------------------------------------------
	//�ӵ�ǰ��дλ�ö�ȡָ�����ȵ�����
	unsigned int Read(const void* lpBuf, unsigned int nCount)
	{
		assert(this);
		
		if (nCount == 0)
			return 0;
		
		assert(lpBuf != NULL);
		//assert(AfxIsValidAddress(lpBuf, nCount));
		
		if (m_nPosition > m_nFileSize)
			return 0;
		
		unsigned int nRead;
		if (m_nPosition + nCount > m_nFileSize)
			nRead = (unsigned int)(m_nFileSize - m_nPosition);
		else
			nRead = nCount;
		
		Memcpy((unsigned char*)lpBuf, (unsigned char*)m_lpBuffer + m_nPosition, nRead);
		m_nPosition += nRead;
		
		assert(this);
		
		return nRead;
	}

	// -------------------------------------------------------------------
	template <typename T>
		unsigned int Read(T& buf)
	{
		return Read(&buf, sizeof(buf));
	}

	// -------------------------------------------------------------------
	//дָ�����ȵ����ݵ��ļ���
	void Write(const void* lpBuf, unsigned int nCount, unsigned int nDataLen, bool bRightAlign = true, unsigned char ch = ' ')
	{
		assert(this);
		
		if (nCount == 0)
			return;
		
		assert(lpBuf != NULL);
		//assert(AfxIsValidAddress(lpBuf, nCount, false));
		
		if (m_nPosition + nCount > m_nBufferSize)
			GrowFile(m_nPosition + nCount);
		
		assert(m_nPosition + nCount <= m_nBufferSize);
		
		memset((unsigned char*)m_lpBuffer + m_nPosition, ch, nCount);
		if (bRightAlign && (nDataLen < nCount))
		{
			//memset((unsigned char*)m_lpBuffer + m_nPosition, '', nDataLen - nCount);
			Memcpy((unsigned char*)m_lpBuffer + m_nPosition + nCount - nDataLen, 
						 (unsigned char*)lpBuf, nDataLen);
		}
		else
		{
			Memcpy((unsigned char*)m_lpBuffer + m_nPosition, 
						(unsigned char*)lpBuf, nDataLen < nCount ? nDataLen : nCount);
		}
		
		m_nPosition += nCount;
		
		if (m_nPosition > m_nFileSize)
			m_nFileSize = m_nPosition;
		
		assert(this);
	}

	// -------------------------------------------------------------------
	template <typename T>
		void Write(const T& buf)
	{
		Write(&buf, sizeof(buf));
	}

	// -------------------------------------------------------------------
	//�ƶ���дλ��
	//lOff �ƶ��ľ���
	//nFrom ָ�������￪ʼ�ƶ�
	//
	//�ڶ�����������Ϊ��
	//enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };
	long Seek(long lOff, unsigned int nFrom)
	{
		assert(this);
		assert(nFrom == begin || nFrom == end || nFrom == current);
		
		long lNewPos = m_nPosition;
		
		if (nFrom == begin)
			lNewPos = lOff;
		else if (nFrom == current)
			lNewPos += lOff;
		else if (nFrom == end)
			lNewPos = m_nFileSize + lOff;
		else
			return -1;
		
		if (lNewPos < 0)
			return -1;
//			throw exception("Seek����");
		
		m_nPosition = lNewPos;
		
		assert(this);
		return m_nPosition;
	}

	// -------------------------------------------------------------------
	//�ر��ڴ��ļ����ͷ���ռ�õ��ڴ棬�ó���Ϊ0��
	//���ڴ��ļ�������������������д���κ����ݣ�
	//�����������Զ�����Close
	void Close()
	{
		assert((m_lpBuffer == NULL && m_nBufferSize == 0) ||
			!m_bAutoDelete || true);
		assert(m_nFileSize <= m_nBufferSize);
		
		m_nGrowBytes = 0;
		m_nPosition = 0;
		m_nBufferSize = 0;
		m_nFileSize = 0;
		if (m_lpBuffer && m_bAutoDelete)
			Free(m_lpBuffer);
		m_lpBuffer = NULL;
	}
};

#endif //__ANSIMEMFILE_H__
