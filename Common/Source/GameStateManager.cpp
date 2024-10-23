#include "GameStateManager.h"
#include <iostream>

GameStateManager::GameStateManager()
{
	currentState = nullptr;
	previousState = nullptr;
	nextState = nullptr;
	pausedState = nullptr;
}

GameStateManager::~GameStateManager()
{
	this->Destroy();
}

void GameStateManager::Destroy()
{
	// Set the handlers to CGameState to nullptr
	currentState = nullptr;
	nextState = nullptr;
	previousState = nullptr;
	pausedState = nullptr;

	// Delete all scenes stored and empty the entire map
	std::map<std::string, GameState*>::iterator it, end;
	end = gameStateMap.end();
	for (it = gameStateMap.begin(); it != end; ++it)
	{
		delete it->second;
	}
	// Clear the map
	gameStateMap.clear();
}

bool GameStateManager::Update(double dt)
{
	if (nextState && nextState != currentState)
	{
		if (currentState)
		{
			currentState->Destroy();
		}

		currentState = nextState;
		nextState = nullptr;

		std::cout << "Transitioning to game state: " << currentState->GetName() << " . . ." << std::endl;
		if (currentState->Init())
		{ 
			std::cout << "Successful Transition!" << std::endl;
		}
		else
		{
			std::cout << "Error, unable to transition to game state: " << currentState->GetName() << ", Init failed to run!" << std::endl;
			return false;
		}
	}
	
	if (currentState)
	{
		if (!currentState->Update(dt)) { return false; }
		else if (pausedState) { pausedState->Update(dt); }
	}

	return true;
}

void GameStateManager::Render()
{
	if (currentState)
	{
		currentState->Render();
	}

	if (pausedState)
	{
		pausedState->Render();
	}
}

void GameStateManager::AddNewState(const std::string gameStateName, GameState* newState)
{
	if (gameStateMap.count(gameStateName) > 0)
	{
		std::cout << "Unable to add state: " << gameStateName << ", duplicate name detected!" << std::endl;
		return;
	}
	if (!newState)
	{
		std::cout << "GameState was nullptr, unable to add, name: " << gameStateName << std::endl;
		return;
	}

	newState->SetName(gameStateName);
	gameStateMap.insert(std::pair<std::string, GameState*>(gameStateName, newState));
}

bool GameStateManager::SetCurrentState(const std::string gameStateName)
{
	if (gameStateMap.count(gameStateName) == 0) 
	{
		std::cout << "Unable to transition to Game State: " << gameStateName << ", state not found!" << std::endl;
		return false;
	}

	nextState = gameStateMap.at(gameStateName);
	if (currentState) { previousState = currentState; }

	return true;
}

bool GameStateManager::SetPausedState(const std::string pausedStateName)
{

	if (pausedState)
	{
		pausedState = nullptr;
		return true;
	}

	if (gameStateMap.count(pausedStateName) == 0)
	{
		std::cout << "Unable to set Paused State: " << pausedStateName << ", state not found!" << std::endl;
		return false;
	}

	pausedState = gameStateMap.at(pausedStateName);
	pausedState->Init();

	return true;
}

void GameStateManager::UnPause()
{
	if (!pausedState) { return; }
	pausedState = nullptr;
}

bool GameStateManager::RemoveState(const std::string gameStateName)
{
	if (gameStateMap.count(gameStateName) == 0) { return false; }
	GameState* stateToDelete = gameStateMap.at(gameStateName);
	if (stateToDelete == currentState || stateToDelete == nextState)
	{
		std::cout << "State to delete: " << gameStateName << " has been queued or is currently running, unable to delete!" << std::endl;
		return false;
	}

	delete stateToDelete;
	gameStateMap.erase(gameStateName);

	return true;
}
