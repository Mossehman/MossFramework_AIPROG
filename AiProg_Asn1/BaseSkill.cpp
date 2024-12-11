#include "BaseSkill.h"
#include "LOLEntity.h"

void BaseSkill::Update(float dt)
{
	AttackCooldownTimer -= dt;
	if (IsUsingSkill)
	{
		if (targetEntities.empty() && targetFilter != NUM_SEARCH_PARAMS)
		{
			if (!this->championEntity->targetEntities.empty())
			{
				for (LOLEntity* entityToAdd : this->championEntity->targetEntities)
				{
					this->targetEntities.push_back(entityToAdd); //copy the target entities into the current target list
				}
			}

		}

		WhileAttacking(dt);
	}
}
