#pragma once
#include <FiniteState.h>

class LOLEntity;

class DeadState : public FiniteState
{
protected:
	LOLEntity* attachedEntity = nullptr;

public:
	virtual void OnEnter() override;

	// Inherited via FiniteState
	virtual void Update(float dt) override;
};

