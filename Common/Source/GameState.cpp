#include "GameState.h"
#include "GameState.h"

GameState::GameState(std::string gameStateName)
{
    this->gameStateName = gameStateName;
}

void GameState::SetName(std::string newName)
{
    newName = this->gameStateName;
}

std::string GameState::GetName(void) const
{
    return gameStateName;
}

int GameState::HandleMessage(BaseMessage* msg)
{
    return 0;
}

std::vector<GameObject*> GameState::GetGameObjectList()
{
    return this->GameObjectList;
}


