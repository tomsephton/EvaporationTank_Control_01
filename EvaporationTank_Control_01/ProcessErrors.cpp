#include "StdAfx.h"
#include "ProcessErrors.h"

// Class Constructor
ProcessErrors::ProcessErrors(void)
{
	// Initialize array of Process Error structs
	for (int index = 0; index < MAX_PROCESS_ERRORS; index++)
	{
		SetProcessError(index, "", "", false);
	}
}

// Class Destructor
ProcessErrors::~ProcessErrors(void)
{
	// Clear array of Process Error structs
	for (int index = 0; index < MAX_PROCESS_ERRORS; index++)
	{
		SetProcessError(index, "", "", false);
	}
}

bool ProcessErrors::SetProcessError(int index, std::string component, std::string errorText, bool status)
{
	if (index >= 0 && index < MAX_PROCESS_ERRORS)
	{
		processErrorArray[index].index = index;
		processErrorArray[index].component = component;
		processErrorArray[index].errorText = errorText;
		processErrorArray[index].status = status;
		return true;
	}
	else return false;
}

std::string ProcessErrors::GetProcessErrorComponent(int index)
{
	return processErrorArray[index].component;
}

std::string ProcessErrors::GetProcessErrorText(int index)
{
	return processErrorArray[index].errorText;
}

bool ProcessErrors::GetProcessErrorStatus(int index)
{
	return processErrorArray[index].status;
}
int ProcessErrors::GetNumberProcessErrors(void)
{
	int number = 0;

	// Check array of Process Error structs for number of slots marked 'true'
	for (int index = 0; index < MAX_PROCESS_ERRORS; index++)
	{
		if (processErrorArray[index].status == true) number++;
	}
	return number;
}

void ProcessErrors::DisplayProcessErrors(GLint nFontList)
{
	unsigned int numberLines = 0;
	unsigned int lengthComponent = 0;
	unsigned int lengthErrorText = 0;

	// Check array of Process Error structs for number and size of slots marked 'true'
	for (int index = 0; index < MAX_PROCESS_ERRORS; index++)
	{
		if (processErrorArray[index].status == true)
		{
			numberLines++;
			if (GetProcessErrorComponent(index).size() > lengthComponent) lengthComponent = GetProcessErrorComponent(index).size();
			if (GetProcessErrorText(index).size() > lengthErrorText) lengthErrorText = GetProcessErrorText(index).size();
		}
	}

	if (numberLines < 1) return;

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
	glColor3f(0.9f, 0.9f, 0.1f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 1.0f, -1.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(float(lengthComponent + lengthErrorText + 1) * 8.0f + 1.0f, float(numberLines * 20.0) + 1.0f);
	glEnd();

	// Display text of Process Error marked 'true'
	int slot = 0;
	for (int index = 0; index < MAX_PROCESS_ERRORS; index++)
	{
		if (processErrorArray[index].status == true)
		{
			// Display Process Error text
			glColor3f(0.0f, 0.0f, 0.0f);
			glRasterPos2i(0, (slot * 20) + 4);
			glListBase(nFontList);
			glCallLists (GetProcessErrorComponent(index).size(), GL_UNSIGNED_BYTE, GetProcessErrorComponent(index).c_str());
			glRasterPos2i((lengthComponent + 1) * 8, (slot * 20) + 4);
			glListBase(nFontList);
			glCallLists (GetProcessErrorText(index).size(), GL_UNSIGNED_BYTE, GetProcessErrorText(index).c_str());
			slot++;
		}
	}
}