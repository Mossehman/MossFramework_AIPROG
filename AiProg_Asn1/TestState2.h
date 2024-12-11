#pragma once
#include <FiniteState.h>

class TestState2 : public FiniteState
{
	// Inherited via FiniteState
	virtual void Update(float dt) override;
};

