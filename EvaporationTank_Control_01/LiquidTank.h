#pragma once

#include "ProgramException.h"
#include "ProgramExceptions.h"
#include "TankFillLogFileOutput.h"
#include "ProcessErrors.h"
#include "MathUtilities.h"
#include "LevelSwitch.h"
#include "LevelSensor.h"
#include "TemperatureProbe.h"
#include "MeteringPump.h"
#include "pyranometer.h"
#include "SystemTime.h"
#include "MathUtilities.h"
#include "LogFileOutput.h"
#include "AnalogInput.h"
#include "AnalogOutput.h"
#include "BinaryInput.h"
#include "BinaryOutput.h"

class LiquidTank : public ProcessErrors, MathUtilities
{
private:
	SystemTime* ptrSystemTime;
	MathUtilities* ptrMathUtilities;
	LogFileOutput* ptrLogFileOutput;

	double tankLevel; // water level in tank read by continuous level sensor

	char tankName[80]; // should be set to null terminated string with name of tank

	// Struct to store tank fill rate data on change over extended time for calculations
	struct storedFillRate
	{
		double rate;
		double volume;
		double levelEquivalent;
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
	unsigned int maxFillRateBufferSize;
	std::deque<storedFillRate> storedFillRateBuffer;
	std::deque<storedFillRate>::iterator fillRateIter;

protected:
	// Tank filling/draining parameters
	bool draining;
	bool filling;
	bool cleaning;
	double drainStartTime;
	double lastDrainTime;
	double fillStartTime;
	double lastFillTime;
	double fillVolumeSwitch;
	double fillVolumeSensor;
	double surfaceArea;
	bool fillSourceAvailable;
	bool continuousLevelSensorControlling;

	ProgramExceptions* ptrProgramExceptions;
	LevelSwitch* ptrHighLevelSwitch;
	LevelSwitch* ptrLowLevelSwitch;
	LevelSwitch* ptrHighAlarmSwitch;
	LevelSwitch* ptrLowAlarmSwitch;
	LevelSensor* ptrTankLevelSensor; // pointer to proportional level sensor object, extends analog sensor
	double GetTankLevel(void);
	bool SetTankLevel(double newLevel);

	TemperatureProbe* ptrUpperTemperatureProbe;
	TemperatureProbe* ptrLowerTemperatureProbe;
	MeteringPump* ptrDistillateMeteringPump;
	TemperatureProbe* ptrAmbientTemperatureProbe;
	Pyranometer* ptrPyranometer;
	TankFillLogFileOutput* ptrTankFillLogFileOutput;

	double upperLiquidTemperature;
	double lowerLiquidTemperature;
	double flowrate;
	float level;
	float maxVolume; // Should be set in gallons

	bool upperLiquidTemperatureSensorExists;
	bool lowerLiquidTemperatureSensorExists;
	bool distillateMeteringPumpExists;
	bool continuousLevelSensorExists;
	bool ambientTemperatureSensorExists;
	bool pyranometerExists;

	bool highAlarmExists;
	bool highAlarmActive;
	short highAlarmOn;

	bool lowAlarmExists;
	bool lowAlarmActive;
	short lowAlarmOn;

	bool highLevelExists;
	bool highLevelActive;
	short highLevelOn;

	bool lowLevelExists;
	bool lowLevelActive;
	short lowLevelOn;

	short outputDeviceID, outputPortID, outputChannelID;

	short highLevelInputDeviceID, highLevelInputDevicePort, highLevelInputDeviceChannel;
	short lowLevelInputDeviceID, lowLevelInputDevicePort, lowLevelInputDeviceChannel;
	short highAlarmInputDeviceID, highAlarmInputDevicePort, highAlarmInputDeviceChannel;
	short lowAlarmInputDeviceID, lowAlarmInputDevicePort, lowAlarmInputDeviceChannel;

	enum fillOrDrain{fill, drain};

	struct cleaningDisplayCoordinates
	{
		int displayTopLeft[2];
		int displayBottomRight[2];
		int dragBoxTopLeft[2];
		int dragBoxBottomRight[2];
	};
	cleaningDisplayCoordinates cleaningDisplayEdges;

	bool mouseOverCleaningDisplay;
	bool mouseOverCleaningDragBox;
	
public:
	LiquidTank();
	virtual ~LiquidTank(void);
	
	void InstantiateObjects(BinaryInput* ptrBinaryInput, BinaryOutput* ptrBinaryOutput, AnalogInput* ptrAnalogInput, AnalogOutput* ptrAnalogOutput, std::string id);

	SystemTime* GetSystemTimePtr(void);

	void SetLogFileOutputPtr(LogFileOutput* ptr);
	LogFileOutput* GetLogFileOutputPtr(void);

	void SetVesselName (char name[80]);
	char* GetVesselNamePtr();
	std::string GetTankNameStr();

	TemperatureProbe* GetUpperTemperatureProbePtr(void);
	TemperatureProbe* GetLowerTemperatureProbePtr(void);
	MeteringPump* GetDistillateMeteringPumpPtr(void);
	LevelSensor* GetTankLevelSensorPtr(void);
	TemperatureProbe* GetAmbientTemperatureProbePtr(void);
	Pyranometer* GetPyranometerPtr(void);

