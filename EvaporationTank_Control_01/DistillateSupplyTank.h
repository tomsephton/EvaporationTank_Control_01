#pragma once
#include "liquidtank.h"

class DistillateSupplyTank : public LiquidTank
{
public:
	DistillateSupplyTank(void);
	virtual ~DistillateSupplyTank(void);

	void InitializeObjects();
	void DrawFilledCircle(float cx, float cy, float r, int numSegments);
	void Display2D(GLint nFontList, float xOrigin, float yOrigin);
	void DrawLevelSensors(int xPos, int yPos, int xOrigin, int yOrigin);
	void DisplayProcessErrors2D(GLint nFontList, float xOrigin, float yOrigin);

	void TankControlLogic(void);
};

