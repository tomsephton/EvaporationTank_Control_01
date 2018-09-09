// DBK23.h: interface for the DBK23 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBK23_H__27231847_70EE_4492_A2F3_BDAED0AB01BC__INCLUDED_)
#define AFX_DBK23_H__27231847_70EE_4492_A2F3_BDAED0AB01BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DaqDevice.h"

class DBK23 : public DaqDevice
{
private:
	DaqDevice* ptrDaqDevice;
	short inputDeviceID;
public:
	DBK23(short deviceID);
	virtual ~DBK23();
	short GetInputDeviceID(void);

	void SetDaqDevicePtr(DaqDevice* ptr);

	// DBK23NUM specifies which DBK23 board to read from, should match S1 setting on board
	bool Read(char* devName, DaqHandleT handle, DWORD DBK23NUM, unsigned valueA[1], unsigned valueB[1], unsigned valueC[1]);
};

#endif // !defined(AFX_DBK23_H__27231847_70EE_4492_A2F3_BDAED0AB01BC__INCLUDED_)
