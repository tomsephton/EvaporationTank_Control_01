#pragma once
#include "processerrors.h"
#include "ProgramException.h"
#include "ProgramExceptions.h"

#include "DBK5.h"

#define  AMP0        4.0F			//can output between 4 and 20 mA
#define  AMP1        4.0F
#define  AMP2        4.0F	
#define  AMP3        4.0F
#define  DEVCOUNT	1
#define  CHANCOUNT  4

class AnalogOutput :
	public ProcessErrors
{
private:
	DaqDevice* ptrDaqDevice;
	DBK5* ptrDBK5device0;
//	DBK5* ptrDBK5device1;
//	DBK5* ptrDBK5device2;
	float requestedValveSetting[DEVCOUNT][CHANCOUNT];
protected:
	ProgramExceptions* ptrProgramExceptions;
public:
	AnalogOutput();
	virtual ~AnalogOutput();

	void SetDaqDevicePtr(DaqDevice* ptr);

	DBK5* GetDBK5Ptr(short deviceID);
	bool SetRequestedValveSetting(short deviceID, short channelID, float value);
	float GetRequestedValveSetting(short deviceID, short channelID);
	bool UpdateAnalogValves(char* devName, DaqHandleT handle);
	void DisplayExceptions(GLint NFontList, int xpos, int ypos);
};

