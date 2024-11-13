#pragma once
#include <BaseLevel.h>
class TestLevel : public BaseLevel
{
	// Inherited via BaseLevel
	virtual void InitTiles() override;
	using BaseLevel::BaseLevel;
};

