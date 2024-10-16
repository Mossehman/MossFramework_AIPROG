#pragma once
#include <vector>
#include "Projectile2D.h"

class GunBase
{
protected:
	//max number of magazines in the gun
	unsigned int magazineCount;
	unsigned int magazineCountMax;
	//number of bullets in magazine
	unsigned int bulletCount;
	unsigned int bulletCountMax;
	//damage
	unsigned int damageCount;

	bool isExplosive;
	unsigned int splashDamage;
	float damageRadius;
	
	bool isAutomatic;
	bool isBurst;
	
	float shootInterval;
	unsigned int burstCount;
	float burstInterval;

	float reloadTime;

	float timerShoot;
	float timerReload;

	unsigned int shotCounter;

	bool startReload;



public:
	glm::vec2 shootPos;
	glm::vec2 shootDir;

	GunBase();
	~GunBase();

	std::vector<Projectile2D*> bullets;
	bool hasShot = false;

	virtual void Reload();
	virtual void Shoot();
	virtual void Update(double dt, glm::vec2 camPos, std::vector<Entity2D*> entities);

	void setMagCount(unsigned int magazineCount);
	unsigned int getMagCount(void);

	void setBulletCount(unsigned int bulletCount);
	unsigned int getBulletCount(void);


	void setMagCountMax(unsigned int magazineCountMax);
	unsigned int getMagCountMax(void);

	void setBulletCountMax(unsigned int bulletCountMax);
	unsigned int getBulletCountMax(void);
	
	void setDamage(unsigned int damage);
	unsigned int getDamage(void);

	void setExplosive(bool isExplosive);
	bool getExplosive(void);

	void setAutomatic(bool isAutomatic);
	bool getAutomatic(void);

	void setBurst(bool isBurst);
	bool getBurst(void);

	void setInterval(float shootInterval);
	float getInterval(void);

	void setBurstCount(unsigned int burstCount);
	unsigned int getBurstCount(void);

	void setBurstInterval(float burstInterval);
	float getBurstInterval(void);
	
	void setReloadTime(float reloadTime);
	float getReloadTime(void);

	void setShootPos(glm::vec2 shootPos);
	glm::vec2 getShootPos(void);

};

