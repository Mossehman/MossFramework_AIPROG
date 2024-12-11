#pragma once
#include "BaseSkill.h"
#include "SetupSkill.h"

#include <string>

class LOLEntity;

class ChampionSkills
{
protected:
	std::vector<BaseSkill*> skills;
	BaseSkill* currActiveSkill = nullptr;
	LOLEntity* champion = nullptr;

public:

	ChampionSkills(LOLEntity* champion)
	{
		this->champion = champion;
	}

	void AddToSkillList(BaseSkill* newSkill);
	int FindSkillType(int type);

	std::vector<BaseSkill*> GetSkillList();
	BaseSkill* GetCurrSkill();
	bool UseSkill(int index, std::initializer_list<std::string> unitsToTarget = { "Units" });

	void Update(float dt);
	void Render();

	~ChampionSkills();
};

