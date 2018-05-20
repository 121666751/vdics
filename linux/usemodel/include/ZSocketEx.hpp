
#ifndef ZSOCKETEX_HPP
#define ZSOCKETEX_HPP

#include <ZSocket.hpp>
#include <ZStream.hpp>

#pragma warning( push )
#pragma warning( disable : 4018 )

class ZSocketEx : public ZSocket
{
public:
	#define MAXSTREAMLEN	10000000
public:
	int RecvStream(ZStream &Stream, int TimeOut=5000)
	{
		int ret;
		long nToBeRecv = 0;
		ret = RecvEx((char*)&nToBeRecv, sizeof(nToBeRecv), TimeOut);
		if( ret < 0 ) return ZS_TIMEOUT;

		nToBeRecv = ntohl(nToBeRecv);
		if( nToBeRecv==0 ) return 0;	// no data need to recv

		if( nToBeRecv > MAXSTREAMLEN ) return ZS_OVERFLOW;

		Stream.Free();
		Stream.SetCapacity(nToBeRecv);

		char* buf = Stream.GetBufferEx();

		int nHaveRecv = 0;
		do {
			ret = RecvEx( buf+nHaveRecv, nToBeRecv-nHaveRecv, TimeOut );
			nHaveRecv += ret;
		} while( ret > 0 && nHaveRecv < nToBeRecv );

		if(ret<0 || nHaveRecv != nToBeRecv) return ZS_UNMATCHEDLENGTH;

		return nHaveRecv;
		
	}

	int SendStream(unsigned char const *buf, int buflen)
	{
		long nDataLen = buflen;
		int ret;

		nDataLen = htonl(nDataLen);

		ret = Send(&nDataLen, sizeof(nDataLen));
		if(ret != sizeof(nDataLen))
			return ZS_SENDERR;

		int nHaveSend = 0;
		do {
			ret = Send( (void*)(buf+nHaveSend), buflen-nHaveSend );
			nHaveSend += ret;
		} while( ret > 0 && nHaveSend < buflen );

		return (ret<0)? ret : nHaveSend;
	}
};

#pragma warning( pop )

#endif