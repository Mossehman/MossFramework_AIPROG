#include "Move_Aggro.h"
#include "Enemy.h"

void Move_Aggro::Init(FiniteStateMachine* stateMachine)
{
	this->stateMachine = stateMachine;
	enemy = (Enemy*)stateMachine->GetAttachedEntity();
}

void Move_Aggro::OnEnter()
{
	
}

void Move_Aggro::Update(float dt)
{
}
