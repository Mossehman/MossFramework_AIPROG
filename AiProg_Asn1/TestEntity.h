#pragma once
#include <EntityAI2D.h>

class TestEntity : public EntityAI2D
{
public:
	void MoveAlongPath() override;
	virtual int HandleMessage(BaseMessage* message) override;

};

