// DataBox.cpp: implementation of the DataBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataBox::DataBox()
{

}

DataBox::~DataBox()
{

}

void DataBox::SetSensorDisplayName(std::string name)
{
	strSensorName = name;
}

void DataBox::DrawDataBox2D(GLint nFontList, std::string values[MAX_VALUE_LINES], std::string units[MAX_VALUE_LINES], int xPos, int yPos, int xOrigin, int yOrigin)
{
	unsigned int numberLines = 0;
	unsigned int maxValueLength = 0;
	unsigned int maxUnitLength = 0;
	unsigned int maxLineLength = 0;

	// Check arrays of string arguments for number of lines and maximumline length
	for (int index = 0; index < MAX_VALUE_LINES; index++)
	{
		if (values[index].size() > 0 || units[index].size() > 0)
		{
			numberLines++;
			if (values[index].size() > maxValueLength) maxValueLength = values[index].size();
			if (units[index].size() > maxUnitLength) maxUnitLength = units[index].size();
		}
	}

	// Set box width to longest line to be displayed
	if (strSensorName.size() > maxLineLength) maxLineLength = strSensorName.size();
	else maxLineLength = maxValueLength + maxUnitLength + 1;

	// Box modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(float(xPos), float(yPos), 0.0);

	// Top outer border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(-7.0f, float(numberLines * 20.0) + 7.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 7.0f, float(numberLines * 20.0) + 7.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, float(numberLines * 20.0) + 5.0f);
	glVertex2f(-5.0f, float(numberLines * 20.0) + 5.0f);
	glEnd();

	// Right outer border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(float(maxLineLength) * 8.0f + 7.0f, -7.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, -5.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, float(numberLines * 20.0) + 5.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 7.0f, float(numberLines * 20.0) + 7.0f);
	glEnd();

	// Lower outer border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(float(maxLineLength) * 8.0f + 7.0f, -7.0f);
	glVertex2f(-7.0f, -7.0f);
	glVertex2f(-5.0f, -5.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, -5.0f);
	glEnd();

	// Left outer border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(-7.0f, -7.0f);
	glVertex2f(-7.0f, float(numberLines * 20.0) + 7.0f);
	glVertex2f(-5.0f, float(numberLines * 20.0) + 5.0f);
	glVertex2f(-5.0f, -5.0f);
	glEnd();

	// Outer Rim of data box
	glBegin(GL_POLYGON);
	glColor3f(0.7f, 0.7f, 0.7f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, -5.0f);
	glVertex2f(-5.0f, -5.0f);
	glVertex2f(-5.0f, float(numberLines * 20.0) + 5.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 5.0f, float(numberLines * 20.0) + 5.0f);
	glEnd();

	// Top inner border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(-3.0f, float(numberLines * 20.0) + 3.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 3.0f, float(numberLines * 20.0) + 3.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glEnd();
	
	// Right inner border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(float(maxLineLength) * 8.0f + 3.0f, -3.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, -1.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 3.0f, float(numberLines * 20.0) + 3.0f);
	glEnd();

	// Lower inner border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(float(maxLineLength) * 8.0f + 3.0f, -3.0f);
	glVertex2f(-3.0f, -3.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, -1.0f);
	glEnd();

	// Left inner border of data box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex2f(-3.0f, -3.0f);
	glVertex2f(-3.0f, float(numberLines * 20.0) + 3.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(-1.0f, -1.0f);
	glEnd();

	// Text Background of data box
	glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.1f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, -1.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(-1.0f, float(numberLines * 20.0) + 1.0f);
	glVertex2f(float(maxLineLength) * 8.0f + 1.0f, float(numberLines * 20.0) + 1.0f);
	glEnd();

	// Display Sensor Name string
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2i(8, 4);
	glListBase(nFontList);
	glCallLists (strSensorName.size(), GL_UNSIGNED_BYTE, strSensorName.c_str());

	int lineNumber = 1;

	// Display strings passed in argument arrays
	for (int index = 0; index < MAX_VALUE_LINES; index++)
	{
		if (values[index].size() > 0 || units[index].size() > 0)
		{
			
			glRasterPos2i(8, (lineNumber * 20) + 4);
			glListBase(nFontList);
			glCallLists (values[index].size(), GL_UNSIGNED_BYTE, values[index].c_str());
			glRasterPos2i((values[index].size() + 1) * 8, (lineNumber * 20) + 4);
			glListBase(nFontList);
			glCallLists (units[index].size(), GL_UNSIGNED_BYTE, units[index].c_str());
			lineNumber++;
		}
	}

	glPopMatrix();
}