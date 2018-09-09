// DaqDevice.cpp: implementation of the DaqDevice class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DaqDevice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DaqDevice::DaqDevice()
{
	//lists the devices compatible with this example, see DaqRoutines.h
	DWORD compMask = daq2000DIO|daqboard|daqbook;

	//this code will poll for the first compatible device
	//if the preferred device name is known, set devName equal here
	devName = GetDeviceName(compMask);  //found in DaqRoutines.cpp

//	errorStream << "Instantiated Daq Device object";
}

DaqDevice::~DaqDevice()
{
	//Close device connection
//	daqClose(handle);
	handle = NULL;
}

char* DaqDevice::GetDaqName(void)
{
	if (devName != NULL)
	{
		return devName;
	}
	else return NULL;
}

void DaqDevice::SetHandle(DaqHandleT newHandle)
{
	handle = newHandle;
}

DaqHandleT DaqDevice::GetHandle(void)
{
	return handle;
}

bool DaqDevice::OpenDeviceConnection(void)
{
	handle = daqOpen(GetDaqName());
	if (handle + 1) return true; //a return of -1 indicates failure
	else return false;
}

void DaqDevice::CloseDeviceConnection()
{
	daqClose(handle);
}