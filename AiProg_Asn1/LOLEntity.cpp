#include "LOLEntity.h"
#include "MessageHub.h"
#include "MessageTypes.h"
#include "MsgGetUnits.h"
#include "MsgGetUnitsInRange.h"
#include "MsgLane.h"
#include "MsgHelp.h"
#include "MsgGetRespawnTime.h"
#include <glm/gtx/norm.hpp>
#include "MsgGetEntityCount.h"

void LOLEntity::OnDamaged()
{
	this->stateMachine->SetCurrState("Damaged");
}

void LOLEntity::OnDead()
{
	this->isAlive = false;
	this->stateMachine->SetCurrState("Dead");
}

LOLEntity::LOLEntity(int ClassType, glm::vec2 pos, glm::vec2 rot, glm::vec2 scl) : EntityAI2D(pos, rot, scl)
{
	this->ClassType = ClassType;
	skills = new ChampionSkills(this);
	this->health = MaxHealth;
}

int LOLEntity::GetClassType()
{
	return this->ClassType;
}

void LOLEntity::Update(double dt)
{
	stateMachine->Update(dt);
	skills->Update(dt);
	this->UpdateHP(dt);

	this->toRender = this->isAlive;

	if (coutState)
	{
		std::cout << this->GetFSM()->GetCurrStateName() << std::endl;
	}

	targetEntities.clear();
}

void LOLEntity::Render()
{
	EntityAI2D::Render();
	skills->Render();
	this->RenderNodePath(isAlive);

	
}

void LOLEntity::AddSkill(BaseSkill* newSkill)
{
	if (!newSkill) { return; }
	skills->AddToSkillList(newSkill);
	
}

int LOLEntity::HandleMessage(BaseMessage* message)
{
	if (message->GetMessageType() == GET_UNITS)
	{
		MsgGetUnits* msgGetUnits = dynamic_cast<MsgGetUnits*>(message);
		if (!msgGetUnits) { return -1; }
	
		if (msgGetUnits->teamSearch == REPLY)
		{
			this->targetEntities.push_back(msgGetUnits->senderEntity);
		}
		else if (msgGetUnits->teamSearch == ALL ||
			(msgGetUnits->teamSearch == CURR_TEAM && msgGetUnits->senderEntity->Team == this->Team) ||
			(msgGetUnits->teamSearch == OPPOSING_TEAMS && msgGetUnits->senderEntity->Team != this->Team) ||
			msgGetUnits->teamToSearch == this->Team)
		{
			if (this == msgGetUnits->senderEntity) { return -1; }
			if (!this->isAlive) { return -1; }
			std::vector<IMessageReciever*> SendToEntity = { msgGetUnits->senderEntity };
			MsgGetUnits* newMsg = new MsgGetUnits(SendToEntity, REPLY, this);
			MessageHub::GetInstance()->SendMsg(newMsg);
	
		}
	}
	else if (message->GetMessageType() == GET_UNITS_IN_RANGE)
	{
		MsgGetUnitsInRange* msgGetUnitsInRange = dynamic_cast<MsgGetUnitsInRange*>(message);
		if (!msgGetUnitsInRange) { return -1; }
	
		if (msgGetUnitsInRange->teamSearch == ALL ||
			(msgGetUnitsInRange->teamSearch == CURR_TEAM && msgGetUnitsInRange->senderEntity->Team == this->Team) ||
			(msgGetUnitsInRange->teamSearch == OPPOSING_TEAMS && msgGetUnitsInRange->senderEntity->Team != this->Team))
		{

			if (this == msgGetUnitsInRange->senderEntity) { return -1; }
			if (!this->isAlive) { return -1; }
			if (glm::distance2(this->position, msgGetUnitsInRange->radiusOrigin) > msgGetUnitsInRange->viewRadius * msgGetUnitsInRange->viewRadius) { return -1; }
	
			std::vector<IMessageReciever*> SendToEntity = { msgGetUnitsInRange->senderEntity };
			MsgGetUnits* newMsg = new MsgGetUnits(SendToEntity, REPLY, this);
			MessageHub::GetInstance()->SendMsg(newMsg);
	
		}
	}
	else if (message->GetMessageType() == GET_LANE)
	{
		MsgLane* msgLane = dynamic_cast<MsgLane*>(message);
		if (!msgLane) { return -1; }
		this->lane = msgLane->lane;

		stateMachine->SetCurrState("InitLane");
	}
	else if (message->GetMessageType() == GET_HELP)
	{
		MsgHelp* msgHelp = dynamic_cast<MsgHelp*>(message);
		if (!msgHelp) { return -1; }
		if (this->Team != msgHelp->team) { return -1; }
		if (!this->isAlive) { return -1; }

		this->targetPos = msgHelp->helpPos;
		SolveAStar();
	}
	else if (message->GetMessageType() == MSG_GETRESPAWNTIME)
	{
		MsgGetRespawnTime* getRespawnTime = dynamic_cast<MsgGetRespawnTime*>(message);
		if (!getRespawnTime) { return -1; }

		this->RespawnTimer = getRespawnTime->respawnTime;
		this->stateMachine->SetCurrState("Respawn");
	}
	else if (message->GetMessageType() == DESTROY_ENTITY)
	{
		if (this->isAlive)
		{
			this->Damage(MaxHealth + 1);
		}
	}
	else if (message->GetMessageType() == MSG_GETENTITYCOUNT)
	{
		MsgGetEntityCount* getEntityCount = dynamic_cast<MsgGetEntityCount*>(message);
		if (getEntityCount->entityClass != this->ClassType) { return 0; }
		if (getEntityCount->team != this->Team) { return 0; }
		if (!isAlive) { return 0; }
		return 1;
	}

	return 0;
}


