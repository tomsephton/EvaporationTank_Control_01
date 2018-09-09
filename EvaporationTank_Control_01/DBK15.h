// DBK15.h: interface for the DBK15 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBK15_H__1392B5C2_5CF7_4E3A_A059_E00F36C9B662__INCLUDED_)
#define AFX_DBK15_H__1392B5C2_5CF7_4E3A_A059_E00F36C9B662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DaqDevice.h"
#include <conio.h>

#define STARTSOURCE		DatsImmediate
#define STOPSOURCE		DatsScanCount
#define CHANNELCOUNT	16
#define SCANS			10
#define RATE			1000
#define TRIGSENSE		(DaqEnhTrigSensT)NULL
#define MAXIN			5	//DBK15 can have a wide range of input voltage/ current ranges


class DBK15 : public DaqDevice  
{
private:
	DaqDevice* ptrDaqDevice;
	short inputDeviceID;
	double avgScanData[CHANNELCOUNT];

	//used during set up of scan
	WORD	buffer[SCANS*CHANNELCOUNT];
	DWORD channels[CHANNELCOUNT];
	DaqAdcGain gains[CHANNELCOUNT];
	DWORD flags[CHANNELCOUNT];
	//used to monitor scan
	DWORD	active, retCount;
	//used for scaling to SI units
	float maxVolt;
	//when scaling, divide by the gain factor, DBK15 has gains of X1 and X2
	float scale[CHANNELCOUNT];
	float offset[CHANNELCOUNT];
	float conv_buffer[SCANS*CHANNELCOUNT]; 

public:
	DBK15(short deviceID);
	virtual ~DBK15();

	void GetData(double dataArray[CHANNELCOUNT]);
	bool Scan(char* devName, DaqHandleT handle, short deviceID);
	void SetDaqDevicePtr(DaqDevice* ptr);

};

#endif // !defined(AFX_DBK15_H__1392B5C2_5CF7_4E3A_A059_E00F36C9B662__INCLUDED_)
