#include "StdAfx.h"
#include "DistillateSupplyTank.h"

DistillateSupplyTank::DistillateSupplyTank(void)
{
	level = 0.0;
	maxVolume = 0.0;

	upperLiquidTemperatureSensorExists = false;
	lowerLiquidTemperatureSensorExists = false;
	distillateMeteringPumpExists = false;
	ambientTemperatureSensorExists = true;
	pyranometerExists = true;
	
	highAlarmExists = false;
	highAlarmActive = true;
	highAlarmOn = -1;

	lowAlarmExists = false;
	lowAlarmActive = true;
	lowAlarmOn = -1;

	highLevelExists = true;
	highLevelActive = true;
	highLevelOn = -1;

	lowLevelExists = true;
	lowLevelActive = true;
	lowLevelOn = -1;

	draining = true;
	filling = false;
	drainStartTime = 0.0;
	lastDrainTime = 0.0;
	fillStartTime = 0.0;
	lastFillTime = 0.0;

	outputDeviceID = -1;
	outputChannelID = -1;

	highAlarmInputDeviceID = 0;
	highAlarmInputDevicePort = 0;
	highAlarmInputDeviceChannel = 0;

	highLevelInputDeviceID = 0;
	highLevelInputDevicePort = 0;
	highLevelInputDeviceChannel = 4;

	lowLevelInputDeviceID = 0;
	lowLevelInputDevicePort = 0;
	lowLevelInputDeviceChannel = 5;

	lowAlarmInputDeviceID = 0;
	lowAlarmInputDevicePort = 0;
	lowAlarmInputDeviceChannel = 0;

	SetMaxVolume(250.0f);

	SetProcessError(0, "Distillate Supply Tank", "Low Alarm", false);
	SetProcessError(1, "Distillate Supply Tank", "High Alarm", false);

}


DistillateSupplyTank::~DistillateSupplyTank(void)
{
}


