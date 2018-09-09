#include "StdAfx.h"
#include "AnalogSensor.h"

// Class Constructor
AnalogSensor::AnalogSensor(void)
{
	sensorValue = 0.0; // should update to the value of the sensor in the sensor's units
	outputValue = 0.0; // should update to the normalized value read from the sensor by an input device
	precision = 2; // expected precision of the sensor value in decimals after decimal point
	sensorRange[0] = 0.0; // minimum value that the sensor can read
	sensorRange[1] = 0.0; // maximum value that the sensor can read
	minOutput = 0.0; // sensor value corresponding to 4mA current
	maxOutput = 1.0; // sensor value corresponding to 20 mA current
	zeroOffset = 0.0; // +/- offset determined by calibration of each sensor
	reverseActing = false;

	strSystemDisplayName = "";
	strSensorDisplayName = "";
	strSensorValue = "";
	strSensorUnits = "";
	strstrSensorValue.str("");

	maxBufferSize = 10;
	lockStoredValueBuffer = false;
	lockForChartStripDraw = false;

	mouseOverDisplay = false;
	mouseOverKillBox = false;
	mouseOverDragBox = false;

	ptrMathUtilities = new MathUtilities();
}

// Class Destructor
AnalogSensor::~AnalogSensor(void)
{
	strSystemDisplayName = "";
	strSensorDisplayName = "";
	strSensorValue = "";
	strSensorUnits = "";
	strstrSensorValue.str("");
	delete ptrMathUtilities;
	ptrMathUtilities = NULL;
}

void AnalogSensor :: SetSensorValue(double value)
{
	sensorValue = value;
}

double AnalogSensor :: GetSensorValue(void)
{
	return sensorValue;
}

void AnalogSensor :: SetSensorRange(double low, double high)
{
	sensorRange[0] = low;
	sensorRange[1] = high;
}

double AnalogSensor :: GetSensorLowRange(void)
{
	return sensorRange[0];
}

double AnalogSensor :: GetSensorHighRange(void)
{
	return sensorRange[1];
}

void AnalogSensor :: SetOutputValue(double value)
{
	outputValue = value;
}

double AnalogSensor :: GetOutputValue (void)
{
	return outputValue;
}

void AnalogSensor :: SetPrecision (short decimals)
{
	precision = decimals;
}

short AnalogSensor :: GetPrecision (void)
{
	return precision;
}

void AnalogSensor::SetMinOutput(double value)
{
	minOutput = value;
}

double AnalogSensor::GetMinOutput(void)
{
	return minOutput;
}

void AnalogSensor::SetMaxOutput(double value)
{
	maxOutput = value;
}

double AnalogSensor::GetMaxOutput(void)
{
	return maxOutput;
}

void AnalogSensor::SetZeroOffset(double value)
{
	zeroOffset = value;
}

double AnalogSensor::GetZeroOffset(void)
{
	return zeroOffset;
}

void AnalogSensor::SetReverseActing (bool is)
{
	reverseActing = is;
}

bool AnalogSensor::GetReverseActing()
{
	return reverseActing;
}

void AnalogSensor::SetAnalogInputPtr(AnalogInput* ptr)
{
	ptrAnalogInput = ptr;
}

AnalogInput* AnalogSensor::GetAnalogInputPtr(void)
{
	return ptrAnalogInput;
}

MathUtilities* AnalogSensor::GetMathUtilitiesPtr(void)
{
	return ptrMathUtilities;
}

void AnalogSensor::SetSystemDisplayName(std::string name)
{
	strSystemDisplayName = name;
}

void AnalogSensor::SetSensorDisplayName(std::string name)
{
	strSensorDisplayName = name;
}

void AnalogSensor::SetSensorUnits(std::string units)
{
	strSensorUnits = units;
}

void AnalogSensor::SetMaxBufferSize(unsigned int size)
{
	maxBufferSize = size;
}

