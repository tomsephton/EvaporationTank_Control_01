#pragma once
#include "analogsensor.h"
class Pyranometer :
	public AnalogSensor
{
private:
	double sensitivity;
public:
	bool SetSensitivity(double value);
	double GetSensitivity(void);
	Pyranometer(void);
	virtual ~Pyranometer(void);
};

