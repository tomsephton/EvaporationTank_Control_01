#include "StdAfx.h"
#include "BinarySensor.h"

// Class Constructor
BinarySensor::BinarySensor(void)
{
	status = -1;
	maxBufferSize = 10;
	lockStoredValueBuffer = false;
	lockForChartStripDraw = false;
	storedValueBuffer.clear();

	maxStateChangeBufferSize = 10;
	lockStateChangeBuffer = false;
	lockForStateChangeDraw = false;
	stateChangeBuffer.clear();

	timeBetweenUpdates = 100.0f;
	averageSecondsBetweenStateChange = 0.0;
}

// Class Destructor
BinarySensor::~BinarySensor(void)
{
	status = -1;
	stateChangeBuffer.clear();
	storedValueBuffer.clear();
}

bool BinarySensor :: SetInputDevice(short deviceID, short devicePortID, short deviceChannelID)
{
	try
	{
		// Check that arguments are within range for DBK23 input device
		if (deviceID >= 0 && deviceID <= 7) SetDeviceID(deviceID);
		else
		{
			SetDeviceID(-1);
			throw ProgramException("inputDeviceID out of range\n");
			return false;
		}

		if (devicePortID >= 0 && devicePortID <= 3) SetPortID(devicePortID);
		else
		{
			SetDeviceID(-1);
			SetPortID(-1);
			throw ProgramException("devicePortID out of range\n");
			return false;
		}

		if (deviceChannelID >= 0 && deviceChannelID <= 7) SetChannelID(deviceChannelID);
		else
		{
			SetDeviceID(-1);
			SetPortID(-1);
			SetChannelID(-1);
			throw ProgramException("deviceChannelID out of range\n");
			return false;
		}
		return true;
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "BinarySensor::SetInputDevice ", pe.GetExceptionDescription(), true);
	}
	return false;
}

bool BinarySensor :: SetStatus(short newStatus)
{
	try
	{
		if (newStatus == -1)
		{
			status = newStatus;
			throw ProgramException("BinaryInput object failed to get good value, returned -1");
			return false;
		}
		if (newStatus == 0 || newStatus == 1)
		{
			status = newStatus;
			// TEMPORARY TEST CODE START
//			if (ptrSystemTime->GetSystemTimeMinute()%2 == 0) status = 0;
//			else status = 1;
			// TEMPORARY TEST CODE END
			return true;
		}
		else
		{
			throw ProgramException("invalid parameter\n");
			return false;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(2, "BinarySensor::SetStatus ", pe.GetExceptionDescription(), true);
	}
	return false;
}
	
short BinarySensor::GetStatus(void)
{
	return status;
}

void BinarySensor::SetBinaryInputPtr(BinaryInput* ptr)
{
	ptrBinaryInput = ptr;
}

bool BinarySensor::UpdateStatus(void)
{
	try
	{
		bool stateChanged = false;
		if (GetDeviceID() != -1 && GetPortID() != -1 && GetChannelID() != -1)
		{
			if (ptrBinaryInput != NULL)
			{
				if (SetStatus(ptrBinaryInput->GetBinaryValue(GetDeviceID(), GetPortID(), GetChannelID())))
				{
					// Store current value and system time in buffer
					ptrSystemTime->SetSystemTimeStruct();
					storedValue thisValue;
					thisValue.value = status;
					stateChange thisChange;
					if (storedValueBuffer.size() >= 1)
					{
						if (storedValueBuffer.front().value == status)
						{
							// No state change, increment cycles without a state change
							thisValue.timeSlicesSinceLastChange = storedValueBuffer.front().timeSlicesSinceLastChange + 1;
							if (thisValue.timeSlicesSinceLastChange > 4294967294) thisValue.timeSlicesSinceLastChange = 4294967294;
						}
						else
						{
							// State change, reset cycles without a state change, add record to State Change buffer
							stateChanged = true;
							thisValue.timeSlicesSinceLastChange = 0;

							// Set change data in state change buffer
							thisChange.value = status;
							thisChange.year = ptrSystemTime->GetSystemTimeYear();
							thisChange.month = ptrSystemTime->GetSystemTimeMonth();
							thisChange.day = ptrSystemTime->GetSystemTimeDay();
							thisChange.hour = ptrSystemTime->GetSystemTimeHour();
							thisChange.minute = ptrSystemTime->GetSystemTimeMinute();
							thisChange.second = ptrSystemTime->GetSystemTimeSecond();
							thisChange.millisecond = ptrSystemTime->GetSystemTimeMillisecond();
							thisChange.secondsSinceLastChange = (double) storedValueBuffer.front().timeSlicesSinceLastChange / (1000.0 / timeBetweenUpdates);
						}
					}
					else thisValue.timeSlicesSinceLastChange = 0;
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

					if (stateChanged)
					{
						// Store current value in buffer
						stateChangeBuffer.push_front(thisChange);
						// Drop oldest value if buffer is full
						if (stateChangeBuffer.size() > maxStateChangeBufferSize)
						{
							stateChangeBuffer.pop_back();
							ResizeStateChangeBuffer(maxStateChangeBufferSize);
						}
						// Update average time between state changes
						if (stateChangeBuffer.size() > 1)
						{
							double total = 0.0;
							for (unsigned int index = 0; index < stateChangeBuffer.size(); index ++)
							{
								total = total + stateChangeBuffer.at(index).secondsSinceLastChange;
							}
							averageSecondsBetweenStateChange = total/((double) stateChangeBuffer.size());
						}
					}
					lockStoredValueBuffer = false;
					return true;
				}
				else
				{
					throw ProgramException("call to SetStatus() failed");
					return false;
				}
			}
			// Null pointer to BinaryInput object
			else
			{
				throw ProgramException("Null pointer to BinaryInput object");
				return false;
			}
		}
		// ID parameter for BinaryInput object not set
		else
		{
			throw ProgramException("ID parameter for BinaryInput object not set");
			return false;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(3, "BinarySensor::UpdateStatus ", pe.GetExceptionDescription(), true);
	}
	return false;
}

void BinarySensor::SetMaxBufferSize(unsigned int size)
{
	maxBufferSize = size;
}

bool BinarySensor::ResizeStoredValueBuffer(unsigned int newSize)
{
	if (newSize <= maxBufferSize)
	{
		storedValueBuffer.resize(newSize);
		if (storedValueBuffer.size() == newSize) return true;
		else return false;
	}
	else return false;
}

unsigned int BinarySensor::GetStoredValueBufferSize(void)
{
	return storedValueBuffer.size();
}

short int BinarySensor::GetStoredValue(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).value;
	else return -1;
}

long unsigned int BinarySensor::GetTimeSlicesSinceLastChange()
{
	if (storedValueBuffer.size() >= 1) return storedValueBuffer.front().timeSlicesSinceLastChange;
	else return 0;
}

int BinarySensor::GetStoredYear(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).year;
	else return -1;
}

