#include "StdAfx.h"
#include "LogFileOutput.h"

std::ostringstream strstrDateTime;
	std::string strDateTime = "";

// Class Constructor
LogFileOutput::LogFileOutput(void)
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

	// Main process log file
	strstrLogFileName << "e:\\EvapTank_Log_Files\\EvapTank_Logfile_" << ptrSystemTime->GetSystemTimeToSecondString() << "_UTC.txt";
	stringLogFileName = strstrLogFileName.str();
	logFile.open(stringLogFileName.c_str());

	wroteColumnLabels = false;
}

// Class Destructor
LogFileOutput::~LogFileOutput(void)
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

void LogFileOutput::SetDataItem(int arrayIndex, char* name, bool value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (strlen(name) <= MAXSTRINGLENGTH)
		{
			logDataArray[arrayIndex]->LogData::SetData(name, value);
		}
	}
}

void LogFileOutput::SetDataItem(int arrayIndex, char* name, short value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (strlen(name) <= MAXSTRINGLENGTH)
		{
			logDataArray[arrayIndex]->LogData::SetData(name, value);
		}
	}
}

void LogFileOutput::SetDataItem(int arrayIndex, char* name, int value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (strlen(name) <= MAXSTRINGLENGTH)
		{
			logDataArray[arrayIndex]->LogData::SetData(name, value);
		}
	}
}

void LogFileOutput::SetDataItem(int arrayIndex, char* name, float value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (strlen(name) <= MAXSTRINGLENGTH)
		{
			logDataArray[arrayIndex]->LogData::SetData(name, value);
		}
	}
}

void LogFileOutput::SetDataItem(int arrayIndex, char* name, double value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (strlen(name) <= MAXSTRINGLENGTH)
		{
			logDataArray[arrayIndex]->LogData::SetData(name, value);
		}
	}
}

void LogFileOutput::SetDataItem(int arrayIndex, char* name, char* value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (strlen(name) <= MAXSTRINGLENGTH)
		{
			if (strlen(value) <= MAXSTRINGLENGTH)
			{
				logDataArray[arrayIndex]->LogData::SetData(name, value);
			}
		}
	}
}

void LogFileOutput::SetDataItem(int arrayIndex, char* name, const char* value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (strlen(name) <= MAXSTRINGLENGTH)
		{
			if (strlen(value) <= MAXSTRINGLENGTH)
			{
				logDataArray[arrayIndex]->LogData::SetData(name, value);
			}
		}
	}
}

void LogFileOutput::SetDataItem(int arrayIndex, char* name, std::string value)
{
	if (arrayIndex >= 0 && arrayIndex < DATAARRAYSIZE)
	{
		if (strlen(name) <= MAXSTRINGLENGTH)
		{
			if (value.length() <= MAXSTRINGLENGTH)
			{
				// Convert value string to char*
				char * cstrValue = new char [value.length()+1];
				std::strcpy (cstrValue, value.c_str());
				logDataArray[arrayIndex]->LogData::SetData(name, cstrValue);
			}
		}
	}
}

void LogFileOutput::WriteColumnLabels(void)
{
	// Write data line to tab delimited log file
	logFile << "Date (UTC)" << "\t";
	logFile << "Time (UTC)" << "\t" ;
	logFile << "Run Time (sec)" << "\t";
	SetDataItem(0, "Operator Input Event", "Start Process Control Program");
	for (int arrayIndex = 0; arrayIndex < DATAARRAYSIZE; arrayIndex++)
	{
		switch (logDataArray[arrayIndex]->GetDataType())
		{
		case 0:
			logFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
//			logFile << logDataArray[arrayIndex]->GetBoolValue() << "\t";
			break;
		case 1:
			logFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
//			logFile << logDataArray[arrayIndex]->GetShortValue() << "\t";
			break;
		case 2:
			logFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
//			logFile << logDataArray[arrayIndex]->GetIntValue() << "\t";
			break;
		case 3:
			logFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
//			logFile << logDataArray[arrayIndex]->GetFloatValue() << "\t";
			break;
		case 4:
			logFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
//			logFile << logDataArray[arrayIndex]->GetDoubleValue() << "\t";
			break;
		case 5:
			logFile << logDataArray[arrayIndex]->LogData::GetItemName() << "\t";
//			logFile << logDataArray[arrayIndex]->GetStringValue() << "\t";
			break;
		default:
			logFile << "\t";
			break;
		}

	}
	logFile << std::endl;
	wroteColumnLabels = true;
}

bool LogFileOutput::GetColumnLabelsWritten(void)
{
	return wroteColumnLabels;
}

void LogFileOutput::WriteCurrentData(void)
{
	// Set current values for time and date
	ptrSystemTime->SetSystemTimeStruct();
	ptrSystemTime->SetLocalTimeStruct();

	// Write data line to tab delimited log file
	logFile << ptrSystemTime->GetSystemTimeYear() << "_" << ptrSystemTime->GetSystemTimeMonth() << "_" << ptrSystemTime->GetSystemTimeDay() << "\t";
	logFile << ptrSystemTime->GetSystemTimeHour() << ":" << ptrSystemTime->GetSystemTimeMinute() << ":" << ptrSystemTime->GetSystemTimeSecond() << "." << ptrSystemTime->GetSystemTimeMillisecond() << "\t";
	logFile << ptrSystemTime->GetSystemTimeInSeconds() - startRunTime << "\t";
	for (int arrayIndex = 0; arrayIndex < DATAARRAYSIZE; arrayIndex++)
	{
		switch (logDataArray[arrayIndex]->GetDataType())
		{
		case 0:
			logFile << logDataArray[arrayIndex]->LogData::GetBoolValue() << "\t";
			break;
		case 1:
			logFile << logDataArray[arrayIndex]->LogData::GetShortValue() << "\t";
			break;
		case 2:
			logFile << logDataArray[arrayIndex]->LogData::GetIntValue() << "\t";
			break;
		case 3:
			logFile << logDataArray[arrayIndex]->LogData::GetFloatValue() << "\t";
			break;
		case 4:
			logFile << logDataArray[arrayIndex]->LogData::GetDoubleValue() << "\t";
			break;
		case 5:
			logFile << logDataArray[arrayIndex]->LogData::GetStringValue() << "\t";
			break;
		default:
			logFile << "\t";
			break;
		}
	}
	logFile << std::endl;
}

