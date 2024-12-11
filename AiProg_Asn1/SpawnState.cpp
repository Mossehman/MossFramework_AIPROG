#include "SpawnState.h"
#include "LOLEntity.h"
#include "EntityTypes.h"
#include "Map2D.h"

#include "ProjectileSkill.h"
#include "MessageHub.h"
#include "MsgSpawn.h"
#include "MsgLane.h"

#include "MoveState.h"
#include "AttackState.h"
#include "InitLaneState.h"
#include "AttackTurretState.h"
#include "ChaseState.h"
#include "NPCDeadState.h"

#include <random>

void SpawnState::Init(FiniteStateMachine* stateMachine)
{
	FiniteState::Init(stateMachine);
	for (int i = 0; i < MAX_MINIONS; i++)
	{
		minions[i] = new LOLEntity(MINION);
		minions[i]->SetMesh(MeshBuilder::GenerateQuad("quad", Color(1, 1, 1)));
		minions[i]->GetFSM()->AddState("InitLane", new InitLaneState());
		minions[i]->GetFSM()->AddState("Move", new MoveState());
		minions[i]->GetFSM()->AddState("AttackTurret", new AttackTurretState());
		minions[i]->GetFSM()->AddState("Attack", new AttackState());
		minions[i]->GetFSM()->AddState("Chase", new ChaseState());
		minions[i]->MaxHealth = 50;
		minions[i]->GetFSM()->AddState("Dead", new NPCDeadState());
		minions[i]->setScale(glm::vec2(Map2D::GetInstance()->GetLevel()->GetTileSize().x, Map2D::GetInstance()->GetLevel()->GetTileSize().y));

		minions[i]->AddSkill(new ProjectileSkill({ ATTACK }, OPPOSING_TEAMS));
		minions[i]->InitAStar(); 
		MessageHub::GetInstance()->AddReciever("Minions", minions[i]);
		minions[i]->isAlive = false;
		MessageHub::GetInstance()->SendMsg(new MsgSpawn({ "GameScene" }, minions[i]));

	}
}

void SpawnState::OnEnter()
{
	attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	spawnedMinions = 0;
}

void SpawnState::Update(float dt)
{
	if (!attachedEntity) { return; }

	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> dis(-YSpawnRandomRange, YSpawnRandomRange); 

	for (int i = 0; i < MAX_MINIONS; i++)
	{
		if (spawnedMinions >= NumMinionsToSpawn) { break; }
		if (!minions[i]) { continue; }
		if (minions[i]->isAlive) { continue; }

		std::cout << spawnedMinions << ", " << NumMinionsToSpawn << std::endl;
 
		float randomYOffset = dis(gen) * Map2D::GetInstance()->GetLevel()->GetTileSize().y;

		minions[i]->Team = attachedEntity->Team;
		minions[i]->spawnPos = glm::vec2(attachedEntity->getPosition().x, attachedEntity->getPosition().y + randomYOffset);
		minions[i]->setPosition(glm::vec2(attachedEntity->getPosition().x, attachedEntity->getPosition().y + randomYOffset));

		int lane = 0;
		if (randomYOffset > YSpawnRandomRange * 0.4f)
		{
			lane = 1;
		}
		else if (randomYOffset < -YSpawnRandomRange * 0.4f)
		{
			lane = -1;
		}

		minions[i]->lane = lane;
		minions[i]->GetFSM()->SetCurrState("InitLane");
		minions[i]->health = minions[i]->MaxHealth;

		if (attachedEntity->Team == 1)
		{
			minions[i]->SetTexture("Image/Minion_Red.png");
		}
		else
		{
			minions[i]->SetTexture("Image/Minion_Blue.png");
		}
		minions[i]->isAlive = true;
		spawnedMinions++;
	}

	this->stateMachine->SetCurrState("Move");
}
