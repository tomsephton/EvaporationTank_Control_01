#include "StdAfx.h"
#include "LiquidTank.h"

// Class Constructor
LiquidTank::LiquidTank(void)
{
	tankName[0] = NULL;
	upperLiquidTemperature = 0.0;
	lowerLiquidTemperature = 0.0;
	maxVolume = 0.0;
	fillVolumeSwitch = 3.0;
	fillVolumeSensor = 2.0;
	surfaceArea = 1.0517;
	fillSourceAvailable = false;

	upperLiquidTemperatureSensorExists = true;
	lowerLiquidTemperatureSensorExists = true;
	distillateMeteringPumpExists = true;

	continuousLevelSensorExists = false;
	continuousLevelSensorControlling = false;
	ambientTemperatureSensorExists = false;
	pyranometerExists = false;

	highAlarmExists = false;
	highAlarmActive = false;
	highAlarmOn = -1;

	lowAlarmExists = false;
	lowAlarmActive = false;
	lowAlarmOn = -1;

	highLevelExists = false;
	highLevelActive = false;
	highLevelOn = -1;

	lowLevelExists = false;
	lowLevelActive = false;
	lowLevelOn = -1;

	draining = false;
	filling = false;
	cleaning = false;
	drainStartTime = 0.0;
	lastDrainTime = 0.0;
	fillStartTime = 0.0;
	lastFillTime = 0.0;

	outputDeviceID = -1;
	outputPortID = -1;
	outputChannelID = -1;

	highLevelInputDeviceID = -1;
	highLevelInputDevicePort = -1;
	highLevelInputDeviceChannel = -1;

	lowLevelInputDeviceID = -1;
	lowLevelInputDevicePort = -1;
	lowLevelInputDeviceChannel = -1;

	highAlarmInputDeviceID = -1;
	highAlarmInputDevicePort = -1;
	highAlarmInputDeviceChannel = -1;

	lowAlarmInputDeviceID = -1;
	lowAlarmInputDevicePort = -1;
	lowAlarmInputDeviceChannel = -1;

	mouseOverCleaningDisplay = false;
	mouseOverCleaningDragBox = false;

	maxFillRateBufferSize = 10;
	lockFillRateBuffer = false;
	lockForFillRateDisplay = false;
	lockRateBuffer = false;
	lockForRateDisplay = false;

	ptrSystemTime = new SystemTime();
	ptrMathUtilities = new MathUtilities();
	ptrProgramExceptions = new ProgramExceptions();
	ptrLogFileOutput = NULL;
	ptrTankFillLogFileOutput = NULL;
}

// Class Destructor
LiquidTank::~LiquidTank(void)
{
	if (highLevelExists)
	{
		delete ptrHighLevelSwitch;
		ptrHighLevelSwitch = NULL;
	}

	if (lowLevelExists)
	{
		delete ptrLowLevelSwitch;
		ptrLowLevelSwitch = NULL;
	}

	if (highAlarmExists)
	{
		delete ptrHighAlarmSwitch;
		ptrHighAlarmSwitch = NULL;
	}

	if (lowAlarmExists)
	{
		delete ptrLowAlarmSwitch;
		ptrLowAlarmSwitch = NULL;
	}

	if (upperLiquidTemperatureSensorExists)
	{
		delete ptrUpperTemperatureProbe;
		ptrUpperTemperatureProbe = NULL;
	}

	if (lowerLiquidTemperatureSensorExists)
	{
		delete ptrLowerTemperatureProbe;
		ptrLowerTemperatureProbe = NULL;
	}

	if (distillateMeteringPumpExists)
	{
		delete ptrDistillateMeteringPump;
		ptrDistillateMeteringPump = NULL;
	}

	if (ambientTemperatureSensorExists)
	{
		delete ptrAmbientTemperatureProbe;
		ptrAmbientTemperatureProbe = NULL;
	}

	if (pyranometerExists)
	{
		delete ptrPyranometer;
		ptrPyranometer = NULL;
	}

	if (ptrSystemTime != NULL)
	{
		delete ptrSystemTime;
		ptrSystemTime = NULL;
	}

	if (ptrMathUtilities != NULL)
	{
		delete ptrMathUtilities;
		ptrMathUtilities = NULL;
	}

	if (ptrProgramExceptions != NULL)
	{
		delete ptrProgramExceptions;
		ptrProgramExceptions = NULL;
	}
	storedFillRateBuffer.clear();
}

// Creates objects such as level switches and temperature sensors
void LiquidTank::InstantiateObjects(BinaryInput* ptrBinaryInput, BinaryOutput* ptrBinaryOutput, AnalogInput* ptrAnalogInput, AnalogOutput* ptrAnalogOutput, std::string id)
{
	if (highLevelExists)
	{
		ptrHighLevelSwitch = new LevelSwitch();
		ptrHighLevelSwitch->SetBinaryInputPtr(ptrBinaryInput);
	}
	if (lowLevelExists)
	{
		ptrLowLevelSwitch = new LevelSwitch();
		ptrLowLevelSwitch->SetBinaryInputPtr(ptrBinaryInput);
	}
	if (highAlarmExists)
	{
		ptrHighAlarmSwitch = new LevelSwitch();
		ptrHighAlarmSwitch->SetBinaryInputPtr(ptrBinaryInput);
	}
	if (lowAlarmExists)
	{
		ptrLowAlarmSwitch = new LevelSwitch();
		ptrLowAlarmSwitch->SetBinaryInputPtr(ptrBinaryInput);
	}
	if (continuousLevelSensorExists)
	{
		ptrTankLevelSensor = new LevelSensor();
		ptrTankLevelSensor->SetAnalogInputPtr(ptrAnalogInput);
	}
	if (upperLiquidTemperatureSensorExists)
	{
		ptrUpperTemperatureProbe = new TemperatureProbe();
		ptrUpperTemperatureProbe->SetAnalogInputPtr(ptrAnalogInput);
	}
	if (lowerLiquidTemperatureSensorExists)
	{
		ptrLowerTemperatureProbe = new TemperatureProbe();
		ptrLowerTemperatureProbe->SetAnalogInputPtr(ptrAnalogInput);
	}
	if (distillateMeteringPumpExists)
	{
		ptrDistillateMeteringPump = new MeteringPump(id);
		ptrDistillateMeteringPump->SetAnalogOutputPtr(ptrAnalogOutput);
	}
	if (ambientTemperatureSensorExists)
	{
		ptrAmbientTemperatureProbe = new TemperatureProbe();
		ptrAmbientTemperatureProbe->SetAnalogInputPtr(ptrAnalogInput);
	}
	if (pyranometerExists)
	{
		ptrPyranometer = new Pyranometer();
		ptrPyranometer->SetAnalogInputPtr(ptrAnalogInput);
	}
	ptrTankFillLogFileOutput = new TankFillLogFileOutput(id);
}

// Set and Get methods for object pointers

SystemTime* LiquidTank::GetSystemTimePtr(void)
{
	return ptrSystemTime;
}

void LiquidTank::SetLogFileOutputPtr(LogFileOutput* ptr)
{
	ptrLogFileOutput = ptr;
}

LogFileOutput* LiquidTank::GetLogFileOutputPtr(void)
{
	return ptrLogFileOutput;
}

TemperatureProbe* LiquidTank::GetUpperTemperatureProbePtr(void)
{
	return ptrUpperTemperatureProbe;
}

TemperatureProbe* LiquidTank::GetLowerTemperatureProbePtr(void)
{
	return ptrLowerTemperatureProbe;
}

MeteringPump* LiquidTank::GetDistillateMeteringPumpPtr(void)
{
	return ptrDistillateMeteringPump;
}

TemperatureProbe* LiquidTank::GetAmbientTemperatureProbePtr(void)
{
	return ptrAmbientTemperatureProbe;
}

Pyranometer* LiquidTank::GetPyranometerPtr(void)
{
	return ptrPyranometer;
}

// Member variable Set and Get methods

void LiquidTank :: SetVesselName (char name[80])
{
	strncpy_s(tankName, 80, name, 80);
}

char* LiquidTank :: GetVesselNamePtr()
{
	return &tankName[0];
}

std::string LiquidTank :: GetTankNameStr()
{
	return strVesselName;
}

bool LiquidTank::SetUpperLiquidTemperature(double newTemperature)
{
	if (upperLiquidTemperatureSensorExists)
	{
		upperLiquidTemperature = newTemperature;
		return true;
	}
	else return false;
}

double LiquidTank::GetUpperLiquidTemperature(void)
{
	return upperLiquidTemperature;
}

