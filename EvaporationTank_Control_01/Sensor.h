#pragma once

#include <string.h>
#include <iostream>
#include <math.h>
#include "SystemTime.h"
#include "ProgramException.h"
#include "ProgramExceptions.h"

#define SENSOR_NAME_SIZE 80
#define SENSOR_TYPE_SIZE 160

class Sensor
{
private:
	short inputDeviceID; // should be set to data aquisition input device number for this sensor
	short inputPortID; // should be set to data aquisition input device port number for this sensor
	short inputChannelID; // should be set to data aquisition input device channel number for this sensor
	double location[3]; // should be set to physical x,y,z coordinates of sensor in meters
	char sensorName[SENSOR_NAME_SIZE]; // should be to null terminated string with name of sensor
	char sensorType[SENSOR_TYPE_SIZE]; // should be set to null terminated string with type of sensor

protected:
	SystemTime* ptrSystemTime;
	ProgramExceptions* ptrProgramExceptions;
	bool sensorOperatingOK; // set to true unless operator or system identifies a failure sensor

public:
	Sensor(void);
	~Sensor(void);
	void SetDeviceID (short device);
	short GetDeviceID (void);
	void SetPortID (short port);
	short GetPortID (void);
	void SetChannelID (short channel);
	short GetChannelID (void);
	void SetLocation (double x, double y, double z);
	bool GetLocation (double returnedLocation [3]);
	void SetSensorName (char name[80]);
	char* GetSensorNamePtr();
	void SetSensorType (char type[80]);
	char* GetSensorTypePtr();
	void SetSensorOperatingOK(bool status);
	bool GetSensorOperatingOK(void);
	void DisplaySensorExceptions(GLint NFontList, int xpos, int ypos);
};

