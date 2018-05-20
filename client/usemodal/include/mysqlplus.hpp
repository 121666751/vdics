 
#include <iostream.h> 
#include <windows.h> 
#include <mysqlplus.hpp> 
 
int main(int argc, char* argv[]) 
{ 
	Mysql_Db db; 
	db.Open("localhost","root",""); 
	db.Select_Db("test"); 
	db.Query("select * from clerk"); 
 
	Mysql_Res res; 
	res.StoreResult(&db); 
 
	MYSQL_ROW row; 
	 
	cout<< (int)res.NumRows()<<endl; 
 
	while( (row=res.FetchRow()) ) 
	{ 
		cout<<row[0]<<" "<<row[1]<<endl; 
	} 
 
	return 0; 
} 
 
------------------------------------------------------------------- 

***************************************************************************************/

#ifndef MYSQL_PLUS_H__
#define MYSQL_PLUS_H__


/*  Include Needed Header  */     
#include <zsocket.h>
#include <stdarg.h>
#include <zstandard.h>



#include <Mysql/mysql.h>
#if defined(GNU_PTH)
	#include <pth.h>
#elif defined(POSIX_THREAD)
	#include <pthread.h>
#elif defined(WIN32)
#endif //GNU_PTH

#if defined( __BCC32__ )
	#define z_ulonglong int
#elif defined( WIN32 )
	typedef unsigned __int64 z_ulonglong;
	#define vsnprintf _vsnprintf
#else
	typedef unsigned long long z_ulonglong;
#endif // WIN32


// ------------------------------------------------------------------------
#ifdef GNU_PTH
extern pth_mutex_t mysql_init_lock;
#elif POSIX_THREAD
extern pthread_mutex_t mysql_init_lock;
#endif
// ------------------------------------------------------------------------

/* Mysql Database Connection Class */
class Mysql_Db {

protected:
	MYSQL *mysql;
	
private:
	bool is_connected;

public:
	bool IsConnected() {
		return is_connected;
	}

	/*
	int Open(const char *host, const char *user, const char *passwd) {
		if(mysql_connect(mysql, host, user, passwd)) 
			is_connected = 1; 
		else
			is_connected = 0;
		return is_connected;
	}*/
 
	bool Open( const char *host, const char *user, const char *passwd, const char *db = NULL, 
				unsigned int port = 0, const char *unix_sock = NULL, unsigned int flag = 0 ) 
	{
		if( mysql != NULL && is_connected == true ) return true;
//-----------------------------------------------------------
#if defined( GNU_PTH )
	pth_mutex_acquire( &mysql_init_lock, FALSE, NULL );
#elif defined( POSIX_THREAD_LOCK )
	pthread_mutex_lock( &mysql_init_lock );
#endif
//------------------------------------------------------------
		Close();

		mysql = mysql_init( NULL );
		if(mysql_real_connect(mysql,host,user,passwd,db,port,unix_sock,flag)) 
			is_connected = true; 
		else
			is_connected = false;

//------------------------------------------------------------
#if defined( GNU_PTH )
	pth_mutex_release( &mysql_init_lock );
#elif defined( POSIX_THREAD_LOCK )
	pthread_mutex_lock( &mysql_init_lock );
#endif
//------------------------------------------------------------
		return is_connected;
	}

	void Close() {
		if( mysql )  {
			mysql_close(mysql); 
			mysql = NULL;
		}
		is_connected = false;
	}

	Mysql_Db () { 
		is_connected = false;
		mysql = NULL;
	}

	~Mysql_Db () {
		Close(); 
	}

	z_ulonglong getAffectedRows() {
		return mysql_affected_rows(mysql);
	}

	int CreateDatabase(const char *db) {
		return mysql_create_db(mysql, db);
	}

	int DropDatabase(const char *db) {
		return mysql_drop_db(mysql, db);
	}

	char* getError () {
		return mysql_error(mysql);
	}

	char* getClientInfo() {
		return mysql_get_client_info();
	}

	char* getHostInfo() {
		return mysql_get_host_info(mysql);
	}

	char* getServerInfo() {
		return mysql_get_server_info(mysql);
	}

	char* getInfo() {
		return mysql_info(mysql);
	}

	/* return zero if successful, non-zero if an error occurred! */
	int Query(const char *cmd) {
		return mysql_query (mysql, cmd);
	}

	int QueryEx(const char *cmd, unsigned int length) {
		return mysql_real_query (mysql, cmd, length);
	}

	int VQuery(unsigned int cmdlen, const char *fmt, ...) {
		char *cmd_ptr = new char[cmdlen];
		va_list ap;
		va_start(ap,fmt);
		vsnprintf(cmd_ptr,cmdlen,fmt,ap);
		va_end(ap);

		int ret = QueryEx(cmd_ptr, strlen(cmd_ptr));
		
		delete[] cmd_ptr;
		return ret;
	}

	int Reload() {
		return mysql_reload(mysql);
	}

	int Select_Db(const char *db) {
		return mysql_select_db(mysql,db);
	}

	int Shutdown() {
		return mysql_shutdown(mysql);
	}

	char* getStat() {
		return mysql_stat(mysql);
	}

	MYSQL* getHandle() {
		return mysql;
	}

};

/* Mysql Result class */
class Mysql_Res {
private:
	MYSQL_RES *mysql_res;
public:
	void FreeRes() {
		if(mysql_res)
			mysql_free_result(mysql_res);
		mysql_res = 0;
	}

	Mysql_Res () {
		mysql_res = 0;
	}

	~Mysql_Res () {
		if(mysql_res)
			FreeRes();
	}

	int IsEof () {
		return mysql_eof(mysql_res);
	}

	void Move(unsigned int offset) {
		mysql_data_seek(mysql_res, offset);
	}

	MYSQL_FIELD *FetchField () {
		return mysql_fetch_field(mysql_res);
	}

	unsigned int FetchLengths () {
		return *mysql_fetch_lengths(mysql_res);
	}

	MYSQL_ROW FetchRow() {
		return mysql_fetch_row(mysql_res);
	}

	void FieldSeek(int field) {
		mysql_field_seek (mysql_res, field);
	}

	// Return the number of fields in current result
	unsigned int getFieldCount() {
		return mysql_num_fields(mysql_res);
	}

	// Return the number of records in current result
	z_ulonglong getRecordCount() {
		return mysql_num_rows(mysql_res);
	}

	MYSQL_RES* getHandle() {
		return mysql_res;
	}

	void StoreResult(Mysql_Db* mdb) {
		StoreResult(mdb->getHandle());
	}

	void StoreResult(MYSQL* hMysql) {
		FreeRes();
		mysql_res = mysql_store_result(hMysql);
	}

	void UseResult(MYSQL* hMysql) {
		FreeRes();
		mysql_res = mysql_use_result(hMysql);
	}


	// Return non zero if succeed, and zero when failed
	unsigned int getFieldDataLen(unsigned int fieldnr) {
		if( fieldnr > getFieldCount()-1 ) return 0;
#ifdef ZMYSQL_V3_21
		MYSQL_FIELD mf;
		mf = mysql_fetch_field_direct(mysql_res, fieldnr);
		return mf.max_length;
#else		
		MYSQL_FIELD *mf;
		mf = mysql_fetch_field_direct(mysql_res, fieldnr);
		return mf->max_length;
#endif		
	}
};


#endif // MYSQL_PLUS_H__


