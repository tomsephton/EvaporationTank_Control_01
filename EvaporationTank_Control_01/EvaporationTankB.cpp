#include "StdAfx.h"
#include "EvaporationTankB.h"

EvaporationTankB::EvaporationTankB(void)
{
	ptrDistillateSupplyTank = NULL;
	upperLiquidTemperature = 0.0;
	level = 0.0;
	maxVolume = 300.0; // tank capacity in gallons
	// Geometric estimate of tank surface area = 12.23 square feet = 1.1362 square meters
	// Volumetric displacement estimate of tank surface area = 11.32 square feet = 1.0517 square meters
	fillVolumeSwitch = 2.47f; // liters between low and high level switches (measured value)
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
	outputChannelID = 1;

	highAlarmInputDeviceID = 0;
	highAlarmInputDevicePort = 0;
	highAlarmInputDeviceChannel = 0;

	highLevelInputDeviceID = 0;
	highLevelInputDevicePort = 0;
	highLevelInputDeviceChannel = 3;

	lowLevelInputDeviceID = 0;
	lowLevelInputDevicePort = 0;
	lowLevelInputDeviceChannel = 2;

	lowAlarmInputDeviceID = 0;
	lowAlarmInputDevicePort = 0;
	lowAlarmInputDeviceChannel = 0;

	SetProcessError(0, "Evaporation Tank B", "", false);
	SetProcessError(1, "Evaporation Tank B", "", false);
	SetProcessError(2, "Evaporation Tank B", "", false);
}

EvaporationTankB::~EvaporationTankB(void)
{
}

