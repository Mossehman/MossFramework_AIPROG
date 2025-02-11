#pragma once
#include <EntityAI2D.h>
#include "TurnBasedEntity.h"
#include <set>
#include "ivec2Compare.h"

class Enemy : public EntityAI2D, public TurnBasedEntity
{
public:
    bool startedMoving;
    bool finishedMoving = true;

    glm::vec2 destination;
    int remainingMovement = 0;
    int visionRange = 3;

    std::vector<glm::ivec2> walkableAreas;
    std::vector<glm::ivec2> visionTiles;
    

    virtual void OnTurnStart() override;
    virtual void OnMovementStart(glm::vec2 destination) override;
    virtual void OnTurnEnd() override;

    virtual void Update(double dt) override;

    void CalculateVisionRange();

    int HandleMessage(BaseMessage* msg) override;

    virtual glm::vec2 GetDestination();
};

