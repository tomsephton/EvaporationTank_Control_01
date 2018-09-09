#include "StdAfx.h"
#include "ProgramExceptions.h"


ProgramExceptions::ProgramExceptions()
{
	// Initialize array of Program Exception structs
	for (int index = 0; index < MAX_PROGRAM_EXCEPTIONS; index++)
	{
		SetProgramException(index, "", "", false);
	}
}


ProgramExceptions::~ProgramExceptions(void)
{
	// Clear array of Program Exception structs
	for (int index = 0; index < MAX_PROGRAM_EXCEPTIONS; index++)
	{
		SetProgramException(index, "", "", false);
	}
}

bool ProgramExceptions::SetProgramException(int index, std::string function, std::string exceptionDescription, bool status)
{
	if (index >= 0 && index < MAX_PROGRAM_EXCEPTIONS)
	{
		programExceptionArray[index].index = index;
		programExceptionArray[index].function = function;
		programExceptionArray[index].exceptionDescription = exceptionDescription;
		programExceptionArray[index].status = status;
		return true;
	}
	else return false;
}

std::string ProgramExceptions::GetProgramExceptionFunction(int index)
{
	return programExceptionArray[index].function;
}

std::string ProgramExceptions::GetProgramExceptionDescription(int index)
{
	return programExceptionArray[index].exceptionDescription;
}

bool ProgramExceptions::GetProgramExceptionStatus(int index)
{
	return programExceptionArray[index].status;
}
int ProgramExceptions::GetNumberProgramExceptions(void)
{
	int number = 0;

	// Check array of Process Error structs for number of slots marked 'true'
	for (int index = 0; index < MAX_PROGRAM_EXCEPTIONS; index++)
	{
		if (programExceptionArray[index].status == true) number++;
	}
	return number;
}

void ProgramExceptions::DisplayProgramExceptions(GLint nFontList, int xpos, int ypos)
{
	unsigned int numberLines = 0;
	unsigned int lengthComponent = 0;
	unsigned int lengthErrorText = 0;

	// Check array of Program Exception structs for number and size of slots marked 'true'
	for (int index = 0; index < MAX_PROGRAM_EXCEPTIONS; index++)
	{
		if (programExceptionArray[index].status == true)
		{
			numberLines++;
			if (GetProgramExceptionFunction(index).size() > lengthComponent) lengthComponent = GetProgramExceptionFunction(index).size();
			if (GetProgramExceptionDescription(index).size() > lengthErrorText) lengthErrorText = GetProgramExceptionDescription(index).size();
		}
	}

	if (numberLines < 1) return;

	// Program Exception display modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef((float)xpos + 8.0f, (float)ypos + 8.0f, 0.0f);

	// Top outer border of error box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(-7.0f, float(numberLines * 20.0) + 7.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 7.0f, float(numberLines * 20.0) + 7.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 5.0f, float(numberLines * 20.0) + 5.0f);
	glVertex2f(-5.0f, float(numberLines * 20.0) + 5.0f);
	glEnd();

	// Right outer border of error box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 7.0f, -7.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 5.0f, -5.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 5.0f, float(numberLines * 20.0) + 5.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 7.0f, float(numberLines * 20.0) + 7.0f);
	glEnd();

	// Lower outer border of error box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 7.0f, -7.0f);
	glVertex2f(-7.0f, -7.0f);
	glVertex2f(-5.0f, -5.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 5.0f, -5.0f);
	glEnd();

	// Left outer border of error box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(-7.0f, -7.0f);
	glVertex2f(-7.0f, float(numberLines * 20.0) + 7.0f);
	glVertex2f(-5.0f, float(numberLines * 20.0) + 5.0f);
	glVertex2f(-5.0f, -5.0f);
	glEnd();

	// Outer Rim of error box
	glBegin(GL_POLYGON);
	glColor3f(0.7f, 0.7f, 0.7f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 5.0f, -5.0f);
	glVertex2f(-5.0f, -5.0f);
	glVertex2f(-5.0f, float(numberLines * 20.0) + 5.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 5.0f, float(numberLines * 20.0) + 5.0f);
	glEnd();

	// Top inner border of error box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(-3.0f, float(numberLines * 20.0) + 3.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 3.0f, float(numberLines * 20.0) + 3.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glEnd();
	
	// Right inner border of error box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 3.0f, -3.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 1.0f, -1.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 3.0f, float(numberLines * 20.0) + 3.0f);
	glEnd();

	// Lower inner border of error box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 3.0f, -3.0f);
	glVertex2f(-3.0f, -3.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 1.0f, -1.0f);
	glEnd();

	// Left inner border of error box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(-3.0f, -3.0f);
	glVertex2f(-3.0f, float(numberLines * 20.0) + 3.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(-1.0f, -1.0f);
	glEnd();

	// Text Background of error box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.6f, 0.1f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 1.0f, -1.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 1.0f, float(numberLines * 20.0) + 1.0f);
	glEnd();

	// Display text of Program Exceptions marked 'true'
	int slot = 0;
	for (int index = 0; index < MAX_PROGRAM_EXCEPTIONS; index++)
	{
		if (programExceptionArray[index].status == true)
		{
			// Display Process Error text
			glColor3f(0.0f, 0.0f, 0.0f);
			glRasterPos2i(0, (slot * 20) + 4);
			glListBase(nFontList);
			glCallLists (GetProgramExceptionFunction(index).size(), GL_UNSIGNED_BYTE, GetProgramExceptionFunction(index).c_str());
			glRasterPos2i((lengthComponent + 1) * 8, (slot * 20) + 4);
			glListBase(nFontList);
			glCallLists (GetProgramExceptionDescription(index).size(), GL_UNSIGNED_BYTE, GetProgramExceptionDescription(index).c_str());
			slot++;
		}
	}
	glPopMatrix();
}

void ProgramExceptions::ClearProgramException(int index)
{
	// Clear one Program Exception structs on array
	if (index >= 0 && index < MAX_PROGRAM_EXCEPTIONS)
	{
		SetProgramException(index, "", "", false);
	}
	else SetProgramException(0, "ProgramExceptions::ClearProgramException ", "index out of range", true);
}

// Returns tab delimited string with all active exceptions
std::string ProgramExceptions::GetAllProgramExceptions(void)
{
	std::string strAllExceptions = "";
	std::ostringstream strstrAllExceptions;
	for (int i = 0; i < MAX_PROGRAM_EXCEPTIONS; i++)
	{
		if (programExceptionArray[i].status == true) strstrAllExceptions << programExceptionArray[i].index << " " << programExceptionArray[i].function << " " << programExceptionArray[i].exceptionDescription << "\t";
	}
	strAllExceptions = strstrAllExceptions.str();
	strstrAllExceptions.str("");
	return strAllExceptions;
}