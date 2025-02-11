#pragma once
#include <DesignPatterns/SingletonTemplate.h>
#include "Enemy.h"
#include "Player.h"
#include <vector>

class EnemyManager : public SingletonTemplate<EnemyManager>
{
	friend SingletonTemplate<EnemyManager>;
public:

	Player* enemyPlayer;
	bool isTurn = false;
	std::vector<Enemy*> enemies; //keep a list of all our enemies
	std::set<glm::ivec2, ivec2Compare> visionRanges;

	void Update(double dt);
	void Render();

};

