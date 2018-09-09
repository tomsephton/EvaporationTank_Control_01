#pragma once

#include "LogFileOutput.h"

class PumpLogFileOutput : public LogFileOutput
{
private:
	LogData* logDataArray[DATAARRAYSIZE];
	SystemTime* ptrSystemTime;
	double startRunTime;

	std::ostringstream strstrPumpLogFileName;
	std::string stringPumpLogFileName;

	// Sets up output to log file
	std::ofstream pumpLogFile;

public:
	PumpLogFileOutput(std::string pumpID);
	virtual ~PumpLogFileOutput(void);

	void SetDataItem(int arrayIndex, std::string name, bool value);
	void SetDataItem(int arrayIndex, std::string name, short value);
	void SetDataItem(int arrayIndex, std::string name, int value);
	void SetDataItem(int arrayIndex, std::string name, float value);
	void SetDataItem(int arrayIndex, std::string name, double value);
	void SetDataItem(int arrayIndex, std::string name, char* value);
	void SetDataItem(int arrayIndex, std::string name, const char* value);
	void SetDataItem(int arrayIndex, std::string name, std::string value);

	void WriteColumnLabels(void);
	bool GetColumnLabelsWritten(void);
	void WriteCurrentData(void);
};