	bool SetUpperLiquidTemperature(double newTemperature);
	double GetUpperLiquidTemperature(void);

	bool SetLowerLiquidTemperature(double newTemperature);
	double GetLowerLiquidTemperature(void);

	bool SetDistillatePumpSpeed(float newPumpSpeed);
	float GetDistillatePumpSpeed(void);

	void SetMaxVolume(float volume);
	float GetMaxVolume(void);

	bool SetVolumeSwitch(double newVolume);
	double GetVolumeSwitch(void);
	bool SetVolumeSensor(double newVolume);
	double GetVolumeSensor(void);

	void SetUpperLiquidTemperatureSensorExists(bool exists);
	bool GetUpperLiquidTemperatureSensorExists(void);

	void SetLowerLiquidTemperatureSensorExists(bool exists);
	bool GetLowerLiquidTemperatureSensorExists(void);

	void SetDistillateMeteringPumpExists(bool exists);
	bool GetDistillateMeteringPumpExists(void);

	void SetContinuousLevelSensorExists(bool exists);
	bool GetContinuousLevelSensorExists(void);

	void SetHighAlarmExists(bool exists);
	bool GetHighAlarmExists(void);

	bool SetHighAlarmActive(bool active);
	bool GetHighAlarmActive(void);
	
	bool SetHighAlarmOn(short on);
	short GetHighAlarmOn(void);

	bool GetHighAlarmWavesDetected();

	void SetLowAlarmExists(bool exists);
	bool GetLowAlarmExists(void);
	
	bool SetLowAlarmActive(bool active);
	bool GetLowAlarmActive(void);
	
	bool SetLowAlarmOn(short on);
	short GetLowAlarmOn(void);

	bool GetLowAlarmWavesDetected();

	void SetHighLevelExists(bool exists);
	bool GetHighLevelExists(void);
	
	bool SetHighLevelActive(bool active);
	bool GetHighLevelActive(void);
	
	bool SetHighLevelOn(short on);
	short GetHighLevelOn(void);

	bool GetHighLevelWavesDetected();

	void SetLowLevelExists(bool exists);
	bool GetLowLevelExists(void);
	
	bool SetLowLevelActive(bool active);
	bool GetLowLevelActive(void);
	
	bool SetLowLevelOn(short on);
	short GetLowLevelOn(void);

	bool GetLowLevelWavesDetected();

	bool GetDraining(void);
	bool GetFilling(void);
	bool GetCleaning(void);
	double GetDrainStartTime(void);
	double GetLastDrainTime(void);
	double GetFillStartTime(void);
	double GetLastFillTime(void);

	double GetLastDrainRate(void);
	double GetLastFillRate(void);

	bool lockRateBuffer;
	bool lockForRateDisplay;

	// Fill rate methods for calculations
	void SetMaxFillRateBufferSize(unsigned int size);
	bool ResizeFillRateBuffer(unsigned int newSize);
	unsigned int GetFillRateBufferSize(void);
	double GetStoredFillRate(unsigned int i);
	double GetStoredFillVolume(unsigned int i);
	double GetStoredLevelEquivalent(unsigned int i);
	int GetFillRateBufferYear(unsigned int i);
	int GetFillRateBufferMonth(unsigned int i);
	int GetFillRateBufferDay(unsigned int i);
	int GetFillRateBufferHour(unsigned int i);
	int GetFillRateBufferMinute(unsigned int i);
	int GetFillRateBufferSecond(unsigned int i);
	int GetFillRateBufferMillisecond(unsigned int i);
	double GetFillRateBufferTimeInSeconds(unsigned int i);
	bool lockFillRateBuffer;
	bool lockForFillRateDisplay;

	// Tank fill rate calculation
	double GetLast24HourTankFill(void);
	double GetAny24HourTankFill(int year, int month, int day, int hour);
	bool StoreFillRate (double lastFillTime, bool proportional);
	double GetLevelEquivalent(double volumeChange);

	// Mouse interacton methods for cleaning button
	bool GetMouseOverCleaningDisplay(int mouseX, int mouseY);
	bool GetMouseOverCleaningDragBox(int mouseX, int mouseY);
	void CheckMouseAction(int mouseX, int mouseY, int mouseAction);

	bool UpdateLevelState(void);
	bool TankControlLogic(void);
	bool TankControlProportional(void);
	void Display2D(GLint nFontList, float xOrigin, float yOrigin);
	void DrawFillDataBox2D(GLint nFontList, int xPos, int yPos, int xOrigin, int yOrigin, int fillOrDrain);
	void DrawCleaningButton2D(GLint nFontList, int xPos, int yPos, int xOrigin, int yOrigin, bool cleaningStatus);
	void DrawLevelSensors(int xPos, int yPos, int xOrigin, int yOrigin);
	void DisplayProcessErrors2D(GLint nFontList, float xOrigin, float yOrigin);
};

