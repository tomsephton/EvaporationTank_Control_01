#pragma once

#include <fstream>
#include <iostream>

#include "SystemTime.h"
#include "LogData.h"

#define  MAXSTRINGLENGTH 80

class LogFileOutput
{
private:
	LogData* logDataArray[DATAARRAYSIZE];
	SystemTime* ptrSystemTime;
	double startRunTime;

	std::ostringstream strstrLogFileName;
	std::string stringLogFileName;

	// Sets up output to log file
	std::ofstream logFile;

protected:
	bool wroteColumnLabels;

public:
	LogFileOutput();
	virtual ~LogFileOutput();

	void SetDataItem(int arrayIndex, char* name, bool value);
	void SetDataItem(int arrayIndex, char* name, short value);
	void SetDataItem(int arrayIndex, char* name, int value);
	void SetDataItem(int arrayIndex, char* name, float value);
	void SetDataItem(int arrayIndex, char* name, double value);
	void SetDataItem(int arrayIndex, char* name, char* value);
	void SetDataItem(int arrayIndex, char* name, const char* value);
	void SetDataItem(int arrayIndex, char* name, std::string value);

	void WriteColumnLabels(void);
	bool GetColumnLabelsWritten(void);
	void WriteCurrentData(void);
	void PrintAllData(void);
	void PrintSelectedData(void);
};

