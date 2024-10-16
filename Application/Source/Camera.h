#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "glm/glm.hpp"

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	glm::vec2 minPos, maxPos;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif