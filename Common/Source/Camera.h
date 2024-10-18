#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "glm/glm.hpp"

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;

	glm::vec2 minPos, maxPos;

	Camera();
	~Camera();
	virtual void Init(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif