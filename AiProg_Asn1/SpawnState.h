#pragma once
#include <FiniteState.h>

class LOLEntity;
#define MAX_MINIONS 10

class SpawnState : public FiniteState
{
protected:
	LOLEntity* attachedEntity = nullptr;
	LOLEntity* minions[MAX_MINIONS];
	int spawnedMinions = 0;

public:
	int YSpawnRandomRange = 14;
	int NumMinionsToSpawn = 2;

	virtual void Init(FiniteStateMachine* stateMachine) override;
	virtual void OnEnter() override;
	virtual void Update(float dt) override;
};

