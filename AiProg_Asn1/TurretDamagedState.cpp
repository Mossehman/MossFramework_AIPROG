#include "TurretDamagedState.h"
#include "MsgHelp.h"
#include "MessageHub.h"
#include "LOLEntity.h"

void TurretDamagedState::OnEnter()
{
	this->attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
}

void TurretDamagedState::Update(float dt)
{
	if (!attachedEntity) { return; }
	MessageHub::GetInstance()->SendMsg(new MsgHelp({"Units", "Minions"}, attachedEntity->getPosition(), attachedEntity->Team));
	this->stateMachine->SetCurrState("Attack");

}
