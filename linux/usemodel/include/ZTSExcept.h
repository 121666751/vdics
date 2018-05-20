/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef ZTSEXCEPT_H_INCLUDED_C2FB3E3C
#define ZTSEXCEPT_H_INCLUDED_C2FB3E3C

#include <stdio.h>
using namespace std;
class ZTSExcept
{
public:
    enum  { EMPTY_ERROR_NO = 32767, EMPTY_LINE = -1 };

    ZTSExcept(char* errstr, const char* sFile = 0, int iLine = EMPTY_LINE)
    {
		ErrorNo = -1;
		sprintf(ErrorInfo,"%s:%d:%s",sFile,iLine,errstr);
	}
	
	 ZTSExcept(string errstr, const char* sFile = 0, int iLine = EMPTY_LINE)
    {
		ErrorNo = -1;
		sprintf(ErrorInfo,"%s:%d:%s",sFile,iLine,errstr.c_str());
	}

    virtual ~ZTSExcept() {}

    int GetErrNo() { return ErrorNo; }

    const char* GetErrInfo() {	return ErrorInfo;  }

protected:
    int ErrorNo;

	char ErrorInfo[512];

};

#define throw_err(a) throw( ZTSExcept(a,__FILE__,__LINE__) )

#endif /* ZTSEXCEPT_H_INCLUDED_C2FB3E3C */
