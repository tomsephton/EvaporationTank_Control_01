#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <xstring>
#include <sstream>

class SystemTime
{
	private:
//	mmtime_tag * ptrSystemTime;
	LPSYSTEMTIME lptrSystemTime;   // Windows system time struct
	LPSYSTEMTIME lptrLocalTime;   // Windows system time struct

	struct systemTime
	{ 
		int Year; 
		int Month; 
		int DayOfWeek; 
		int Day; 
		int Hour; 
		int Minute; 
		int Second; 
		int Milliseconds; 
	};

	systemTime* ptrSystemTime;

	struct localTime
	{ 
		int Year; 
		int Month; 
		int DayOfWeek; 
		int Day; 
		int Hour; 
		int Minute; 
		int Second; 
		int Milliseconds; 
	};

	localTime* ptrLocalTime;

	std::ostringstream systemTimeStringStream;

public:
	SystemTime(void);
	virtual ~SystemTime(void);

	double GetSystemTimeInSeconds();
	std::string GetSystemTimeToSecondString();

	void SetSystemTimeStruct();
	int GetSystemTimeYear();
	int GetSystemTimeMonth();
	int GetSystemTimeDayOfWeek();
	int GetSystemTimeDay();
	int GetSystemTimeHour();
	int GetSystemTimeMinute();
	int GetSystemTimeSecond();
	int GetSystemTimeMillisecond();

	void SetLocalTimeStruct();
	int GetLocalTimeYear();
	int GetLocalTimeMonth();
	int GetLocalTimeDayOfWeek();
	int GetLocalTimeDay();
	int GetLocalTimeHour();
	int GetLocalTimeMinute();
	int GetLocalTimeSecond();
	int GetLocalTimeMillisecond();

	void Wait(unsigned milliseconds);
};

