#pragma once
#include "GameObject.h"
 
class InventoryItem: public GameObject
{
protected:
	unsigned int stackCount;
	bool isPickedUp;
	char* inventoryTexture;

public:
	void setInventoryTexture(const char* inventoryTexture);
	char* getInventoryTexture(void) const;

	void setStackCount(unsigned int stackCount);
	unsigned int getStackCount(void);

	void setPickupState(bool isPickedUp);
	bool getPickupState(void);
};