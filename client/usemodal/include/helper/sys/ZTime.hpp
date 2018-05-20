#ifndef __ZDATETIME_H__
#define __ZDATETIME_H__

#include <time.h>
#include <string>

/* By default,Exception will be throw when Error happend, you can #define Z_NO_EXCEPTION
to disable it.	
#ifndef Z_NO_EXCEPTION
#include "ZException.hpp"
#endif*/
//----------------------------------------------------------------------------------------

#include "zbase.hpp"

ZDEV_NAMESPACE_BEGIN

typedef double ztime_t;

/* Seconds and milliseconds per day */
const int ct_SecsPerDay = 24 * 60 * 60;
const int ct_MSecsPerDay = ct_SecsPerDay * 1000;

/* Days between 1/1/0001 and 12/31/1899 */
const int ct_DateDelta = 693594;

/* MonthDays */
static char ZMonthDays[2][12] = {	31,28,31,30,31,30,31,31,30,31,30,31,
									31,29,31,30,31,30,31,31,30,31,30,31		};

class ZDateTime
{
public:
	ZDateTime( ztime_t dInitTime ) { m_Value = dInitTime; }
	ZDateTime( bool bSetToNow = false )
	{
		if( bSetToNow )	Now();
		else m_Value = 0;
	}

	~ZDateTime() {}

	void SetValueDirect( ztime_t val )
	{
		m_Value = val;
	}

	ztime_t GetValueDirect() { return m_Value; }
	ztime_t GetTime() { return m_Value; }

	const ztime_t operator=(ZDateTime &zdt)
	{
		SetValueDirect( zdt.GetTime() );
		return m_Value;
	}

	const ztime_t operator=(const ztime_t dt)
	{
		m_Value = dt;
		return m_Value;
	}

	const ztime_t operator=(const char* str)
	{
		StrToDateTime(str);
		return m_Value;
	}

	operator ztime_t(void)
	{
		return m_Value;
	}

	/* YYYY-MM-DD */
	std::string DateString()
	{
		struct tm realtime;
		DecodeDateTime(&realtime);
		char s[20];
		sprintf(s,"%.4d-%.2d-%.2d",realtime.tm_year,realtime.tm_mon,realtime.tm_mday);

		return std::string(s);
	}
	/* hh:mm:ss */
	std::string TimeString()
	{
		struct tm realtime;
		DecodeDateTime(&realtime);
		char s[20];
		sprintf(s,"%.2d:%.2d:%.2d\0",realtime.tm_hour,realtime.tm_min,realtime.tm_sec);

		return std::string(s);
	}
	/* YYYY-MM-DD hh:mm:ss */
	std::string DateTimeString()
	{
		struct tm realtime;
		DecodeDateTime(&realtime);
		char s[30];
		sprintf(s,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d\0",
			realtime.tm_year,realtime.tm_mon,realtime.tm_mday,
			realtime.tm_hour,realtime.tm_min,realtime.tm_sec);

		return std::string(s);
	}

	bool Now() { return GetCurrentTime(); }

	/* Sun - 1, Mon - 2,...  */
	int GetDayOfWeek()
	{
		tm tmpTime;
		DecodeDateTime(&tmpTime);
		return tmpTime.tm_wday;
	}

	/* the string must has the format like "YYYY-MM-DD hh:mm:ss" */
	bool StrToDateTime(const char* timestr)
	{
		tm tst;
		StrToDateTime(timestr, &tst);
		return SetDateTime(tst.tm_year,tst.tm_mon,tst.tm_mday,tst.tm_hour,tst.tm_min,tst.tm_sec);
	}

	bool SetDateTime(unsigned short Year, unsigned short Month, unsigned short Day, unsigned short Hour, unsigned short Min,unsigned short Sec=0,unsigned short MSec=0)
	{
		ztime_t Date,Time;
		bool bResult;

		bResult = DoEncodeDate(Year,Month,Day, Date);
		if( !bResult )
		{
/*
// ----------------------------------
#ifndef Z_NO_EXCEPTION
			throw( ZException(-1) );
#endif //Z_NO_EXCEPTION
// ----------------------------------
*/
			return false;
		}

		bResult = DoEncodeTime(Hour,Min,Sec,MSec, Time);
		if( !bResult )
		{
/*
// ----------------------------------
#ifndef Z_NO_EXCEPTION
			throw( ZException(-1) );
#endif //Z_NO_EXCEPTION
// ----------------------------------
*/
			return false;
		}

		m_Value = Date+Time;
		return true;

	}