void EvaporationTankB::InitializeObjects()
{
	SetVesselName("Evaporation Tank B");
	strstrText << "Evaporation Tank B";
	strVesselName = strstrText.str();
	SetMaxFillRateBufferSize(3000);

	if (highAlarmExists && ptrHighAlarmSwitch != NULL)
	{
		ptrHighAlarmSwitch->SetSensorName("Evaporation Tank B High Alarm Switch");
		ptrHighAlarmSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrHighAlarmSwitch->SetWhichSwitch(3);
		ptrHighAlarmSwitch->SetInputDevice(highAlarmInputDeviceID, highAlarmInputDevicePort, highAlarmInputDeviceChannel);
		
		// Set display strings
		ptrHighAlarmSwitch->SetSystemDisplayName("Evaporation Tank B");
		ptrHighAlarmSwitch->SetSensorDisplayName("High Alarm Switch");
		ptrHighAlarmSwitch->SetMaxBufferSize(3000);
		ptrHighAlarmSwitch->SetMaxStateChangeBufferSize(30);
		ptrHighAlarmSwitch->SetTimeBetweenUpdates(100);
	}

	if (highLevelExists && ptrHighLevelSwitch != NULL)
	{
		ptrHighLevelSwitch->SetSensorName("Evaporation Tank B High Level Switch");
		ptrHighLevelSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrHighLevelSwitch->SetWhichSwitch(2);
		ptrHighLevelSwitch->SetInputDevice(highLevelInputDeviceID, highLevelInputDevicePort, highLevelInputDeviceChannel);
		
		// Set display strings
		ptrHighLevelSwitch->SetSystemDisplayName("Evaporation Tank B");
		ptrHighLevelSwitch->SetSensorDisplayName("High Level Switch");
		ptrHighLevelSwitch->SetMaxBufferSize(3000);
		ptrHighLevelSwitch->SetMaxStateChangeBufferSize(30);
		ptrHighLevelSwitch->SetTimeBetweenUpdates(100);
	}

	if (lowLevelExists && ptrLowLevelSwitch != NULL)
	{
		ptrLowLevelSwitch->SetSensorName("Evaporation Tank B Low Level Switch");
		ptrLowLevelSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrLowLevelSwitch->SetWhichSwitch(1);
		ptrLowLevelSwitch->SetInputDevice(lowLevelInputDeviceID, lowLevelInputDevicePort, lowLevelInputDeviceChannel);
		
		// Set display strings
		ptrLowLevelSwitch->SetSystemDisplayName("Evaporation Tank B");
		ptrLowLevelSwitch->SetSensorDisplayName("Low Level Switch");
		ptrLowLevelSwitch->SetMaxBufferSize(3000);
		ptrLowLevelSwitch->SetMaxStateChangeBufferSize(30);
		ptrLowLevelSwitch->SetTimeBetweenUpdates(100);
	}

	if (lowAlarmExists && ptrLowAlarmSwitch != NULL)
	{
		ptrLowAlarmSwitch->SetSensorName("Evaporation Tank B Low Alarm Switch");
		ptrLowAlarmSwitch->SetSensorType("Rosemount 2110 Tuning Fork Level Switch");
		ptrLowAlarmSwitch->SetWhichSwitch(0);
		ptrLowAlarmSwitch->SetInputDevice(lowAlarmInputDeviceID, lowAlarmInputDevicePort, lowAlarmInputDeviceChannel);
		
		// Set display strings
		ptrLowAlarmSwitch->SetSystemDisplayName("Evaporation Tank B");
		ptrLowAlarmSwitch->SetSensorDisplayName("Low Alarm Switch");
		ptrLowAlarmSwitch->SetMaxBufferSize(3000);
		ptrLowAlarmSwitch->SetMaxStateChangeBufferSize(30);
		ptrLowAlarmSwitch->SetTimeBetweenUpdates(100);
	}

	if (ptrTankLevelSensor != NULL)
	{
		ptrTankLevelSensor->SetSensorName("Tank B Level Sensor");
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
		ptrTankLevelSensor->SetInputDevice(0, -1, 13);

		// Set display strings
		ptrTankLevelSensor->SetSystemDisplayName("Tank B Level");
		ptrTankLevelSensor->SetSensorDisplayName("Level");
		ptrTankLevelSensor->SetSensorUnits("mm");
		ptrTankLevelSensor->SetMaxBufferSize(3000);
	}

	if (upperLiquidTemperatureSensorExists && ptrUpperTemperatureProbe != NULL)
	{
		ptrUpperTemperatureProbe->SetSensorName("Evaporation Tank B Upper Temperature Probe");
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
		ptrUpperTemperatureProbe->SetInputDevice(0, -1, 11);

		// Set display strings
		ptrUpperTemperatureProbe->SetSystemDisplayName("Evaporation Tank B");
		ptrUpperTemperatureProbe->SetSensorDisplayName("Upper Temp.");
		ptrUpperTemperatureProbe->SetSensorUnits("deg C");
		ptrUpperTemperatureProbe->SetMaxBufferSize(3000);
	}

	if (lowerLiquidTemperatureSensorExists && ptrLowerTemperatureProbe != NULL)
	{
		ptrLowerTemperatureProbe->SetSensorName("Evaporation Tank B Lower Temperature Probe");
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
		ptrLowerTemperatureProbe->SetInputDevice(0, -1, 10);

		// Set display strings
		ptrLowerTemperatureProbe->SetSystemDisplayName("Evaporation Tank B");
		ptrLowerTemperatureProbe->SetSensorDisplayName("Lower Temp.");
		ptrLowerTemperatureProbe->SetSensorUnits("deg C");
		ptrLowerTemperatureProbe->SetMaxBufferSize(3000);
	}

	if  (distillateMeteringPumpExists && ptrDistillateMeteringPump != NULL)
	{ 
		ptrDistillateMeteringPump->SetPumpName("Distillate Make-up Pump to Evaporation Tank B");
		ptrDistillateMeteringPump->SetPumpType("LMI etering Pump with 4-20 mA converter");
		ptrDistillateMeteringPump->SetOutputDeviceID(outputDeviceID); // not connected
		ptrDistillateMeteringPump->SetOutputChannelID(outputChannelID); // not connected}
		ptrDistillateMeteringPump->SetMaxPumpVolumeDeliveredPerMinute(0.03668); // measured value at 100% speed and stroke in liters/min (measured 23 Oct 2014)

		// Set display strings
		ptrDistillateMeteringPump->SetSystemDisplayName("Evaporation Tank B");
		ptrDistillateMeteringPump->SetPumpDisplayName("Distillate Pump");
		ptrDistillateMeteringPump->SetPumpUnits("%");

		// Set up rate memory buffers
		ptrDistillateMeteringPump->SetMaxBufferSize(3000);
		ptrDistillateMeteringPump->SetMaxRateBufferSize(3000);
	}
}

