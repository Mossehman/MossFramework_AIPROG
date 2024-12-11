#pragma once
#include <FiniteState.h>

class LOLEntity;

class RetreatState : public FiniteState
{
protected:
	float unitCheckTimer = 0.0f;


public:

	float unitCheckTime = 0.5f;
	LOLEntity* attachedEntity = nullptr;

	virtual void OnEnter() override;
	virtual void Update(float dt) override;
};

