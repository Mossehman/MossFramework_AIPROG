#ifndef SCENE_H
#define SCENE_H

#include "GameObject.h"
#include "Vector3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Camera.h"
#include "Vertex.h"
#include "RenderParameters.h"
#include "MouseController.h"

class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual bool Init() = 0;
	virtual bool Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};

#endif