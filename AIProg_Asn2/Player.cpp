#include "Player.h"
#include <Map2D.h>
#include "KeyboardController.h"
#include "TurnManager.h"
#include "MessageHub.h"


void Player::OnTurnStart()
{
	isTurn = true;
	glm::ivec2 playerTilePos = Map2D::GetInstance()->PosToTilePos(position);
	std::vector<glm::ivec2> targetPositions;

	std::set<glm::ivec2, ivec2Compare> walkableTiles_Temp;
	std::set<glm::ivec2, ivec2Compare> walkableTiles;

	int startY = (std::max)(-movementRange + playerTilePos.y, 0);
	int endY = (std::min)(movementRange + playerTilePos.y, Map2D::GetInstance()->GetLevel()->GetMapY() - 1);

	int startX = (std::max)(-movementRange + playerTilePos.x, 0);
	int endX = (std::min)(movementRange + playerTilePos.x, Map2D::GetInstance()->GetLevel()->GetMapX() - 1);

	for (int y = startY; y <= endY; y++)
	{
		for (int x = startX; x <= endX; x++)
		{
			targetPositions.push_back(glm::ivec2(x, y));
		}
	}

	walkableAreas.clear();

	for (int i = 0; i < targetPositions.size(); i++)
	{
		glm::vec2 targetWorldPos = Map2D::GetInstance()->TileToPos(targetPositions[i]);
		std::vector<glm::vec2> pathData = Map2D::GetInstance()->GetPath(position, targetWorldPos, false, true);
		int remainingMovement = movementRange;

		for (int j = pathData.size() - 1; j >= 0; j--)
		{
			glm::ivec2 pathTileCoords = Map2D::GetInstance()->PosToTilePos(pathData[j]);
			if (Map2D::GetInstance()->GetLevel()->GetTilemap()[pathTileCoords.y][pathTileCoords.x]->Passability != 0 || remainingMovement <= 0)
			{
				continue;
			}

			remainingMovement -= Map2D::GetInstance()->GetLevel()->GetTilemap()[pathTileCoords.y][pathTileCoords.x]->cost;
			//std::cout << remainingMovement << ", " << Map2D::GetInstance()->GetLevel()->GetTilemap()[pathTileCoords.y][pathTileCoords.x]->cost << " (" << Map2D::GetInstance()->GetLevel()->GetTilemap()[pathTileCoords.y][pathTileCoords.x]->tileID << ", {" << pathTileCoords.x << ", " << pathTileCoords.y << " })" << std::endl;

			if (walkableTiles.count(pathTileCoords) == 0)
			{
				walkableTiles.insert(pathTileCoords);

				GameObject* highlightTile = new GameObject();
				highlightTile->setPosition(Map2D::GetInstance()->TileToPos(pathTileCoords));
				highlightTile->setScale(Map2D::GetInstance()->GetLevel()->GetTileSize());
				highlightTile->SetMesh(MeshBuilder::GenerateQuad("highlight", Color(1, 0, 0)));
				highlightTile->setOpacity(0.4f);

				highlightTiles.push_back(highlightTile);
				walkableAreas.insert(pathTileCoords);
			}
		}
	}

	//std::list<glm::ivec2> coordsToCheck;
	//coordsToCheck.push_front(playerTilePos);
	//
	//while (!coordsToCheck.empty())
	//{
	//	glm::ivec2 curr = coordsToCheck.front();
	//	if (walkableTiles_Temp.count(glm::ivec2(curr.x, curr.y - 1)) > 0 && walkableTiles.count(glm::ivec2(curr.x, curr.y - 1)) == 0)
	//	{
	//		walkableTiles.insert(glm::ivec2(curr.x, curr.y - 1));
	//		coordsToCheck.push_front(glm::ivec2(curr.x, curr.y - 1));
	//
	//		GameObject* highlightTile = new GameObject();
	//		highlightTile->setPosition(Map2D::GetInstance()->TileToPos(glm::ivec2(curr.x, curr.y - 1)));
	//		highlightTile->setScale(Map2D::GetInstance()->GetLevel()->GetTileSize());
	//		highlightTile->SetMesh(MeshBuilder::GenerateQuad("highlight", Color(1, 0, 0)));
	//		highlightTile->setOpacity(0.4f);
	//
	//		highlightTiles.push_back(highlightTile);
	//		walkableAreas.insert(glm::ivec2(curr.x, curr.y - 1));
	//	}
	//	else if (walkableTiles_Temp.count(glm::ivec2(curr.x, curr.y + 1)) > 0 && walkableTiles.count(glm::ivec2(curr.x, curr.y + 1)) == 0)
	//	{
	//		walkableTiles.insert(glm::ivec2(curr.x, curr.y + 1));
	//		coordsToCheck.push_front(glm::ivec2(curr.x, curr.y + 1));
	//
	//		GameObject* highlightTile = new GameObject();
	//		highlightTile->setPosition(Map2D::GetInstance()->TileToPos(glm::ivec2(curr.x, curr.y + 1)));
	//		highlightTile->setScale(Map2D::GetInstance()->GetLevel()->GetTileSize());
	//		highlightTile->SetMesh(MeshBuilder::GenerateQuad("highlight", Color(1, 0, 0)));
	//		highlightTile->setOpacity(0.4f);
	//
	//		highlightTiles.push_back(highlightTile);
	//		walkableAreas.insert(glm::ivec2(curr.x, curr.y + 1));
	//	}
	//	else if (walkableTiles_Temp.count(glm::ivec2(curr.x - 1, curr.y)) > 0 && walkableTiles.count(glm::ivec2(curr.x - 1, curr.y)) == 0)
	//	{
	//		walkableTiles.insert(glm::ivec2(curr.x - 1, curr.y));
	//		coordsToCheck.push_front(glm::ivec2(curr.x - 1, curr.y));
	//
	//		GameObject* highlightTile = new GameObject();
	//		highlightTile->setPosition(Map2D::GetInstance()->TileToPos(glm::ivec2(curr.x - 1, curr.y)));
	//		highlightTile->setScale(Map2D::GetInstance()->GetLevel()->GetTileSize());
	//		highlightTile->SetMesh(MeshBuilder::GenerateQuad("highlight", Color(1, 0, 0)));
	//		highlightTile->setOpacity(0.4f);
	//
	//		highlightTiles.push_back(highlightTile);
	//		walkableAreas.insert(glm::ivec2(curr.x - 1, curr.y));
	//	}
	//	else if (walkableTiles_Temp.count(glm::ivec2(curr.x + 1, curr.y)) > 0 && walkableTiles.count(glm::ivec2(curr.x + 1, curr.y)) == 0)
	//	{
	//		walkableTiles.insert(glm::ivec2(curr.x + 1, curr.y));
	//		coordsToCheck.push_front(glm::ivec2(curr.x + 1, curr.y));
	//
	//		GameObject* highlightTile = new GameObject();
	//		highlightTile->setPosition(Map2D::GetInstance()->TileToPos(glm::ivec2(curr.x + 1, curr.y)));
	//		highlightTile->setScale(Map2D::GetInstance()->GetLevel()->GetTileSize());
	//		highlightTile->SetMesh(MeshBuilder::GenerateQuad("highlight", Color(1, 0, 0)));
	//		highlightTile->setOpacity(0.4f);
	//
	//		highlightTiles.push_back(highlightTile);
	//		walkableAreas.insert(glm::ivec2(curr.x + 1, curr.y));
	//	}
	//	else
	//	{
	//		coordsToCheck.pop_front();
	//	}
	//}


	walkableTiles.clear();
	walkableTiles_Temp.clear();
	targetPositions.clear();
}

