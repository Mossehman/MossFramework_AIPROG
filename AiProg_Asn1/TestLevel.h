#pragma once
#include <BaseLevel.h>
#include "NodePassabilityAI.h"
class TestLevel : public BaseLevel
{
	// Inherited via BaseLevel
	virtual void InitTiles() override;
	using BaseLevel::BaseLevel;
};

