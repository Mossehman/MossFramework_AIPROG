#pragma once
#include <EntityAI2D.h>
#include "ChampionSkills.h"
#include "IDamageable.h"
	
class LOLEntity : public EntityAI2D, public IDamageable
{	
protected:
	virtual void OnDamaged() override;
	virtual void OnDead() override;


public:
	int lane = 0;
	bool coutState = false;

	std::vector<LOLEntity*> targetEntities;
	bool isAlive = true;
	int Team = 0;
	int ClassType = 0;
	float TeamStrengthModifier = 0.0f;
	float RespawnTimer = 0.0f;

	float ViewRange = 200.0f;
	
	float BaseAttackCooldown = 1.0f;
	float BaseAttackTimer = 0.0f;
	

	LOLEntity(int ClassType, glm::vec2 pos = glm::vec2(0, 0), glm::vec2 rot = glm::vec2(0, 0), glm::vec2 scl = glm::vec2(1, 1));

	glm::vec2 spawnPos = glm::vec2(0, 0);

	int GetClassType();
	
	void Update(double dt) override;
	void Render() override;
	ChampionSkills* skills = nullptr;
	void AddSkill(BaseSkill* newSkill);


	
	virtual int HandleMessage(BaseMessage* message) override;

	int GetLane() { return this->lane; }



};

