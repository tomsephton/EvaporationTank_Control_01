// DBK5.h: interface for the DBK5 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBK5_H__96887DDC_5A5B_41EA_9286_CA2E4E38CDE4__INCLUDED_)
#define AFX_DBK5_H__96887DDC_5A5B_41EA_9286_CA2E4E38CDE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DaqDevice.h"

#define  AMP0        4.0F			//can output between 4 and 20 mA
#define  AMP1        4.0F
#define  AMP2        4.0F	
#define  AMP3        4.0F	
#define  CHANCOUNT   4

class DBK5 : public DaqDevice  
{
private:
	DaqDevice* ptrDaqDevice;
	short outputDeviceID;

public:
	DBK5(short deviceID);
	virtual ~DBK5();

	void SetDaqDevicePtr(DaqDevice* ptr);

	bool SetControlValves(char* deviceName, DaqHandleT handle, DaqDacDeviceType deviceTypes[CHANCOUNT], DWORD chans[CHANCOUNT], float mAmps0, float mAmps1, float mAmps2, float mAmps3);
	bool CloseControlValves(char* deviceName, DaqHandleT handle, DaqDacDeviceType deviceTypes[CHANCOUNT], DWORD chans[CHANCOUNT]);


};

#endif // !defined(AFX_DBK5_H__96887DDC_5A5B_41EA_9286_CA2E4E38CDE4__INCLUDED_)
