//  DAQROUTINES.CPP
//
//  uses 32bit enh API  
//
//  This file contains functions created for the Daq* examples 
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include "daqx.h"
#include "DaqRoutines.h"

DaqHandleT        handle;
DaqDeviceListT    *devList;
DaqDevicePropsT   devProps;
DWORD             devCount, deviceMask;
char              *deviceName;
BOOL              deviceFound = FALSE;
DWORD             deviceIndex = 0;
DWORD					subType		= 0;

	// GetDeviceName accepts a mask of compatible devices defined in daqroutines.h
	// It polls through the currently configured devices and returns the name of the
	// first one that is compatible as defined in the mask.  The flag is overwritten
	// as the returned device type

char* GetDeviceName(DWORD& capabilityMask)	//the device type is returned in the
											//flag to aid in scaling
{
	deviceName = "";
		// Find out how many devices are installed and allocate memory for device list
	daqGetDeviceCount(&devCount);
	devList = (DaqDeviceListT*)malloc(sizeof(DaqDeviceListT)*devCount);
		// Get the names of all installed devices and the device count
	daqGetDeviceList(devList, &devCount);

		// Loop 
	while ( (deviceIndex < devCount) && !deviceFound )
	{
		
		daqGetDeviceProperties(devList[deviceIndex].daqName, &devProps);		
		subType = devProps.socket;

		switch(devProps.deviceType) {
			case DaqBook2000:
				switch(subType) {
					case DaqSubTypeDaqBook2000A: deviceMask = (DWORD)pow((double)2, devProps.deviceType); break;
					case DaqSubTypeDaqBook2000E: deviceMask = (DWORD)pow((double)2, devProps.deviceType); break;
					case DaqSubTypeDaqBook2001 : deviceMask = (DWORD)pow((double)2, 26						); break;
					case DaqSubTypeDaqBook2005 : deviceMask = (DWORD)pow((double)2, 27						); break;
					case DaqSubTypeDaqBook2020 : deviceMask = (DWORD)pow((double)2, 28						); break;
					default							: deviceMask = (DWORD)pow((double)2, devProps.deviceType); break;
				} break;
			case DaqBoard1000 : deviceMask = (DWORD)pow((double)2, 24); break;
			case DaqBoard1005 : deviceMask = (DWORD)pow((double)2, 25); break;
			case DaqScan2000  : deviceMask = (DWORD)pow((double)2, 11); break;
			case DaqLab2000   : deviceMask = (DWORD)pow((double)2, 12); break;	
			case DaqBoard500  : deviceMask = (DWORD)pow((double)2, 14); break;
			case DaqBoard505  : deviceMask = (DWORD)pow((double)2, 15); break;
			default:
				if ( (DWORD)pow((double)2,devProps.deviceType) < 31 ) {
					capabilityMask &= 0;
					deviceFound = FALSE;
				} else {
					deviceMask = (DWORD)pow((double)2, devProps.deviceType);
				}
		};

			// If we found a compatable device, exit and return the device alias name
		if ( capabilityMask & deviceMask )
		{
			capabilityMask &= deviceMask;
			deviceName = devProps.daqName; 
			deviceFound = TRUE;
		}
		deviceIndex++;		
	}	
	
		// Release allocated memory
	free(devList);
	return deviceName;
}