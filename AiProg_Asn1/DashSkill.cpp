#include "DashSkill.h"
#include "LOLEntity.h"

void DashSkill::OnSkillEnter()
{
	championEntity->MoveSpeedMultiplier += DashModifier;
	DashDurationTimer = DashDuration;
}

void DashSkill::OnSkillEnd()
{
	championEntity->MoveSpeedMultiplier -= DashModifier; 
}

void DashSkill::WhileAttacking(float dt)
{
	championEntity->MoveAlongPath(dt);
	DashDurationTimer -= dt;
	if (DashDurationTimer <= 0)
	{
		EndSkill();
		return;
	}

	for (LOLEntity* entityToDmg : targetEntities)
	{
		float dx = entityToDmg->getPosition().x - championEntity->getPosition().x;
		float dy = entityToDmg->getPosition().y - championEntity->getPosition().y;
		if ((dx * dx) + (dy * dy) > AttackRange * AttackRange) { continue; }

		entityToDmg->Damage(AttackDamage);
		HitEntities++;

		if (MaxTargetCount > 0 && HitEntities >= MaxTargetCount) {
			EndSkill();
			break;
		}
	}

}
