// DBK15.cpp: implementation of the DBK15 class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DBK15.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DBK15::DBK15(short deviceID)
{
	if (deviceID >= 0 && deviceID <= 7) inputDeviceID = deviceID;
	else inputDeviceID = -1;

	for (int i = 0; i < CHANNELCOUNT; i++)
	{
		channels[i] = 16 + i;
		gains[i] = Dbk15BiX1;
		flags[i] = DafBipolar;
		//when scaling, divide by the gain factor, DBK15 has gains of X1 and X2
		scale[i] = 1.0F;
	}
}

DBK15::~DBK15()
{

}

void DBK15::SetDaqDevicePtr(DaqDevice* ptr)
{
	if (ptr != NULL) ptrDaqDevice = ptr;
}

void DBK15::GetData(double dataArray[CHANNELCOUNT])
{
	for (int i = 0; i < CHANNELCOUNT; i++)
	{
		dataArray[i] = avgScanData[i];
	}
}

bool DBK15::Scan(char* devName, DaqHandleT handle, short deviceID)
{
	DWORD compMask = daq2000DIO|daqboard|daqbook;
	if (*devName != NULL)
	{
//		printf( "Attempting to Connect with %s\n", devName );		
		//attempt to open device
//		handle = daqOpen(devName);
		if (handle + 1)				//a return of -1 indicates failure
		{
			// Channel number set up for scan
			// 16 channels per device starting with device 0 at channels 16-31
			// Gains, flags, and scaling all set for 4-20mA input
			for (int index = 0; index < CHANNELCOUNT; index++)
			{
				channels[index] = ((deviceID + 1) * 16) + index;
				gains[index] = Dbk15UniX2;
				flags[index] = DafBipolar;
				//when scaling, divide by the gain factor, DBK15 has gains of X1 and X2
				scale[index] = 1.0F;
			}
			
			//scan set up
			// set # of scans to perform and scan mode
			daqAdcSetAcq(handle, DaamNShot, NULL, SCANS);	

			//Scan settings
			daqAdcSetScan(handle, channels, gains, flags, CHANNELCOUNT);   	

			//DBK's require 100 kHz clock, Daq2K has 200kHz 
			if (compMask & daq2000) daqAdcSetClockSource(handle, DacsAdcClockDiv2);

			//set scan rate
			daqAdcSetFreq(handle, RATE);

			//Set buffer location, size and flag settings
			daqAdcTransferSetBuffer(handle, buffer, SCANS, DatmUpdateSingle|DatmCycleOn);	
			
			//Set to Trigger Immediatly
			daqSetTriggerEvent(handle, STARTSOURCE, TRIGSENSE, channels[0], gains[0], flags[0],
		    						DaqTypeDBK15, NULL, NULL, DaqStartEvent);
			//Set to Stop when scan is complete
			daqSetTriggerEvent(handle, STOPSOURCE, TRIGSENSE, channels[0], gains[0], flags[0],
									DaqTypeDBK15, NULL, NULL, DaqStopEvent);

			//begin data acquisition
			daqAdcTransferStart(handle);
			daqAdcArm(handle);		
//			printf("\nTrigger Armed...");
			
			do 
			{
				//transfer data (voltage readings) into computer memory and halt acquisition when done
				daqAdcTransferGetStat(handle, &active, (ULONG*) &retCount);
			}
			while ( active & DaafAcqActive ); 

			// Added (by TS) to prevent invalid handle errors
			daqWaitForEvent(handle, DteAdcDone);

			//Disarm Daqboard
			daqAdcDisarm(handle);

//			printf("Scan Completed\n");

			//Close device connection
//			daqClose(handle);

			///convert the data to volts:
			//DaqBoards convert all data to an unsigned, 16-bit number (range 0 to 65535.  Zero corresponds 
			//to the minimum voltage, which is -maxVolt if in bipolar mode, or zero if unipolar.  
			//65535 corresponds to maxVolt if bipolar, or 2xmaxVolt if unipolar.  Note that a voltage 
			//higher than the device's absolute range (+/-10V for DaqBoard2000, +/-5V for other Daq* devices)
			//can damage the device.  Setting flags and gain settings which indicate a voltage higher than 
			//the device's max, such as an unipolar, unscaled scan will result in an error before the scan 
			//is performed.
			//The following routine automatically determines the max voltage for the device used
			//and the proper scaling and offset factors for the polarity flags selected.

			if ( compMask & daq10V)
				maxVolt = 10.0;		//Max voltage for Daq2K is +/-10V
			else							//Max voltage for Legacy is +/-5V
				maxVolt = 5.0;

			for (int p = 0; p < CHANNELCOUNT; p++)
			{	//The maximum output of the DBK15 is 5V, scaling factors are set at initialization.  
				offset[p] = scale[p]*(maxVolt/5.0F)*MAXIN;
				scale[p] = scale[p]*(maxVolt/5.0F)*MAXIN/32767.0F;	
			}
			for (WORD t = 0; t < retCount*CHANNELCOUNT; t++)
					conv_buffer[t] = buffer[t]*scale[t%CHANNELCOUNT] - offset[t%CHANNELCOUNT];

			int i, j;

			//Reorganize and print scan data
//			printf("Here is the Test Data: \n");
			double allScanData[SCANS][CHANNELCOUNT];
 			
			for(i = 0; i < SCANS; i++)
			{
			   for(j = 0; j < CHANNELCOUNT; j++)
			   {
					allScanData[i][j] = conv_buffer[(CHANNELCOUNT * i) + j];
//					printf("%2.3fV\t", conv_buffer[(CHANNELCOUNT * i) + j]); //change label for Amps!
			   }
//			   printf("\n");
			}
		
			// Average value for each channel over number of scans
			double scanSum = 0.0;
			for (j = 0; j < CHANNELCOUNT; j++)
			{
				for (i = 0; i < SCANS; i++)
				{
					scanSum += allScanData[i][j];
				}
				avgScanData[j] = scanSum/(double)SCANS;
				scanSum = 0.0;
//				printf("Channel %i average= %2.3fV\n", ((deviceID + 1) * 16) + j, avgScanData[j]); // Use to calibrate line loss for 4-20mA sensors
			}
//			printf("\n");
			return true;
		}
		else
		printf("Could not connect to device\n");
		return false;
	}
	else
	printf("No compatible devices were found for this example\n");
	return false;
}