bool AnalogSensor::ResizeStoredValueBuffer(unsigned int newSize)
{
	if (newSize <= maxBufferSize)
	{
		storedValueBuffer.resize(newSize);
		if (storedValueBuffer.size() == newSize) return true;
		else return false;
	}
	else return false;
}

unsigned int AnalogSensor::GetStoredValueBufferSize(void)
{
	return storedValueBuffer.size();
}

double AnalogSensor::GetStoredValue(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).value;
	else return 0.0;
}

int AnalogSensor::GetStoredYear(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).year;
	else return -1;
}

int AnalogSensor::GetStoredMonth(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).month;
	else return -1;
}

int AnalogSensor::GetStoredDay(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).day;
	else return -1;
}

int AnalogSensor::GetStoredHour(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).hour;
	else return -1;
}

int AnalogSensor::GetStoredMinute(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).minute;
	else return -1;
}

int AnalogSensor::GetStoredSecond(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).second;
	else return -1;
}

int AnalogSensor::GetStoredMillisecond(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).millisecond;
	else return -1;
}

float AnalogSensor::GetStoredTimeInSeconds(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return (float)((GetStoredDay(i) * 86400 + GetStoredHour(i) * 3600 + GetStoredMinute(i) * 60 + GetStoredSecond(i)) + (float)(GetStoredMillisecond(i)) / 1000.0f);
	else return 0.0;
}

