#pragma once
#include "CreatureAI.h"

class TestEnemy: public CreatureAI
{
public:
	TestEnemy(LevelIDs entityLevel, Entity2D* playerEntity, glm::vec2 pos = glm::vec2(0, 0), glm::vec2 rot = glm::vec2(0, 0), glm::vec2 scl = glm::vec2(1, 1));

	unsigned int damageValue = 5;
	float viewRange = 1000.f;
	float attackRange = 100.f;

	float attackTimer = 0.6f;
	float attackTimerCurrent = 0;
	int xPatrolDirection = 1;

	float randomPatrolTimer = 5.f;
	float randomPatrolTimerCurrent = 0;

	Entity2D* playerEntity;

	void initialiseStates() override;
	void updateFSM(double dt) override;
};

