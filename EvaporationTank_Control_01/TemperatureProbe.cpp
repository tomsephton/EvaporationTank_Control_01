#include "StdAfx.h"
#include "TemperatureProbe.h"

// Class Constructor
TemperatureProbe :: TemperatureProbe(void)
{
	scaleID = -1; // set to error value
	scaleString = "";
}

// Class Destructor
TemperatureProbe :: ~TemperatureProbe(void)
{
	scaleString = "";
}

bool TemperatureProbe :: SetScaleID(int scale)
{
	scaleID = scale;
	if (scale == 0)
	{
		scaleString = "degK";
		return true;
	}
	else if (scale == 1)
	{
		scaleString = "degC";
		return true;
	}
	else if (scale == 2)
	{
		scaleString = "degF";
		return true;
	}
	else return false;
}

char* TemperatureProbe :: GetScalePtr(void)
{
	return &scaleString[0];
}