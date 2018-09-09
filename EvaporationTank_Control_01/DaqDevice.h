// DaqDevice.h: interface for the DaqDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAQDEVICE_H__652D181C_0CCC_441F_8F49_43641C49B175__INCLUDED_)
#define AFX_DAQDEVICE_H__652D181C_0CCC_441F_8F49_43641C49B175__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "daqx.h"
#include "daqroutines.h"

class DaqDevice
{
private:
	//used to connect to device
	char* devName;
	DaqHandleT handle;

protected:
	// Method sets name of DAQboard or related device
	bool SetDeviceName();

public:
	DaqDevice();
	virtual ~DaqDevice();
	char* GetDaqName(void);
	void SetHandle(DaqHandleT handle);
	DaqHandleT GetHandle(void);

	bool OpenDeviceConnection(void);
	void CloseDeviceConnection();
};

#endif // !defined(AFX_DAQDEVICE_H__652D181C_0CCC_441F_8F49_43641C49B175__INCLUDED_)
