#pragma once

#include "processerrors.h"
#include "ProgramException.h"
#include "ProgramExceptions.h"

#include "DBK15.h"

#define DEVICECOUNT	1
// CHANNELCOUNT defined in DBK15.h = 16, define here
#define CHANNELCOUNT 16

class AnalogInput :
	public ProcessErrors
{
	private:
	DaqDevice* ptrDaqDevice;
	DBK15* ptrDBK15device0;
//	DBK15* ptrDBK15device1;
//	DBK15* ptrDBK15device2;
	// Arrays for current input data, CHANNELCOUNT id defined in DBK15.h = 16
	double analogInputData[DEVICECOUNT][CHANNELCOUNT];
	double normalizedInputData[DEVICECOUNT][CHANNELCOUNT];
	double zeroInputVoltage[DEVICECOUNT][CHANNELCOUNT];
	double maxInputVoltage[DEVICECOUNT][CHANNELCOUNT];
protected:
	ProgramExceptions* ptrProgramExceptions;
public:
	AnalogInput();
	virtual ~AnalogInput();

	void SetDaqDevicePtr(DaqDevice* ptr);

	double GetAnalogValue(short deviceID, short channelID);
	double GetNormalizedValue(short deviceID, short channelID);
	bool ReadInputDevices(char* devName, DaqHandleT handle);
	void NormalizeInputData();
	void DisplayExceptions(GLint NFontList, int xpos, int ypos);
};

