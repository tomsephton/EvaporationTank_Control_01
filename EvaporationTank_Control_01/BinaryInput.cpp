#include "StdAfx.h"
#include "BinaryInput.h"

BinaryInput::BinaryInput()
{
	dataArrayLoaded = false;
	// Initialize data array
	int i, j, k;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			for (k = 0; k < 8; k++)
			{
				currentDataArray[i][j][k] = -1;
			}
		}
	}

	// Instantiate input device objects
	ptrDBK23device0 = new DBK23(0);
	ptrProgramExceptions = new ProgramExceptions();
}

BinaryInput::~BinaryInput()
{
	dataArrayLoaded = false;
	// Initialize data array
	int i, j, k;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			for (k = 0; k < 8; k++)
			{
				currentDataArray[i][j][k] = -1;
			}
		}
	}

	// Delete device objects
	if (ptrDBK23device0 != NULL)
	{
		delete ptrDBK23device0;
		ptrDBK23device0 = NULL;
	}

	// Delete ProgramExceptions object
	if (ptrProgramExceptions != NULL)
	{
		delete ptrProgramExceptions;
		ptrProgramExceptions = NULL;
	}
}

void BinaryInput::SetDaqDevicePtr(DaqDevice* ptr)
{
	if (ptr != NULL)
	{
		ptrDaqDevice = ptr;
		if (ptrDBK23device0 != NULL) ptrDBK23device0->SetDaqDevicePtr(ptrDaqDevice);
	}
}

short BinaryInput::GetBinaryValue(short deviceID, short devicePort, short deviceChannel)
{
//if (currentDataArray[deviceID][devicePort][deviceChannel] != 0 && currentDataArray[deviceID][devicePort][deviceChannel] != 1) return 0;
	if (currentDataArray[deviceID][devicePort][deviceChannel] != 0 && currentDataArray[deviceID][devicePort][deviceChannel] != 1) return -1;
	else return currentDataArray[deviceID][devicePort][deviceChannel];
}

bool BinaryInput::ReadInputDevices(char* devName, DaqHandleT handle)
{
	DWORD DBK23NUM;
	unsigned valueA[1];
	unsigned valueB[1];
	unsigned valueC[1];

	valueA[0] = -1;
	valueB[0] = -1;
	valueC[0] = -1;
	try
	{
		if (*devName == NULL || handle == -1)
		{
			throw ProgramException("Null devName or handle in BinaryInput::ReadInputDevices()");
			return false;
		}

		if (ptrDBK23device0 == NULL)
		{
			throw ProgramException("Null pointer to DBK23 device in BinaryInput::ReadInputDevices()");
			return false;
		}

		// Read DBK23 device 0
		DBK23NUM = 0;
		if (!ptrDBK23device0->Read(devName, handle, DBK23NUM, valueA, valueB, valueC))
		{
			throw ProgramException("Read of DBK23 device failed");
			return false;
		}
		else
		{
			// Convert Port A data and copy to array
			unsigned returnValue = valueA[0];

			//Convert low byte of unsigned integer to binary data
			currentDataArray[0][0][0] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][0][1] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][0][2] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][0][3] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][0][4] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][0][5] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][0][6] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][0][7] = returnValue;
	/*
			// Convert Port B data and copy to array
			returnValue = valueB[0];

			//Convert low byte of unsigned integer to binary data
			currentDataArray[0][1][0] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][1][1] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][1][2] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][1][3] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][1][4] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][1][5] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][1][6] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][1][7] = returnValue;

			// Convert Port C ata and copy to array
			returnValue = valueC[0];

			//Convert low byte of unsigned integer to binary data
			currentDataArray[0][2][0] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][2][1] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][2][2] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][2][3] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][2][4] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][2][5] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][2][6] = returnValue - (returnValue/2) * 2;
			returnValue = returnValue/2;
			currentDataArray[0][2][7] = returnValue;
	*/
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "BinaryInput::ReadInputDevices", pe.GetExceptionDescription(), true);
	}
	return true;
}


void BinaryInput::DisplayExceptions(GLint nFontList, int xpos, int ypos)
{
	ptrProgramExceptions->DisplayProgramExceptions(nFontList, xpos, ypos);
}