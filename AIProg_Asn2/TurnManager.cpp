#include "TurnManager.h"
#include "BaseMessage.h"
#include <MessageHub.h>
#include <Map2D.h>
#include "MazeMap.h"

#include <iostream>
#include <random>

void TurnManager::RegisterTurnID(int turnNumber, std::string listenerID)
{
	turnIDs.insert({ turnNumber, listenerID });
}

void TurnManager::NextTurn()
{
	MazeMap* map = (MazeMap*) Map2D::GetInstance()->GetLevel();
	if (turnOrder > maxTurns) { 
		turnOrder = 0;
		map->ResetTiles();

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(1, 20);

		int randomEvent = dist(gen);

		if (randomEvent == 1)
		{
			map->LavaEvent();
			currentEvent = "Heatwave";
		}
		else if (randomEvent == 2)
		{
			map->WaterEvent();
			currentEvent = "Flood";
		}
		else if (randomEvent == 3)
		{
			map->Drought();
			currentEvent = "Drought";
		}
	}

	if (turnIDs.count(turnOrder) == 0) { return; }
	MessageHub::GetInstance()->SendMsg(new BaseMessage({ turnIDs.at(turnOrder) }, 1)); //tell all units on that team that it's their turn
	turnOrder++;
}
