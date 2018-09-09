#pragma once

#include <windows.h>
#include <string.h>
#include <iostream>

#define  MAXSTRINGLENGTH 80
#define  DATAARRAYSIZE 72

class LogData
{
private:
	// Type of data actually stored 0=bool, 1=short, 2=int, 3=float, 4=double, 5=string
	int dataIndex;
	short dataType;
	char itemName[MAXSTRINGLENGTH];
	char* ptrItemName;
	bool boolValue;
	short shortValue;
	int intValue;
	float floatValue;
	double doubleValue;
	char stringValue[MAXSTRINGLENGTH];
	char* ptrStringValue;
	const char* cptrStringValue;

public:
	LogData(int index);
	virtual ~LogData();

	void SetData(char* name, bool value);
	void SetData(char* name, short value);
	void SetData(char* name, int value);
	void SetData(char* name, float value);
	void SetData(char* name, double value);
	void SetData(char* name, char* value);
	void SetData(char* name, const char* value);

	short GetDataType();
	char* GetItemName();
	bool GetBoolValue();
	short GetShortValue();
	int GetIntValue();
	float GetFloatValue();
	double GetDoubleValue();
	char* GetStringValue();
};

