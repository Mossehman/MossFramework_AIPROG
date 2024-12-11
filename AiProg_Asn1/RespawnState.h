#pragma once
#include <FiniteState.h>
class LOLEntity;

class RespawnState : public FiniteState
{
protected:
	float respawnTime = 0.0f;
	LOLEntity* attachedEntity = nullptr;
public:

	virtual void OnEnter() override;
	virtual void Update(float dt) override;
};