bool LiquidTank::SetLowerLiquidTemperature(double newTemperature)
{
	if (lowerLiquidTemperatureSensorExists)
	{
		lowerLiquidTemperature = newTemperature;
		return true;
	}
	else return false;
}

double LiquidTank::GetLowerLiquidTemperature(void)
{
	return lowerLiquidTemperature;
}

// Set speed for pump to run stopped=0.0 to maximum=1.0
bool LiquidTank::SetDistillatePumpSpeed(float newPumpSpeed)
{
	if (distillateMeteringPumpExists && ptrDistillateMeteringPump != NULL)
	{
		ptrDistillateMeteringPump->SetPumpSpeed(newPumpSpeed);
		return true;
	}
	else return false;
}

// Get speed at which pump is set to run stopped=0.0 to maximum=1.0
float LiquidTank::GetDistillatePumpSpeed(void)
{
	if (distillateMeteringPumpExists && ptrDistillateMeteringPump != NULL)
	{
		return ptrDistillateMeteringPump->GetPumpSpeed();
	}
	else return 0.0;
}

// Should be set in gallons
void LiquidTank::SetMaxVolume(float volume)
{
	maxVolume = volume;
}

// Returns gallons
float LiquidTank::GetMaxVolume(void)
{
	return maxVolume;
}

// Should be set to liters between high and low level switches
bool LiquidTank::SetVolumeSwitch(double newVolume)
{
	if (newVolume <= newVolume)
	{
		fillVolumeSwitch = newVolume;
		return true;
	}
	else return false;
}

// Returns liters between high and low level sensor set points
double LiquidTank::GetVolumeSensor(void)
{
	return fillVolumeSensor;
}

// Should be set to liters between high and low level sensor set points
bool LiquidTank::SetVolumeSensor(double newVolume)
{
	if (newVolume <= newVolume)
	{
		fillVolumeSensor = newVolume;
		return true;
	}
	else return false;
}

// Returns liters between high and low level swiches
double LiquidTank::GetVolumeSwitch(void)
{
	return fillVolumeSwitch;
}

void LiquidTank::SetUpperLiquidTemperatureSensorExists(bool exists)
{
	upperLiquidTemperatureSensorExists = exists;
}

bool LiquidTank::GetUpperLiquidTemperatureSensorExists(void)
{
	return upperLiquidTemperatureSensorExists;
}

void LiquidTank::SetLowerLiquidTemperatureSensorExists(bool exists)
{
	lowerLiquidTemperatureSensorExists = exists;
}

bool LiquidTank::GetLowerLiquidTemperatureSensorExists(void)
{
	return lowerLiquidTemperatureSensorExists;
}

void LiquidTank::SetDistillateMeteringPumpExists(bool exists)
{
	distillateMeteringPumpExists = exists;
}

bool LiquidTank::GetDistillateMeteringPumpExists(void)
{
	return distillateMeteringPumpExists;
}

void LiquidTank::SetContinuousLevelSensorExists(bool exists)
{
	continuousLevelSensorExists = exists;
}

bool LiquidTank::GetContinuousLevelSensorExists(void)
{
	return continuousLevelSensorExists;
}

LevelSensor* LiquidTank::GetTankLevelSensorPtr(void)
{
	return ptrTankLevelSensor;
}

double LiquidTank::GetTankLevel(void)
{
	return tankLevel;
}

bool LiquidTank::SetTankLevel(double newLevel)
{
	if (continuousLevelSensorExists)
	{
		tankLevel = newLevel;
		return true;
	}
	else return false;
}

void LiquidTank::SetHighAlarmExists(bool exists)
{
	highAlarmExists = exists;
}

bool LiquidTank::GetHighAlarmExists(void)
{
	return highAlarmExists;
}

bool LiquidTank::SetHighAlarmActive(bool active)
{
	if (highAlarmExists)
	{
		highAlarmActive = active;
		return true;
	}
	else return false;
}

bool LiquidTank::GetHighAlarmActive(void)
{
	return highAlarmActive;
}

bool LiquidTank::SetHighAlarmOn(short on)
{
	if (highAlarmExists && highAlarmActive)
	{
		if (on == 0 || on == 1)
		{
			highAlarmOn = on;
			return true;
		}
		else return false;
	}
	else
	{
		highAlarmOn = -1;
		return false;
	}
}

short LiquidTank::GetHighAlarmOn(void)
{
	return highAlarmOn;
}

bool LiquidTank::GetHighAlarmWavesDetected(void)
{
	try
	{
		if (ptrHighAlarmSwitch != NULL)	return ptrHighAlarmSwitch->GetWavesDetected();
		else throw ProgramException("Null pointer to high alarm switch in " +  GetTankNameStr() + "\n");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(7, "LiquidTank::GetHighAlarmWavesDetected", pe.GetExceptionDescription(), true);
		return false;
	}
}

void LiquidTank::SetLowAlarmExists(bool exists)
{
	lowAlarmExists = exists;
}

bool LiquidTank::GetLowAlarmExists(void)
{
	return lowAlarmExists;
}
	
bool LiquidTank::SetLowAlarmActive(bool active)
{
	if (lowAlarmExists)
	{
		lowAlarmActive = active;
		return true;
	}
	else return false;
}

bool LiquidTank::GetLowAlarmActive(void)
{
	return lowAlarmActive;
}
	
bool LiquidTank::SetLowAlarmOn(short on)
{
	if (lowAlarmExists && lowAlarmActive)
	{
		if (on == 0 || on == 1)
		{
			lowAlarmOn = on;
			return true;
		}
		else return false;
	}
	else
	{
		lowAlarmOn = -1;
		return false;
	}
}

short LiquidTank::GetLowAlarmOn(void)
{
	return lowAlarmOn;
}

bool LiquidTank::GetLowAlarmWavesDetected(void)
{
	try
	{
		if (ptrLowAlarmSwitch != NULL)	return ptrLowAlarmSwitch->GetWavesDetected();
		else throw ProgramException("Null pointer to low alarm switch in " +  GetTankNameStr() + "\n");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(7, "LiquidTank::GetLowAlarmWavesDetected", pe.GetExceptionDescription(), true);
		return false;
	}
}

void LiquidTank::SetHighLevelExists(bool exists)
{
	highLevelExists = exists;
}

bool LiquidTank::GetHighLevelExists(void)
{
	return highLevelExists;
}

bool LiquidTank::SetHighLevelActive(bool active)
{
	if (highLevelExists)
	{
		highLevelActive = active;
		return true;
	}
	else return false;
}

bool LiquidTank::GetHighLevelActive(void)
{
	return highLevelActive;
}
	
bool LiquidTank::SetHighLevelOn(short on)
{
	if (highLevelExists && highLevelActive)
	{
		if (on == 0 || on == 1)
		{
			highLevelOn = on;
			return true;
		}
		else return false;
	}
	else
	{
		highLevelOn = -1;
		return false;
	}
}

short LiquidTank::GetHighLevelOn(void)
{
	return highLevelOn;
}

bool LiquidTank::GetHighLevelWavesDetected(void)
{
	try
	{
		if (ptrHighLevelSwitch != NULL)	return ptrHighLevelSwitch->GetWavesDetected();
		else throw ProgramException("Null pointer to high level switch in " +  GetTankNameStr() + "\n");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(7, "LiquidTank::GetHighLevelWavesDetected", pe.GetExceptionDescription(), true);
		return false;
	}
}

void LiquidTank::SetLowLevelExists(bool exists)
{
	lowLevelExists = exists;
}

bool LiquidTank::GetLowLevelExists(void)
{
	return lowLevelExists;
}
	
bool LiquidTank::SetLowLevelActive(bool active)
{
	if (lowLevelExists)
	{
		lowLevelActive = active;
		return true;
	}
	else return false;
}

bool LiquidTank::GetLowLevelActive(void)
{
	return lowLevelActive;
}

bool LiquidTank::SetLowLevelOn(short on)
{
	if (lowLevelExists && lowLevelActive)
	{
		if (on == 0 || on == 1)
		{
			lowLevelOn = on;
			return true;
		}
		else return false;
	}
	else
	{
		lowLevelOn = -1;
		return false;
	}
}

short LiquidTank::GetLowLevelOn(void)
{
	return lowLevelOn;
}

bool LiquidTank::GetLowLevelWavesDetected(void)
{
	try
	{
		if (ptrLowLevelSwitch != NULL)	return ptrLowLevelSwitch->GetWavesDetected();
		else throw ProgramException("Null pointer to low level switch in " +  GetTankNameStr() + "\n");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(7, "LiquidTank::GetLowLevelWavesDetected", pe.GetExceptionDescription(), true);
		return false;
	}
}

