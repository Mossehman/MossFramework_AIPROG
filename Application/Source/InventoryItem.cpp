#include "InventoryItem.h"
#include <iostream>

void InventoryItem::setInventoryTexture(const char* inventoryTexture)
{
	this->inventoryTexture = (char*)inventoryTexture;
}

char* InventoryItem::getInventoryTexture(void) const
{
	return inventoryTexture;
}

void InventoryItem::setStackCount(unsigned int stackCount)
{
	this->stackCount = std::max(1, (int)stackCount);
}

unsigned int InventoryItem::getStackCount(void)
{
	return stackCount;
}

void InventoryItem::setPickupState(bool isPickedUp)
{
	this->isPickedUp = isPickedUp;
}

bool InventoryItem::getPickupState(void)
{
	return isPickedUp;
}


