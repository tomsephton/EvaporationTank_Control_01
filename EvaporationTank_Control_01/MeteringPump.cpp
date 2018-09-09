#include "StdAfx.h"
#include "MeteringPump.h"

// Class Constructor
MeteringPump::MeteringPump(std::string pumpID) // pumpID should be set to pump identifier for log file "A", "B", etc
{
	location[0] = 0.0; // should be set to physical x,y,z coordinates of sensor in meters
	location[1] = 0.0; // should be set to physical x,y,z coordinates of sensor in meters
	location[2] = 0.0; // should be set to physical x,y,z coordinates of sensor in meters
	for (int index = 0; index < 80; index++)
	{
		pumpName[index] = '\0';
		pumpType[index] = '\0';
	}

	// Instantiate SystemTime object
	ptrSystemTime = new SystemTime();
	ptrMathUtilities = new MathUtilities();
	ptrProgramExceptions = new ProgramExceptions();
	ptrPumpLogFileOutput = new PumpLogFileOutput(pumpID);
	pumpSpeed = 0.0;
	strPumpSpeedUnits = "%";
	pumpSpeedAuto = true;
	pumpOperatingOK = true;
	maxPumpVolumeDeliveredPerMinute = 0.1; // Reset for each pump in initialization method (liters/minute)

	maxBufferSize = 10;
	lockStoredValueBuffer = false;
	lockForChartStripDraw = false;
	maxRateBufferSize = 10;
	lockRateBuffer = false;
	lockForRateDisplay = false;

	mouseOverDisplay = false;
	mouseOverKillBox = false;
	mouseOverDragBox = false;
	for (int i = 0; i < 7; i++)
	{
		mouseOverMenu[i] = false;
	}
	showRateControlMenu = false;
}

// Class Destructor
MeteringPump::~MeteringPump(void)
{
	// Delete SystemTime object
	if (ptrSystemTime != NULL)
	{
		delete ptrSystemTime;
		ptrSystemTime = NULL;
	}
	// Delete MathUtilities object
	if (ptrMathUtilities != NULL)
	{
		delete ptrMathUtilities;
		ptrMathUtilities = NULL;
	}
	// Delete ProgramExceptions object
	if (ptrProgramExceptions != NULL)
	{
		delete ptrProgramExceptions;
		ptrProgramExceptions = NULL;
	}
	// Delete PumpLogFileOutput object
	if (ptrPumpLogFileOutput != NULL)
	{
		delete ptrPumpLogFileOutput;
		ptrPumpLogFileOutput = NULL;
	}
	storedValueBuffer.clear();
	storedRateBuffer.clear();
}

void MeteringPump :: SetOutputDeviceID (short device)
{
	outputDeviceID = device;
}

short MeteringPump :: GetOutputDeviceID (void)
{
	return outputDeviceID;
}

void MeteringPump :: SetOutputChannelID (short channel)
{
	outputChannelID = channel;
}

short MeteringPump :: GetOutputChannelID (void)
{
	return outputChannelID;
}

void MeteringPump :: SetPumpName (char name[80])
{
	strncpy_s(pumpName, name, 80);
}

char* MeteringPump :: GetPumpNamePtr()
{
	return &pumpName[0];
}

void MeteringPump :: SetPumpType (char type[80])
{
	strncpy_s(pumpType, type, 80);
}

char* MeteringPump :: GetPumpTypePtr()
{
	return &pumpType[0];
}

void MeteringPump :: SetLocation (double x, double y, double z)
{	
	location[0] = x;
	location[1] = y;
	location[2] = z;
}

bool MeteringPump :: GetLocation (double returnedLocation [3])
{
	returnedLocation[0] = location [0];
	returnedLocation[1] = location [1];
	returnedLocation[2] = location [2];
	return true;
}

bool MeteringPump::SetMaxPumpVolumeDeliveredPerMinute(double maxVolume)
{
	if (maxVolume > 0.0 && maxVolume < 10.0)
	{
		maxPumpVolumeDeliveredPerMinute = maxVolume;
		return true;
	}
	else return false;
}

void MeteringPump::SetAnalogOutputPtr(AnalogOutput* ptr)
{
	ptrAnalogOutput = ptr;
}

