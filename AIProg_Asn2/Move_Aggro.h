#pragma once
#include <FiniteState.h>
class Enemy;

class Move_Aggro : public FiniteState
{
	Enemy* enemy;
	virtual void Init(FiniteStateMachine* stateMachine) override;
	virtual void OnEnter() override;
	virtual void Update(float dt) override;
};

