#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(double dt)
{
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