AnalogOutput* MeteringPump::GetAnalogOutputPtr(void)
{
	return ptrAnalogOutput;
}

// Set metering pump speed with input range of 0 to 1 equals 0% to 100%
// Also store any speed change in buffer and log speed changes to file
bool MeteringPump::SetPumpSpeed(float speed)
{
	float mAmpSetting = 4.0;
	try
	{
		if (speed >= 0.0f && speed <= 1.0f)
		{
			mAmpSetting = (speed * 16.0f) + 4.0f;
			if (ptrAnalogOutput != NULL)
			{
				if (ptrAnalogOutput->SetRequestedValveSetting(GetOutputDeviceID(), GetOutputChannelID(), mAmpSetting))
				{
					pumpSpeed = speed;

					// Store rate change time point data
					storedRate thisRate;
					thisRate.rate = speed;
					if (storedRateBuffer.size() > 0)
					{
						thisRate.secondsSinceLastRate = ptrSystemTime->GetSystemTimeInSeconds() - GetRateBufferTimeInSeconds(0);
					}
					else thisRate.secondsSinceLastRate = 0.0;
					thisRate.systemTimeInSeconds = ptrSystemTime->GetSystemTimeInSeconds();
					thisRate.year = ptrSystemTime->GetSystemTimeYear();
					thisRate.month = ptrSystemTime->GetSystemTimeMonth();
					thisRate.day = ptrSystemTime->GetSystemTimeDay();
					thisRate.hour = ptrSystemTime->GetSystemTimeHour();
					thisRate.minute = ptrSystemTime->GetSystemTimeMinute();
					thisRate.second = ptrSystemTime->GetSystemTimeSecond();
					thisRate.millisecond = ptrSystemTime->GetSystemTimeMillisecond();
					// Wait for pump speed indicator draw to finish if in progress
					while (lockForRateDisplay) ptrSystemTime->Wait(1);
					// Store current value in buffer
					lockRateBuffer = true;
					storedRateBuffer.push_front(thisRate);
					// Drop oldest value if buffer is full
					if (storedRateBuffer.size() > maxRateBufferSize) storedRateBuffer.pop_back();
					lockRateBuffer = false;

					// Log pump rate change
					if (pumpSpeedAuto) ptrPumpLogFileOutput->SetDataItem(1, "Pump Rate Reset", "Automatic");
					else ptrPumpLogFileOutput->SetDataItem(1, "Pump Rate Reset", "Operator");
					ptrPumpLogFileOutput->SetDataItem(2, strSystemDisplayName + " " + strPumpDisplayName + " set Speed", speed);
					ptrPumpLogFileOutput->SetDataItem(3, strSystemDisplayName + " " + strPumpDisplayName + " seconds since last set", thisRate.secondsSinceLastRate);
					ptrPumpLogFileOutput->SetDataItem(4, strSystemDisplayName + " " + strPumpDisplayName + " liters in last 24 hrs", GetLast24HourPumpDelivery());
					if (!ptrPumpLogFileOutput->GetColumnLabelsWritten()) ptrPumpLogFileOutput->WriteColumnLabels();
					ptrPumpLogFileOutput->WriteCurrentData();
					return true;
				}
				else // Request to set pump speed failed
				{
					throw ProgramException("Request to set speed failed in " + strPumpDisplayName + "\n");
					return false;
				}
			}	
			else // Null pointer to AnalogOutput object
			{
				throw ProgramException("Null pointer to AnalogOutput object in " + strPumpDisplayName + "\n");
				return false;
			}
		}
		else // Speed value out of range
		{
			throw ProgramException("Speed value out of range in " + strPumpDisplayName + "\n");
			return false;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "MeteringPump::SetPumpSpeed() ", pe.GetExceptionDescription(), true);
	}
	return true;
}

float MeteringPump :: GetPumpSpeed(void)
{
	return pumpSpeed;
}

bool MeteringPump :: GetPumpSpeedAuto(void)
{
	return pumpSpeedAuto;
}

void MeteringPump::SetSystemDisplayName(std::string name)
{
	strSystemDisplayName = name;
}

