#include "BrainState_Scared.h"
#include "Enemy.h"
#include "Map2D.h"
#include "EnemyManager.h"
#include <glm/gtx/norm.hpp>

void BrainState_Scared::Init(FiniteStateMachine* stateMachine)
{
	this->stateMachine = stateMachine;
	enemy = (Enemy*)stateMachine->GetAttachedEntity();
}

void BrainState_Scared::OnEnter()
{
	enemy->CalculateVisionRange();
	enemy->finishedMoving = false;
	enemy->startedMoving = true;
	enemy->isTurn = false;
	enemy->remainingMovement = enemy->movementRange;

	glm::ivec2 playerTile = Map2D::GetInstance()->PosToTilePos(EnemyManager::GetInstance()->enemyPlayer->getPosition());
	for (int i = 0; i < enemy->visionTiles.size(); i++)
	{
		for (int j = 0; j < EnemyManager::GetInstance()->enemies.size(); j++)
		{
			glm::ivec2 enemyTile = Map2D::GetInstance()->PosToTilePos(EnemyManager::GetInstance()->enemies[i]->getPosition());
			if (enemyTile.x == enemy->visionTiles[i].x && enemyTile.y == enemy->visionTiles[i].y) {
			}
		}

		if (enemy->visionTiles[i].x == playerTile.x && enemy->visionTiles[i].y == playerTile.y)
		{
			if (glm::distance2(EnemyManager::GetInstance()->enemyPlayer->getPosition(), stateMachine->GetAttachedEntity()->getPosition()) < 15 * 15)
			{
				stateMachine->SetCurrState("Attack");
			}
			else
			{
				stateMachine->SetCurrState("Move");
			}
			return;
		}
	}

	stateMachine->SetCurrState("Wander");
}

void BrainState_Scared::Update(float dt)
{
}
