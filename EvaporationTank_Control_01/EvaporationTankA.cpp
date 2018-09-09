#include "StdAfx.h"
#include "EvaporationTankA.h"

EvaporationTankA::EvaporationTankA(void)
{
	ptrDistillateSupplyTank = NULL;
	upperLiquidTemperature = 0.0;
	level = 0.0;
	maxVolume = 300.0; // tank capacity in gallons
	// Geometric estimate of tank surface area = 12.23 square feet = 1.1362 square meters
	// Volumetric displacement estimate of tank surface area = 11.32 square feet = 1.0517 square meters
	fillVolumeSwitch = 2.81f; // liters between low and high level switches (measured value)
	SetVolumeSwitch(fillVolumeSwitch);
	surfaceArea = 1.0517; // square meters at normal level
	fillVolumeSensor = 1.0f*(float)surfaceArea; // liters between low and high sensor set points (using difference in millimeters between level sensor set points)
	SetVolumeSensor(fillVolumeSensor);

	upperLiquidTemperatureSensorExists = true;
	lowerLiquidTemperatureSensorExists = true;
	distillateMeteringPumpExists = true;
	
	continuousLevelSensorExists = true;
	continuousLevelSensorControlling = true;

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

	outputDeviceID = 0;
	outputChannelID = 0;

	highAlarmInputDeviceID = 0;
	highAlarmInputDevicePort = 0;
	highAlarmInputDeviceChannel = 0;

	highLevelInputDeviceID = 0;
	highLevelInputDevicePort = 0;
	highLevelInputDeviceChannel = 1;

	lowLevelInputDeviceID = 0;
	lowLevelInputDevicePort = 0;
	lowLevelInputDeviceChannel = 0;

	lowAlarmInputDeviceID = 0;
	lowAlarmInputDevicePort = 0;
	lowAlarmInputDeviceChannel = 0;

	SetProcessError(0, "Evaporation Tank A", "", false);
	SetProcessError(1, "Evaporation Tank A", "", false);
	SetProcessError(2, "Evaporation Tank A", "", false);
}

EvaporationTankA::~EvaporationTankA(void)
{
}

