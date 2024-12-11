#include "AttackState.h"
#include "LOLEntity.h"
#include "SetupSkill.h"

void AttackState::Init(FiniteStateMachine* stateMachine)
{
	FiniteState::Init(stateMachine);
	this->attachedEntity = dynamic_cast<LOLEntity*>(stateMachine->GetAttachedEntity());
}

void AttackState::OnEnter()
{
	for (int i = 0; i < attachedEntity->skills->GetSkillList().size(); i++)
	{
		if (!attachedEntity->skills->GetSkillList()[i]->IsType(ATTACK) || !attachedEntity->skills->GetSkillList()[i]->CanUseSkill()) { continue; }
		if (attachedEntity->skills->GetSkillList()[i]->IsType(SETUP))
		{
			SetupSkill* setupSkill = dynamic_cast<SetupSkill*>(attachedEntity->skills->GetSkillList()[i]);
			if (setupSkill) {
				setupSkill->Setup();
				continue;
			}
		}
		skill = attachedEntity->skills->GetSkillList()[i]; 
		break;
	}
}

void AttackState::OnExit()
{
	if (!this->attachedEntity) { return; }
	HasUsedSkill = false;
	skill->EndSkill();
	//for (int i = 0; i < attachedEntity->skills->GetSkillList().size(); i++)
	//{
	//	attachedEntity->skills->GetSkillList()[i]->EndSkill();
	//}
}

void AttackState::Update(float dt)
{
	attachedEntity->MoveAlongPath(dt);

	for (int i = 0; i < attachedEntity->skills->GetSkillList().size(); i++)
	{
		if (skill != attachedEntity->skills->GetSkillList()[i]) { continue; }
		if (attachedEntity->skills->UseSkill(i, { "Units", "Minions", "Towers" }))
		{
			HasUsedSkill = true;
		}
	}

	if (attachedEntity->skills->GetCurrSkill() ) { return; }

	this->stateMachine->SetCurrState("Move");
}
