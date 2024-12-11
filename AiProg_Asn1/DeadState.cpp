#include "DeadState.h"
#include "LOLEntity.h"
#include "MessageHub.h"
#include "MsgDied.h"

void DeadState::OnEnter()
{
	attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	if (!attachedEntity) { return; }
	attachedEntity->isAlive = false;
	MessageHub::GetInstance()->SendMsg(new MsgDied({ "GameScene" }, attachedEntity));
}

void DeadState::Update(float dt)
{
	return;
}
