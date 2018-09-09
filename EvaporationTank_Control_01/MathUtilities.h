#pragma once

#include <math.h>
#include "ProgramException.h"
#include "ProgramExceptions.h"

#define  SMOOTHINGARRAYSIZE 50
#define  SMOOTHINGARRAYVARIABLES 2

#define  MINUTEAVERAGEARRAYSIZE 600
#define  MINUTEAVERAGEARRAYVARIABLES 2

class MathUtilities
{
public:
	private:
	double smoothingArray[SMOOTHINGARRAYSIZE][SMOOTHINGARRAYVARIABLES];
	double minuteAverageArray[MINUTEAVERAGEARRAYSIZE][MINUTEAVERAGEARRAYVARIABLES];
protected:
	ProgramExceptions* ptrProgramExceptions;
public:
	MathUtilities(void);
	virtual ~MathUtilities(void);

	bool UpdateSmoothingData(double value, int arrayColumn);
	double GetSmoothedData(int arrayColumn);
	bool UpdateMinuteAverageData(double value, int arrayColumn);
	double GetMinuteAverageData(int arrayColumn);
	double RoundOff(double value, short decimals);
	void DisplayExceptions(GLint NFontList, int xpos, int ypos);
};