void AnalogSensor::DrawDataBox2D(GLint nFontList, int xPos, int yPos, int xOrigin, int yOrigin)
{
	int numberLines = 1;
	int maxLineLength = 0;

	strstrSensorValue << strSensorDisplayName << " " << GetSensorValue() << " " << strSensorUnits;
	strSensorValue = strstrSensorValue.str();
	strstrSensorValue.str("");
	
	// Set box width to longest line to be displayed
	if (strSystemDisplayName.size() > strSensorValue.size()) maxLineLength = strSystemDisplayName.size() + 1;
	else maxLineLength = strSensorValue.size() + 1;

	// Box modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(float(xPos), float(yPos), 0.0);

	// Top outer border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(-7.0f, float(numberLines + 1) * 20.0f + 7.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 7.0f, float(numberLines + 1) * 20.0f + 7.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, float(numberLines + 1) * 20.0f + 5.0f);
	glVertex2f(-5.0f, float(numberLines + 1) * 20.0f + 5.0f);
	glEnd();

	// Set top left display coordinates
	displayEdges.displayTopLeft[0] = xOrigin + xPos - 7;
	displayEdges.displayTopLeft[1] = yOrigin + yPos + ((numberLines + 1) * 20) + 7;
	displayEdges.dragBoxTopLeft[0] = xOrigin + xPos - 5;
	displayEdges.dragBoxTopLeft[1] = yOrigin + yPos + ((numberLines + 1) * 20) + 5;

	// Right outer border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(float(maxLineLength) * 8.0f + 7.0f, -7.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, -5.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, float(numberLines + 1) * 20.0f + 5.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 7.0f, float(numberLines + 1) * 20.0f + 7.0f);
	glEnd();

	// Lower outer border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(float(maxLineLength) * 8.0f + 7.0f, -7.0f);
	glVertex2f(-7.0f, -7.0f);
	glVertex2f(-5.0f, -5.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, -5.0f);
	glEnd();

	// Set bottom right display coordinates
	displayEdges.displayBottomRight[0] = xOrigin + xPos + ((maxLineLength) * 8) + 7;
	displayEdges.displayBottomRight[1] = yOrigin + yPos - 7;

	// Left outer border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(-7.0f, -7.0f);
	glVertex2f(-7.0f, float(numberLines + 1) * 20.0f + 7.0f);
	glVertex2f(-5.0f, float(numberLines + 1) * 20.0f + 5.0f);
	glVertex2f(-5.0f, -5.0f);
	glEnd();

	// Outer Rim of data box
	glBegin(GL_POLYGON);
	if (mouseOverDragBox) glColor3f(0.6f, 0.6f, 0.6f);
	else glColor3f(0.7f, 0.7f, 0.7f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, -5.0f);
	glVertex2f(-5.0f, -5.0f);
	glVertex2f(-5.0f, float(numberLines + 1) * 20.0f + 5.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, float(numberLines + 1) * 20.0f + 5.0f);
	glEnd();

	// Top inner border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(-3.0f, float(numberLines * 20.0) + 3.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 3.0f, float(numberLines) * 20.0f + 3.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, float(numberLines) * 20.0f + 1.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glEnd();

	// Set bottom right drag box coordinates
	displayEdges.dragBoxBottomRight[0] = xOrigin + xPos + (maxLineLength * 8) + 3;
	displayEdges.dragBoxBottomRight[1] = yOrigin + yPos + (numberLines * 20) + 3;
	
	// Right inner border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(float(maxLineLength) * 8.0f + 3.0f, -3.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, -1.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, float(numberLines) * 20.0f + 1.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 3.0f, float(numberLines) * 20.0f + 3.0f);
	glEnd();

	// Lower inner border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(float(maxLineLength) * 8.0f + 3.0f, -3.0f);
	glVertex2f(-3.0f, -3.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, -1.0f);
	glEnd();

	// Left inner border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(-3.0f, -3.0f);
	glVertex2f(-3.0f, float(numberLines) * 20.0f + 3.0f);
	glVertex2f(-1.0f, float(numberLines) * 20.0f + 1.0f);
	glVertex2f(-1.0f, -1.0f);
	glEnd();

	// Kill box
	glBegin(GL_POLYGON);
	if (mouseOverKillBox) glColor3f(0.8f, 0.4f, 0.4f);
	else glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(float(maxLineLength) * 8.0f - 12.0f, float(numberLines + 1) * 20.0f + 4.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 4.0f, float(numberLines + 1) * 20.0f + 4.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 4.0f, float(numberLines + 1) * 20.0f - 12.0f);
	glVertex2f(float(maxLineLength) * 8.0f - 12.0f, float(numberLines + 1) * 20.0f - 12.0f);
	glEnd();

	// Set kill box coordinates
	displayEdges.killBoxTopLeft[0] = xOrigin + xPos + (maxLineLength * 8) - 12; 
	displayEdges.killBoxTopLeft[1] = yOrigin + yPos + ((numberLines + 1) * 20) + 4;
	displayEdges.killBoxBottomRight[0] = xOrigin + xPos + (maxLineLength * 8) + 4; 
	displayEdges.killBoxBottomRight[1] = yOrigin + yPos + ((numberLines + 1) * 20) - 12;

	// Text Background of data box
	glBegin(GL_POLYGON);
	// Display green text background if value is within full senor output range, otherwise yellow, or red when flagged as failed
	if (!sensorOperatingOK)
	{
		if (mouseOverDisplay) glColor3f(0.7f, 0.2f, 0.2f);
		else glColor3f(0.9f, 0.3f, 0.3f);
	}
	else if (GetSensorValue() >= GetMinOutput() && GetSensorValue() <= GetMaxOutput())
	{
		if (mouseOverDisplay) glColor3f(0.1f, 0.7f, 0.1f);
		else glColor3f(0.2f, 0.8f, 0.2f);
	}
	else glColor3f(0.8f, 0.8f, 0.1f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, -1.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, float(numberLines) * 20.0f + 1.0f);
	glEnd();

	// Display Sensor Name string
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2i(8, 28);
	glListBase(nFontList);
	glCallLists (strSystemDisplayName.size(), GL_UNSIGNED_BYTE, strSystemDisplayName.c_str());

	int lineNumber = 0;

	// Display sensor value and units
	if (strSensorValue.size() > 0)
	{	
		glRasterPos2i(8, (lineNumber * 20) + 4);
		glListBase(nFontList);
		glCallLists (strSensorValue.size(), GL_UNSIGNED_BYTE, strSensorValue.c_str());
		strSensorValue = "";
	}

	glPopMatrix();
}

