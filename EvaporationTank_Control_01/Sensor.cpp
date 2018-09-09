#include "StdAfx.h"
#include "Sensor.h"


Sensor::Sensor(void)
{
	inputDeviceID = -1; // should be set to data aquisition device number for this sensor
	inputPortID = -1; // should be set to data aquisition device port number for this sensor
	inputChannelID = -1; // should be set to data aquisition device channel number for this sensor
	location[0] = 0.0; // should be set to physical x,y,z coordinates of sensor in meters
	location[1] = 0.0; // should be set to physical x,y,z coordinates of sensor in meters
	location[2] = 0.0; // should be set to physical x,y,z coordinates of sensor in meters
	for (int index = 0; index < 80; index++)
	{
		sensorName[index] = '\0';
		sensorType[index] = '\0';
	}

	// Instantiate SystemTime object
	ptrSystemTime = new SystemTime();
	ptrProgramExceptions = new ProgramExceptions();

	// Flag set true unless a sensor failure is identied
	sensorOperatingOK = true;
}


Sensor::~Sensor(void)
{
	// Delete SystemTime object
	if (ptrSystemTime != NULL)
	{
		delete ptrSystemTime;
		ptrSystemTime = NULL;
	}
	// Delete ProgramExceptions object
	if (ptrProgramExceptions != NULL)
	{
		delete ptrProgramExceptions;
		ptrProgramExceptions = NULL;
	}
}

void Sensor::SetDeviceID (short device)
{
	inputDeviceID = device;
}

short Sensor::GetDeviceID (void)
{
	return inputDeviceID;
}

void Sensor::SetPortID (short port)
{
	inputPortID = port;
}

short Sensor::GetPortID (void)
{
	return inputPortID;
}

void Sensor::SetChannelID (short channel)
{
	inputChannelID = channel;
}

short Sensor::GetChannelID (void)
{
	return inputChannelID;
}

void Sensor :: SetLocation (double x, double y, double z)
{
	location[0] = x;
	location[1] = y;
	location[2] = z;
}

bool Sensor :: GetLocation (double returnedLocation [3])
{
	returnedLocation[0] = location [0];
	returnedLocation[1] = location [1];
	returnedLocation[2] = location [2];
	return true;
}

void Sensor :: SetSensorName (char name[80])
{
	strncpy_s(GetSensorNamePtr(), SENSOR_NAME_SIZE, name, strlen(name));
}

char* Sensor :: GetSensorNamePtr()
{
	return &sensorName[0];
}

void Sensor :: SetSensorType (char type[80])
{
	strncpy_s(GetSensorTypePtr(), SENSOR_TYPE_SIZE, type, strlen(type));
}

char* Sensor :: GetSensorTypePtr()
{
	return &sensorType[0];
}

void Sensor :: SetSensorOperatingOK(bool status)
{
	sensorOperatingOK = status;
}

bool Sensor :: GetSensorOperatingOK(void)
{
	return sensorOperatingOK;
}

void Sensor::DisplaySensorExceptions(GLint nFontList, int xpos, int ypos)
{
	ptrProgramExceptions->DisplayProgramExceptions(nFontList, xpos, ypos);
}