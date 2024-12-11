#include "NPCDeadState.h"
#include "LOLEntity.h"

void NPCDeadState::OnEnter()
{
	this->attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	if (!attachedEntity) { return; }
	
	attachedEntity->isAlive = false;
	attachedEntity->health = 0;
}

void NPCDeadState::Update(float dt)
{
	return;
}
