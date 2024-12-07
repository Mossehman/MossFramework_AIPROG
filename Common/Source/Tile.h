#pragma once
#include "GameObject.h"

class Tile : public GameObject
{
public:
	unsigned int tileID;
	int Passability = 0;
	bool isDangerous = false;

	Tile(int tileIndex, glm::vec2 pos = glm::vec2(0, 0), glm::vec2 rot = glm::vec2(0, 0), glm::vec2 scl = glm::vec2(0, 0))
	{
		tileID = tileIndex;
		position = pos;
		rotation = rot;
		scale = scl;
		Passability = 0;
		isDangerous = false;
	}
};
