#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"
#include "MyMath.h"

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

void Camera::Reset()
{
	position = glm::vec3(1, 0, 0);
	target = glm::vec3(0, 0, 0);
	up = glm::vec3(0, 1, 0);
}

void Camera::Update(double dt)
{
	zoomVal = glm::clamp(zoomVal, 0.1f, 10.0f);

	if (!ToClamp) { return; }
	if (position.x < minPos.x)
	{
		position.x = minPos.x;
		target.x = minPos.x;
	}
	if (position.x > maxPos.x)
	{
		position.x = maxPos.x;
		target.x = maxPos.x;
	}

	if (position.y < minPos.y)
	{
		position.y = minPos.y;		
		target.y = minPos.y;
	}

	if (position.y > maxPos.y)
	{
		position.y = maxPos.y;
		target.y = maxPos.y;
	}
}