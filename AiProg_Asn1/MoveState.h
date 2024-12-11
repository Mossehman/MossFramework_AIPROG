#pragma once
#include <FiniteState.h>

class LOLEntity;

class MoveState : public FiniteState
{
protected:
	float EnemyCheckTimer = 0.0f;
	float EnemyCheckThresholdTimer = 0.0f;
	LOLEntity* attachedEntity = nullptr;

public:
	float EnemyCheckTime = 0.5f;
	float EnemyCheckThresholdTime = 0.85f;

	virtual void OnEnter() override;
	virtual void Update(float dt) override;
};

