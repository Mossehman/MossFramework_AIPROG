#pragma once
#include <map>
#include "GameState.h"
#include "DesignPatterns/SingletonTemplate.h"
class GameStateManager: public SingletonTemplate<GameStateManager>
{
	friend SingletonTemplate<GameStateManager>;

protected:
	std::map<std::string, GameState*> gameStateMap;
	GameStateManager();
	~GameStateManager();

	GameState *currentState, *previousState, *nextState, *pausedState;

public:
	void Destroy();
	bool Update(double dt);
	void Render();

	void AddNewState(const std::string gameStateName, GameState* newState);
	bool SetCurrentState(const std::string gameStateName);
	bool SetPausedState(const std::string pausedStateName);
	void UnPause();
	bool RemoveState(const std::string gameStateName);

};

