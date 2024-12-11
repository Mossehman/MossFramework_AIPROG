#include "RetreatState.h"
#include "LOLEntity.h"
#include "MsgHelp.h"

#include "MessageHub.h"
#include "MsgGetUnitsInRange.h"
#include "Map2D.h"

#include <glm/gtx/norm.hpp>
#include <limits>

void RetreatState::OnEnter()
{
	this->attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	unitCheckTimer = 0;

	if (!attachedEntity) { return; }

}

void RetreatState::Update(float dt)
{
	this->stateMachine->GetAttachedEntity()->MoveAlongPath(dt);

	if (!attachedEntity) { return; }
	unitCheckTimer += dt;
	if (attachedEntity->targetEntities.empty())
	{
		if (unitCheckTimer >= unitCheckTime)
		{
			MessageHub::GetInstance()->SendMsg(new MsgGetUnits({ "Units" }, ALL , attachedEntity));
		}
	}
	else
	{
		attachedEntity->ResetPathWeight();
		float nearestTeammateDistance = std::numeric_limits<float>().max();
		bool foundTeammateWithinRange = false;
		bool enemyInRange = false;

		//float nearThreshold = (std::max)(attachedEntity->moveSpeed * attachedEntity->MoveSpeedMultiplier * 3.0f, attachedEntity->ViewRange * 2.0f);
		float nearThreshold = attachedEntity->ViewRange * 6.0f;
		float minDistance = attachedEntity->ViewRange * 1.5f; //ensure we do not target an entity that is too near (this means that they will still be forced to retreat regardless)

		for (LOLEntity* unit : attachedEntity->targetEntities)
		{
			if (unit->Team == attachedEntity->Team)
			{
				float distSq = glm::distance2(unit->getPosition(), attachedEntity->getPosition());
				if (distSq > nearThreshold * nearThreshold || distSq < minDistance * minDistance || distSq > nearestTeammateDistance) { continue; }
				nearestTeammateDistance = distSq;
				attachedEntity->targetPos = unit->getPosition();
				foundTeammateWithinRange = true;
			}
			else
			{
				int avoidanceRange = 3;

				glm::ivec2 tilePos = Map2D::GetInstance()->PosToTilePos(unit->getPosition());
				for (int y = tilePos.y - avoidanceRange; y <= tilePos.y + avoidanceRange; y++)
				{
					for (int x = tilePos.x - avoidanceRange; x <= tilePos.x + avoidanceRange; x++)
					{
						attachedEntity->SetPathWeightage(1000.0f, x, y);
					}
				}

				if (glm::distance2(unit->getPosition(), attachedEntity->getPosition()) <= this->attachedEntity->ViewRange * 2.0f * this->attachedEntity->ViewRange * 2.0f)
				{
					enemyInRange = true;
				}
			}
		}

		if (enemyInRange && (float)attachedEntity->health / (float)attachedEntity->MaxHealth < 0.4f)
		{
			MessageHub::GetInstance()->SendMsg(new MsgHelp({ "Units" }, attachedEntity->getPosition(), attachedEntity->Team));
		}
		else if (!enemyInRange && (float)attachedEntity->health / (float)attachedEntity->MaxHealth >= 0.4f)
		{
			this->attachedEntity->ResetPathWeight();
			this->stateMachine->SetCurrState("AttackTurret");
			return;
		}

		if (!foundTeammateWithinRange)
		{
			attachedEntity->targetPos = attachedEntity->spawnPos;
		}

		attachedEntity->SolveAStar();

		//if we are retreating, attempt to use a movement skill
		for (int i = 0; i < attachedEntity->skills->GetSkillList().size(); i++)
		{
			if (!attachedEntity->skills->GetSkillList()[i]->IsType(MOVEMENT) || !attachedEntity->skills->GetSkillList()[i]->CanUseSkill()) { continue; }
			if (attachedEntity->skills->UseSkill(i))
			{
				break;
			}
		}

		this->stateMachine->SetCurrState("Move");

	}
}
