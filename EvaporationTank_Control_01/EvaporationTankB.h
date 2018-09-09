#pragma once

#include "liquidtank.h"
#include "DistillateSupplyTank.h"

class EvaporationTankB : public LiquidTank
{
private:
	DistillateSupplyTank* ptrDistillateSupplyTank;
public:
	EvaporationTankB(void);
	virtual ~EvaporationTankB(void);
	void InitializeObjects();
	void DrawPumpSpeedChart(GLint nFontList);
	bool SetDistillateSupplyTankPtr(DistillateSupplyTank* ptr);
	void TankControlLogic(void);
};
