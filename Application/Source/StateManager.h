#pragma once
#include <string>
#include <map>

struct AIstate
{
protected:
	std::string stateName;
	std::map<std::string, AIstate*> listOfTransitions;

public:
	AIstate(const std::string stateName) { this->stateName = stateName; }
	~AIstate()
	{
		if (!listOfTransitions.empty())
		{
			listOfTransitions.clear();
		}
	}

	void addStateTransition(std::string stateName, AIstate* stateToAdd)
	{
		if (!stateToAdd) { return; }
		listOfTransitions.insert(std::pair<std::string, AIstate*>(stateName, stateToAdd));
	}

	bool checkTransition(std::string stateToTransitionTo)
	{
		if (listOfTransitions.count(stateToTransitionTo) != 0)
		{
			return true;
		}

		return false;
	}

	std::string getStateName(void) { return this->stateName; }

};

class StateManager
{
protected:
	std::map<std::string, AIstate*> stateMap;
public:
	AIstate* currentState;
	void addState(const std::string newStateName);
	void createTransition(const std::string stateBaseName, const std::string stateToTransitionName, bool twoWayTransition = true);
	void transitionToState(const std::string stateName);
	void transitionToStateForce(const std::string stateName);
	~StateManager();
};

