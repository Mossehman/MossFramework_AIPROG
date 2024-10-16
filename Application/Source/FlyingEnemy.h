#pragma once
#include "CreatureAI.h"
#include "GunBase.h"
class FlyingEnemy: public CreatureAI
{
public:
	FlyingEnemy(LevelIDs entityLevel, Entity2D* playerEntity, glm::vec2 pos = glm::vec2(0, 0), glm::vec2 rot = glm::vec2(0, 0), glm::vec2 scl = glm::vec2(1, 1));

	GunBase* rangedAttack;
	std::vector<Entity2D*> playerVector;

	unsigned int damageValue = 10;
	float viewRange = 1000.f;
	float rangedAttackDist = 600.f;
	float meleeAttackDist = 300.f;
	float attackRange = 100.f;

	float meleeAttackTimer = 0.6f;
	float meleeAttackTimerCurrent = 0;
	int xPatrolDirection = 1;

	float randomPatrolTimer = 5.f;
	float randomPatrolTimerCurrent = 0;

	Entity2D* playerEntity;

	void initialiseStates() override;
	void updateFSM(double dt) override;

	virtual void Render();

};

