// DBK25.cpp: implementation of the DBK25 class.
//
//////////////////////////////////////////////////////////////////////

#include "DBK25.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DBK25::DBK25(short deviceID)
{
	if (deviceID >= 0 && deviceID <= 31) outputDeviceID = deviceID;
	else outputDeviceID = -1;
}

DBK25::~DBK25()
{

}

short DBK25::GetDeviceID()
{
	return outputDeviceID;
}

void DBK25::SetDaqDevicePtr(DaqDevice* ptr)
{
	if (ptr != NULL) ptrDaqDevice = ptr;
}

bool DBK25::WriteByte(char* devName, DaqHandleT handle, DWORD deviceNum, DWORD value)
{
	if (*devName != NULL)
	{
//		printf( "Attempting to Connect with %s for deviceID %i\n", devName, deviceNum );		
		//attempt to open device
//		handle = daqOpen(devName);
		if (handle + 1)				//a return of -1 indicates failure
		{
			//write will turn on and/or off specific relays
			daqIOWrite(handle, DiodtDbk25, DiodpDbk25, deviceNum, DioepP2, value);

			//Close device connection
//			daqClose(handle);
			return true;
		}
		else
		{
			printf("Could not connect to device in DBK25 WriteByte()\n");
			return false;
		}
	}
	else
	{
		printf("No compatible devices were found in DBK25 WriteByte()\n");
		return false;
	}
}

bool DBK25::WriteBit(char* devName, DaqHandleT handle, DWORD deviceNum, DWORD bitNum, bool value)
{
	if (*devName != NULL)
	{
//		printf( "Attempting to Connect with %s\n", devName );		
		//attempt to open device
//		handle = daqOpen(devName);
		if (handle + 1)				//a return of -1 indicates failure
		{
			//write will turn on or off a specific relay
			daqIOWriteBit(handle, DiodtDbk25, DiodpDbk25, deviceNum, DioepP2, bitNum, value);

			//Close device connection
//			daqClose(handle);
			return true;
		}
		else
		{
			printf("Could not connect to device in DBK25 WriteBit()\n");
			return false;
		}
	}
	else
	{
		printf("No compatible devices were found in DBK25 WriteBit()\n");
		return false;
	}
}