void Player::OnMovementStart(glm::vec2 destination)
{
	path.clear();
	for (int i = 0; i < highlightTiles.size(); i++)
	{
		delete highlightTiles[i];
	}

	highlightTiles.clear();

	path = Map2D::GetInstance()->GetPath(position, destination);
	startedMoving = true;
	isTurn = false;
}

void Player::OnTurnEnd()
{
	glm::ivec2 tilePos = Map2D::GetInstance()->PosToTilePos(position);
	int ID = Map2D::GetInstance()->GetLevel()->GetTilemap()[tilePos.y][tilePos.x]->tileID;

	switch (ID)
	{
	case 6:
		MessageHub::GetInstance()->SendMsg(new BaseMessage({ "Scene" }, 1));
		break;
	case 5:
		health -= 1;
		break;
	}

	isTurn = false;

	path.clear(); 
	for (int i = 0; i < highlightTiles.size(); i++) 
	{
		delete highlightTiles[i]; 
	} 

	highlightTiles.clear();

	TurnManager::GetInstance()->NextTurn();
}

void Player::RenderTiles()
{
	for (int i = 0; i < highlightTiles.size(); i++)
	{
		highlightTiles[i]->Render();
	}
}

void Player::RenderPath(Color color)
{
	if (path.empty()) { return; }

	for (int i = 0; i < path.size(); i++)
	{
		if (i == path.size() - 1) { break; }

		GameObject* lineObj = new GameObject();
		lineObj->SetMesh(MeshBuilder::GenerateLineDir("Line", color, path[i], path[i + 1]));
		lineObj->Render();
		delete lineObj;
	}
}

void Player::Update(double dt)
{
	if (health <= 0)
	{
		MessageHub::GetInstance()->SendMsg(new BaseMessage({ "Scene" }, 2));
	}

	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_SPACE) && isTurn)
	{
		OnTurnEnd();
		startedMoving = false;
		return;
	}

	if (path.empty())
	{
		if (startedMoving)
		{
			OnTurnEnd();
			startedMoving = false;
		}
		return;
	}

	float movement = 40.0f * dt;
	if (glm::distance2(position, path[path.size() - 1]) < movement * movement)
	{
		position = path[path.size() - 1];
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

int Player::HandleMessage(BaseMessage* msg)
{
	if (!msg) { return -1; }

	if (msg->GetMessageType() == 1)
	{
		isTurn = true;
		OnTurnStart();
	}

	return 0;
}
