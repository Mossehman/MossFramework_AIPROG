#pragma once
#include <FiniteState.h>

class LOLEntity;

class AttackTurretState : public FiniteState
{
protected:
	float TowerCheckTimer = 0.0f;

public:
	float TowerCheckTime = 0.5f;
	LOLEntity* attachedEntity = nullptr;

	virtual void OnEnter() override;
	virtual void Update(float dt) override;

};

