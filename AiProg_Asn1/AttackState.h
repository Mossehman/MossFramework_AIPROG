#pragma once
#include <FiniteState.h>
#include "BaseSkill.h"

class LOLEntity;

class AttackState : public FiniteState
{
protected:
	LOLEntity* attachedEntity = nullptr;
	BaseSkill* skill = nullptr;
	bool HasUsedSkill = false;

public:
	std::vector<LOLEntity*> entitiesToTarget;

	virtual void Init(FiniteStateMachine* stateMachine) override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update(float dt) override;

};