void EvaporationTankA::InitializeObjects()
{
	SetVesselName("Evaporation Tank A");
	strstrText << "Evaporation Tank A";
	strVesselName = strstrText.str();
	SetMaxFillRateBufferSize(3000);

	if (highAlarmExists && ptrHighAlarmSwitch != NULL)
	{
		ptrHighAlarmSwitch->SetSensorName("Evaporation Tank A High Alarm Switch");
		ptrHighAlarmSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrHighAlarmSwitch->SetWhichSwitch(3);
		ptrHighAlarmSwitch->SetInputDevice(highAlarmInputDeviceID, highAlarmInputDevicePort, highAlarmInputDeviceChannel);
		
		// Set display strings
		ptrHighAlarmSwitch->SetSystemDisplayName("Evaporation Tank A");
		ptrHighAlarmSwitch->SetSensorDisplayName("High Alarm Switch");
		ptrHighAlarmSwitch->SetMaxBufferSize(3000);
		ptrHighAlarmSwitch->SetMaxStateChangeBufferSize(30);
		ptrHighAlarmSwitch->SetTimeBetweenUpdates(100);
	}

	if (highLevelExists && ptrHighLevelSwitch != NULL)
	{
		ptrHighLevelSwitch->SetSensorName("Evaporation Tank A High Level Switch");
		ptrHighLevelSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrHighLevelSwitch->SetWhichSwitch(2);
		ptrHighLevelSwitch->SetInputDevice(highLevelInputDeviceID, highLevelInputDevicePort, highLevelInputDeviceChannel);
		
		// Set display strings
		ptrHighLevelSwitch->SetSystemDisplayName("Evaporation Tank A");
		ptrHighLevelSwitch->SetSensorDisplayName("High Level Switch");
		ptrHighLevelSwitch->SetMaxBufferSize(3000);
		ptrHighLevelSwitch->SetMaxStateChangeBufferSize(30);
		ptrHighLevelSwitch->SetTimeBetweenUpdates(100);
	}

	if (lowLevelExists && ptrLowLevelSwitch != NULL)
	{
		ptrLowLevelSwitch->SetSensorName("Evaporation Tank A Low Level Switch");
		ptrLowLevelSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrLowLevelSwitch->SetWhichSwitch(1);
		ptrLowLevelSwitch->SetInputDevice(lowLevelInputDeviceID, lowLevelInputDevicePort, lowLevelInputDeviceChannel);
		
		// Set display strings
		ptrLowLevelSwitch->SetSystemDisplayName("Evaporation Tank A");
		ptrLowLevelSwitch->SetSensorDisplayName("Low Level Switch");
		ptrLowLevelSwitch->SetMaxBufferSize(3000);
		ptrLowLevelSwitch->SetMaxStateChangeBufferSize(30);
		ptrLowLevelSwitch->SetTimeBetweenUpdates(100);
	}

	if (lowAlarmExists && ptrLowAlarmSwitch != NULL)
	{
		ptrLowAlarmSwitch->SetSensorName("Evaporation Tank A Low Alarm Switch");
		ptrLowAlarmSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrLowAlarmSwitch->SetWhichSwitch(0);
		ptrLowAlarmSwitch->SetInputDevice(lowAlarmInputDeviceID, lowAlarmInputDevicePort, lowAlarmInputDeviceChannel);
		
		// Set display strings
		ptrLowAlarmSwitch->SetSystemDisplayName("Evaporation Tank A");
		ptrLowAlarmSwitch->SetSensorDisplayName("Low Alarm Switch");
		ptrLowAlarmSwitch->SetMaxBufferSize(3000);
		ptrLowAlarmSwitch->SetMaxStateChangeBufferSize(30);
		ptrLowAlarmSwitch->SetTimeBetweenUpdates(100);
	}

	if (continuousLevelSensorExists && ptrTankLevelSensor != NULL)
	{
		ptrTankLevelSensor->SetSensorName("Tank A Level Sensor");
		ptrTankLevelSensor->SetSensorType("Automation Products Dynatrol vibrating plate proportional level sensor with 4-20 mA transmitter");
		// Set expected precision of the sensor value in decimals after decimal point
		ptrTankLevelSensor->SetPrecision(1);
		// Set reverse acting flag for this sensor
		ptrTankLevelSensor->SetReverseActing(true);
		// Set sensor value with zero (4mA) output
		ptrTankLevelSensor->SetMinOutput(-9.0);
		// Set sensor value with full scale (20mA) output
		ptrTankLevelSensor->SetMaxOutput(9.0);
		// Set operating level target range
		ptrTankLevelSensor->SetLevelRange(-5.0, -4.0);
		// Set operating level exceeds range
		ptrTankLevelSensor->SetLevelExceedsRange(-6.0, -3.0);
		// Set offset based on zero point calibration of sensor
		ptrTankLevelSensor->SetZeroOffset(0.0);
		// Set deviceID, PortID (none), and channelID for data input device to update data from
		ptrTankLevelSensor->SetInputDevice(0, -1, 12);

		// Set display strings
		ptrTankLevelSensor->SetSystemDisplayName("Tank A Level");
		ptrTankLevelSensor->SetSensorDisplayName("Level");
		ptrTankLevelSensor->SetSensorUnits("mm");
		ptrTankLevelSensor->SetMaxBufferSize(3000);
	}

	if (upperLiquidTemperatureSensorExists && ptrUpperTemperatureProbe != NULL)
	{
		ptrUpperTemperatureProbe->SetSensorName("Evaporation Tank A Upper Temperature Probe");
		ptrUpperTemperatureProbe->SetSensorType("9 inch platinum RTD with 4-20mA TX69 transmitter");
		// Set temperature scale 0=degK, 1=degC, 2=degF
		ptrUpperTemperatureProbe->SetScaleID(1);
		// Set expected precision of the sensor value in decimals after decimal point
		ptrUpperTemperatureProbe->SetPrecision(1);
		// Set sensor value with zero (4mA) output
		ptrUpperTemperatureProbe->SetMinOutput(0.0);
		// Set sensor value with full scale (20mA) output
		ptrUpperTemperatureProbe->SetMaxOutput(100.0);
		// Set offset based on zero point calibration of sensor
		ptrUpperTemperatureProbe->SetZeroOffset(0.0);
		// Set deviceID, PortID (none), and channelID for data input device to update data from
		ptrUpperTemperatureProbe->SetInputDevice(0, -1, 9);

		// Set display strings
		ptrUpperTemperatureProbe->SetSystemDisplayName("Evaporation Tank A");
		ptrUpperTemperatureProbe->SetSensorDisplayName("Upper Temp.");
		ptrUpperTemperatureProbe->SetSensorUnits("deg C");
		ptrUpperTemperatureProbe->SetMaxBufferSize(3000);
	}

	if (lowerLiquidTemperatureSensorExists && ptrLowerTemperatureProbe != NULL)
	{
		ptrLowerTemperatureProbe->SetSensorName("Evaporation Tank A Lower Temperature Probe");
		ptrLowerTemperatureProbe->SetSensorType("9 inch platinum RTD with 4-20mA TX69 transmitter");
		// Set temperature scale 0=degK, 1=degC, 2=degF
		ptrLowerTemperatureProbe->SetScaleID(1);
		// Set expected precision of the sensor value in decimals after decimal point
		ptrLowerTemperatureProbe->SetPrecision(1);
		// Set sensor value with zero (4mA) output
		ptrLowerTemperatureProbe->SetMinOutput(0.0);
		// Set sensor value with full scale (20mA) output
		ptrLowerTemperatureProbe->SetMaxOutput(100.0);
		// Set offset based on zero point calibration of sensor
		ptrLowerTemperatureProbe->SetZeroOffset(0.0);
		// Set deviceID, PortID (none), and channelID for data input device to update data from
		ptrLowerTemperatureProbe->SetInputDevice(0, -1, 8);

		// Set display strings
		ptrLowerTemperatureProbe->SetSystemDisplayName("Evaporation Tank A");
		ptrLowerTemperatureProbe->SetSensorDisplayName("Lower Temp.");
		ptrLowerTemperatureProbe->SetSensorUnits("deg C");
		ptrLowerTemperatureProbe->SetMaxBufferSize(3000);
	}

	if  (distillateMeteringPumpExists && ptrDistillateMeteringPump != NULL)
	{ 
		ptrDistillateMeteringPump->SetPumpName("Distillate Make-up Pump to Evaporation Tank A");
		ptrDistillateMeteringPump->SetPumpType("LMI etering Pump with 4-20 mA converter");
		ptrDistillateMeteringPump->SetOutputDeviceID(outputDeviceID); // not connected
		ptrDistillateMeteringPump->SetOutputChannelID(outputChannelID); // not connected}
		ptrDistillateMeteringPump->SetMaxPumpVolumeDeliveredPerMinute(0.03604); // measured value at 100% speed and stroke in liters/min (measured 23 Oct 2014)

		// Set display strings
		ptrDistillateMeteringPump->SetSystemDisplayName("Evaporation Tank A");
		ptrDistillateMeteringPump->SetPumpDisplayName("Distillate Pump");
		ptrDistillateMeteringPump->SetPumpUnits("%");

		// Set up rate memory buffers
		ptrDistillateMeteringPump->SetMaxBufferSize(3000);
		ptrDistillateMeteringPump->SetMaxRateBufferSize(3000);
	}
}

