#include "IDamageable.h"
#include <glm/glm.hpp>

void IDamageable::Damage(int damageVal, float stunTimer)
{
	this->health -= damageVal;
	if (health <= 0)
	{
		OnDead();
		return;
	}
	this->stunTime = stunTimer;
	OnDamaged();
	CanRegenTimer = 0.0f;

}

void IDamageable::UpdateHP(float dt)
{
	if (!this->CanRegenHP) { RegenTimer = 0; }
	if (this->CanRegenTimer >= this->CanRegenThreshold)
	{
		this->RegenTimer += dt;
		if (this->RegenTimer >= this->RegenTimerThreshold)
		{
			this->health += this->RegenVal;
			RegenTimer = 0;
		}
	}
	else
	{
		RegenTimer = 0;
	}

	this->health = glm::clamp(this->health, 0, MaxHealth + MaxHealthModifier);
	stunTime = 0.0f;
}
