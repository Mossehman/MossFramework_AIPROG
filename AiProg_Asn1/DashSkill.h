#pragma once
#include "BaseSkill.h"
#include <glm/glm.hpp>

class DashSkill : public BaseSkill
{
protected:
	float DashDurationTimer = 0.0f;
	

public:

	DashSkill(std::initializer_list<int> types, TeamSearchParam msgType = NUM_SEARCH_PARAMS) : BaseSkill(types, msgType) {}
	

	float DashDuration = 0.1f;
	float DashModifier = 200.0f;

	// Inherited via BaseSkill
	virtual void OnSkillEnter() override;
	virtual void OnSkillEnd() override;
	virtual void WhileAttacking(float dt) override;
};

