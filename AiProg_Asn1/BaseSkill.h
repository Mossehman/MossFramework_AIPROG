#pragma once
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include "MessageHub.h"
#include "MsgGetUnits.h"

class LOLEntity;


enum SkillTypes {
	ATTACK = 0,
	HEAL,
	BUFF_SELF,
	BUFF_TEAM,
	DEBUFF,
	MOVEMENT,
	SETUP,
	NUM_SKILL_TYPES
};

class BaseSkill
{
protected: 

	int HitEntities = 0;
	bool IsUsingSkill = false;
	float AttackCooldownTimer = 0.0f;
	std::vector<int> skillTypes;
	LOLEntity* championEntity = nullptr;
	std::vector<LOLEntity*> targetEntities;

	TeamSearchParam targetFilter = NUM_SEARCH_PARAMS;

public:
	bool CanOverlay = false;
	
	BaseSkill(std::initializer_list<int> types, TeamSearchParam msgType = NUM_SEARCH_PARAMS) : skillTypes(types), targetFilter(msgType) {}

	virtual void Init(LOLEntity* champion)
	{
		this->championEntity = champion;
	}

	std::vector<int> GetTypes() { return skillTypes; }

	bool IsType(int type)
	{
		for (int i : skillTypes)
		{
			if (i == type) { return true; }
		}
		return false;
	}

	float AttackCooldown = 1.0f;
	int AttackDamage = 5;
	int MaxTargetCount = 0;
	float AttackRange = 200.0f;

	virtual bool UseSkill(std::initializer_list<std::string> unitToTarget)
	{
		if (!CanUseSkill() || GetAttackStatus()) { return false; }
		IsUsingSkill = true;
		MessageHub::GetInstance()->SendMsg(new MsgGetUnits(unitToTarget, targetFilter, this->championEntity));
		OnSkillEnter();
		return true;
	}

	bool GetAttackStatus() { return IsUsingSkill; }
	virtual bool CanUseSkill() { return AttackCooldownTimer <= 0; }
 
	virtual void Update(float dt);

	virtual void OnSkillEnter() {}
	virtual void OnSkillEnd() {}

	virtual void WhileAttacking(float dt) = 0;
	virtual void Render() {}

	virtual void EndSkill() 
	{ 
		if (!IsUsingSkill) { return; }
		this->AttackCooldownTimer = AttackCooldown; 
		HitEntities = 0;
		IsUsingSkill = false;
		targetEntities.clear();
		OnSkillEnd(); 
	}

	void SetTargets(std::vector<LOLEntity*> targetEntities)
	{
		this->targetEntities = targetEntities;
	}
};

