#pragma once

#include "Sensor.h"
#include "BinaryInput.h"
#include <string>
#include <xstring>
#include <sstream>
#include <deque>

class BinarySensor : public Sensor
{
private:
	short int status; // on/off status of sensor, 0=off, 1=on, -1=not set or error
	BinaryInput* ptrBinaryInput;
	double timeBetweenUpdates; // Frequency at which values will be updated in milliseconds (set by calling routine)

struct storedValue
	{
		short int value;
		long int timeSlicesSinceLastChange;
		int year;
		int month;
		int day;
		int dayOfWeek;
		int hour;
		int minute;
		int second;
		int millisecond;
	};
	unsigned int maxBufferSize;
	std::deque<storedValue> storedValueBuffer;
	std::deque<storedValue>::iterator iter;

	struct stateChange
	{
		short int value;
		double secondsSinceLastChange;
		int year;
		int month;
		int day;
		int dayOfWeek;
		int hour;
		int minute;
		int second;
		int millisecond;
	};
	unsigned int maxStateChangeBufferSize;
	std::deque<stateChange> stateChangeBuffer;
	std::deque<stateChange>::iterator iterat;

protected:
	double averageSecondsBetweenStateChange;
	std::string strSystemDisplayName;
	std::string strSensorDisplayName;
	std::string strSensorValue;
	std::ostringstream strstrSensorValue;

	struct displayCoordinates
	{
		int displayTopLeft[2];
		int displayBottomRight[2];
		int killBoxTopLeft[2];
		int killBoxBottomRight[2];
		int dragBoxTopLeft[2];
		int dragBoxBottomRight[2];
	};
	displayCoordinates displayEdges;

public:
	BinarySensor(void);
	virtual ~BinarySensor(void);
	bool SetInputDevice(short deviceID, short devicePort, short deviceChannel);
	bool SetStatus(short newStatus);
	bool UpdateStatus(void);
	short GetStatus(void);
	short GetInputDeviceID(void);
	short GetInputDevicePort(void);
	short GetInputDeviceChannel(void);
	void SetBinaryInputPtr(BinaryInput* ptr);

	// Stored value methods and variables
	void SetMaxBufferSize(unsigned int size);
	bool ResizeStoredValueBuffer(unsigned int newSize);
	unsigned int GetStoredValueBufferSize(void);
	short int GetStoredValue(unsigned int i);
	long unsigned int GetTimeSlicesSinceLastChange();
	int GetStoredYear(unsigned int i);
	int GetStoredMonth(unsigned int i);
	int GetStoredDay(unsigned int i);
	int GetStoredHour(unsigned int i);
	int GetStoredMinute(unsigned int i);
	int GetStoredSecond(unsigned int i);
	int GetStoredMillisecond(unsigned int i);
	float GetStoredTimeInSeconds(unsigned int i);
	bool lockStoredValueBuffer;
	bool lockForChartStripDraw;

	// State Change methods and variables
	void SetMaxStateChangeBufferSize(unsigned int size);
	bool ResizeStateChangeBuffer(unsigned int newSize);
	unsigned int GetStateChangeBufferSize(void);
	short int GetStateChangeState(unsigned int i);
	double GetSecondsSinceLastChange();
	bool lockStateChangeBuffer;
	bool lockForStateChangeDraw;

	void SetSystemDisplayName(std::string name);
	void SetSensorDisplayName(std::string name);
	void SetTimeBetweenUpdates(int milliseconds);
	void DrawDataBox2D(GLint nFontList, int xPos, int yPos, int xOrigin, int yOrigin);
};

