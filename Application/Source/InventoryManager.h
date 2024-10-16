#pragma once
#include "GunBase.h"
#include "DesignPatterns/SingletonTemplate.h"
class InventoryManager: public SingletonTemplate<InventoryManager>
{
	friend SingletonTemplate<InventoryManager>;

protected:
	GunBase* primaryGun;

public:
	std::vector<Entity2D*> entities;
	//std::vector<Entity2D*> enemies;
	glm::vec2 positionReference;

	GunBase*& getPrimaryGun(void);
	void setPrimaryGun(GunBase* newGun);

	virtual void Update(double dt);
	virtual void Render();

};