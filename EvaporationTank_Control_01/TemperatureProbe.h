#pragma once
#include "AnalogSensor.h"

class TemperatureProbe : public AnalogSensor
{
private:
	int scaleID; // identifies temperature scale 0=degK, 1=degC, 2=degF
	std::string scaleString;
public:
	TemperatureProbe();
	virtual ~TemperatureProbe(void);
	bool SetScaleID(int scale);
	char* GetScalePtr(void);
};

