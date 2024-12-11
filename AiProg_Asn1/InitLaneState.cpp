#include "InitLaneState.h"
#include "LOLEntity.h"
#include "MessageHub.h"

#include "MsgGetUnits.h"

#include <glm/gtx/norm.hpp>
#include <limits>

void InitLaneState::OnEnter()
{
	this->attachedEntity = dynamic_cast<LOLEntity*>(this->stateMachine->GetAttachedEntity());
	this->LaneCheckTimer = LaneCheckTime;
	this->MaxTimeWaitingTimer = 0.0f;
}

void InitLaneState::Update(float dt)
{
	if (!attachedEntity) { return; }
	MaxTimeWaitingTimer += dt;
	LaneCheckTimer += dt;

	if (attachedEntity->targetEntities.empty())
	{
		if (LaneCheckTimer >= LaneCheckTime)
		{
			LaneCheckTimer = 0;
			switch (attachedEntity->GetLane())
			{
			case 1:
				MessageHub::GetInstance()->SendMsg(new MsgGetUnits({ "TowersTop", "InhibitorTower" }, OPPOSING_TEAMS, attachedEntity));

				break;

			case 0:
				MessageHub::GetInstance()->SendMsg(new MsgGetUnits({ "TowersMid", "InhibitorTower" }, OPPOSING_TEAMS, attachedEntity));

				break;

			case -1:
				MessageHub::GetInstance()->SendMsg(new MsgGetUnits({ "TowersBot", "InhibitorTower" }, OPPOSING_TEAMS, attachedEntity));
				break;
			}
		}
	}
	else
	{

		float nearestDistance = std::numeric_limits<float>().max();
		for (LOLEntity* tower : attachedEntity->targetEntities)
		{
			float distance = glm::distance2(tower->getPosition(), attachedEntity->getPosition());
			if (distance < nearestDistance)
			{
				nearestDistance = distance;
				attachedEntity->targetPos = tower->getPosition();
			}
		}
		//std::cout << attachedEntity->targetPos.x << ", " << attachedEntity->targetPos.y << std::endl;
		attachedEntity->SolveAStar();
		this->stateMachine->SetCurrState("Move");
	}
}
