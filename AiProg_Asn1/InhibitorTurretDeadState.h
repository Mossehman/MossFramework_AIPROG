#pragma once
#include <FiniteState.h>

class LOLEntity;

class InhibitorTurretDeadState : public FiniteState
{
protected:
	LOLEntity* attachedEntity = nullptr;

public:
	virtual void OnEnter() override;
	virtual void Update(float dt) override;
};

