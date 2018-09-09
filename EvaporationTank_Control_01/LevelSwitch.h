#pragma once
#include "BinarySensor.h"

#define  WHICHSWITCHSTRINGSIZE 12

class LevelSwitch : public BinarySensor
{
private:
	short configuration; // normal switch configuration 0="normal closed", 1="normal open", -1= "not set or error"
	short whichSwitch; // identifies switch as 0="unspecified", 1="low alarm", 2="low", 3="high", 4="high alarm", -1= "not set or error"
	char whichSwitchString[WHICHSWITCHSTRINGSIZE]; // string identifier of switch as "unspecified", "low alarm", "low level", "high level", "high alarm"
	short levelInputDeviceID; // ID of device from which current switch status can be read
	short levelInputDevicePort; // Port ID of device from which current switch status can be read
	short levelInputDeviceChannel; // Channel ID of device from which current switch status can be read
	bool wavesDetected;

public:
	LevelSwitch();
	virtual ~LevelSwitch(void);
	void SetConfiguration(bool normalOpen);
	short GetConfiguration(void);
	void SetWhichSwitch(short setting);
	short GetWhichSwitch(void);
	char* GetWhichSwitchStringPtr(void);
	bool GetWavesDetected();
	void DrawLevelIndicator(int xPos, int yPos, int xOrigin, int yOrigin);
};