bool AnalogSensor :: GetMouseOverDisplay(int mouseX, int mouseY)
{
	if (mouseX > displayEdges.displayTopLeft[0] && mouseX < displayEdges.displayBottomRight[0] && mouseY < displayEdges.displayTopLeft[1] && mouseY > displayEdges.displayBottomRight[1])
	{
		mouseOverDisplay = true;
		return true;
	}
	else
	{
		mouseOverDisplay = false;
		return false;
	}
}

bool AnalogSensor :: GetMouseOverKillBox(int mouseX, int mouseY)
{
	if (mouseX > displayEdges.killBoxTopLeft[0] && mouseX < displayEdges.killBoxBottomRight[0] && mouseY < displayEdges.killBoxTopLeft[1] && mouseY > displayEdges.killBoxBottomRight[1])
	{
		mouseOverKillBox = true;
		return true;
	}
	else
	{
		mouseOverKillBox = false;
		return false;
	}
}

bool AnalogSensor :: GetMouseOverDragBox(int mouseX, int mouseY)
{
	if (mouseX > displayEdges.dragBoxTopLeft[0] && mouseX < displayEdges.dragBoxBottomRight[0] && mouseY < displayEdges.dragBoxTopLeft[1] && mouseY > displayEdges.dragBoxBottomRight[1])
	{
		mouseOverDragBox = true;
		return true;
	}
	else
	{
		mouseOverDragBox = false;
		return false;
	}
}

void AnalogSensor :: CheckMouseAction(int mouseX, int mouseY, int mouseAction)
{
	GetMouseOverDisplay(mouseX, mouseY);
	if (GetMouseOverKillBox(mouseX, mouseY) && mouseAction == 1)
	{
		if (sensorOperatingOK) sensorOperatingOK = false;
		else sensorOperatingOK = true;
	}
	GetMouseOverDragBox(mouseX, mouseY);
}

