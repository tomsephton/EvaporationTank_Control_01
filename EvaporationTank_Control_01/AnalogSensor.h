#pragma once

#include "sensor.h"
#include "Sensor.h"
#include "AnalogInput.h"
#include "MathUtilities.h"
#include <string>
#include <xstring>
#include <sstream>
#include <deque>

#include <Gl\Gl.h>              // OpenGL
#include <Gl\Glu.h>             // GLU library
//#include <Gl\Glaux.h>			// AUX library objects
#include ".\INCLUDE\Gl\Glaux.h"	// AUX library objects

#define MAX_VALUE_LINES	3
#define SCALE_STRING_SIZE 80

class AnalogSensor : public Sensor
{
private:
	double sensorValue; // should update to the value of the sensor in the sensor's units
	double outputValue; // should update to the normalized 4-20 mA current value read from the sensor
	short precision; // expected precision of the sensor value in decimals after decimal point
	double sensorRange[2]; // minimum [0] and maximum [1] values that the sensor can read
	double minOutput; // sensor value corresponding to 4mA current
	double maxOutput; // sensor value corresponding to 20 mA current
	double zeroOffset; // offset determined by calibration of each sensor
	bool reverseActing; // should be set true if sensor value increases as process value decreases, normally false

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

	AnalogInput* ptrAnalogInput;
	MathUtilities* ptrMathUtilities;

protected:
	std::string strSystemDisplayName;
	std::string strSensorDisplayName;
	std::string strSensorValue;
	std::string strSensorUnits;
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

	bool mouseOverDisplay;
	bool mouseOverKillBox;
	bool mouseOverDragBox;

public:
	AnalogSensor(void);
	virtual ~AnalogSensor(void);

	void SetInputDevice(short deviceID, short devicePort, short deviceChannel);
	void SetSensorValue(double value);
	double GetSensorValue(void);
	void SetSensorRange(double low, double high);
	double GetSensorLowRange(void);
	double GetSensorHighRange(void);
	void SetOutputValue(double value);
	double GetOutputValue (void);
	void SetPrecision (short decimals);
	short GetPrecision (void);
	void SetMinOutput(double value);
	double GetMinOutput(void);
	void SetMaxOutput(double value);
	double GetMaxOutput(void);
	void SetZeroOffset(double value);
	double GetZeroOffset(void);
	void SetAnalogInputPtr(AnalogInput* ptr);
	void SetReverseActing (bool is);
	bool GetReverseActing();	

	AnalogInput* GetAnalogInputPtr(void);
	MathUtilities* GetMathUtilitiesPtr(void);

	// Stored value methods and variables
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

	void SetSensorUnits(std::string units);
	void SetSystemDisplayName(std::string name);
	void SetSensorDisplayName(std::string name);
	void DrawDataBox2D(GLint nFontList, int xPos, int yPos, int xOrigin, int yOrigin);
	void DrawChartRecorderStrip2D(GLint nFontList, int xStripPos, int yStripPos, int xStripWidth, int yStripHeight, int xScale, int yScale, bool autoScale, bool autoScaleOnExceed);

	bool GetMouseOverDisplay(int mouseX, int mouseY);
	bool GetMouseOverKillBox(int mouseX, int mouseY);
	bool GetMouseOverDragBox(int mouseX, int mouseY);
	void CheckMouseAction(int mouseX, int mouseY, int mouseAction);

	bool UpdateSensorValue();
	bool UpdateSensorValueSmoothed();
};

