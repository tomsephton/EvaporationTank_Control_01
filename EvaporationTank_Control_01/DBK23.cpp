// DBK23.cpp: implementation of the DBK23 class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DBK23.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DBK23::DBK23(short deviceID)
{
	if (deviceID >= 0 && deviceID <= 7) inputDeviceID = deviceID;
	else inputDeviceID = -1;
}

DBK23::~DBK23()
{

}

short DBK23::GetInputDeviceID(void)
{
	return inputDeviceID;
}

void DBK23::SetDaqDevicePtr(DaqDevice* ptr)
{
	if (ptr != NULL) ptrDaqDevice = ptr;
}

// DBK23NUM specifies which DBK23 board to read from, should match S1 setting on board
bool DBK23::Read(char* devName, DaqHandleT handle, DWORD DBK23NUM, unsigned valueA[1], unsigned valueB[1], unsigned valueC[1])
{
	DWORD retvalueA = valueA[0];
	DWORD retvalueB = valueC[0];
	DWORD retvalueC = valueC[0];

	if (*devName != NULL)
	{
//		printf( "Attempting to Connect with %s\n", devName );		
		//attempt to open device
//		handle = daqOpen(devName);
		if (handle + 1)				//a return of -1 indicates failure
		{
			//read DBK23 inputs
			daqIORead(handle, DiodtDbk23, DiodpDbk23A, DBK23NUM, DioepP2, &retvalueA);
			daqIORead(handle, DiodtDbk23, DiodpDbk23B, DBK23NUM, DioepP2, &retvalueB);
			daqIORead(handle, DiodtDbk23, DiodpDbk23C, DBK23NUM, DioepP2, &retvalueC);
/*
			printf("Digital input complete\n\n");
			printf("PortA: %i\n\n", retvalueA);
			printf("PortB: %i\n\n", retvalueB);
			printf("PortC: %i\n\n", retvalueC);
*/
			//Close device connection
//			daqClose(handle);

			valueA[0] = retvalueA;
			valueB[0] = retvalueB;
			valueC[0] = retvalueC;
			return true;
		}
		else
		{
			printf("Could not connect to device in DBK23::Read()\n");
			return false;
		}
	}
	else
	{
		printf("No compatible devices were found for DBK23::Read()\n");
		return false;
	}
}