bool LiquidTank::GetDraining(void)
{
	return draining;
}

bool LiquidTank::GetFilling(void)
{
	return filling;
}

bool LiquidTank::GetCleaning(void)
{
	return cleaning;
}

double LiquidTank::GetDrainStartTime(void)
{
	return drainStartTime;
}

double LiquidTank::GetLastDrainTime(void)
{
	return lastDrainTime;
}

double LiquidTank::GetFillStartTime(void)
{
	return fillStartTime;
}

double LiquidTank::GetLastFillTime(void)
{
	return lastFillTime;
}

// Returns drain rate in LPM
double LiquidTank::GetLastDrainRate(void)
{
	if (lastDrainTime <= 0.0) return 0.0;
	return GetVolumeSwitch()/(lastDrainTime/60.0);
}

// Returns fill rate in LPM
double LiquidTank::GetLastFillRate(void)
{
	if (lastFillTime <= 0.0) return 0.0;
	return GetVolumeSwitch()/(lastFillTime/60.0);
}


// Liquid Tank rate method for calculations
void LiquidTank::SetMaxFillRateBufferSize(unsigned int size)
{
	maxFillRateBufferSize = size;
}

// Liquid Tank rate method for calculations
bool LiquidTank::ResizeFillRateBuffer(unsigned int newSize)
{
	if (newSize <= maxFillRateBufferSize)
	{
		storedFillRateBuffer.resize(newSize);
		if (storedFillRateBuffer.size() == newSize) return true;
		else return false;
	}
	else return false;
}

// Liquid Tank rate method for calculations
unsigned int LiquidTank::GetFillRateBufferSize(void)
{
	return storedFillRateBuffer.size();
}

// Liquid Tank rate method for calculations
double LiquidTank::GetStoredFillRate(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).rate;
	else return 0.0;
}

// Liquid Tank rate method for calculations
double LiquidTank::GetStoredFillVolume(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).volume;
	else return 0.0;
}

// Liquid Tank rate method for calculations
double LiquidTank::GetStoredLevelEquivalent(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).levelEquivalent;
	else return 0.0;
}

// Liquid Tank rate method for calculations
int LiquidTank::GetFillRateBufferYear(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).year;
	else return -1;
}

// Liquid Tank rate method for calculations
int LiquidTank::GetFillRateBufferMonth(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).month;
	else return -1;
}

// Liquid Tank rate method for calculations
int LiquidTank::GetFillRateBufferDay(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).day;
	else return -1;
}

// Liquid Tank rate method for calculations
int LiquidTank::GetFillRateBufferHour(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).hour;
	else return -1;
}

// Liquid Tank rate method for calculations
int LiquidTank::GetFillRateBufferMinute(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).minute;
	else return -1;
}

// Liquid Tank rate method for calculations
int LiquidTank::GetFillRateBufferSecond(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).second;
	else return -1;
}

// Liquid Tank rate method for calculations
int LiquidTank::GetFillRateBufferMillisecond(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).millisecond;
	else return -1;
}

// Liquid Tank rate method for calculations
double LiquidTank::GetFillRateBufferTimeInSeconds(unsigned int i)
{
	if (storedFillRateBuffer.size() >= i) return storedFillRateBuffer.at(i).systemTimeInSeconds;
	else return -1.0;
}


// Tank fill volume in last 24 hour period from now (returns value in liters)
double LiquidTank::GetLast24HourTankFill(void)
{
	double fillTotal = 0.0;
	double timeNow = ptrSystemTime->GetSystemTimeInSeconds();
	double timeOneDayAgo = ptrSystemTime->GetSystemTimeInSeconds() - 86400.0;
	double rateBufferTime = timeNow;
	while (lockFillRateBuffer) ptrSystemTime->Wait(1);
	if (storedFillRateBuffer.size() > 0)
	{
		while (lockFillRateBuffer) ptrSystemTime->Wait(1);
		for (unsigned int i = 0; i < storedFillRateBuffer.size(); i++)
		{
			while (lockFillRateBuffer) ptrSystemTime->Wait(1);
			rateBufferTime = storedFillRateBuffer.at(i).systemTimeInSeconds;
			if (rateBufferTime < timeOneDayAgo) break;
			fillTotal = fillTotal + storedFillRateBuffer.at(i).volume;
		}
		fillTotal = ptrMathUtilities->RoundOff(fillTotal, 1);
		return fillTotal;
	}
	else return 0.0;
}

// Tank fill in 24 hour period prior to time/date specified (returns value in liters)
double LiquidTank::GetAny24HourTankFill(int year, int month, int day, int hour)
{
	double fillTotal = 0.0;
	unsigned int startIndex;
	try
	{
		if (storedFillRateBuffer.size() > 0)
		{
			// Check date and time input values for reasonable range
			if (year < 2015 || year > storedFillRateBuffer.front().year) throw ProgramException("Year value out of range");
			if (month < 1 || month > 12) throw ProgramException("Month value out of range");
			if (day < 1 || day > 31) throw ProgramException("Day value out of range");
			if (hour < 0 || hour > 24) throw ProgramException("Hour value out of range");
			
			while (lockRateBuffer) ptrSystemTime->Wait(1);
			for (unsigned int i = 0; i < storedFillRateBuffer.size(); i++)
			{
				if (storedFillRateBuffer.at(i).year <= year && storedFillRateBuffer.at(i).month <= month && storedFillRateBuffer.at(i).day <= day && storedFillRateBuffer.at(i).hour <= hour)
				{
					startIndex = i;
				}
				else break;
			}

			double timeStart = storedFillRateBuffer.at(startIndex).systemTimeInSeconds;
			double timeOneDayEarlier = timeStart - 86400.0;
			double rateBufferTime = timeStart;
			for (unsigned int i = startIndex; i < storedFillRateBuffer.size() - startIndex - 1; i++)
			{
				rateBufferTime = storedFillRateBuffer.at(i).systemTimeInSeconds;
				if (rateBufferTime < timeOneDayEarlier) break;
				fillTotal = fillTotal + storedFillRateBuffer.at(i).volume;
			}
			fillTotal = ptrMathUtilities->RoundOff(fillTotal, 1);
			return fillTotal;
		}
		else return 0.0;
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "LiquidTank::GetAny24HourTankFill() ", pe.GetExceptionDescription(), true);
		return 0.0;
	}
}

