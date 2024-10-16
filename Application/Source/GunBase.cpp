#include "GunBase.h"
#include <iostream>
#include "MouseController.h"
#include "Map2D.h"

GunBase::GunBase(): isExplosive(false), magazineCount(4), magazineCountMax(4), bulletCount(40), 
bulletCountMax(40), isAutomatic(true), shootInterval(0.2f), isBurst(false), reloadTime(1), damageCount(10), splashDamage(5), damageRadius(300)
{
}

GunBase::~GunBase()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i])
		{
			delete bullets[i];
			bullets[i] = nullptr;
		}
	}
	bullets.clear();
}

void GunBase::Reload()
{
	if (magazineCount > 0 && !startReload)
	{
		startReload = true;
		magazineCount--;
		timerReload = reloadTime;
	}
}

void GunBase::Shoot()
{
	if (timerShoot > 0 || bulletCount <= 0 || startReload) { return; }

	//hasShot = true;
	shotCounter++;
	bulletCount--;
	
	if (isBurst && shotCounter >= burstCount)
	{
		timerShoot = burstInterval;
		shotCounter = 0;
	}
	else
	{
		timerShoot = shootInterval;
	}

	Projectile2D* newProjectile = new Projectile2D();
	newProjectile->setBulletDirection(shootDir);
	newProjectile->setBulletSpeed(700);
	newProjectile->setExplode(isExplosive);
	newProjectile->setDamage(damageCount);
	newProjectile->setExplosionDamage(this->splashDamage);
	newProjectile->setExplosionForce(glm::vec2(10, 10));
	newProjectile->setExplosionRadius(damageRadius);
	newProjectile->setHitboxSize(glm::vec2(8, 8));
	newProjectile->setHitboxOffset(glm::vec2(0, 0));
	newProjectile->setMesh(MeshBuilder::GeneratePolygon("Bullet", Color(0, 1, 0.5)));
	newProjectile->setPosition(shootPos);
	newProjectile->setScale(glm::vec2(10, 10));
	newProjectile->Init(Map2D::GetInstance()->getCurrentLevel());

	bullets.push_back(newProjectile);
	
}

void GunBase::Update(double dt, glm::vec2 camPos, std::vector<Entity2D*> entities)
{
	timerShoot -= dt;
	timerReload -= dt;
	if (startReload && timerReload <= 0)
	{
		bulletCount = bulletCountMax;
		startReload = false;
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		if (!bullets[i]) { continue; }
		if (bullets[i]->getToDestroy())
		{
			delete bullets[i];
			bullets[i] = nullptr;
			//bullets.erase(bullets.begin() + i);
			continue;
		}

		bullets[i]->Update(dt, camPos, entities);

		for (int j = 0; j < entities.size(); j++)
		{
			if (bullets[i]->checkEntityCollision(entities[j]))
			{
				bullets[i]->damageEntity(entities[j]);
			}
		}
	}
}

void GunBase::setMagCount(unsigned int magazineCount)
{
	this->magazineCount = magazineCount;
}

unsigned int GunBase::getMagCount(void)
{
	return magazineCount;
}

void GunBase::setBulletCount(unsigned int bulletCount)
{
	this->bulletCount = bulletCount;
}

unsigned int GunBase::getBulletCount(void)
{
	return bulletCount;
}

void GunBase::setMagCountMax(unsigned int magazineCountMax)
{
	this->magazineCountMax = magazineCountMax;
}

unsigned int GunBase::getMagCountMax(void)
{
	return magazineCountMax;
}

void GunBase::setBulletCountMax(unsigned int bulletCountMax)
{
	this->bulletCountMax = bulletCountMax;
}

unsigned int GunBase::getBulletCountMax(void)
{
	return bulletCountMax;
}

void GunBase::setDamage(unsigned int damage)
{
	this->damageCount = damage;
}

unsigned int GunBase::getDamage(void)
{
	return damageCount;
}

void GunBase::setExplosive(bool isExplosive)
{
	this->isExplosive = isExplosive;
}

bool GunBase::getExplosive(void)
{
	return isExplosive;
}

void GunBase::setAutomatic(bool isAutomatic)
{
	this->isAutomatic = isAutomatic;
}

bool GunBase::getAutomatic(void)
{
	return isAutomatic;
}

void GunBase::setBurst(bool isBurst)
{
	this->isBurst = isBurst;
}

bool GunBase::getBurst(void)
{
	return isBurst;
}

void GunBase::setInterval(float shootInterval)
{
	
	this->shootInterval = std::max(0.0001f, shootInterval);
}

float GunBase::getInterval(void)
{
	return shootInterval;
}

void GunBase::setBurstCount(unsigned int burstCount)
{
	this->burstCount = burstCount;
}

unsigned int GunBase::getBurstCount(void)
{
	return burstCount;
}

void GunBase::setBurstInterval(float burstInterval)
{
	this->burstInterval = std::max(0.0001f, burstInterval);
}

float GunBase::getBurstInterval(void)
{
	return burstInterval;
}

void GunBase::setReloadTime(float reloadTime)
{
	this->reloadTime = std::max(0.0001f, reloadTime);
}

float GunBase::getReloadTime(void)
{
	return reloadTime;
}

void GunBase::setShootPos(glm::vec2 shootPos)
{
	this->shootPos = shootPos;
}

glm::vec2 GunBase::getShootPos(void)
{
	return shootPos;
}