void MeteringPump::SetPumpDisplayName(std::string name)
{
	strPumpDisplayName = name;
}

void MeteringPump::SetPumpUnits(std::string units)
{
	strPumpSpeedUnits = units;
}

// Metering Pump value method for display
void MeteringPump::SetMaxBufferSize(unsigned int size)
{
	maxBufferSize = size;
}

// Metering Pump value method for display
bool MeteringPump::ResizeStoredValueBuffer(unsigned int newSize)
{
	if (newSize <= maxBufferSize)
	{
		storedValueBuffer.resize(newSize);
		if (storedValueBuffer.size() == newSize) return true;
		else return false;
	}
	else return false;
}

// Metering Pump value method for display
unsigned int MeteringPump::GetStoredValueBufferSize(void)
{
	return storedValueBuffer.size();
}

// Metering Pump value method for display
double MeteringPump::GetStoredValue(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).value;
	else return 0.0;
}

// Metering Pump value method for display
int MeteringPump::GetStoredYear(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).year;
	else return -1;
}

// Metering Pump value method for display
int MeteringPump::GetStoredMonth(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).month;
	else return -1;
}

// Metering Pump value method for display
int MeteringPump::GetStoredDay(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).day;
	else return -1;
}

// Metering Pump value method for display
int MeteringPump::GetStoredHour(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).hour;
	else return -1;
}

// Metering Pump value method for display
int MeteringPump::GetStoredMinute(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).minute;
	else return -1;
}

// Metering Pump value method for display
int MeteringPump::GetStoredSecond(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).second;
	else return -1;
}

// Metering Pump value method for display
int MeteringPump::GetStoredMillisecond(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return storedValueBuffer.at(i).millisecond;
	else return -1;
}

// Metering Pump value method for display
float MeteringPump::GetStoredTimeInSeconds(unsigned int i)
{
	if (storedValueBuffer.size() >= i) return (float)((GetStoredDay(i) * 86400 + GetStoredHour(i) * 3600 + GetStoredMinute(i) * 60 + GetStoredSecond(i)) + (float)(GetStoredMillisecond(i)) / 1000.0f);
	else return -1.0;
}

// Metering Pump rate method for calculations
void MeteringPump::SetMaxRateBufferSize(unsigned int size)
{
	maxRateBufferSize = size;
}

// Metering Pump rate method for calculations
bool MeteringPump::ResizeRateBuffer(unsigned int newSize)
{
	if (newSize <= maxRateBufferSize)
	{
		storedRateBuffer.resize(newSize);
		if (storedRateBuffer.size() == newSize) return true;
		else return false;
	}
	else return false;
}

// Metering Pump rate method for calculations
unsigned int MeteringPump::GetRateBufferSize(void)
{
	return storedRateBuffer.size();
}

// Metering Pump rate method for calculations
double MeteringPump::GetStoredRate(unsigned int i)
{
	if (storedRateBuffer.size() >= i) return storedRateBuffer.at(i).rate;
	else return 0.0;
}

// Metering Pump rate method for calculations
int MeteringPump::GetRateBufferYear(unsigned int i)
{
	if (storedRateBuffer.size() >= i) return storedRateBuffer.at(i).year;
	else return -1;
}

// Metering Pump rate method for calculations
int MeteringPump::GetRateBufferMonth(unsigned int i)
{
	if (storedRateBuffer.size() >= i) return storedRateBuffer.at(i).month;
	else return -1;
}

// Metering Pump rate method for calculations
int MeteringPump::GetRateBufferDay(unsigned int i)
{
	if (storedRateBuffer.size() >= i) return storedRateBuffer.at(i).day;
	else return -1;
}

// Metering Pump rate method for calculations
int MeteringPump::GetRateBufferHour(unsigned int i)
{
	if (storedRateBuffer.size() >= i) return storedRateBuffer.at(i).hour;
	else return -1;
}

// Metering Pump rate method for calculations
int MeteringPump::GetRateBufferMinute(unsigned int i)
{
	if (storedRateBuffer.size() >= i) return storedRateBuffer.at(i).minute;
	else return -1;
}

// Metering Pump rate method for calculations
int MeteringPump::GetRateBufferSecond(unsigned int i)
{
	if (storedRateBuffer.size() >= i) return storedRateBuffer.at(i).second;
	else return -1;
}

