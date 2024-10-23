#include "FiniteState.h"
#include "EntityAI2D.h"

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

FiniteState::FiniteState(std::string stateName)
{
	this->stateName = stateName;
}
