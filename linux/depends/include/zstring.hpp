#ifndef __ZSTRING_H__
#define __ZSTRING_H__


#ifdef WIN32

#include <strings/vstring.hpp>
	
class ZString : public VString
{
public:
	ZString() : VString() {;}
	ZString( VString& str ) : VString(str) {;}
	ZString( const char* str ) : VString( str ) {;}
	
	const char* c_str()
	{
		return (const char *)m_pszString;
	}
	
	VINT			FindFirstString(	VSTRING_CONST	pszSubString,
								VBOOL			bCaseSensitive =
													VTRUE) const
	{
		VASSERT(VSTRLEN_CHECK(pszSubString))

		if ( GetLength() && pszSubString )
		{
			if ( bCaseSensitive )
			{
				VSTRING pszFound = VSTRSTR(m_pszString, pszSubString);

				if ( pszFound )
					return pszFound - m_pszString;
			}
			else
			{
				/* Create lowercase version of string and pszSubString.*/
				ZString sThis(VREF_THIS);
				sThis.LowerCase();

				VString sSub(pszSubString);
				sSub.LowerCase();

				return sThis.FindFirstString(sSub, VTRUE);
			}
		}

		return -1;
	}

	
	int ReplaceAll(const VString& vsOld, const VString& vsNew)
	{
		ZString aktString = *this;
		this->Empty();
	
		int len = vsOld.GetLength();
		int i, pos;
		
		for (pos = aktString.FindFirstString(vsOld), i = 0; pos >= 0; pos = aktString.FindFirstString(vsOld))
		{
			if( pos > 0 )	*this += aktString.Left(pos);
			*this		+= vsNew;
			aktString	= ZString( aktString.Mid(pos + len) );
			i++;
		}
	
		*this += aktString;
	
		return i;
	}	
};
	
#else // !WIN32

// this class used in class ZDateTime.
class ZString
{
public:
	ZString(const char* str) 
	{
		m_pStr = new char[strlen(str)+1];
		strcpy(m_pStr, str);
	}
	
	~ZString() { delete[] m_pStr; }
	
	const char* c_str() { return m_pStr; }
	
private:
	char *m_pStr;
};
		
#endif   //!WIN32

//----------------------------------------------------------------------------------------
#endif // __ZSTRING_H__
