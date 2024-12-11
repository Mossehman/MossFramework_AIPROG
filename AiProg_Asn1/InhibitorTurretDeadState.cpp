#include "InhibitorTurretDeadState.h"
#include "LOLEntity.h"
#include "MessageHub.h"
#include "MsgGameOver.h"

void InhibitorTurretDeadState::OnEnter()
{
	this->attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	if (!attachedEntity) { return; }

	attachedEntity->isAlive = false;

	MessageHub::GetInstance()->SendMsg(new MsgGameOver({ "GameScene" }, attachedEntity->Team));
}

void InhibitorTurretDeadState::Update(float dt)
{
	return;
}