// Metering Pump rate method for calculations
int MeteringPump::GetRateBufferMillisecond(unsigned int i)
{
	if (storedRateBuffer.size() >= i) return storedRateBuffer.at(i).millisecond;
	else return -1;
}

// Metering Pump rate method for calculations
double MeteringPump::GetRateBufferTimeInSeconds(unsigned int i)
{
	if (storedRateBuffer.size() >= i) return storedRateBuffer.at(i).systemTimeInSeconds;
	else return -1.0;
}

// Metering pump delivery in last 24 hour period from now (returns value in liters)
double MeteringPump::GetLast24HourPumpDelivery(void)
{
	double delivery = 0.0;
	double timeNow = ptrSystemTime->GetSystemTimeInSeconds();
	double timeOneDayAgo = ptrSystemTime->GetSystemTimeInSeconds() - 86400.0;
	double rateBufferTime = timeNow;
	if (storedRateBuffer.size() > 0)
	{
		while (lockRateBuffer) ptrSystemTime->Wait(1);
		for (unsigned int i = 0; i < storedRateBuffer.size() - 1; i++)
		{
			rateBufferTime = storedRateBuffer.at(i).systemTimeInSeconds;
			if (rateBufferTime < timeOneDayAgo) break;
			delivery = delivery + (storedRateBuffer.at(i + 1).rate * maxPumpVolumeDeliveredPerMinute * (storedRateBuffer.at(i).secondsSinceLastRate/60.0));
		}
		delivery = ptrMathUtilities->RoundOff(delivery, 1);
		return delivery;
	}
	else return 0.0;
}

// Metering pump delivery in 24 hour period prior to time/date specified (returns value in liters)
double MeteringPump::GetAny24HourPumpDelivery(int year, int month, int day, int hour)
{
	double delivery = 0.0;
	unsigned int startIndex;
	try
	{
		if (storedRateBuffer.size() > 0)
		{
			// Check date and time input values for reasonable range
			if (year < 2015 || year > storedRateBuffer.front().year) throw ProgramException("Year value out of range for " + strPumpDisplayName + "\n");
			if (month < 1 || month > 12) throw ProgramException("Month value out of range for " + strPumpDisplayName + "\n");
			if (day < 1 || day > 31) throw ProgramException("Day value out of range for " + strPumpDisplayName + "\n");
			if (hour < 0 || hour > 24) throw ProgramException("Hour value out of range for " + strPumpDisplayName + "\n");
			
			while (lockRateBuffer) ptrSystemTime->Wait(1);
			for (unsigned int i = 0; i < storedRateBuffer.size(); i++)
			{
				if (storedRateBuffer.at(i).year <= year && storedRateBuffer.at(i).month <= month && storedRateBuffer.at(i).day <= day && storedRateBuffer.at(i).hour <= hour)
				{
					startIndex = i;
				}
				else break;
			}

			double timeStart = storedRateBuffer.at(startIndex).systemTimeInSeconds;
			double timeOneDayEarlier = timeStart - 86400.0;
			double rateBufferTime = timeStart;
			for (unsigned int i = startIndex; i < storedRateBuffer.size() - startIndex - 1; i++)
			{
				rateBufferTime = storedRateBuffer.at(i).systemTimeInSeconds;
				if (rateBufferTime < timeOneDayEarlier) break;
				delivery = delivery + (storedRateBuffer.at(i + 1).rate * maxPumpVolumeDeliveredPerMinute * (storedRateBuffer.at(i).secondsSinceLastRate/60.0));
			}
			delivery = ptrMathUtilities->RoundOff(delivery, 1);
			return delivery;
		}
		else return 0.0;
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "MeteringPump::GetAny24HourPumpDelivery() ", pe.GetExceptionDescription(), true);
		return 0.0;
	}
}

bool MeteringPump :: GetMouseOverDisplay(int mouseX, int mouseY)
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

bool MeteringPump :: GetMouseOverKillBox(int mouseX, int mouseY)
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

bool MeteringPump :: GetMouseOverDragBox(int mouseX, int mouseY)
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

