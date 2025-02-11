#pragma once
#include <GameObject.h>
#include "TurnBasedEntity.h"
#include <list>
#include <set>
#include "ivec2Compare.h"

class Player : public GameObject, public TurnBasedEntity
{
private:
    bool startedMoving = false;

public:
    int health = 5;
	std::vector<GameObject*> highlightTiles;
    std::set<glm::ivec2, ivec2Compare> walkableAreas;

	void OnTurnStart() override;
	void OnMovementStart(glm::vec2 destination) override;
	void OnTurnEnd() override;

    void RenderTiles();
    void RenderPath(Color color);

    void Update(double dt) override;

    int HandleMessage(BaseMessage* msg) override;
};



struct TileNode {
    glm::ivec2 pos;
    float cost;
};