	bool GetCurrentTime()
	{
		bool bResult;
        struct tm *newtime;
        time_t long_time;


        time( &long_time );						/* Get time as long integer. */
        newtime = localtime( &long_time );		/* Convert to local time. */

		newtime->tm_year += 1900;
		newtime->tm_mon += 1;
		ztime_t Date,Time;
		bResult = DoEncodeDate(newtime->tm_year,newtime->tm_mon,newtime->tm_mday,Date);
		if( !bResult )
		{
/*
// ----------------------------------
#ifndef Z_NO_EXCEPTION
			throw( ZException(-1) );
#endif //Z_NO_EXCEPTION
// ----------------------------------
*/
			return false;
		}

		bResult = DoEncodeTime(newtime->tm_hour,newtime->tm_min,newtime->tm_sec,0, Time);
		if( !bResult )
		{
/*
// ----------------------------------
#ifndef Z_NO_EXCEPTION
			throw( ZException(-1) );
#endif //Z_NO_EXCEPTION
// ----------------------------------
*/
			return false;
		}

		m_Value = Date+Time;
		return true;

	}

	bool DecodeDateTime(struct tm *realtime)
	{
		int Date,Time;

		Date = int(m_Value);
		Time = int( (m_Value - Date)*ct_MSecsPerDay + 0.5 );

		if( Time<0 ) return false;

		const int D1 = 365;
		const int D4 = D1 * 4 + 1;
		const int D100 = D4 * 25 - 1;
		const int D400 = D100 * 4 + 1;
		unsigned short Y, M, D, I;
		int T, nDaysAbsolute;
		char*  DayTable;

		T = Date + ct_DateDelta; Y = 1; nDaysAbsolute = T;


		while( T >= D400 )
		{
			T -= D400;	Y += 400;
		}

		I = T/D100;	D = T%D100;

		if( I == 4 )
		{
			I -= 1;	D += D100;
		}

		Y = Y + I*100;
		I = D / D4;	D = D % D4;	Y = Y + I*4;
		I = D / D1;	D = D % D1;

		if( I == 4 )
		{
			I--; D += D1;
		}

		Y += I;

		// To solve problem of date 'XXXX-01-00 '
		if( D == 0 )
		{
			Y -= 1;
			D = IsLeapYear(Y)? 366:365;
		}
		//

		DayTable = ZMonthDays[IsLeapYear(Y)];
		M = 0;

		while(1)
		{
			I = DayTable[M];
			if( D<=I ) break;
			D -= I;	M++;
		}


		// Calculate the day of week (sun=1, mon=2...)
		// because 01/01/0001 is Sun.  +1 because we want 1-based
		realtime->tm_wday = (int)((nDaysAbsolute) % 7L) + 1;

		realtime->tm_year = Y;
		realtime->tm_mon = M+1;
		realtime->tm_mday = D;

		realtime->tm_hour = Time/3600000;
		realtime->tm_min = (Time-realtime->tm_hour*3600000)/60000;
		realtime->tm_sec = ((Time-realtime->tm_hour*3600000)%60000)/1000;

		return true;
	}

protected:
	ztime_t m_Value;

//////////////////////////////////////////////////////////////////////
private:

	bool IsLeapYear(unsigned short Year)
	{
		return (Year%4 == 0) && ((Year%100 != 0)||(Year%400 == 0));
	}

	bool DoEncodeDate(unsigned short Year,unsigned short Month,unsigned short Day,ztime_t& Date)
	{
		bool bResult = false;

		char* DayTable = ZMonthDays[IsLeapYear(Year)];

		if( (Year >= 1) && (Year <= 9999) && (Month >= 1) && (Month <= 12) &&
					(Day >= 1) && (Day <= DayTable[Month-1]) )
		{
			int I;
			for( I = 0 ; I < Month - 1; I++ )
				Day += DayTable[I];

			I = Year - 1;
			Date = I * 365 + I / 4 - I / 100 + I / 400 + Day - ct_DateDelta;
			bResult = true;
		}

		return bResult;
	}
	bool DoEncodeTime(unsigned short Hour,unsigned short Min,unsigned short Sec,unsigned short MSec, ztime_t& Time)
	{
		bool bResult = false;

		if( (Hour < 24) && (Min < 60) && (Sec < 60) && (MSec < 1000) )
		{
			Time = float(Hour * 3600000 + Min * 60000 + Sec * 1000 + MSec) / float(ct_MSecsPerDay);
			bResult = true;
		}

		return bResult;
	}

