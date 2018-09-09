// DBK25.h: interface for the DBK25 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBK25_H__DC8F261F_6D7E_4744_A215_D194BB945351__INCLUDED_)
#define AFX_DBK25_H__DC8F261F_6D7E_4744_A215_D194BB945351__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DaqDevice.h"

//#define	 RELAY0		0x00		//turns off all relays
//#define	 DEVICENUM	0			//set on DBK25 S1

class DBK25 : public DaqDevice
{
private:
	DaqDevice* ptrDaqDevice;
	short outputDeviceID;
public:
	DBK25(short deviceID);
	virtual ~DBK25();
	short GetDeviceID();

	void SetDaqDevicePtr(DaqDevice* ptr);

	bool WriteByte(char* devName, DaqHandleT handle, DWORD deviceNum, DWORD value);
	bool WriteBit(char* devName, DaqHandleT handle, DWORD deviceNum, DWORD bitNum, bool value);

};

#endif // !defined(AFX_DBK25_H__DC8F261F_6D7E_4744_A215_D194BB945351__INCLUDED_)