int BinarySensor::GetStoredMonth(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).month;
	else return -1;
}

int BinarySensor::GetStoredDay(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).day;
	else return -1;
}

int BinarySensor::GetStoredHour(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).hour;
	else return -1;
}

int BinarySensor::GetStoredMinute(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).minute;
	else return -1;
}

int BinarySensor::GetStoredSecond(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).second;
	else return -1;
}

int BinarySensor::GetStoredMillisecond(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).millisecond;
	else return -1;
}

float BinarySensor::GetStoredTimeInSeconds(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return (float)((GetStoredDay(i) * 86400 + GetStoredHour(i) * 3600 + GetStoredMinute(i) * 60 + GetStoredSecond(i)) + (float)(GetStoredMillisecond(i)) / 1000.0f);
	else return 0.0;
}

void BinarySensor::SetMaxStateChangeBufferSize(unsigned int size)
{
	maxStateChangeBufferSize = size;
}

bool BinarySensor::ResizeStateChangeBuffer(unsigned int newSize)
{
	if (newSize <= maxStateChangeBufferSize)
	{
		stateChangeBuffer.resize(newSize);
		if (stateChangeBuffer.size() == newSize) return true;
		else return false;
	}
	else return false;
}

unsigned int BinarySensor::GetStateChangeBufferSize(void)
{
	return stateChangeBuffer.size();
}

short int BinarySensor::GetStateChangeState(unsigned int i)
{
	if (stateChangeBuffer.size() >= i) return stateChangeBuffer.at(i).value;
	else return -1;
}

double BinarySensor::GetSecondsSinceLastChange()
{
	if (stateChangeBuffer.size() >= 1) return stateChangeBuffer.front().secondsSinceLastChange;
	else return 0.0;
}

void BinarySensor::SetSystemDisplayName(std::string name)
{
	strSystemDisplayName = name;
}

void BinarySensor::SetSensorDisplayName(std::string name)
{
	strSensorDisplayName = name;
}

// Set frequency at which values will be updated in milliseconds
void BinarySensor::SetTimeBetweenUpdates(int milliseconds)
{
	timeBetweenUpdates = milliseconds;
}

void BinarySensor::DrawDataBox2D(GLint nFontList, int xPos, int yPos, int xOrigin, int yOrigin)
{
	lockForChartStripDraw = true;

	int numberLines = 1;
	int maxLineLength = 0;

//	strstrSensorValue << strSensorDisplayName << " " << GetTimeSlicesSinceLastChange();
//	strstrSensorValue << strSensorDisplayName << " " << GetSecondsSinceLastChange();
	strstrSensorValue << strSensorDisplayName << " " << GetSecondsSinceLastChange() << " avg=" << averageSecondsBetweenStateChange;
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
	glColor3f(0.7f, 0.7f, 0.7f);
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

	// Text Background of data box
	glBegin(GL_POLYGON);
	// Display green text background if value is within full senor output range, otherwise yellow, or red when flagged as failed
	if (!sensorOperatingOK)
	{
		glColor3f(0.9f, 0.3f, 0.3f);
	}
	else if (GetStatus() == 0 || GetStatus() == 1)
	{
		glColor3f(0.2f, 0.8f, 0.2f);
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
	lockForChartStripDraw = false;
}
