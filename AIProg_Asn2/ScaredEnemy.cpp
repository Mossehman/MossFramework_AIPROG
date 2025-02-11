#include "ScaredEnemy.h"
#include <Map2D.h>
#include <limits>

#include "EnemyManager.h"

glm::vec2 ScaredEnemy::GetDestination()
{
	glm::ivec2 playerTile = Map2D::GetInstance()->PosToTilePos(EnemyManager::GetInstance()->enemyPlayer->getPosition());

	bool enemyInSight = false;

	for (int i = 0; i < visionTiles.size(); i++)
	{
		for (int j = 0; j < EnemyManager::GetInstance()->enemies.size(); j++)
		{
			if (EnemyManager::GetInstance()->enemies[j] == this) { continue; }
			glm::ivec2 enemyTile = Map2D::GetInstance()->PosToTilePos(EnemyManager::GetInstance()->enemies[j]->getPosition());
			if (enemyTile.x == visionTiles[i].x && enemyTile.y == visionTiles[i].y)
			{
				enemyInSight = true;
				return GetNearestTileToDestination(glm::normalize(glm::vec2(EnemyManager::GetInstance()->enemies[j]->getPosition() - position)) * 100000.0f);
			}
		}

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

glm::vec2 ScaredEnemy::GetNearestTileToDestination(glm::vec2 dest)
{
	std::vector<glm::vec2> passableTiles = Map2D::GetInstance()->GetLevel()->GetTilesWithID(2);
	glm::vec2 returnVal;

	float nearestDist = std::numeric_limits<float>().max();
	for (int i = 0; i < passableTiles.size(); i++)
	{
		float dist = glm::distance2(dest, passableTiles[i]);
		if (dist < nearestDist) { nearestDist = dist; }

		returnVal = passableTiles[i];
	}

	return returnVal;
}
