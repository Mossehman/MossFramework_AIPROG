#include "ChaseState.h"
#include "LOLEntity.h"
#include "MessageHub.h"
#include "MsgGetUnitsInRange.h"
#include "BaseSkill.h"

#include <glm/gtx/norm.hpp>
#include <limits>

void ChaseState::OnEnter()
{
	attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	EnemyCheckTimer = 0;
	EnemyCheckThresholdTimer = 0;
}

void ChaseState::Update(float dt)
{
	if (!attachedEntity) { return; }
	
	attachedEntity->MoveAlongPath(dt); //while we have not found any entity, move along the astar path
	EnemyCheckTimer += dt;
	
	if (attachedEntity->targetEntities.empty())
	{
		EnemyCheckThresholdTimer += dt;
		if (EnemyCheckThresholdTimer >= EnemyCheckThresholdTime)
		{
			this->stateMachine->SetCurrState("AttackTurret");
			return;
		}

		if (EnemyCheckTimer >= EnemyCheckTime)
		{
			MessageHub::GetInstance()->SendMsg(new MsgGetUnitsInRange({ "Units", "Minions", "Towers" }, OPPOSING_TEAMS, attachedEntity, attachedEntity->ViewRange * 1.5f));
			EnemyCheckTimer = 0.0f;
		}
	}
	else
	{
		EnemyCheckThresholdTimer = 0;
		float nearestDistance = std::numeric_limits<float>().max();
		glm::vec2 nearestPos;
		for (LOLEntity* entityToCheck : attachedEntity->targetEntities)
		{
			bool IsNearEnough = false;

			if (glm::distance2(entityToCheck->getPosition(), attachedEntity->getPosition()) < nearestDistance)
			{
				for (BaseSkill* skill : attachedEntity->skills->GetSkillList())
				{
					if (!skill->IsType(ATTACK) || !skill->CanUseSkill()) { continue; }
					if (skill->AttackRange * skill->AttackRange < nearestDistance)
					{
						IsNearEnough = true;
					}
				}

				nearestDistance = glm::distance2(entityToCheck->getPosition(), attachedEntity->getPosition());
				if (!IsNearEnough)
				{
					attachedEntity->targetPos = entityToCheck->getPosition();
				}
				else
				{
					attachedEntity->targetPos = attachedEntity->getPosition();
				}
			}
		}

		attachedEntity->SolveAStar(); //get the astar path with the entity

		for (BaseSkill* skill : attachedEntity->skills->GetSkillList())
		{
			if (!skill->IsType(ATTACK) || !skill->CanUseSkill() || skill->AttackRange * skill->AttackRange < nearestDistance) { continue; } 
			if (this->stateMachine->SetCurrState("Attack"))
			{
				return;
			}
		}
		
		this->stateMachine->SetCurrState("Move");
	}
}
