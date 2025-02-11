#pragma once
class Enemy;
#include <FiniteState.h>

class BrainState : public FiniteState
{
public:
	Enemy* enemy;
	virtual void Init(FiniteStateMachine* stateMachine) override;
	virtual void OnEnter() override;
	virtual void Update(float dt) override;
};

