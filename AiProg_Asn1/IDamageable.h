#pragma once
class IDamageable
{
public:
	int health;
	int MaxHealth = 100;
	int MaxHealthModifier = 0;
	
	int RegenVal = 10;
	float RegenTimerThreshold = 2.0f;
	float RegenTimer = 0.0f;

	//If we have not taken damage beyond this set period of time, we can start regenerating HP
	float CanRegenThreshold = 5.0f;
	float CanRegenTimer = 0.0f;

	float stunTime = 0.0f;

	bool CanRegenHP = true;

	void Damage(int damageVal, float stunTimer = 0.0f);

protected:
	virtual void OnDead() { return; }
	virtual void OnDamaged() { return; }
	virtual void UpdateHP(float dt);

};

