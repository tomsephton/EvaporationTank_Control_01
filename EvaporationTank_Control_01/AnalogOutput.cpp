#include "StdAfx.h"
#include "AnalogOutput.h"


AnalogOutput::AnalogOutput(void)
{
	ptrDBK5device0 = new DBK5(0);

	for (int i = 0; i < DEVCOUNT; i++)
	{
		for (int j = 0; j < CHANCOUNT; j++)
		{
			// Set all valves closed to start
			requestedValveSetting[i][j] = 4.0;
		}
	}
	ptrProgramExceptions = new ProgramExceptions();
}


AnalogOutput::~AnalogOutput(void)
{
	delete ptrDBK5device0;
	ptrDBK5device0 = NULL;

	// Delete ProgramExceptions object
	if (ptrProgramExceptions != NULL)
	{
		delete ptrProgramExceptions;
		ptrProgramExceptions = NULL;
	}
}


DBK5* AnalogOutput::GetDBK5Ptr(short deviceID)
{
	if (deviceID == 0) return ptrDBK5device0;
	else return NULL;
}

void AnalogOutput::SetDaqDevicePtr(DaqDevice* ptr)
{
	try
	{
		if (ptr != NULL)
		{
			ptrDaqDevice = ptr;
			if (ptrDBK5device0 != NULL) ptrDBK5device0->SetDaqDevicePtr(ptrDaqDevice);
			else throw ProgramException("Null pointer to DBK 5 object\n");
		}
		else throw ProgramException("Null pointer to DBK 5 object in argument\n");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "AnalogOutput::SetDaqDevicePtr() ", pe.GetExceptionDescription(), true);
	}
}

bool AnalogOutput::SetRequestedValveSetting(short deviceID, short channelID, float value)
{
	try
	{
		if (deviceID >= 0 && deviceID < DEVCOUNT)
		{
			if (channelID >=0 && channelID < CHANCOUNT)
			{
				if (value >= 4.0 && value <= 20.0)
				{
					requestedValveSetting[deviceID][channelID] = value;
					return true;
				}
				else // Valve setting value out of range
				{
					throw ProgramException("4-20 mA setting out of range");
					return false;
				}
			}
			else // channelID invalid
			{
				throw ProgramException("channelID out of range");
				return false;
			}
		}
		else // deviceID out of range
		{
			throw ProgramException("deviceID out of range");
			return false;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(2, "AnalogOutput::SetRequestedValveSetting() ", pe.GetExceptionDescription(), true);
	}
	return false;
}

float AnalogOutput::GetRequestedValveSetting(short deviceID, short channelID)
{
	if (channelID >=0 && channelID < CHANCOUNT)
	{
		return requestedValveSetting[deviceID][channelID];
	}
	else return 4.0;
}

bool AnalogOutput::UpdateAnalogValves(char* devName, DaqHandleT handle)
{
	try
	{
		if (*devName == NULL || handle == -1)
		{
			throw ProgramException("Null DAQ device name or handle in AnalogOutput::UpdateAnalogValves()\n");
			return false;
		}

		if (ptrDBK5device0 == NULL)
		{
			throw ProgramException("Null pointer to DBK 5 object in AnalogOutput::UpdateAnalogValves()\n");
			return false;
		}

		 //lists the devices compatible, see DaqRoutines.h
		DWORD compMask = daq2000AI|daqboard|daqbook|daq216|daqPCcard;
		//used to set up output
		DaqDacDeviceType  deviceTypes[CHANCOUNT] = {DddtDbk, DddtDbk, DddtDbk, DddtDbk};

		// Sets analog P1 bus channels for DBK5 device 0 (P1 channe l-3 sub 0)
		DWORD chansDev0[CHANCOUNT] = {32,33,34,35};

		if (devName != NULL && handle != -1)
		{
			if (ptrDBK5device0->SetControlValves(devName, handle, deviceTypes, chansDev0, requestedValveSetting[0][0], requestedValveSetting[0][1], requestedValveSetting[0][2], requestedValveSetting[0][3]));
			else
			{
				throw ProgramException("Set control valves failed for DBK5 device 0\n");
				return false;
			}
		}
		else
		{
			throw ProgramException("NULL device name or handle passed to UpdateAnalogValves()\n");
			return false;
		}
//		throw ProgramException(" completed without errors");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(3, "AnalogOutput::UpdateAnalogValves() ", pe.GetExceptionDescription(), true);
		return false;
	}
	return true;
}

void AnalogOutput::DisplayExceptions(GLint nFontList, int xpos, int ypos)
{
	ptrProgramExceptions->DisplayProgramExceptions(nFontList, xpos, ypos);
}