void EvaporationTankB::DrawPumpSpeedChart(GLint nFontList)
{
	ptrDistillateMeteringPump->DrawChartRecorderStrip2D(nFontList, 10, 480, 850, 60, 5, 40, false, true);
//	ptrLowerTemperatureProbe->DrawChartRecorderStrip2D(nFontList, 10, 380, 850, 60, 5, 40, false, true);
}

// Set pointer to Distillate Supply Tank for control logic
bool EvaporationTankB::SetDistillateSupplyTankPtr(DistillateSupplyTank* ptr)
{
	if (ptr != NULL)
	{
		ptrDistillateSupplyTank = ptr;
		return true;
	}
	else return false;
}

void EvaporationTankB::TankControlLogic()
{
	try
	{
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
		ptrProgramExceptions->SetProgramException(7, "EvaporationTankB::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	try
	{
		// Run level control logic in base class
		if (!LiquidTank :: TankControlLogic()) throw ProgramException("Call to base class TankControlLogic failed for " +  GetTankNameStr());
		else if (GetLogFileOutputPtr() != NULL)
		{
			GetLogFileOutputPtr()->SetDataItem(11, "Evaporation Tank B Level", ptrTankLevelSensor->GetSensorValue());
			if (LiquidTank :: GetCleaning()) GetLogFileOutputPtr()->SetDataItem(12, "Evaporation Tank B Status", "cleaning");
			else GetLogFileOutputPtr()->SetDataItem(12, "Evaporation Tank B Status", "normal");
		}
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(8, "EvaporationTankB::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	// Update upper layer liquid temperature input
	try
	{
		if (ptrUpperTemperatureProbe != NULL)
		{
			if (!ptrUpperTemperatureProbe->UpdateSensorValueSmoothed()) throw ProgramException("Upper Temperature Probe UpdateSensorValue() failed in " +  GetTankNameStr());
			if (GetLogFileOutputPtr() != NULL) GetLogFileOutputPtr()->SetDataItem(7, "Evaporation Tank B Upper Temperature Probe", ptrUpperTemperatureProbe->GetSensorValue());
		}
		else throw ProgramException("Null pointer to Upper Temperature Probe in " +  GetTankNameStr());
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(9, "EvaporationTankB::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	// Update lower layer liquid temperature input
	try
	{
		if (ptrLowerTemperatureProbe != NULL)
		{
			if (!ptrLowerTemperatureProbe->UpdateSensorValueSmoothed()) throw ProgramException("Lower Temperature Probe UpdateSensorValue() failed in " +  GetTankNameStr());
			if (GetLogFileOutputPtr() != NULL) GetLogFileOutputPtr()->SetDataItem(8, "Evaporation Tank B Lower Temperature Probe", ptrLowerTemperatureProbe->GetSensorValue());
		}
		else throw ProgramException("Null pointer to Lower Temperature Probe in " +  GetTankNameStr());
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(10, "EvaporationTankB::TankControlLogic", pe.GetExceptionDescription(), true);
	}

	// Update distillate metering pump chart display
	try
	{
		if (ptrDistillateMeteringPump != NULL)
		{
			if (!ptrDistillateMeteringPump->UpdatePumpSpeed()) throw ProgramException("Distillate Metering Pump UpdatePumpSpeed() failed in " +  GetTankNameStr());
			if (GetLogFileOutputPtr() != NULL) GetLogFileOutputPtr()->SetDataItem(9, "Evaporation Tank B Distillate Pump Speed", ptrDistillateMeteringPump->GetPumpSpeed());
			if (GetLogFileOutputPtr() != NULL) GetLogFileOutputPtr()->SetDataItem(10, "Evaporation Tank B Distillate Pump Last 24 hr Delivery", ptrDistillateMeteringPump->GetLast24HourPumpDelivery());
		}
		else throw ProgramException("Null pointer to Distillate Metering Pump in " +  GetTankNameStr());
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(11, "EvaporationTankB::TankControlLogic", pe.GetExceptionDescription(), true);
	}
}