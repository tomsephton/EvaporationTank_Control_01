#include "StdAfx.h"
#include "LevelSensor.h"


LevelSensor::LevelSensor(void)
{
	levelSetRange[0] = 0.0; // minimum value that the sensor can read
	levelSetRange[1] = 0.0; // maximum value that the sensor can read
	levelExceedsRange[0] = 0.0; // minimum value that the sensor can read
	levelExceedsRange[1] = 0.0; // maximum value that the sensor can read
}


LevelSensor::~LevelSensor(void)
{
}

void LevelSensor :: SetLevelRange(double low, double high)
{
	levelSetRange[0] = low;
	levelSetRange[1] = high;
}

double LevelSensor :: GetLevelLowRange(void)
{
	return levelSetRange[0];
}

double LevelSensor :: GetLevelHighRange(void)
{
	return levelSetRange[1];
}

void LevelSensor :: SetLevelExceedsRange(double low, double high)
{
	levelExceedsRange[0] = low;
	levelExceedsRange[1] = high;
}

double LevelSensor :: GetLevelExceedsLowRange(void)
{
	return levelExceedsRange[0];
}

double LevelSensor :: GetLevelExceedsHighRange(void)
{
	return levelExceedsRange[1];
}