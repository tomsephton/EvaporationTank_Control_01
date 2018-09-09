#include "StdAfx.h"
#include "PumpLogFileOutput.h"

PumpLogFileOutput::PumpLogFileOutput(std::string pumpID)
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

	// Pump log file
	strstrPumpLogFileName << "e:\\EvapTank_Log_Files\\EvapTank" << pumpID << "_PumpLogfile_" << ptrSystemTime->GetSystemTimeToSecondString() << "_UTC.txt";
	stringPumpLogFileName = strstrPumpLogFileName.str();
	pumpLogFile.open(stringPumpLogFileName.c_str());

	wroteColumnLabels = false;
}

PumpLogFileOutput::~PumpLogFileOutput(void)
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

void PumpLogFileOutput::SetDataItem(int arrayIndex, std::string name, bool value)
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

void PumpLogFileOutput::SetDataItem(int arrayIndex, std::string name, short value)
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

void PumpLogFileOutput::SetDataItem(int arrayIndex, std::string name, int value)
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

void PumpLogFileOutput::SetDataItem(int arrayIndex, std::string name, float value)
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

void PumpLogFileOutput::SetDataItem(int arrayIndex, std::string name, double value)
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

void PumpLogFileOutput::SetDataItem(int arrayIndex, std::string name, char* value)
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

void PumpLogFileOutput::SetDataItem(int arrayIndex, std::string name, const char* value)
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

void PumpLogFileOutput::SetDataItem(int arrayIndex, std::string name, std::string value)
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

void PumpLogFileOutput::WriteColumnLabels(void)
{
	// Write data line to tab delimited log file
	pumpLogFile << "Date (UTC)" << "\t";
	pumpLogFile << "Time (UTC)" << "\t" ;
	pumpLogFile << "Run Time (sec)" << "\t";
	SetDataItem(0, "Operator Input Event", "Start Process Control Program");
	for (int arrayIndex = 0; arrayIndex < DATAARRAYSIZE; arrayIndex++)
	{
		switch (logDataArray[arrayIndex]->GetDataType())
		{
		case 0:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		case 1:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		case 2:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		case 3:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		case 4:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		case 5:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
			break;
		default:
			pumpLogFile << "\t";
			break;
		}

	}
	pumpLogFile << std::endl;
	wroteColumnLabels = true;
}

bool PumpLogFileOutput::GetColumnLabelsWritten(void)
{
	return wroteColumnLabels;
}

void PumpLogFileOutput::WriteCurrentData(void)
{
	// Set first column
	SetDataItem(0, "Event", "Set Pump Rate");
	// Set current values for time and date
	ptrSystemTime->SetSystemTimeStruct();
	ptrSystemTime->SetLocalTimeStruct();

	// Write data line to tab delimited log file
	pumpLogFile << ptrSystemTime->GetSystemTimeYear() << "_" << ptrSystemTime->GetSystemTimeMonth() << "_" << ptrSystemTime->GetSystemTimeDay() << "\t";
	pumpLogFile << ptrSystemTime->GetSystemTimeHour() << ":" << ptrSystemTime->GetSystemTimeMinute() << ":" << ptrSystemTime->GetSystemTimeSecond() << "." << ptrSystemTime->GetSystemTimeMillisecond() << "\t";
	pumpLogFile << ptrSystemTime->GetSystemTimeInSeconds() - startRunTime << "\t";
	for (int arrayIndex = 0; arrayIndex < DATAARRAYSIZE; arrayIndex++)
	{
		switch (logDataArray[arrayIndex]->GetDataType())
		{
		case 0:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetBoolValue() << "\t";
			break;
		case 1:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetShortValue() << "\t";
			break;
		case 2:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetIntValue() << "\t";
			break;
		case 3:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetFloatValue() << "\t";
			break;
		case 4:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetDoubleValue() << "\t";
			break;
		case 5:
			pumpLogFile << logDataArray[arrayIndex]->LogData::GetStringValue() << "\t";
			break;
		default:
			pumpLogFile << "\t";
			break;
		}
	}
	pumpLogFile << std::endl;
}
