#pragma once
#include <FiniteState.h>

class LOLEntity;

class ChaseState : public FiniteState
{
protected:
	float EnemyCheckTimer = 0.0f;
	float EnemyCheckThresholdTimer = 0.0f;

public:
	float EnemyCheckTime = 0.5f;
	float EnemyCheckThresholdTime = 0.85f;

	LOLEntity* attachedEntity = nullptr;

	virtual void OnEnter() override;
	virtual void Update(float dt) override;

};

