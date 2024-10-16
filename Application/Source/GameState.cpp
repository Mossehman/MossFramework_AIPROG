#include "GameState.h"

GameState::GameState(std::string gameStateName)
{
    this->gameStateName = gameStateName;
}

void GameState::setName(std::string newName)
{
    newName = this->gameStateName;
}

std::string GameState::getName(void) const
{
    return gameStateName;
}
