#pragma once
#include <EntityAI2D.h>

class TestEntity : public EntityAI2D
{
public:
	void MoveAlongPath(float dt) override;
	virtual int HandleMessage(BaseMessage* message) override;

};