void AnalogSensor::DrawChartRecorderStrip2D(GLint nFontList, int xStripPos, int yStripPos, int xStripWidth, int yStripHeight, int xScale, int yScale, bool autoScale, bool autoScaleOnExceed)
{
	// Strip modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef((float)(xStripPos), (float)(yStripPos), 0.0);

	// Top outer border of strip
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(-7.0f, (float)(yStripHeight));
	glVertex2f((float)(xStripWidth) + 7.0f, (float)(yStripHeight) + 7.0f);
	glVertex2f((float)(xStripWidth) + 5.0f, (float)(yStripHeight) + 5.0f);
	glVertex2f(-5.0f, float(yStripHeight) + 5.0f);
	glEnd();

	// Right outer border of strip
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f((float)(xStripWidth) + 7.0f, -7.0f);
	glVertex2f((float)(xStripWidth) + 5.0f, -5.0f);
	glVertex2f((float)(xStripWidth) + 5.0f, (float)(yStripHeight) + 5.0f);
	glVertex2f((float)(xStripWidth) + 7.0f, (float)(yStripHeight) + 7.0f);
	glEnd();

	// Lower outer border of strip
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f((float)(xStripWidth) + 7.0f, -7.0f);
	glVertex2f(-7.0f, -7.0f);
	glVertex2f(-5.0f, -5.0f);
	glVertex2f((float)(xStripWidth) + 5.0f, -5.0f);
	glEnd();

	// Left outer border of strip
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(-7.0f, -7.0f);
	glVertex2f(-7.0f, (float)(yStripHeight) + 7.0f);
	glVertex2f(-5.0f, (float)(yStripHeight) + 5.0f);
	glVertex2f(-5.0f, -5.0f);
	glEnd();

	// Outer Rim of strip
	glBegin(GL_POLYGON);
	glColor3f(0.7f, 0.7f, 0.7f);
	glVertex2f((float)(xStripWidth) + 5.0f, -5.0f);
	glVertex2f(-5.0f, -5.0f);
	glVertex2f(-5.0f, (float)(yStripHeight) + 5.0f);
	glVertex2f((float)(xStripWidth) + 5.0f, (float)(yStripHeight) + 5.0f);
	glEnd();

	// Top inner border of strip
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(-3.0f, (float)(yStripHeight) + 3.0f);
	glVertex2f((float)(xStripWidth) + 3.0f, (float)(yStripHeight) + 3.0f);
	glVertex2f((float)(xStripWidth) + 1.0f, (float)(yStripHeight) + 1.0f);
	glVertex2f(-1.0f, (float)(yStripHeight) + 1.0f);
	glEnd();
	
	// Right inner border of strip
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f((float)(xStripWidth) + 3.0f, -3.0f);
	glVertex2f((float)(xStripWidth) + 1.0f, -1.0f);
	glVertex2f((float)(xStripWidth) + 1.0f, (float)(yStripHeight) + 1.0f);
	glVertex2f((float)(xStripWidth) + 3.0f, (float)(yStripHeight) + 3.0f);
	glEnd();

	// Lower inner border of strip
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f((float)(xStripWidth) + 3.0f, -3.0f);
	glVertex2f(-3.0f, -3.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(float(xStripWidth) + 1.0f, -1.0f);
	glEnd();

	// Left inner border of strip
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(-3.0f, -3.0f);
	glVertex2f(-3.0f, (float)(yStripHeight) + 3.0f);
	glVertex2f(-1.0f, (float)(yStripHeight) + 1.0f);
	glVertex2f(-1.0f, -1.0f);
	glEnd();

	// Background of chart recorder strip
	glBegin(GL_POLYGON);
	// Display green text background if value is within full senor output range, otherwise yellow
	if (GetSensorValue() >= GetMinOutput() && GetSensorValue() <= GetMaxOutput()) glColor3f(0.3f, 0.9f, 0.3f);
	else glColor3f(0.9f, 0.9f, 0.1f);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, (float)(yStripHeight));
	glVertex2f((float)(xStripWidth), (float)(yStripHeight));
	glVertex2f((float)(xStripWidth), 0.0);
	glEnd();

	strstrSensorValue << strSystemDisplayName << " " << strSensorDisplayName << " = " << GetSensorValue() << " " << strSensorUnits;
	strSensorValue = strstrSensorValue.str();
	strstrSensorValue.str("");

	// Display Sensor Name, Value, and Units
	if (strSensorValue.size() > 0)
	{	
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos2i(8, yStripHeight - 12);
		glListBase(nFontList);
		glCallLists (strSensorValue.size(), GL_UNSIGNED_BYTE, strSensorValue.c_str());
		strSensorValue = "";
	}

	// Draw graph of recent sensor values scaled over time
	while (lockStoredValueBuffer) ptrSystemTime->Wait(1);
	lockForChartStripDraw = true;

	if (autoScale)
	{
		// Set time offset in pixels per second to center graph to current value
		float xOffset = GetStoredTimeInSeconds(0) * (float)(float)(xScale);
		// Set vertical offset to center graph to current value
		float yOffset = (float)(yStripHeight) / 2.0f - (float)GetStoredValue(0) * (float)(yScale);
		// 
		float yMax = yOffset;
		float yMin = yOffset;
		float yFloatScale = (float)(yStripHeight) / 2.0f - 6.0f;

		// Find maximum and minimum y values
		for (unsigned int i = 0; i < storedValueBuffer.size(); i++)
		{
			float y = (float)((GetStoredValue(i) * yFloatScale) + yOffset);
			if (y > yMax) yMax = y;
			else if (y < yMin) yMin = y;
			// Ignore values past display area
			float x = xOffset - (float)(GetStoredTimeInSeconds(i) * xScale);
			if (x > (float)(xStripWidth)) break;
		}

		// Adjust y scale proportionally if a value is offscale by more than one unit
		if(yMax > yOffset + (float)(yScale) || yMin < yOffset - (float)(yScale))
		{
			if (yMax - (float)(yStripHeight) / 2.0f - 6.0f >= abs((float)(yStripHeight) / 2.0 - yMin + 6.0)) yFloatScale = yFloatScale * (((float)(yStripHeight) / 2.0f) / (yMax - (float)(yStripHeight) / 2.0f - 6.0f));
			else if (yMin == 0.0) yFloatScale = yFloatScale; // prevent divide by zero
			else if (yMax - (float)(yStripHeight) / 2.0f - 6.0f < abs((float)(yStripHeight) / 2.0f - yMin + 6.0f)) yFloatScale = yFloatScale * (((float)(yStripHeight) / 2.0f) / abs((float)(yStripHeight) / 2.0f - yMin + 6.0f));
		}

		// Draw line segment graph
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0f, 0.0f, 3.0f);
		
		// Set vertical offset to center graph to current value
		yOffset = (float)(yStripHeight) / 2.0f - (float)GetStoredValue(0) * yFloatScale;
		for (unsigned int i = 0; i < storedValueBuffer.size(); i++)
		{
			float x = xOffset - (float)(GetStoredTimeInSeconds(i) * xScale);
			float y = ((float)GetStoredValue(i) * yFloatScale) + yOffset;
			if (y >= 0.0 && y <= (float)(yStripHeight)) glVertex2f(x, y);
			else if (y > (float)(yStripHeight)) glVertex2f(x, (float)(float)(yStripHeight));
			else if (y < 0.0) glVertex2f(x, 0.0);
			else glVertex2f(x, (float)(yStripHeight) / 2.0f);
			// Stop display at end of strip
			if (x > (float)(xStripWidth)) break;
		}
		glEnd();

		// Draw up ticks on graph
		float yTick = ((float)GetStoredValue(0) * yFloatScale) + yOffset;
		while(yTick + yFloatScale < (float)(yStripHeight))
		{
			yTick = yTick + yFloatScale;
			// Draw 1 unit up tick on graph
			glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(0.0, yTick);
			glVertex2f(6.0, yTick);
			glEnd();
		}

		// Draw down ticks on graph
		yTick = ((float)GetStoredValue(0) * yFloatScale) + yOffset;
		while(yTick - yFloatScale > 0.0)
		{
			yTick = yTick - yFloatScale;
			// Draw 1 unit up tick on graph
			glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(0.0, yTick);
			glVertex2f(6.0, yTick);
			glEnd();
		}
	}
	else if (autoScaleOnExceed)
	{
		// Set time offset in pixels per second to center graph to current value
		float xOffset = GetStoredTimeInSeconds(0) * (float)(xScale);
		// Set vertical offset to center graph to current value
		float yOffset = (float)(yStripHeight) / 2.0f - (float)GetStoredValue(0) * (float)(yScale);
		float yMax = (float)(yStripHeight);
		float yMin = 0.0;
		float yFloatScale = (float)(yScale);
		
		// Find maximum and minimum y values
		for (unsigned int i = 0; i < storedValueBuffer.size(); i++)
		{
			float y = ((float)GetStoredValue(i) * yFloatScale) + yOffset;
			if (y > yMax) yMax = y;
			else if (y < yMin) yMin = y;
			// Ignore values past display area
			float x = xOffset - (float)(GetStoredTimeInSeconds(i) * xScale);
			if (x > (float)(xStripWidth)) break;
		}

		// Reduce y scale proportionally if any y value is offscale
		if (yMax > (float)(yStripHeight) || yMin < 0.0)
		{
			// Adjust y scale proportionally
			if (yMax >= abs(yMin)) yFloatScale = yFloatScale * (((float)(yStripHeight) / 2.0f) / yMax);
			else if (abs(yMin) > yMax) yFloatScale = yFloatScale * ((-(float)(yStripHeight) / 2.0f) / yMin);
		}

		// Draw line segment graph
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0f, 0.0f, 3.0f);
		
		// Set vertical offset to center graph to current value
		yOffset = (float)(yStripHeight) / 2.0f - (float)GetStoredValue(0) * yFloatScale;
		for (unsigned int i = 0; i < storedValueBuffer.size(); i++)
		{
			float x = xOffset - (float)(GetStoredTimeInSeconds(i) * xScale);
			float y = ((float)GetStoredValue(i) * yFloatScale) + yOffset;
			if (y >= 0.0 && y <= (float)(yStripHeight)) glVertex2f(x, y);
			else if (y > (float)(yStripHeight)) glVertex2f(x, (float)(yStripHeight));
			else if (y < 0.0) glVertex2f(x, 0.0);
			else glVertex2f(x, (float)(yStripHeight) / 2.0f);
			if (x > (float)(xStripWidth)) break;
		}
		glEnd();

		// Draw up ticks on graph
		float yTick = ((float)GetStoredValue(0) * yFloatScale) + yOffset;
		while(yTick + yFloatScale < (float)(yStripHeight))
		{
			yTick = yTick + yFloatScale;
			// Draw 1 unit up tick on graph
			glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(0.0, yTick);
			glVertex2f(6.0, yTick);
			glEnd();
		}

		// Draw down ticks on graph
		yTick = ((float)GetStoredValue(0) * yFloatScale) + yOffset;
		while(yTick - yFloatScale > 0.0)
		{
			yTick = yTick - yFloatScale;
			// Draw 1 unit up tick on graph
			glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(0.0, yTick);
			glVertex2f(6.0, yTick);
			glEnd();
		}
	}
	else
	{
		// Draw line segment graph
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0f, 0.0f, 3.0f);
		// Set time offset in pixels per second to center graph to current value
		float xOffset = GetStoredTimeInSeconds(0) * (float)(xScale);
		// Set vertical offset to center graph to current value
		float yOffset = (float)(yStripHeight) / 2.0f - (float)GetStoredValue(0) * (float)(yScale);
		for (unsigned int i = 0; i < storedValueBuffer.size(); i++)
		{
			float x = xOffset - (float)(GetStoredTimeInSeconds(i) * xScale);
			float y = ((float)GetStoredValue(i) * (float)(yScale)) + yOffset;
			if (y >= 0.0 && y <= (float)(yStripHeight)) glVertex2f(x, y);
			else if (y > (float)(yStripHeight)) glVertex2f(x, (float)(yStripHeight));
			else if (y < 0.0) glVertex2f(x, 0.0);
			else glVertex2f(x, (float)(yStripHeight) / 2.0f);
			if (x > float(xStripWidth)) break;
		}
		glEnd();

		// Draw up ticks on graph
		float yTick = ((float)GetStoredValue(0) * (float)(yScale)) + yOffset;
		while(yTick < (float)(yStripHeight))
		{
			yTick = yTick + (float)(yScale);
			// Draw 1 unit up tick on graph
			glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(0.0, yTick);
			glVertex2f(6.0, yTick);
			glEnd();
		}

		// Draw down ticks on graph
		yTick = ((float)GetStoredValue(0) * (float)(yScale)) + yOffset;
		while(yTick > 0.0)
		{
			yTick = yTick - (float)(yScale);
			// Draw 1 unit up tick on graph
			glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(0.0, yTick);
			glVertex2f(6.0, yTick);
			glEnd();
		}
	}

	lockForChartStripDraw = false;
	glPopMatrix();
}

