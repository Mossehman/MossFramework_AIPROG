#include "EntityAI2D.h"

void EntityAI2D::SolveAStar()
{
	return;

	//TODO: Port the AStar solver script once the Path Node generation is completed
}

void EntityAI2D::InitialiseStates()
{
	for (int i = 0; i < finiteStates.size(); i++)
	{
		finiteStates[i]->Init(this);
	}
}

void EntityAI2D::AddState(FiniteState* newState)
{
	if (!newState) { return; } //check if the state we want to add is Null

	for (int i = 0; i < finiteStates.size(); i++) //loop through all currently added states
	{
		if (newState->GetName() == finiteStates[i]->GetName()) //if newState's name is equal to the currently checked name
		{
			std::cout << "Duplicate State Name detected: " << newState->GetName() << ", unable to add!" << std::endl; //output error value
			return; //stop program here
		}
	}

	finiteStates.push_back(newState); //if no duplicate name detected, add to the list
}

void EntityAI2D::RemoveState(std::string stateName)
{
	for (int i = 0; i < finiteStates.size(); i++)
	{
		if (finiteStates[i]->GetName() == stateName)
		{
			finiteStates.erase(finiteStates.begin() + i); //if state is found, delete state from the list
			return;
		}
	}

	std::cout << "State with name: " << stateName << " removed!" << std::endl;
}

std::vector<FiniteState*> EntityAI2D::GetStates(void)
{
	return finiteStates;
}

FiniteState* EntityAI2D::GetCurrentState(void)
{
	return currentState;
}

void EntityAI2D::SetCurrentState(FiniteState* newCurrentState)
{
	currentState->OnExit(); //Run the exit code for the current state
	delete currentState; //Delete this state to avoid memory leaks
	newCurrentState->OnEnter(); //Run the entry code for the newly added state
	currentState = newCurrentState; //Set the state passed in via param as the new Current State
}
