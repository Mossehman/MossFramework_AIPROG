#pragma once
#include <FiniteState.h>

class LOLEntity;

class TurretIdleState : public FiniteState
{
protected:
	float checkForActivityTimer = 0.0f;
	float spawnMinionsTimer = 0.0f;
	LOLEntity* attachedEntity = nullptr;

public:
	float checkForActivityTime = 0.6f;
	float spawnMinionsTime = 30.0f;

	virtual void OnEnter() override;
	virtual void Update(float dt) override;
};

