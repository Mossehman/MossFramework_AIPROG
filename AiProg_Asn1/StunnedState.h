#pragma once
#include <FiniteState.h>

class LOLEntity;

class StunnedState : public FiniteState
{
protected:
	float stunTimer = 0.0f;
public:
	float stunTime = 0.0f;

	virtual void OnEnter() override;
	virtual void Update(float dt) override;
};

