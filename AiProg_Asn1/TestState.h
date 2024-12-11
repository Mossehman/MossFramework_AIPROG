#pragma once
#include <FiniteState.h>

class TestState : public FiniteState
{
	// Inherited via FiniteState
	virtual void Update(float dt) override;
};

