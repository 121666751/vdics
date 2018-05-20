#ifndef __MYSTRING_EX_H__
#define __MYSTRING_EX_H__

#include <stdarg.h>
#include <string>
// ---------------------------------------------------------------------

template<typename CT>
class strex_t : public std::basic_string<CT>
{
	typedef std::basic_string<CT>	mybase_t;

public:
	strex_t() {};
	strex_t(const strex_t<CT>& str):mybase_t(str.c_str()) {};
	strex_t(const std::string& str):mybase_t(str) {};
	
	strex_t<CT>& trim();
	strex_t<CT>& trim_left();
	strex_t<CT>& trim_right();

	int toi() { return atoi(c_str()); }
	double tof() { return atof(c_str()); }
	
	void printf( const CT* fmt, ... ); 
	
	strex_t<CT>& operator=( const char* str ) { *(mybase_t*)this=str; return *this; }
	strex_t<CT>& operator=( std::string str ) { *(mybase_t*)this=str; return *this; }
};


template<typename CT>
struct not_space_t : public std::unary_function<CT,bool> 
{
	inline bool operator() (CT tchar) { return !std::isspace(tchar,std::locale()); }
};

template<typename CT>
inline strex_t<CT>& strex_t<CT>::trim()
{
	return trim_left().trim_right();
}

template<typename CT>
inline strex_t<CT>& strex_t<CT>::trim_left()
{
	iterator it = std::find_if(begin(), end(), not_space_t<CT>());
	if( end() != it ) {
		traits_type().move(&(*begin()), &(*it), (end()-it+1)*sizeof(CT));
		resize(end()-it);
	}
	return *this;
}

template<typename CT>
strex_t<CT>& strex_t<CT>::trim_right()
{
	reverse_iterator it = std::find_if(rbegin(), rend(), not_space_t<CT>());
	if ( rend() != it )
		erase(rend() - it);

	erase(it != rend() ? find_last_of(*it) + 1 : 0);
	return *this;
}

template<typename CT>
void strex_t<CT>::printf(const CT* fmt, ... )
{
	char buf[1024];
    try{
        va_list ap;
        va_start(ap,fmt);
        vsnprintf(buf,sizeof(buf),fmt,ap);
        va_end(ap);
    }
    catch(...) {
    }
    
    *this = buf;
}

// -----------------------------------------------------------------
typedef strex_t<char>	stringex;	// a better std::string


// -----------------------------------------------------------------
#endif
