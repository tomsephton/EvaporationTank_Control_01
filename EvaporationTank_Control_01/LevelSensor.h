#pragma once

#include "analogsensor.h"

class LevelSensor : public AnalogSensor
{
protected:
	double levelSetRange[2]; // minimum [0] and maximum [1] values of the sensor's target level
	double levelExceedsRange[2]; // minimum [0] and maximum [1] values of the sensor's level out of range settings

public:
	LevelSensor(void);
	virtual ~LevelSensor(void);
	void SetLevelRange(double low, double high);
	double GetLevelLowRange(void);
	double GetLevelHighRange(void);
	void SetLevelExceedsRange(double low, double high);
	double GetLevelExceedsLowRange(void);
	double GetLevelExceedsHighRange(void);
};

