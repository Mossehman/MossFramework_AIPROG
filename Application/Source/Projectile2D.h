#pragma once
#include "Entity2D.h"
class Projectile2D : public Entity2D
{
protected:
	float bulletSpeed;
	glm::vec2 bulletDirection;

	unsigned int damage;

	bool isExplode;
	float explosionRadius;
	unsigned int explosionDamage;

	glm::vec2 explosionForce = glm::vec2(1, 1);
	bool toExplode;

	glm::vec2 screenBounds;
	bool outOfBounds = false;
	
public:
	virtual void damageEntity(Entity2D* entity);
	virtual void splashDamage(std::vector<Entity2D*> entities);
	virtual void checkTiles(glm::vec2 cameraPos);
	virtual void Update(double dt, glm::vec2 cameraPos, std::vector<Entity2D*> entities);

	void setScreenBounds(glm::vec2 screenBounds);
	glm::vec2 getScreenBounds(void);

	void setDamage(unsigned int damage);
	unsigned int getDamage(void);

	void setExplode(bool isExplode);
	bool getExplode(void);

	void setExplosionRadius(float explosionRadius);
	float getExplosionRadius(void);

	void setExplosionDamage(unsigned int explosionDamage);
	unsigned int getExplosionDamage(void);

	void setExplosionForce(glm::vec2 explosionForce);
	glm::vec2 getExplosionForce(void);

	void setBulletSpeed(float bulletSpeed);
	float getBulletSpeed(void);

	void setBulletDirection(glm::vec2 bulletDirection);
	glm::vec2 getBulletDirection(void);

};

