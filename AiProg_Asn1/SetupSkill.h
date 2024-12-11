#pragma once
#include "BaseSkill.h"

class SetupSkill : public BaseSkill
{
protected:
	virtual void OnSetup() = 0;

public:
	SetupSkill(std::initializer_list<int> types, TeamSearchParam msgType = NUM_SEARCH_PARAMS) : BaseSkill(types, msgType) {
		if (!IsType(SETUP))
		{
			this->skillTypes.push_back(SETUP);
		}
	}

	bool IsSetup = false;
	void Setup()
	{
		if (!CanUseSkill() || IsSetup) { return; }

		OnSetup();
		IsSetup = true;
	}

	virtual bool UseSkill(std::initializer_list<std::string> unitToTarget = { "Units" }) override
	{
		if ((!CanUseSkill() || GetAttackStatus()) || !IsSetup) { return false; }
		IsUsingSkill = true;
		OnSkillEnter();
		MessageHub::GetInstance()->SendMsg(new MsgGetUnits(unitToTarget, targetFilter, this->championEntity));
		return true;
	}

	virtual void EndSkill() override
	{
		if (!IsUsingSkill) { return; }
		this->AttackCooldownTimer = AttackCooldown;
		HitEntities = 0;
		IsUsingSkill = false;
		IsSetup = false;
		targetEntities.clear();
		OnSkillEnd();
	}
};