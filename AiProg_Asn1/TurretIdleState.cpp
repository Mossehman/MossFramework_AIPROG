#include "TurretIdleState.h"
#include "LOLEntity.h"
#include "MessageHub.h"
#include "MsgGetUnitsInRange.h"

void TurretIdleState::OnEnter()
{
	attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	checkForActivityTimer = 0.0f;
	spawnMinionsTimer = 0.0f;
}

void TurretIdleState::Update(float dt)
{
	if (!attachedEntity) { return; }
	checkForActivityTimer += dt;
	spawnMinionsTimer += dt;

	if (spawnMinionsTimer >= spawnMinionsTime)
	{
		if (this->stateMachine->SetCurrState("Spawn"))
		{
			spawnMinionsTimer = 0;
			checkForActivityTimer = 0;
			return;
		}
	}

	if (attachedEntity->targetEntities.empty())
	{
		if (checkForActivityTimer >= checkForActivityTime)
		{
			MessageHub::GetInstance()->SendMsg(new MsgGetUnitsInRange({ "Units", "Minions" }, OPPOSING_TEAMS, attachedEntity, attachedEntity->ViewRange));
			checkForActivityTimer = 0.0f;
		}
	}
	else
	{
		this->stateMachine->SetCurrState("Attack");
	}
}
