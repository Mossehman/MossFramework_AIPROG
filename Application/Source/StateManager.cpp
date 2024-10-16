#include "StateManager.h"
#include <iostream>

void StateManager::addState(const std::string newStateName)
{
	AIstate* newState = new AIstate(newStateName);

	if (stateMap.empty()) { currentState = newState; }
	else if (stateMap.count(newStateName) != 0) { std::cout << "Duplicate State Name detected!" << std::endl; return; }

	stateMap.insert(std::pair<std::string, AIstate*>(newStateName, newState));
}

void StateManager::createTransition(const std::string stateBaseName, const std::string stateToTransitionName, bool twoWayTransition)
{
	if (stateMap.count(stateBaseName) == 0 || stateMap.count(stateToTransitionName) == 0)
	{
		std::cout << "Unable to create transition, could not find one or more AI states, states are: " << stateBaseName << ", " << stateToTransitionName << std::endl;
		return;
	}

	stateMap.at(stateBaseName)->addStateTransition(stateToTransitionName, stateMap.at(stateToTransitionName));

	if (twoWayTransition)
	{
		stateMap.at(stateToTransitionName)->addStateTransition(stateBaseName, stateMap.at(stateBaseName));
	}
}

void StateManager::transitionToState(const std::string stateName)
{
	if (!currentState->checkTransition(stateName)) //if no transition from current state to new state is found
	{
		std::cout << "Unable to transition to" << stateName << ", no transition found" << std::endl;
		return; //do not transition
	}

	currentState = stateMap.at(stateName);
	std::cout << "Transitioned to state: " << stateName << std::endl;

	
}

void StateManager::transitionToStateForce(const std::string stateName)
{
	if (stateMap.count(stateName) == 0) { return; }
	currentState = stateMap.at(stateName);
}

StateManager::~StateManager()
{
	// Delete all scenes stored and empty the entire map
	std::map<std::string, AIstate*>::iterator it, end;
	end = stateMap.end();
	for (it = stateMap.begin(); it != end; ++it)
	{
		if (it->second)
		{
			delete it->second;
		}
	}
	// Clear the map
	stateMap.clear();

	if (currentState)
	{
		delete currentState;
	}
}
