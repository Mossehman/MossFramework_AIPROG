#include "FiniteState.h"
#include "EntityAI2D.h"
#include <random>

void FiniteState::Init(EntityAI2D* entity)
{
	this->entity = entity;
}

std::string FiniteState::GetName(void)
{
	return this->stateName;
}

void FiniteState::SwitchState(std::string stateName)
{
	for (int i = 0; i < entity->GetStates().size(); i++)
	{
		if (entity->GetStates()[i]->GetName() == stateName)
		{
			entity->SetCurrentState(entity->GetStates()[i]); //if state name is found, swap the entity's state to this new state
			return;
		}
	}

	std::cout << "State with name: " << stateName << " not found, unable to switch state!" << std::endl;
}

void FiniteState::SwitchStateRand(std::vector<std::string> stateNames)
{
	bool stateNameCheck = false;
	for (int i = 0; i < stateNames.size(); i++)
	{
		for (int j = 0; j < entity->GetStates().size(); j++)
		{
			if (entity->GetStates()[j]->GetName() == stateNames[i])
			{
				stateNameCheck = true;
				return;
			}
		}
	}
	if (!stateNameCheck) { return; } //states we are trying to swap to do not exist, return

	bool hasSwapped = false;
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distr(0, stateNames.size() - 1); 

	for (int i = 0; i < 100; i++)
	{
		int swapIndex = distr(gen);
		for (int i = 0; i < entity->GetStates().size(); i++) 
		{
			if (entity->GetStates()[i]->GetName() == stateNames[swapIndex])
			{
				entity->SetCurrentState(entity->GetStates()[i]); //if state name is found, swap the entity's state to this new state 
				hasSwapped = true;
				return;
			}
		}

		if (hasSwapped) { break; }
	}
	
}

FiniteState::FiniteState(std::string stateName)
{
	this->stateName = stateName;
}