void EvaporationTankA::DrawPumpSpeedChart(GLint nFontList)
{
	ptrDistillateMeteringPump->DrawChartRecorderStrip2D(nFontList, 10, 560, 850, 60, 5, 40, false, true);
//	ptrLowerTemperatureProbe->DrawChartRecorderStrip2D(nFontList, 10, 380, 850, 60, 5, 40, false, true);
}

// Set pointer to Distillate Supply Tank for control logic
bool EvaporationTankA::SetDistillateSupplyTankPtr(DistillateSupplyTank* ptr)
{
	if (ptr != NULL)
	{
		ptrDistillateSupplyTank = ptr;
		return true;
	}
	else return false;
}

void EvaporationTankA::TankControlLogic()
{
	try
	{
		// Read availability of distillate from supply tank
		if (ptrDistillateSupplyTank != NULL)
		{
			if (ptrDistillateSupplyTank->GetLowLevelOn() == 1) fillSourceAvailable = true;
			else if (ptrDistillateSupplyTank->GetLowLevelOn() == 0) fillSourceAvailable = false;
			else throw ProgramException("Distillate Supply Tank low level value is not set for " +  GetTankNameStr());
		}
		else throw ProgramException("Null pointer to Distillate Supply Tank for " +  GetTankNameStr());
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(7, "EvaporationTankA::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	try
	{
		// Run level control logic in base class
		if (!LiquidTank :: TankControlLogic()) throw ProgramException("Call to base class TankControlLogic failed for " +  GetTankNameStr());
		else if (GetLogFileOutputPtr() != NULL)
		{
			GetLogFileOutputPtr()->SetDataItem(5, "Evaporation Tank A Level", ptrTankLevelSensor->GetSensorValue());
			if (LiquidTank :: GetCleaning()) GetLogFileOutputPtr()->SetDataItem(6, "Evaporation Tank A Status", "cleaning");
			else GetLogFileOutputPtr()->SetDataItem(6, "Evaporation Tank A Status", "normal");
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(8, "EvaporationTankA::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	// Update upper layer liquid temperature input
	try
	{
		if (ptrUpperTemperatureProbe != NULL)
		{
			if (!ptrUpperTemperatureProbe->UpdateSensorValueSmoothed()) throw ProgramException("Upper Temperature Probe UpdateSensorValue() failed in " +  GetTankNameStr());
			if (GetLogFileOutputPtr() != NULL) GetLogFileOutputPtr()->SetDataItem(1, "Evaporation Tank A Upper Temperature Probe", ptrUpperTemperatureProbe->GetSensorValue());
		}
		else throw ProgramException("Null pointer to Upper Temperature Probe in " +  GetTankNameStr());
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(9, "EvaporationTankA::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	// Update lower layer liquid temperature input
	try
	{
		if (ptrLowerTemperatureProbe != NULL)
		{
			if (!ptrLowerTemperatureProbe->UpdateSensorValueSmoothed()) throw ProgramException("Lower Temperature Probe UpdateSensorValue() failed in " +  GetTankNameStr());
			if (GetLogFileOutputPtr() != NULL) GetLogFileOutputPtr()->SetDataItem(2, "Evaporation Tank A Lower Temperature Probe", ptrLowerTemperatureProbe->GetSensorValue());
		}
		else throw ProgramException("Null pointer to Lower Temperature Probe in " +  GetTankNameStr());
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(10, "EvaporationTankA::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	// Update distillate metering pump chart display
	try
	{
		if (ptrDistillateMeteringPump != NULL)
		{
			if (!ptrDistillateMeteringPump->UpdatePumpSpeed()) throw ProgramException("Distillate Metering Pump UpdatePumpSpeed() failed in " +  GetTankNameStr());
			if (GetLogFileOutputPtr() != NULL) GetLogFileOutputPtr()->SetDataItem(3, "Evaporation Tank A Distillate Pump Speed", ptrDistillateMeteringPump->GetPumpSpeed());
			if (GetLogFileOutputPtr() != NULL) GetLogFileOutputPtr()->SetDataItem(4, "Evaporation Tank A Distillate Pump Last 24 hr Delivery", ptrDistillateMeteringPump->GetLast24HourPumpDelivery());
		}
		else throw ProgramException("Null pointer to Distillate Metering Pump in " +  GetTankNameStr());
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(11, "EvaporationTankA::TankControlLogic", pe.GetExceptionDescription(), true);
	}
}