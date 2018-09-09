#include "StdAfx.h"
#include "LevelSwitch.h"

// Class Constructor
LevelSwitch::LevelSwitch(void)
{
	for (int index = 0; index < 12; index++)
	{
		whichSwitchString[index] = '\0';
	}
	wavesDetected = false;
}

// Class Destructor
LevelSwitch::~LevelSwitch(void)
{
}


void LevelSwitch :: SetConfiguration(bool normalOpen)
{
	if (normalOpen) configuration = 1;
	else configuration = 0;
}

short LevelSwitch :: GetConfiguration(void)
{
	return configuration;
}

void LevelSwitch :: SetWhichSwitch(short setting)
{
	if (setting == -1)
	{
		whichSwitch = -1;
		strcpy_s(GetWhichSwitchStringPtr(), WHICHSWITCHSTRINGSIZE, "unspecified");
	}
	if (setting == 0)
	{
		whichSwitch = 0;
		strcpy_s(GetWhichSwitchStringPtr(), WHICHSWITCHSTRINGSIZE, "low alarm");
	}
	if (setting == 1)
	{
		whichSwitch = 1;
		strcpy_s(GetWhichSwitchStringPtr(), WHICHSWITCHSTRINGSIZE, "low level");
	}
	if (setting == 2)
	{
		whichSwitch = 2;
		strcpy_s(GetWhichSwitchStringPtr(), WHICHSWITCHSTRINGSIZE, "high level");
	}
	if (setting == 3)
	{
		whichSwitch = 3;
		strcpy_s(GetWhichSwitchStringPtr(), WHICHSWITCHSTRINGSIZE, "high alarm");
	}
	else whichSwitch = -1;
}
	
short LevelSwitch :: GetWhichSwitch(void)
{
	return whichSwitch;
}

char* LevelSwitch :: GetWhichSwitchStringPtr(void)
{
	return &whichSwitchString[0];
}

// Detect waves as average time between level switch state change between 1 sec and 30 sec
bool LevelSwitch :: GetWavesDetected()
{
	if (averageSecondsBetweenStateChange > 1.0 && averageSecondsBetweenStateChange < 30.0) wavesDetected = true;
	else wavesDetected = false;
	return wavesDetected;
}

