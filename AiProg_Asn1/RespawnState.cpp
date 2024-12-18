#include "RespawnState.h"
#include "LOLEntity.h"
#include <random>

void RespawnState::OnEnter()
{
	this->attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	if (!attachedEntity) { return; }

	this->respawnTime = attachedEntity->RespawnTimer;
}

void RespawnState::Update(float dt)
{
	respawnTime -= dt;
	if (respawnTime <= 0)
	{
		attachedEntity->setPosition(attachedEntity->spawnPos);
		attachedEntity->health = attachedEntity->MaxHealth;
		attachedEntity->isAlive = true;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(-1, 1);
		attachedEntity->lane = dis(gen);

		this->stateMachine->SetCurrState("InitLane");
		return;
	}
}
