#pragma once
#include "BaseSkill.h"

class Projectile;
#define MAX_PROJECTILES 20

class ProjectileSkill : public BaseSkill
{
protected:
	glm::vec2 ShootDir;
	Projectile* projectiles[MAX_PROJECTILES];
	float burstIntervalTimer = 0.0f;
	int shotsFired = 0;

	void Shoot();
	glm::vec2 GetNearestDir();

public:
	int ShotCount = 3;
	float burstInterval = 0.2f;

	virtual void OnSkillEnter() override;
	virtual void OnSkillEnd() override;
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual void WhileAttacking(float dt) override;

	ProjectileSkill(std::initializer_list<int> types, TeamSearchParam msgType = NUM_SEARCH_PARAMS);
	~ProjectileSkill();
};

