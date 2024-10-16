#pragma once
#include "DesignPatterns/SingletonTemplate.h"
#include <vector>
#include "InteractionEntity.h"

class InteractionManager : public SingletonTemplate<InteractionManager>
{
	friend SingletonTemplate<InteractionManager>;
	
public:
	void addToInteractionList(InteractionEntity* interactionEntity);
	InteractionEntity* getClosestEntity(void);

	void Update();
	void Render();

protected:
	Mesh* pointerMesh;
	float pointerHoverRange;

	~InteractionManager();

	std::vector<InteractionEntity*> interactionEntities;
	InteractionEntity* closestEntity;
};

