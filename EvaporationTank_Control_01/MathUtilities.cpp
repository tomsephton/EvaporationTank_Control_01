#include "StdAfx.h"
#include "MathUtilities.h"

// Class Constructor
MathUtilities::MathUtilities(void)
{
	// Initialize smoothing array
	for (int i = 0; i < SMOOTHINGARRAYSIZE; i++)
	{
		for (int j = 0; j < SMOOTHINGARRAYVARIABLES; j++)
		{
			smoothingArray[i][j] = 0.0;
		}
	}

	// Initialize minute average array
	for (int k = 0; k < MINUTEAVERAGEARRAYSIZE; k++)
	{
		for (int l = 0; l < MINUTEAVERAGEARRAYVARIABLES; l++)
		{
			minuteAverageArray[k][l] = 0.0;
		}
	}
	ptrProgramExceptions = new ProgramExceptions();
}

// Class Destructor
MathUtilities::~MathUtilities(void)
{
	// Delete ProgramExceptions object
	if (ptrProgramExceptions != NULL)
	{
		delete ptrProgramExceptions;
		ptrProgramExceptions = NULL;
	}
}

bool MathUtilities::UpdateSmoothingData(double value, int arrayColumn)
{
	try
	{
		// Check if array column is valid
		if (arrayColumn >= 0 && arrayColumn < SMOOTHINGARRAYVARIABLES)
		{
			// Copy each existing value to next slot down in array
			for (int index = SMOOTHINGARRAYSIZE - 1; index > 0 ; index--)
			{
				smoothingArray[index][arrayColumn] = smoothingArray[index - 1][arrayColumn];
			}
			smoothingArray[0][arrayColumn] = value;
			return true;
		}
		else
		{
			throw ProgramException("Invalid array column");
			return false;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "MathUtilities::UpdateSmoothingData() ", pe.GetExceptionDescription(), true);
	}
	return false;
}

double MathUtilities::GetSmoothedData(int arrayColumn)
{
	try
	{
		// Check if array column is valid
		if (arrayColumn >= 0 && arrayColumn < SMOOTHINGARRAYVARIABLES)
		{
			// Average all values in smoothing array
			double sum = 0.0;
			// Sum each value in array
			for (int index = 0; index < SMOOTHINGARRAYSIZE; index++)
			{
				sum += smoothingArray[index][arrayColumn];
			}
			return sum / (double)SMOOTHINGARRAYSIZE;
		}
		else
		{
			throw ProgramException("Invalid array column");
			return 0.0;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "MathUtilities::GetSmoothedData() ", pe.GetExceptionDescription(), true);
	}
	return 0.0;
}

bool MathUtilities::UpdateMinuteAverageData(double value, int arrayColumn)
{
	try
	{
		// Check if array column is valid
		if (arrayColumn >= 0 && arrayColumn < MINUTEAVERAGEARRAYVARIABLES)
		{
			// Copy each existing value to next slot down in array
			for (int index = MINUTEAVERAGEARRAYSIZE - 1; index > 0 ; index--)
			{
				minuteAverageArray[index][arrayColumn] = minuteAverageArray[index - 1][arrayColumn];
			}
			minuteAverageArray[0][arrayColumn] = value;
			return true;
		}
		else
		{
			throw ProgramException("Invalid array column");
			return false;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "MathUtilities::UpdateMinuteAverageData() ", pe.GetExceptionDescription(), true);
	}
	return false;
}

double MathUtilities::GetMinuteAverageData(int arrayColumn)
{
	try
	{
		// Check if array column is valid
		if (arrayColumn >= 0 && arrayColumn < MINUTEAVERAGEARRAYVARIABLES)
		{
			// Average all values in smoothing array
			double sum = 0.0;
			// Sum each value in array
			for (int index = 0; index < MINUTEAVERAGEARRAYSIZE; index++)
			{
				sum += minuteAverageArray[index][arrayColumn];
			}
			return sum / (double)MINUTEAVERAGEARRAYSIZE;
		}
		else
		{
			throw ProgramException("Invalid array column");
			return 0.0;
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "MathUtilities::GetMinuteAverageData() ", pe.GetExceptionDescription(), true);
	}
	return 0.0;
}

// Rounds off double value to zero, one, two, three, or four decimal places
// according to precision preset for this sensor on startup
double MathUtilities::RoundOff(double value, short decimals)
{
	double intPart;
	double* ptrIntPart = &intPart;;
	double fractPart;
	double firstDec;
	double* ptrFirstDec = &firstDec;
	switch (decimals)
	{
	case 0:
		fractPart = modf(value, ptrIntPart);
		if (fractPart >= 0.5) intPart += 1.0;
		return intPart;
		break;
	case 1:
		fractPart = modf(value, ptrIntPart);
		fractPart = fractPart * 10.0;
		fractPart = modf(fractPart, ptrFirstDec);
		if (fractPart >= 0.5) firstDec += 1.0;
		return intPart + (firstDec/10.0);
		break;
	case 2:
		fractPart = modf(value, ptrIntPart);
		fractPart = fractPart * 100.0;
		fractPart = modf(fractPart, ptrFirstDec);
		if (fractPart >= 0.5) firstDec += 1.0;
		return intPart + (firstDec/100.0);
	case 3:
		fractPart = modf(value, ptrIntPart);
		fractPart = fractPart * 1000.0;
		fractPart = modf(fractPart, ptrFirstDec);
		if (fractPart >= 0.5) firstDec += 1.0;
		return intPart + (firstDec/1000.0);
	case 4:
		fractPart = modf(value, ptrIntPart);
		fractPart = fractPart * 10000.0;
		fractPart = modf(fractPart, ptrFirstDec);
		if (fractPart >= 0.5) firstDec += 1.0;
		return intPart + (firstDec/10000.0);
	default:
		return value;
	}
}