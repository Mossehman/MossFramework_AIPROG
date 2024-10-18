#include "Projectile2D.h"
#include "Map2D.h"


void Projectile2D::damageEntity(Entity2D* entity)
{
	if (checkEntityCollision(entity))
	{
		entity->setHealth(entity->getHealth() - damage);
		toDestroy = true;
	}
}

void Projectile2D::splashDamage(std::vector<Entity2D*> entities)
{
	if (!toDestroy || !isExplode || outOfBounds) 
	{ 
		return; 
	}
	for (int i = 0; i < entities.size(); i++)
	{
		if ((entities[i]->getPosition().x - this->position.x) * (entities[i]->getPosition().x - this->position.x) + 
			(entities[i]->getPosition().y - this->position.y) * (entities[i]->getPosition().y - this->position.y) > explosionRadius * explosionRadius) {
			continue;
		}

		entities[i]->setHealth(entities[i]->getHealth() - explosionDamage);
		entities[i]->setForce(glm::normalize(entities[i]->getPosition() - this->position) * explosionForce);
	}

	unsigned int tileRadiusX = std::ceil(explosionRadius / Map2D::GetInstance()->mapSizeX[Map2D::GetInstance()->GetCurrentLevel()]);
	unsigned int tileRadiusY = std::ceil(explosionRadius / Map2D::GetInstance()->mapSizeY[Map2D::GetInstance()->GetCurrentLevel()]);
	

	for (int y = Map2D::GetInstance()->PosToTilePos(this->position).y - tileRadiusY; y < Map2D::GetInstance()->PosToTilePos(this->position).y + tileRadiusY * 2; y++)
	{
		//std::cout << y << std::endl;
		for (int x = Map2D::GetInstance()->PosToTilePos(this->position).x - tileRadiusX; x < Map2D::GetInstance()->PosToTilePos(this->position).x + tileRadiusX * 2; x++)
		{
			if (Map2D::GetInstance()->GetMapInfo(y, x, Map2D::GetInstance()->GetCurrentLevel()) != 0)
			{
				Map2D::GetInstance()->SetMapInfo(y, x, 0, Map2D::GetInstance()->GetCurrentLevel());
			}
		}
	}
}

void Projectile2D::checkTiles(glm::vec2 cameraPos)
{
	if (this->position.x < minPos.x || this->position.y < minPos.y ||
		this->position.x > maxPos.x || this->position.y > maxPos.y)
	{

		std::cout << "Out of bounds" << std::endl;
		toDestroy = true;
	}

	//if (this->position.x < cameraPos.x - screenBounds.x || this->position.y < cameraPos.y - screenBounds.y ||
	//	this->position.x > cameraPos.x + screenBounds.x || this->position.y > cameraPos.y + screenBounds.y)
	//{
	//	toDestroy = true;
	//	outOfBounds = true;
	//}
}

void Projectile2D::Update(double dt, glm::vec2 cameraPos, std::vector<Entity2D*> entities)
{

	this->velocity = (glm::normalize(bulletDirection)) * (bulletSpeed);
	this->doPhysics(dt);
	this->checkPosUpdate();
	this->checkMinMaxPos();


	checkTiles(cameraPos);
	splashDamage(entities);
}

void Projectile2D::setScreenBounds(glm::vec2 screenBounds)
{
	this->screenBounds = screenBounds;
}

glm::vec2 Projectile2D::getScreenBounds(void)
{
	return screenBounds;
}

void Projectile2D::setDamage(unsigned int damage)
{
	this->damage = damage;
}

unsigned int Projectile2D::getDamage(void)
{
	return damage;
}

void Projectile2D::setExplode(bool isExplode)
{
	this->isExplode = isExplode;
}

bool Projectile2D::getExplode(void)
{
	return isExplode;
}

void Projectile2D::setExplosionRadius(float explosionRadius)
{
	this->explosionRadius = std::max(0.0001f, explosionRadius);
}

float Projectile2D::getExplosionRadius(void)
{
	return explosionRadius;
}

void Projectile2D::setExplosionDamage(unsigned int explosionDamage)
{
	this->explosionDamage = explosionDamage;
}

unsigned int Projectile2D::getExplosionDamage(void)
{
	return explosionDamage;
}

void Projectile2D::setExplosionForce(glm::vec2 explosionForce)
{
	this->explosionForce = explosionForce;
}

glm::vec2 Projectile2D::getExplosionForce(void)
{
	return explosionForce;
}

void Projectile2D::setBulletSpeed(float bulletSpeed)
{
	this->bulletSpeed = bulletSpeed;
}

float Projectile2D::getBulletSpeed(void)
{
	return bulletSpeed;
}

void Projectile2D::setBulletDirection(glm::vec2 bulletDirection)
{
	this->bulletDirection = bulletDirection;
}

glm::vec2 Projectile2D::getBulletDirection(void)
{
	return bulletDirection;
}
