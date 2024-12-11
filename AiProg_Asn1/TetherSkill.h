#pragma once
#include "SetupSkill.h"
#include <glm/glm.hpp>

class TetherSkill : public SetupSkill
{
public:
	TetherSkill (std::initializer_list<int> types, TeamSearchParam msgType = NUM_SEARCH_PARAMS) : SetupSkill(types, msgType) {}



	glm::vec2 tetherPos = glm::vec2();

	virtual void WhileAttacking(float dt) override;
	virtual void OnSetup() override;
};

