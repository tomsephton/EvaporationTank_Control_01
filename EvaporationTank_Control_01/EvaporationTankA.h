#pragma once

#include "liquidtank.h"
#include "DistillateSupplyTank.h"

class EvaporationTankA : public LiquidTank
{
private:
	DistillateSupplyTank* ptrDistillateSupplyTank;
public:
	EvaporationTankA(void);
	virtual ~EvaporationTankA(void);
	void InitializeObjects();
	void DrawPumpSpeedChart(GLint nFontList);
	bool SetDistillateSupplyTankPtr(DistillateSupplyTank* ptr);
	void TankControlLogic(void);
};

