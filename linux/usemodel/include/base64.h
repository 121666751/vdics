/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __ZBASE64_FUNS_H__
#define __ZBASE64_FUNS_H__

#include <string>
#include <string.h>
#include <sstream>

using namespace std;



/**
 * base 64 encoding
 */
string base64_encode(unsigned char const* , unsigned int len);


/**
 * base 64 decoding
 */
string base64_decode(string const& s);


namespace boost
{


} // namespace boost
#endif // __ZBASE64_FUNS_H__