int MeteringPump :: GetMouseOverMenu(int mouseX, int mouseY)
{
	if (mouseX > displayEdges.menuTopLeft[0][0] && mouseX < displayEdges.menuBottomRight[0][0] && mouseY < displayEdges.menuTopLeft[0][1] && mouseY > displayEdges.menuBottomRight[0][1])
	{
		mouseOverMenu[0] = true;
	}
	else
	{
		mouseOverMenu[0] = false;
		return -1;
	}
	for (int i = 1; i < 7; i++)
	{
		if (mouseX > displayEdges.menuTopLeft[i][0] && mouseX < displayEdges.menuBottomRight[i][0] && mouseY < displayEdges.menuTopLeft[i][1] && mouseY > displayEdges.menuBottomRight[i][1])
		{
			mouseOverMenu[i] = true;
		}
		else mouseOverMenu[i] = false;
	}
	for (int i = 1; i < 7; i++)
	{
		if (mouseOverMenu[i]) return i;
	}
	return 0;
}

void MeteringPump :: CheckMouseAction(int mouseX, int mouseY, int mouseAction)
{
	GetMouseOverDisplay(mouseX, mouseY);

	if (GetMouseOverKillBox(mouseX, mouseY) && mouseAction == 1)
	{
		if (pumpOperatingOK) pumpOperatingOK = false;
		else pumpOperatingOK = true;
	}

	if (GetMouseOverDragBox(mouseX, mouseY) && mouseAction == 0)
	{
		showRateControlMenu = true;
	}
	else if (GetMouseOverMenu(mouseX, mouseY) >= 0 && mouseAction == 0)
	{
		showRateControlMenu = true;
	}
	else showRateControlMenu = false;

	if (GetMouseOverMenu(mouseX, mouseY) == 1 && mouseAction == 1)
	{
		pumpSpeedAuto = false;
		SetPumpSpeed(0.0);
	}
	else if (GetMouseOverMenu(mouseX, mouseY) == 2 && mouseAction == 1)
	{
		pumpSpeedAuto = false;
		SetPumpSpeed(0.25);
	}
	else if (GetMouseOverMenu(mouseX, mouseY) == 3 && mouseAction == 1)
	{
		pumpSpeedAuto = false;
		SetPumpSpeed(0.5);
	}
	else if (GetMouseOverMenu(mouseX, mouseY) == 4 && mouseAction == 1)
	{
		pumpSpeedAuto = false;
		SetPumpSpeed(0.75);
	}
	else if (GetMouseOverMenu(mouseX, mouseY) == 5 && mouseAction == 1)
	{
		pumpSpeedAuto = false;
		SetPumpSpeed(1.0);
	}
	else if (GetMouseOverMenu(mouseX, mouseY) == 6 && mouseAction == 1)
	{
		pumpSpeedAuto = true;
	}
}

