#include "StunnedState.h"
#include "LOLEntity.h"

void StunnedState::OnEnter()
{
	LOLEntity* entity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	this->stunTimer = 0.0f;
	this->stunTime = entity->stunTime;
}

void StunnedState::Update(float dt)
{
	stunTimer += dt;
	if (stunTime > stunTimer) { return; }

	this->stateMachine->SetCurrState("Move");
}
