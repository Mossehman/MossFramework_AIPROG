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

	void addNewState(const std::string gameStateName, GameState* newState);
	bool setCurrentState(const std::string gameStateName);
	bool setPausedState(const std::string pausedStateName);
	void unPause();
	bool removeState(const std::string gameStateName);

};

