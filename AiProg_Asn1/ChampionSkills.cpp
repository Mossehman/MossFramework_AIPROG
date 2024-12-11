#include "ChampionSkills.h"

void ChampionSkills::AddToSkillList(BaseSkill* newSkill)
{
	newSkill->Init(this->champion);
	skills.push_back(newSkill);
}

int ChampionSkills::FindSkillType(int type)
{
	int returnVal = 0;
	for (BaseSkill* skill : skills)
	{
		for (int i = 0; i < skill->GetTypes().size(); i++)
		{
			if (skill->GetTypes()[i] == type)
			{
				returnVal++;
				break;
			}
		}
	}

	return returnVal;
}

std::vector<BaseSkill*> ChampionSkills::GetSkillList()
{
	return skills;
}

BaseSkill* ChampionSkills::GetCurrSkill()
{
	return currActiveSkill;
}

bool ChampionSkills::UseSkill(int index, std::initializer_list<std::string> unitsToTarget)
{
	if (index >= skills.size() || (!skills[index]->CanUseSkill() && !skills[index]->GetAttackStatus())) { return false; }
	if (!skills[index]->CanOverlay && currActiveSkill != nullptr) { return false; }


	if (skills[index]->UseSkill(unitsToTarget))
	{
		if (!skills[index]->CanOverlay)
		{
			currActiveSkill = skills[index];
		}
		return true;
	}
	return false;

}

void ChampionSkills::Update(float dt)
{
	for (BaseSkill* skill : skills)
	{
		skill->Update(dt);
	}
	if (currActiveSkill == nullptr) { return; }
	if (!currActiveSkill->CanUseSkill() && !currActiveSkill->GetAttackStatus())
	{
		currActiveSkill = nullptr;
	}
}

void ChampionSkills::Render()
{
	for (BaseSkill* skill : skills)
	{
		if (!skill) { continue; }
		skill->Render();
	}
}

ChampionSkills::~ChampionSkills()
{
	for (BaseSkill* skill : skills)
	{
		delete skill;
		skill = nullptr;
	}

	skills.clear();
}
