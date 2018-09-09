#pragma once
#include "processerrors.h"
#include "ProgramException.h"
#include "ProgramExceptions.h"

#include "DBK23.h"

class BinaryInput :
	public ProcessErrors
{
private:
	bool dataArrayLoaded;
	// Data array representing current state of each input [device][port][channel]
	short currentDataArray[3][3][8];

	// Pointers to Input Devices
	DaqDevice* ptrDaqDevice;
	DBK23* ptrDBK23device0;
//	DBK23* ptrDBK23device1;
//	DBK23* ptrDBK23device2;

protected:
	ProgramExceptions* ptrProgramExceptions;
public:
	BinaryInput();
	virtual ~BinaryInput();

	void SetDaqDevicePtr(DaqDevice* ptr);

	short GetBinaryValue(short deviceID, short devicePort, short deviceChannel);
	bool ReadInputDevices(char* devName, DaqHandleT handle);
	void DisplayExceptions(GLint NFontList, int xpos, int ypos);
};

