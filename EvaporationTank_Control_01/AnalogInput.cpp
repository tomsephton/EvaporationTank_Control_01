#include "StdAfx.h"
#include "AnalogInput.h"

AnalogInput::AnalogInput(void)
{
	// Initialize data array
	int i,j;
	for (i = 0; i < DEVICECOUNT; i++)
	{
		for (j = 0; j < CHANNELCOUNT; j++)
		{
			analogInputData[i][j] = 0.0;
		}
	}

	// Set actual voltages read for each input when
	// zero input (ie. 4 mA current) is connected
	zeroInputVoltage[0][0] = -3.010; // 16 
	zeroInputVoltage[0][1] = -3.002; // 17 
	zeroInputVoltage[0][2] = -3.011; // 18 
	zeroInputVoltage[0][3] = -3.007; // 19 
	zeroInputVoltage[0][4] = -3.000; // 20 
	zeroInputVoltage[0][5] = -3.000; // 21 
	zeroInputVoltage[0][6] = -2.904; // 22 
	zeroInputVoltage[0][7] = -2.968; // 23 1
	zeroInputVoltage[0][8] = -4.998; // 24 Set to Tank A lower temp 4mA (6 Aug 2015)
	zeroInputVoltage[0][9] = -4.994; // 25 Set to Tank A upper temp 4mA (6 Aug 2015)
	zeroInputVoltage[0][10] = -4.993; // 26 Set to Tank B lower temp 4mA (6 Aug 2015)
	zeroInputVoltage[0][11] = -4.991; // 27 Set to Tank B upper temp 4mA (6 Aug 2015)
	zeroInputVoltage[0][12] = -6.408; // 28 Set to Tank A continuous level 4mA (18 July 2015)
	zeroInputVoltage[0][13] = -4.651; // 29 Set to Tank B continuous level 4mA (18 July 2015)
	zeroInputVoltage[0][14] = -4.992; // 30 Set to ambient temperature sensor 4mA (6 Aug 2015)
	zeroInputVoltage[0][15] = -4.987; // 31 Set to transmitter on pyranometer line 4mA (6 Aug 2015)


	
	// Set actual voltages read for each input when
	// maximum input (ie. 20 mA current) is connected
	maxInputVoltage[0][0] = 4.911; // 16 
	maxInputVoltage[0][1] = 4.927; // 17 
	maxInputVoltage[0][2] = 4.917; // 18 
	maxInputVoltage[0][3] = 4.905; // 19 
	maxInputVoltage[0][4] = 4.911; // 20 
	maxInputVoltage[0][5] = 4.927; // 21 
	maxInputVoltage[0][6] = 4.987; // 22 
	maxInputVoltage[0][7] = 5.030; // 23 
	maxInputVoltage[0][8] = 4.977; // 24 Set to Tank A lower temp 20mA (6 Aug 2015)
	maxInputVoltage[0][9] = 4.904; // 25 Set to Tank A upper temp 20mA (6 Aug 2015)
	maxInputVoltage[0][10] = 5.003; // 26 Set to Tank B lower temp 20mA (6 Aug 2015)
	maxInputVoltage[0][11] = 5.028; // 27 Set to Tank B upper temp 20mA (6 Aug 2015)
	maxInputVoltage[0][12] = 4.119; // 28 Set to Tank A continuous level 20mA (18 July 2015)
	maxInputVoltage[0][13] = 4.420; // 29 Set to Tank B continuous level 20mA (18 July 2015)
	maxInputVoltage[0][14] = 5.001; // 30 Set to ambient temperature sensor 20mA (6 Aug 2015)
	maxInputVoltage[0][15] = 4.996; // 31 Set to transmitter on pyranometer line 20mA (6 Aug 2015)

	// Instantiate DBK15 analog input objects
	ptrDBK15device0 = new DBK15(0);
	ptrProgramExceptions = new ProgramExceptions();
}

AnalogInput::~AnalogInput(void)
{
	// Delete DBK15 analog input objects
	delete ptrDBK15device0;
	ptrDBK15device0 = NULL;

	// Delete ProgramExceptions object
	if (ptrProgramExceptions != NULL)
	{
		delete ptrProgramExceptions;
		ptrProgramExceptions = NULL;
	}
}


void AnalogInput::SetDaqDevicePtr(DaqDevice* ptr)
{
	if (ptr != NULL)
	{
		ptrDaqDevice = ptr;
		if (ptrDBK15device0 != NULL) ptrDBK15device0->SetDaqDevicePtr(ptrDaqDevice);
	}
}

double AnalogInput::GetAnalogValue(short deviceID, short channelID)
{
	try
	{
		if (deviceID >= 0 && deviceID < DEVICECOUNT)
		{
			if (channelID >= 0 && channelID < CHANNELCOUNT)
			{
				return analogInputData[deviceID][channelID];
			}
			else
			{
				throw ProgramException("channelID parameter out of range");
				return 0.0;
			}
		}
		else
		{
			throw ProgramException("deviceID parameter out of range");
			return 0.0;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "AnalogInput::GetAnalogValue() ", pe.GetExceptionDescription(), true);
	}
	return 0.0;
}

double AnalogInput::GetNormalizedValue(short deviceID, short channelID)
{
	try
	{
		if (deviceID >= 0 && deviceID < DEVICECOUNT)
		{
			if (channelID >= 0 && channelID < CHANNELCOUNT)
			{
				return normalizedInputData[deviceID][channelID];
			}
			else
			{
				throw ProgramException("channelID parameter out of range");
				return 0.0;
			}
		}
		else
		{
			throw ProgramException("deviceID parameter out of range");
			return 0.0;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "AnalogInput::GetNormalizedValue() ", pe.GetExceptionDescription(), true);
	}
	return 0.0;
}


bool AnalogInput::ReadInputDevices(char* devName, DaqHandleT handle)
{
	try
	{
		if (*devName == NULL || handle == -1)
		{
			throw ProgramException("Null devName or handle in AnalogInput::ReadInputDevices()");
			return false;
		}

		if (ptrDBK15device0 == NULL)
		{
			throw ProgramException("Null pointer to device object in AnalogInput::ReadInputDevices()");
			return false;
		}

		// Scan device 0
		if (ptrDBK15device0->Scan(devName, handle, 0))
		{
			ptrDBK15device0->GetData(analogInputData[0]);
		}
		else
		{
			throw ProgramException("DBK15 scan failed for device 0 in AnalogInput::ReadInputDevices()");
			return false;
		}
//		throw ProgramException(" loop OK");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "AnalogInput::ReadInputDevices() ", pe.GetExceptionDescription(), true);
	}
	// DBK15 devices scanned OK
	NormalizeInputData();
	return true;
}


// Converts input data from +/- Voltage to 0.0 - 1.0 range based on preset zero and maximum calibrated voltages
void AnalogInput::NormalizeInputData()
{
	// Normalize data array
	int device, channel;
	for (device = 0; device < DEVICECOUNT; device++)
	{
		for (channel = 0; channel < CHANNELCOUNT; channel++)
		{
			normalizedInputData[device][channel] = (analogInputData[device][channel] - zeroInputVoltage[device][channel]) / (maxInputVoltage[device][channel] - zeroInputVoltage[device][channel]);
		}
	}
}

void AnalogInput::DisplayExceptions(GLint nFontList, int xpos, int ypos)
{
	ptrProgramExceptions->DisplayProgramExceptions(nFontList, xpos, ypos);
}