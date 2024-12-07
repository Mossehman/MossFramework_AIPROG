#include "FiniteState.h"
#include "FiniteStateMachine.h"

void FiniteState::Init(FiniteStateMachine* stateMachine)
{
	this->stateMachine = stateMachine;
}

bool FiniteState::SwitchState(std::string stateName)
{
	return stateMachine->SetCurrState(stateName);

}
