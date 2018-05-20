
#ifndef __MYPLUS_H__
#define __MYPLUS_H__


/*  Include Needed Header  */
#include "zstandard.h"
#include "zsocket.h"
#include <stdarg.h>

#include "Mysql/mysql.h"

#define MY_SUCCESS	0

class MyRes;
typedef MYSQL_ROW MyRow;

////////////////////////////////////////////////////////
//
class MyDb
{
public:
	// Constructions
	MyDb()		{ m_hMysql=NULL; m_bConnected=false; m_bSelected=false;}
	MyDb(const char* Host, const char* UserName, const char* Password, const char* DatabaseName)
				{ m_bConnected=false;m_bSelected=false;Open(Host, UserName, Password, DatabaseName); }
	~MyDb()		{ Close(); }

	// Operations
	int 	Open(const char* Host, const char* UserName, const char* Password, const char* DatabaseName);
	void 	Close();

	int Query(const char* Command, int CmdLen=0);
	int VQuery(unsigned int BufferLen, const char* fmt, ...);


	my_ulonglong AffectedRows() 	{ return mysql_affected_rows(m_hMysql); }
	MYSQL_RES* 	Store() 			{ m_bSelected=false;return mysql_store_result(m_hMysql); }

	// Attributes
	MYSQL* Handle() 					{ return m_hMysql; }
	MYSQL* operator () (MyDb& db) 		{ return db.Handle(); }

	bool IsConnected() 					{ return m_bConnected; }

	int InsertID();

protected:
	MYSQL*	Detach();
protected:
	MYSQL* 	m_hMysql;
	bool 	m_bConnected;
	bool	m_bSelected;
};

////////////////////////////////////////////////////////
//
class MyRes
{
public:
	// Constructions
	MyRes() 					{ m_hRes = NULL; }
	MyRes(MYSQL_RES* res_ptr) 	{ m_hRes = res_ptr; }
	~MyRes()					{ Free(); }

	// Operations
	MyRow Fetch()			{ return mysql_fetch_row(m_hRes); }
	//MyRow Next();
	my_ulonglong Count()  	{ return mysql_num_rows(m_hRes); }

	void Free();

	// Attributes
	MYSQL_RES* Handle()		{ return m_hRes; }
	MYSQL_RES* operator () (MyRes& res)
							{ return res.Handle(); }
    MYSQL_RES* operator = (MYSQL_RES* res_ptr)
    						{ Free(); m_hRes=res_ptr;return m_hRes; }

	bool IsEmpty()			{ return ( Count()==0 ); }
	bool IsEOF()			{ return mysql_eof(m_hRes)!=0; }

	unsigned int DataLength(int FieldNo);

	unsigned int FetchLengths () {	return *mysql_fetch_lengths(m_hRes);	}


protected:
	MYSQL_RES* m_hRes;
};

/*
class MyRow
{
public:
	char* operator [] (int FieldNo);
protected:
	char** m_hRow;
};
*/

///////////////////////////////////////////////////////////////////////
// inline functions start from here

// MyDb Section
inline int MyDb::Open(const char* Host, const char* UserName, const char* Password, const char* DatabaseName)
{
	if( m_bConnected ) 	Close();

	m_hMysql = mysql_init( NULL );

	if( mysql_real_connect(m_hMysql,Host,UserName,Password,DatabaseName,0,NULL,0) )
		m_bConnected = true;
	else
		m_bConnected = false;

	if( !m_bConnected ) {
		Close();
		return -1;
	}

	return MY_SUCCESS;
}

inline void MyDb::Close()
{
	if( m_hMysql )  {
		mysql_close(m_hMysql);
		m_hMysql = NULL;
	}
	m_bConnected = false;
}


inline int MyDb::Query(const char* Command, int CmdLen)
{
	if( m_bSelected ) { MyRes res=Store();res.Free(); }

	int cmd_len = CmdLen;
	if( cmd_len == 0 ) cmd_len = strlen(Command);

	int ret = mysql_real_query(m_hMysql,Command,cmd_len);
	m_bSelected = (mysql_field_count(m_hMysql) != 0);
	return ret;
}

inline int MyDb::VQuery(unsigned int BufferLen, const char* fmt, ...)
{
	char *cmd_ptr = new char[BufferLen];
    int ret;
    try{
        va_list ap;
        va_start(ap,fmt);
        vsnprintf(cmd_ptr,BufferLen,fmt,ap);
        va_end(ap);

        ret = Query(cmd_ptr, strlen(cmd_ptr));
    }
    catch(...)
    {
        ret = -1000;
    }

	delete[] cmd_ptr;
	return ret;
}

inline MYSQL* MyDb::Detach()
{
	MYSQL* hMysql=m_hMysql;
	m_hMysql=NULL;
	m_bConnected=false;
	return hMysql;
}

inline int MyDb::InsertID()
{
/*    if( Query("select LAST_INSERT_ID()") ) return 0;
    MyRes res = Store();
    MyRow row = res.Fetch();
    int id = (row[0])? atoi(row[0]) : 0;
    return id;*/
    return (int)mysql_insert_id(m_hMysql);
}

// MyRes Section
inline void MyRes::Free()
{
	if(m_hRes) {
		mysql_free_result(m_hRes);
		m_hRes = 0;
    }
}

inline unsigned int MyRes::DataLength(int FieldNo)
{
	MYSQL_FIELD* mf=mysql_fetch_field_direct(m_hRes, FieldNo);
	return (mf)?mf->max_length:0;
}


//////////////////////////////////////////////////////////////////
// Auto Link Section
#ifdef _MSC_VER
	#pragma comment(lib,"libMysql.lib")
	#pragma message("--> Automatically linking with libMysql.dll")
#endif
///////////////////////////////////////////////////////////////////

#endif //__MYPLUS_H__
