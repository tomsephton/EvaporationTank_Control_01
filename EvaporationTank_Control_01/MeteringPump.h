#pragma once

#include "AnalogOutput.h"
#include "SystemTime.h"
#include "MathUtilities.h"
#include "ProgramException.h"
#include "ProgramExceptions.h"
#include "PumpLogFileOutput.h"
#include <string>
#include <xstring>
#include <sstream>
#include <deque>

class MeteringPump
{
private:
	int outputDeviceID; // should be set to process control device number for this valve
	int outputChannelID; // should be set to process control device channel number for this valve
	char pumpName[80]; // should be to null terminated string with name of pump
	char pumpType[80]; // should be set to null terminated string with type of pump
	double location[3]; // should be set to physical x,y,z coordinates of center of pump in meters
	float pumpSpeed; // Speed at which pump is set to run stopped=0.0 to maximum=1.0
	bool pumpSpeedAuto; // Allow automatic speed control when true, manual override when false
	bool pumpOperatingOK; // Set true if pump speed setting is within normal range 0.0 to 1.0
	double maxPumpVolumeDeliveredPerMinute; // Set value at initialization based on measured delivery at 100% rate for each pump (liters/minute)

	AnalogOutput* ptrAnalogOutput; // Pointer to AnalogOutput object, must be set after instantiation
	MathUtilities* ptrMathUtilities; // Pointer to object with utility math functions

	// Struct to store metering pump output value over time for display
	struct storedValue
	{
		double value;
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

	// Struct to store metering pump rate data on change over extended time for calculations
	struct storedRate
	{
		double rate;
		double secondsSinceLastRate;
		double systemTimeInSeconds;
		int year;
		int month;
		int day;
		int dayOfWeek;
		int hour;
		int minute;
		int second;
		int millisecond;
	};
	unsigned int maxRateBufferSize;
	std::deque<storedRate> storedRateBuffer;
	std::deque<storedRate>::iterator rateIter;

	protected:
	std::string strSystemDisplayName;
	std::string strPumpDisplayName;
	std::string strPumpSpeed;
	std::string strPumpSpeedUnits;
	std::string strPumpDelivery;
	std::ostringstream strstrPumpSpeed;
	std::ostringstream strstrPumpDelivery;

	std::string strPumpMenuLabel;
	std::ostringstream strstrPumpMenuLabel;
	std::string strPumpMenuText[6];
	std::ostringstream strstrPumpMenuText[6];

	SystemTime* ptrSystemTime;
	ProgramExceptions* ptrProgramExceptions;
	PumpLogFileOutput* ptrPumpLogFileOutput;
	
	struct displayCoordinates
	{
		int displayTopLeft[2];
		int displayBottomRight[2];
		int killBoxTopLeft[2];
		int killBoxBottomRight[2];
		int dragBoxTopLeft[2];
		int dragBoxBottomRight[2];
		int menuTopLeft[7][2]; // first slot holds overall menu coordinates, remaining slots hold option cells
		int menuBottomRight[7][2]; // first slot holds overall menu coordinates, remaining slots hold option cells
	};
	displayCoordinates displayEdges;

	bool mouseOverDisplay;
	bool mouseOverKillBox;
	bool mouseOverDragBox;
	bool mouseOverMenu[7];
	bool showRateControlMenu;

public:
	MeteringPump(std::string pumpID); // pumpID should be set to pump identifier for log file "A", "B", etc
	virtual ~MeteringPump(void);

	void SetOutputDeviceID (short device);
	short GetOutputDeviceID (void);
	void SetOutputChannelID (short channel);
	short GetOutputChannelID (void);
	void SetPumpName (char name[80]);
	char* GetPumpNamePtr();
	void SetPumpType (char type[80]);
	char* GetPumpTypePtr();
	void SetLocation (double x, double y, double z);
	bool GetLocation (double returnedLocation [3]);
	bool SetMaxPumpVolumeDeliveredPerMinute(double maxVolume);

	void SetAnalogOutputPtr(AnalogOutput* ptr);
	AnalogOutput* GetAnalogOutputPtr(void);
	bool SetPumpSpeed(float newPumpSpeed);
	float GetPumpSpeed(void);
	bool GetPumpSpeedAuto(void);

	// Metering pump output value methods for display
	void SetMaxBufferSize(unsigned int size);
	bool ResizeStoredValueBuffer(unsigned int newSize);
	unsigned int GetStoredValueBufferSize(void);
	double GetStoredValue(unsigned int i);
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

	// Metering pump rate methods for calculations
	void SetMaxRateBufferSize(unsigned int size);
	bool ResizeRateBuffer(unsigned int newSize);
	unsigned int GetRateBufferSize(void);
	double GetStoredRate(unsigned int i);
	int GetRateBufferYear(unsigned int i);
	int GetRateBufferMonth(unsigned int i);
	int GetRateBufferDay(unsigned int i);
	int GetRateBufferHour(unsigned int i);
	int GetRateBufferMinute(unsigned int i);
	int GetRateBufferSecond(unsigned int i);
	int GetRateBufferMillisecond(unsigned int i);
	double GetRateBufferTimeInSeconds(unsigned int i);
	bool lockRateBuffer;
	bool lockForRateDisplay;

	// Mouse interaction methods
	bool GetMouseOverDisplay(int mouseX, int mouseY);
	bool GetMouseOverKillBox(int mouseX, int mouseY);
	bool GetMouseOverDragBox(int mouseX, int mouseY);
	int GetMouseOverMenu(int mouseX, int mouseY);
	void CheckMouseAction(int mouseX, int mouseY, int mouseAction);

	// Pump delivery calculation
	double GetLast24HourPumpDelivery(void);
	double GetAny24HourPumpDelivery(int year, int month, int day, int hour);

	void SetPumpUnits(std::string units);
	void SetSystemDisplayName(std::string name);
	void SetPumpDisplayName(std::string name);
	void DrawDataBox2D(GLint nFontList, int xPos, int yPos, int xOrigin, int yOrigin);
	void DrawChartRecorderStrip2D(GLint nFontList, int xStripPos, int yStripPos, int xStripWidth, int yStripHeight, int xScale, int yScale, bool autoScale, bool autoScaleOnExceed);
	bool UpdatePumpSpeed(void);
};

