#pragma once
#include <GameObject.h>
#include <list>
#include <set>

struct ivec2Compare {
    bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
        if (a.x < b.x) {
            return true;
        }
        if (a.x > b.x) {
            return false;
        }
        return a.y < b.y;
    }
};

class Player : public GameObject
{
public:
	int movementRange = 5;
	std::vector<GameObject*> highlightTiles;
    std::set<glm::ivec2, ivec2Compare> walkableAreas;

    std::vector<glm::vec2> path;

	void OnTurnStart();
	void OnMovementStart(glm::vec2 destination);

    void RenderTiles();
    void RenderPath(Color color);

    void Update(double dt) override;
};



struct TileNode {
    glm::ivec2 pos;
    float cost;
};

