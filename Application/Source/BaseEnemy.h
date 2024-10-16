#pragma once
#include "CreatureAI.h"

class BaseEnemy : public CreatureAI
{
public:

protected:
	

	float visionMultiplier;
	float visionFront;
	float visionBack;

	bool facingRight;
	bool canSeeTarget;

	glm::vec2 lastTargetPos;
};

