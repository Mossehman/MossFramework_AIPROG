#include "InteractionManager.h"

void InteractionManager::addToInteractionList(InteractionEntity* interactionEntity)
{
	interactionEntities.push_back(interactionEntity);
}

InteractionEntity* InteractionManager::getClosestEntity(void)
{
	return closestEntity;
}

void InteractionManager::Update()
{
	float distanceCheck = std::numeric_limits<float>::infinity();

	//clear the current closestEntity
	if (closestEntity) { closestEntity = nullptr; }

	for (int i = 0; i < interactionEntities.size(); i++)
	{
		interactionEntities[i]->Update();
		//we are outside of this entity's interaction range, do not check further
		if (!interactionEntities[i]->getInteractionState()) { continue; }

		//get the closest interaction entity to the player by comparing all the distance squared for the entities within interaction range and getting the setting the entity with the smallest distance as the closest entity
		if (interactionEntities[i]->getDistFromPlayerSq() < distanceCheck)
		{
			closestEntity = interactionEntities[i];
		}
	}

	//we cannot interact with any of the entities, do not check further
	if (!closestEntity) { return; }
}

InteractionManager::~InteractionManager()
{
	for (int i = 0; i < interactionEntities.size(); i++)
	{
		if (interactionEntities[i])
		{
			//on destroy, cleanup all of our interaction entities
			delete interactionEntities[i];
		}
	}

	interactionEntities.clear();
}