// Draws pump speed and volume delivered in last 24 hrs data and displays interactive pump speed control
void MeteringPump::DrawDataBox2D(GLint nFontList, int xPos, int yPos, int xOrigin, int yOrigin)
{
	lockForRateDisplay = true;
	int numberLines = 2;
	int numberMenuLines = 1;
	int maxLineLength = 0;
	int maxMenuLineLength = 0;

	strstrPumpSpeed << strPumpDisplayName << " " << GetPumpSpeed() * 100.0f << " " << strPumpSpeedUnits;
	strPumpSpeed = strstrPumpSpeed.str();
	strstrPumpSpeed.str("");

	strstrPumpDelivery << "Volume in last 24 hours " << GetLast24HourPumpDelivery() << " liters";
	strPumpDelivery = strstrPumpDelivery.str();
	strstrPumpDelivery.str("");

	strstrPumpMenuLabel << "Override Pump Rate or return to Auto";
	strPumpMenuLabel = strstrPumpMenuLabel.str();
	strstrPumpMenuLabel.str("");

	strstrPumpMenuText[0] << "  0% ";
	strPumpMenuText[0] = strstrPumpMenuText[0].str();
	strstrPumpMenuText[0].str("");

	strstrPumpMenuText[1] << " 25% ";
	strPumpMenuText[1] = strstrPumpMenuText[1].str();
	strstrPumpMenuText[1].str("");

	strstrPumpMenuText[2] << " 50% ";
	strPumpMenuText[2] = strstrPumpMenuText[2].str();
	strstrPumpMenuText[2].str("");

	strstrPumpMenuText[3] << " 75% ";
	strPumpMenuText[3] = strstrPumpMenuText[3].str();
	strstrPumpMenuText[3].str("");

	strstrPumpMenuText[4] << "100% ";
	strPumpMenuText[4] = strstrPumpMenuText[4].str();
	strstrPumpMenuText[4].str("");

	strstrPumpMenuText[5] << " Auto ";
	strPumpMenuText[5] = strstrPumpMenuText[5].str();
	strstrPumpMenuText[5].str("");
	
	// Set box width to longest line to be displayed
	if (strSystemDisplayName.size() > strPumpSpeed.size() && strSystemDisplayName.size() > strPumpDelivery.size()) maxLineLength = strSystemDisplayName.size() + 1;
	else if (strPumpDelivery.size() > strPumpSpeed.size()) maxLineLength = strPumpDelivery.size() + 1;
	else maxLineLength = strPumpSpeed.size() + 1;

	// Box modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(float(xPos), float(yPos), 0.0);

	// Draw pump control menu box on mouse over
	if (showRateControlMenu)
	{
		// Set width of pump control box by text
		for (int i = 0; i < 6; i++)
		{
			maxMenuLineLength = maxMenuLineLength + strPumpMenuText[i].size();
		}
		maxMenuLineLength = maxMenuLineLength + 8;

		glPushMatrix();
		// Translate above data box
		glTranslatef(0.0f, (float (numberLines) * 20.0f) + 28.0f, 0.0f);

		// Top outer border of data box
		glBegin(GL_POLYGON);
		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex2f(-7.0f, float(numberMenuLines + 1) * 20.0f + 7.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 7.0f, float(numberMenuLines + 1) * 20.0f + 7.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 5.0f, float(numberMenuLines + 1) * 20.0f + 5.0f);
		glVertex2f(-5.0f, float(numberMenuLines + 1) * 20.0f + 5.0f);
		glEnd();

		// Set top left overall menu coordinates
		displayEdges.menuTopLeft[0][0] = xOrigin + xPos - 7;
		displayEdges.menuTopLeft[0][1] = yOrigin + yPos + (numberLines * 20) + 28 + ((numberMenuLines + 1) * 20) + 7;

		// Right outer border of data box
		glBegin(GL_POLYGON);
		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 7.0f, -7.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 5.0f, -5.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 5.0f, float(numberMenuLines + 1) * 20.0f + 5.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 7.0f, float(numberMenuLines + 1) * 20.0f + 7.0f);
		glEnd();

		// Lower outer border of data box
		glBegin(GL_POLYGON);
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 7.0f, -7.0f);
		glVertex2f(-7.0f, -7.0f);
		glVertex2f(-5.0f, -5.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 5.0f, -5.0f);
		glEnd();

		// Set bottom right overall menu coordinates
		displayEdges.menuBottomRight[0][0] = xOrigin + xPos + ((maxMenuLineLength) * 8) + 7;
		displayEdges.menuBottomRight[0][1] = yOrigin + yPos + (numberLines * 20) + 28 - 7;

		// Left outer border of data box
		glBegin(GL_POLYGON);
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex2f(-7.0f, -7.0f);
		glVertex2f(-7.0f, float(numberMenuLines + 1) * 20.0f + 7.0f);
		glVertex2f(-5.0f, float(numberMenuLines + 1) * 20.0f + 5.0f);
		glVertex2f(-5.0f, -5.0f);
		glEnd();

		// Outer Rim of data box
		glBegin(GL_POLYGON);
		if (mouseOverDragBox) glColor3f(0.6f, 0.6f, 0.6f);
		else glColor3f(0.7f, 0.7f, 0.7f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 5.0f, -5.0f);
		glVertex2f(-5.0f, -5.0f);
		glVertex2f(-5.0f, float(numberMenuLines + 1) * 20.0f + 5.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 5.0f, float(numberMenuLines + 1) * 20.0f + 5.0f);
		glEnd();

		// Top inner border of data box
		glBegin(GL_POLYGON);
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex2f(-3.0f, float(numberMenuLines * 20.0) + 3.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 3.0f, float(numberMenuLines) * 20.0f + 3.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 1.0f, float(numberMenuLines) * 20.0f + 1.0f);
		glVertex2f(-1.0f, float(numberMenuLines * 20.0) + 1.0f);
		glEnd();

		// Right inner border of data box
		glBegin(GL_POLYGON);
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 3.0f, -3.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 1.0f, -1.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 1.0f, float(numberMenuLines) * 20.0f + 1.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 3.0f, float(numberMenuLines) * 20.0f + 3.0f);
		glEnd();

		// Lower inner border of data box
		glBegin(GL_POLYGON);
		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 3.0f, -3.0f);
		glVertex2f(-3.0f, -3.0f);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(float(maxMenuLineLength) * 8.0f + 1.0f, -1.0f);
		glEnd();

		// Left inner border of data box
		glBegin(GL_POLYGON);
		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex2f(-3.0f, -3.0f);
		glVertex2f(-3.0f, float(numberMenuLines) * 20.0f + 3.0f);
		glVertex2f(-1.0f, float(numberMenuLines) * 20.0f + 1.0f);
		glVertex2f(-1.0f, -1.0f);
		glEnd();

		// Text Background of pump rate control menu box
		for (int i = 0; i < 6; i++)
		{
			glPushMatrix();
			glTranslatef(float (i*52 + 1), 0.0f, 0.0f);
			glBegin(GL_POLYGON);
			// Display green text background if value is within full pump operating range, otherwise yellow, or red when flagged as failed
			if (!pumpOperatingOK)
			{
				if (mouseOverMenu[i+1]) glColor3f(0.9f, 0.2f, 0.2f);
				else glColor3f(0.7f, 0.3f, 0.3f);
			}
			else
			{
				if (mouseOverMenu[i+1]) glColor3f(0.1f, 0.9f, 0.1f);
				else glColor3f(0.2f, 0.7f, 0.2f);
			}

			// Text background
			glVertex2f(float(50) + 1.0f, -1.0f);
			glVertex2f(-1.0f, -1.0f);
			glVertex2f(-1.0f, float(numberMenuLines * 20.0) + 1.0f);
			glVertex2f(float(50) + 1.0f, float(numberMenuLines) * 20.0f + 1.0f);
			glEnd();

			// Divider bar left
			glBegin(GL_POLYGON);
			glColor3f(0.3f, 0.3f, 0.3f);
			glVertex2f(float(50) + 0.0f, -1.0f);
			glVertex2f(float(50) - 1.0f, -1.0f);
			glVertex2f(float(50) - 1.0f, float(numberMenuLines * 20.0) + 1.0f);
			glVertex2f(float(50) + 0.0f, float(numberMenuLines) * 20.0f + 1.0f);
			glEnd();

			// divider bar right
			glBegin(GL_POLYGON);
			glColor3f(0.9f, 0.9f, 0.9f);
			glVertex2f(float(50) + 1.0f, -1.0f);
			glVertex2f(float(50) - 0.0f, -1.0f);
			glVertex2f(float(50) - 0.0f, float(numberMenuLines * 20.0) + 1.0f);
			glVertex2f(float(50) + 1.0f, float(numberMenuLines) * 20.0f + 1.0f);
			glEnd();

			// Set top left menu cell coordinates
			displayEdges.menuTopLeft[i+1][0] = xOrigin + xPos - 1 + (i*52 + 1);
			displayEdges.menuTopLeft[i+1][1] = yOrigin + yPos + (numberLines * 20) + 28 + ((numberMenuLines + 1) * 20) + 7;
			// Set bottom right menu cell coordinates
			displayEdges.menuBottomRight[i+1][0] = xOrigin + xPos + 50 + 1 + (i*52 + 1);
			displayEdges.menuBottomRight[i+1][1] = yOrigin + yPos + (numberLines * 20) + 28 - 7;

			glPopMatrix();
		}

		int lineNumbers = 1;
		// Display Menu Name string
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos2i(8, (lineNumbers * 20) + 8);
		glListBase(nFontList);
		glCallLists (strPumpMenuLabel.size(), GL_UNSIGNED_BYTE, strPumpMenuLabel.c_str());
		strPumpMenuLabel = "";

		// Display pump rate control menu text
		lineNumbers = 0;
		for (int i = 0; i < 6; i++)
		{
			if (strPumpMenuText[i].size() > 0)
			{	
				glRasterPos2i(i*52 + 8, (lineNumbers * 20) + 4);
				glListBase(nFontList);
				glCallLists (strPumpMenuText[i].size(), GL_UNSIGNED_BYTE, strPumpMenuText[i].c_str());
				strPumpMenuText[i] = "";
			}
		}
		glPopMatrix();
	}

	// Draw pump data box
	numberLines = 2;

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
	// Display green text background if value is within full pump operating range, otherwise yellow, or red when flagged as failed
	if (!pumpOperatingOK)
	{
		if (mouseOverDisplay) glColor3f(0.7f, 0.2f, 0.2f);
		else glColor3f(0.9f, 0.3f, 0.3f);
	}
	else if (GetPumpSpeed() >= 0.0 && GetPumpSpeed() <= 1.0)
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

	int lineNumber = 2;
	// Display Sensor Name string
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2i(8, (lineNumber * 20) + 8);
	glListBase(nFontList);
	glCallLists (strSystemDisplayName.size(), GL_UNSIGNED_BYTE, strSystemDisplayName.c_str());

	// Display pump rate and units
	lineNumber = 1;
	if (strPumpSpeed.size() > 0)
	{	
		glRasterPos2i(8, (lineNumber * 20) + 4);
		glListBase(nFontList);
		glCallLists (strPumpSpeed.size(), GL_UNSIGNED_BYTE, strPumpSpeed.c_str());
		strPumpSpeed = "";
	}

	// Display pump last 24 hour delivery and units
	lineNumber = 0;
	if (strPumpDelivery.size() > 0)
	{	
		glRasterPos2i(8, (lineNumber * 20) + 4);
		glListBase(nFontList);
		glCallLists (strPumpDelivery.size(), GL_UNSIGNED_BYTE, strPumpDelivery.c_str());
		strPumpDelivery = "";
	}
	glPopMatrix();
	lockForRateDisplay = false;
	ptrProgramExceptions->DisplayProgramExceptions(nFontList, 0, 240);
}

