#include "BrainState.h"
#include "EnemyManager.h"
#include <glm/gtx/norm.hpp>
#include "Map2D.h"
#include "Enemy.h"

void BrainState::Init(FiniteStateMachine* stateMachine)
{
	this->stateMachine = stateMachine; 
	enemy = (Enemy*)stateMachine->GetAttachedEntity(); 
}

void BrainState::OnEnter()
{
	enemy->CalculateVisionRange();
	enemy->finishedMoving = false;
	enemy->startedMoving = true;
	enemy->isTurn = false;
	enemy->remainingMovement = enemy->movementRange;

	glm::ivec2 playerTile = Map2D::GetInstance()->PosToTilePos(EnemyManager::GetInstance()->enemyPlayer->getPosition());
	for (int i = 0; i < enemy->visionTiles.size(); i++)
	{
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

void BrainState::Update(float dt)
{
	return;
}