void AnalogSensor::SetInputDevice(short deviceID, short devicePort, short deviceChannel)
{
	SetDeviceID(deviceID);
	SetPortID(devicePort);
	SetChannelID(deviceChannel);
}

// Run to update to current values read from analog input device
bool AnalogSensor::UpdateSensorValue()
{
	try{
		if (ptrAnalogInput != NULL)
		{
			// Get normalized value read from analog input device
			outputValue = ptrAnalogInput->GetNormalizedValue(GetDeviceID(), GetChannelID());
			//Scale value to zero (4mA) and full scale (20mA) values set for this sensor
			if (reverseActing) sensorValue = (outputValue * (minOutput - maxOutput)) + maxOutput - zeroOffset;
			else sensorValue = (outputValue * (maxOutput - minOutput)) + minOutput + zeroOffset;
			// Round off to precision set for this sensor
			sensorValue = ptrMathUtilities->RoundOff(sensorValue, GetPrecision());

			// Store current value and system time in buffer
			ptrSystemTime->SetSystemTimeStruct();
			storedValue thisValue;
			thisValue.value = sensorValue;
			thisValue.year = ptrSystemTime->GetSystemTimeYear();
			thisValue.month = ptrSystemTime->GetSystemTimeMonth();
			thisValue.day = ptrSystemTime->GetSystemTimeDay();
			thisValue.hour = ptrSystemTime->GetSystemTimeHour();
			thisValue.minute = ptrSystemTime->GetSystemTimeMinute();
			thisValue.second = ptrSystemTime->GetSystemTimeSecond();
			thisValue.millisecond = ptrSystemTime->GetSystemTimeMillisecond();

			// Wait for chart recorder draw to finish if in progress
			while (lockForChartStripDraw) ptrSystemTime->Wait(1);
			// Store current value in buffer
			lockStoredValueBuffer = true;
			storedValueBuffer.push_front(thisValue);
			// Drop oldest value if buffer is full
			if (storedValueBuffer.size() > maxBufferSize) storedValueBuffer.pop_back();
			lockStoredValueBuffer = false;
			return true;
		}
		else
		{
			throw ProgramException("Null pointer to AnalogInput object\n");
			return false;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(3, "AnalogSensor::UpdateSensorValue) ", pe.GetExceptionDescription(), true);
	}
	return false;
}

// Run to update to current values read from analog input device
bool AnalogSensor::UpdateSensorValueSmoothed()
{
	try
	{
		if (ptrAnalogInput != NULL)
		{
			double instantSensorValue;

			// Get normalized value read from analog input device
			outputValue = ptrAnalogInput->GetNormalizedValue(GetDeviceID(), GetChannelID());
			// Scale value to zero (4mA) and full scale (20mA) values set for this sensor
			if (reverseActing) instantSensorValue = (outputValue * (minOutput - maxOutput)) + maxOutput - zeroOffset;
			else instantSensorValue = (outputValue * (maxOutput - minOutput)) + minOutput + zeroOffset;
			// Round off to precision set for this sensor

			if (ptrMathUtilities->UpdateSmoothingData(instantSensorValue, 0))
			{
				sensorValue = ptrMathUtilities->GetSmoothedData(0);
				sensorValue = ptrMathUtilities->RoundOff(sensorValue, GetPrecision());

				// Store current smoothed value and system time in buffer
				ptrSystemTime->SetSystemTimeStruct();
				storedValue thisValue;
				thisValue.value = sensorValue;
				thisValue.year = ptrSystemTime->GetSystemTimeYear();
				thisValue.month = ptrSystemTime->GetSystemTimeMonth();
				thisValue.day = ptrSystemTime->GetSystemTimeDay();
				thisValue.hour = ptrSystemTime->GetSystemTimeHour();
				thisValue.minute = ptrSystemTime->GetSystemTimeMinute();
				thisValue.second = ptrSystemTime->GetSystemTimeSecond();
				thisValue.millisecond = ptrSystemTime->GetSystemTimeMillisecond();

				// Wait for chart recorder draw to finish if in progress
				while (lockForChartStripDraw) ptrSystemTime->Wait(1);
				// Store current value in buffer
				lockStoredValueBuffer = true;
				storedValueBuffer.push_front(thisValue);
				// Drop oldest value if buffer is full
				if (storedValueBuffer.size() > maxBufferSize) storedValueBuffer.pop_back();
				lockStoredValueBuffer = false;
				return true;
			}
			else
			{
				throw ProgramException("call to UpdateSmoothingData() failed\n");
				return false;
			}
		}
		else
		{
			throw ProgramException("Null pointer to AnalogInput object\n");
			return false;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(3, "AnalogSensor::UpdateSensorValueSmoothed() ", pe.GetExceptionDescription(), true);
	}
	return false;
}
