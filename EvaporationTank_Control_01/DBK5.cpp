// DBK5.cpp: implementation of the DBK5 class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DBK5.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DBK5::DBK5(short deviceID)
{
	if (deviceID >= 0 && deviceID <= 7) outputDeviceID = deviceID;
	else outputDeviceID = -1;
}

DBK5::~DBK5()
{

}

void DBK5::SetDaqDevicePtr(DaqDevice* ptr)
{
	if (ptr != NULL) ptrDaqDevice = ptr;
}

bool DBK5::SetControlValves(char* deviceName, DaqHandleT handle, DaqDacDeviceType deviceTypes[CHANCOUNT], DWORD chans[CHANCOUNT], float mAmps0, float mAmps1, float mAmps2, float mAmps3)
{
	//0 = 4mA, 65535 = 20 mA
	WORD counts0 = (WORD)(65535*(mAmps0-4)/16);
	WORD counts1 = (WORD)(65535*(mAmps1-4)/16);
	WORD counts2 = (WORD)(65535*(mAmps2-4)/16);
	WORD counts3 = (WORD)(65535*(mAmps3-4)/16);
	WORD dataVals[CHANCOUNT] = {counts0, counts1, counts2, counts3};
	
//		printf( "Attempting to Connect with %s\n", deviceName );		
		//attempt to open device
//		handle = daqOpen(deviceName);
		if (handle + 1)				//a return of -1 indicates failure
		{			
			daqAdcExpSetBank(handle, chans[0], DaetDbk5);

			for(int i=0; i<CHANCOUNT; i++)
				daqDacSetOutputMode(handle, deviceTypes[i], chans[i], DdomVoltage);

			daqDacWtMany(handle, deviceTypes, chans, dataVals, CHANCOUNT);
/*		
			printf("\nOutputting %2.2f mA to Stage 1 Steam Control Valve on chan %d\n", mAmps0, chans[0]);
			printf("\nOutputting %2.2f mA to Stage 2 Steam Control Valve on chan %d\n", mAmps1, chans[1]);
			printf("\nOutputting %2.2f mA to Steam Side Vacuum Control Valve on chan %d\n", mAmps2, chans[2]);
			printf("\nOutputting %2.2f mA to Coolant Return Control Valve on chan %d\n", mAmps3, chans[3]);
*/
//			daqClose(handle);
			return true;
		}
		else
		printf("Could not connect to device\n");
		return false;
	
}

bool DBK5::CloseControlValves(char* deviceName, DaqHandleT handle, DaqDacDeviceType deviceTypes[CHANCOUNT], DWORD chans[CHANCOUNT])
{
	//set 4 mA data for shutdown
	WORD minVals[CHANCOUNT] = {0,0,0,0};

//		printf( "Attempting to Connect with %s\n", deviceName );		
		//attempt to open device
//		handle = daqOpen(deviceName);
		if (handle + 1)				//a return of -1 indicates failure
		{			
			daqAdcExpSetBank(handle, chans[0], DaetDbk5);

			for(int i=0; i<CHANCOUNT; i++)
				daqDacSetOutputMode(handle, deviceTypes[i], chans[i], DdomVoltage);

			// set output to 4mA minimum on closing
			daqDacWtMany(handle, deviceTypes, chans, minVals, CHANCOUNT);

			printf("\nSetting minumum %2.2f mA on chan %d\n", 4.0F, chans[0]);
			printf("\nSetting minumum %2.2f mA on chan %d\n", 4.0F, chans[1]);
			printf("\nSetting minumum %2.2f mA on chan %d\n", 4.0F, chans[2]);
			printf("\nSetting minumum %2.2f mA on chan %d\n", 4.0F, chans[3]);

//			daqClose(handle);
			return true;
		}
		else
		printf("Could not connect to device\n");
		return false;
	
}