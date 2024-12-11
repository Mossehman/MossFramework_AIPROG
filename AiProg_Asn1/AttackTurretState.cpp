#include "AttackTurretState.h"
#include "MessageHub.h"
#include "MsgGetUnits.h"
#include "LOLEntity.h"

#include <glm/gtx/norm.hpp>
#include <limits>

void AttackTurretState::OnEnter()
{
	attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	this->TowerCheckTimer = TowerCheckTime;
}

void AttackTurretState::Update(float dt)
{
	if (!attachedEntity) { return; }
	TowerCheckTimer += dt;
	attachedEntity->MoveAlongPath(dt);

	if (attachedEntity->targetEntities.empty())
	{
		if (TowerCheckTimer >= TowerCheckTime)
		{
			MessageHub::GetInstance()->SendMsg(new MsgGetUnits({ "Towers" }, OPPOSING_TEAMS, attachedEntity));
			TowerCheckTimer = 0.0f;
		}
	}
	else
	{
		float nearestDistance = std::numeric_limits<float>().max();

		for (LOLEntity* tower : attachedEntity->targetEntities)
		{
			if (glm::distance2(tower->getPosition(), attachedEntity->getPosition()) < nearestDistance)
			{
				nearestDistance = glm::distance2(tower->getPosition(), attachedEntity->getPosition());
				attachedEntity->targetPos = tower->getPosition();
				
			}
		}
		attachedEntity->SolveAStar();
		this->stateMachine->SetCurrState("Move"); //switch to move state
	}
}
