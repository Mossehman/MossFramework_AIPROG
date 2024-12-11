
#include "FiniteStateMachine.h"
#include <iostream>

void FiniteStateMachine::Update(float dt)
{

	if (NextState)
	{
		if (CurrentState) { CurrentState->OnExit(); }
		CurrentState = NextState;
		CurrentState->OnEnter();
		NextState = nullptr;
	}
	
	if (!CurrentState) { return; }
	CurrentState->Update(dt);
}

void FiniteStateMachine::SetCurrState(FiniteState* state)
{
	NextState = state;
}

bool FiniteStateMachine::SetCurrState(std::string stateName)
{
	if (States.count(stateName) <= 0) { return false; }
	NextState = States.at(stateName);
	return true;
}

void FiniteStateMachine::AddState(std::string name, FiniteState* state)
{
	if (States.count(name) > 0) { return; }
	States.insert(std::pair<std::string, FiniteState*>(name, state));

	if (this->NextState == nullptr) //check if any state has been initialised as the current state yet
	{
		SetCurrState(state); //initialise the newly added state as the current state if current state is null
	}

	state->Init(this);

}

void FiniteStateMachine::RemoveState(std::string name)
{
	if (States.count(name) <= 0) { return; }
	delete States.at(name); //free up memory (idk if this breaks yet, TODO: check if deleting states crashes the program)
	States.erase(name);
}

FiniteState* FiniteStateMachine::GetCurrState()
{
	return CurrentState;
}

std::string FiniteStateMachine::GetCurrStateName()
{
	if (!CurrentState) { return std::string(); }
	for (auto it = States.begin(); it != States.end(); ++it)
	{
		if (it->second == CurrentState)
		{
			return it->first;
		}
	}
	return std::string();
}

FiniteStateMachine::FiniteStateMachine(EntityAI2D* attachedEntity)
{
	this->attachedEntity = attachedEntity;
}

FiniteStateMachine::~FiniteStateMachine()
{
	for (auto it = States.begin(); it != States.end(); ++it)
	{
		FiniteState* state = it->second;
		if (!state) { continue; }
		delete state;
	}

	States.clear();
}

EntityAI2D* FiniteStateMachine::GetAttachedEntity()
{
	return attachedEntity;
}