void MeteringPump::DrawChartRecorderStrip2D(GLint nFontList, int xStripPos, int yStripPos, int xStripWidth, int yStripHeight, int xScale, int yScale, bool autoScale, bool autoScaleOnExceed)
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
	if (GetPumpSpeed() >= 0.0 && GetPumpSpeed() <= 1.0) glColor3f(0.3f, 0.9f, 0.3f);
	else glColor3f(0.9f, 0.9f, 0.1f);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, (float)(yStripHeight));
	glVertex2f((float)(xStripWidth), (float)(yStripHeight));
	glVertex2f((float)(xStripWidth), 0.0);
	glEnd();

	strstrPumpSpeed << strSystemDisplayName << " " << strPumpDisplayName << " = " << GetPumpSpeed() * 100.0f << " " << strPumpSpeedUnits;
	strPumpSpeed = strstrPumpSpeed.str();
	strstrPumpSpeed.str("");

	// Display Sensor Name, Value, and Units
	if (strPumpSpeed.size() > 0)
	{	
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos2i(8, yStripHeight - 12);
		glListBase(nFontList);
		glCallLists (strPumpSpeed.size(), GL_UNSIGNED_BYTE, strPumpSpeed.c_str());
		strPumpSpeed = "";
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

// Run to update chart strip display buffer to current values
bool MeteringPump::UpdatePumpSpeed(void)
{
		// TEMPORARY TEST CODE
//	float currentTime;
//	currentTime = (float)ptrSystemTime->GetSystemTimeSecond();
//	SetPumpSpeed(currentTime/100.0f);
	// TEMPORARY TEST CODE

	// Store current value and system time in buffer
	ptrSystemTime->SetSystemTimeStruct();
	storedValue thisValue;
	thisValue.value = GetPumpSpeed();
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