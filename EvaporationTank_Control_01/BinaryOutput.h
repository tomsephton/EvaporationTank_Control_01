#pragma once
#include "processerrors.h"
#include "ProgramException.h"
#include "ProgramExceptions.h"

//#include "DBK25.h"

// BinaryOutput class
// This class encapsulates data exchange with On/Off output devices such as relays,
// acts as a container for the current state of those output devices,
// and provides an interface for all objects that need to send binary state change commands to relays, etc.
// 
class BinaryOutput :
	public ProcessErrors
{
private:
	bool outputStateSet;
	// Array representing current state of each output [device][channel]
	short outputStateArray[3][8];
	// Array holding state of each output[device][channel] requested by other objects
	short requestedStateArray[3][8];

	// Pointers to Output Devices
//	DaqDevice* ptrDaqDevice;
//	DBK25* ptrDBK25device0;
//	DBK25* ptrDBK25device1;
//	DBK25* ptrDBK25device2;

protected:
	ProgramExceptions* ptrProgramExceptions;
public:
	BinaryOutput();
	virtual ~BinaryOutput();

//	void SetDaqDevicePtr(DaqDevice* ptr);

	short GetBinaryState(short deviceID, short deviceChannel);
	bool SetRequestedState(short deviceID, short deviceChannel, short value);
//	bool WriteBit(char* devName, DaqHandleT handle, short deviceID, short deviceChannel, bool value);
//	bool WriteByte(char* devName, DaqHandleT handle, short deviceID, short deviceChannel, int value);
//	bool ZeroAllBits(char* devName, DaqHandleT handle);
//	bool UpdateAllRelays(char* devName, DaqHandleT handle);
	void DisplayExceptions(GLint NFontList, int xpos, int ypos);
};