bool LiquidTank::StoreFillRate(double lastFillTime, bool proportional)
{
	if (lastFillTime < 0.0) return false;

	// Store rate change time point data
	storedFillRate thisRate;
	if (proportional)
	{
		thisRate.volume = fillVolumeSensor;
		thisRate.rate = fillVolumeSensor/(lastFillTime/60.0);
		thisRate.levelEquivalent = GetLevelEquivalent(fillVolumeSensor);
	}
	else
	{
		thisRate.volume = fillVolumeSwitch;
		thisRate.rate = fillVolumeSwitch/(lastFillTime/60.0);
		thisRate.levelEquivalent = GetLevelEquivalent(fillVolumeSwitch);
	}

	if (storedFillRateBuffer.size() > 0)
	{
		thisRate.secondsSinceLastRate = ptrSystemTime->GetSystemTimeInSeconds() - GetFillRateBufferTimeInSeconds(0);
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
	// Wait for display draw to finish if in progress
	while (lockForFillRateDisplay) ptrSystemTime->Wait(1);
	// Store current value in buffer
	lockFillRateBuffer = true;
	storedFillRateBuffer.push_front(thisRate);
	// Drop oldest value if buffer is full
	if (storedFillRateBuffer.size() > maxFillRateBufferSize) storedFillRateBuffer.pop_back();
	lockFillRateBuffer = false;
	return true;
}

// Returns change in tank level equivalent to volume change argument (in liters) at normal level in millimeters
double LiquidTank::GetLevelEquivalent(double volumeChange)
{
	if (ptrMathUtilities != NULL) return ptrMathUtilities->RoundOff(volumeChange/surfaceArea, 0);
	else return 0.0;
}

// Updates state of level switches and sensor
bool LiquidTank::UpdateLevelState(void)
{
	// Update brine level
	if (continuousLevelSensorExists)
	{
		try
		{
			if (ptrTankLevelSensor != NULL)
			{
				if (ptrTankLevelSensor->UpdateSensorValueSmoothed())
				{
					tankLevel = ptrTankLevelSensor->GetSensorValue();
				}
				else
				{
					throw ProgramException("UpdateSensorValueSmoothed() failed for continuous level sensor on " +  GetTankNameStr() + "\n");
				}
			}
			else
			{
				throw ProgramException("Null pointer to continuous level sensor on " +  GetTankNameStr() + "\n");
			}
		}
		catch (ProgramException& pe)
		{
			ptrProgramExceptions->SetProgramException(1, "LiquidTank::UpdateLevelState", pe.GetExceptionDescription(), true);
		}
	}
	if (lowAlarmExists)
	{
		try
		{
			if (ptrLowAlarmSwitch != NULL)
			{
				if (ptrLowAlarmSwitch->UpdateStatus()) SetLowAlarmOn(ptrLowAlarmSwitch->GetStatus());
				else
				{
					throw ProgramException("UpdateStatus() failed for low alarm switch on " +  GetTankNameStr() + "\n");
				}
			}
			else
			{
				throw ProgramException("Null pointer to low alarm switch on " +  GetTankNameStr() + "\n");
			}
		}
		catch (ProgramException& pe)
		{
			ptrProgramExceptions->SetProgramException(2, "LiquidTank::UpdateLevelState", pe.GetExceptionDescription(), true);
		}
	}
	if (lowLevelExists)
	{
		try
		{
			if (ptrLowLevelSwitch != NULL)
			{
				if (ptrLowLevelSwitch->UpdateStatus()) SetLowLevelOn(ptrLowLevelSwitch->GetStatus());
				else
				{
					throw ProgramException("UpdateStatus() failed for low level switch on " +  GetTankNameStr() + "\n");
				}
			}
			else
			{
				throw ProgramException("Null pointer to low level switch on " +  GetTankNameStr() + "\n");
			}
		}
		catch (ProgramException& pe)
		{
			ptrProgramExceptions->SetProgramException(3, "LiquidTank::UpdateLevelState", pe.GetExceptionDescription(), true);
		}
	}
	if (highLevelExists)
	{
		try
		{
			if (ptrHighLevelSwitch != NULL)
			{
				if (ptrHighLevelSwitch->UpdateStatus()) SetHighLevelOn(ptrHighLevelSwitch->GetStatus());
				else
				{
					throw ProgramException("UpdateStatus() failed for high level switch on " +  GetTankNameStr() + "\n");
				}
			}
			else
			{
				throw ProgramException("Null pointer to high level switch on " +  GetTankNameStr() + "\n");
			}
		}
		catch (ProgramException& pe)
		{
			ptrProgramExceptions->SetProgramException(4, "LiquidTank::UpdateLevelState", pe.GetExceptionDescription(), true);
		}
	}
	if (highAlarmExists)
	{
		try
		{
			if (ptrHighAlarmSwitch != NULL)
			{
				if (ptrHighAlarmSwitch->UpdateStatus()) SetHighAlarmOn(ptrHighAlarmSwitch->GetStatus());
				else
				{
					throw ProgramException("UpdateStatus() failed for high alarm switch on " +  GetTankNameStr() + "\n");
				}
			}
			else
			{
				throw ProgramException("Null pointer to high alarm switch on " +  GetTankNameStr() + "\n");
			}
		}
		catch (ProgramException& pe)
		{
			ptrProgramExceptions->SetProgramException(5, "LiquidTank::UpdateLevelState", pe.GetExceptionDescription(), true);
		}
	}
	return true;
}

bool LiquidTank::TankControlLogic(void)
{
	float mAmpSetting = 4.0;

	// Update level switch status
	UpdateLevelState();
	GetLowLevelWavesDetected();
	GetHighLevelWavesDetected();

	// Run proportional sensor level control first, set process error on failure
	if (!TankControlProportional())
	{
		continuousLevelSensorControlling = false;
		// Set process error
		SetProcessError(6, GetTankNameStr(), "Continuous level sensor out of range or not operating OK", true);
	}
	else
	{
		continuousLevelSensorControlling = true;
		// clear process error
		SetProcessError(6, GetTankNameStr(), "Continuous level sensor out of range or not operating OK", false);
	}

	// Run level switch control logic to back up continuous level sensor
	// Check fill source and cleaning status, bypass logic if either interfere with normal operation
	try
	{
		// Overide pump control logic if distillate tank is low, if tank or switches are being cleaned, or for manual pump speed override
		// If distillate source is not available, stop pump
		if (!fillSourceAvailable)
		{
			// Check for manual override or automatic control
			if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
			{
				if (ptrDistillateMeteringPump->GetPumpSpeed() != 0.0)
				{
					// Stop pump
					SetProcessError(3, GetTankNameStr(), "Low distillate supply tank level, stopping pump during auto fill", true);
					if (ptrDistillateMeteringPump->SetPumpSpeed(0.0f)) return true;
					else
					{
						// Pump setting failure
						throw ProgramException("Cannot Stop Metering Pump on " +  GetTankNameStr());
						return false;
					}
				}
				// Pump is already stopped
				else return true;
			}
			else // Stop pump during manual override
			{
				if (ptrDistillateMeteringPump->GetPumpSpeed() != 0.0)
				{
					// Stop pump in spite of manual override due to low distillate supply tank
					SetProcessError(3, GetTankNameStr(), "Low distillate supply tank level, stopping pump during manual override", true);
					if (ptrDistillateMeteringPump->SetPumpSpeed(0.0f)) return true;
					else
					{
						// Pump setting failure
						throw ProgramException("Cannot Stop Metering Pump on " +  GetTankNameStr());
						return false;
					}
				}
				// Pump is already stopped
				else return true;
			}
		}
		// If tank or sensors are being cleaned, stop pump under automatic control while cleaning
		else if (cleaning)
		{
			// Check for manual override or automatic control
			if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
			{
				if (ptrDistillateMeteringPump->GetPumpSpeed() != 0.0)
				{
					// Stop pump
					SetProcessError(3, GetTankNameStr(), "Cleaning tank or sensors, stopping pump during auto fill", true);
					if (ptrDistillateMeteringPump->SetPumpSpeed(0.0f)) return true;
					else
					{
						// Pump setting failure
						throw ProgramException("Cannot Stop Metering Pump on " +  GetTankNameStr());
						return false;
					}
				}
				// Pump is already stopped, clear process error if set
				else
				{
					SetProcessError(3, GetTankNameStr(), "Cleaning tank or sensors, stopping pump during auto fill", false);
					return true;
				}
			}
			// Manual override, no automatic change in pump speed
			else return true;
		}
		// Distillate supply tank level is normal and not cleaning tank or sensors, clear errors and continue
		else
		{
			// Clear process error if previously set
			SetProcessError(3, GetTankNameStr(), "", false);
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(6, "LiquidTank::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	// Test for error: simultaneous hi and low level switches on condition
	if (GetHighLevelExists() && GetLowLevelExists())
	{
		try
		{
			if (GetHighLevelActive() && GetLowLevelActive())
			{
				// If both high level switch and low level switch are simultneously on and neither reading is suspect due to wave action
				if (GetHighLevelOn() == 0 && !GetHighLevelWavesDetected() && GetLowLevelOn() == 0 && !GetLowLevelWavesDetected())
				{
					// Set proceess error
					SetProcessError(7, GetTankNameStr(), "Error: both level switches triggered, check connections", true);
					// Set level switch operational status
					if (ptrLowLevelSwitch != NULL) ptrLowLevelSwitch->SetSensorOperatingOK(false);
					if (ptrHighLevelSwitch != NULL) ptrHighLevelSwitch->SetSensorOperatingOK(false);
				}
				else
				{
					// Clear process error
					SetProcessError(7, GetTankNameStr(), "Error: both level switches triggered, check connections", false);
					if (ptrLowLevelSwitch != NULL) ptrLowLevelSwitch->SetSensorOperatingOK(true);
					if (ptrHighLevelSwitch != NULL) ptrHighLevelSwitch->SetSensorOperatingOK(true);
				}
			}
			else
			{
				// Low and high level exist but one or both are inactive
				throw ProgramException("Low and-or high level level switch, not active for " +  GetTankNameStr());
				return false;
			}
		}
		catch (ProgramException& pe)
		{
			ptrProgramExceptions->SetProgramException(8, "LiquidTank::TankControlLogic", pe.GetExceptionDescription(), true);
		}
	}
	// High or low level switch do not exist, continue

	// Check low level first, start pump if below
	if (GetLowLevelExists())
	{
		try
		{
			// Check operation status of low level of switch
			if (GetLowLevelActive() && ptrLowLevelSwitch != NULL && ptrLowLevelSwitch->GetSensorOperatingOK())
			{
				// Clear process error if set
				SetProcessError(9, GetTankNameStr(), "Low level switch, not operating OK", false);

				// If tank is below low level and reading is not suspect due to wave action
				if (GetLowLevelOn() == 0 && !GetLowLevelWavesDetected())
				{
					// Clear process error if set
					SetProcessError(1, GetTankNameStr(), "below low level reading suspect due to wave action", false);
				
					// Just reached low level, reset timers, start metering pump		
					if (draining)
					{
						// Skip switch control if continuous level sensor control is working
						if (continuousLevelSensorControlling) return true;

						// STOP DRAIN TIMER, RECORD DRAIN TIME
						lastDrainTime = ptrSystemTime->GetSystemTimeInSeconds() - drainStartTime;
						drainStartTime = 0.0;
						// SET FILL TIMER
						fillStartTime = ptrSystemTime->GetSystemTimeInSeconds();
						draining = false;
						filling = true;
						// Continue to high level test
					}
					else if (filling)
					{
						draining = false;
						
						if (GetHighLevelOn() == 0)
						{
							// Display process error and continue to high level test
							SetProcessError(8, GetTankNameStr(), "Both high and low level triggered, check for debris", true);
						}
						else
						{
							// Clear process error and continue to high level test
							SetProcessError(8, GetTankNameStr(), "Both high and low level triggered, check for debris", false);
						}
					}
				}
				// If tank is below low level, but reading is suspect due to wave action
				else if (GetLowLevelOn() == 0 && GetLowLevelWavesDetected())
				{
					// Display process error and continue to high level test
					SetProcessError(1, GetTankNameStr(), "below low level reading suspect due to wave action", true);
				}
				else if (GetLowLevelOn() != 1 && GetLowLevelOn() != 0)
				{
					// Low level value returned is undefined
					throw ProgramException("Low level value is undefined for " +  GetTankNameStr());
					return false;
				}
				// Low level normal, continue
				else
				{
					// Clear process error if set and continue to high level test
					SetProcessError(1, GetTankNameStr(), "below low level reading suspect due to wave action", false);
				}
			}
			else
			{
				// Low level exists but is inactive or non operational
				SetProcessError(9, GetTankNameStr(), "Low level switch, not operating OK", true);
			}
		}
		catch (ProgramException& pe)
		{
			ptrProgramExceptions->SetProgramException(7, "LiquidTank::TankControlLogic", pe.GetExceptionDescription(), true);
		}
	// No Low level switch, continue	
	}
					
	// Check high level
	if (GetHighLevelExists())
	{
		try
		{
			if (GetHighLevelActive() && ptrHighLevelSwitch != NULL && ptrHighLevelSwitch->GetSensorOperatingOK())
			{
				// If tank is above high level and reading is not suspect due to wave action
				if (GetHighLevelOn() == 0 && !GetHighLevelWavesDetected())
				{
					// Clear process error if set
					SetProcessError(2, GetTankNameStr(), "", false);

					// Skip switch control if continuous level sensor control is working
					if (continuousLevelSensorControlling) return true;

					// Reset timers
					if (filling)
					{
						// STOP FILL TIMER, RECORD FILL TIME
						lastFillTime = ptrSystemTime->GetSystemTimeInSeconds() - fillStartTime;
						fillStartTime = 0.0;
						// SET DRAIN TIMER
						drainStartTime = ptrSystemTime->GetSystemTimeInSeconds();

						// Store fill rate in display buffer
						if (!StoreFillRate(lastFillTime, false)) throw ProgramException("StoreFillRate failed on " +  GetTankNameStr());

						// Log fill time
						SetVolumeSwitch(fillVolumeSwitch);
						ptrTankFillLogFileOutput->SetDataItem(1, GetTankNameStr() + " fill time", lastFillTime);
						ptrTankFillLogFileOutput->SetDataItem(2, GetTankNameStr() + " fill rate", GetStoredFillRate(0));
						ptrTankFillLogFileOutput->SetDataItem(3, GetTankNameStr() + " last 24hr volume", GetLast24HourTankFill());
						ptrTankFillLogFileOutput->SetDataItem(4, GetTankNameStr() + " last 24hr level", GetLevelEquivalent(GetLast24HourTankFill()));
						if (!ptrTankFillLogFileOutput->GetColumnLabelsWritten()) ptrTankFillLogFileOutput->WriteColumnLabels();
						ptrTankFillLogFileOutput->WriteCurrentData();
						draining = true;
						filling = false;
						// Check for manual override
						if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
						{
							if (ptrDistillateMeteringPump->SetPumpSpeed(0.0)) return true;
							else
							{
								// Pump setting failure
								throw ProgramException("Cannot Set Metering Pump on " +  GetTankNameStr());
								return false;
							}
						}
						// Under manual override
						else return true;
					}
					// Tank is not filling
					else return true;
				}
				// If tank is below high level and reading is not suspect due to wave action
				else if (GetHighLevelOn() == 1 && !GetHighLevelWavesDetected())
				{
					// Clear process error if set
					SetProcessError(2, GetTankNameStr(), "Above high level reading suspect due to wave action", false);

					// High level normal, control pump speed
					if (filling)
					{
						// Skip switch control if continuous level sensor control is working
						if (continuousLevelSensorControlling) return true;
						draining = false;

						// Ramp up pump speed
						double elapsedFillTime = ptrSystemTime->GetSystemTimeInSeconds() - fillStartTime;
						// Update elapsedFillTimeMinutes every 10 minutes
						float elapsedFillTimeMinutes = (float) RoundOff(elapsedFillTime/60.0, 0);
						float newPumpSpeed = 0.1f + 0.1f * elapsedFillTimeMinutes;
						if (newPumpSpeed > 1.0f) newPumpSpeed = 1.0f;
						// Skip setting pump speed if unchanged
						if (newPumpSpeed == ptrDistillateMeteringPump->GetPumpSpeed()) return true;
						// Check for manual override
						if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
						{
							if (ptrDistillateMeteringPump->SetPumpSpeed(newPumpSpeed)) return true;
							else
							{
								// Pump setting failure
								throw ProgramException("Cannot Set Metering Pump on " +  GetTankNameStr());
								return false;
							}
						}
						// Under manual override
						else return true;
					}
					else if (draining)
					{
						// Set pump to stop
						if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
						{
							if (ptrDistillateMeteringPump->GetPumpSpeed() != 0.0)
							{
								if (ptrDistillateMeteringPump->SetPumpSpeed(0.0f)) return true;
								else
								{
									// Pump setting failure
									throw ProgramException("Cannot Stop Metering Pump on " +  GetTankNameStr());
									return false;
								}
							}
							else return true;
						}
						else return true;
					}
				}
				// If tank is below high level, but reading is suspect due to wave action
				else if (GetHighLevelOn() == 1 && GetHighLevelWavesDetected())
				{
					// Set process error
					SetProcessError(2, GetTankNameStr(), "Below high level reading suspect due to wave action", true);

					// High level suspect, retain pump speed
					if (filling)
					{
						// Skip switch control if continuous level sensor control is working
						if (continuousLevelSensorControlling) return true;
						draining = false;

						// Keep steady pump speed
						float newPumpSpeed = ptrDistillateMeteringPump->GetPumpSpeed();
						if (newPumpSpeed > 1.0f) newPumpSpeed = 1.0f;
						if (newPumpSpeed < 0.0f) newPumpSpeed = 0.0f;
						// Skip setting pump speed if unchanged
						if (newPumpSpeed == ptrDistillateMeteringPump->GetPumpSpeed()) return true;
						if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
						{
							if (ptrDistillateMeteringPump->SetPumpSpeed(newPumpSpeed)) return true;
							else
							{
								// Pump setting failure
								throw ProgramException("Cannot Set Metering Pump on " +  GetTankNameStr());
								return false;
							}
						}
						else return true;
					}
					else if (draining)
					{
						// Skip switch control if continuous level sensor control is working
						if (continuousLevelSensorControlling) return true;

						// Set pump to stop if not under manual override
						if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
						{
							if (ptrDistillateMeteringPump->GetPumpSpeed() != 0.0)
							{
								if (ptrDistillateMeteringPump->SetPumpSpeed(0.0f))
								{
									filling = false;
									return true;
								}
								else
								{
									// Pump setting failure
									throw ProgramException("Cannot Set Metering Pump on " +  GetTankNameStr());
									return false;
								}
							}
							else return true;
						}
						// Under manual override
						else return true;
					}
				}
				// If tank is above high level, but reading is suspect due to wave action
				else if (GetHighLevelOn() == 0 && GetHighLevelWavesDetected())
				{
					// Set process error
					SetProcessError(2, GetTankNameStr(), "Above high level reading suspect due to wave action", true);

					// Suspect high level reading, reduce pump speed
					if (filling)
					{
						// Skip switch control if continuous level sensor control is working
						if (continuousLevelSensorControlling) return true;
						draining = false;

						// Ramp down pump speed
						double elapsedFillTime = ptrSystemTime->GetSystemTimeInSeconds() - fillStartTime;
						// Update elapsedFillTimeMinutes every 10 minutes
						float elapsedFillTimeMinutes = (float) RoundOff(elapsedFillTime/60.0, 0);
						float newPumpSpeed = 0.1f - 0.1f * elapsedFillTimeMinutes;
						if (newPumpSpeed < 0.0f) newPumpSpeed = 0.0f;
						// Skip setting pump speed if unchanged
						if (newPumpSpeed == ptrDistillateMeteringPump->GetPumpSpeed()) return true;
						if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
						{
							if (ptrDistillateMeteringPump->SetPumpSpeed(newPumpSpeed)) return true;
							else
							{
								// Pump setting failure
								throw ProgramException("Cannot Set Metering Pump on " +  GetTankNameStr());
								return false;
							}
						}
						else return true;
					}
					else if (draining)
					{
						// Skip switch control if continuous level sensor control is working
						if (continuousLevelSensorControlling) return true;

						// Set pump to stop
						if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
						{
							if (ptrDistillateMeteringPump->GetPumpSpeed() != 0.0)
							{
								if (ptrDistillateMeteringPump->SetPumpSpeed(0.0f))
								{
									filling = false;
									return true;
								}
								else
								{
									// Pump setting failure
									throw ProgramException("Cannot Set Metering Pump on " +  GetTankNameStr());
									return false;
								}
							}
							else return true;
						}
						else return true;
					}
				}
				else if (GetHighLevelOn() == -1)
				{
					// High level value has not been set
					throw ProgramException("High level value failed to set for " +  GetTankNameStr());
					return false;
				}
				else if (GetHighLevelOn() != 1 && GetHighLevelOn() != 0)
				{
					// High level value returned is undefined
					throw ProgramException("High level value is undefined for " +  GetTankNameStr());
					return false;
				}
				else return true;
			}
			else
			{
				// High level exists but is inactive or non operational
				throw ProgramException("High level, not active or not operating for " +  GetTankNameStr());
				return false;
			}
		}
		catch (ProgramException& pe)
		{
			ptrProgramExceptions->SetProgramException(8, "LiquidTank::TankControlLogic", pe.GetExceptionDescription(), true);
		}
		return false;
	}
	// No High level switch, return	
	else return true;
}

bool LiquidTank::TankControlProportional(void)
{
	if (continuousLevelSensorExists)
	{
		try
		{
			if (ptrTankLevelSensor != NULL)
			{
				if (ptrTankLevelSensor->GetSensorOperatingOK())
				{
					// Check tank and sensor cleaning status, if cleaning, skip all sensor logic
					if (cleaning) return true;

					// Check that level is within operating range, if not pass control to level switches in calling routine
					// High side range check
					if (ptrTankLevelSensor->GetSensorValue() < ptrTankLevelSensor->GetLevelExceedsHighRange()) SetProcessError(4, GetTankNameStr(), "Level sensor above range, pass control to switches", false); // clear process error
					else
					{
						// Set above range process error
						SetProcessError(4, GetTankNameStr(), "Level sensor above range, pass control to switches", true);
						return false;
					}
					// Low side range check
					if (ptrTankLevelSensor->GetSensorValue() > ptrTankLevelSensor->GetLevelExceedsLowRange()) SetProcessError(5, GetTankNameStr(), "Level sensor below range, pass control to switches", false); // clear process error
					else
					{
						// Set below range process error
						SetProcessError(5, GetTankNameStr(), "Level sensor below range, pass control to switches", true);
						return false;
					}
					// Overide pump control logic if distillate tank is low or for manual pump speed override
					// If distillate source is not available, stop pump
					if (!fillSourceAvailable)
					{
						// Check for manual override or automatic control
						if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
						{
							if (ptrDistillateMeteringPump->GetPumpSpeed() != 0.0)
							{
								// Stop pump
								SetProcessError(3, GetTankNameStr(), "Low distillate supply tank level, stopping pump during auto fill", true);
								if (ptrDistillateMeteringPump->SetPumpSpeed(0.0f)) return true;
								else
								{
									// Pump setting failure
									throw ProgramException("Cannot Stop Metering Pump on " +  GetTankNameStr());
									return false;
								}
							}
							// Pump is already stopped
							else return true;
						}
						else // Stop pump during manual override
						{
							if (ptrDistillateMeteringPump->GetPumpSpeed() != 0.0)
							{
								// Stop pump in spite of manual override due to low distillate supply tank
								SetProcessError(3, GetTankNameStr(), "Low distillate supply tank level, stopping pump during manual override", true);
								if (ptrDistillateMeteringPump->SetPumpSpeed(0.0f)) return true;
								else
								{
									// Pump setting failure
									throw ProgramException("Cannot Stop Metering Pump on " +  GetTankNameStr());
									return false;
								}
							}
							// Pump is already stopped
							else return true;
						}
					}
					// Distillate supply tank level is normal, clear errors and continue
					else
					{
						// Clear process error if previously set
						SetProcessError(3, GetTankNameStr(), "", false);
					}

					// Run if reading is not suspect due to wave action
					if ((GetLowLevelExists() && GetLowLevelActive() && !GetLowLevelWavesDetected()) && (GetHighLevelExists() && GetHighLevelActive() && !GetHighLevelWavesDetected()))
					{
						// If tank is below low level
						if (ptrTankLevelSensor->GetSensorValue() < ptrTankLevelSensor->GetLevelLowRange())
						{
							// Clear process error if set
							SetProcessError(1, GetTankNameStr(), "below low level reading suspect due to wave action", false);

							// Just reached low level, reset timers, start metering pump		
							if (draining)
							{
								// STOP DRAIN TIMER, RECORD DRAIN TIME
								lastDrainTime = ptrSystemTime->GetSystemTimeInSeconds() - drainStartTime;
								drainStartTime = 0.0;
								// SET FILL TIMER
								fillStartTime = ptrSystemTime->GetSystemTimeInSeconds();
								draining = false;
								filling = true;
							}

							// Ramp up pump speed
							double elapsedFillTime = ptrSystemTime->GetSystemTimeInSeconds() - fillStartTime;
							float elapsedFillTimeMinutes = (float) RoundOff(elapsedFillTime/60.0, 0);
							float newPumpSpeed = 0.1f + 0.1f * elapsedFillTimeMinutes;
							if (newPumpSpeed > 1.0f) newPumpSpeed = 1.0f;
							// Skip setting pump speed if unchanged
							if (newPumpSpeed == ptrDistillateMeteringPump->GetPumpSpeed()) return true;
							// Check for manual override, set speed if under automatic control
							if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
							{
								if (ptrDistillateMeteringPump->SetPumpSpeed(newPumpSpeed)) return true;
								else
								{
									// Pump setting failure
									throw ProgramException("Cannot Set Metering Pump on " +  GetTankNameStr());
									return false;
								}
							}
							// Under manual override
							else return true;
						}
						// If tank is between low range level and high range level
						if (ptrTankLevelSensor->GetSensorValue() >= ptrTankLevelSensor->GetLevelLowRange() && ptrTankLevelSensor->GetSensorValue() <= ptrTankLevelSensor->GetLevelHighRange())
						{
							// Clear process error if set
							SetProcessError(1, GetTankNameStr(), "below low level reading suspect due to wave action", false);

							if (filling)
							{
								// Ramp up pump speed
								double elapsedFillTime = ptrSystemTime->GetSystemTimeInSeconds() - fillStartTime;
								float elapsedFillTimeMinutes = (float) RoundOff(elapsedFillTime/60.0, 0);
								float newPumpSpeed = 0.1f + 0.1f * elapsedFillTimeMinutes;
								if (newPumpSpeed > 1.0f) newPumpSpeed = 1.0f;
								// Skip setting pump speed if unchanged
								if (newPumpSpeed == ptrDistillateMeteringPump->GetPumpSpeed()) return true;
								// Check for manual override, set speed if under automatic control
								if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
								{
									if (ptrDistillateMeteringPump->SetPumpSpeed(newPumpSpeed)) return true;
									else
									{
										// Pump setting failure
										throw ProgramException("Cannot Set Metering Pump on " +  GetTankNameStr());
										return false;
									}
								}
								// Under manual override
								else return true;
							}
							// Tank is draining (evaporating) and within target range, no action
							else return true;
						}
						// If tank is above high level
						if (ptrTankLevelSensor->GetSensorValue() > ptrTankLevelSensor->GetLevelHighRange())
						{
							// Clear process error if set
							SetProcessError(2, GetTankNameStr(), "", false);

							// Reset timers
							if (filling)
							{
								// STOP FILL TIMER, RECORD FILL TIME
								lastFillTime = ptrSystemTime->GetSystemTimeInSeconds() - fillStartTime;
								fillStartTime = 0.0;
								// SET DRAIN TIMER
								drainStartTime = ptrSystemTime->GetSystemTimeInSeconds();

								// Store fill rate in display buffer
								if (!StoreFillRate(lastFillTime, true)) throw ProgramException("StoreFillRate failed on " +  GetTankNameStr());

								// Log fill time
								SetVolumeSensor(fillVolumeSensor);
								ptrTankFillLogFileOutput->SetDataItem(1, GetTankNameStr() + " fill time", lastFillTime);
								ptrTankFillLogFileOutput->SetDataItem(2, GetTankNameStr() + " fill rate", GetStoredFillRate(0));
								ptrTankFillLogFileOutput->SetDataItem(3, GetTankNameStr() + " last 24hr volume", GetLast24HourTankFill());
								ptrTankFillLogFileOutput->SetDataItem(4, GetTankNameStr() + " last 24hr level", GetLevelEquivalent(GetLast24HourTankFill()));
								if (!ptrTankFillLogFileOutput->GetColumnLabelsWritten()) ptrTankFillLogFileOutput->WriteColumnLabels();
								ptrTankFillLogFileOutput->WriteCurrentData();
								draining = true;
								filling = false;
								// Check for manual override, set speed if under automatic control
								if (ptrDistillateMeteringPump->GetPumpSpeedAuto())
								{
									if (ptrDistillateMeteringPump->SetPumpSpeed(0.0)) return true;
									else
									{
										// Pump setting failure
										throw ProgramException("Cannot Set Metering Pump on " +  GetTankNameStr());
										return false;
									}
								}
								// Under manual override
								else return true;
							}
							// Tank is draining (evaporating) and above target range, no action 
							else return true;
						}
						else // Tank level is not defined by sensor range
						{
							// Throw exception
							throw ProgramException("Undefined proportional level sensor reading on " +  GetTankNameStr());
							return false;
						}
					}
					else // Reading is suspect due to wave action
					{
						// Set process error
						SetProcessError(4, GetTankNameStr(), "Suspect reading due to wave action", true);
						return false;
					}
				}
				else // Proportional level sensor not operating OK
				{
					// Set process error
					SetProcessError(4, GetTankNameStr(), "Continuous level sensor not operating OK", true);
					return false;
				}
			}
			else
			{
				throw ProgramException("Null pointer to continuous level sensor on " +  GetTankNameStr());
				return false;
			}
		}
		catch (ProgramException& pe)
		{
			ptrProgramExceptions->SetProgramException(9, "LiquidTank::LevelControlProportional", pe.GetExceptionDescription(), true);
			return false;
		}
	}
	// No proportional level sensor
	else return false;
}

void LiquidTank::DrawFillDataBox2D(GLint nFontList, int xPos, int yPos, int xOrigin, int yOrigin, int fillOrDrain)
{
	int numberLines = 1;
	int maxLineLength = 0;

	std::ostringstream strstrFillValue;
	std::string strFillValue;
	std::string strFillUnits = " liters";
	std::string strSystemDisplayName = strVesselName;

	std::string strFillDisplayName = "";

	if (fillOrDrain == 0)
	{
		lockForFillRateDisplay = true;
		strFillDisplayName = "Last 24 hr Fill = ";
		strstrFillValue << strFillDisplayName << " " << GetLast24HourTankFill() << " " << strFillUnits <<  " = " << GetLevelEquivalent(GetLast24HourTankFill()) << " mm";
		lockForFillRateDisplay = false;
	}
	else if (fillOrDrain == 1)
	{
		strFillDisplayName = "Drain Rate = ";
		strstrFillValue << strFillDisplayName << " " << GetLastDrainRate() << " " << strFillUnits;
	}
	strFillValue = strstrFillValue.str();
	strstrFillValue.str("");
	
	// Set box width to longest line to be displayed
	if (strSystemDisplayName.size() > strFillValue.size()) maxLineLength = strSystemDisplayName.size() + 1;
	else maxLineLength = strFillValue.size() + 1;

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
	// Display green text background
	glColor3f(0.2f, 0.8f, 0.2f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, -1.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, float(numberLines) * 20.0f + 1.0f);
	glEnd();

	// Display Fill Name string
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2i(8, 28);
	glListBase(nFontList);
	glCallLists (strSystemDisplayName.size(), GL_UNSIGNED_BYTE, strSystemDisplayName.c_str());

	int lineNumber = 0;

	// Display fill rate value and units
	if (strFillValue.size() > 0)
	{	
		glRasterPos2i(8, (lineNumber * 20) + 4);
		glListBase(nFontList);
		glCallLists (strFillValue.size(), GL_UNSIGNED_BYTE, strFillValue.c_str());
		strFillValue = "";
	}

	glPopMatrix();
}

void LiquidTank::DrawCleaningButton2D(GLint nFontList, int xPos, int yPos, int xOrigin, int yOrigin, bool cleaningStatus)
{
	int numberLines = 1;
	int maxLineLength = 0;

	std::ostringstream strstrCleaningValue;
	std::string strCleaningValue;
	std::string strSystemDisplayName = strVesselName;

	std::string strCleaningDisplayName = "";

	if (cleaningStatus)
	{
		strCleaningDisplayName = "Cleaning On";
	}
	else
	{
		strCleaningDisplayName = "Cleaning Off";
	}
	
	// Set box width to longest line to be displayed
	if (strSystemDisplayName.size() > strCleaningDisplayName.size()) maxLineLength = strSystemDisplayName.size() + 1;
	else maxLineLength = strCleaningDisplayName.size() + 1;

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
	cleaningDisplayEdges.displayTopLeft[0] = xOrigin + xPos - 7;
	cleaningDisplayEdges.displayTopLeft[1] = yOrigin + yPos + ((numberLines + 1) * 20) + 7;
	cleaningDisplayEdges.dragBoxTopLeft[0] = xOrigin + xPos - 5;
	cleaningDisplayEdges.dragBoxTopLeft[1] = yOrigin + yPos + ((numberLines + 1) * 20) + 5;

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
	cleaningDisplayEdges.displayBottomRight[0] = xOrigin + xPos + ((maxLineLength) * 8) + 7;
	cleaningDisplayEdges.displayBottomRight[1] = yOrigin + yPos - 7;

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
	cleaningDisplayEdges.dragBoxBottomRight[0] = xOrigin + xPos + (maxLineLength * 8) + 3;
	cleaningDisplayEdges.dragBoxBottomRight[1] = yOrigin + yPos + (numberLines * 20) + 3;

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
	if (cleaningStatus)
	{
		// Display yellow text background when cleaning is on
		if (mouseOverCleaningDisplay) glColor3f(0.9f, 0.9f, 0.3f);
		else glColor3f(0.8f, 0.8f, 0.2f);
	}
	else
	{
		// Display green text background when cleaning is off
		if (mouseOverCleaningDisplay) glColor3f(0.3f, 0.9f, 0.3f);
		else glColor3f(0.2f, 0.8f, 0.2f);
	}
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, -1.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, float(numberLines) * 20.0f + 1.0f);
	glEnd();

	// Display Fill Name string
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2i(8, 28);
	glListBase(nFontList);
	glCallLists (strSystemDisplayName.size(), GL_UNSIGNED_BYTE, strSystemDisplayName.c_str());

	int lineNumber = 0;

	// Display fill rate value and units
	if (strCleaningDisplayName.size() > 0)
	{	
		glRasterPos2i(8, (lineNumber * 20) + 4);
		glListBase(nFontList);
		glCallLists (strCleaningDisplayName.size(), GL_UNSIGNED_BYTE, strCleaningDisplayName.c_str());
		strCleaningDisplayName = "";
	}

	glPopMatrix();
}

void LiquidTank::Display2D(GLint nFontList, float xOrigin, float yOrigin)
{
	// String and string stream object to format data to string
	std::string strData;
	std::ostringstream strstrData;
	int fontHeight = 15;
	GLfloat hueOffset = 0.5f;
	GLfloat brightness = 0.0f;
	GLfloat red = 0.0f;
	GLfloat green = 0.0f;
	GLfloat blue = 0.0f;

	// Liquid Tank modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(xOrigin, yOrigin, 0.0);

	// Liquid Tank outline
	glBegin(GL_POLYGON);
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex2f(-125.0f, 2.0f);
	glVertex2f(125.0f, 2.0f);
	glVertex2f(120.0f, -82.0f);
	glVertex2f(117.0f, -87.0f);
	glVertex2f(-117.0f, -87.0f);
	glVertex2f(-120.0f, -82.0f);
	glEnd();

	// Liquid Tank fill
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.9f);
	glVertex2f(-123.0f, 0.0f);
	glVertex2f(123.0f, 0.0f);
	glVertex2f(118.0f, -80.0f);
	glVertex2f(115.0f, -85.0f);
	glVertex2f(-115.0f, -85.0f);
	glVertex2f(-118.0f, -80.0f);
	glEnd();

	if (upperLiquidTemperatureSensorExists && ptrUpperTemperatureProbe != NULL)
	{
		glPushMatrix();
		glTranslatef(90, 40, 0.0);

		// Temperature probe shadow
		glBegin(GL_POLYGON);
		glColor3f(0.1f, 0.1f, 0.1f);
		glVertex2f(-5.0f, 20.0f);
		glVertex2f(5.0f, 20.0f);
		glVertex2f(5.0f, -62.0f);
		glVertex2f(3.0f, -67.0f);
		glVertex2f(-3.0f, -67.0f);
		glVertex2f(-5.0f, -62.0f);
		glEnd();

		// Temperature probe
		glBegin(GL_POLYGON);
		glColor3f(0.7f, 0.7f, 0.7f);
		glVertex2f(-3.0f, 20.0f);
		glVertex2f(3.0f, 20.0f);
		glVertex2f(3.0f, -60.0f);
		glVertex2f(1.0f, -65.0f);
		glVertex2f(-1.0f, -65.0f);
		glVertex2f(-3.0f, -60.0f);
		glEnd();

		// Temperature probe highlight
		glBegin(GL_POLYGON);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(-1.0f, 20.0f);
		glVertex2f(4.0f, 20.0f);
		glVertex2f(4.0f, -60.0f);
		glVertex2f(2.0f, -65.0f);
		glVertex2f(0.0f, -65.0f);
		glVertex2f(-1.0f, -60.0f);
		glEnd();

		glPopMatrix();

		// Draw temperature display box
		ptrUpperTemperatureProbe->DrawDataBox2D(nFontList, 20, 20, int(xOrigin), int(yOrigin));
	}
	if (lowerLiquidTemperatureSensorExists && ptrLowerTemperatureProbe != NULL)
	{
		glPushMatrix();
		glTranslatef(-100, 0, 0.0);

		// Temperature probe shadow
		glBegin(GL_POLYGON);
		glColor3f(0.1f, 0.1f, 0.1f);
		glVertex2f(-5.0f, 20.0f);
		glVertex2f(5.0f, 20.0f);
		glVertex2f(5.0f, -62.0f);
		glVertex2f(3.0f, -67.0f);
		glVertex2f(-3.0f, -67.0f);
		glVertex2f(-5.0f, -62.0f);
		glEnd();

		// Temperature probe
		glBegin(GL_POLYGON);
		glColor3f(0.7f, 0.7f, 0.7f);
		glVertex2f(-3.0f, 20.0f);
		glVertex2f(3.0f, 20.0f);
		glVertex2f(3.0f, -60.0f);
		glVertex2f(1.0f, -65.0f);
		glVertex2f(-1.0f, -65.0f);
		glVertex2f(-3.0f, -60.0f);
		glEnd();

		// Temperature probe highlight
		glBegin(GL_POLYGON);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(-1.0f, 20.0f);
		glVertex2f(4.0f, 20.0f);
		glVertex2f(4.0f, -60.0f);
		glVertex2f(2.0f, -65.0f);
		glVertex2f(0.0f, -65.0f);
		glVertex2f(-1.0f, -60.0f);
		glEnd();

		glPopMatrix();

		ptrLowerTemperatureProbe->DrawDataBox2D(nFontList, -180, 20, int(xOrigin), int(yOrigin));
	}
	
	if (distillateMeteringPumpExists && ptrDistillateMeteringPump != NULL)
	{
		glPushMatrix();
		glTranslatef(10.0, 0.0, 0.0);

		float pumpSpeed = ptrDistillateMeteringPump->GetPumpSpeed();
		if (pumpSpeed >= 0.0)
		{
			// Distillate Injection Arrow
			glBegin(GL_POLYGON);
			glColor3f(0.1f, 0.1f, 1.0f);
			glVertex2f(-2.0f * pumpSpeed, 80.0f);
			glVertex2f(2.0f * pumpSpeed, 80.0f);
			glVertex2f(2.0f * pumpSpeed, -70.0f);
			glVertex2f(-2.0f * pumpSpeed, -70.0f);
			glEnd();
			// Arrowhead
			glBegin(GL_POLYGON);
			glColor3f(0.1f, 0.1f, 1.0f);
			glVertex2f(8.0f * pumpSpeed, -70.0f);
			glVertex2f(0.0f, -80.0f);
			glVertex2f(-8.0f * pumpSpeed, -70.0f);
			glEnd();
		}
		glPopMatrix();

		ptrDistillateMeteringPump->DrawDataBox2D(nFontList, -160, 80, int(xOrigin), int(yOrigin));
	}

	if (highLevelExists && highLevelActive)
	{
		ptrHighLevelSwitch->DrawLevelIndicator(60, -10, int(xOrigin), int(yOrigin));
	}
	if (lowLevelExists && lowLevelActive)
	{
		ptrLowLevelSwitch->DrawLevelIndicator(-60, -10, int(xOrigin), int(yOrigin));
	}
	if (continuousLevelSensorExists && ptrTankLevelSensor != NULL)
	{
		ptrTankLevelSensor->DrawDataBox2D(nFontList, -100, -200, int(xOrigin), int(yOrigin));
	}

	DrawCleaningButton2D(nFontList, 50, -200, int(xOrigin), int(yOrigin), cleaning);
	while (lockFillRateBuffer) ptrSystemTime->Wait(1);
	lockForFillRateDisplay = true;
	DrawFillDataBox2D(nFontList, -100, -140, int(xOrigin), int(yOrigin), fill);
	lockForFillRateDisplay = false;
	glPopMatrix();

	DisplayProcessErrors2D(nFontList, xOrigin, yOrigin);
	ptrProgramExceptions->DisplayProgramExceptions(nFontList, (int)xOrigin - 160, 40);
	ptrHighLevelSwitch->DisplaySensorExceptions(nFontList, (int)xOrigin - 160, 180);
	ptrLowLevelSwitch->DisplaySensorExceptions(nFontList, (int)xOrigin - 160, 120);
}

bool LiquidTank :: GetMouseOverCleaningDisplay(int mouseX, int mouseY)
{
	if (mouseX > cleaningDisplayEdges.displayTopLeft[0] && mouseX < cleaningDisplayEdges.displayBottomRight[0] && mouseY < cleaningDisplayEdges.displayTopLeft[1] && mouseY > cleaningDisplayEdges.displayBottomRight[1])
	{
		mouseOverCleaningDisplay = true;
		return true;
	}
	else
	{
		mouseOverCleaningDisplay = false;
		return false;
	}
}

bool LiquidTank :: GetMouseOverCleaningDragBox(int mouseX, int mouseY)
{
	if (mouseX > cleaningDisplayEdges.dragBoxTopLeft[0] && mouseX < cleaningDisplayEdges.dragBoxBottomRight[0] && mouseY < cleaningDisplayEdges.dragBoxTopLeft[1] && mouseY > cleaningDisplayEdges.dragBoxBottomRight[1])
	{
		mouseOverCleaningDragBox = true;
		return true;
	}
	else
	{
		mouseOverCleaningDragBox = false;
		return false;
	}
}

void LiquidTank :: CheckMouseAction(int mouseX, int mouseY, int mouseAction)
{
	if (GetMouseOverCleaningDisplay(mouseX, mouseY))
	{
		mouseOverCleaningDisplay = true;
		if (mouseAction == 1)
		{
			if (cleaning) cleaning = false;
			else cleaning = true;
		}
	}
	else mouseOverCleaningDisplay = false;
	
	if (GetMouseOverCleaningDragBox(mouseX, mouseY)) mouseOverCleaningDragBox = true;
	else mouseOverCleaningDragBox = false;
}

void LiquidTank::DisplayProcessErrors2D(GLint nFontList, float xOrigin, float yOrigin)
{
	float xPos = xOrigin - 180.0f;
	float yPos = yOrigin - 290.0f;

	// Evaporation Tank modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(xPos, yPos, 0.0);

	if (GetNumberProcessErrors() > 0)
	{
		// Display any Process Errors
		glTranslatef(0.0, 0.0, 0.0);
		DisplayProcessErrors(nFontList);
	}

	glPopMatrix();
}