void LevelSwitch :: DrawLevelIndicator(int xPos, int yPos, int xOrigin, int yOrigin)
{
	// Indicator modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(float(xPos), float(yPos), 0.0);

	// Black background of ring around level indicator
	GLfloat red = 0.1f;
	GLfloat green = 0.1f;
	GLfloat blue = 0.1f;
	
	glBegin(GL_POLYGON);
	glColor3f(red, green, blue);
	glVertex2f(-1.0f, 5.0f);
	glVertex2f(-2.0f, 5.0f);
	glVertex2f(-2.0f, 4.0f);
	glVertex2f(-4.0f, 3.0f);
	glVertex2f(-4.0f, 2.0f);
	glVertex2f(-5.0f, 2.0f);
	glVertex2f(-5.0f, 1.0f);

	glVertex2f(-5.0f, -1.0f);
	glVertex2f(-5.0f, -2.0f);
	glVertex2f(-4.0f, -2.0f);
	glVertex2f(-4.0f, -3.0f);
	glVertex2f(-2.0f, -4.0f);
	glVertex2f(-2.0f, -5.0f);
	glVertex2f(-1.0f, -5.0f);

	glVertex2f(1.0f, -5.0f);
	glVertex2f(2.0f, -5.0f);
	glVertex2f(2.0f, -4.0f);
	glVertex2f(4.0f, -3.0f);
	glVertex2f(4.0f, -2.0f);
	glVertex2f(5.0f, -2.0f);
	glVertex2f(5.0f, -1.0f);

	glVertex2f(5.0f, 1.0f);
	glVertex2f(5.0f, 2.0f);
	glVertex2f(4.0f, 2.0f);
	glVertex2f(4.0f, 3.0f);
	glVertex2f(2.0f, 4.0f);
	glVertex2f(2.0f, 5.0f);
	glVertex2f(1.0f, 5.0f);
	glEnd();

	// Lit ring of level indicator
	glBegin(GL_POLYGON);
	// Display green if sensor reads on/normal, otherwise yellow for off/abnormal, or red when flagged as failed
	if (GetStatus() == -1)
	{
		// Non functional, red
		red = 0.9f;
		green = 0.2f;
		blue = 0.2f;
	}
	else if (GetStatus() == 0)
	{
		if (wavesDetected)
		{
			// waves detected, red
			red = 0.9f;
			green = 0.2f;
			blue = 0.2f;
		}
		else
		{
			// switch off, yellow
			red = 0.9f;
			green = 0.9f;
			blue = 0.2f;
		}
	}
	else if (GetStatus() == 1)
	{
		if (wavesDetected)
		{
			// waves detected, red
			red = 0.9f;
			green = 0.2f;
			blue = 0.2f;
		}
		else
		{
			// switch on, green
			red = 0.2f;
			green = 0.9f;
			blue = 0.2f;
		}
	}
	glColor3f(red, green, blue);
	glVertex2f(-1.0f, 4.0f);
	glVertex2f(-2.0f, 3.0f);
	glVertex2f(-3.0f, 3.0f);
	glVertex2f(-3.0f, 2.0f);
	glVertex2f(-4.0f, 1.0f);
	glVertex2f(-4.0f, -1.0f);
	glVertex2f(-3.0f, -2.0f);
	glVertex2f(-3.0f, -3.0f);
	glVertex2f(-2.0f, -3.0f);
	glVertex2f(-1.0f, -4.0f);
	glVertex2f(1.0f, -4.0f);
	glVertex2f(2.0f, -3.0f);
	glVertex2f(3.0f, -3.0f);
	glVertex2f(3.0f, -2.0f);
	glVertex2f(4.0f, -1.0f);
	glVertex2f(4.0f, 1.0f);
	glVertex2f(3.0f, 2.0f);
	glVertex2f(3.0f, 3.0f);
	glVertex2f(2.0f, 3.0f);
	glVertex2f(1.0f, 4.0f);
	glEnd();

	// Black background of center level indicator
	red = 0.1f;
	green = 0.1f;
	blue = 0.1f;

	// Level Indicator center outline
	glBegin(GL_POLYGON);
	glColor3f(red, green, blue);
	glVertex2f(3.0f, 1.0f);
	glVertex2f(1.0f, 3.0f);
	glVertex2f(-1.0f, 3.0f);
	glVertex2f(-3.0f, 1.0f);
	glVertex2f(-3.0f, -1.0f);
	glVertex2f(-1.0f, -3.0f);
	glVertex2f(1.0f, -3.0f);
	glVertex2f(3.0f, -1.0f);
	glEnd();

	// Level Indicator center color
	// Display green if sensor reads on/normal, otherwise yellow for off/abnormal, or red when flagged as failed
	if (GetStatus() == 1)
	{
		// Set fill color for normal indication, green
		red = 0.2f;
		green = 0.9f;
		blue = 0.2f;
	}
	else if (GetStatus() == 0)
	{
		// Set fill color for off indication, yellow
		red = 0.9f;
		green = 0.9f;
		blue = 0.2f;
	}
	else
	{
		// Set fill color for fail indication, red
		red = 0.9f;
		green = 0.2f;
		blue = 0.2f;
	}

	// Level Indicator, center
	glBegin(GL_POLYGON);
	glColor3f(red, green, blue);
	glVertex2f(2.0f, 1.0f);
	glVertex2f(1.0f, 2.0f);
	glVertex2f(-1.0f, 2.0f);
	glVertex2f(-2.0f, 1.0f);
	glVertex2f(-2.0f, -1.0f);
	glVertex2f(-1.0f, -2.0f);
	glVertex2f(1.0f, -2.0f);
	glVertex2f(2.0f, -1.0f);
	glEnd();

	glPopMatrix();
}