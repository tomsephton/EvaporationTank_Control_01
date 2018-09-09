#include "StdAfx.h"
#include "Pyranometer.h"

Pyranometer::Pyranometer(void)
{
	sensitivity = 0.00000824;
	SetSensorRange(0.0, 1600.0);
}


Pyranometer::~Pyranometer(void)
{
}

bool Pyranometer::SetSensitivity(double value)
{
	if (value > 0.0 && value < 0.00001)
	{
		sensitivity = value;
		return true;
	}
	else return false;
}

double Pyranometer::GetSensitivity(void)
{
	return sensitivity;
}