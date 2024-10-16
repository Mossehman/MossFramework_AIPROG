#pragma once
#include "Scene.h"

#include "Entity2D.h"
#include "Map2D.h"
class SceneAI : public Scene
{
public:
	virtual bool Init();
	virtual bool Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	std::vector<Entity2D> entityList;
	Camera camera;
	glm::vec2 mouseWorldPos;

};

