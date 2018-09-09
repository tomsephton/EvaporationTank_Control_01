#include "StdAfx.h"
#include "TankFillLogFileOutput.h"

TankFillLogFileOutput::TankFillLogFileOutput(std::string tankID)
{
	// Initialize array of dataItems to hold current output data
	int arrayIndex;
	for (arrayIndex = 0; arrayIndex < DATAARRAYSIZE; arrayIndex++)
	{
		logDataArray[arrayIndex] = new LogData(arrayIndex);
	}

	// Instantiate SystemTime object
	ptrSystemTime = new SystemTime();
	startRunTime = ptrSystemTime->GetSystemTimeInSeconds();
	ptrSystemTime->SetSystemTimeStruct();

	// Tank Fill log file
	strstrTankFillLogFileName << "e:\\EvapTank_Log_Files\\EvapTank" << tankID << "_FillLogfile_" << ptrSystemTime->GetSystemTimeToSecondString() << "_UTC.txt";
	stringTankFillLogFileName = strstrTankFillLogFileName.str();
	tankFillLogFile.open(stringTankFillLogFileName.c_str());

	wroteColumnLabels = false;
}

TankFillLogFileOutput::~TankFillLogFileOutput(void)
{
	int arrayIndex;
	// Delete data array content
	for (arrayIndex = 0; arrayIndex < DATAARRAYSIZE; arrayIndex++)
	{
		delete logDataArray[arrayIndex];
		logDataArray[arrayIndex] = NULL;
	}

	// Delete SystemTime object
	delete ptrSystemTime;
	ptrSystemTime = NULL;
}

void TankFillLogFileOutput::SetDataItem(int arrayIndex, std::string name, bool value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (name.length() <= MAXSTRINGLENGTH)
		{
			// Convert name string to char*
			char * cstrName = new char [name.length()+1];
			std::strcpy (cstrName, name.c_str());
			logDataArray[arrayIndex]->LogData::SetData(cstrName, value);
		}
	}
}

void TankFillLogFileOutput::SetDataItem(int arrayIndex, std::string name, short value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (name.length() <= MAXSTRINGLENGTH)
		{
			// Convert name string to char*
			char * cstrName = new char [name.length()+1];
			std::strcpy (cstrName, name.c_str());
			logDataArray[arrayIndex]->LogData::SetData(cstrName, value);
		}
	}
}

void TankFillLogFileOutput::SetDataItem(int arrayIndex, std::string name, int value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (name.length() <= MAXSTRINGLENGTH)
		{
			// Convert name string to char*
			char * cstrName = new char [name.length()+1];
			std::strcpy (cstrName, name.c_str());
			logDataArray[arrayIndex]->LogData::SetData(cstrName, value);
		}
	}
}

void TankFillLogFileOutput::SetDataItem(int arrayIndex, std::string name, float value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (name.length() <= MAXSTRINGLENGTH)
		{
			// Convert name string to char*
			char * cstrName = new char [name.length()+1];
			std::strcpy (cstrName, name.c_str());
			logDataArray[arrayIndex]->LogData::SetData(cstrName, value);
		}
	}
}

void TankFillLogFileOutput::SetDataItem(int arrayIndex, std::string name, double value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (name.length() <= MAXSTRINGLENGTH)
		{
			// Convert name string to char*
			char * cstrName = new char [name.length()+1];
			std::strcpy (cstrName, name.c_str());
			logDataArray[arrayIndex]->LogData::SetData(cstrName, value);
		}
	}
}

void TankFillLogFileOutput::SetDataItem(int arrayIndex, std::string name, char* value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (name.length() <= MAXSTRINGLENGTH)
		{
			if (strlen(value) <= MAXSTRINGLENGTH)
			{
				// Convert name string to char*
				char * cstrName = new char [name.length()+1];
				std::strcpy (cstrName, name.c_str());
				logDataArray[arrayIndex]->LogData::SetData(cstrName, value);
			}
		}
	}
}

void TankFillLogFileOutput::SetDataItem(int arrayIndex, std::string name, const char* value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (name.length() <= MAXSTRINGLENGTH)
		{
			if (strlen(value) <= MAXSTRINGLENGTH)
			{
				// Convert name string to char*
				char * cstrName = new char [name.length()+1];
				std::strcpy (cstrName, name.c_str());
				logDataArray[arrayIndex]->LogData::SetData(cstrName, value);
			}
		}
	}
}

void TankFillLogFileOutput::SetDataItem(int arrayIndex, std::string name, std::string value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (name.length() <= MAXSTRINGLENGTH)
		{
			if (value.length() <= MAXSTRINGLENGTH)
			{
				// Convert name string to char*
				char * cstrName = new char [name.length()+1];
				std::strcpy (cstrName, name.c_str());
				// Convert value string to char*
				char * cstrValue = new char [value.length()+1];
				std::strcpy (cstrValue, value.c_str());
				logDataArray[arrayIndex]->LogData::SetData(cstrName, cstrValue);
			}
		}
	}
}

void TankFillLogFileOutput::WriteColumnLabels(void)
{
	// Write data line to tab delimited log file
	tankFillLogFile << "Date (UTC)" << "\t";
	tankFillLogFile << "Time (UTC)" << "\t" ;
	tankFillLogFile << "Run Time (sec)" << "\t";
	SetDataItem(0, "Operator Input Event", "Start Process Control Program");
	for (int arrayIndex = 0; arrayIndex < DATAARRAYSIZE; arrayIndex++)
	{
		switch (logDataArray[arrayIndex]->GetDataType())
		{
		case 0:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		case 1:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		case 2:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		case 3:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		case 4:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		case 5:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		default:
			tankFillLogFile << "\t";
			break;
		}

	}
	tankFillLogFile << std::endl;
	wroteColumnLabels = true;
}

bool TankFillLogFileOutput::GetColumnLabelsWritten(void)
{
	return wroteColumnLabels;
}

void TankFillLogFileOutput::WriteCurrentData(void)
{
	// Set first column
	SetDataItem(0, "Event", "Tank Fill");
	// Set current values for time and date
	ptrSystemTime->SetSystemTimeStruct();
	ptrSystemTime->SetLocalTimeStruct();

	// Write data line to tab delimited log file
	tankFillLogFile << ptrSystemTime->GetSystemTimeYear() << "_" << ptrSystemTime->GetSystemTimeMonth() << "_" << ptrSystemTime->GetSystemTimeDay() << "\t";
	tankFillLogFile << ptrSystemTime->GetSystemTimeHour() << ":" << ptrSystemTime->GetSystemTimeMinute() << ":" << ptrSystemTime->GetSystemTimeSecond() << "." << ptrSystemTime->GetSystemTimeMillisecond() << "\t";
	tankFillLogFile << ptrSystemTime->GetSystemTimeInSeconds() - startRunTime << "\t";
	for (int arrayIndex = 0; arrayIndex < DATAARRAYSIZE; arrayIndex++)
	{
		switch (logDataArray[arrayIndex]->GetDataType())
		{
		case 0:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetBoolValue() << "\t";
			break;
		case 1:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetShortValue() << "\t";
			break;
		case 2:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetIntValue() << "\t";
			break;
		case 3:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetFloatValue() << "\t";
			break;
		case 4:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetDoubleValue() << "\t";
			break;
		case 5:
			tankFillLogFile << logDataArray[arrayIndex]->LogData::GetStringValue() << "\t";
			break;
		default:
			tankFillLogFile << "\t";
			break;
		}
	}
	tankFillLogFile << std::endl;
}