#include "StdAfx.h"
#include "BinaryOutput.h"

BinaryOutput::BinaryOutput()
{
	outputStateSet = false;
	// Initialize output state array
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 8; j++)
		{
			outputStateArray[i][j] = -1;
		}
	}

	// Initialize requested output state array
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 8; j++)
		{
			requestedStateArray[i][j] = 0;
		}
	}

	// Initialize device objects
//	ptrDBK25device0 = new DBK25(12);
//	ptrDBK25device1 = new DBK25(13);
//	ptrDBK25device2 = new DBK25(14);

	ptrProgramExceptions = new ProgramExceptions();
}

BinaryOutput::~BinaryOutput()
{
	outputStateSet = false;
	// Initialize state array
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 8; j++)
		{
			outputStateArray[i][j] = -1;
		}
	}
/*
	// Delete device objects
	if (ptrDBK25device0 != NULL)
	{
		delete ptrDBK25device0;
		ptrDBK25device0 = NULL;
	}

	if (ptrDBK25device1 != NULL)
	{
		delete ptrDBK25device1;
		ptrDBK25device1 = NULL;
	}

	if (ptrDBK25device2 != NULL)
	{
		delete ptrDBK25device2;
		ptrDBK25device2 = NULL;
	}
*/
	// Delete ProgramExceptions object
	if (ptrProgramExceptions != NULL)
	{
		delete ptrProgramExceptions;
		ptrProgramExceptions = NULL;
	}
}
/*
void BinaryOutput::SetDaqDevicePtr(DaqDevice* ptr)
{
	if (ptr != NULL)
	{
		ptrDaqDevice = ptr;
		if (ptrDBK25device0 != NULL) ptrDBK25device0->SetDaqDevicePtr(ptrDaqDevice);
		if (ptrDBK25device1 != NULL) ptrDBK25device1->SetDaqDevicePtr(ptrDaqDevice);
		if (ptrDBK25device2 != NULL) ptrDBK25device2->SetDaqDevicePtr(ptrDaqDevice);
	}
}

short BinaryOutput::GetBinaryState(short deviceID, short deviceChannel)
{
	if (outputStateArray[deviceID][deviceChannel] != 0 && outputStateArray[deviceID][deviceChannel] != 1) return -1;
	else return outputStateArray[deviceID][deviceChannel];
}

// Sets bit value in requestedStateArray for later write to output device
bool BinaryOutput::SetRequestedState(short deviceID, short deviceChannel, short value)
{
	if (deviceID < 0 || deviceID > 3)
	{
		errorStream << "DeviceID parameter " << deviceID << "is out of range." << "\n";
		return false;
	}
	else if (deviceChannel < 0 || deviceChannel > 7)
	{
		errorStream << "DeviceChannel parameter " << deviceChannel << "is out of range." << "\n";	
		return false;
	}
	else if (value != 0 && value != 1)
	{
		errorStream << "State parameter " << value << "is out of range." << "\n";
		return false;
	}
	else 
	{
		requestedStateArray[deviceID][deviceChannel] = value;
		return true;
	}
}
*/
/*
void BinaryOutput::InitializeOutputDevices(void)
{
	errorStream << "Constructing DBK25 objects" << "\n";
	// Instantiate with hardware device ID set on S1 on each board
	ptrDBK25device0 = new DBK25(12);
	ptrDBK25device1 = new DBK25(13);
	ptrDBK25device2 = new DBK25(14);
}
*/
/*
bool BinaryOutput::WriteBit(char* devName, DaqHandleT handle, short deviceID, short deviceChannel, bool value)
{
	// Hardware device ID as set on S1 on board
	short hardwareDeviceID;

	if (*devName == NULL || handle == -1)
	{
//		errorStream << "Null devName or handle in BinaryOutput::WriteBit()." << "\n";
		return false;
	}

//	errorStream << "Attempting to write bit to channel " << deviceChannel << " on DBK25 device " << deviceID << " with value " << value << "\n";

	if (deviceID == 0)
	{
		hardwareDeviceID = ptrDBK25device0->GetDeviceID();
//		errorStream << "Attempting to set relay on channel " << deviceChannel << " on DBK25 hardware device " << hardwareDeviceID << " with value " << value << "\n";
		if (ptrDBK25device0->WriteBit(devName, handle, (DWORD)hardwareDeviceID, (DWORD)deviceChannel, value)) return true;
		else return false;
	}
	else if (deviceID == 1)
	{
		hardwareDeviceID = ptrDBK25device1->GetDeviceID();
//		errorStream << "Attempting to set relay on channel " << deviceChannel << " on DBK25 hardware device " << hardwareDeviceID << " with value " << value << "\n";
		if (ptrDBK25device1->WriteBit(devName, handle, (DWORD)hardwareDeviceID, (DWORD)deviceChannel, value)) return true;
		else return false;
	}
	else if (deviceID == 2)
	{
		hardwareDeviceID = ptrDBK25device2->GetDeviceID();
//		errorStream << "Attempting to set relay on channel " << deviceChannel << " on DBK25 hardware device " << hardwareDeviceID << " with value " << value << "\n";
		if (ptrDBK25device2->WriteBit(devName, handle, (DWORD)hardwareDeviceID, (DWORD)deviceChannel, value)) return true;
		else return false;
	}
	else return false;
}
*/
/*
bool BinaryOutput::WriteByte(char* devName, DaqHandleT handle, short deviceID, short deviceChannel, int value)
{
	// Hardware device ID as set on S1 on board
	short hardwareDeviceID;

	if (*devName == NULL || handle == -1)
	{
		errorStream << "Null devName or handle in BinaryOutput::WriteByte()." << "\n";
		return false;
	}

//	errorStream << "Attempting to write byte to DBK25 device " << deviceID << " with value " << value << "\n";

	if (deviceID == 0)
	{
		if (ptrDBK25device0 == NULL)
		{
			errorStream << "Null pointer to DBK25 device 0 in BinaryOutput::WriteByte())." << "\n";
			return false;
		}
		hardwareDeviceID = ptrDBK25device0->GetDeviceID();
//		errorStream << "Attempting to set relays on DBK25 hardware device " << hardwareDeviceID << " with value " << value << "\n";
		if (ptrDBK25device0->WriteByte(devName, handle, (DWORD)hardwareDeviceID, (DWORD)value)) return true;
		else return false;
	}
	else if (deviceID == 1)
	{
		if (ptrDBK25device1 == NULL)
		{
			errorStream << "Null pointer to DBK25 device 1 in BinaryOutput::WriteByte())." << "\n";
			return false;
		}
		hardwareDeviceID = ptrDBK25device1->GetDeviceID();
//		errorStream << "Attempting to set relays on DBK25 hardware device " << hardwareDeviceID << " with value " << value << "\n";
		if (ptrDBK25device1->WriteByte(devName, handle, (DWORD)hardwareDeviceID, (DWORD)value)) return true;
		else return false;
	}
	else if (deviceID == 2)
	{
		if (ptrDBK25device2 == NULL)
		{
			errorStream << "Null pointer to DBK25 device 2 in BinaryOutput::WriteByte())." << "\n";
			return false;
		}
		hardwareDeviceID = ptrDBK25device2->GetDeviceID();
//		errorStream << "Attempting to set relays on DBK25 hardware device " << hardwareDeviceID << " with value " << value << "\n";
		if (ptrDBK25device2->WriteByte(devName, handle, (DWORD)hardwareDeviceID, (DWORD)value)) return true;
		else return false;
	}
	else return false;
}
*/
/*
bool BinaryOutput::ZeroAllBits(char* devName, DaqHandleT handle)
{
	// Hardware device ID as set on S1 on board
	short hardwareDeviceID;

	if (*devName == NULL || handle == -1)
	{
		errorStream << "Null DAQ device name or handle in BinaryOutput::ZeroAllBits()." << "\n";
		return false;
	}

	// Turn off all relays on DBK25 device 0
	if (ptrDBK25device0 != NULL)
	{
		hardwareDeviceID = ptrDBK25device0->GetDeviceID();
//		errorStream << "Attempting to turn off relays on DBK25 hardware device " << hardwareDeviceID << "\n";
		if (!ptrDBK25device0->WriteByte(devName, handle, (DWORD)hardwareDeviceID, 0))
		{
			errorStream << "Call to ptrDBK25device0->WriteByte() failed in BinaryOutput::ZeroAllBits()." << "\n";
			return false;
		}
	}
	else
	{
		errorStream << "Null pointer to DBK25 device 0 in BinaryOutput::ZeroAllBits()." << "\n";
		return false;
	}

	// Turn off all relays on DBK25 device 1
	if (ptrDBK25device1 != NULL)
	{
		hardwareDeviceID = ptrDBK25device1->GetDeviceID();
//		errorStream << "Attempting to turn off relays on DBK25 hardware device " << hardwareDeviceID << "\n";
		if (!ptrDBK25device1->WriteByte(devName, handle, (DWORD)hardwareDeviceID, 0))
		{
			errorStream << "Call to ptrDBK25device1->WriteByte() failed in BinaryOutput::ZeroAllBits()." << "\n";
			return false;
		}
	}
	else
	{
		errorStream << "Null pointer to DBK25 device 1 in BinaryOutput::ZeroAllBits()." << "\n";
		return false;
	}

	// Turn off all relays on DBK25 device 2
	if (ptrDBK25device2 != NULL)
	{
		hardwareDeviceID = ptrDBK25device2->GetDeviceID();
//		errorStream << "Attempting to turn off relays on DBK25 hardware device " << hardwareDeviceID << "\n";
		if (!ptrDBK25device2->WriteByte(devName, handle, (DWORD)hardwareDeviceID, 0))
		{
			errorStream << "Call to ptrDBK25device2->WriteByte() failed in BinaryOutput::ZeroAllBits()." << "\n";
			return false;
		}
	}
	else
	{
		errorStream << "Null pointer to DBK25 device 2 in BinaryOutput::ZeroAllBits()." << "\n";
		return false;
	}
	return true;
}
*/
/*
bool BinaryOutput::UpdateAllRelays(char* devName, DaqHandleT handle)
{
	// Integer value to send to Output Device
	int value = 0;

	if (*devName == NULL || handle == -1)
	{
		errorStream << "Null DAQ device name or handle in BinaryOutput::UpdateAllRelays()." << "\n";
		return false;
	}

	if (ptrDBK25device0 == NULL || ptrDBK25device1 == NULL || ptrDBK25device2 == NULL)
	{
		errorStream << "Null pointer to DBK 25 object in BinaryOutput::UpdateAllRelays()." << "\n";
		return false;
	}

	// Set integer to send to Device 0 from first byte in state array
	value += requestedStateArray[0][7] * 128;
	value += requestedStateArray[0][6] * 64;
	value += requestedStateArray[0][5] * 32;
	value += requestedStateArray[0][4] * 16;
	value += requestedStateArray[0][3] * 8;
	value += requestedStateArray[0][2] * 4;
	value += requestedStateArray[0][1] * 2;
	value += requestedStateArray[0][0];

	// Hardware device ID as set on S1 on board
	short hardwareDeviceID;

	hardwareDeviceID = ptrDBK25device0->GetDeviceID();
//	errorStream << "Attempting to set relays on DBK25 hardware device " << hardwareDeviceID << " with value=" << value << "\n";
	if (!ptrDBK25device0->WriteByte(devName, handle, (DWORD)hardwareDeviceID, (DWORD)value)) return false;

	// Set integer to send to Device 1 from second byte in state array
	value = 0;
	value += requestedStateArray[1][7] * 128;
	value += requestedStateArray[1][6] * 64;
	value += requestedStateArray[1][5] * 32;
	value += requestedStateArray[1][4] * 16;
	value += requestedStateArray[1][3] * 8;
	value += requestedStateArray[1][2] * 4;
	value += requestedStateArray[1][1] * 2;
	value += requestedStateArray[1][0];

	hardwareDeviceID = ptrDBK25device1->GetDeviceID();
//	errorStream << "Attempting to set relays on DBK25 hardware device " << hardwareDeviceID << " with value=" << value << "\n";
	if (!ptrDBK25device1->WriteByte(devName, handle, (DWORD)hardwareDeviceID, (DWORD)value)) return false;

	// Set integer to send to Device 2 from third byte in state array
	value = 0;
	value += requestedStateArray[2][7] * 128;
	value += requestedStateArray[2][6] * 64;
	value += requestedStateArray[2][5] * 32;
	value += requestedStateArray[2][4] * 16;
	value += requestedStateArray[2][3] * 8;
	value += requestedStateArray[2][2] * 4;
	value += requestedStateArray[2][1] * 2;
	value += requestedStateArray[2][0];

	hardwareDeviceID = ptrDBK25device2->GetDeviceID();
//	errorStream << "Attempting to set relays on DBK25 hardware device " << hardwareDeviceID << " with value=" << value << "\n";
	if (!ptrDBK25device2->WriteByte(devName, handle, (DWORD)hardwareDeviceID, (DWORD)value)) return false;

	return true;
}
*/

void BinaryOutput::DisplayExceptions(GLint nFontList, int xpos, int ypos)
{
	ptrProgramExceptions->DisplayProgramExceptions(nFontList, xpos, ypos);
}