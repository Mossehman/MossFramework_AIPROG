#include "Enemy.h"
#include <Map2D.h>
#include <limits>

#include "EnemyManager.h"

void Enemy::OnTurnStart()
{
	EnemyManager::GetInstance()->isTurn = true;
	CalculateVisionRange();
	finishedMoving = false;
	startedMoving = true;
	isTurn = false;
	remainingMovement = movementRange;
	glm::vec2 endPt = GetDestination();
	path = Map2D::GetInstance()->GetPath(position, endPt);

	//OnMovementStart(this->destination);
}

void Enemy::OnMovementStart(glm::vec2 destination)
{
	
}

void Enemy::OnTurnEnd()
{
	finishedMoving = true;
	startedMoving = false;

	glm::ivec2 enemyTilePos = Map2D::GetInstance()->PosToTilePos(position);
	glm::ivec2 playerTilePos = Map2D::GetInstance()->PosToTilePos(EnemyManager::GetInstance()->enemyPlayer->getPosition());

	if (enemyTilePos.x == playerTilePos.x && enemyTilePos.y == playerTilePos.y)
	{
		EnemyManager::GetInstance()->enemyPlayer->health -= 1;
	}

	path.clear();
}

void Enemy::Update(double dt)
{
	if (path.empty() || remainingMovement <= 0)
	{
		if (startedMoving)
		{
			OnTurnEnd();
		}
		return;
	}

	float movement = 40.0f * dt;
	if (glm::distance2(position, path[path.size() - 1]) < movement * movement)
	{
		position = path[path.size() - 1];
		glm::ivec2 tileIndex = Map2D::GetInstance()->PosToTilePos(position);
		remainingMovement -= Map2D::GetInstance()->GetLevel()->GetTilemap()[tileIndex.y][tileIndex.x]->cost;

		path.pop_back();
		return;
	}

	glm::vec2 dir = glm::normalize(path[path.size() - 1] - position);

	if (path[path.size() - 1].x < position.x)
	{
		this->scale.x = -1 * cachedScale.x;
	}
	else
	{
		this->scale.x = cachedScale.x;
	}

	position += dir * movement;
}

void Enemy::CalculateVisionRange()
{
	visionTiles.clear();

	glm::ivec2 enemyTilePos = Map2D::GetInstance()->PosToTilePos(position);
	EnemyManager::GetInstance()->visionRanges.insert(enemyTilePos);
	visionTiles.push_back(enemyTilePos);
	std::vector<glm::ivec2> targetPositions;

	
	std::set<glm::ivec2, ivec2Compare> walkableTiles;

	int startY = std::max(-visionRange + enemyTilePos.y, 0);
	int endY = std::min(visionRange + enemyTilePos.y, Map2D::GetInstance()->GetLevel()->GetMapY() - 1);

	int startX = std::max(-visionRange + enemyTilePos.x, 0);
	int endX = std::min(visionRange + enemyTilePos.x, Map2D::GetInstance()->GetLevel()->GetMapX() - 1);

	for (int y = startY; y <= endY; y++)
	{
		for (int x = startX; x <= endX; x++)
		{
			targetPositions.push_back(glm::ivec2(x, y));
		}
	}

	for (int i = 0; i < targetPositions.size(); i++)
	{
		int remainingVision = visionRange;
		glm::vec2 targetWorldPos = Map2D::GetInstance()->TileToPos(targetPositions[i]);
		std::vector<glm::vec2> pathData = Map2D::GetInstance()->GetPath(position, targetWorldPos, false, true);

		for (int j = pathData.size() - 1; j >= 0; j--)
		{
			glm::ivec2 pathTileCoords = Map2D::GetInstance()->PosToTilePos(pathData[j]);
			if (Map2D::GetInstance()->GetLevel()->GetTilemap()[pathTileCoords.y][pathTileCoords.x]->Passability != 0 || remainingVision <= 0)
			{
				continue;
			}
			remainingVision--;

			visionTiles.push_back(pathTileCoords);
			EnemyManager::GetInstance()->visionRanges.insert(pathTileCoords);
		}
	}
}

int Enemy::HandleMessage(BaseMessage* msg)
{
	if (!msg) { return -1; }

	if (msg->GetMessageType() == 1)
	{
		isTurn = true;
		OnTurnStart();
	}

	return 0;
}

glm::vec2 Enemy::GetDestination()
{
	glm::ivec2 playerTile = Map2D::GetInstance()->PosToTilePos(EnemyManager::GetInstance()->enemyPlayer->getPosition());

	for (int i = 0; i < visionTiles.size(); i++)
	{
		if (visionTiles[i].x == playerTile.x && visionTiles[i].y == playerTile.y)
		{
			return EnemyManager::GetInstance()->enemyPlayer->getPosition();
		}
	}

	if (glm::distance2(position, destination) > 15 * 15) { return destination; }

	std::vector<glm::vec2> passableTiles = Map2D::GetInstance()->GetLevel()->GetTilesWithID(2);
	if (!passableTiles.empty())
	{
		int randomIndex = std::rand() % passableTiles.size();
		return passableTiles[randomIndex];
	}

}
