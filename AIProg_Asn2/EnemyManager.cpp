#include "EnemyManager.h"
#include "TurnManager.h"
#include <Map2D.h>

void EnemyManager::Update(double dt)
{
	if (!isTurn) { 
		visionRanges.clear();
		return; 
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		if (!enemies[i]->finishedMoving) { return; }
	}
	isTurn = false;
	TurnManager::GetInstance()->NextTurn();

}

void EnemyManager::Render()
{
	if (!isTurn) { return; }

	for (int y = 0; y < Map2D::GetInstance()->GetLevel()->GetMapY(); y++)
	{
		for (int x = 0; x < Map2D::GetInstance()->GetLevel()->GetMapY(); x++)
		{
			if (visionRanges.count(glm::ivec2(x, y)) > 0) { continue; }
			GameObject* shadowTile = new GameObject();
			shadowTile->setPosition(Map2D::GetInstance()->TileToPos(glm::ivec2(x, y)));
			shadowTile->setScale(Map2D::GetInstance()->GetLevel()->GetTileSize());
			shadowTile->SetMesh(MeshBuilder::GenerateQuad("shadow", Color(0, 0, 0)));
			shadowTile->setOpacity(0.5f);

			shadowTile->Render();

			delete shadowTile;
		}
	}
}
