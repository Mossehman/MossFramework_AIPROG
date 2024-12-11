#pragma once
#include <FiniteState.h>

class LOLEntity;

class InitLaneState : public FiniteState
{
protected:
	float LaneCheckTimer = 0.0f;
	float MaxTimeWaitingTimer = 0.0f;

public:
	float LaneCheckTime = 0.5f;
	float MaxTimeBeforeAutoSelect = 1.0f;

	LOLEntity* attachedEntity = nullptr;

	virtual void OnEnter() override;
	virtual void Update(float dt) override;

};

