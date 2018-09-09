#pragma once

#include "LogFileOutput.h"

class TankFillLogFileOutput : public LogFileOutput
{
private:
	LogData* logDataArray[DATAARRAYSIZE];
	SystemTime* ptrSystemTime;
	double startRunTime;

	std::ostringstream strstrTankFillLogFileName;
	std::string stringTankFillLogFileName;

	// Sets up output to log file
	std::ofstream tankFillLogFile;

public:
	TankFillLogFileOutput(std::string tankID);
	virtual ~TankFillLogFileOutput(void);

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