	char* findstr(char *srcstr, char *substr)
	{
		for(int i=0; i<(int)(strlen(srcstr)-strlen(substr)); i++)
		{
			if( strncmp(srcstr+i,substr,strlen(substr))==0 )  	return (srcstr+i);
		}

		return NULL;
	}

	////////////////////////////////////////////////////////////////////
	//input a string like	99-3-6 12:20:30
	//					or	99-3-6
	//					or	1999-3-6
	//					or 	99年3月6日
	//
	bool StrToDateTime(const char *timestr,tm *tst)
	{
		char	*msg1,*msg2,*msg,timeComplete=0;
		char    *pos;
        struct tm *newtime;
        time_t long_time;

		int i1,i2,i3;
		char* timestring = new char[strlen(timestr)+1];

        time( &long_time );						/* Get time as long integer. */
        newtime = localtime( &long_time );		/* Convert to local time. */

		memcpy( tst, newtime, sizeof(tm) );
		tst->tm_hour = 0;
		tst->tm_min = 0;
		tst->tm_sec = 0;
		strcpy(timestring,timestr);

		pos=findstr(timestring,"年");
		if( pos ){
			*pos='-';
			*(pos+1)='0';
		} //
		pos=findstr(timestring,"月");
		if( pos ){
			*pos='-';
			*(pos+1)='0';
		} //
		pos=findstr(timestring,"日");
		if( pos ){
			*pos=' ';
			*(pos+1)='0';
		} //

		msg1=timestring;
		msg=msg1;

		while(*msg!='\0')
		{
			if(*msg=='-'||*msg=='/')
			{
				*msg=' ';
			}
			else if(*msg==':')
			{
				*msg=' ';
				timeComplete=1;
			}

			else if(*msg==' ')
			{
				*msg='\0';
				msg2=msg+1;
			}
			msg++;
		};
		sscanf(msg1,"%d %d %d",&i1,&i2,&i3);
		if( i1 < 50 ) i1+=2000;
		if( i1>=50 && i1<100 ) i1+=1900;

		tst->tm_year = i1;
		tst->tm_mon = i2;
		tst->tm_mday = i3;

		if(timeComplete==1)
		{
			i1 = 0; i2 = 0; i3 = 0;
			sscanf(msg2,"%d %d %d",&i1,&i2,&i3);
			tst->tm_hour = i1;
			tst->tm_min = i2;
			tst->tm_sec = i3;
		}
		else
		{
			tst->tm_hour=0;
			tst->tm_min=0;
			tst->tm_sec=0;
		}

		delete[] timestring;

		return(true);
	}


};


//////////////////////////////////////////////////////////////////////////////////////////
// Some useful functions
/*
static char* z_itoa(int ival)
{
	char tmp[20];
	sprintf(tmp,"%d",ival);
	return tmp;
}
*/
inline ztime_t StrToZDateTime(char* timestr)
{
	ZDateTime dt;
	dt.StrToDateTime(timestr);
	return dt.GetTime();
}
//////////////////////////////////////////////////////////////////////////////////////////

/*
	Deta = LocalTime - RemoteTime;
*/

class ZClock : public ZDateTime
{
public:
	ZClock( ztime_t realtime = 0 ):ZDateTime(false)
		{
			if( realtime < 1 ) return;
			Adjust( realtime );
		}

	void SetDiff( ztime_t Deta ) { m_Deta = Deta; }

	void Adjust( ztime_t realtime )
		{
			ZDateTime::Now();
			m_Deta = m_Value-realtime;
		}

	ztime_t Now()
		{
			if( !ZDateTime::Now() ) return -1;
			m_Value = m_Value - m_Deta;
			return m_Value;
		}

protected:
	ztime_t m_Deta;
};
//----------------------------------------------------------------------------------------

#define z_str_to_datetime StrToZDateTime
#define ZDate ZDateTime
#define ZTime ZDateTime

ZDEV_NAMESPACE_END

#endif // __ZTIME_H__