void DistillateSupplyTank::InitializeObjects()
{
	SetVesselName("Distillate Supply Tank");
	strstrText << "Distillate Supply Tank";
	strVesselName = strstrText.str();

	if (highAlarmExists && ptrHighAlarmSwitch != NULL)
	{
		ptrHighAlarmSwitch->SetSensorName("Distillate Supply Tank A High Alarm Switch");
		ptrHighAlarmSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrHighAlarmSwitch->SetWhichSwitch(3);
		ptrHighAlarmSwitch->SetInputDevice(highAlarmInputDeviceID, highAlarmInputDevicePort, highAlarmInputDeviceChannel);
		
		// Set display strings
		ptrHighAlarmSwitch->SetSystemDisplayName("Distillate Supply Tank");
		ptrHighAlarmSwitch->SetSensorDisplayName("High Alarm Switch");
		ptrHighAlarmSwitch->SetMaxBufferSize(3000);
		ptrHighAlarmSwitch->SetMaxStateChangeBufferSize(3000);
		ptrHighAlarmSwitch->SetTimeBetweenUpdates(100);
	}

	if (highLevelExists && ptrHighLevelSwitch != NULL)
	{
		ptrHighLevelSwitch->SetSensorName("Distillate Supply Tank High Level Switch");
		ptrHighLevelSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrHighLevelSwitch->SetWhichSwitch(2);
		ptrHighLevelSwitch->SetInputDevice(highLevelInputDeviceID, highLevelInputDevicePort, highLevelInputDeviceChannel);
		
		// Set display strings
		ptrHighLevelSwitch->SetSystemDisplayName("Distillate Supply Tank");
		ptrHighLevelSwitch->SetSensorDisplayName("High Level Switch");
		ptrHighLevelSwitch->SetMaxBufferSize(3000);
		ptrHighLevelSwitch->SetMaxStateChangeBufferSize(3000);
		ptrHighLevelSwitch->SetTimeBetweenUpdates(100);
	}

	if (lowLevelExists && ptrLowLevelSwitch != NULL)
	{
		ptrLowLevelSwitch->SetSensorName("Distillate Supply Tank Low Level Switch");
		ptrLowLevelSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrLowLevelSwitch->SetWhichSwitch(1);
		ptrLowLevelSwitch->SetInputDevice(lowLevelInputDeviceID, lowLevelInputDevicePort, lowLevelInputDeviceChannel);
		
		// Set display strings
		ptrLowLevelSwitch->SetSystemDisplayName("Distillate Supply Tank");
		ptrLowLevelSwitch->SetSensorDisplayName("Low Level Switch");
		ptrLowLevelSwitch->SetMaxBufferSize(3000);
		ptrLowLevelSwitch->SetMaxStateChangeBufferSize(3000);
		ptrLowLevelSwitch->SetTimeBetweenUpdates(100);
	}

	if (lowAlarmExists && ptrLowAlarmSwitch != NULL)
	{
		ptrLowAlarmSwitch->SetSensorName("Distillate Supply Tank Low Alarm Switch");
		ptrLowAlarmSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrLowAlarmSwitch->SetWhichSwitch(0);
		ptrLowAlarmSwitch->SetInputDevice(lowAlarmInputDeviceID, lowAlarmInputDevicePort, lowAlarmInputDeviceChannel);
		
		// Set display strings
		ptrLowAlarmSwitch->SetSystemDisplayName("Distillate Supply Tank");
		ptrLowAlarmSwitch->SetSensorDisplayName("Low Alarm Switch");
		ptrLowAlarmSwitch->SetMaxBufferSize(3000);
		ptrLowAlarmSwitch->SetMaxStateChangeBufferSize(3000);
		ptrLowAlarmSwitch->SetTimeBetweenUpdates(100);
	}

	if (ambientTemperatureSensorExists && ptrAmbientTemperatureProbe != NULL)
	{
		ptrAmbientTemperatureProbe->SetSensorName("Ambient Temperature Probe");
		ptrAmbientTemperatureProbe->SetSensorType("6 inch platinum RTD with 4-20mA TX66 transmitter");
		// Set temperature scale 0=degK, 1=degC, 2=degF
		ptrAmbientTemperatureProbe->SetScaleID(1);
		// Set expected precision of the sensor value in decimals after decimal point
		ptrAmbientTemperatureProbe->SetPrecision(1);
		// Set sensor value with zero (4mA) output
		ptrAmbientTemperatureProbe->SetMinOutput(0.0);
		// Set sensor value with full scale (20mA) output
		ptrAmbientTemperatureProbe->SetMaxOutput(60.0);
		// Set offset based on zero point calibration of sensor
		ptrAmbientTemperatureProbe->SetZeroOffset(0.0);
		// Set deviceID, PortID (none), and channelID for data input device to update data from
		ptrAmbientTemperatureProbe->SetInputDevice(0, -1, 14);

		// Set display strings
		ptrAmbientTemperatureProbe->SetSystemDisplayName("Pyranometer Tower");
		ptrAmbientTemperatureProbe->SetSensorDisplayName("Ambient Temp");
		ptrAmbientTemperatureProbe->SetSensorUnits("deg C");
		ptrAmbientTemperatureProbe->SetMaxBufferSize(3000);
	}

	if (pyranometerExists && ptrPyranometer != NULL)
	{
		ptrPyranometer->SetSensorName("Pyranometer");
		ptrPyranometer->SetSensorType("Eppley PSP pyranometer with Kipp & Zonene 4-20mA transmitter");
		// Set pyranometer parameters
		ptrPyranometer->SetSensitivity(0.00000824);
		ptrPyranometer->SetSensorRange(0.0, 1600.0);
		// Set expected precision of the sensor value in decimals after decimal point
		ptrPyranometer->SetPrecision(1);
		// Set sensor value with zero (4mA) output in Watts per square meter
		ptrPyranometer->SetMinOutput(0.0);
		// Set sensor value with full scale (20mA) output in Watts per square meter
		ptrPyranometer->SetMaxOutput(1600.0);
		// Set offset based on zero point calibration of sensor
		ptrPyranometer->SetZeroOffset(0.0);
		// Set deviceID, PortID (none), and channelID for data input device to update data from
		ptrPyranometer->SetInputDevice(0, -1, 15);

		// Set display strings
		ptrPyranometer->SetSystemDisplayName("Pyranometer Tower");
		ptrPyranometer->SetSensorDisplayName("Insolation");
		ptrPyranometer->SetSensorUnits("W/m2");
		ptrAmbientTemperatureProbe->SetMaxBufferSize(3000);
	}
}

