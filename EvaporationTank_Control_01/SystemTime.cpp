#include "StdAfx.h"
#include "SystemTime.h"

// Class constructor
SystemTime::SystemTime(void)
{
	lptrSystemTime = new SYSTEMTIME;
	ptrSystemTime = new systemTime;

	ptrSystemTime->Year = 0;
	ptrSystemTime->Month = 0;
	ptrSystemTime->DayOfWeek = 0;
	ptrSystemTime->Day = 0;
	ptrSystemTime->Hour = 0;
	ptrSystemTime->Minute = 0;
	ptrSystemTime->Second = 0;
	ptrSystemTime->Milliseconds = 0;

	lptrLocalTime = new SYSTEMTIME;
	ptrLocalTime = new localTime;

	ptrLocalTime->Year = 0;
	ptrLocalTime->Month = 0;
	ptrLocalTime->DayOfWeek = 0;
	ptrLocalTime->Day = 0;
	ptrLocalTime->Hour = 0;
	ptrLocalTime->Minute = 0;
	ptrLocalTime->Second = 0;
	ptrLocalTime->Milliseconds = 0;
}

// Class destructor
SystemTime::~SystemTime(void)
{
	delete lptrSystemTime;
	lptrSystemTime = NULL;

	delete ptrSystemTime;
	ptrSystemTime = NULL;

	delete lptrLocalTime;
	lptrLocalTime = NULL;

	delete ptrLocalTime;
	ptrLocalTime = NULL;
}

// Returns a time in seconds based on total milliseconds since system startup
double SystemTime::GetSystemTimeInSeconds(void)
{
	// Get current system time in milliseconds and convert to seconds
	DWORD sysTime;
	double seconds = 0.0;
	sysTime = timeGetTime();
	seconds = ((double)sysTime)/1000.0;
	return seconds;
}

// Returns a string with the system (UTC) date and time formatted for a file name
std::string SystemTime::GetSystemTimeToSecondString()
{
	// Clear system time string stream
	systemTimeStringStream.str("");

	GetSystemTime(lptrSystemTime);
	// Parse Year
	systemTimeStringStream << "date_" << (int)lptrSystemTime->wYear;
	// Parse Month
	int month = (int)lptrSystemTime->wMonth;
	if (month > 9 && month < 13) systemTimeStringStream << "_" << (int)lptrSystemTime->wMonth;
	else systemTimeStringStream << "_0" << (int)lptrSystemTime->wMonth;
	// Parse Day
	int day = (int)lptrSystemTime->wDay;
	if (day > 9 && day < 32) systemTimeStringStream << "_" << (int)lptrSystemTime->wDay;
	else systemTimeStringStream << "_0" << (int)lptrSystemTime->wDay;
	// Parse Hour
	int hour = (int)lptrSystemTime->wHour;
	if (hour > 9 && hour < 25) systemTimeStringStream << "_time_" << (int)lptrSystemTime->wHour;
	else systemTimeStringStream << "_time_0" << (int)lptrSystemTime->wHour;
	// Parse Minute
	int minute = (int)lptrSystemTime->wMinute;
	if (minute > 9 && minute < 61) systemTimeStringStream << "_" << (int)lptrSystemTime->wMinute;
	else systemTimeStringStream << "_0" << (int)lptrSystemTime->wMinute;
	// Parse Second
	int second = (int)lptrSystemTime->wSecond;
	if (second > 9 && second < 61) systemTimeStringStream << "_" << (int)lptrSystemTime->wSecond;
	else systemTimeStringStream << "_0" << (int)lptrSystemTime->wSecond;
	
	return systemTimeStringStream.str();
}

// Sets systemTime struct with current UTC date and time as integers
void SystemTime::SetSystemTimeStruct()
{
	GetSystemTime(lptrSystemTime);
	ptrSystemTime->Year = (int)lptrSystemTime->wYear;
	ptrSystemTime->Month = (int)lptrSystemTime->wMonth;
	ptrSystemTime->DayOfWeek = (int)lptrSystemTime->wDayOfWeek;
	ptrSystemTime->Day = (int)lptrSystemTime->wDay;
	ptrSystemTime->Hour = (int)lptrSystemTime->wHour;
	ptrSystemTime->Minute = (int)lptrSystemTime->wMinute;
	ptrSystemTime->Second = (int)lptrSystemTime->wSecond;
	ptrSystemTime->Milliseconds = (int)lptrSystemTime->wMilliseconds;
}

int SystemTime::GetSystemTimeYear()
{
	return ptrSystemTime->Year;
}

int SystemTime::GetSystemTimeMonth()
{
	return ptrSystemTime->Month;
}

int SystemTime::GetSystemTimeDayOfWeek()
{
	return ptrSystemTime->DayOfWeek;
}

int SystemTime::GetSystemTimeDay()
{
	return ptrSystemTime->Day;
}

int SystemTime::GetSystemTimeHour()
{
	return ptrSystemTime->Hour;
}

int SystemTime::GetSystemTimeMinute()
{
	return ptrSystemTime->Minute;
}

int SystemTime::GetSystemTimeSecond()
{
	return ptrSystemTime->Second;
}

int SystemTime::GetSystemTimeMillisecond()
{
	return ptrSystemTime->Milliseconds;
}

void SystemTime::Wait(unsigned milliseconds)
{
	Sleep(milliseconds);
}

// Sets localTime struct with current local date and time as integers
void SystemTime::SetLocalTimeStruct()
{
	GetLocalTime(lptrLocalTime);
	ptrLocalTime->Year = (int)lptrLocalTime->wYear;
	ptrLocalTime->Month = (int)lptrLocalTime->wMonth;
	ptrLocalTime->DayOfWeek = (int)lptrLocalTime->wDayOfWeek;
	ptrLocalTime->Day = (int)lptrLocalTime->wDay;
	ptrLocalTime->Hour = (int)lptrLocalTime->wHour;
	ptrLocalTime->Minute = (int)lptrLocalTime->wMinute;
	ptrLocalTime->Second = (int)lptrLocalTime->wSecond;
	ptrLocalTime->Milliseconds = (int)lptrLocalTime->wMilliseconds;
}

int SystemTime::GetLocalTimeYear()
{
	return ptrLocalTime->Year;
}

int SystemTime::GetLocalTimeMonth()
{
	return ptrLocalTime->Month;
}

int SystemTime::GetLocalTimeDayOfWeek()
{
	return ptrLocalTime->DayOfWeek;
}

int SystemTime::GetLocalTimeDay()
{
	return ptrLocalTime->Day;
}

int SystemTime::GetLocalTimeHour()
{
	return ptrLocalTime->Hour;
}

int SystemTime::GetLocalTimeMinute()
{
	return ptrLocalTime->Minute;
}

int SystemTime::GetLocalTimeSecond()
{
	return ptrLocalTime->Second;
}

int SystemTime::GetLocalTimeMillisecond()
{
	return ptrLocalTime->Milliseconds;
}
