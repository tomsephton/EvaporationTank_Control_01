#include "StdAfx.h"
#include "LogData.h"

LogData::LogData(int index)
{
	dataIndex = index;
	dataType = -1;
	for (int stringIndex = 0; stringIndex < MAXSTRINGLENGTH; stringIndex++)
	{
		itemName[stringIndex] = '\0';
	}
	ptrItemName = &itemName[0];
	boolValue = false;
	shortValue = -1;
	intValue = -1;
	floatValue = 0.0;
	doubleValue = 0.0;
	dataType = -1;
	for (int stringIndex = 0; stringIndex < MAXSTRINGLENGTH; stringIndex++)
	{
		stringValue[stringIndex] = '\0';
	}
	ptrStringValue = &stringValue[0];
	cptrStringValue = &stringValue[0];
}

LogData::~LogData(void)
{
	dataType = -1;
	for (int stringIndex = 0; stringIndex < MAXSTRINGLENGTH; stringIndex++)
	{
		itemName[stringIndex] = '\0';
	}
	ptrItemName = NULL;
	boolValue = false;
	shortValue = -1;
	intValue = -1;
	floatValue = 0.0;
	doubleValue = 0.0;
	dataType = -1;
	for (int stringIndex = 0; stringIndex < MAXSTRINGLENGTH; stringIndex++)
	{
		stringValue[stringIndex] = '\0';
	}
	ptrStringValue = NULL;
}

void LogData::SetData(char* name, bool value)
{
	if (strlen(name) < MAXSTRINGLENGTH)
	{
		dataType = 0;
		strncpy_s(ptrItemName, MAXSTRINGLENGTH, name, strlen(name));
		itemName[strlen(name)] = '\0';
		boolValue = value;
	}
}

void LogData::SetData(char* name, short value)
{
	if (strlen(name) < MAXSTRINGLENGTH)
	{
		dataType = 1;
		strncpy_s(ptrItemName, MAXSTRINGLENGTH, name, strlen(name));
		itemName[strlen(name)] = '\0';
		shortValue = value;
	}
}

void LogData::SetData(char* name, int value)
{
	if (strlen(name) < MAXSTRINGLENGTH)
	{
		dataType = 2;
		strncpy_s(ptrItemName, MAXSTRINGLENGTH, name, strlen(name));
		itemName[strlen(name)] = '\0';
		intValue = value;
	}
}

void LogData::SetData(char* name, float value)
{
	if (strlen(name) < MAXSTRINGLENGTH)
	{
		dataType = 3;
		strncpy_s(ptrItemName, MAXSTRINGLENGTH, name, strlen(name));
		itemName[strlen(name)] = '\0';
		floatValue = value;
	}
}

void LogData::SetData(char* name, double value)
{
	if (strlen(name) < MAXSTRINGLENGTH)
	{
		dataType = 4;
		strncpy_s(ptrItemName, MAXSTRINGLENGTH, name, strlen(name));
		itemName[strlen(name)] = '\0';
		doubleValue = value;
	}
}

void LogData::SetData(char* name, char* value)
{
	if (strlen(name) < MAXSTRINGLENGTH)
	{
		if (strlen(value) < MAXSTRINGLENGTH)
		{
			dataType = 5;
			strncpy_s(ptrItemName, MAXSTRINGLENGTH, name, strlen(name));
			itemName[strlen(name)] = '\0';
			strncpy_s(ptrStringValue, MAXSTRINGLENGTH, value, strlen(value));
			stringValue[strlen(value)] = '\0';
		}
	}
}

void LogData::SetData(char* name, const char* value)
{
	if (strlen(name) < MAXSTRINGLENGTH)
	{
		if (strlen(value) < MAXSTRINGLENGTH)
		{
			dataType = 5;
			strncpy_s(ptrItemName, MAXSTRINGLENGTH, name, strlen(name));
			itemName[strlen(name)] = '\0';
			strncpy_s(ptrStringValue, MAXSTRINGLENGTH, value, strlen(value));
			stringValue[strlen(value)] = '\0';
		}
	}
}

short LogData::GetDataType()
{
	return dataType;
}

char* LogData::GetItemName()
{
	return ptrItemName;
}

bool LogData::GetBoolValue()
{
	return boolValue;
}

short LogData::GetShortValue()
{
	return shortValue;
}

int LogData::GetIntValue()
{
	return intValue;
}

float LogData::GetFloatValue()
{
	return floatValue;
}

double LogData::GetDoubleValue()
{
	return doubleValue;
}

char* LogData::GetStringValue()
{
	return ptrStringValue;
}