void DistillateSupplyTank::TankControlLogic()
{
	try
	{
		// Run level control logic in base class
		if (!LiquidTank :: UpdateLevelState())  throw ProgramException("Call to base class UpdateLevelState() failed for " +  GetTankNameStr() + "\n");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "DistillateSupplyTank::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	// Update ambient temperature input
	try
	{
		if (ptrAmbientTemperatureProbe != NULL)
		{
			if (!ptrAmbientTemperatureProbe->UpdateSensorValueSmoothed()) throw ProgramException("Ambient Probe UpdateSensorValue() failed in " +  GetTankNameStr());
			if (GetLogFileOutputPtr() != NULL) GetLogFileOutputPtr()->SetDataItem(13, "Ambient Temperature", ptrAmbientTemperatureProbe->GetSensorValue());
		}
		else throw ProgramException("Null pointer to Ambient Temperature Probe in " +  GetTankNameStr());
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(2, "DistillateSupplyTank::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	// Update pyranometer input
	try
	{
		if (ptrPyranometer != NULL)
		{
			if (!ptrPyranometer->UpdateSensorValueSmoothed()) throw ProgramException("Pyranometer UpdateSensorValue() failed in " +  GetTankNameStr());
			if (GetLogFileOutputPtr() != NULL) GetLogFileOutputPtr()->SetDataItem(14, "Pyranometer", ptrPyranometer->GetSensorValue());
		}
		else throw ProgramException("Null pointer to Pyranometer in " +  GetTankNameStr());
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(2, "DistillateSupplyTank::TankControlLogic", pe.GetExceptionDescription(), true);
	}
}

// Efficient routine to draw a circle in OpenGL
void DistillateSupplyTank::DrawFilledCircle(float cx, float cy, float r, int numSegments) 
{ 
	float theta = 2 * 3.1415926f / float(numSegments); 
	float tangetial_factor = tanf(theta);//calculate the tangential factor 

	float radial_factor = cosf(theta);//calculate the radial factor 
	
	float x = r;//we start at angle = 0 

	float y = 0; 
    
	glBegin(GL_POLYGON); 
	for(int ii = 0; ii < numSegments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//calculate the tangential vector, the radial vector is (x, y) 
		//to get the tangential vector, flip those coordinates and negate one of them 

		float tx = -y; 
		float ty = x; 
        
		//add the tangential vector 

		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
        
		//correct using the radial factor 

		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}

void DistillateSupplyTank::Display2D(GLint nFontList, float xOrigin, float yOrigin)
{
	// String and string stream object to format data to string
	std::string strData;
	std::ostringstream strstrData;
	int fontHeight = 15;
	GLfloat hueOffset = 0.5f;
	GLfloat brightness = 0.0f;
	GLfloat red = 0.0f;
	GLfloat green = 0.0f;
	GLfloat blue = 0.0f;

	// Liquid Tank modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(xOrigin, yOrigin, 0.0);

	// Draw Distillate Tank outlet pipes
	// Draw upward pipe outline
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex2f(-2.0f, 170.0f);
	glVertex2f(2.0f, 170.0f);
	glVertex2f(2.0f, 30.0f);
	glVertex2f(-2.0f, 30.0f);
	glEnd();

	// Draw horizontal pipe outline
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex2f(-840.0f, 170.0f);
	glVertex2f(2.0f, 170.0f);
	glVertex2f(2.0f, 166.0f);
	glVertex2f(-840.0f, 166.0f);
	glEnd();

	// Draw left downward pipe outline
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex2f(-840.0f, 170.0f);
	glVertex2f(-836.0f, 170.0f);
	glVertex2f(-836.0f, 160.0f);
	glVertex2f(-840.0f, 160.0f);
	glEnd();

	// Left Arrowhead outline
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex2f(-830.0f, 160.0f);
	glVertex2f(-838.0f, 150.0f);
	glVertex2f(-846.0f, 160.0f);
	glEnd();

	// Draw right downward pipe outline
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex2f(-440.0f, 170.0f);
	glVertex2f(-436.0f, 170.0f);
	glVertex2f(-436.0f, 155.0f);
	glVertex2f(-440.0f, 155.0f);
	glEnd();

	// Right Arrowhead outline
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex2f(-430.0f, 160.0f);
	glVertex2f(-438.0f, 150.0f);
	glVertex2f(-446.0f, 160.0f);
	glEnd();

	// Draw upward pipe fill
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.9f);
	glVertex2f(-1.0f, 170.0f);
	glVertex2f(1.0f, 170.0f);
	glVertex2f(1.0f, 30.0f);
	glVertex2f(-1.0f, 30.0f);
	glEnd();

	// Draw horizontal pipe fill
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.9f);
	glVertex2f(-840.0f, 169.0f);
	glVertex2f(1.0f, 169.0f);
	glVertex2f(1.0f, 167.0f);
	glVertex2f(-840.0f, 167.0f);
	glEnd();

	// Draw left downward pipe fill
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.9f);
	glVertex2f(-839.0f, 169.0f);
	glVertex2f(-837.0f, 169.0f);
	glVertex2f(-837.0f, 159.0f);
	glVertex2f(-839.0f, 159.0f);
	glEnd();

	// Left Arrowhead fill
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.9f);
	glVertex2f(-831.0f, 159.0f);
	glVertex2f(-838.0f, 151.0f);
	glVertex2f(-845.0f, 159.0f);
	glEnd();

	// Draw right downward pipe fill
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.9f);
	glVertex2f(-439.0f, 169.0f);
	glVertex2f(-437.0f, 169.0f);
	glVertex2f(-437.0f, 159.0f);
	glVertex2f(-439.0f, 159.0f);
	glEnd();

	// Right Arrowhead fill
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.3f, 0.9f);
	glVertex2f(-431.0f, 159.0f);
	glVertex2f(-438.0f, 151.0f);
	glVertex2f(-445.0f, 159.0f);
	glEnd();

	// Draw end on tank outline
	glColor3f(0.2f, 0.2f, 0.2f);
	DrawFilledCircle(0.0f, -30.0f, 62.0f, 32);
	// Draw end on tank fill
	glColor3f(0.3f, 0.3f, 0.9f);
	DrawFilledCircle(0.0f, -30.0f, 60.0f, 32);

	if (highLevelExists && highLevelActive)
	{
		ptrHighLevelSwitch->DrawLevelIndicator(0, -10, int(xOrigin), int(yOrigin));
	}
	if (lowLevelExists && lowLevelActive)
	{
		ptrLowLevelSwitch->DrawLevelIndicator(0, -50, int(xOrigin), int(yOrigin));
	}

	DrawFillDataBox2D(nFontList, -100, -150, int(xOrigin), int(yOrigin), fill);

	// Draw ambient temperature sensor icon and data display
	if (ambientTemperatureSensorExists && ptrAmbientTemperatureProbe != NULL)
	{
		glPushMatrix();
		glTranslatef(30, 200, 0.0);

		// Temperature probe shadow
		glBegin(GL_POLYGON);
		glColor3f(0.1f, 0.1f, 0.1f);
		glVertex2f(-3.0f, 20.0f);
		glVertex2f(3.0f, 20.0f);
		glVertex2f(3.0f, -62.0f);
		glVertex2f(3.0f, -67.0f);
		glVertex2f(-2.0f, -67.0f);
		glVertex2f(-2.0f, -62.0f);
		glEnd();

		// Temperature probe
		glBegin(GL_POLYGON);
		glColor3f(0.7f, 0.7f, 0.7f);
		glVertex2f(-2.0f, 20.0f);
		glVertex2f(2.0f, 20.0f);
		glVertex2f(2.0f, -60.0f);
		glVertex2f(0.5f, -65.0f);
		glVertex2f(-0.5f, -65.0f);
		glVertex2f(-2.0f, -60.0f);
		glEnd();

		// Temperature probe highlight
		glBegin(GL_POLYGON);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(-0.5f, 20.0f);
		glVertex2f(2.5f, 20.0f);
		glVertex2f(2.5f, -60.0f);
		glVertex2f(1.0f, -65.0f);
		glVertex2f(0.0f, -65.0f);
		glVertex2f(-0.5f, -60.0f);
		glEnd();

		glPopMatrix();

		ptrAmbientTemperatureProbe->DrawDataBox2D(nFontList, -50, 220, int(xOrigin), int(yOrigin));
	}

	if (pyranometerExists && ptrPyranometer != NULL)
	{
		// Draw pyranometer display
		ptrPyranometer->DrawDataBox2D(nFontList, -50, 280, int(xOrigin), int(yOrigin));
	}

	LiquidTank::DisplayProcessErrors2D(nFontList, xOrigin, yOrigin);
	ptrProgramExceptions->DisplayProgramExceptions(nFontList, 0, 0);
	ptrHighLevelSwitch->DisplaySensorExceptions(nFontList, 0, 180);
	ptrLowLevelSwitch->DisplaySensorExceptions(nFontList, 0, 120);
	glPopMatrix();
}
