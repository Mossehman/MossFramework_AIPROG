#include "MoveState.h"
#include <EntityAI2D.h>
#include <MessageHub.h>
#include "MsgGetUnits.h"
#include "LOLEntity.h"
#include "MsgGetUnitsInRange.h"

#include <glm/gtx/norm.hpp>
#include "Map2D.h"

void MoveState::OnEnter()
{
	attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	EnemyCheckTimer = 0;
	EnemyCheckThresholdTimer = 0;
}

void MoveState::Update(float dt)
{
	this->stateMachine->GetAttachedEntity()->MoveAlongPath(dt);
	if (!attachedEntity) { return; }

	if (attachedEntity->targetEntities.empty())
	{
		EnemyCheckTimer += dt;
		EnemyCheckThresholdTimer += dt;
		if (EnemyCheckThresholdTimer >= EnemyCheckThresholdTime && attachedEntity->GetPathSize() <= 1)
		{
			this->stateMachine->SetCurrState("AttackTurret");
			return;
		}

		if (EnemyCheckTimer >= EnemyCheckTime)
		{
			MessageHub::GetInstance()->SendMsg(new MsgGetUnitsInRange({ "Units", "Minions", "Towers" }, ALL, attachedEntity, attachedEntity->ViewRange));
			EnemyCheckTimer = 0.0f;
		}
	}
	else
	{
		EnemyCheckTimer = 0;
		float TeamStrength = (attachedEntity->health / attachedEntity->MaxHealth) + attachedEntity->TeamStrengthModifier;
		float OpposingTeamStrength = 0;
		bool foundEnemy = false;

		for (LOLEntity* targetEntity : attachedEntity->targetEntities)
		{
			if (targetEntity->Team == attachedEntity->Team)
			{
				TeamStrength += ((float)targetEntity->health / (float)targetEntity->MaxHealth) + targetEntity->TeamStrengthModifier;
				//TODO: Change to account for target strength
			}
			else
			{
				foundEnemy = true;
				OpposingTeamStrength += ((float)targetEntity->health / (float)targetEntity->MaxHealth) + targetEntity->TeamStrengthModifier;
				//TODO: Change to account for target priority (high priority target decreases the strength and makes them more likely to fight), target strength, and if there is time, account for class weaknesses
			}
		}

		if (!foundEnemy) { return; }

		if (TeamStrength * 1.4f >= OpposingTeamStrength) //TODO: Change 1.5 to an agression float value
		{
			this->stateMachine->SetCurrState("Chase");
		}
		else
		{
			if (!this->stateMachine->SetCurrState("Retreat"))
			{
				this->stateMachine->SetCurrState("Chase");
			}
		}
	}
}
