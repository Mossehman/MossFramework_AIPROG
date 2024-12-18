#pragma once
#include "Scene.h"

#include "Entity2D.h"
#include "Map2D.h"
#include "LevelIDs.h"

class SceneAI : public Scene
{
public:
	virtual bool Init();
	virtual bool Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	Map2D* newMap = Map2D::GetInstance();

	std::vector<Entity2D> entityList;
	Camera camera;
	glm::vec2 mouseWorldPos;

	GameObject* go; //TODO: delete for submission

};

