#include "ProjectileSkill.h"
#include "LOLEntity.h"
#include "Projectile.h"

#include <limits>
#include <glm/gtx/norm.hpp>


void ProjectileSkill::WhileAttacking(float dt)
{
	burstIntervalTimer += dt;
	if (targetEntities.empty()) { return; }
	if (burstIntervalTimer >= burstInterval)
	{
		Shoot();
		burstIntervalTimer = 0;
	}

	if (shotsFired >= ShotCount)
	{
		EndSkill();
	}

}

void ProjectileSkill::Shoot()
{
	for (int i = 0; i < MAX_PROJECTILES; i++)
	{
		if (!projectiles[i] || projectiles[i]->IsActive) { continue; }
		projectiles[i]->IsActive = true;
		projectiles[i]->setPosition(this->championEntity->getPosition());
		projectiles[i]->currentLevel = this->championEntity->currentLevel;
		projectiles[i]->Direction = GetNearestDir();
		for (int j = 0; j < targetEntities.size(); j++)
		{
			projectiles[i]->EntitiesToCollideWith.push_back(targetEntities[j]);
		}
		projectiles[i]->Damage = AttackDamage;
		shotsFired++;
		break;
	}
}

glm::vec2 ProjectileSkill::GetNearestDir()
{
	float NearestDistance = std::numeric_limits<float>().max();
	glm::vec2 directionToReturn = glm::vec2(0, 0);

	for (int i = 0; i < targetEntities.size(); i++)
	{
		//if (glm::distance2(championEntity->getPosition(), targetEntities[i]->getPosition()) >= AttackRange * AttackRange) { continue; }
		if (glm::distance2(championEntity->getPosition(), targetEntities[i]->getPosition()) >= NearestDistance) { continue; }
		NearestDistance = glm::distance2(championEntity->getPosition(), targetEntities[i]->getPosition());
		directionToReturn = targetEntities[i]->getPosition() - championEntity->getPosition();
	}

	return glm::normalize(directionToReturn);
}

void ProjectileSkill::OnSkillEnter()
{
	burstIntervalTimer = burstInterval;
	shotsFired = 0;

}

void ProjectileSkill::OnSkillEnd()
{
}

void ProjectileSkill::Update(float dt)
{
	BaseSkill::Update(dt);
	for (int i = 0; i < MAX_PROJECTILES; i++)
	{
		if (!projectiles[i] || !projectiles[i]->IsActive) { continue; }
		projectiles[i]->Update(dt);
		if (projectiles[i]->Collided)
		{
			projectiles[i]->Collided = false;
			projectiles[i]->IsActive = false;
			projectiles[i]->EntitiesToCollideWith.clear();
		}
	}


}

void ProjectileSkill::Render()
{
	for (int i = 0; i < MAX_PROJECTILES; i++)
	{
		if (!projectiles[i] || !projectiles[i]->IsActive) { continue; }
		projectiles[i]->Render();
	}
}

ProjectileSkill::~ProjectileSkill()
{
	for (int i = 0; i < MAX_PROJECTILES; i++)
	{
		delete projectiles[i];
	}
}

ProjectileSkill::ProjectileSkill(std::initializer_list<int> types, TeamSearchParam msgType) : BaseSkill(types, msgType)
{
	for (int i = 0; i < MAX_PROJECTILES; i++)
	{
		projectiles[i] = new Projectile();

		projectiles[i]->setHitboxSize(glm::vec2(25, 25));
		projectiles[i]->setScale(glm::vec2(25, 25));
		projectiles[i]->SetMesh(MeshBuilder::GenerateQuad("quad", Color(1, 1, 1)));
		projectiles[i]->SetTexture("Image/Pan_Blue_Circle.png");

	}
}