void LogFileOutput::PrintAllData(void)
{
	// Set current values for time and date
	ptrSystemTime->SetSystemTimeStruct();
	ptrSystemTime->SetLocalTimeStruct();

	// String for name of file to print one time data block
	std::ostringstream strstrPrintFileName;
	std::string stringPrintFileName;

	strstrPrintFileName << "d:\\EvapTank_Log_Files\\EvapTank_Logfile_" << ptrSystemTime->GetSystemTimeToSecondString() << "_UTC.txt";
	stringPrintFileName = strstrPrintFileName.str();

	// Sets up printing stream object for one time data block
	std::ofstream printStream;
	printStream.open(stringPrintFileName.c_str());

	// Clear file name string stream buffer and string
	strstrPrintFileName.str("");
	stringPrintFileName = "";

	// Write data to print stream object
	printStream << ptrSystemTime->GetSystemTimeYear() << "_" << ptrSystemTime->GetSystemTimeMonth() << "_" << ptrSystemTime->GetSystemTimeDay() << "\n";
	printStream << ptrSystemTime->GetSystemTimeHour() << ":" << ptrSystemTime->GetSystemTimeMinute() << ":" << ptrSystemTime->GetSystemTimeSecond() << "." << ptrSystemTime->GetSystemTimeMillisecond() << " UTC\n";
	printStream << ptrSystemTime->GetSystemTimeInSeconds() - startRunTime << " Seconds since software start\n";
	for (int arrayIndex = 0; arrayIndex < DATAARRAYSIZE; arrayIndex++)
	{
		printStream << arrayIndex << ".\t";
		switch (logDataArray[arrayIndex]->GetDataType())
		{
		case 0:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetBoolValue() << "\n";
			break;
		case 1:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetShortValue() << "\n";
			break;
		case 2:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetIntValue() << "\n";
			break;
		case 3:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetFloatValue() << "\n";
			break;
		case 4:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetDoubleValue() << "\n";
			break;
		case 5:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetStringValue() << "\n";
			break;
		default:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName();
			printStream << "\n";
			break;
		}
	}
	printStream.close();
}

void LogFileOutput::PrintSelectedData(void)
{
	// Set current values for time and date
	ptrSystemTime->SetSystemTimeStruct();
	ptrSystemTime->SetLocalTimeStruct();

	// String for name of file to print one time data block
	std::ostringstream strstrPrintFileName;
	std::string stringPrintFileName;

	strstrPrintFileName << "d:\\EvapTank_Log_Files\\EvapTank_Logfile_" << ptrSystemTime->GetSystemTimeToSecondString() << "_UTC.txt";
	stringPrintFileName = strstrPrintFileName.str();

	// Sets up printing stream object for one time data block
	std::ofstream printStream;
	printStream.open(stringPrintFileName.c_str());

	// Clear file name string stream buffer and string
	strstrPrintFileName.str("");
	stringPrintFileName = "";

	// Write data to print stream object
	printStream << ptrSystemTime->GetSystemTimeYear() << "_" << ptrSystemTime->GetSystemTimeMonth() << "_" << ptrSystemTime->GetSystemTimeDay() << " UTC date\n";
	printStream << ptrSystemTime->GetSystemTimeHour() << ":" << ptrSystemTime->GetSystemTimeMinute() << ":" << ptrSystemTime->GetSystemTimeSecond() << "." << ptrSystemTime->GetSystemTimeMillisecond() << " UTC time\n";
	printStream << ptrSystemTime->GetSystemTimeInSeconds() - startRunTime << " Seconds since software start\n\n";

	int selectionArray[] = {2,34,35,21,32,49,4,6,7,8,28,29,38,1,71,61,67,66,64,57,54,55,56,59,60,40,50,71,12,10,11};
	int selectionArraySize = 31;
	int arrayIndex = 0;
	
	for (int selectionArrayIndex = 0; selectionArrayIndex < selectionArraySize; selectionArrayIndex++)
	{
		arrayIndex = selectionArray[selectionArrayIndex];
		switch (logDataArray[arrayIndex]->GetDataType())
		{
		case 0:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetBoolValue() << "\n";
			break;
		case 1:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetShortValue() << "\n";
			break;
		case 2:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetIntValue() << "\n";
			break;
		case 3:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetFloatValue() << "\n";
			break;
		case 4:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetDoubleValue() << "\n";
			break;
		case 5:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName() << " = ";
			printStream << logDataArray[arrayIndex]->LogData::GetStringValue() << "\n";
			break;
		default:
			printStream << logDataArray[arrayIndex]->LogData::GetItemName();
			printStream << "\n";
			break;
		}
	}
	printStream.close();
}